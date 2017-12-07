//
//  Game.cpp
//  440-Q-Learning-Pong
//
//  Created by Liuyi Shi on 12/5/17.
//  Copyright © 2017 Liuyi Shi. All rights reserved.
//

#include "Game.hpp"
#include <random>

game::Game::Game(){
    paddle_height = 0.2;
    
    ball_x = 0.5;
    ball_y = 0.5;
    velocity_x = 0.03;
    velocity_y = 0.01;
    paddle_y = 0.5 - paddle_height/2;
}

void game::Game::reset(){
    ball_x = 0.5;
    ball_y = 0.5;
    velocity_x = 0.03;
    velocity_y = 0.01;
    paddle_y = 0.5 - paddle_height/2;
}

void game::Game::move_ball(){
    ball_x += velocity_x;
    ball_y += velocity_y;
}
bool game::Game::bounce(){
    if(ball_y < 0){
        ball_y = -1 * ball_y;
        velocity_y = -1 * velocity_y;
    }
    else if(ball_y > 1){
        ball_y = 2 - ball_y;
        velocity_y = -1 * velocity_y;
    }
    if(ball_x < 0){
        ball_x = -1 * ball_x;
        velocity_x = -1 * velocity_x;
    }
    else if(ball_x > 1){
        //if the ball passes the line of the paddle but it's y value is within the range of the paddle, it can be rebounded
        if(ball_y >= paddle_y && ball_y <= paddle_y + paddle_height){
            ball_x = 2 - ball_x;
            velocity_x = -1 * velocity_x + (rand() % 3001 - 1500)/100000.0;
            if( abs(velocity_x) < 0.03 ){
                if( velocity_x != 0 )
                    velocity_x = 0.03 * (velocity_x/abs(velocity_x));
                else
                    velocity_x = (rand()%2 == 0)?0.03:(-0.03);
            }
            else if( abs(velocity_x) > 1 )
                velocity_x = 1 * (velocity_x/abs(velocity_x));
            
            velocity_y = velocity_y + (rand() % 6001 - 3000)/100000.0;
            if( abs(velocity_y) > 1 )
                velocity_y = 1 * (velocity_y/abs(velocity_y));
            //return true if the ball is rebounded
            return true;
        }
    }
    //otherwise return false, meaning the ball is not rebounded by the paddle
    //can may still bounced by the wall
    return false;
}

bool game::Game::is_termination(){
    if(ball_x > 1)
        return true;
    else
        return false;
}

void game::Game::output_status(){
    printf("b_x:%f b_y:%f v_x:%f v_y:%f p_y:%f\n", ball_x, ball_y, velocity_x, velocity_y, paddle_y);
}

void game::Game::move_paddle(Action_Set a){
    if(a == Up)
        paddle_y -= 0.04;
    else if(a == Down)
        paddle_y += 0.04;
    
    if( paddle_y<0 )
        paddle_y = 0;
    else if( paddle_y > 1-paddle_height )
        paddle_y = 1-paddle_height;

}

int game::Game::play_a_round(){
    int num = 0;
    while(1){
        move_paddle(choose_action());
        move_ball();
        if(bounce())
            num++;
        if(is_termination()){
            reset();
            return num;
        }
    }
    return 0;
}

unsigned int game::Game::get_state(unsigned int board_discretion, unsigned int paddle_discretion){
    unsigned int b_x_discrete;
    unsigned int b_y_discrete;
    int v_x_discrete;
    int v_y_discrete;
    unsigned int p_y_disctete;
    bool is_special_state;
    
    unsigned int state = 0;

    is_special_state = ball_x > 1;
    
    //discrete ball_x between 0 to board_discretion-1
    b_x_discrete = floor(ball_x*board_discretion);
    if(b_x_discrete == board_discretion)
        b_x_discrete = board_discretion-1;
    //discrete ball_y between 0 to board_discretion-1
    b_y_discrete = floor(ball_y*board_discretion);
    if(b_y_discrete == board_discretion)
        b_y_discrete = board_discretion-1;
    //discrete velocity_x between -1 and 1
    v_x_discrete = velocity_x/abs(velocity_x);
    //discrete velocity_y between -1,0,1
    if( abs(velocity_y) < 0.015 )
        v_y_discrete = 0;
    else
        v_y_discrete = velocity_y/abs(velocity_y);
    //discrete paddle_y between 0 to paddle_discretion-1
    p_y_disctete = floor(paddle_discretion*paddle_y/(1-paddle_height));
    if(p_y_disctete == paddle_discretion)
        p_y_disctete = paddle_discretion-1;
    if(is_special_state)
        state = 0;
    else{
        state = 0;
        state += (b_y_discrete*board_discretion + b_x_discrete) * 2 * 3 * paddle_discretion;
        state += p_y_disctete * 2 * 3;
        state += (v_y_discrete + 1) * 2;
        state += (v_x_discrete==-1)?0:1;
        state += 1;
    }
    return state;
}

unsigned int game::Game::get_state_size(unsigned int board_discretion, unsigned int paddle_discretion){
    return (board_discretion*board_discretion*paddle_discretion*2*3+1);
}

int game::Game::get_reward(Action_Set a){
    int reward;
    float padde_y_after_action = paddle_y;
    
    if(a == Up)
        padde_y_after_action -= 0.04;
    else if(a == Down)
        padde_y_after_action += 0.04;
    
    if( padde_y_after_action<0 )
        padde_y_after_action = 0;
    else if( padde_y_after_action > 1-paddle_height )
        padde_y_after_action = 1-paddle_height;
    
    if(ball_x + velocity_x > 1){
        if(ball_y + velocity_y >= padde_y_after_action && ball_y + velocity_y<= padde_y_after_action + paddle_height)
            reward = 1;
        else
            reward = -1;
    }
    else
        reward = 0;
    return reward;
}

void game::Game::Q_init(){
    //init Q, total size if action_number * state_size, each entry for Q(s,a)
    Q.resize(get_state_size(B_DISCRETE, P_DISCRETE));
    for(auto &i : Q)
        i.resize(3);
    N.resize(get_state_size(B_DISCRETE, P_DISCRETE));
    for(auto &i : N)
        i.resize(3);
}

//helper function for exploration method below
float f(float q,int n){
    if(n<100)
        return 3;
    else
        return q;
}

game::Action_Set game::Game::exploration(bool is_epsilon, float epsilon){
    Action_Set a = Nothing;
    if(is_epsilon){
        if( (rand()%100)/100.0 < epsilon){
            a = static_cast<Action_Set>(rand() % 3);
        }
        else{
            if( Q[get_state(B_DISCRETE, P_DISCRETE)][Up] > Q[get_state(B_DISCRETE, P_DISCRETE)][a])
                a = Up;
            if( Q[get_state(B_DISCRETE, P_DISCRETE)][Down] > Q[get_state(B_DISCRETE, P_DISCRETE)][a])
                a = Down;
            if( Q[get_state(B_DISCRETE, P_DISCRETE)][Up] == Q[get_state(B_DISCRETE, P_DISCRETE)][Down] && Q[get_state(B_DISCRETE, P_DISCRETE)][Down] == Q[get_state(B_DISCRETE, P_DISCRETE)][Nothing])
                a = static_cast<Action_Set>(rand()%3);
        }
    }
    else{
        if( f(Q[get_state(B_DISCRETE, P_DISCRETE)][Up],N[get_state(B_DISCRETE, P_DISCRETE)][Up]) > f(Q[get_state(B_DISCRETE, P_DISCRETE)][a],N[get_state(B_DISCRETE, P_DISCRETE)][a]))
            a = Up;
        if( f(Q[get_state(B_DISCRETE, P_DISCRETE)][Down],N[get_state(B_DISCRETE, P_DISCRETE)][Down]) > f(Q[get_state(B_DISCRETE, P_DISCRETE)][a],N[get_state(B_DISCRETE, P_DISCRETE)][a]))
            a = Down;
        if(f(Q[get_state(B_DISCRETE, P_DISCRETE)][Up],N[get_state(B_DISCRETE, P_DISCRETE)][Up]) == f(Q[get_state(B_DISCRETE, P_DISCRETE)][Down],N[get_state(B_DISCRETE, P_DISCRETE)][Down]) && f(Q[get_state(B_DISCRETE, P_DISCRETE)][Down],N[get_state(B_DISCRETE, P_DISCRETE)][Down]) == f(Q[get_state(B_DISCRETE, P_DISCRETE)][Nothing],N[get_state(B_DISCRETE, P_DISCRETE)][Nothing]) ){
            int r = rand()%3;
            a = static_cast<Action_Set>(r);
        }
    }
    return a;
}

game::Action_Set game::Game::choose_action(){
    Action_Set a = Nothing;
    if( Q[get_state(B_DISCRETE, P_DISCRETE)][Up] > Q[get_state(B_DISCRETE, P_DISCRETE)][a])
        a = Up;
    if( Q[get_state(B_DISCRETE, P_DISCRETE)][Down] > Q[get_state(B_DISCRETE, P_DISCRETE)][a])
        a = Down;
    if( Q[get_state(B_DISCRETE, P_DISCRETE)][Up] == Q[get_state(B_DISCRETE, P_DISCRETE)][Down] && Q[get_state(B_DISCRETE, P_DISCRETE)][Down] == Q[get_state(B_DISCRETE, P_DISCRETE)][Nothing])
        a = static_cast<Action_Set>(rand()%3);
    return a;
}

float game::Game::get_utility(unsigned int state){
    float u = -9999999;
    if(Q[state][Nothing] > u)
        u = Q[state][Nothing];
    if(Q[state][Up] > u)
        u = Q[state][Up];
    if(Q[state][Down] > u)
        u = Q[state][Down];
    return u;
}

void game::Game::train_a_round(){
    unsigned int C = 200;
    float alpha;
    float gamma = 0.9;
    
    unsigned int s_current;
    int R;
    Action_Set a = Nothing;
    unsigned int s_next;

    while(1){
        s_current = get_state(B_DISCRETE, P_DISCRETE);
        a = exploration(false,0.05);
        R = get_reward(a);
        alpha = (float)C/(C+N[s_current][a]);
        
        N[s_current][a]++;
        move_paddle(a);
        move_ball();
        bounce();
        s_next = get_state(B_DISCRETE, P_DISCRETE);
        
        Q[s_current][a] = Q[s_current][a] + alpha * (R + gamma * get_utility(s_next) - Q[s_current][a]);
        
        if(is_termination()){
            reset();
            break;
        }
    }
}

void game::Game::test(){
    float maxQ = -9999;
    for(auto &i:Q){
        for(auto &j:i){
            if(j > maxQ)
                maxQ = j;
        }
    }
    printf("%f\n",maxQ);
}
