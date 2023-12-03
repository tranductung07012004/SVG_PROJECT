#include "stdafx.h"
#include "rapidxml.hpp"
#include "classSVG.h"
#include "FunctionRead.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <vector>
#include <fstream>
#include <gdiplusgraphics.h>
#include <gdiplusheaders.h>
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

double width = CW_USEDEFAULT, height = CW_USEDEFAULT;
double minX = CW_USEDEFAULT, minY = CW_USEDEFAULT, maxX = CW_USEDEFAULT, maxY = CW_USEDEFAULT;
float rotate_angle = 0.0f;
string filename = "svg-02.svg";

VOID OnPaint(HDC hdc, float zoomFactor)
{
    Graphics graphics(hdc);
    vector<SVGElement> elements = parseSVG(filename, width, height, minX, minY, maxX, maxY);
    pointMinMax ptMM;

    // Initialize zoom and rotation transformations
    Matrix zoomTransform(zoomFactor, 0.0f, 0.0f, zoomFactor, 0.0f, 0.0f);
    Matrix rotationTransform;

    // Apply zoom transformation
    graphics.SetTransform(&zoomTransform);

    vector<unique_ptr<ShapeSVG>> shapes;

    for (const SVGElement& element : elements) {
        unique_ptr<ShapeSVG> shapeElement;

        if (element.type == "rect") {
            shapeElement = make_unique<RectSVG>();
        }
        else if (element.type == "text") {
            shapeElement = make_unique<TextSVG>();
        }
        else if (element.type == "circle") {
            shapeElement = make_unique<CircleSVG>();
        }
        else if (element.type == "polyline") {
            shapeElement = make_unique<PolylineSVG>();
        }
        else if (element.type == "ellipse") {
            shapeElement = make_unique<EllipseSVG>();
        }
        else if (element.type == "line") {
            shapeElement = make_unique<LineSVG>();
        }
        else if (element.type == "polygon") {
            shapeElement = make_unique<PolygonSVG>();
        }
        else if (element.type == "path") {
            shapeElement = make_unique<PathSVG>();
        }
        else if (element.type == "g") {
            shapeElement = make_unique<GroupSVG>();
        }
        if (shapeElement) {
            shapeElement->parseShapeSVG(element, 1, 0);
            shapes.push_back(move(shapeElement));
            shapes.back()->getPointMINMAX(ptMM);
        }
    }


    PointF p;
    p.X = (ptMM.pointMin.X + ptMM.pointMax.X) / 2 + 10;
    p.Y = (ptMM.pointMin.Y + ptMM.pointMax.Y) / 2 + 10;

    graphics.TranslateTransform(p.X, p.Y);

    rotationTransform.Rotate(rotate_angle);
    graphics.MultiplyTransform(&rotationTransform);

    graphics.TranslateTransform(-p.X, -p.Y);

    for (const auto& shape : shapes) {
        shape->drawSVG(graphics);
    }
    graphics.ResetTransform();


}

void DrawSVGContent(Graphics& graphics) {
    int widthOnScreen = static_cast<int>((maxX - minX) * width / 322.0f);
    int heightOnScreen = static_cast<int>((maxY - minY) * height / 626.0f);

  
    RectF viewBoxRect(minX, minY, maxX - minX, maxY - minY);
    RectF screenRect(0, 0, static_cast<float>(widthOnScreen), static_cast<float>(heightOnScreen));

  
    SolidBrush brush(Color(255, 255, 255, 255));
    graphics.FillRectangle(&brush, screenRect);

}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    vector<SVGElement> elements = parseSVG(filename, width, height, minX, minY, maxX, maxY);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("GettingStarted");

    RegisterClass(&wndClass);
    if (width != CW_USEDEFAULT)
        width += 17;
    if (height != CW_USEDEFAULT)
        height += 20;

    hWnd = CreateWindow(
        TEXT("GettingStarted"),   // window class name
        TEXT("SVG Demo"),  // window caption
        WS_OVERLAPPEDWINDOW,      // window style
        CW_USEDEFAULT,            // initial x position
        CW_USEDEFAULT,            // initial y position
        // CW_USEDEFAULT,            // initial x size
         //CW_USEDEFAULT,            // initial y size
        width,
        height,
        NULL,                     // parent window handle
        NULL,                     // window menu handle
        hInstance,                // program instance handle
        NULL);                    // creation parameters


    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}  // WinMain

void HandleButtonClick(HWND hWnd, POINT point)
{
    RECT buttonRect;
    GetClientRect(hWnd, &buttonRect);
    buttonRect.left += 400;
    buttonRect.top += 470;
    buttonRect.right = 500;
    buttonRect.bottom = 500;

    // Check if the button is clicked
    if (PtInRect(&buttonRect, point))
    {
        // Toggle the button state
        isButtonClicked = !isButtonClicked;
        // Toggle the scrollbar visibility
        isScrollBarVisible = !isScrollBarVisible;
        // Show or hide the scrollbar
        ShowWindow(g_hScrollBar, isScrollBarVisible ? SW_SHOWNORMAL : SW_HIDE);

        // Invalidate the entire window to redraw both button and scrollbar
        InvalidateRect(hWnd, NULL, TRUE);
    }
}

void DrawButton(HDC hdc)
{
    RECT rect;
    rect.left = 400;    // Specify the left coordinate of the button
    rect.top = 470;     // Specify the top coordinate of the button
    rect.right = 500;   // Specify the right coordinate of the button
    rect.bottom = 500;  // Specify the bottom coordinate of the button

    // Draw the button
    if (isButtonClicked)
    {
        // Button is clicked, draw it in a different color
        HBRUSH hCustomBrush = CreateSolidBrush(RGB(255, 0, 255));
        // Button is not clicked, draw it in a different color
        FillRect(hdc, &rect, hCustomBrush);
        DeleteObject(hCustomBrush);
    }
    else
    {
        HBRUSH hCustomBrush = CreateSolidBrush(RGB(0, 0, 255));
        // Button is not clicked, draw it in a different color
        FillRect(hdc, &rect, hCustomBrush);
        DeleteObject(hCustomBrush);
    }

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static float zoomFactor = 1.0f;
    static POINT lastMousePos;

    static HDC hdcBuffer = nullptr;
    static HBITMAP hBitmap = nullptr;

    switch (message) {
    case WM_KEYDOWN: {
        if (GetKeyState(VK_LEFT) & 0x8000) {
            rotate_angle -= 5;
            if (rotate_angle < 0) {
                rotate_angle = 0;
            }
        }
        else if (GetKeyState(VK_RIGHT) & 0x8000) {
            rotate_angle += 5;
            if (rotate_angle > 360) {
                rotate_angle = 360;
            }
        }
        InvalidateRect(hWnd, NULL, TRUE);
        UpdateWindow(hWnd);
        break;

    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        int clientWidth = clientRect.right - clientRect.left;
        int clientHeight = clientRect.bottom - clientRect.top;

        int rectWidth = static_cast<int>(50 * zoomFactor);
        int rectHeight = static_cast<int>(50 * zoomFactor);
        int rectX = (clientWidth - rectWidth) / 2;
        int rectY = (clientHeight - rectHeight) / 2;

        if (!hdcBuffer) {
            hdcBuffer = CreateCompatibleDC(hdc);
            hBitmap = CreateCompatibleBitmap(hdc, clientWidth, clientHeight);
            SelectObject(hdcBuffer, hBitmap);
        }

        Graphics graphicsBuffer(hdcBuffer);
        graphicsBuffer.Clear(Color(255, 255, 255, 255)); // Set the background to white
        DrawSVGContent(graphicsBuffer);
        OnPaint(hdcBuffer, zoomFactor);
        BitBlt(hdc, 0, 0, clientWidth, clientHeight, hdcBuffer, 0, 0, SRCCOPY);

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        int delta = GET_WHEEL_DELTA_WPARAM(wParam);

        if (delta > 0)
            zoomFactor *= 1.1f;  // Increase the zoom factor by 10%
        else
            zoomFactor /= 1.1f;  // Decrease the zoom factor by 10%

        // Clear the buffer for next paint
        if (hdcBuffer) {
            Graphics graphicsBuffer(hdcBuffer);
            graphicsBuffer.Clear(Color(255, 255, 255, 255));  // Set the background to white
        }

        // Redraw the window
        InvalidateRect(hWnd, NULL, TRUE);
        UpdateWindow(hWnd);

        break;
    }
    case WM_DESTROY:
    {
        if (hdcBuffer) {
            DeleteDC(hdcBuffer);
            DeleteObject(hBitmap);
        }
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
