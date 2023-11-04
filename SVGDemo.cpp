#include "stdafx.h"
#include "rapidxml.hpp"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <wingdi.h>
#include <vector>
#include <fstream>
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
//
//void DrawStar(HDC hdc, int cx, int cy, int size, float rotation_angle)
//{
//    // Define the points for the 5-pointed star
//    POINT points[12];
//    for (int i = 0; i < 10; i++)
//    {
//        double outerRadius = i % 2 == 0 ? size : size / 2.5;
//        double angle = 3.14159 * i / 5 + rotation_angle;
//        points[i].x = cx + static_cast<int>(outerRadius * cos(angle));
//        points[i].y = cy + static_cast<int>(outerRadius * sin(angle));
//    }
//    points[10] = points[0];
//    points[11] = points[1];
//    // Create and select a yellow brush
//    HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0));
//    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, yellowBrush);
//
//    // Create a black pen
//    HPEN redPen = CreatePen(PS_SOLID, 7, RGB(255, 0, 0));
//    HPEN oldPen = (HPEN)SelectObject(hdc, redPen);
//
//    GraphicsPath path;
//    Graphics new_graphics(hdc);
//    Pen another_red_pen(Color(255, 255, 0, 0), 7);
//    path.StartFigure();
//    for (int i = 0; i < 11; i++) {
//        path.AddLine(Point(points[i].x, points[i].y), Point(points[i + 1].x, points[i + 1].y));
//    }
//    another_red_pen.SetLineJoin(LineJoinMiter);
//    new_graphics.DrawPath(&another_red_pen, &path);
//    //Draw the star
//    Polygon(hdc, points, 10);
//
//    // Restore the old brush and pen
//    SelectObject(hdc, oldBrush);
//    SelectObject(hdc, oldPen);
//
//    // Clean up
//    DeleteObject(yellowBrush);
//    DeleteObject(redPen);
//}
void DrawStar(HDC hdc, int cx, int cy, int size, float rotation_angle, float fill_opacity, float stroke_opacity)
{
    // Define the points for the 5-pointed star
    POINT points[12];
    for (int i = 0; i < 10; i++)
    {
        double outerRadius = i % 2 == 0 ? size : size / 2.5;
        double angle = 3.14159 * i / 5 + rotation_angle;
        points[i].x = cx + static_cast<int>(outerRadius * cos(angle));
        points[i].y = cy + static_cast<int>(outerRadius * sin(angle));
    }
    points[10] = points[0];
    points[11] = points[1];

    // Create a GraphicsPath to draw the star
    
    GraphicsPath path;
    path.StartFigure();
    for (int i = 0; i < 11; i++) {
        path.AddLine(Point(points[i].x, points[i].y), Point(points[i + 1].x, points[i + 1].y));
    }

    // Fill the star with a yellow color
    SolidBrush yellowBrush(Color(fill_opacity * 255, 255, 255, 0)); // Yellow color
    Graphics new_graphics(hdc);
    new_graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    new_graphics.FillPath(&yellowBrush, &path);

    // Create a Pen to draw the star outline with a red color
    Pen redPen(Color(stroke_opacity *  255, 255, 0, 0), 7); // Red color, line width 7
    new_graphics.DrawPath(&redPen, &path);

}

void drawPolyline(HDC hdc) {
    Graphics graphics(hdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    Point points[] = { Point(0, 40), Point(40, 40), Point(40, 80), 
                        Point(80, 80), Point(80, 120), Point(120, 120), Point(120, 140) };

    // Define the brush with fill-opacity
    SolidBrush brush(Color(179, 80, 80, 80)); // Blue color with 50% opacity

    // Fill the polyline shape
    graphics.FillPolygon(&brush, points, 7);
}


void drawLineLeftCorner(HDC hdc) {
    Graphics graphics(hdc);
    Pen pen(Color(255, 0, 0, 255), 5);
    graphics.DrawLine(&pen, 10, 30, 30, 10);
    Pen pen1(Color(255, 0, 0, 255), 10);
    graphics.DrawLine(&pen1, 30, 30, 50, 10);
    Pen pen2(Color(255, 0, 0, 255), 15);
    graphics.DrawLine(&pen2, 50, 30, 70, 10);
    Pen pen3(Color(255, 0, 0, 255), 20);
    graphics.DrawLine(&pen3, 70, 30, 90, 10);
    Pen pen4(Color(255, 0, 0, 255), 25);
    graphics.DrawLine(&pen4, 90, 30, 110, 10);
}

void drawPolylineLeftCorner(HDC hdc) {
    Graphics graphics(hdc);
    Pen pen(Color(255, 255, 0, 0), 2);
    PointF point1(5.0f, 37.0f);
    PointF point2(15.0f, 37.0f);
    PointF point3(15.0f, 32.0f);
    PointF point4(25.0f, 32.0f);
    PointF point5(25.0f, 37.0f);
    PointF point6(35.0f, 37.0f);
    PointF point7(35.0f, 25.0f);
    PointF point8(45.0f, 25.0f);
    PointF point9(45.0f, 37.0f);
    PointF point10(55.0f, 37.0f);
    PointF point11(55.0f, 17.0f);
    PointF point12(65.0f, 17.0f);
    PointF point13(65.0f, 37.0f);
    PointF point14(75.0f, 37.0f);
    PointF point15(75.0f, 10.0f);
    PointF point16(85.0f, 10.0f);
    PointF point17(85.0f, 37.0f);
    PointF point18(95.0f, 37.0f);
    PointF point19(95.0f, 2.0f);
    PointF point20(105.0f, 2.0f);
    PointF point21(105.0f, 37.0f);
    PointF point22(115.0f, 37.0f);
    PointF points[22] = { point1, point2, point3, point4, point5, point6, point7, point8,
                            point9, point10, point11, point12, point13, point14, point15, point16,
                            point17, point18, point19, point20, point21, point22 };
    graphics.DrawLines(&pen, points, 22);

    SolidBrush fillBrush(Color(128, 0, 255, 255));
    graphics.FillPolygon(&fillBrush, points, 22);
}
void drawRectangleFrame(HDC hdc) {
    Graphics graphics(hdc);
    // vẽ hai khung hình chữ nhật.
    Pen blackPen2(Color(255, 55, 55, 55), 2);
    graphics.DrawRectangle(&blackPen2, 10, 10, 670, 330);
    Pen outer_blackPen(Color(50, 55, 55, 55), 2);
    graphics.DrawRectangle(&outer_blackPen, 15, 15, 670, 330);
}
void drawBackgroundColor(HDC hdc) {
    Graphics graphics(hdc);
    // Tô màu hình chữ nhật ở trong.
    SolidBrush solidBrush2(Color(90, 255, 182, 193));
    graphics.FillRectangle(&solidBrush2, 10, 10, 670, 330);
}
void drawEllipse(HDC hdc) {
    Graphics graphics(hdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    Pen      pen1(Color(255, 255, 255, 0));
    pen1.SetWidth(6);
    Rect rect(500, 100, 200, 100);
    graphics.DrawEllipse(&pen1, rect);
    SolidBrush solidBrush(Color(255, 0, 255, 0));
    graphics.FillEllipse(&solidBrush, 500, 100, 200, 100);
}

void drawCircle(HDC hdc, int centerX, int centerY, int radius) // x = 100,y =  200,r = 210.
{
    Graphics graphics(hdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    Pen pen(Color(255, 0, 255, 255), 10); // Blue color pen
    graphics.DrawEllipse(&pen, centerX, centerY, radius, radius);
    SolidBrush solidBrush1(Color(128, 255, 255, 0));
    graphics.FillEllipse(&solidBrush1, centerX, centerY, radius, radius);
}
void drawStringNVA(HDC hdc) {
    Graphics graphics(hdc);
    SolidBrush  brush(Color(255, 200, 150, 255));
    FontFamily  fontFamily(L"Times New Roman");
    Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
    PointF      pointF(340.0f, 300.0f);
    graphics.DrawString(L"Nguyen Van A", -1, &font, pointF, &brush);
}
void drawPolygon(HDC hdc) {
    Graphics graphics(hdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    Pen blackPen1(Color(255, 255, 0, 102), 10);
    PointF point1(950.0f, 120.0f);
    PointF point2(1050.0f, 180.0f);
    PointF point3(1050.0f, 280.0f);
    PointF point4(950.0f, 340.0f);
    PointF point5(850.0f, 280.0f);
    PointF point6(850.0f, 180.0f);
    PointF points[6] = { point1, point2, point3, point4, point5, point6 };
    PointF* pPoints = points;

    // Draw the polygon.
    graphics.DrawPolygon(&blackPen1, pPoints, 6);
    // Fill polygon
    SolidBrush blueBrush(Color(255, 153, 204, 255));
    graphics.FillPolygon(&blueBrush, points, 6);
}
VOID OnPaint(HDC hdc)
{
    Graphics graphics(hdc);

    drawRectangleFrame(hdc);
    drawBackgroundColor(hdc);
    drawCircle(hdc, 100, 170,150);
    // this code is used to draw string "Nguyen Van A".
    drawStringNVA(hdc);
    drawEllipse(hdc);
    drawPolygon(hdc);
    // Vẽ hình ngôi sao
    DrawStar(hdc, 300, 160, 120, 2.2f, 0.6f, 0.7f);
    drawPolylineLeftCorner(hdc);
    drawLineLeftCorner(hdc);
    drawPolyline(hdc);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
   HWND                hWnd;
   MSG                 msg;
   WNDCLASS            wndClass;
   GdiplusStartupInput gdiplusStartupInput;
   ULONG_PTR           gdiplusToken;

   // Read XML
   xml_document<> doc;
   xml_node<> *rootNode;
   // Read the xml file into a vector
   ifstream file("sample.svg");
   vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
   buffer.push_back('\0');
   // Parse the buffer using the xml file parsing library into doc 
   doc.parse<0>(&buffer[0]);

   rootNode = doc.first_node();
   xml_node<> *node = rootNode->first_node();

   while (node != NULL) {
	   char *nodeName = node->name();
	   xml_attribute<> *firstAttribute = node->first_attribute();
	   char *attributeName = firstAttribute->name();
	   char *attributeValue = firstAttribute->value();
	   xml_attribute<> *secondAttribute = firstAttribute->next_attribute();
	   // Set breakpoint here to view value
	   // Ref: http://rapidxml.sourceforge.net/manual.html
	   node = node->next_sibling();
   }

   
   
   // Initialize GDI+.
   GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
   
   wndClass.style          = CS_HREDRAW | CS_VREDRAW;
   wndClass.lpfnWndProc    = WndProc;
   wndClass.cbClsExtra     = 0;
   wndClass.cbWndExtra     = 0;
   wndClass.hInstance      = hInstance;
   wndClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
   wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
   wndClass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
   wndClass.lpszMenuName   = NULL;
   wndClass.lpszClassName  = TEXT("GettingStarted");
   
   RegisterClass(&wndClass);
   
   hWnd = CreateWindow(
      TEXT("GettingStarted"),   // window class name
      TEXT("SVG Demo"),  // window caption
      WS_OVERLAPPEDWINDOW,      // window style
      CW_USEDEFAULT,            // initial x position
      CW_USEDEFAULT,            // initial y position
      CW_USEDEFAULT,            // initial x size
      CW_USEDEFAULT,            // initial y size
      NULL,                     // parent window handle
      NULL,                     // window menu handle
      hInstance,                // program instance handle
      NULL);                    // creation parameters
      
   ShowWindow(hWnd, iCmdShow);
   UpdateWindow(hWnd);
   
   while(GetMessage(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
   
   GdiplusShutdown(gdiplusToken);
   return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, 
   WPARAM wParam, LPARAM lParam)
{
   HDC          hdc;
   PAINTSTRUCT  ps;
   
   switch(message)
   {
   case WM_PAINT:
      hdc = BeginPaint(hWnd, &ps);
      OnPaint(hdc);
      EndPaint(hWnd, &ps);
      return 0;
   case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
   default:
      return DefWindowProc(hWnd, message, wParam, lParam);
   }
} // WndProc
