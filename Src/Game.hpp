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

namespace game{
    enum action_set {Nothing = 0, Up, Down};
    typedef enum action_set Action_Set;
    
    class Game{
    private:
        float ball_x;
        float ball_y;
        float velocity_x;
        float velocity_y;
        float paddle_y;
        
        float paddle_height;
        
        std::vector<std::vector<float>> Q;
        std::vector<std::vector<unsigned int>> N;
        
    public:
        Game();
        void reset();
        
        void move_ball();
        
        //return value is whether a rebound by the paddle happens
        bool bounce();
        
        void output_status();
        
        bool is_termination();
        
        void move_paddle(Action_Set a);
        
        int play_a_round();
        
        //board_size is how to divide the board, for example 12*12 then board_size=12;
        //paddle_discretion is how to divide the location of the paddle
        unsigned int get_state(unsigned int board_discretion, unsigned int paddle_discretion);
        
        unsigned int get_state_size(unsigned int board_discretion, unsigned int paddle_discretion);
        
        int get_reward();
        
        //Q Learning part
        void Q_init();
        Action_Set exploration(float e);
        Action_Set choose_action();
        float get_utility(unsigned int state);
        
        void train_a_round();
        
        void test();
        
    };
}

#endif /* Game_hpp */
