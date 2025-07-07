#include "iGraphics.h"
#include "iSound.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
//anik
/*
function iDraw() is called again and again by the system.

*/
#define WIDTH 1800
#define HEIGHT 1000
Image img;
Image frames[6];
Image gameover;
Image frames_obstacle[10];
Sprite sprite;
Sprite obs[5];

int bird_y = 500;
int bird_x = 850;
int fall_timer = 1;

int obstacle_position_x = 1200;

void birdFall();
void gameOver();
void showObs(int i);
int randNum();

int c = 0;

void iDraw()
{
    // place your drawing codes here
    iClear();
    
    iShowLoadedImage(0, 0, &img);
    iWrapImage(&img, -5);
    iShowSprite(&sprite);
    showObs(c);
    
}

void increaseObs() {
    if(c < 4) {
    c++;
    iChangeSpriteFrames(&obs[c], frames_obstacle, 1);
    iSetSpritePosition(&obs[c], 700, -200);
    iScaleSprite(&obs[c], 0.7);
    }
}

void showObs(int i) {
    for(int j = 0; j<=i; j++) {
        iShowSprite(&obs[i]);
        obstacle_position_x -=5;
        iSetSpritePosition(&obs[i], obstacle_position_x + randNum(), -200 + randNum());
        if(iCheckCollision(&sprite, &obs[i])) {
            gameOver();  
        }
    }
}

void gameOver() {
    iShowLoadedImage(520, 200, &gameover);
}

int randNum() {
    int lower = 100;
    int upper = 300;

    int randomNumber = lower + rand() % (upper - lower + 1);

    std::cout << "Random number: " << randomNumber << std::endl;

    return randomNumber;
}

void birdFall () {
    fall_timer+=1;
    bird_y -=fall_timer*fall_timer;
    iSetSpritePosition(&sprite, bird_x, bird_y);
}

void loadResources() {
    iInitSprite(&sprite, -1);
    iInitSprite(&obs[0], -1);
    
    iLoadFramesFromFolder(frames, "assets/images/sprites/Flappy_Bird/");
    iLoadFramesFromFolder(frames_obstacle, "assets/images/sprites/Obstacle/");
    iLoadImage(&img, "assets/images/1951.png");
    iLoadImage(&gameover, "assets/images/sprites/Obstacle/gameover.png");

    iResizeImage(&img, 1800, 1000);
    iResizeImage(&gameover, 700, 720);

    iChangeSpriteFrames(&sprite, frames, 6);
    iChangeSpriteFrames(&obs[0], frames_obstacle, 1);
    
    iSetSpritePosition(&sprite, 350 , 200);
    iSetSpritePosition(&obs[0], 700, -200);

    iScaleSprite(&sprite, 0.5);
    iScaleSprite(&obs[0], 0.7);
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
    iSetTimer(100, iAnim);
    iSetTimer(2000, increaseObs);
    iSetTimer(100, birdFall);
    iInitializeSound();
    iInitialize(WIDTH, HEIGHT, "Flappy Bird");

    return 0;
}
// ./runner.bat iMain.cpp
