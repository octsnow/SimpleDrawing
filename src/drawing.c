#include "drawing.h"
#include <stdio.h>
#include <time.h>
#include <stdio.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Gdi32.lib")

#define CHANNEL 4
#define BITCOUNT 8 * CHANNEL
#define MILLISEC_PER_FRAME 1000 / FPS

int8_t *pImage = NULL;

void setClientSize(HWND hWnd, int width, int height) {
    RECT cr, wr;
    int nw, nh;

    GetClientRect(hWnd, &cr);
    GetWindowRect(hWnd, &wr);

    nw = width + (wr.right - wr.left) - (cr.right - cr.left);
    nh = height + (wr.bottom - wr.top) - (cr.bottom - cr.top);

    SetWindowPos(hWnd, NULL, 0, 0, nw, nh, (SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE));
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void ClearScreen() {
    memset(pImage, 0xFF000000, WINDOW_W * WINDOW_H * 3);
}

int WINAPI Start() {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    TCHAR appName[] = "Game";
    WNDCLASS wc;

    // setting window class attribute.
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = WndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = hInstance;
    wc.hIcon            = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = appName;

    if(!RegisterClass(&wc)) return -1;

    HWND hWnd = CreateWindow(
            appName, appName,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            CW_USEDEFAULT, CW_USEDEFAULT,
            NULL, NULL,
            hInstance, NULL);

    setClientSize(hWnd, WINDOW_W, WINDOW_H);
    if(!hWnd) {
        return -1;
    }

    BITMAPINFO bmi;
    ZeroMemory(&bmi, sizeof(bmi));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = WINDOW_W;
    bmi.bmiHeader.biHeight = -WINDOW_H;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = BITCOUNT;
    bmi.bmiHeader.biCompression = BI_RGB;

    HDC tmpDC = GetDC(GetDesktopWindow());

    HBITMAP hBitmap = CreateDIBSection(tmpDC, &bmi, DIB_RGB_COLORS, (void**)&pImage, 0, 0);

    if(hBitmap == NULL) {
        return -1;
    }

    HDC hDC = CreateCompatibleDC(tmpDC);
    HGDIOBJ hOldHandle = SelectObject(hDC, hBitmap);

    ReleaseDC(GetDesktopWindow(), tmpDC);

    ShowWindow(hWnd, SW_SHOW);

    HDC destDC = GetDC(hWnd);

    clock_t timeCount = 0;
    clock_t lastTime = clock();
    MSG msg = {};
    while(msg.message != WM_QUIT) {
        if(timeCount >= MILLISEC_PER_FRAME) {
            ClearScreen();
            Update();
            timeCount -= MILLISEC_PER_FRAME;
            BitBlt(destDC, 0, 0, WINDOW_W, WINDOW_H, hDC, 0, 0, SRCCOPY);
        }

        timeCount += clock() - lastTime;
        lastTime = clock();

        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    SelectObject(hDC, hOldHandle);
    DeleteDC(hDC);
    DeleteObject(hBitmap);

    return 0;
}

void SetPix(int x, int y, int32_t p) {
    if(x < 0 || x >= WINDOW_W || y < 0 || y >= WINDOW_H) {
        return;
    }
    int32_t* target = (int32_t*)(pImage + (y * WINDOW_W + x) * CHANNEL);
    float srcA = ((p >> 24) & 0xFF) / 255.0f;
    float dstA = ((*target >> 24) & 0xFF) / 255.0f;
    int srcRGB = p & 0xFFFFFF;
    int dstRGB = *target & 0xFFFFFF;
    float outA = srcA + dstA * (1 - srcA);
    int outRGB = (srcRGB * srcA + dstRGB * dstA * (1 - srcA)) / outA;
    outRGB = outA == 0 ? 0 : outRGB;
    *target = ((int)(outA * 0xFF) << 24) | outRGB;

//    *target = p;

    printf("%f", outA);
}

void DrawLine(int x1, int y1, int x2, int y2, int32_t color) {
    float slope = 0;
    if(x1 != x2) {
        slope = (float)(y2 - y1) / (x2 - x1);
        if(x1 > x2) {
            int tmp = x1;
            x1 = x2;
            x2 = tmp;
            tmp = y1;
            y1 = y2;
            y2 = tmp;
        }
    } else if(y1 < 0 || y2 >= WINDOW_H) {
        return;
    }

    if(x2 < 0 || x1 >= WINDOW_W) {
        return;
    }
    if(x1 < 0) {
        y1 += slope * (-x1);
        x1 = 0;
    }
    if(x2 >= WINDOW_W) {
        y2 -= slope * (x2 - WINDOW_W - 1);
        x2 = WINDOW_W - 1;
    }

    slope = 0;
    if(y1 != y2) {
        slope = (float)(x2 - x1) / (y2 - y1);
        if(y1 > y2) {
            int tmp = x1;
            x1 = x2;
            x2 = tmp;
            tmp = y1;
            y1 = y2;
            y2 = tmp;
        }
    } else if(x1 < 0 || x2 >= WINDOW_W) {
        return;
    }

    if(y2 < 0 || y1 >= WINDOW_H) {
        return;
    }
    if(y1 < 0) {
        x1 += slope * (-y1);
        y1 = 0;
    }
    if(y2 >= WINDOW_H) {
        x2 -= slope * (y2 - WINDOW_H - 1);
        y2 = WINDOW_H - 1;
    }

    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);

    if(dx > dy) {
        if(x1 > x2) {
            int tmp = x1;
            x1 = x2;
            x2 = tmp;
            tmp = y1;
            y1 = y2;
            y2 = tmp;
        }
        dy = y2 - y1;

        int x = 0;
        int y = 0;
        int f = 0;
        int k = y1 < y2 ? 1 : -1;
        while(x <= dx) {
            int _x = x + x1;
            int _y = y * k + y1;
            SetPix(_x, _y, color);

            f = f + k * (dy << 1) - dx;
            if(f < 0) {
                x++;
                f += dx;
            } else {
                x++;
                y++;
                f -= dx;
            }
        }
    } else if(dy > dx) {
        if(y1 > y2) {
            int tmp = x1;
            x1 = x2;
            x2 = tmp;
            tmp = y1;
            y1 = y2;
            y2 = tmp;
        }
        dx = x2 - x1;

        int x = 0;
        int y = 0;
        int f = 0;
        int k = x1 < x2 ? 1 : -1;
        while(y <= dy) {
            int _x = x * k + x1;
            int _y = y + y1;
            SetPix(_x, _y, color);

            f = f + k * (dx << 1) - dy;
            if(f < 0) {
                y++;
                f += dy;
            } else {
                x++;
                y++;
                f -= dy;
            }
        }
    }
}

void DrawRect(int x1, int y1, int w, int h, int32_t color) {
    for(int x = 0; x < w; x++) {
        for(int y = 0; y < h; y++) {
            SetPix(x + x1, y + y1, color);
        }
    }
}

void DrawCircle(int x0, int y0, int r, int32_t color, int fillFlag) {
    int x = r;
    int y = 0;
    int f = 0;

    if(fillFlag) {
        while(y <= x) {
            for(int i = 0; i <= x; i++) {
                SetPix(x0 + i, y0 + y, color);
                SetPix(x0 + i, y0 - y, color);
                SetPix(x0 - i, y0 + y, color);
                SetPix(x0 - i, y0 - y, color);
                SetPix(x0 + y, y0 + i, color);
                SetPix(x0 + y, y0 - i, color);
                SetPix(x0 - y, y0 + i, color);
                SetPix(x0 - y, y0 - i, color);
            }
            f = f + 5 - (x << 2) + (y << 3);
            if(f < 0) {
                y++;
                f = f - 1 + (x << 2);
            } else {
                x--;
                y++;
                f = f + 3 - (x << 2);
            }
        }
    } else {
        while(y <= x) {
            SetPix(x0 + x, y0 + y, color);
            SetPix(x0 + x, y0 - y, color);
            SetPix(x0 - x, y0 + y, color);
            SetPix(x0 - x, y0 - y, color);
            SetPix(x0 + y, y0 + x, color);
            SetPix(x0 + y, y0 - x, color);
            SetPix(x0 - y, y0 + x, color);
            SetPix(x0 - y, y0 - x, color);
            f = f + 5 - (x << 2) + (y << 3);
            if(f < 0) {
                y++;
                f = f - 1 + (x << 2);
            } else {
                x--;
                y++;
                f = f + 3 - (x << 2);
            }
        }
    }
}

void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int32_t color) {
    int miny = y1 > y2 ? (y2 > y3 ? y3 : y2) : (y1 > y3 ? y3 : y1);
    int maxy = y1 > y2 ? (y1 > y3 ? y1 : y3) : (y2 > y3 ? y2 : y3);

    if(miny >= WINDOW_H || maxy < 0) {
        return;
    }

    miny = miny < 0 ? 0 : miny;
    maxy = maxy >= WINDOW_H ? WINDOW_H : maxy;

    for(int y = miny; y <= maxy; y++) {
    }
}
