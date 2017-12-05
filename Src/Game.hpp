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

namespace game{
    enum action_set {Nothing, Up, Down};
    typedef enum action_set Action_Set;
    
    class Game{
    private:
        float ball_x;
        float ball_y;
        float velocity_x;
        float velocity_y;
        float paddle_y;
        
        float paddle_height;
        
    public:
        Game();
        void reset();
        
        void run();
        bool bounce();
        
        void output_status();
        
        bool is_termination();
        
        void move_paddle(Action_Set a);
        
        Action_Set choose_action();
        
        int play_a_round();
        
    };
}

#endif /* Game_hpp */
