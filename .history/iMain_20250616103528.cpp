#include "iGraphics.h"
#include "iSound.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
//anik
/*
function iDraw() is called again and again by the system.

*/
#define WIDTH 1000
#define HEIGHT 1000
#define ANIM_DELAY 100
#define FASTEN_DELAY 15*1000
#define OBS_COUNT 3

Image img;
Image frames[6];
Image frames_obstacle[10];
Image frame_gameover[1];
Sprite sprite;
Sprite obs[OBS_COUNT];
Sprite gameover;

int bird_y = 500;
int bird_x = 400;
int fall_timer = 1;
int fall_rate = 1;
int increase_speed = 5;
int game_is_over = 1;

int obstacle_position_x[5];
int obstacle_position_y[5];

void birdFall();
void gameOver();
void showObs();
void setObs(int c);
void playGameOver();
int randHeight();

void iDraw()
{
    // place your drawing codes here
    iClear();
    
    iShowLoadedImage(0, 0, &img);
    iWrapImage(&img, -5);
    iShowSprite(&sprite);
    showObs();
}

void setObs(int c) {
    obstacle_position_x[c] = 1200;
    obstacle_position_y[c] = randHeight();
    iSetSpritePosition(&obs[c], obstacle_position_x[c], obstacle_position_y[c]);
}

void showObs() {
    for(int j = 0; j<OBS_COUNT; j++) {
        if(obstacle_position_x[j] < -120) {
            setObs(j);
        }
        else {
            obstacle_position_x[j] -= increase_speed;
            iSetSpritePosition(&obs[j], obstacle_position_x[j], obstacle_position_y[j]);
        }
        iShowSprite(&obs[j]);

        if(iCheckCollision(&sprite, &obs[j])) {
            // gameOver();
        }
    }
}

void iniObsPos() {
    for(int i = 0; i<OBS_COUNT; i++) {
        obstacle_position_x[i] = 800 + i*420;
    }
}

void gameOver() {
    iSetSpritePosition(&gameover, 300, 350);
    iShowSprite(&gameover);
    if(game_is_over) {
        playGameOver();
        iDelay(2);
    }
}

void playGameOver() {
    iPlaySound("assets/sounds/gameover.wav", false, 100);
    game_is_over = 0;
}

int randHeight() {
    int lower = -100;
    int upper = 100;
    int randomNumber = lower + rand() % (upper - lower + 1);

    std::cout << "Random number: " << randomNumber << std::endl;

    return -200 + randomNumber;
}

void birdFall () {
    fall_timer+=fall_rate;
    bird_y -= fall_timer*fall_timer;
    iSetSpritePosition(&sprite, bird_x, bird_y);
}

void fastenBird() {
    increase_speed += 2;
}

void setObsFrame() {
    for(int i = 0; i<5; i++) {
        iChangeSpriteFrames(&obs[i], frames_obstacle, 1);
        iScaleSprite(&obs[i], 0.7);
    }
}

void loadResources() {
    iInitSprite(&sprite, -1);

    iLoadFramesFromFolder(frame_gameover, "assets/images/sprites/gameover/");
    iLoadFramesFromFolder(frames, "assets/images/sprites/Flappy_Bird/");
    iLoadFramesFromFolder(frames_obstacle, "assets/images/sprites/Obstacle/");
    iLoadImage(&img, "assets/images/1951.png");

    iResizeImage(&img, 1800, 1000);

    iChangeSpriteFrames(&sprite, frames, 6);
    iChangeSpriteFrames(&gameover, frame_gameover, 1);

    setObsFrame();
    iniObsPos();
    
    iScaleSprite(&sprite, 0.4);
    iScaleSprite(&gameover, 0.5);
}

void iAnim() {
    iAnimateSprite(&sprite);
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
        // place your codes here
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // place your codes here
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
    iSetTimer(ANIM_DELAY, iAnim);
    iSetTimer(FASTEN_DELAY, fastenBird);
    iSetTimer(ANIM_DELAY, birdFall);
    iInitializeSound();
    iInitialize(WIDTH, HEIGHT, "Flappy Bird");

    return 0;
}
// ./runner.bat iMain.cpp
