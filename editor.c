/**
 *  @file editor.c
 *  @brief - Implementacije funkcija za editor mape.
 */
 #include "global.h"

int highlightedTank, tankAnim;
Tank editorTanks[TANKS_PER_LEVEL];

void print_editor()
{
    int i,j,tank_x=-1,tank_y=-1,base_x=-1,base_y=-1;
     for (i=0;i<MAP_SIZE;i++)
        for(j=0;j<MAP_SIZE;j++)
            switch(editor[i][j])
            {
            case BASE: if(base_x==-1)base_x=i,base_y=j; break;
            case TANK: if(tank_x==-1)tank_x=i,tank_y=j; break;
            default: draw_field(i,j);break;
            }
    Tank temp;
    temp.x = tank_x;
    temp.y = tank_y;
    temp.dir = UP;
    temp.player = true;
    print_tank(&temp);
    print_base(MAP_OFFSET_X+base_x,MAP_OFFSET_Y+base_y);
}

void save_editor(char * mapFile)
{
   int i,j;
   FILE * output_file = fopen(mapFile, "w");
   erase_tank(36,12);
    for(i=0;i<MAP_SIZE;i++)
    {
        for(j=0;j<MAP_SIZE;j++) fprintf(output_file,"%c",editor[i][j]);
        fprintf(output_file,"\n");
    }
    for ( i = 0; i < TANKS_PER_LEVEL; i++ ) fprintf(output_file, "%d ", editorTanks[i].type );
    fclose(output_file);
}

void change_cursor()
{
    editor_cursor_id++;
    editor_cursor_id%=5;
}

void create_base(int x, int y)
{
    int i,j;
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
            editor[i+1+x][j+1+y]=BASE;
    for(j=0;j<5;j++)
        editor[x][j+y]=BRICK;
    for(i=0;i<4;i++)
        editor[i+x][y]=editor[i+x][y+4]=BRICK;
}

void create_tank(int x, int y)
{
    int i,j;
    for (i=0;i<3;i++)
        for (j=0;j<3;j++)
            editor[i+x][j+y]=TANK;
}

void erase_tank(int x, int y)
{
    int i,j;
    for (i=0;i<3;i++)
        for (j=0;j<3;j++)
            editor[i+x][j+y]=EMPTY;
}

void create_cursor(int x, int y)
{
    int i,j;
    for (i=0;i<=editor_cursor_size;i++)
        for(j=0;j<=editor_cursor_size;j++)
            editor[x+i][y+j]=characterMap[editor_cursor_id];
    create_base(35,17);
    create_tank(36,12);
}

void move_right()
{
    if (editor_cursor_y+editor_cursor_size<MAP_SIZE-1)
        editor_cursor_y++;
}

void move_left()
{
    if (editor_cursor_y>0) editor_cursor_y--;
}

void move_up()
{
    if (editor_cursor_x>0) editor_cursor_x--;
}

void move_down()
{
    if (editor_cursor_x+editor_cursor_size<MAP_SIZE-1)
        editor_cursor_x++;
}

void printEditorTanks(void)
{
    int i, j, currX = MAP_OFFSET_X, currY = MAP_OFFSET_Y + MAP_SIZE + 5;
    // tank indicators
    for ( i = 0; i < TANKS_PER_LEVEL / 2; i++ )
    {
        for ( j = 0; j < 2; j++ )
        {
            if ( i * 2 + j == highlightedTank && tankAnim >= 3 && tankAnim <= 6 ) attron ( A_BLINK );
            print_empty_tank ( currX, currY + j * 4 );
            editorTanks[i * 2 + j].dir = RIGHT;
            editorTanks[i * 2 + j].x = currX - MAP_OFFSET_X;
            editorTanks[i * 2 + j].y = currY + j * 4 - MAP_OFFSET_Y;
            print_tank ( &editorTanks[i * 2 + j] );
            if ( i * 2 + j == highlightedTank && tankAnim >= 3 && tankAnim <= 6 ) attroff ( A_BLINK );
        }
        currX += 4;
    }
}

void load_editor(int level)
{
    int i,j,c,get_me_out_of_here=0,iter;
    char level_name[20], buffer[20];
    strcpy(level_name, "level");
    //itoa(level, buffer, 10);
    sprintf(buffer,"%d",level);
    strcat(level_name, buffer);
    strcat(level_name, ".map");
    FILE * input_file = fopen(level_name, "r");
    clock_t prev,curr;
    print_border(MAP_OFFSET_X-1, MAP_OFFSET_Y -1, MAP_OFFSET_X + MAP_SIZE, MAP_OFFSET_Y + MAP_SIZE);
    for(i=0;i<MAP_SIZE;i++)
    {
        for(j=0;j<MAP_SIZE;j++) fscanf(input_file,"%c",&editor[i][j]);
        fgetc(input_file);
    }
    for ( i = 0; i < TANKS_PER_LEVEL; i++ ) fscanf(input_file,"%d",&editorTanks[i].type);
    fclose(input_file);
    editor_cursor_x=0;
    editor_cursor_y=0;
    editor_cursor_id=0;
    create_base(35,17);
    create_tank(36,12);
    print_editor();
    draw_cursor();
    highlightedTank = 0;
    tankAnim = 0;
    while(1)
    {
      tankAnim = ( tankAnim + 1 ) % 13;
      printEditorTanks ();
      if(kbhit())
      {
          while(kbhit()){
                    c = read_input();

            }
      switch(c)
      {
          get_me_out_of_here=0;
          case KEY_RIGHT: move_right(); break;
          case KEY_LEFT: move_left(); break;
          case KEY_UP: move_up(); break;
          case KEY_DOWN: move_down(); break;
          case 'Q':
          case 'q': change_cursor(); break;
          case 'W':
          case 'w': change_size(); break;
          case 'E':
          case 'e': create_cursor(editor_cursor_x,editor_cursor_y); break;
          case 'G':
          case 'g': map_generator(18); break;
          case 'n':
          case 'N': clear_editor(); create_base(35,17); create_tank(36,12); break;
          case 'j':
          case 'J':
              highlightedTank = ( highlightedTank - 1 + TANKS_PER_LEVEL ) % TANKS_PER_LEVEL;
            break;
          case 'k':
          case 'K':
              highlightedTank = ( highlightedTank + 1 ) % TANKS_PER_LEVEL;
            break;
          case 'l':
          case 'L':
              editorTanks[highlightedTank].type = ( editorTanks[highlightedTank].type + 1 ) % 4;
            break;
          case KEY_F(2): get_me_out_of_here=1; break;
          case KEY_F(12): get_me_out_of_here=2; break;
      }
      }
      if(get_me_out_of_here==1)
      {
          save_editor(level_name);
          break;
      }
      else if(get_me_out_of_here==2) break;
      print_editor();
      if ( tankAnim >= 3 && tankAnim <= 6 ) attron(A_BLINK);
      draw_cursor();
      if ( tankAnim >= 3 && tankAnim <= 6 ) attroff(A_BLINK);
      refresh();
      Sleep(50);
    }
}

void draw_cursor()
{
    int i,j;
    for (i=0;i<=editor_cursor_size;i++)
        for(j=0;j<=editor_cursor_size;j++)
    switch(editor_cursor_id)
    {
    case 0 :
            attron(COLOR_PAIR(3));
            print_empty(MAP_OFFSET_X+editor_cursor_x+i,MAP_OFFSET_Y+editor_cursor_y+j);
            attroff(COLOR_PAIR(3));
            break;
    case 1 : print_brick(MAP_OFFSET_X+editor_cursor_x+i,MAP_OFFSET_Y+editor_cursor_y+j); break;
    case 2 : print_steel(MAP_OFFSET_X+editor_cursor_x+i,MAP_OFFSET_Y+editor_cursor_y+j); break;
    case 3 : print_water(MAP_OFFSET_X+editor_cursor_x+i,MAP_OFFSET_Y+editor_cursor_y+j); break;
    case 4 : print_grass(MAP_OFFSET_X+editor_cursor_x+i,MAP_OFFSET_Y+editor_cursor_y+j); break;
    }
}



void draw_field(int x, int y)
{
    switch(editor[x][y])
    {
    case EMPTY : print_empty(MAP_OFFSET_X+x,MAP_OFFSET_Y+y); break;
    case BRICK : print_brick(MAP_OFFSET_X+x,MAP_OFFSET_Y+y); break;
    case STEEL : print_steel(MAP_OFFSET_X+x,MAP_OFFSET_Y+y); break;
    case WATER : print_water(MAP_OFFSET_X+x,MAP_OFFSET_Y+y); break;
    case GRASS : print_grass(MAP_OFFSET_X+x,MAP_OFFSET_Y+y); break;
    }
}

void change_size()
{
    editor_cursor_size++;
    editor_cursor_size%=3;
    if(editor_cursor_x+editor_cursor_size>=MAP_SIZE) editor_cursor_size=0;
    if(editor_cursor_y+editor_cursor_size>=MAP_SIZE) editor_cursor_size=0;
}

void clear_editor()
{
    int i,j;
    for (i=0;i<MAP_SIZE;i++)
        for(j=0;j<MAP_SIZE;j++) editor[i][j]=EMPTY;
}

void print_wall(int a, int b)
{
    int i,j,x,y,type;
    x=rand()%(MAP_SIZE-a);
    y=rand()%(MAP_SIZE-b);
    type= rand()%10;
    for (i=0;i<a;i++)
        for (j=0;j<b;j++)
    {
        switch(type)
        {
            case 2: editor[x+i][y+j]=WATER; break;
            case 3: editor[x+i][y+j]=STEEL; break;
            case 4: editor[x+i][y+j]=GRASS; break;
            default: editor[x+i][y+j]=BRICK; break;
        }
    }
}

void map_generator(int n)
{
    int i,a,b;
    clear_editor();
    for (i=0;i<n;i++)
    {
        a=rand()%10+6; b=rand()%2+2;
        if (rand()%3) print_wall(a,b);
        else print_wall(b,a);
    }
    create_base(35,17);
    create_tank(36,12);
}
