#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BIRD_WIDTH 20
#define BIRD_HEIGHT 20
#define PIPE_WIDTH 60
#define PIPE_GAP 150
#define PIPE_SPEED 5
#define GRAVITY 1
#define FLAP_STRENGTH -10

typedef struct {
    int x, y;
} Pipe;

int birdX, birdY;
int birdSpeedY;
Pipe pipes[2];
int pipeIndex = 0;

void initGame() {
    birdX = SCREEN_WIDTH / 4;
    birdY = SCREEN_HEIGHT / 2;
    birdSpeedY = 0;

    for (int i = 0; i < 2; ++i) {
        pipes[i].x = SCREEN_WIDTH + i * (SCREEN_WIDTH / 2);
        pipes[i].y = rand() % (SCREEN_HEIGHT - PIPE_GAP - 40) + 20;
    }
}

void drawBird() {
    setcolor(YELLOW);
    rectangle(birdX, birdY, birdX + BIRD_WIDTH, birdY + BIRD_HEIGHT);
    setfillstyle(SOLID_FILL, YELLOW);
    floodfill(birdX + 1, birdY + 1, YELLOW);
}

void drawPipes() {
    setcolor(GREEN);
    for (int i = 0; i < 2; ++i) {
        rectangle(pipes[i].x, 0, pipes[i].x + PIPE_WIDTH, pipes[i].y);
        rectangle(pipes[i].x, pipes[i].y + PIPE_GAP, pipes[i].x + PIPE_WIDTH, SCREEN_HEIGHT);
        setfillstyle(SOLID_FILL, GREEN);
        floodfill(pipes[i].x + 1, pipes[i].y / 2, GREEN);
        floodfill(pipes[i].x + 1, pipes[i].y + PIPE_GAP + 1, GREEN);
    }
}

void updateBird() {
    birdSpeedY += GRAVITY;
    birdY += birdSpeedY;

    // Collision with top and bottom walls
    if (birdY <= 0) {
        birdY = 0;
        birdSpeedY = 0;
    }
    if (birdY >= SCREEN_HEIGHT - BIRD_HEIGHT) {
        birdY = SCREEN_HEIGHT - BIRD_HEIGHT;
        birdSpeedY = 0;
    }
}

void updatePipes() {
    for (int i = 0; i < 2; ++i) {
        pipes[i].x -= PIPE_SPEED;

        // Reposition pipes when they go off screen
        if (pipes[i].x < -PIPE_WIDTH) {
            pipes[i].x = SCREEN_WIDTH;
            pipes[i].y = rand() % (SCREEN_HEIGHT - PIPE_GAP - 40) + 20;
        }

        // Collision detection
        if (birdX + BIRD_WIDTH > pipes[i].x &&
            birdX < pipes[i].x + PIPE_WIDTH &&
            (birdY < pipes[i].y || birdY + BIRD_HEIGHT > pipes[i].y + PIPE_GAP)) {
            outtextxy(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, "Game Over!");
            getch();
            exit(0);
        }
    }
}

void updateGame() {
    if (kbhit()) {
        char ch = getch();
        if (ch == ' ') { // Space key to flap
            birdSpeedY = FLAP_STRENGTH;
        } else if (ch == 27) { // ESC key to quit
            closegraph();
            exit(0);
        }
    }

    updateBird();
    updatePipes();
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");
    srand(time(NULL));

    initGame();
    while (1) {
        cleardevice();
        
        updateGame();
        
        drawBird();
        drawPipes();

        delay(30); // Control the speed of the game
    }

    closegraph();
    return 0;
}