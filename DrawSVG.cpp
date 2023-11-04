#include "readSVGa.h"

void CircleSVG::drawCircleSVG(HDC hdc) {
    Graphics graphics(hdc);
    Pen pen(Color(0, 0, 0, 0));
    RectF ellipseRect(cx, cy, r, r);
    graphics.DrawEllipse(&pen, ellipseRect);
}

void EllipseSVG::drawEllipseSVG(HDC hdc) {
    Graphics graphics(hdc);
    Pen pen(Color(0, 0, 0, 0));
    RectF ellipseRect(cx, cy, rx, ry);
    graphics.DrawEllipse(&pen, ellipseRect);
}

void LineSVG::drawLineSVG(HDC hdc) {
    Graphics graphics(hdc);
    PointF point1(x1, y1);
    PointF point2(x2, y2);
    Pen pen(Color(0, 0, 0, 0));
    graphics.DrawLine(&pen, point1, point2);
}

void PolygonSVG::drawPolygonSVG(HDC hdc) {
    Graphics graphics(hdc);
    Pen pen(Color(0, 0, 0, 0));
    int size = points.size();
    PointF* point = new PointF[size];
    for (int i = 0; i < points.size(); i++) {
        point[i].X = points[i].x;
        point[i].Y = points[i].y;
    }
    graphics.DrawPolygon(&pen, point, points.size());
}

void PolylineSVG::drawPolylineSVG(HDC hdc) {
    Graphics graphics(hdc);
    Pen pen(Color(0, 0, 0, 0));
    PointF* point = new PointF[points.size()];
    for (int i = 0; i < points.size(); i++) {
        point[i].X = points[i].x;
        point[i].Y = points[i].y;
    }
    graphics.DrawLines(&pen, point, points.size());
}

void RectSVG::drawRectSVG(HDC hdc) {
    Graphics graphics(hdc);
    Pen pen(Color(0, 0, 0, 0));
    graphics.DrawRectangle(&pen, (int)x, (int)y, width, height);
}

void TextSVG::drawTextSVG(HDC hdc) {
    Graphics graphics(hdc);
    Pen pen(Color(0, 0, 0, 0));
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring ws = converter.from_bytes(fontFamily);
    FontFamily fontFamily(ws.c_str());

    int font1 = 0;
    if (fontStyle == "bold" || fontStyle == "Bold") {
        font1 = 1;
    }
    else if (fontStyle == "italic" || fontStyle == "Italic") {
        font1 = 2;
    }

    Font font(&fontFamily, fontSize, font1, UnitPixel);
    PointF point(static_cast<float>(x), static_cast<float>(y));
    SolidBrush brush(Color(0, 0, 0, 0));
    graphics.DrawString(ws.c_str(), -1, &font, point, &brush);
}
