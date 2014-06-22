#include "global.h"

void updateDecisions(Tank *tank,float baseRandom, float mytankRandom){

    int x = tank->x, y = tank->y,i,dx,dy,di,dj, currX, currY, empty, popX, popY,dir, fromX, fromY;
    float newRand = (float)rand()/(float)RAND_MAX;
    int mapMove[MAP_SIZE][MAP_SIZE], mapMoveLast[MAP_SIZE][MAP_SIZE];
    char mapAI[MAP_SIZE][MAP_SIZE];
    queue matrixQueue;
    //what map to use ??
    for(di=0; di<MAP_SIZE; di++) for (dj =0; dj<MAP_SIZE; dj++) mapAI[di][dj] = map[di][dj];

    //update player_1 to mapAI
    for (dx = 0; dx < 3; dx++)
        for (dy = 0; dy < 3; dy++) mapAI[player1.x + dx][player1.y + dy] = 'T';

    //update tanks to mapAI
    for (i = 0; i < MAX_SPRITES; i++) if (tanks[i].alive)
    {
        for (dx = 0; dx < 3; dx++)
            for (dy = 0; dy < 3; dy++) mapAI[tanks[i].x + dx][tanks[i].y + dy] = 'T';
    }

    //set empty for this tank
    for (dx = 0; dx < 3; dx++)
        for (dy = 0; dy < 3; dy++) mapAI[tank->x + dx][tank->y + dy] = EMPTY;



    matrixQueue.front = matrixQueue.end = 0;
    memset(mapMove,0,sizeof(mapMove));
    memset(mapMoveLast,0,sizeof(mapMove));


    //go towards base
    if (newRand < baseRandom + mytankRandom){

        if (newRand < baseRandom){
            //go toward base
            fromX = base_x;
            fromY = base_y;

        }
        else{
            //go toward ME
            fromX = player1.x+1;
            fromY = player1.y+1;

        }

        //generate mapShoot and init for mapMove (set ones from the spot that can shoot)
        for (dx = -1; dx < 2; dx++)
            for (dy = -1; dy < 2; dy++){
                if ( (dx+dy)%2 == 0) continue;

                currX = fromX;
                currY = fromY;

                empty=1;
                while(empty && currX < MAP_SIZE -2 && currX >= 0 && currY < MAP_SIZE -2 && currY >= 0){

                    for(di=0;di<3;di++) for(dj=0;dj<3;dj++){
                        if (mapAI[ currX + di ][ currY + dj ] != EMPTY ) empty = 0;
                    }

                    if (empty){
                        mapMove[currX][currY] = 1;
                        queuePush(&matrixQueue, currX*MAP_SIZE + currY);
                    }


                    empty =1;
                    for(di=0;di<3;di++) for(dj=0;dj<3;dj++){
                        if (mapAI[ currX + di ][ currY + dj ] == STEEL ) empty = 0;
                    }

                    currX += dx;
                    currY += dy;

                }


            }

            //now find path to player
            while(! queueEmpty( &matrixQueue )){

                //this is some retarded data representation in a queue (two ints in a int)
                popX = queuePop(&matrixQueue);
                popY = popX % MAP_SIZE;
                popX /= MAP_SIZE;

                if (popX ==0 && popY ==0){
                    popX =0 ;

                }

                for (dx = -1; dx < 2; dx++)
                    for (dy = -1; dy < 2; dy++){
                        if ( (dx+dy)%2 == 0) continue;

                        currX = popX + dx;
                        currY = popY + dy;

                        //off the map
                        if (currX >MAP_SIZE -2 || currY > MAP_SIZE -2 || currX < 0 || currY < 0) continue;

                        empty =1;
                        for(di=0;di<3;di++) for(dj=0;dj<3;dj++){
                            if (mapAI[ currX + di ][ currY + dj ] != EMPTY ) empty = 0;
                        }

                        if (empty && ( mapMove[ currX ][ currY ]==0 || mapMove[ currX ][ currY ] > mapMove[ popX ][ popY ] + 1) ){
                            mapMove[ currX ][ currY ] = mapMove[ popX ][ popY ] + 1;
                            queuePush(&matrixQueue, currX*MAP_SIZE + currY);

                            switch (dx){

                            case 0:
                                switch(dy){

                                case -1:
                                    mapMoveLast[ currX ][ currY ] = RIGHT;
                                    break;
                                case 1:
                                    mapMoveLast[ currX ][ currY ] = LEFT;
                                    break;

                                }
                                break;
                            case -1:
                                mapMoveLast[ currX ][ currY ] = DOWN;
                                break;

                            case 1:
                                mapMoveLast[ currX ][ currY ] = UP;
                                break;
                            }
                        }

                }

            }

            currX = tank->x;
            currY = tank->y;


            //you can go to the base
            if (mapMove[ currX ][ currY ]){

                i=AI_SPEED;

                while(i--){

                    if (mapMove[ currX ][ currY ] == 1 ){

                        if (currX == fromX){
                            if (currY > base_y){
                                dir = LEFT;
                            }
                            else{
                                dir = RIGHT;
                            }
                        }
                        else{
                            if (currX > fromY){
                                dir = UP;
                            }
                            else{
                                dir = DOWN;
                            }

                        }

                        //now check if the last decision was exactly this direction
                        if ( i== AI_SPEED-1 ){
                            if (tank->dir != dir){

                                tank->AIDecisions[ i-- ] = dir;

                            }

                        }
                        else{
                            if (tank->AIDecisions[ i+1 ] != dir){

                                tank->AIDecisions[ i-- ] = dir;
                            }

                        }

                        //now lets shoot !!!

                        while(i >=0 ) tank->AIDecisions[ i-- ] = 4;
                        return ;

                    }
                    else {
                        tank->AIDecisions[ i ] = mapMoveLast[ currX ][ currY ];
                        switch(mapMoveLast[currX][currY]){
                        case DOWN:
                            currX++;
                            break;
                        case UP:
                            currX--;
                            break;
                        case RIGHT:
                            currY++;
                            break;
                        case LEFT:
                            currY--;
                            break;
                        }
                    }


                }

                return ;
            }

    }

    //just do it random
    for( i=0; i<AI_SPEED; i++ ){
        tank->AIDecisions[i] = rand()%6;

    }


}

int get_decision_easy(Tank *tank){
    if (--(tank->AIState) < 0){

        updateDecisions(tank, BASE_EASY,MYTANK_EASY);

        tank->AIState = AI_SPEED-1;
    }

    return tank->AIDecisions[ tank->AIState ];
}

int get_decision_medium(Tank *tank){
    if (--(tank->AIState) < 0){

        updateDecisions(tank, BASE_MEDIUM,MYTANK_MEDIUM);

        tank->AIState = AI_SPEED-1;
    }

    return tank->AIDecisions[ tank->AIState ];
}

int get_decision_hard(Tank *tank){
    if (--(tank->AIState) < 0){

        updateDecisions(tank, BASE_HARD,MYTANK_HARD);

        tank->AIState = AI_SPEED-1;
    }

    return tank->AIDecisions[ tank->AIState ];
}

