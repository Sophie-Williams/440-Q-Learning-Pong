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
    time_t t = clock();
    game::Game g;
    g.Q_init();
    for(int i = 0; i<300000; ++i)
        g.train_a_round();
    printf("Training time:%fs\n",(clock() - t)/(float)CLOCKS_PER_SEC);
    t=clock();
    float ave = 0;
    int Right_win_num = 0;
    int round = 1000;
    game::Player winner;
    for(int i = 0; i < round; ++i){
        int numthis = g.play_a_round(winner);
        printf("%d %s\n",numthis, (winner==game::Left)?"Left":"Right");
        ave += numthis;
        if(winner == game::Right)
            Right_win_num++;
    }
    ave /= round;
    printf("Testing time:%fs\n",(clock() - t)/(float)CLOCKS_PER_SEC);
    
    printf("Average rebound:%f\nRight winning rate:%f\n", ave,Right_win_num/(float)round);
    g.test();

    return 0;
}
