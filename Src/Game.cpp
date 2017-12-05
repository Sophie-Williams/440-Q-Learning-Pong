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

void game::Game::run(){
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
        if(ball_y >= paddle_y && ball_y <= paddle_y + paddle_height){
            ball_x = 2 - ball_x;
            velocity_x = -1 * velocity_x + (rand() % 3001 - 1500)/100000.0;
            if( abs(velocity_x) < 0.03 ){
                if( velocity_x != 0 )
                    velocity_x = 0.03 * (velocity_x/abs(velocity_x));
                else
                    velocity_x = 0.03;
            }
            velocity_y = velocity_y + (rand() % 6001 - 3000)/100000.0;
            return true;
        }
    }
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

game::Action_Set game::Game::choose_action(){
    if(paddle_y+paddle_height/2 > ball_y)
        return Up;
    else if(paddle_y+paddle_height/2 < ball_y)
        return Down;
    else
        return  Nothing;
}

int game::Game::play_a_round(){
    int num = 0;
    while(1){
        move_paddle(choose_action());
        run();
        if(bounce())
            num++;
        if(is_termination()){
            reset();
            return num;
        }
    }
    return 0;
}
