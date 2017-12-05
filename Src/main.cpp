//
//  main.cpp
//  440-Q-Learning-Pong
//
//  Created by Liuyi Shi on 12/4/17.
//  Copyright © 2017 Liuyi Shi. All rights reserved.
//

#include <stdio.h>
#include "Game.hpp"
#include <random>

int main(){
    srand(clock());
    float ave = 0;
    int round = 1000;
    game::Game g;
    for(int i = 0; i < round; ++i){
        ave += g.play_a_round();
    }
    ave /= round;
    printf("Average:%f\n",ave);
    return 0;
}
