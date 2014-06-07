#include "global.h"

void choose_level(char * level_name){
    //this is for choosing level and difficulty, not done yet.

    load_map(level_name);
    gameDifficulty = 2;
}

int get_decision_easy(Tank *tank){
    //0,1,2,3 are directions, 4 is shoot, and 5 is nothing
    return rand()%6;
}

int get_decision_medium(Tank *tank){

}

int get_decision_hard(Tank *tank){
}

void start_game(){

    time_t current_time;
    char* c_time_string;
    /* Obtain current time as seconds elapsed since the Epoch. */
    current_time = time(NULL);
    /* Convert to local time format. */
    c_time_string = ctime(&current_time);

    int i,keyPressed,enemySpawn,x,y,enemyChoice,di,dj;
    const int sleepTime = 1000 / FRAMES_PER_SEC;

    //init some elements
    gameOver = 0;
    for (i=0; i<MAXSPRITES ; i++){
        tanks[i].val = bullets[i].val = 0;
    }
    enemySpawn = 0;


    while(!gameOver){

        /* Obtain current time as seconds elapsed since the Epoch. */
        current_time = time(NULL);
        /* Convert to local time format. */
        c_time_string = ctime(&current_time);



        //check if we should spawn an enemy tank
        enemySpawn++;
        if (1) {
                move(18,50);
                printw("enemyspawn %d",enemySpawn);
        }
        if (enemySpawn == confDiff[ gameDifficulty ].spawn){
            enemySpawn = 0;
            //find me a spot for new enemy tank
            find_space_tank(&x,&y);

            //if valid
            if (x!=-1){
                spawn_tank(x,y,DOWN);
                if (1) {
                        move(19,50);
                        printw("spawned tank at %d %d",x,y);
                }

                //we should also update the mapUsed
                for(di=0;di<3;di++) for(dj=0;dj<3;dj++){
                    mapUsed[x+di][y+dj]=1;
                }
            }

        }

        //move the tanks !
        for ( i = 0; i < MAXSPRITES; i++ ) if (tanks[i].val) {
            enemyChoice = confDiff[gameDifficulty].AI(tanks + i);
            if (enemyChoice >=0 && enemyChoice <=3) move_tank(tanks+i, enemyChoice);
            else if (enemyChoice == 4) shoot(tanks+i);
        }

        if (DEBUG) {
            move(2,50);
            printw("%s",c_time_string);
        }

        //if keyboard is hit update the main tank position or shoot
        if (kbhit()){

            //get the last pressed key
            while(kbhit()){
                    keyPressed = getch();
            }

            if (DEBUG) {
                    move(3,50);
                    printw("not pressed");
            }
            switch(keyPressed){

                case KEY_RIGHT:
                case KEY_LEFT:
                case KEY_UP:
                case KEY_DOWN:
                    move_tank(&myTank, keyPressed - KEY_DOWN);
                    break;
                case SPACE:
                    shoot(&myTank);
                    break;
            }

            if (DEBUG) {
                move(10,50);
                printw("pressed %d, but keyright is %d, and keyright == pressed %d",keyPressed,KEY_RIGHT,keyPressed==KEY_RIGHT);
                refresh();
            }
        }

        update_states();
        collision();

        print_map();
        Sleep(sleepTime);

    }

}
