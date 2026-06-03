#include "Wav.hpp"
#include "drawing.hpp"
#include "sound.hpp"

#pragma comment(lib, "oct_binary.lib")
#pragma comment(lib, "oct_wav.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Winmm.lib")

int gX = 0;
int gY = 0;

void Update() {
    DrawLine(0, 0, WINDOW_W, WINDOW_H, 0xFFFFFFFF);
    DrawRect(gX + 20, gY + 20, 100, 100, 0x0FFF00FF);
    DrawRect(gX + 40, gY + 40, 100, 100, 0x0F00FF00);
    DrawCircle(200, 200, 50, 0xFF00FFFF, TRUE);
    DrawTriangle(100, 100, 50, 200, 150, 300, 0xFFFFFFFF);

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

//int main() {
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Sound::SOUNDDATA sd;

    Sound::Load("H.wav", &sd);
    Sound::Play(&sd);

    Start();

    Sound::DestroySoundData(&sd);

    return 0;
}
