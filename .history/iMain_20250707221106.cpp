#include "iGraphics.h"
#include "iSound.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include<unistd.h>
//anik farreed
/*
function iDraw() is called again and again by the system.

*/
#define WIDTH 1000
#define HEIGHT 1000
#define ANIM_DELAY 100
#define FASTEN_DELAY 15*1000
#define OBS_COUNT 3

Image play_button;
Image score_button;
Image img;
Image title;
Image frames[6];
Image frames_obstacle[3];
Sprite sprite;
Sprite obs[OBS_COUNT];
Image gameover;
Image restart_button;

int bird_y = 500;
int bird_x = 470;
int fall_timer = 1;
int fall_rate = 1;
int increase_speed = 5;
int game_status = 1;
int start_checker = 0;
int scoreboard = 0;



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
int randHeight();


void iDraw()
{
    // place your drawing codes here
    
    if(game_status) {
        
        iClear();
        iShowLoadedImage(0, 0, &img);
        loadPlayButton();
        iWrapImage(&img, -5);
        iShowSprite(&sprite);
        showObs();
    }
    else {
        
        iShowLoadedImage(260, 350, &gameover);
        iShowLoadedImage(400, 250, &restart_button);
    }
}

void restart_game() {
    iniObsPos();
    start_checker =1;
    increase_speed = 5;
    game_status = 1;
    fall_timer = 1;
    bird_y = 500;
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
            if(obstacle_position_x[j] < -120) {
                setObs(j);
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
        if(obstacle_position_x[j] > 460 && obstacle_position_x[j] < 470) {
            iPlaySound("assets/sounds/scoreplus.wav", false, 25);
            scoreboard++;
        }
            }
        }
    else {
        return;
    }
}

void showScoreboard() {
    sprintf(score, "%d", scoreboard);
    iText(470, 920, "SCORE: ", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(560, 920, score, GLUT_BITMAP_TIMES_ROMAN_24);
}

void iniObsPos() {
    for(int i = 0; i<OBS_COUNT; i++) {
        obstacle_position_x[i] = 1100 + i*420;
        obstacle_position_y[i] = 0;
    }
}

void gameOver() {
    if(game_status) {
        iPlaySound("assets/sounds/hit.wav", false, 100);
        
        game_status = 0;
    }
}

int randHeight() {
    int lower = -100;
    int upper = 100;
    int randomNumber = lower + rand() % (upper - lower + 1);

    std::cout << "Random number: " << randomNumber << std::endl;

    return -200 + randomNumber;
}

void birdFall () {
    if (game_status == 1 && start_checker == 1) {
        
        fall_timer+=fall_rate;
        bird_y -= fall_timer*fall_timer/2;
        iSetSpritePosition(&sprite, bird_x, bird_y);
    }
}

void fastenBird() {
    if (game_status == 1) {
    increase_speed += 2;
    }
}

void setObsFrame() {
    for(int i = 0; i<OBS_COUNT; i++) {
        iChangeSpriteFrames(&obs[i], frames_obstacle, 1);
        iScaleSprite(&obs[i], 0.7);
    }
}

void loadPlayButton() {
    if (!start_checker) {
        iShowLoadedImage(180,250, &play_button);
        iShowLoadedImage(230, 700, &title);
        iShowLoadedImage(550, 250, &score_button);
    }
    else {
        showScoreboard();
    }
}

void loadResources() {
    iInitSprite(&sprite, -1);
    iLoadFramesFromFolder(frames, "assets/images/sprites/Flappy_Bird/");
    iLoadFramesFromFolder(frames_obstacle, "assets/images/sprites/Obstacle/");
    iLoadImage(&img, "assets/images/1951.png");
    iLoadImage(&gameover, "assets/images/sprites/gameover/gameover.png");
    iLoadImage(&play_button, "assets/images/button1.png");
    iLoadImage(&title, "assets/images/title.png");
    iLoadImage(&score_button, "assets/images/score.png");
    iLoadImage(&restart_button, "assets/images/restart_button.png");

    iResizeImage(&gameover, 585, 590);
    iResizeImage(&img, 1800, 1000);
    iResizeImage(&title, 587, 224);
    iResizeImage(&play_button, 308, 108);
    iResizeImage(&score_button, 308, 108);
    iResizeImage(&restart_button, 237, 142);

    
    iSetSpritePosition(&sprite, bird_x, bird_y);
    iChangeSpriteFrames(&sprite, frames, 6);

    setObsFrame();
    iniObsPos();
    
    iScaleSprite(&sprite, 0.4);
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
(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {   
        
        if ((mx>180 && mx<416) && (my > 250 && my<358) && start_checker == 0) {
            start_checker = 1;
            iFreeImage(&play_button);
            iFreeImage(&title);
            iFreeImage(&score_button);
            iniObsPos();
        }
        else if (game_status == 0 && start_checker ==00 && mx >400 && mx < 690 && my>265 && my < 350) {
            restart_game();
        }
        else if (start_checker == 1 && game_status == 1){
            iPlaySound("assets/sounds/flappy.wav", false, 100);
            bird_y += 100;
            fall_timer = 1;
            iSetSpritePosition(&sprite, bird_x, bird_y);
        }
    }

}



/*
function iMouseWheel() is called when the user scrolls the mouse wheel.
dir = 1 for up, -1 for down.
*/
void iMouseWheel(int dir, int mx, int my)
{
    // place your code here
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
}

/*
function iSpecialKeyboard() is called whenver user hits special keys likefunction
keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
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

    return 0;
}
// ./runner.bat iMain.cpp