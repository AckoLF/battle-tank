/**
 *  @file engine.c
 *  @brief - Implementacije funkcija za logiku igrice.
 */
 #include "global.h"

// moves the tank
void move_tank ( Tank *tank, int direction){

    //if this tank moved TANKSPEED earlier, just ignore. else, set move_state to 0
    if (tank->move_state < tank->move_speed ) return;

    tank->move_state = 0;

    switch(direction){
    case LEFT:
        go_left(tank);
        break;
    case RIGHT:
        go_right(tank);
        break;
    case UP:
        go_up(tank);
        break;
    case DOWN:
        go_down(tank);
        break;
    }
}

void go_up( Tank *tank )
{
    int up = tank->x - 1;
    int x = tank->x, y = tank->y;
    tank->dir = UP;
    if ( (up >=0 ) && ( mapUsed[ up ][ y ] == 0 ) && ( mapUsed[ up ][ y + 1 ] == 0 ) && ( mapUsed[ up ][ y + 2 ] == 0 ) ) tank->x = up;
}

void go_down( Tank *tank )
{
    int down = tank->x + 3;
    int x = tank->x, y = tank->y;
    tank->dir = DOWN;
    if ( (down < MAP_SIZE ) && ( mapUsed[ down ][ y ] == 0 ) && ( mapUsed[ down ][ y + 1 ] == 0 ) && ( mapUsed[ down ][ y + 2 ] == 0 ) ) tank->x = down - 2;
}
void go_left( Tank *tank )
{
    int left = tank->y - 1;
    int x = tank->x, y = tank->y;
    tank->dir = LEFT;
    if ( (left >= 0 ) && ( mapUsed[ x ][ left ] == 0 ) && ( mapUsed[ x + 1 ][ left ] == 0 ) && ( mapUsed[ x + 2 ][ left ] == 0 ) ) tank->y = left;
}
void go_right( Tank *tank )
{
    int right = tank->y + 3;
    int x = tank->x, y = tank->y;
    tank->dir = RIGHT;
    if ( (right < MAP_SIZE ) && ( mapUsed[ x ][ right ] == 0 ) && ( mapUsed[ x + 1 ][ right ] == 0 ) && ( mapUsed[ x + 2 ][ right ] == 0 ) ) tank->y = right - 2;
}
void shoot( Tank *tank ) // Spawns new bullet after shoot command
{
    int i, x, y, x2, y2;


    //if this tank shoot SHOOTSPEED earlier, just ignore.
    if (tank->shoot_state < tank->shoot_speed) return;

    //play a sound !
    if (tank == &player1) sound_shot();

    if (tank != &player1 || player1.stars < 2) {

        //get x, y of a new bullet
        switch( tank->dir ){
        case ( UP ):
            x = tank->x - 1;
            y = tank->y + 1;
            break;
        case ( RIGHT ):
            x = tank->x + 1;
            y = tank->y + 3;
            break;
        case ( LEFT ):
            x = tank->x + 1;
            y = tank->y - 1;
            break;
        case ( DOWN ):
            x = tank->x + 3;
            y = tank->y + 1;
            break;
        }

        //spawn a new bullet at first free spot in bullets array
        for ( i = 0; i < MAX_SPRITES; i++ )
            if ( bullets[i].alive == false )
            {
                bullets[i].alive = true;
                bullets[i].x = x;
                bullets[i].y = y;
                bullets[i].dir = tank->dir;
                bullets[i].state = 0;
                bullets[i].source = tank->player;

                break;
            }

    }

    else {

        //get x, y of a new bullet
        switch( tank->dir ){
        case ( UP ):
            x = tank->x - 1;
            y = tank->y;
            x2 = tank->x - 1;
            y2 = tank->y + 2;
            break;
        case ( RIGHT ):
            x = tank->x ;
            y = tank->y + 3;
            x2 = tank->x + 2;
            y2 = tank->y + 3;
            break;
        case ( LEFT ):
            x = tank->x ;
            y = tank->y - 1;
            x2 = tank->x + 2;
            y2 = tank->y - 1;
            break;
        case ( DOWN ):
            x = tank->x + 3;
            y = tank->y ;
            x2 = tank->x + 3;
            y2 = tank->y + 2;
            break;
        }

        //spawn a new bullet at first free spot in bullets array
        for ( i = 0; i < MAX_SPRITES; i++ )
            if ( bullets[i].alive == false )
            {
                bullets[i].alive = true;
                bullets[i].x = x;
                bullets[i].y = y;
                bullets[i].dir = tank->dir;
                bullets[i].state = 0;
                bullets[i].source = tank->player;

                break;
            }

        for ( i = 0; i < MAX_SPRITES; i++ )
            if ( bullets[i].alive == false )
            {
                bullets[i].alive = true;
                bullets[i].x = x2;
                bullets[i].y = y2;
                bullets[i].dir = tank->dir;
                bullets[i].state = 0;
                bullets[i].source = tank->player;

                break;
            }
    }

    //tank shouldnt fire to fast, and also shouldnt kill itself by moving to bullet position by accident :D
    tank->shoot_state = 0;
    tank->move_state = 0;

    //fire one bullet than two bullets than one bullet ...
    if (player1.stars == 3 || player1.stars == 2 || player1.stars == -2 || player1.stars == -3)
        player1.stars*=-1;

}

void base_set(char fieldType){
    int i,j;

    j= base_y -1;
    for (i= base_x -1;  i< base_x + 4; i++ )
        if (i >=0 && i < MAP_SIZE && j >= 0 && j< MAP_SIZE)
            map[ i ][ j ] = fieldType;

    j= base_y +3;
    for (i= base_x -1;  i< base_x + 4; i++ )
        if (i >=0 && i < MAP_SIZE && j >= 0 && j< MAP_SIZE)
            map[ i ][ j ] = fieldType;

    i= base_x -1;
    for (j= base_y -1;  j< base_y + 4; j++ )
        if (i >=0 && i < MAP_SIZE && j >= 0 && j< MAP_SIZE)
            map[ i ][ j ] = fieldType;

    i= base_x +3;
    for (j= base_y -1;  j< base_y + 4; j++ )
        if (i >=0 && i < MAP_SIZE && j >= 0 && j< MAP_SIZE)
            map[ i ][ j ] = fieldType;

}

void update_states() // Updating bullets states and moving them, and tank shooting states, and tank moving states
{
    int i,j;

    //update the tanks shoot state and move_state
    for(i=0; i < MAX_SPRITES ; i++){
        if (tanks[ i ].alive == true){

            if (tanks[ i ].move_state < tanks[i].move_speed){
                tanks[ i ].move_state += tanks[i].move_rate;
            }

            if (tanks[ i ].shoot_state < tanks[ i ].shoot_speed){
                tanks[ i ].shoot_state += tanks[i].shoot_rate;
            }

        }


    }
    //also for player_1
    if (player1.shoot_state < player1.shoot_speed) player1.shoot_state += player1.shoot_rate;
    if (player1.move_state < player1.move_speed) player1.move_state += player1.move_rate;



    //update power up state
    if (power_up.time) {
        power_up.time++;
        switch(player1.power_type){
        case TIMER:
            for(i=0; i < MAX_SPRITES ; i++)
                if (tanks[ i ].alive == true)
                    tanks[ i ].move_state = 0;
                    tanks[ i ].shoot_state =0 ;
        }

    }

    else {
        switch(player1.power_type){
        case SHOVEL:
            base_set(BRICK);
            break;
        }
        player1.power_type = NORMAL;
    }


    for ( i = 0; i < MAX_SPRITES; i++ )
        if ( bullets[ i ].alive == true )
        {
            bullets[ i ].state++;
            if ( bullets[ i ].state == BULLET_SPEED )
            {
                switch ( bullets[ i ].dir )
                {
                    case ( UP ):
                        bullets[ i ].x--;
                        break;
                    case ( DOWN ):
                        bullets[ i ].x++;
                        break;
                    case ( LEFT ):
                        bullets[ i ].y--;
                        break;
                    case ( RIGHT ):
                        bullets[ i ].y++;
                        break;
                }
                bullets[ i ].state = 0;
            }
        }
}

int bullet_can_collide(int x,int y){
    if (map[ x ][ y ] == BRICK
        || ( map[ x ][ y ] == STEEL && player1.stars == 3 )) return 1;
    return 0;
}

void collision() // Check for collisions of tanks and bullets, respectively bullets and bullets
{
    int i, j, di, dj;
    bool empty;
    //check for tank-bullet colisions
    for (i = 0; i < MAX_SPRITES; i++)
        for (j = 0; j < MAX_SPRITES; j++) if (tanks[i].alive && bullets[j].alive )
        {
            if ( bullets[j].source == 0 ) continue;
            bool hit = false;
            for( di = 0; di < 3; di++) for( dj = 0; dj < 3; dj++){

                if ( ( tanks[ i ].x + di == bullets[ j ].x ) && ( tanks[ i ].y + dj == bullets[ j ].y ) ) hit = true;
            }
            if ( hit )
            {
                tanks[i].hit_points--;
                bullets[ j ].alive = false;
                if (tanks[i].hit_points <= 0)
                {
                    tanks[i].alive = false;
                    if ( tanks[i].power_type != NORMAL )
                    {
                        power_up.type = tanks[i].power_type;
                        while ( 1 )
                        {
                            power_up.x = rand ( ) % ( MAP_SIZE - 3 );
                            power_up.y = rand ( ) % ( MAP_SIZE - 3 );
                            if (demo) empty = true;
                            else empty = false;
                            for(di=0;di<3;di++)
                                for(dj=0;dj<3;dj++)
                                {
                                    if (!demo) {
                                            if (map[ power_up.x + di ][ power_up.y + dj ] == EMPTY ) empty = true;
                                    }
                                    else{
                                            if (map[ power_up.x + di ][ power_up.y + dj ] != EMPTY ) empty = false;
                                    }
                                }
                            if(empty) break;
                        }
                    }
                    for ( di = 0; di < 3; di++ ) for ( dj = 0; dj < 3; dj++ )
                        if ( map[ tanks[i].x + di ][ tanks[i].y + dj ] == GRASS )
                        map[ tanks[i].x + di ][ tanks[i].y + dj ] = EXPLOSION_GRASS;
                        else if ( map[tanks[i].x + di][tanks[i].y + dj] == WATER )
                                  map[tanks[i].x + di][tanks[i].y + dj] = EXPLOSION_WATER;
                             else map[ tanks[i].x + di ][ tanks[i].y + dj ] = EXPLOSION;
                    sound_explosion();

                    score += tanks[i].value;
                    cntKilled++;
                    numberOfTanks--;

                    CNT_KILLED[tanks[i].type]++;
                }
            }
    }

    // tank powerUP collision
    if ( fabs ( power_up.x - player1.x ) <= 2 && fabs ( power_up.y - player1.y ) <= 2  && power_up.type !=NORMAL)
    {
        switch ( power_up.type )
        {
        case BOMB:
            for ( i = 0; i < MAX_SPRITES; i++ ) if (tanks[ i ].alive){
                cntKilled++;
                tanks[ i ].alive= 0;
            }
            sound_explosion();
            break;
        case HELMET:
            player1.invulnerable = FRAMES_PER_SEC * HELMET_SECS;
            break;
        case SHOVEL:
            power_up.time = - FRAMES_PER_SEC * SHOVEL_SECS;
            base_set(STEEL);
            break;
        case STAR:
            if (fabs(player1.stars) < 3) {
                    player1.stars++;
                    switch(player1.stars){
                    case 1:
                        player1.shoot_rate = 2;
                        break;
                    case -1:
                        player1.stars = 3;
                        break;
                    }
            }
            break;
        case LIFE:
            if (player1.hit_points<9) player1.hit_points++;
            break;
        case TIMER:
            power_up.time = - FRAMES_PER_SEC * TIMER_SECS;
            break;
        }

        score += 500;
        player1.power_type = power_up.type;
        power_up.type = NORMAL;
    }
    //check for bullet-bullet colisions
    for ( i = 0; i < MAX_SPRITES; i++ )
        for ( j = 0; j < MAX_SPRITES; j++ )
            if (bullets[ i ].alive && bullets[ j ].alive)
            if (  ( i != j ) && bullets_collision( bullets[ i ], bullets[ j ] ) )
            {
                if (bullets[i].source == 0 && bullets[j].source == 0) continue;
                if (bullets[i].source > 0 && bullets[j].source > 0) continue;
                bullets[ i ].alive = 0;
                bullets[ j ].alive = 0;
                if ( map[ bullets[ i ].x ][ bullets[ i ].y ] == WATER ) map[ bullets[ i ].x ][ bullets[ i ].y ] = EXPLOSION_WATER;
                else if ( map[ bullets[ i ].x ][ bullets[ i ].y ] == GRASS ) map[ bullets[ i ].x ][ bullets[ i ].y ] = EXPLOSION_GRASS;
                else map[ bullets[ i ].x ][ bullets[ i ].y ] = EXPLOSION;
            }


    //check for bullet-frame collisions
    for ( i = 0; i < MAX_SPRITES; i++ ) if (bullets[ i ].alive)
        if ( bullets[ i ].x < 0 || bullets[ i ].y < 0 || bullets[ i ].x >= MAP_SIZE || bullets[ i ].y >= MAP_SIZE  )
        {
            bullets[ i ].alive = 0;
        }

    //check for bullet-wall collisions
    for ( i = 0; i < MAX_SPRITES; i++ ) if (bullets[ i ].alive )
    {
        if ( bullet_can_collide(bullets[ i ].x, bullets[ i ].y ) )
        {
            map[ bullets[ i ].x ][ bullets[ i ].y ] = EXPLOSION;
            bullets[ i ].alive = 0;
            // should actually destroy 1x3 instead of 1x1
            switch (bullets[i].dir)
            {
            case UP:
            case DOWN:
                if ( bullet_can_collide( bullets[i].x ,bullets[i].y - 1) ) map[bullets[i].x][bullets[i].y - 1] = EXPLOSION;
                if ( bullet_can_collide( bullets[i].x ,bullets[i].y + 1) ) map[bullets[i].x][bullets[i].y + 1] = EXPLOSION;
                break;
            case LEFT:
            case RIGHT:
                if ( bullet_can_collide( bullets[i].x - 1 ,bullets[i].y) ) map[bullets[i].x - 1][bullets[i].y] = EXPLOSION;
                if ( bullet_can_collide( bullets[i].x + 1 ,bullets[i].y) ) map[bullets[i].x + 1][bullets[i].y] = EXPLOSION;
                break;
            }
        }
        if ( map[ bullets[ i ].x ][ bullets[ i ].y ] == STEEL ) bullets[ i ].alive = 0;
    }

    //check for bullet-base collisions
    for ( i = 0; i < MAX_SPRITES; i++ ) if (bullets[ i ].alive )
    {
        if (map[ bullets[ i ].x ][ bullets[ i ].y ] == BASE){
            //game end
            gameOver = 1;
        }
    }

    // check for bullet-player_1 collisions
    for (i = 0; i < MAX_SPRITES; i++)
    {
        if (bullets[i].alive == 0) continue;
        if (player1.x <= bullets[i].x && bullets[i].x <= player1.x + 2 && player1.y <= bullets[i].y && bullets[i].y <= player1.y + 2)
        {
            bullets[i].alive = 0;
            if ( player1.invulnerable == 0 )
            {
                player1.stars = 0;
                player1.shoot_rate =1 ;
                player1.hit_points--;
                player1.x = 36;
                player1.y = 12;
                player1.dir = UP;
                player1.invulnerable = FRAMES_PER_SEC * INVULNERABLE_SECS;
                sound_explosion();
            }
        }
    }
}

void spawn_tank( int x, int y, int dir, int type, int speed, int shootSpeed )
{
    numberOfTanks++;
    totalSpawned++;
    int i, j;
    for ( i = 0; i < MAX_SPRITES; i++ )
        if ( !tanks[ i ].alive )
        {
            tanks[i].alive = 1;
            tanks[i].dir = dir;
            tanks[i].x = x;
            tanks[i].y = y;
            tanks[i].move_state = 0;
            tanks[i].shoot_state = 0;
            tanks[i].AIState = 1;
            tanks[i].power_type = NORMAL;
            tanks[i].player = false;
            tanks[i].move_speed = speed;
            tanks[i].shoot_speed = shootSpeed;
            tanks[i].type = type;
            for ( j = 0; j < POWERS_PER_LEVEL; j++ )
            {
                if ( totalSpawned == power_indexes[j] )
                {
                    tanks[i].power_type = rand ( ) % ( NUMBER_OF_POWERS - 1 ) + 1;
                    power_up.type = NORMAL;
                }
            }
            switch ( type )
            {
            case BASIC_TANK:
                tanks[i].move_rate = 1;
                tanks[i].shoot_rate = 1;
                tanks[i].hit_points = 1;
                tanks[i].value = 100;
                break;
            case FAST_TANK:
                tanks[i].move_rate = 3;
                tanks[i].shoot_rate = 1;
                tanks[i].hit_points = 1;
                tanks[i].value = 200;
                break;
            case POWER_TANK:
                tanks[i].move_rate = 1;
                tanks[i].shoot_rate = 3;
                tanks[i].hit_points = 1;
                tanks[i].value = 300;
                break;
            case ARMOR_TANK:
                tanks[i].move_rate = 1;
                tanks[i].shoot_rate = 1;
                tanks[i].hit_points = 4;
                tanks[i].value = 400;
                break;
            }
            break;
        }
}

bool bullets_collision( Bullet bullet1, Bullet bullet2 )
{
    if ( bullet1.x == bullet2.x     && bullet1.y == bullet2.y                                                     ) return true;
    if ( bullet1.x == bullet2.x + 1 && bullet1.y == bullet2.y     && bullet1.dir == DOWN  && bullet2.dir == UP    ) return true;
    if ( bullet1.x == bullet2.x - 1 && bullet1.y == bullet2.y     && bullet1.dir == UP    && bullet2.dir == DOWN  ) return true;
    if ( bullet1.x == bullet2.x     && bullet1.y == bullet2.y + 1 && bullet1.dir == RIGHT && bullet2.dir == LEFT  ) return true;
    if ( bullet1.x == bullet2.x     && bullet1.y == bullet2.y - 1 && bullet1.dir == LEFT  && bullet2.dir == RIGHT ) return true;
    return false;
}


