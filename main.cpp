#include <iostream>
#include <ncurses.h>
#include <random>

const int width = 20;
const int height = 20;

int score;

int x, y;
int fruitX, fruitY;

int tailPositionsX[100], tailPositionsY[100];
int tailLength = 0;
enum direction {
    STOP = 0,
    LEFT,
    RIGHT,
    UPP,
    DOWN
};
direction dir;
bool isGameOver;

int getRandom(int bound) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, bound);
    return dis(gen);
}

void init() {
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);


    isGameOver = false;
    dir = STOP;

    x = width / 2;
    y = height / 2;


    fruitX = getRandom(width);
    fruitY = getRandom(height);

    score = 0;
}

void renderWindow() {
    clear();
    for (int i = 0; i < width + 2; i++) {
        mvprintw(0, i, "+");
    }

    for (int h = 0; h < height + 2; h++) {
        for (int w = 0; w <= width + 1; w++) {
            if (h == 0 | h == height + 1) {
                mvprintw(w, h, "#");
            } else if (w == 0 | w == width + 1) {
                mvprintw(w, h, "+");
            } else if (y == h && x == w) {
                mvprintw(w, h, "0");
            } else if (fruitY == h && fruitX == w) {
                mvprintw(w, h, "$");
            } else {
                for (int j = 0; j < tailLength; j++) {
                    if (tailPositionsX[j] == w && tailPositionsY[j] == h) {
                        mvprintw(w, h, "*");
                    }
                }
            }
        }

        mvprintw(height + 2, 0, "Score: %d", score);
    }
    refresh();
}

void getInput() {
    keypad(stdscr, TRUE);
    halfdelay(1);
    int input = getch();
    switch (input) {
        case KEY_LEFT:
            dir = LEFT;
            break;
        case KEY_RIGHT:
            dir = RIGHT;
            break;
        case KEY_UP:
            dir = UPP;
            break;
        case KEY_DOWN:
            dir = DOWN;
            break;
        case 81:
            isGameOver = true;
    }
}
void updateWindow() {
    int lastX = tailPositionsX[0];
    int lastY = tailPositionsY[0];
    int l2x, l2y;

    tailPositionsX[0] = x;
    tailPositionsY[0] = y;

    for (int i = 1; i < tailLength; i++) {
        l2x = tailPositionsX[i];
        l2y = tailPositionsY[i];
        tailPositionsX[i] = lastX;
        tailPositionsY[i] = lastY;
        lastX = l2x;
        lastY = l2y;
    }

    switch (dir) {
        case LEFT:
            y--;
            break;
        case RIGHT:
            y++;
            break;
        case UPP:
            x--;
            break;
        case DOWN:
            x++;
            break;
    }
    if (x > width || y < 0) {
        isGameOver = true;
    }
    if (x < 1 || y < 1) {
        isGameOver = true;
    }
    if (fruitX == x && fruitY == y) {
        score++;
        fruitX = getRandom(width);
        fruitY = getRandom(height);
        tailLength++;
    }
    for (int i = 0; i < tailLength; i++) {
        if (tailPositionsX[i] == x && tailPositionsY[i] == y) {
            isGameOver = true;
        }
    }
}

int main() {
    init();

    while (!isGameOver) {
        renderWindow();
        getInput();
        updateWindow();
    }
    getch();
    endwin();
    refresh();
    std::cout << "\nGame Over";
    return 0;
}
