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
#include <wingdi.h>
#include <shellapi.h>
#include <d2d1.h>
#include <d2d1svg.h>
#include <wincodec.h>
#include <wincodecsdk.h>

using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

double width = CW_USEDEFAULT, height = CW_USEDEFAULT;
double viewBoxX = CW_USEDEFAULT, viewBoxY = CW_USEDEFAULT, viewBoxWidth = CW_USEDEFAULT, viewBoxHeight = CW_USEDEFAULT;

D2D1_SVG_VIEWBOX viewBox;
float rotate_angle = 0.0f;
string filename = "svg-204.svg";

vector<SVGElement> elements; //= parseSVG(filename, width, height, viewBoxX, viewBoxY, viewBoxWidth, viewBoxHeight);

VOID OnPaint(HDC hdc, float zoomFactor, PAINTSTRUCT ps, int clientWidth, int clientHeight)
{
    Graphics graphics(hdc);
    vector<unique_ptr<ShapeSVG>> shapes;
    vector<Gradient> Gradients;
    for (const SVGElement& element : elements) {
        if (element.type == "lineargradient" || element.type == "radialgradient") {
            parseGradientSVG(Gradients, element);
        }
        if (element.type == "defs") {
            for (const SVGElement& childElement : element.children)
                parseGradientSVG(Gradients, childElement);
        }
    }
    for (SVGElement& element : elements) {
        if (element.type == "lineargradient" || element.type == "radialgradient") {
            parseGradientSVG2(Gradients, element);
        }
        if (element.type == "defs") {
            for (SVGElement& childElement : element.children)
                parseGradientSVG2(Gradients, childElement);
        }
    }
    Stop s;
    for (auto& gradientc : Gradients) {
        if (gradientc.stops.size() == 0) {
            gradientc.stops.push_back(s);
        }
    }
    printGradientSVG(Gradients);
    pointMinMax ptMM;
    for (const SVGElement& element : elements) {
        unique_ptr<ShapeSVG> shapeElement;
        printSVGElement(element);
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
            shapeElement->parseShapeSVG(element, 1, 0, Gradients);
            shapes.push_back(move(shapeElement));
            shapes.back()->getPointMINMAX(ptMM);

        }
    }

    // Initialize zoom and rotation transformations
    Matrix zoomTransform(zoomFactor, 0.0f, 0.0f, zoomFactor, 0.0f, 0.0f);
    Matrix rotationTransform;

    // Apply zoom transformation
    graphics.SetTransform(&zoomTransform);

    PointF p;
    p.X = (ptMM.pointMin.X + ptMM.pointMax.X) / 2 + 10;
    p.Y = (ptMM.pointMin.Y + ptMM.pointMax.Y) / 2 + 10;

    graphics.TranslateTransform(p.X, p.Y);

    rotationTransform.Rotate(rotate_angle);
    graphics.MultiplyTransform(&rotationTransform);

    graphics.TranslateTransform(-p.X, -p.Y);

    if (viewBoxX == CW_USEDEFAULT && width != CW_USEDEFAULT) {// which means there is only viewPort but no viewBox
        Region viewBox(Rect(0, 0, width, height));
        graphics.SetClip(&viewBox, CombineModeReplace);
    }
    // this is for viewBox only
        
    //else if (viewBoxX != CW_USEDEFAULT && width == CW_USEDEFAULT) {  // which means there is viewBox but no viewPort
    //    double scale_x = min(width / viewBoxWidth, height / viewBoxHeight);
    //    double scale_y = scale_x;
    //    double translate_x = width/2 - scale_x * (viewBoxX + viewBoxWidth / 2);
    //    double translate_y = height/2 - scale_y * (viewBoxY + viewBoxHeight / 2);
    //  
    //    graphics.ScaleTransform(scale_x, scale_y);

    //    graphics.TranslateTransform(translate_x, translate_y);
    //}
    else if (viewBoxX != CW_USEDEFAULT && width != CW_USEDEFAULT) { // which means there are viewPort and viewBox
        graphics.ScaleTransform(width / viewBoxWidth, height / viewBoxHeight);
        Region viewBox(Rect(viewBoxX, viewBoxY, viewBoxWidth + 2, viewBoxHeight + 2));
        graphics.SetClip(&viewBox, CombineModeReplace);
    }

    for (const auto& shape : shapes) {
        shape->drawSVG(graphics);
    }
    graphics.ResetTransform();
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR CmdLine, INT iCmdShow)
{

    if (CmdLine && CmdLine[0] != '\0')
    {
        filename = CmdLine;
    }
    elements = parseSVG(filename, width, height, viewBoxX, viewBoxY, viewBoxWidth, viewBoxHeight);

    viewBox.x = viewBoxX;
    viewBox.y = viewBoxY;
    viewBox.width = viewBoxWidth;
    viewBox.height = viewBoxHeight;
    ID2D1SvgElement* pRootElement;
    //pRootElement->SetViewBox(viewBox);
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
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    int horizontal = desktop.right;
    int vertical = desktop.bottom;


    HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDC_SVGDEMO));
    hWnd = CreateWindow(
        TEXT("GettingStarted"),   // window class name
        TEXT("SVG Demo"),  // window caption
        WS_OVERLAPPEDWINDOW,      // window style
        CW_USEDEFAULT,            // initial x position
        CW_USEDEFAULT,            // initial y position
        horizontal,
        vertical,
        NULL,                     // parent window handle
        hMenu,                     // window menu handle
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


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static float zoomFactor = 1.0f;
    static POINT lastMousePos;

    static HDC hdcBuffer = nullptr;
    static HBITMAP hBitmap = nullptr;
    PAINTSTRUCT ps;
    switch (message) {
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDM_ABOUT: {
            LPCWSTR content = L"A PRODUCT FROM TEAM1.CORP";
            LPCWSTR exWindowName = L"ABOUT US";
            MessageBox(hWnd, content, exWindowName, MB_OK | MB_ICONINFORMATION);
            break;
        }
        case IDM_EXIT:
            SendMessage(hWnd, WM_CLOSE, 0, 0);
            break;
        case ID_ZOOM_ZOOMIN:
            zoomFactor *= 1.5f;
            break;

        case ID_ZOOM_ZOOMOUT:
            zoomFactor /= 1.5f;
            break;

        case ID_ROTATE_ROTATERIGHT32775:
            rotate_angle += 5;
            if (rotate_angle > 360) {
                rotate_angle = 360;
            }
            break;

        case ID_ROTATE_ROTATELEFT:
            rotate_angle -= 5;
            if (rotate_angle < 0) {
                rotate_angle = 0;
            }
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
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
        OnPaint(hdcBuffer, zoomFactor, ps, clientWidth, clientHeight);
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
        /*    RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);*/

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
