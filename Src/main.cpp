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
    srand((unsigned int)clock());

    game::Game g;
    g.Q_init();
    for(int i = 0; i<100000; ++i)
        g.train_a_round();
    
    float ave = 0;
    int round = 1000;
    for(int i = 0; i < round; ++i){
        int numthis = g.play_a_round();
        printf("%d\n",numthis);
        ave += numthis;
    }
    ave /= round;
    printf("Average:%f\n",ave);
    
    g.test();

    return 0;
}
