#include<GL/glut.h>
#include<ctime>

#define width 80
#define height 45

using namespace std;

class game{
public:
    int delay;
    float scale;
    int WinScale, WinPosX, WinPosY,
        StartPosX, StartPosY, StartSize;

        game(){
            StartSize = 4; // start size snake
            StartPosX = (int) (width / 2); // start coordinate head snake
            StartPosY = (int) (height / 2);
            delay = 100; // lead time
            scale = 1; // scope
            WinScale = 10; // scope window
            WinPosX = 200;
            WinPosY = 0;
        }
        void init();

} settings;

class block{
public:
    int x, y;

    block(){
        x = -1;
        y = -1;
    }
    void draw();
} blocks[width * height];

class food{
public:
    int x, y;

    food(){
        x = -1;
        y = -1;
    }
    void draw_food();
    void spawn();
} food;

class snake{
public:
    int direction; // move snake
    int size; // snake size
    bool turn;
    snake(){
        turn = false;
        size = settings.StartSize;
    }
    void drawSnake();
    void move();
    void cut();
    void eat();
} player;

void snake::drawSnake(){
    for(int iter = 0; iter < size; ++iter){
        blocks[iter].draw();
    }
}

void snake::move(){
    for (int iter = size; iter > 0; iter--){
        blocks[iter].x = blocks[iter - 1].x;
        blocks[iter].y = blocks[iter - 1].y;
}

    if (blocks[0].x < food.x) {
        ++blocks[0].x;
    }
    else if (blocks[0].x > food.x){
        --blocks[0].x;
    }
    else if (blocks[0].y < food.y){
        ++blocks[0].y;
    }
    else if (blocks[0].y > food.y)
        --blocks[0].y;

}

void snake::eat(){
    if((blocks[0].x == food.x) && (blocks[0].y == food.y)){
        size++;
        food.spawn();
    }
}

void snake::cut(){
    for(int iter = 3; iter < size; iter++){
        if((blocks[0].x == blocks[iter].x) && (blocks[0].y == blocks[iter].y)){
            break;
        }
    }
}

void block::draw(){
    glColor3f(0, 1, 0);
    glVertex2f(x * settings.scale, y * settings.scale);
    glVertex2f(x * settings.scale + settings.scale, y * settings.scale);
    glVertex2f(x * settings.scale + settings.scale, y * settings.scale + settings.scale);
    glVertex2f(x * settings.scale, y * settings.scale + settings.scale);
}

void food::draw_food(){
    glColor3f(1, 0, 0);
    glVertex2f(x * settings.scale, y * settings.scale);
    glVertex2f(x * settings.scale + settings.scale, y * settings.scale);
    glVertex2f(x * settings.scale + settings.scale, y * settings.scale + settings.scale);
    glVertex2f(x * settings.scale, y * settings.scale + settings.scale);
}

void game::init(){
    blocks[0].x = StartPosX;
    blocks[0].y = StartPosY;
}

void food::spawn(){
    x = rand() % width;
    y = rand() % height;
    for(int iter = 0; iter < player.size; ++iter){
        if((x == blocks[iter].x) && (y == blocks[iter].y))
            spawn();
    }
}

void timer(int value){
    player.turn = true;
    player.move();
    player.cut();
    player.eat();
    glutPostRedisplay();
    glutTimerFunc(settings.delay, timer, 0);
}

void draw(){
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    food.draw_food();
    player.drawSnake();
    glEnd();
    glutSwapBuffers();
}

int main(int argc, char **argv){
    settings.init();
    food.spawn();
    srand(time(0));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(width * settings.WinScale, height * settings.WinScale);
    glutInitWindowPosition(settings.WinPosX, settings.WinPosY);
    glutCreateWindow("SnakeWindow");
    glutDisplayFunc(draw);
    glutTimerFunc(settings.delay, timer, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);

    glutMainLoop();

    return 0;
}
