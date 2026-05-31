#include "drawing.h"

int gX = 0;
int gY = 0;

void Update() {
    DrawLine(0, 0, WINDOW_W, WINDOW_H, 0xFFFFFF);
    DrawRect(gX + 20, gY + 20, gX + 100, gY + 100, 0xFF00FF);
    DrawCircle(200, 200, 50, 0x00FFFF, TRUE);

    static const int speed = 10;
    if(GetAsyncKeyState(VK_LEFT)) {
        gX -= speed;
    }
    if(GetAsyncKeyState(VK_RIGHT)) {
        gX += speed;
    }
    if(GetAsyncKeyState(VK_UP)) {
        gY -= speed;
    }
    if(GetAsyncKeyState(VK_DOWN)) {
        gY += speed;
    }
}

int main() {
    Start();

    return 0;
}
