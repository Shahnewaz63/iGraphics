#include "iGraphics.h"
#include "iSound.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <string.h>
//anik farreed
/*
function iDraw() is called again and again by the system.

*/
#define WIDTH 1000
#define HEIGHT 800
#define ANIM_DELAY 100
#define FASTEN_DELAY 10*1000
#define OBS_COUNT 3

Image play_button;
Image score_button;
Image img;
Image title;
Image frames[6];
Image frames_obstacle[3];
Image frames_obstacle2[3];
Image frames_obstacle3[3];
Sprite sprite;
Sprite obs[OBS_COUNT];
Image gameover;
Image restart_button;
Image input_name;
Image leaderboard_bg;
Image close_button;
Image home_button;
Image levels_button;
Image levels;
FILE *leaderboard;


int channel;
int sound_checker = 0;
int bird_y = 430;
int bird_x = 470;
int fall_timer = 1;
int fall_rate = 1;
int increase_speed = 5;
int game_status = 1;
int start_checker = 0;
int scoreboard = 0;
int is_name = 0;
int name_len = 0;
int player_count = 0;
int is_played = 0;
int is_home = 1;
int scroll_down = 0;
int start_idx = 0;
int end_idx = 14;
char player_list[100][30];
char score_list[100][10];
int is_leaderboard = 0;
int scroll_up = 0;
int level_checker = 0;
int selected_level = 1;
int is_scored[OBS_COUNT];

char name[30];
char score[10];

int obstacle_position_x[OBS_COUNT];
int obstacle_position_y[OBS_COUNT];
//void animateLogo();
void restart_game();
void gameStarter();
void birdFall();
void gameOver();
void showObs();
void setObs(int c);
void loadPlayButton();
void showScoreboard();
void iniObsPos();
void showLeaderboard();
void createLeaderboard();
void saveScore();
void inputName();
void select_level_button();
int randHeight();
void select_level(int x);
void change_background();
void showBird();

void iDraw()
{
    // place your drawing codes here
    
    if(game_status) {
        iShowLoadedImage(0, 0, &img);
        iWrapImage(&img, -5);
        inputName();
        showLeaderboard();
        showBird();
        showObs();
        loadPlayButton();
        select_level_button();
        if (sound_checker ==0) {
            iPlaySound("assets/sounds/level.wav", true, 30);
            sound_checker = 1;
        }
    }
    
    else {
        iShowLoadedImage(280, 350, &gameover);
        iShowLoadedImage(550, 250, &restart_button);
        iShowLoadedImage(350, 250, &home_button);
    }
}

void change_background() {
    
    if (selected_level == 1) {
        increase_speed = 5;
        iFreeImage(&img);
        iLoadImage(&img, "assets/images/1951.png");
        iStopSound(channel);
        channel = iPlaySound("assets/sounds/level.wav", true, 30);
    }
    else if (selected_level == 2) {
        increase_speed = 5 ;
        iFreeImage(&img);
        iLoadImage(&img, "assets/images/1952.png");
        iStopAllSounds();
        channel = iPlaySound("assets/sounds/levell.wav", true, 30);

    }
    else if (selected_level == 3) {
        increase_speed = 10;
        iFreeImage(&img);
        iLoadImage(&img, "assets/images/1953.png");
        iStopSound(channel);
        channel = iPlaySound("assets/sounds/levelll.wav", true, 30);
    }
    iResizeImage(&img, 1800, 1000);
}

void select_level_button() {
    if (level_checker ==1) {
        iShowLoadedImage(200,140, &levels);
    }
}

void select_level(int x) {
    if (level_checker==1) {
        selected_level = x;
        level_checker = 0;
    }
}

void showBird() {
    if(!is_leaderboard && game_status) {
        iShowSprite(&sprite);
    }
}

void restart_game() {
    for(int i = 0; i<OBS_COUNT; i++) {
        is_scored[i] = 0;
    }
    increase_speed = 5;
    if(selected_level == 3) {
        increase_speed = 10;
    }
    game_status = 1;
    fall_timer = 1;
    bird_y = 430;
    iSetSpritePosition(&sprite, bird_x, bird_y);
    scoreboard = 0;
}

void setObs(int c) {
    obstacle_position_x[c] = 1200;
    obstacle_position_y[c] = randHeight();
    
    iSetSpritePosition(&obs[c], obstacle_position_x[c], obstacle_position_y[c]);
}

void showObs() {
    if (start_checker == 1 && game_status ==1) {
        for(int j = 0; j<OBS_COUNT; j++) {
            if (selected_level == 3 && obstacle_position_x[j] < -250) {
                setObs(j);
                is_scored[j] = 0;
            }
            if(obstacle_position_x[j] < -120 && (selected_level == 1 || selected_level == 2)) {
                setObs(j);
                is_scored[j] = 0;
            }
            
        else {
            obstacle_position_x[j] -= increase_speed;
            iSetSpritePosition(&obs[j], obstacle_position_x[j], obstacle_position_y[j]);
        }
            iShowSprite(&obs[j]);

        if(iCheckCollision(&sprite, &obs[j]) || bird_y >1100 || bird_y < -100) {
            gameOver();
            start_checker = 0;
            game_status = 0;
        }
        if(obstacle_position_x[j] > 430 && obstacle_position_x[j] < 455 && !is_scored[j]) {
            iPlaySound("assets/sounds/scoreplus.wav", false, 15);
            scoreboard++;
            is_scored[j] = 1;
        }
            }
        }
}

void inputName() {
    if(is_name) {
        iShowLoadedImage(350, 550, &input_name);
        iText(430, 580, "NAME:", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(515, 580, name, GLUT_BITMAP_TIMES_ROMAN_24);
    }
}

void saveScore() {
    leaderboard = fopen("saves/scoreboard.txt", "a+");
    fprintf(leaderboard, "%s %d\n", name, scoreboard);
    fclose(leaderboard);
}

void exchangeRank(char score1[], char score2[]) {
    char temp[30];
    strcpy(temp, score1);
    strcpy(score1, score2);
    strcpy(score2, temp);
}

void sortPlayers() {
    for(int i = 0; i<player_count; i++) {
        for(int j = i+1; j<player_count; j++) {
            if(atoi(score_list[i]) < atoi(score_list[j])) {
                exchangeRank(score_list[i], score_list[j]);
                exchangeRank(player_list[i], player_list[j]);
            }
        }
    }
}

void createLeaderboard() {
    player_count = 0;
    leaderboard = fopen("saves/scoreboard.txt", "a+");
    int player_score;
    char player_name[50];
    char line[100];
    
    if(leaderboard == NULL) {
        printf("Could not open leaderboard file.\n");
    }
    else {
        while(fgets(line, sizeof(line), leaderboard)) {
            line[strcspn(line, "\n")] = '\0';
            
            if (sscanf(line + strlen(line) - 2, "%d", &player_score) == 1) {
                line[strlen(line) - 2] = '\0';
                strcpy(player_name, line);
                sprintf(player_list[player_count], "%s", player_name);
                sprintf(score_list[player_count], "%d", player_score);
                player_count++;
            } 
            else {
                printf("Error reading player data: %s\n", line);
            }
        }
        sortPlayers();
    }
}


void showLeaderboard() {
    if(is_leaderboard) {
        iShowLoadedImage(75, 140, &leaderboard_bg);
        iShowLoadedImage(120, 180, &close_button);
        char temp_count[20];
        if(scroll_down && end_idx<player_count-1) {
            start_idx++;
            end_idx++;
            scroll_down = 0;
        }
        if(scroll_up && start_idx>0) {
            end_idx--;
            start_idx--;
            scroll_up = 0;
        }
        for(int i = 0; i<player_count; i++) {
            if(i>=start_idx && i<=end_idx) {
                sprintf(temp_count, "%d.", i+1);
                iText(235, 600-(i-start_idx)*30, temp_count, GLUT_BITMAP_TIMES_ROMAN_24);
                iText(265, 600-(i-start_idx)*30, player_list[i], GLUT_BITMAP_TIMES_ROMAN_24);
                iText(650, 600-(i-start_idx)*30, score_list[i], GLUT_BITMAP_TIMES_ROMAN_24);
            }
        }
    }
}


void showScoreboard() {
    sprintf(score, "%d", scoreboard);
    iText(470, 730, "SCORE: ", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(560, 730, score, GLUT_BITMAP_TIMES_ROMAN_24);
}

void iniObsPos() {
    is_home = 0;
    is_played = 1;
    for(int i = 0; i<OBS_COUNT; i++) {
        obstacle_position_x[i] = 1100 + i*420;
        obstacle_position_y[i] = randHeight();
    }
}

void gameOver() {
    if(game_status) {
        iPlaySound("assets/sounds/hit.wav", false, 100);
        saveScore();
        game_status = 0;
    }
}

int randHeight() {
    int lower = -150;
    int upper = 150;
    int randomNumber = lower + rand() % (upper - lower + 1);

    std::cout << "Random number: " << randomNumber << std::endl;

    return -200 + randomNumber;
}

void birdFall () {
    if (game_status == 1 && start_checker == 1) {
        
        fall_timer+=fall_rate;
        bird_y -= fall_timer*fall_timer*0.5;
        iSetSpritePosition(&sprite, bird_x, bird_y);
    }
}

void fastenBird() {
    if (game_status && !is_home && !is_name && start_checker) {
    increase_speed += 1;
    }
}

void setObsFrame(int level) {
    if (level==1) {
        for(int i = 0; i<OBS_COUNT; i++) {
        iChangeSpriteFrames(&obs[i], frames_obstacle, 1);
        iScaleSprite(&obs[i], 0.6);
        }
    }
    else if (level == 2) {
        for(int i = 0; i<OBS_COUNT; i++) {
        iChangeSpriteFrames(&obs[i], frames_obstacle2, 1);
        iScaleSprite(&obs[i], 0.6);
        }
    }
    else if (level == 3) {
        for(int i = 0; i<OBS_COUNT; i++) {
        iChangeSpriteFrames(&obs[i], frames_obstacle3, 1);
        iScaleSprite(&obs[i], 0.6);
        }
    }
    
    
}

void loadPlayButton() {
    if (!start_checker && !is_leaderboard && !is_name && is_home && !level_checker) {
        iShowLoadedImage(200,250, &play_button);
        iShowLoadedImage(280, 570, &title);
        iShowLoadedImage(545, 250, &score_button);
        iShowLoadedImage(360, 130, &levels_button);
    }
    else if(!is_leaderboard && !is_name && !is_home) {
        showScoreboard();
    }
}

void loadResources() {
    iInitSprite(&sprite, -1);
    iLoadFramesFromFolder(frames, "assets/images/sprites/Flappy_Bird/");
    iLoadFramesFromFolder(frames_obstacle, "assets/images/sprites/Obstacle/");
    iLoadFramesFromFolder(frames_obstacle2, "assets/images/sprites/obstacle2/" );
    iLoadFramesFromFolder(frames_obstacle3, "assets/images/sprites/obstacle3/" );       
    iLoadImage(&img, "assets/images/1951.png");
    iLoadImage(&gameover, "assets/images/sprites/gameover/gameover.png");
    iLoadImage(&play_button, "assets/images/button1.png");
    iLoadImage(&title, "assets/images/title.png");
    iLoadImage(&score_button, "assets/images/score.png");
    iLoadImage(&restart_button, "assets/images/restart_button.png");
    iLoadImage(&input_name, "assets/images/name.png");
    iLoadImage(&leaderboard_bg, "assets/images/leaderboard.png");
    iLoadImage(&close_button, "assets/images/close.png"); 
    iLoadImage(&home_button, "assets/images/home.png");
    iLoadImage(&levels_button, "assets/images/levels.png");
    iLoadImage(&levels, "assets/images/select_levels.png");

    iResizeImage(&levels_button,  262, 92);
    iResizeImage(&gameover, 500, 510);
    iResizeImage(&img, 1800, 800);
    iResizeImage(&title, 469, 179);
    iResizeImage(&play_button, 262, 92);
    iResizeImage(&score_button, 262, 92);
    iResizeImage(&restart_button, 152, 93);
    iResizeImage(&home_button, 132, 93);
    iResizeImage(&levels, 580, 580);
    iResizeImage(&input_name, 320, 75);
    
    iSetSpritePosition(&sprite, bird_x, bird_y);
    iChangeSpriteFrames(&sprite, frames, 6);
    setObsFrame(selected_level);
    //iniObsPos();
    
    iScaleSprite(&sprite, 0.35);
}

void iAnim() {
    if (game_status == 1) {
        iAnimateSprite(&sprite);
    }
}


/*
function iMouseMove() is called when the user moves the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
    // place your codes here
}
/*
function iMouseDrag() is called when the user presses and drags the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseDrag(int mx, int my)
{
    // place your codes here
}

/*
function iMouse() is called when the user presses/releases the mouse.
(mx, my) is the position where the mouse pointer is.RRSR
*/
void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {   
        
        if ((mx>180 && mx<440) && (my > 250 && my<358) && start_checker == 0 && !is_leaderboard && is_home) {
            iPlaySound("assets/sounds/click.wav", false, 100);
            is_name = 1;
        }

        else if ((mx > 380 && mx < 660) && (my > 130 && my < 227) && !is_leaderboard && !is_name && is_home && !level_checker ) {
            iPlaySound("assets/sounds/click.wav", false, 100);
            level_checker = 1;
            is_home = 0;
        }
        
        else if (!is_leaderboard && !is_name && !is_home && (mx>290 && mx<376 && my>550 && my< 640) && level_checker) {
            is_home = 1;
            level_checker = 0;
            
        }
        else if (!is_leaderboard && !is_name && !is_home && (mx>377 && mx<630 && my>520 && my<600) && level_checker) {//level 1
            iPlaySound("assets/sounds/click.wav", false, 100);
            is_home = 1;
            select_level(1);
            change_background();
            setObsFrame(selected_level);
            
        }

        else if (!is_leaderboard && !is_name && !is_home && (mx>377 && mx<639 && my>400 && my<480) && level_checker) {//level 1
            iPlaySound("assets/sounds/click.wav", false, 100);
            is_home = 1;
            select_level(2);
            change_background();
            setObsFrame(selected_level);
        }

        else if (!is_leaderboard && !is_name && !is_home && (mx>377 && mx<630 && my>270 && my<350) && level_checker) {//level 1
            iPlaySound("assets/sounds/click.wav", false, 100);
            is_home = 1;
            select_level(3);
            change_background();
            setObsFrame(selected_level);
            
        }

        else if (game_status == 0 && start_checker == 0 && mx >550 && mx < 700 && my>260 && my < 350) {
            iPlaySound("assets/sounds/click.wav", false, 100);
            restart_game();
            is_name = 1;
        }
        else if (start_checker == 1 && game_status == 1){
            iPlaySound("assets/sounds/flappy.wav", false, 100);
            bird_y += 75;
            fall_timer = 1;
            iSetSpritePosition(&sprite, bird_x, bird_y);
        }
        else if(is_leaderboard == 0 && (mx >550 && mx<800) && (my>250 && my<350) && !start_checker && is_home && !level_checker && !is_name) {
            iPlaySound("assets/sounds/click.wav", false, 100);
            createLeaderboard();
            is_leaderboard = 1;
        }
        else if(is_leaderboard == 1 &&(my>180 && my<300) &&(mx > 100 && mx <200) && !start_checker && is_home) {
            is_home = 1;
            level_checker = 0;
            is_leaderboard = 0;
            fclose(leaderboard);
        }
        else if(!is_home && !start_checker && ((mx>100 && mx<500) && (my>260 && my<350)) && !level_checker && !is_name) {
            iPlaySound("assets/sounds/click.wav", false, 100);
            is_home = 1;
            is_name = 0;
            restart_game();
        }
        else if(is_name) {
            iPlaySound("assets/sounds/click.wav", false, 100);
            is_name = 0;
            start_checker = 1;
            iniObsPos();
        }
    }

}



/*
function iMouseWheel() is called when the user scrolls the mouse wheel.
dir = 1 for up, -1 for down.
*/
void iMouseWheel(int dir, int mx, int my)
{
    if(dir == -1) {
        scroll_down = 1;
    }
    if(dir == 1) {
        scroll_up = 1;
    }
}

/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    if(game_status && start_checker) {
        switch (key)
        {
        case 32:
            fall_timer = 1;
            bird_y += 75;
            
        
            iSetSpritePosition(&sprite, bird_x, bird_y);

            iPlaySound("assets/sounds/flappy.wav", false, 100);
            break;
        // place your codes for other keys here
        default:
            break;
        }
    }

    if(is_name) {
        switch(key) {
            case 13:
                is_name = 0;
                start_checker = 1;
                iniObsPos();
                break;
            
            case 8:
                if(name_len > 0) {
                    name[name_len - 1] = '\0';
                    name_len--;
                }
                break;

            default:
                name[name_len] = key;
                name_len++;
                break;
        }
    }
}

/*
function iSpecialKeyboard() is called whenver user hits special keys likefunction
keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_RF5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11,
GLUT_KEY_F12, GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN,
GLUT_KEY_PAGE_UP, GLUT_KEY_PAGE_DOWN, GLUT_KEY_HOME, GLUT_KEY_END,
GLUT_KEY_INSERT */
void iSpecialKeyboard(unsigned char key)
{
    switch (key)
    {
    case GLUT_KEY_END:
        // do something
        break;
    // place your codes for other keys here
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    // place your own initialization codes here.
    srand(time(0));
    loadResources();
    iInitializeSound();
    iSetTimer(ANIM_DELAY, iAnim);
    iSetTimer(FASTEN_DELAY, fastenBird);
    iSetTimer(ANIM_DELAY, birdFall);
    iInitialize(WIDTH, HEIGHT, "Flappy Bird");
    //iPlaySound("assets/sounds/level.wav", true, 30);     

    return 0;
}
// ./runner.bat iMain.cpp