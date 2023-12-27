#include "iGraphics.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define width 600
#define height 660
#define button_x 200
#define button_y 98
#define radius_c 80      // centre
#define radius_g 100     // goal
#define radius_d 50      // design
#define ball_radius 25   // ball size
#define player_radius 40 // player size
#define distance 60
#define pi 3.141592
#define True 1
#define False 0
#define first_sec 1
#define second_sec 2
#define third_sec 3
#define forth_sec 4
#define left 1
#define right 2
#define player_movement 15
#define zone_radius 80
#define velocity 15 // ball velocity feel free to increase or decrease if needed
#define max_velocity 25
#define min_velocity 9
#define rubber_velocity 3
// all needed functions
void reset(void);
void clearbox(char str[]);
void draw_pl();
void check_collision();
void print_classic_court();
void print_Picture_Courts(int a);
void let_it_go();
void save_goal(void);
void hit_ball(void);
void move_player(int way);
void locate(void);
void auto_play(void);
void change();
void write_file(char str[]);
void read_files();

struct coordinates
{
    float x;
    float y;
} ball, player1, player2, new_ball, random[5];

 int count_time;
int rubber = False, steel = True; // boundary

int goal_player_1 = False, goal_player_2 = False; // score checking

// taking name
char name_pl_1[100] = "Player 1", name_pl_2[100] = "Player 2";
int len = 0;
int mode_1 = False, mode_2 = False;

int play = False, solo = False, dual = False, music = True, menu_page = True;
int goal_page = False, settings_page = False, help_page = False, credit_page = False, secondary_menu = False, scorelog_page = False; // handling pages and actions
int mouse_play = False;                                                                                                              // player 1 control
int is_in_zone = False, did_locate = False, new_pos, moving = False, reached = False, chose_way = 0, red_alert = False;              // multi player variables(line 46-47)
float d_auto;                                                                                                                        // add a button for this feature
// declaring variable for initial state
float d1, d2;
float vx, vy, angle; // velocity components and angle of velocity;
// menu design
int ran_vel_x[5], ran_vel_y[5];
// picture variabless
char ball_pic[5][20] = {"pics/ball.bmp", "pics/ball2.bmp"};
char courts[10][20] = {"menu/court2.bmp", "menu/court3.bmp", "menu/court4.bmp", "menu/court5.bmp", "menu/court6.bmp"};
int court_number = 1;
char menu[30][40] = {"menu/background.bmp", "menu/button1.bmp", "menu/button2.bmp", "menu/button3.bmp",
                     "menu/button4.bmp", "menu/button5.bmp", "menu/button6.bmp", "menu/button7.bmp",
                     "menu/button8.bmp", "menu/button9.bmp", "menu/button10.bmp", "menu/button11.bmp",
                     "menu/button12.bmp", "menu/button13.bmp", "menu/button14.bmp", "menu/music_On.bmp",
                     "menu/music_Off.bmp", "menu/help.bmp", "menu/button15.bmp", "menu/name.bmp",
                     "menu/button16.bmp", "menu/button17.bmp","menu/button18.bmp"
                    };
char settings_button[5][30] = {"menu/mouse.bmp", "menu/not_mouse.bmp"};
char BG[20][50] = {"BG/settings bg.bmp", "BG/credit page.bmp", "BG/return.bmp", "BG/secondary menu2.bmp",
                   "BG/secondary menu_bg.bmp", "BG/help page.bmp", "BG/scorelog_page.bmp"
                  };
// sound variables
char BGM[20] = "black clover.wav";
char goal_sound[10][30] = {"sounds/goal.wav", "sounds/glass.wav", "sounds/rubber boundary.wav"};
void reset(void)
{
    player1.x = width / 2.0, player1.y = height / 4.0;
    player2.x = width / 2.0, player2.y = height * 3.0 / 4.0;
    ball.x = width / 2.0, ball.y = height / 2.0;
    vx = 0;
    vy = 0;
    angle = 0;
    is_in_zone = False;
    did_locate = False;
    reached = False;
    moving = False;
    red_alert = False;
}

void clearbox(char str[])
{
    int i = 0;
    while (str[i])
    {
        str[i] = '\0';
        i++;
    }
}
void draw_pl()
{
    iSetColor(0, 0, 0);
    iFilledCircle(ball.x, ball.y, ball_radius);
    if (court_number == 6)
        iShowBMP2(ball.x - 25, ball.y - 25, ball_pic[1], 0x00ffffff);
    else
        iShowBMP2(ball.x - 25, ball.y - 24, ball_pic[0], 0);
    iSetColor(226, 135, 67);
    iFilledCircle(player1.x, player1.y, player_radius); // player 1 position -- feel free to modify design
    iSetColor(255, 255, 255);
    iCircle(player1.x, player1.y, player_radius / 2.0);
    iSetColor(118, 181, 197);
    iFilledCircle(player2.x, player2.y, player_radius); // player 2 position -- feel free to modify design
    iSetColor(255, 255, 255);
    iCircle(player2.x, player2.y, player_radius / 2.0);
}

void check_collision()
{
    d1 = sqrt(((player1.x - ball.x) * (player1.x - ball.x)) + ((player1.y - ball.y) * (player1.y - ball.y)));
    d2 = sqrt(((player2.x - ball.x) * (player2.x - ball.x)) + ((player2.y - ball.y) * (player2.y - ball.y)));
    if (d1 < distance + 5)
    {
        angle = atan((ball.y - player1.y) / (ball.x - player1.x));
        if (ball.x < player1.x)
            angle += pi;
        vx = velocity * cos(angle) + vx;
        vy = velocity * sin(angle) + vy;
        if (solo)
        {
            locate();
        }
    }

    // collision with the player-2

    if (d2 < distance + 5)
    {
        angle = atan((ball.y - player2.y) / (ball.x - player2.x));
        if (ball.x < player2.x)
            angle += pi;
        vx = velocity * cos(angle) + vx;
        vy = velocity * sin(angle) + vy;
        if (solo && is_in_zone)
        {
            locate();
        }
    }
}

void print_classic_court()
{
    iSetColor(255, 255, 255); // colour of court
    iFilledRectangle(0, 0, width, height);
    iSetColor(0, 0, 0); // colour of primary lines
    iFilledRectangle(0, 0, width, 2.0);
    iFilledRectangle(0, 0, 2.0, height);
    iFilledRectangle(0, height - 2.0, width, 2.0);
    iFilledRectangle(width - 2.0, 0, 2.0, height);
    iFilledRectangle(0, height / 2.0, width, 2.0);
    iCircle(width / 2.0, height / 2.0, radius_c, 100);
    iCircle(width / 2.0, height / 2.0, radius_c + 0.5, 100);
    iCircle(width / 2.0, height / 2.0, radius_c + 1.0, 100);
    iCircle(width / 2.0, height / 2.0, radius_c + 1.5, 100);
    iCircle(width / 2.0, height / 2.0, radius_c + 2.0, 100);
    iCircle(width / 2.0, 0, radius_g, 100);
    // lower half circle around goal
    iCircle(width / 2.0, 0, radius_g + 0.5, 100);
    iCircle(width / 2.0, 0, radius_g + 1.0, 100);
    iCircle(width / 2.0, 0, radius_g + 1.5, 100);
    iCircle(width / 2.0, 0, radius_g + 2.0, 100);
    // upper half circle around gaol
    iCircle(width / 2.0, height, radius_g, 100);
    iCircle(width / 2.0, height, radius_g + 0.5, 100);
    iCircle(width / 2.0, height, radius_g + 1.0, 100);
    iCircle(width / 2.0, height, radius_g + 1.5, 100);
    iCircle(width / 2.0, height, radius_g + 2.0, 100);
    iSetColor(255, 0, 0); // colour of secondary lines
    iFilledRectangle(0, height / 4.0, width, 2.0);
    iFilledRectangle(0, height * 3 / 4.0, width, 2.0);
    iFilledCircle(width / 2.0, height / 2.0, 5, 100);
    iSetColor(0, 0, 255); // colour of secondary circles
    iCircle(width / 4.0, height / 4.0, radius_d, 100);
    iCircle(width * 3 / 4.0, height / 4.0, radius_d, 100);
    iCircle(width / 4.0, height * 3 / 4.0, radius_d, 100);
    iCircle(width * 3 / 4.0, height * 3 / 4.0, radius_d, 100);
    iSetColor(0, 255, 0);
    iFilledRectangle((width / 2.0 - radius_g), 0, radius_g * 2, 5);
    iFilledRectangle((width / 2.0 - radius_g), height - 5, radius_g * 2, 5);
    draw_pl();
}
void print_Picture_Courts(int a)
{

    iShowBMP(0, 0, courts[court_number - 2]);
    draw_pl();

    if (a == 5)
    {
        iShowBMP(0, 0, courts[court_number - 2]);
        iSetColor(0, 0, 0);
        iCircle(width / 2.0, 0, radius_g, 100);
        iCircle(width / 2.0, 0, radius_g + 0.5, 100);
        iCircle(width / 2.0, 0, radius_g + 1.0, 100);
        iCircle(width / 2.0, 0, radius_g + 1.5, 100);
        iCircle(width / 2.0, 0, radius_g + 2.0, 100);
        iCircle(width / 2.0, height, radius_g, 100);
        iCircle(width / 2.0, height, radius_g + 0.5, 100);
        iCircle(width / 2.0, height, radius_g + 1.0, 100);
        iCircle(width / 2.0, height, radius_g + 1.5, 100);
        iCircle(width / 2.0, height, radius_g + 2.0, 100);
        draw_pl();
        // iShowBMP2(ball.x - 25, ball.y - 24, court, 0);
    }
}
void iDraw()
{
    // place your drawing codes here
    // check_collision();
    iClear();
    if (play)
    {
        // playing page
        if (court_number == 1)
        {
            print_classic_court();
        }
        else
        {
            print_Picture_Courts(court_number);
        }
        iSetColor(0, 0, 0);
        iLine(0, height + 2, width, height + 2);
        // score
        iSetColor(255, 255, 255);
        iFilledRectangle(0, height, width, 100);
        iSetColor(0, 0, 0);
        iText(100, height + 80, "SCORE: ", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(200, height + 80, name_pl_1, GLUT_BITMAP_TIMES_ROMAN_24);
        iText(300, height + 80, " -- ", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(370, height + 80, name_pl_2, GLUT_BITMAP_TIMES_ROMAN_24);
        char goal_1_str[3], goal_2_str[3], score_str[100] = "";
        itoa(goal_player_1, goal_1_str, 10);
        strcat(score_str, goal_1_str);
        strcat(score_str, "     --    ");
        itoa(goal_player_2, goal_2_str, 10);
        strcat(score_str, goal_2_str);
        strcat(score_str, "    ");
        iText(265, height + 40, score_str, GLUT_BITMAP_TIMES_ROMAN_24);
        // music
        if (music)
        {
            iShowBMP(width - 70, height - 70 + 100, menu[15]);
        }
        if (!music)
        {
            iShowBMP(width - 70, height - 70 + 100, menu[16]);
        }
        if (mouse_play)
            iShowBMP2(0, height, settings_button[0], 0x00ffffff);
        if (!mouse_play)
            iShowBMP2(0, height, settings_button[1], 0x00ffffff);
    }
    if (menu_page)
    {
        // menu page
        iShowBMP(0, 0, menu[0]);
        iShowBMP2(width / 2.0 - 120, height / 2 + 250, menu[19], 0x00ffffff);
        iShowBMP2(width / 2 - 120, height / 2 + 50, menu[1], 0x00000000);
        iShowBMP2(width / 2 - 120, height / 2 - 30, menu[2], 0x00000000);
        iShowBMP2(width / 2 - 120, height / 2 - 110, menu[3], 0x00000000);
        iShowBMP2(width / 2 - 120, height / 2 - 190, menu[4], 0x00000000);
        iShowBMP2(width / 2 - 120, height / 2 - 270, menu[21], 0x00000000);
        iShowBMP2(width - 100, 0, menu[5], 0x00000000);
        if (music)
        {
            iShowBMP(width - 70, height - 70 + 100, menu[15]);
        }
        if (!music)
        {
            iShowBMP(width - 70, height - 70 + 100, menu[16]);
            PlaySound(0, 0, 0);
        }
        iSetColor(126, 79, 172);
        for (int i = 0; i < 5; i++)
        {
            iFilledCircle(random[i].x, random[i].y, 10, 100);
        }
    }
    if (goal_page)
    {
        iShowBMP(0, 0, "goal/goal_bg.bmp");
        iSetColor(20, 190, 209);
        //iFilledRectangle(width / 2 - 120, height / 2 + 100, 250, 50); // WINNER
        iShowBMP2(width / 2 - 140, height / 2 + 100,menu[22],0);
        iSetColor(255, 255, 255);
        if (goal_player_1 == 10)
        {
            iText(width / 2 - 105, height / 2 + 120, "WINNER : ", GLUT_BITMAP_TIMES_ROMAN_24);
            iText(width / 2 + 10, height / 2 + 120, name_pl_1, GLUT_BITMAP_TIMES_ROMAN_24);
        }
        if (goal_player_2 == 10)
        {
            iText(width / 2 - 105, height / 2 + 120, "WINNER : ", GLUT_BITMAP_TIMES_ROMAN_24);
            iText(width / 2 + 10, height / 2 + 120, name_pl_2, GLUT_BITMAP_TIMES_ROMAN_24);
        }
        iShowBMP2(width / 2 - 100, height / 2 - 100, menu[20], 0);
        iShowBMP(width - 75, 0, menu[4]);
    }
    if (settings_page)
    {
        iShowBMP(0, 0, BG[4]);
        iShowBMP(0, 0, BG[0]);
        iShowBMP2(0, 0, BG[2], 0x00000000);
        if (steel && (!rubber))
            iShowBMP2(width / 2 - 120, height / 2 + 130, menu[8], 0x00000000);
        if ((!steel) && rubber)
            iShowBMP2(width / 2 - 120, height / 2 + 130, menu[9], 0x00000000);
        if (court_number == 6)
            iShowBMP2(width / 2 - 120, height / 2 - 120, menu[18], 0x00000000);
        else
            iShowBMP2(width / 2 - 120, height / 2 - 120, menu[(court_number - 1) + 10], 0x00000000);
        if (mouse_play)
            iShowBMP2(0, height - 70, settings_button[0], 0x00ffffff);
        if (!mouse_play)
            iShowBMP2(0, height - 70, settings_button[1], 0x00ffffff);
    }
    if (help_page)
    {
        iShowBMP(0, 0, BG[5]);
        iShowBMP2(0, 0, BG[2], 0x00000000);
    }
    if (credit_page)
    {
        iShowBMP(0, 0, BG[1]);
        iShowBMP2(width / 2 - 250, height / 2 - 290, "BG/return.bmp", 0x00000000);
    }
    if (secondary_menu)
    {
        iShowBMP(0, 0, BG[4]);
        iShowBMP(0, 160, BG[3]);
        iShowBMP2(0, 0, BG[2], 0x00000000);
        iShowBMP2(width / 2 - 120, height / 2 - 30, menu[6], 0x00000000);
        iShowBMP2(width / 2 - 120, height / 2 - 190, menu[7], 0x00000000);
        // naming
        iSetColor(150, 150, 150);
        iFilledRectangle(width / 2.0 - 250, height / 2.0 + 150, 200, 30);
        iSetColor(150, 150, 150);
        iFilledRectangle(width / 2.0 + 50, height / 2.0 + 150, 200, 30);
        iSetColor(255, 255, 255);
        iText(width / 2.0 - 200, height / 2.0 + 150 + 10, name_pl_1);
        iText(width / 2.0 + 100, height / 2.0 + 150 + 10, name_pl_2);
        iSetColor(255, 255, 255);
        iText(170, height / 2.0 + 130, "Click on the box to enter name ");
        iText(100, height / 2.0 + 110, "and don't forget to hit 'enter' after you finish");
    }
    if (scorelog_page)
    {
        iShowBMP(0, 0, BG[6]);
        iShowBMP2(0, 0, BG[2], 0x00000000);
        read_files();
    }
}

void iMouseMove(int mx, int my)
{
    // place your codes here
    if (mouse_play && solo)
    {
        if (my > height / 2)
            my = height / 2 - 5;
        if (my < player_radius)
            my = player_radius + 5;
        if (mx >= width - player_radius)
            mx = width - player_radius - 5;
        if (mx < player_radius)
            mx = player_radius + 5;

        player1.x = mx;
        player1.y = my;
    }
}

void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // place your codes here
        if (menu_page)
        {
            // menu page controls

            if ((mx > width / 2 - 120) && (mx < width / 2 - 120 + button_x) && (my > height / 2 + 50) && (my < height / 2 + 50 + button_y))
            {
                secondary_menu = True;
                menu_page = False;
                goal_page = False;
                settings_page = False;
                goal_player_1 = False;
                goal_player_2 = False;
                reset();
            }

            if ((mx > (width - 70)) && (mx < ((width - 70 + 50))) && (my > (height - 70 + 100)) && (my < ((height - 70 + 50 + 100))))
            {
                music = !music;
                if(!music)
                {
                    PlaySound(0, 0, 0);
                }
                if(music)
                {
                    PlaySound(BGM, NULL, SND_ASYNC | SND_LOOP);
                }
            }
            if (mx > width - 80 && my < 200) // exit
                exit(0);
            if ((mx > width / 2 - 120) && (mx < width / 2 - 120 + button_x) && (my > height / 2 - 30) && (my < height / 2 - 30 + button_y)) // settings page
            {
                settings_page = True;
                menu_page = False;
            }
            if ((mx > width / 2 - 120) && (mx < width / 2 - 120 + button_x) && (my > height / 2 - 110) && (my < height / 2 - 110 + button_y))
            {
                help_page = True;
                menu_page = False;
            }
            if ((mx > width / 2 - 120) && (mx < width / 2 - 120 + button_x) && (my > height / 2 - 190) && (my < height / 2 - 190 + button_y))
            {
                credit_page = True;
                menu_page = False;
            }
            if ((mx > width / 2 - 120) && (mx < width / 2 - 120 + button_x) && (my > height / 2 - 270) && (my < height / 2 - 270 + button_y))
            {
                scorelog_page = True;
                menu_page = False;
            }
        }
        if (settings_page)
        {

            if ((mx > width / 2 - 120) && (mx < width / 2 - 120 + button_x) && (my > height / 2 + 130) && (my < height / 2 + 130 + button_y))
            {
                steel = !steel;
                rubber = !rubber;
            }
            if ((mx > width / 2 - 120) && (mx < width / 2 - 120 + button_x) && (my > height / 2 - 120) && (my < height / 2 - 120 + button_y))
            {
                court_number++;
                if (court_number > 6)
                    court_number = 1;
            }
            if (mx > 0 && mx < 100 && my > 0 && my < 100)
            {
                menu_page = True;
                settings_page = !settings_page;
            }
            if (mx > 0 && mx < 70 && my > height - 70 && my < height)
            {
                mouse_play = !mouse_play;
            }
        }
        if (help_page)
        {
            if (mx > 0 && mx < 100 && my > 0 && my < 100)
            {
                menu_page = True;
                help_page = !help_page;
            }
        }
        if (goal_page)
        {
            // goal page controls

            if ((mx > (width - 75)) && (my < 90))
                exit(0);
            if ((mx > (width / 2 - 100)) && mx < ((width / 2 - 100) + 200) && (my > (height / 2 - 100)) && my < ((height / 2 - 100 + 50)))
            {
                menu_page = True;
                goal_page = False;
            }
        }
        if (credit_page)
        {
            if ((mx > (width / 2 - 250)) && mx < ((width / 2 - 250) + 100) && (my > (height / 2 - 290)) && my < ((height / 2 - 290 + 100)))
            {
                menu_page = True;
                credit_page = !credit_page;
            }
        }
        if (scorelog_page)
        {

            if (mx > 0 && mx < 100 && my > 0 && my < 100)
            {
                menu_page = True;
                scorelog_page = !scorelog_page;
            }
        }
        if (secondary_menu)
        {
            if ((mx > width / 2 - 120) && (mx < width / 2 - 120 + button_x) && (my > height / 2 - 30) && (my < height / 2 - 30 + button_y))
            {
                count_time=0;
                solo = True;
                strcpy(name_pl_2, "BOT");
                dual = False;
                play = True;
                FILE *fptr = fopen("Score log.txt", "a");
                fprintf(fptr, "Game starts at");
                time_t tm;
                time(&tm);
                fprintf(fptr, " %s", ctime(&tm));
                fclose(fptr);
                secondary_menu = False;
            }
            if ((mx > width / 2 - 120) && (mx < width / 2 - 120 + button_x) && (my > height / 2 - 190) && (my < height / 2 - 190 + button_y))
            {
                count_time=0;
                dual = True;
                solo = False;
                play = True;
                FILE *fptr = fopen("Score log.txt", "a");
                fprintf(fptr, "Game starts at");
                time_t tm; // not a primitive datatype
                time(&tm);
                fprintf(fptr, " %s", ctime(&tm));
                fclose(fptr);
                secondary_menu = False;
            }
            if (mx > 0 && mx < 100 && my > 0 && my < 100)
            {
                menu_page = True;
                secondary_menu = !secondary_menu;
            }
            if (mx >= (width / 2.0 - 250) && mx <= ((width / 2.0 - 250) + 200) && my >= (height / 2.0 + 150) && my <= (height / 2.0 + 150 + 30) && mode_1 == False)
            {
                mode_1 = True;
                mode_2 = False;
                clearbox(name_pl_1);
            }
            if (mx >= (width / 2.0 + 50) && mx <= ((width / 2.0 + 50) + 200) && my >= (height / 2.0 + 150) && my <= (height / 2.0 + 150 + 30) && mode_2 == False)
            {
                mode_2 = True;
                mode_1 = False;
                clearbox(name_pl_2);
            }
        }

        if (play)
        {
            if (solo == True || dual == True)
            {
                if ((mx > (width - 70)) && (mx < ((width - 70 + 50))) && (my > (height - 70 + 100)) && (my < ((height - 70 + 50 + 100)))) // BGM
                    music = !music;
                if(!music)
                {
                    PlaySound(0, 0, 0);
                }
            }
        }
    }
}

void iKeyboard(unsigned char key)
{
    if (play)
    {
        if ((!mouse_play) || (dual))
        {

            if (key == 'q' || key == 'Q')
            {
                exit(0);
            }
            if (key == 'g' || key == 'G')
            {
                player1.x = player1.x - player_movement;
                if (player1.x <= player_radius)
                    player1.x = player_radius + 1;
            }

            if (key == 'j' || key == 'J')
            {
                player1.x = player1.x + player_movement;
                if (player1.x >= width - player_radius)
                    player1.x = width - player_radius - 1;
            }

            if (key == 'n' || key == 'N')
            {
                player1.y = player1.y - player_movement;
                if (player1.y <= player_radius)
                    player1.y = player_radius + 1;
            }

            if (key == 'y' || key == 'Y')
            {
                player1.y = player1.y + player_movement;
                if (player1.y >= height / 2.0 - player_radius)
                    player1.y = height / 2.0 - player_radius - 1;
            }
            if (key == 't' || key == 'T')
            {
                player1.x = player1.x - 10.6;
                player1.y = player1.y + 10.6;
                if (player1.x <= player_radius)
                    player1.x = player_radius + 1;
                if (player1.y >= height / 2.0 - player_radius)
                    player1.y = height / 2.0 - player_radius - 1;
            }
            if (key == 'u' || key == 'U')
            {
                player1.x = player1.x + 10.6;
                player1.y = player1.y + 10.6;
                if (player1.x >= width - player_radius)
                    player1.x = width - player_radius - 1;
                if (player1.y >= height / 2.0 - player_radius)
                    player1.y = height / 2.0 - player_radius - 1;
            }
            if (key == 'm' || key == 'M')
            {
                player1.x = player1.x + 10.6;
                player1.y = player1.y - 10.6;
                if (player1.x >= width - player_radius)
                    player1.x = width - player_radius - 1;
                if (player1.y <= player_radius)
                    player1.y = player_radius + 1;
            }
            if (key == 'b' || key == 'B')
            {
                player1.x = player1.x - 10.6;
                player1.y = player1.y - 10.6;
                if (player1.x >= width - player_radius)
                    player1.x = width - player_radius - 1;
                if (player1.y <= player_radius)
                    player1.y = player_radius + 1;
            }
        }

        if (key == '4')
        {
            player2.x = player2.x - player_movement;
            if (player2.x <= player_radius)
                player2.x = player_radius + 1;
        }

        if (key == '6')
        {
            player2.x = player2.x + player_movement;
            if (player2.x >= width - player_radius)
                player2.x = width - player_radius - 1;
        }

        if (key == '2')
        {
            player2.y = player2.y - player_movement;
            if (player2.y <= height / 2.0 + player_radius)
                player2.y = height / 2.0 + player_radius + 1;
        }

        if (key == '8')
        {
            player2.y = player2.y + player_movement;
            if (player2.y >= height - player_radius)
                player2.y = height - player_radius - 1;
        }
        if (key == '7')
        {
            player2.x = player2.x - 10.6;
            player2.y = player2.y + 10.6;
            if (player2.x <= player_radius)
                player2.x = player_radius + 1;
            if (player2.y >= height - player_radius)
                player2.y = height - player_radius - 1;
        }
        if (key == '9')
        {
            player2.x = player2.x + 10.6;
            player2.y = player2.y + 10.6;
            if (player2.x >= width - player_radius)
                player2.x = width - player_radius - 1;
            if (player2.y >= height - player_radius)
                player2.y = height - player_radius - 1;
        }
        if (key == '1')
        {
            player2.x = player2.x - 10.6;
            player2.y = player2.y - 10.6;
            if (player2.x <= player_radius)
                player2.x = player_radius + 1;
            if (player2.y <= height / 2.0 + player_radius)
                player2.y = height / 2.0 + player_radius + 1;
        }
        if (key == '3')
        {
            player2.x = player2.x + 10.6;
            player2.y = player2.y - 10.6;
            if (player2.x >= width - player_radius)
                player2.x = width - player_radius - 1;
            if (player2.y <= height / 2.0 + player_radius)
                player2.y = height / 2.0 + player_radius + 1;
        }
    }
    else
    {
        if (mode_1 == True)
        {
            if (key == '\r')
            {
                mode_1 = False;
                name_pl_1[len] = '\0';
                len = 0;
            }
            else
            {
                name_pl_1[len] = key;
                len++;
            }
        }
        if (mode_2 == True)
        {
            if (key == '\r')
            {
                mode_2 = False;
                name_pl_2[len] = '\0';
                len = 0;
            }
            else
            {
                name_pl_2[len] = key;
                len++;
            }
        }
    }
}

void iSpecialKeyboard(unsigned char key)
{

    if (key == GLUT_KEY_END)
    {
        exit(0);
    }
}

void let_it_go()
{
    if (ball.x > player2.x)
    {
        if (vx >= 0)
            return;
        else
        {
            float new_angle = atan((ball.y - player2.y) / (ball.x - player2.x));
            float temp_angle = atan((vy / vx));
            if (new_angle > temp_angle)
                iKeyboard('3');
            else if (new_angle < temp_angle)
                iKeyboard('7');
        }
    }
    else if (ball.x < player2.x)
    {
        if (vx <= 0)
            return;
        else
        {
            float new_angle = atan((ball.y - player2.y) / (ball.x - player2.x));
            float temp_angle = atan((vy / vx));
            if (new_angle < temp_angle)
                iKeyboard('1');
            else if (new_angle > temp_angle)
                iKeyboard('9');
        }
    }
    else
    {
        if (player2.x < width / 2.0)
            iKeyboard('6');
        if (player2.x >= width / 2.0)
            iKeyboard('4');
    }
}

/* this function is not used too much its only activated if ball is close to the goal then player2 will fastly move to upper and hit the ball from side */
void save_goal(void)
{
    if (player2.y > (height - radius_g / 2.0))
    {
        if (player2.x <= ball.x)
        {
            iKeyboard('6');
        }
        if (player2.x >= ball.x)
        {
            iKeyboard('4');
        }
    }
    else
    {
        if (player2.x <= ball.x)
        {
            iKeyboard('7');
            iKeyboard('4');
        }
        if (player2.x > ball.x)
        {
            iKeyboard('9');
            iKeyboard('6');
        }
    }
}
/* this function hits the ball is the ball is near to hit but this function is not always called. its called randomly when player2 choose to hit instead of block */
void hit_ball(void)
{
    float new_angle = angle = atan((ball.y - player2.y) / (ball.x - player2.x));
    if (new_ball.x < player2.x)
        new_angle += pi;
    new_angle = new_angle * 180.0 / pi;

    if (new_angle > 180 && new_angle <= 270)
    {
        if (new_angle >= 210 && new_angle <= 240)
        {
            iKeyboard('1');
        }
        else if (new_angle < 210)
        {
            iKeyboard('4');
        }
        else
            iKeyboard('2');
    }
    if (new_angle > 270)
    {
        if (new_angle >= 300 && new_angle <= 330)
        {
            iKeyboard('3');
        }
        else if (new_angle < 300)
        {
            iKeyboard('2');
        }
        else
            iKeyboard('6');
    }
}
/* this the main function of this automatic system. it moves the player2*/
void move_player(int way)
{
    float diff_x = abs(new_ball.x - player2.x);
    float diff_y = abs(new_ball.y - player2.y);
    if ((d_auto < zone_radius))
    {
        moving = False;
        reached = True;
        return;
    }

    if (new_pos == first_sec)
    {
        if (way == 1)
        {
            if (diff_x >= zone_radius)
            {
                iKeyboard('6');
            }
            else
            {
                iKeyboard('8');
            }
        }
        if (way == 2)
        {
            if (diff_y >= zone_radius)
            {
                iKeyboard('8');
            }
            else
            {
                iKeyboard('6');
            }
        }
        if (way == 3)
        {
            if (diff_x >= zone_radius && diff_y >= zone_radius)
            {
                iKeyboard('9');
            }
            else if (diff_x < zone_radius && diff_y >= zone_radius)
            {
                iKeyboard('8');
            }
            else if (diff_x >= zone_radius && diff_y < zone_radius)
            {
                iKeyboard('6');
            }
        }
    }
    if (new_pos == second_sec)
    {
        if (way == 1)
        {
            if (diff_x >= zone_radius)
            {
                iKeyboard('4');
            }
            else
            {
                iKeyboard('8');
            }
        }
        if (way == 2)
        {
            if (diff_y >= zone_radius)
            {
                iKeyboard('8');
            }
            else
            {
                iKeyboard('4');
            }
        }
        if (way == 3)
        {
            if (diff_x >= zone_radius && diff_y >= zone_radius)
            {
                iKeyboard('7');
            }
            else if (diff_x < zone_radius && diff_y >= zone_radius)
            {
                iKeyboard('8');
            }
            else if (diff_x >= zone_radius && diff_y < zone_radius)
            {
                iKeyboard('4');
            }
        }
    }
    if (new_pos == third_sec)
    {
        if (way == 1)
        {

            if (diff_x >= zone_radius)
            {
                iKeyboard('4');
            }
            else
            {
                iKeyboard('2');
            }
        }
        if (way == 2)
        {
            if (diff_y >= zone_radius)
            {
                iKeyboard('2');
            }
            else
            {
                iKeyboard('4');
            }
        }
        if (way == 3)
        {
            if (diff_x >= zone_radius && diff_y >= zone_radius)
            {
                iKeyboard('1');
            }
            else if (diff_x < zone_radius && diff_y >= zone_radius)
            {
                iKeyboard('2');
            }
            else if (diff_x >= zone_radius && diff_y < zone_radius)
            {
                iKeyboard('4');
            }
        }
    }
    if (new_pos == forth_sec)
    {

        if (way == 1)
        {
            if (diff_x >= zone_radius)
            {
                iKeyboard('6');
            }
            else
            {
                iKeyboard('2');
            }
        }
        if (way == 2)
        {
            if (diff_y >= zone_radius)
            {
                iKeyboard('2');
            }
            else
            {
                iKeyboard('6');
            }
        }
        if (way == 3)
        {
            if (diff_x >= zone_radius && diff_y >= zone_radius)
            {
                iKeyboard('3');
            }
            else if (diff_x < zone_radius && diff_y >= zone_radius)
            {
                iKeyboard('2');
            }
            else if (diff_x >= zone_radius && diff_y < zone_radius)
            {
                iKeyboard('6');
            }
        }
    }
}
void locate(void)
{
    srand(time(0));
    int steps = 10 + rand() % 5;
    new_ball.x = ball.x + vx * steps * 1.0;
    new_ball.y = ball.y + vy * steps * 1.0;
    if (new_ball.x > (width - ball_radius))
    {
        new_ball.x = (width - ball_radius) - (new_ball.x - (width - ball_radius));
    }
    if (new_ball.y > (height - ball_radius))
    {
        new_ball.y = (height - ball_radius) - (new_ball.y - (height - ball_radius));
    }
    if (new_ball.x < ball_radius)
    {
        new_ball.x = ball_radius + (ball_radius - new_ball.x);
    }
    if (new_ball.y < ball_radius)
    {
        new_ball.y = ball_radius + (ball_radius - new_ball.y);
    }
    if (new_ball.x > player2.x)
    {
        if (new_ball.y > player2.y)
        {
            new_pos = first_sec;
        }
        else
            new_pos = forth_sec;
    }
    else
    {
        if (new_ball.y > player2.x)
        {
            new_pos = second_sec;
        }
        else
            new_pos = third_sec;
    }
    did_locate = True;
    reached = False;
    moving = False;
}
/* this functions call all other functions for auto system.it first check it ball is in zone to hit then calculates the position to move to then call the move_player function and
 it also maintains the randomness */
void auto_play(void)
{
    if (ball.y > height / 2.0)
    {
        is_in_zone = True;
    }
    if (ball.y <= height / 2.0)
    {
        is_in_zone = False;
        did_locate = False;
        reached = False;
        moving = False;
    }

    if (is_in_zone)
    {
        if (!did_locate)
        {
            locate();
        }
        d_auto = sqrt(((player2.x - new_ball.x) * (player2.x - new_ball.x)) + ((player2.y - new_ball.y) * (player2.y - new_ball.y)));
        float distance_ball = sqrt(((ball.x - new_ball.x) * (ball.x - new_ball.x)) + ((ball.y - new_ball.y) * (ball.y - new_ball.y)));
        if (d2 < (distance_ball + player_radius) && (vy < 0) && (ball.y > (player2.y + player_radius)))
        {
            let_it_go();
            locate();
        }
        else
        {
            if (distance_ball <= (zone_radius))
            {
                if (reached)
                {

                    srand(time(0));
                    int hit = rand() % 11;
                    if (hit < 7)
                        hit_ball();
                    did_locate = False;
                    moving = False;
                }
                locate();
            }
            if (did_locate && (!reached) && (!red_alert))
            {
                if (!moving)
                {
                    srand(time(0));
                    chose_way = 1 + rand() % 3;
                    moving = True;
                }
                if (moving)
                {
                    move_player(chose_way);
                }
            }
            if (red_alert)
            {
                save_goal();
                locate();
            }
        }
    }
    if (!is_in_zone && (!reached) && (!red_alert))
    {
        d_auto = sqrt(((player2.x - new_ball.x) * (player2.x - new_ball.x)) + ((player2.y - new_ball.y) * (player2.y - new_ball.y)));
        new_ball.x = width / 2.0;
        new_ball.y = height - radius_g;
        d_auto = sqrt(((player2.x - new_ball.x) * (player2.x - new_ball.x)) + ((player2.y - new_ball.y) * (player2.y - new_ball.y)));
        if (new_ball.x > player2.x)
        {
            if (new_ball.y > player2.y)
            {
                new_pos = first_sec;
            }
            else
                new_pos = forth_sec;
        }
        else
        {
            if (new_ball.y > player2.x)
            {
                new_pos = second_sec;
            }
            else
                new_pos = third_sec;
        }
        if (!moving)
        {
            srand(time(NULL));
            chose_way = 1 + rand() % 3;
            moving = True;
        }
        if (moving && (!reached) && (!red_alert))
        {
            move_player(chose_way);
        }
    }
}

void write_file(char *str)
{
    float total_time=(10.0*count_time)/1000;
    FILE *fptr = fopen("Score log.txt", "a");
    fprintf(fptr, "WINNER: %s Score %d -- %d ", str, goal_player_1, goal_player_2);
    fprintf(fptr, "Duration: %.2f sec\n\n", total_time);
    fclose(fptr);
}

void read_files()
{
    char str[200];
    FILE *fptr = fopen("Score log.txt", "a+");
    int temp = 40;
    while (!feof(fptr))
    {
        iSetColor(0, 204, 204);
        fgets(str, 100, fptr);
        iText(40, height  - temp, str, GLUT_BITMAP_TIMES_ROMAN_24);
        temp += 30;
        if(temp>600) break;
    }
    fclose(fptr);
}

void change()
{
    if (menu_page)
    {
        for (int i = 0; i < 5; i++)
        {
            random[i].x += ran_vel_x[i];
            random[i].y += ran_vel_y[i];
        }
        for (int i = 0; i < 5; i++)
        {
            if (random[i].x >= width)
            {
                ran_vel_x[i] *= (-1);
            }
            if (random[i].x <= 0)
            {
                ran_vel_x[i] *= (-1);
            }
            if (random[i].y >= height)
            {
                ran_vel_y[i] *= (-1);
            }
            if (random[i].y <= 0)
            {
                ran_vel_y[i] *= (-1);
            }
        }
    }
    if (play)
    {
        // distance between and player
        count_time++;
        check_collision();
        if (solo && (!dual))
        {
            if ((ball.x < (width / 2.0 + radius_g)) && (ball.x > (width / 2.0 - radius_g)) && (ball.y > (height - (radius_g + ball_radius + 10))))
            {
                red_alert = True;
            }
            else
                red_alert = False;
            auto_play();
        }
        if ((ball.y >= (height - ball_radius - 5) || (ball.y <= ball_radius + 5)))
        {
            if ((ball.x < width / 2 - 100) || (ball.x > width / 2 + 100))
            {
                if ((!rubber) && (steel)) // playing as steel boundary
                {
                    // play collision sound here
                    PlaySound(goal_sound[1], NULL, SND_ASYNC);

                    vy = vy * (-1);
                }

                else if ((rubber) && (!steel)) // playing as rubber boundary for extra fun
                {

                    if (ball.y >= (height - ball_radius - 5))
                    {
                        // play collision sound here
                        PlaySound(goal_sound[2], NULL, SND_ASYNC);

                        vx = vx + rubber_velocity * cos(3.0 * pi / 2.0);
                        vy = vy + rubber_velocity * sin(3.0 * pi / 2.0);
                    }
                    else if (ball.y <= ball_radius + 5)
                    {
                        // play collision sound here
                        PlaySound(goal_sound[2], NULL, SND_ASYNC);

                        vx = vx + rubber_velocity * cos(pi / 2.0);
                        vy = vy + rubber_velocity * sin(pi / 2.0);
                    }
                }
                if (ball.y >= (height - ball_radius - 5))
                    ball.y = (height - ball_radius - 5);
                else if (ball.y <= ball_radius + 5)
                    ball.y = ball_radius + 5;
            }
        }

        if (ball.x >= (width - ball_radius - 5) || ball.x <= ball_radius + 5)
        {
            if ((!rubber) && (steel)) // playing as steel boundary
            {

                // play collision sound here
                PlaySound(goal_sound[1], NULL, SND_ASYNC);
                vx = (-1) * vx;
            }
            /*if rubber boundary seems too odd then simply comment out or remove next 17 lines and remove if statement 4 lines before */
            else if ((rubber) && (!steel)) // playing as rubber boundary for extra fun
            {
                if (ball.x >= (width - ball_radius - 5))
                {

                    // play collision sound here
                    PlaySound(goal_sound[2], NULL, SND_ASYNC);

                    vx = vx + rubber_velocity * cos(pi);
                    vy = vy + rubber_velocity * sin(pi);
                }
                else if (ball.x <= ball_radius + 5)
                {
                    // play collision sound here
                    PlaySound(goal_sound[2], NULL, SND_ASYNC);

                    vx = vx + rubber_velocity * cos(0);
                    vy = vy + rubber_velocity * sin(0);
                }
            }
            if (ball.x >= (width - ball_radius - 5))
                ball.x = width - ball_radius - 5;
            else if (ball.x <= ball_radius + 5)
                ball.x = ball_radius + 5;
        }
        // collision with the player-1

        // changing balls position according to velocity
        float vel = sqrt(vx * vx + vy * vy);
        if (vel > max_velocity)
        {
            vx /= 2;
            vy /= 2;
        }
        else if (vel < min_velocity)
        {
            vx *= 2;
            vy *= 2;
        }
        ball.x += vx;
        ball.y += vy;

        // goal checking
        if ((ball.y > height + ball_radius))
        {
            goal_player_1 += 1;
            PlaySound(goal_sound[0], NULL, SND_ASYNC);
            reset();
            if (goal_player_1 == 10) // player 1 wins
            {
                write_file(name_pl_1);
                goal_page = True;
                play = False;
                menu_page = False;
            }
        }
        if (ball.y < -(ball_radius))
        {
            goal_player_2 += 1;
            PlaySound(goal_sound[0], NULL, SND_ASYNC);
            reset();
            if (goal_player_2 == 10) // player 2 wins
            {
                write_file(name_pl_2);
                goal_page = True;
                play = False;
                menu_page = False;
                menu_page = False;
            }
        }
    }
}

int main()
{
    // place your own initialization codes here.

    for (int i = 0; i < 5; i++)
    {
        random[i].x = width / 2.0;
        random[i].y = 0;
    }
    for (int i = 0; i < 5; i++)
    {
        ran_vel_x[i] = 25 + i * 2;
        if (i % 2 == 0)
            ran_vel_x[i] *= (-1);
    }
    for (int i = 0; i < 5; i++)
    {
        srand(time(NULL));
        ran_vel_y[i] = 20 + i * 2;
    }
    iSetTimer(10, change);
    PlaySound(BGM, NULL, SND_ASYNC | SND_LOOP);
    iInitialize(width, height + 100, "AIR HOCKEY BY T&A");
    return 0;
}
/*
two mandatory things you need to do now
1-- add a mouse button to choose the control to mouse in single player mode
2-- add a single player and multi player button
*/
