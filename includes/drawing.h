#ifndef __DRAWING_H__
#define __DRAWING_H__

#include <stdint.h>
#include <windows.h>
#include <wingdi.h>

#define WINDOW_W 800
#define WINDOW_H 600
#define CHANNEL 4
#define FPS 60

extern int8_t *pImage;

int Start();
void Update();

void SetPix(int x, int y, int32_t p);
void DrawLine(int x1, int y1, int x2, int y2, int32_t color);
void DrawRect(int x1, int y1, int x2, int y2, int32_t color);
void DrawCircle(int x0, int y0, int r, int32_t color, int fillFlag);
void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int32_t color);

#endif
