﻿#include "stdafx.h"
#include "rapidxml.hpp"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <vector>
#include <fstream>
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
void DrawStar(HDC hdc, int cx, int cy, int size, float rotation_angle)
{
    // Define the points for the 5-pointed star
    POINT points[11];
    for (int i = 0; i < 10; i++)
    {
        double outerRadius = i % 2 == 0 ? size : size / 2.5;
        double angle = 3.14159 * i / 5 + rotation_angle;
        points[i].x = cx + static_cast<int>(outerRadius * cos(angle));
        points[i].y = cy + static_cast<int>(outerRadius * sin(angle));
    }
    points[10].x = points[0].x;
    points[10].y = points[0].y;
    // Create and select a yellow brush
    HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0));
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, yellowBrush);

    // Create a black pen
    HPEN redPen = CreatePen(PS_SOLID, 7, RGB(255, 0, 0));
    HPEN oldPen = (HPEN)SelectObject(hdc, redPen);
    
    GraphicsPath path;
    Graphics new_graphics(hdc);
    Pen another_red_pen(Color(255, 255, 0, 0), 7);
    path.StartFigure();
    for (int i = 0; i < 10; i++) {
        path.AddLine(Point(points[i].x, points[i].y), Point(points[i + 1].x, points[i + 1].y));
    }
    another_red_pen.SetLineJoin(LineJoinMiter);
    new_graphics.DrawPath(&another_red_pen, &path);
     //Draw the star
    Polygon(hdc, points, 10);

    // Restore the old brush and pen
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);

    // Clean up
    DeleteObject(yellowBrush);
    DeleteObject(redPen);
}
VOID OnPaint(HDC hdc)
{
    Graphics graphics(hdc);
    Pen      pen(Color(255, 128, 128, 128));
    pen.SetWidth(1.5);
    graphics.DrawLine(&pen, 0, 38, 100, 125);

    Pen      pen1(Color(255, 255, 255, 0));
    pen1.SetWidth(3);
    Rect rect(500, 100, 200, 100);
    graphics.DrawEllipse(&pen1, rect);
    SolidBrush solidBrush(Color(255, 0, 255, 0));
    graphics.FillEllipse(&solidBrush, 500, 100, 200, 100);

    Pen blackPen(Color(255, 255, 0, 0));
    graphics.DrawRectangle(&blackPen, 0, 0, 200, 50);

    Pen      pen2(Color(255, 0, 255, 255));
    pen2.SetWidth(15);
    Rect rect1(100, 200, 210, 210);
    graphics.DrawEllipse(&pen2, rect1);
    SolidBrush solidBrush1(Color(255, 255, 255, 0));
    graphics.FillEllipse(&solidBrush1, 100, 200, 210, 210);

    Pen blackPen1(Color(255, 255, 192, 203), 10);

    // Create an array of PointF objects that define the polygon.
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
// Phần COMMIT của Tùng ngày 28/10/2023
   // Tô màu hình chữ nhật ở trong.
   SolidBrush solidBrush(Color(90, 255, 182, 193));
   graphics.FillRectangle(&solidBrush, 10, 10, 670, 330);
    
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
