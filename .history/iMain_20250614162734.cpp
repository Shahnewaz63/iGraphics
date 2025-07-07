#include "iGraphics.h"
#include "iSound.h"
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

int bird_y = 500;
int bird_x = 850;
int fall_timer = 1;

int obstacle_position_x = 1200;

void birdFall();
void moveObstacle(int i);
void checkCollision(int i);
void gameOver();
void setObstacle(int i);

void iDraw()
{
    // place your drawing codes here
    iClear();
    iLoadImage(&img, "assets/images/1950.jpg");
    iLoadImage(&gameover, "assets/images/sprites/Obstacle/gameover.png");
    iResizeImage(&gameover, 500, 520);
    iResizeImage(&img, WIDTH, HEIGHT);
    iShowLoadedImage(0, 0, &img);
    iWrapImage(&img, -10);
    iShowSprite(&sprite);
    setObstacle(0);
    birdFall();
    moveObstacle(0);
    checkCollision(0);
    
}

void gameOver() {
    iShowLoadedImage(150, 50, &gameover);
}

Sprite obs[10];

void setObstacle(int i) {
    iInitSprite(&obs[i], -1);
    iChangeSpriteFrames(&obs[i], frames_obstacle, i);
    iSetSpritePosition(&obs[i], 700, -200);
    iShowSprite(&obs[i]);
    iScaleSprite(&obs[i], 0.7);
}

void checkCollision(int i) {
    if(iCheckCollision(&sprite, &obs[i])) {
        gameOver();  
    }
}

void birdFall () {
    fall_timer+=1;
    bird_y -=fall_timer*fall_timer;
    iSetSpritePosition(&sprite, bird_x, bird_y);
}

void moveObstacle(int i) {
    obstacle_position_x -=10;
    iSetSpritePosition(&obs[i], obstacle_position_x, -200);
}

void loadResources() {
    iInitSprite(&sprite, -1);
    
    
    iLoadFramesFromFolder(frames, "assets/images/sprites/Flappy_Bird/");
    iLoadFramesFromFolder(frames_obstacle, "assets/images/sprites/Obstacle/");

    iChangeSpriteFrames(&sprite, frames, 6);
    
    iSetSpritePosition(&sprite, 350 , 200);
    iScaleSprite(&sprite, 0.5);
    
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
        bird_y +=100;
    
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
    loadResources();
    iSetTimer(100, iAnim);
    iInitializeSound();
    iInitialize(WIDTH, HEIGHT, "Flappy Bird");

    return 0;
}
// ./runner.bat iMain.cpp
