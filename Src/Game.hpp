//
//  Game.hpp
//  440-Q-Learning-Pong
//
//  Created by Liuyi Shi on 12/5/17.
//  Copyright © 2017 Liuyi Shi. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include <vector>

#define B_DISCRETE 12
#define P_DISCRETE 12

namespace game{
    enum action_set {Nothing = 0, Up, Down};
    typedef enum action_set Action_Set;
    
    enum player {Left = 0, Right};
    typedef enum player Player;
    
    class Game{
    private:
        //variable for the status of the game (ball position, ball velocity, paddle position)
        float ball_x;
        float ball_y;
        float velocity_x;
        float velocity_y;
        float paddle_y;
        float left_paddle_y;
        
        //the height of the paddle
        float paddle_height;
        
        //data structure to record every Q(s,a)
        std::vector<std::vector<float>> Q;
        //data structure to record every N(s,a), which is how many times (s,a) occurs
        std::vector<std::vector<unsigned int>> N;
        
    public:
        Game();
        //reset the game status to initial value, actually same as constructor
        void reset();
        //move the ball to next position according to velocity, but does not deal with bouce
        //ball_x += volecity_x, ball_y+=volecity_y
        void move_ball();
        //deal with bounce after moving the ball, i.e. if the ball passes the wall or passes the paddle, bounce it
        //if the ball did not pass the wall or paddle simply do nothing
        //return value is whether a rebound by the righter paddle happens
        bool bounce();
        //output current status of the varibales, for debug
        void output_status();
        //judge whether the termination condition happens, i.e. the ball passes the line of the paddle as is not rebounded back
        //parameter will save who is the winner
        bool is_termination(Player &p);
        //move the paddle according to player p and action a
        void move_paddle(Player p, Action_Set a);
        //hash current game status to a state# according to the discreting requirment in the instruction
        //board_size is how to divide the board, for example 12*12 then board_size=12;
        //paddle_discretion is how to divide the range of location of the paddle
        unsigned int get_state(unsigned int board_discretion, unsigned int paddle_discretion);
        //reture the total number of possible states, according to the discreting requirment in the instruction
        unsigned int get_state_size(unsigned int board_discretion, unsigned int paddle_discretion);
        //return the reward of current status, should run after move_ball() but before bounce()
        int get_reward(Action_Set a);
        
    //Q Learning part
        //initial Q and N datastructure
        void Q_init();
        //choose an action from current status, accoring to exploration policy
        Action_Set exploration(bool is_epsilon, float epsilon);
        //get the utility of a specific state, i.e.  MAXa {Q(s,a)}
        float get_utility(unsigned int state);
        //play the game for a round and train the bot
        void train_a_round();
        //just a method for debug
        void test();
        
    //Playing part
        //play the game for a round until termination
        //return how many times rebounded by Right player and parameter will save the winner
        int play_a_round(Player &p);
        //choose an action for player p
        //for Right player it will choose according to currently learned Q value
        //for Left player it will choose according to hard-coded policy (always trying to catch up the ball)
        Action_Set choose_action(Player p);
        
    };
}

#endif /* Game_hpp */
