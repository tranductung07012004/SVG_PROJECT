#include "stdafx.h"
#include "rapidxml.hpp"
#include "ReadSVG.h"
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


HINSTANCE g_hInstance;
bool isButtonClicked = false;
bool isScrollBarVisible = false;
HWND g_hScrollBar = NULL;
int g_nScrollPos = 0;
float rotate_angle = 0.0f;

VOID OnPaint(HDC hdc, float zoomFactor)
{
    Graphics graphics(hdc);
    const string filename = "sample.svg";
    vector<SVGElement> elements = parseSVG(filename);
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
            shapeElement->parseShapeSVG(element);
            shapes.push_back(move(shapeElement));
            shapes.back()->getPointMINMAX(ptMM);
        }
    }


    PointF p;
    p.X = (ptMM.pointMin.x + ptMM.pointMax.x) / 2 + 10;
    p.Y = (ptMM.pointMin.y + ptMM.pointMax.y) / 2 + 10;

    graphics.TranslateTransform(p.X, p.Y);

    rotationTransform.Rotate(rotate_angle);
    graphics.MultiplyTransform(&rotationTransform);

    graphics.TranslateTransform(-p.X, -p.Y);

    for (const auto& shape : shapes) {
        shape->drawSVG(graphics);
    }
    //graphics.ResetTransform();
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
    case WM_CREATE:
    {
        // Create the scrollbar without showing it initially
        g_hScrollBar = CreateWindowEx(0, L"SCROLLBAR", NULL, WS_CHILD | SBS_HORZ, 400, 440, 100, 20, hWnd, NULL, g_hInstance, NULL);

        // Set the scrollbar range and position
        SetScrollRange(g_hScrollBar, SB_CTL, 0, 360, TRUE);
        SetScrollPos(g_hScrollBar, SB_CTL, g_nScrollPos, TRUE);

        break;
    }
    case WM_LBUTTONDOWN:
    {
        POINT pt;
        pt.x = LOWORD(lParam);
        pt.y = HIWORD(lParam);
        HandleButtonClick(hWnd, pt);
        break;
    }
    case WM_HSCROLL:
    {
        int nScrollCode = LOWORD(wParam);  // Scroll code
        int nPos = HIWORD(wParam);         // New scroll position

        switch (nScrollCode)
        {
        case SB_LINELEFT: {
            g_nScrollPos -= 5;
            rotate_angle -= 5;
            if (rotate_angle < 0) {
                rotate_angle = 0;
            }
            break;
        }
        case SB_LINERIGHT: {
            g_nScrollPos += 5;
            rotate_angle += 5;
            if (rotate_angle > 360) {
                rotate_angle = 360;
            }
            break;
        }
        }
        // Update the scroll position of the scrollbar control
        SetScrollPos(g_hScrollBar, SB_CTL, g_nScrollPos, TRUE);

        // Redraw the window
        InvalidateRect(hWnd, NULL, TRUE);
        //UpdateWindow(hWnd);

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
        OnPaint(hdcBuffer, zoomFactor);
        DrawButton(hdcBuffer);
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
