#include "stdafx.h"
#include "classSVG.h"

void RectSVG::drawSVG(Graphics& graphics) {
    GraphicsState state = graphics.Save();
    for (const auto& tf : tfSVG) {
        if (tf.transformType == "translate") {
            this->TranslateRectangle(graphics, tf.translateX, tf.translateY);
        }
        else if (tf.transformType == "scale") {
            this->ScaleRectangle(graphics, tf.scaleX, tf.scaleY);
        }
        else if (tf.transformType == "rotate") {
            this->RotateRect(graphics, tf.rotateAngle);
        }
    }
    this->TranslateRectangle(graphics, dx, dy);
    Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
    SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));
    graphics.FillRectangle(&brush, (int)p.X, (int)p.Y, width, height);
    graphics.DrawRectangle(&pen, (int)p.X, (int)p.Y, width, height);

    graphics.Restore(state);
}

void TextSVG::drawSVG(Graphics& graphics) {
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
    GraphicsState state = graphics.Save();
    bool checkScale = 0;
    for (const auto& tf : tfSVG) {
        if (tf.transformType == "translate") {
            this->TranslateText(graphics, tf.translateX, tf.translateY);
        }
        else if (tf.transformType == "scale") {
            this->ScaleText(graphics, tf.scaleX, tf.scaleY);
            checkScale = 1;
        }
        else if (tf.transformType == "rotate") {
            this->RotateText(graphics, tf.rotateAngle);
        }
    }
    this->TranslateText(graphics, dx, dy);
    int font1 = FontStyleRegular;
    if (fontWeight2 == "bold" || fontWeight2 == "Bold" || fontWeight1 >= 550) {
        font1 = FontStyleBold;
    }
    else if (fontStyle == "italic" || fontStyle == "Italic") {
        font1 = FontStyleItalic;
    }
    else if ((fontWeight2 == "bold" || fontWeight2 == "Bold" || fontWeight1 >= 550) && (fontStyle == "italic" || fontStyle == "Italic")) {
        font1 = FontStyleBoldItalic;
    }
    else if (textDecoration == "underline" || textDecoration == "Underline") {
        font1 = FontStyleUnderline;
    }
    else if (textDecoration == "line-through") {
        font1 = FontStyleStrikeout;
    }


    unordered_map <string, float>offset_map = {
        {"start", -0.15 * fontSize},
        {"middle", 0.0f},
        {"end", 0.15 * fontSize}
    };
    unordered_map <string, Gdiplus::StringAlignment> text_anchor_map = {
     { "start", Gdiplus::StringAlignment::StringAlignmentNear },
     { "middle", Gdiplus::StringAlignment::StringAlignmentCenter },
     { "end", Gdiplus::StringAlignment::StringAlignmentFar }
    };


    wstring wstr = wstring(textContent.begin(), textContent.end());
    std::wstring ws = wstring(fontFamily.begin(), fontFamily.end());
    FontFamily fontFamily1(ws.c_str());


    Gdiplus::PointF origin(p.X - 0.09 * fontSize, p.Y - 0.9 * fontSize);
    origin.X += offset_map[textAnchor];
    Gdiplus::StringFormat format(Gdiplus::StringFormat::GenericDefault());
    format.SetAlignment(text_anchor_map[textAnchor]);

    GraphicsPath path;
    path.AddString(wstr.c_str(), -1, &fontFamily1, font1, fontSize, origin, &format);

    SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));
    graphics.FillPath(&brush, &path);
    Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
    graphics.DrawPath(&pen, &path);
    graphics.Restore(state);
}

void CircleSVG::drawSVG(Graphics& graphics) {
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

    GraphicsState state = graphics.Save();
    for (const auto& tf : tfSVG) {
        if (tf.transformType == "translate") {
            this->TranslateCircle(graphics, tf.translateX, tf.translateY);
        }
        else if (tf.transformType == "scale") {
            this->ScaleCircle(graphics, tf.scaleX, tf.scaleY);
        }
        else if (tf.transformType == "rotate") {
            this->RotateCircle(graphics, tf.rotateAngle);
        }
    }
    this->TranslateCircle(graphics, dx, dy);
    Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
    RectF ellipseRect(c.X - r, c.Y - r, r * 2, r * 2);

    SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));

    graphics.FillEllipse(&brush, ellipseRect);
    graphics.DrawEllipse(&pen, ellipseRect);
    graphics.Restore(state);
}

void EllipseSVG::drawSVG(Graphics& graphics) {
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    GraphicsState state = graphics.Save();
    for (const auto& tf : tfSVG) {
        if (tf.transformType == "translate") {

            this->TranslateEllipse(graphics, tf.translateX, tf.translateY);

        }
        else if (tf.transformType == "scale") {

            this->ScaleEllipse(graphics, tf.scaleX, tf.scaleY);

        }
        else if (tf.transformType == "rotate") {
            this->RotateEllipse(graphics, tf.rotateAngle);
        }
    }
    this->TranslateEllipse(graphics, dx, dy);
    Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
    RectF ellipseRect(c.X - rx, c.Y - ry, rx * 2, ry * 2);
    SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));
    graphics.FillEllipse(&brush, ellipseRect);
    graphics.DrawEllipse(&pen, ellipseRect);

    graphics.Restore(state);
}

void LineSVG::drawSVG(Graphics& graphics) {
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    GraphicsState state = graphics.Save();
    for (const auto& tf : tfSVG) {
        if (tf.transformType == "translate") {

            this->TranslateLine(graphics, tf.translateX, tf.translateY);

        }
        else if (tf.transformType == "scale") {
            this->ScaleLine(graphics, tf.scaleX, tf.scaleY);
        }
        else if (tf.transformType == "rotate") {
            this->RotateLine(graphics, tf.rotateAngle);
        }
    }
    this->TranslateLine(graphics, dx, dy);
    Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
    PointF point1((REAL)p1.X, (REAL)p1.Y);
    PointF point2(p2.X, p2.Y);
    graphics.DrawLine(&pen, point1, point2);
    graphics.Restore(state);
}

void PolygonSVG::drawSVG(Graphics& graphics) {
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    GraphicsState state = graphics.Save();
    int size = points.size();
    PointF* point = new PointF[size];

    for (int i = 0; i < size; i++) {
        point[i].X = points[i].X;
        point[i].Y = points[i].Y;
    }
    for (const auto& tf : tfSVG) {
        if (tf.transformType == "translate") {

            this->TranslatePolygon(graphics, tf.translateX, tf.translateY);
        }
        else  if (tf.transformType == "scale") {
            this->ScalePolygon(graphics, tf.scaleX, tf.scaleY);
        }
        else if (tf.transformType == "rotate") {
            this->RotatePolygon(graphics, tf.rotateAngle);
        }
    }
    this->TranslatePolygon(graphics, dx, dy);
    Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
    SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));
    graphics.FillPolygon(&brush, point, size, FillModeWinding);
    graphics.DrawPolygon(&pen, point, size);
    delete[] point;
    graphics.Restore(state);
}

void PolylineSVG::drawSVG(Graphics& graphics) {
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    GraphicsState state = graphics.Save();
    int size = points.size();
    PointF* point = new PointF[size];

    for (int i = 0; i < size; i++) {
        point[i].X = points[i].X;
        point[i].Y = points[i].Y;
    }
    for (const auto& tf : tfSVG) {
        if (tf.transformType == "translate") {
            this->TranslatePolyline(graphics, tf.translateX, tf.translateY);
        }
        if (tf.transformType == "scale") {
            this->ScalePolyline(graphics, tf.scaleX, tf.scaleY);
        }
        else if (tf.transformType == "rotate") {
            this->RotatePolyline(graphics, tf.rotateAngle);
        }
    }
    this->TranslatePolyline(graphics, dx, dy);

    SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));
    graphics.FillPolygon(&brush, point, size);
    Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
    graphics.DrawLines(&pen, point, size);

    delete[] point;
    graphics.Restore(state);
}

void PathSVG::drawSVG(Graphics& graphics) {
    double pi = atan(1) * 4;
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    GraphicsState state = graphics.Save();
    for (const auto& tf : tfSVG) {
        if (tf.transformType == "translate") {
            this->TranslatePath(graphics, tf.translateX, tf.translateY);
        }
        if (tf.transformType == "scale") {
            this->ScalePath(graphics, tf.scaleX, tf.scaleY);
        }
        else if (tf.transformType == "rotate") {
            this->RotatePath(graphics, tf.rotateAngle);
        }
    }
    this->TranslatePath(graphics, dx, dy);
    PointF start = { -FLT_MAX, -FLT_MAX };
    PointF start1 = start;
    PointF start2 = { 0,0 };
    PointF controlPoint;
    PointF control;
    GraphicsPath path;
    char typeBefore = NULL;
    bool check = 1;

    for ( auto& data : PathData) {
        if (data.typePointPath == 'M') {
            if (data.points.size() == 1) {
                if (data.points[0].Y != -FLT_MAX) {
                    // Move to the starting point
                    path.StartFigure();
                    PointF startPoint(static_cast<float>(data.points[0].X), static_cast<float>(data.points[0].Y));
                    start = startPoint;
                    start2 = startPoint;
                }
                else break;
            }
            else {

                // Move to the starting point
                path.StartFigure();
                PointF startPoint(static_cast<float>(data.points[0].X), static_cast<float>(data.points[0].Y));
                start = startPoint;

                for (const auto& point : data.points) {
                    if (point.Y != -FLT_MAX) {
                        PointF endPoint(static_cast<float>(point.X), static_cast<float>(point.Y));
                        path.AddLine(start, endPoint);
                        start = endPoint;
                        start2 = startPoint;
                    }
                    else {
                        check = 0;
                        break;
                    }
                }
                if (check == 0) break;
            }
        }

        else if (data.typePointPath == 'L') {
            // Draw a line
            for (const auto& point : data.points) {
                if (point.Y != -FLT_MAX) {
                    PointF endPoint(static_cast<float>(point.X), static_cast<float>(point.Y));
                    path.AddLine(start, endPoint);
                    start = endPoint;
                }
                else {
                    check = 0;
                    break;
                }
            }
            if (check == 0) break;

        }

        else if (data.typePointPath == 'C') {
            // Draw a Bezier curve
            if (data.points.size() < 3) break;
            if (data.points.size() == 3) {
                if (data.points[2].Y != -FLT_MAX) {
                    PointF controlPoint1(static_cast<float>(data.points[0].X), static_cast<float>(data.points[0].Y));
                    PointF controlPoint2(static_cast<float>(data.points[1].X), static_cast<float>(data.points[1].Y));
                    PointF endPoint(static_cast<float>(data.points[2].X), static_cast<float>(data.points[2].Y));
                    path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint2;
                    typeBefore = 'C';
                }
                else {
                    check = 0;
                    break;
                }
            }
            else if (data.points.size() > 3) {
                for (int i = 0; i < (data.points.size()) / 3 * 3; i += 3) {
                    if (data.points[i + 2].Y != -FLT_MAX) {
                        PointF controlPoint1(static_cast<float>(data.points[i].X), static_cast<float>(data.points[i].Y));
                        PointF controlPoint2(static_cast<float>(data.points[i + 1].X), static_cast<float>(data.points[i + 1].Y));
                        PointF endPoint(static_cast<float>(data.points[i + 2].X), static_cast<float>(data.points[i + 2].Y));
                        path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                        start = endPoint;
                        controlPoint = controlPoint2;
                        typeBefore = 'C';
                    }
                    else {
                        check = 0;
                        break;
                    }
                }
                if (check == 0) break;
                if (data.points.size() % 3 != 0) break;
            }
        }

        else if (data.typePointPath == 'V') {
            for (const auto& point : data.points) {

                PointF endPoint(static_cast<float>(start.X), static_cast<float>(point.Y));
                path.AddLine(start, endPoint);
                start = endPoint;
            }
        }


        else if (data.typePointPath == 'H') {
            for (const auto& point : data.points) {
                PointF endPoint(static_cast<float>(point.X), static_cast<float>(start.Y));
                path.AddLine(start, endPoint);
                start = endPoint;
            }
        }

        else if (data.typePointPath == 'm') {
            if (start.X == start1.X && start.Y == start1.Y) {
                start = { 0,0 };

            }
            if (data.points.size() == 1) {
                if (data.points[0].Y != -FLT_MAX) {
                    // Move to the starting point
                    path.StartFigure();
                    PointF startPoint(static_cast<float>(data.points[0].X) + start.X, static_cast<float>(data.points[0].Y) + start.Y);
                    start = startPoint;
                    start2 = startPoint;
                }
                else break;
            }
            else {
                // Move to the starting point
                path.StartFigure();
                PointF startPoint(static_cast<float>(data.points[0].X) + start.X, static_cast<float>(data.points[0].Y) + start.Y);
                start = startPoint;

                for (int i = 1; i < data.points.size(); i++) {
                    if (data.points[i].Y != -FLT_MAX) {
                        PointF endPoint(static_cast<float>(data.points[i].X) + start.X, static_cast<float>(data.points[i].Y) + start.Y);
                        path.AddLine(start, endPoint);
                        start = endPoint;
                        start2 = startPoint;
                    }
                    else {
                        check = 0;
                        break;
                    }
                }
                if (check == 0) break;
            }
        }

        else if (data.typePointPath == 'l') {
            for (const auto& point : data.points) {
                if (point.Y != -FLT_MAX) {
                    PointF endPoint(static_cast<float>(point.X) + start.X, static_cast<float>(point.Y) + start.Y);
                    path.AddLine(start, endPoint);
                    start = endPoint;
                }
                else {
                    check = 0;
                    break;
                }
            }
            if (check == 0) break;
        }

        else if (data.typePointPath == 'c') {
            if (data.points.size() < 3) break;
            if (data.points.size() == 3) {
                if (data.points[2].Y != -FLT_MAX) {
                    PointF controlPoint1(static_cast<float>(data.points[0].X) + start.X, static_cast<float>(data.points[0].Y) + start.Y);
                    PointF controlPoint2(static_cast<float>(data.points[1].X) + start.X, static_cast<float>(data.points[1].Y) + start.Y);
                    PointF endPoint(static_cast<float>(data.points[2].X) + start.X, static_cast<float>(data.points[2].Y) + start.Y);
                    path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint2;
                    typeBefore = 'c';
                }
                else break;
            }
            else if (data.points.size() > 3) {
                for (int i = 0; i < (data.points.size()) / 3 * 3; i += 3) {
                    if (data.points[i + 2].Y != -FLT_MAX) {
                        PointF controlPoint1(static_cast<float>(data.points[i].X) + start.X, static_cast<float>(data.points[i].Y) + start.Y);
                        PointF controlPoint2(static_cast<float>(data.points[i + 1].X) + start.X, static_cast<float>(data.points[i + 1].Y) + start.Y);
                        PointF endPoint(static_cast<float>(data.points[i + 2].X) + start.X, static_cast<float>(data.points[i + 2].Y) + start.Y);
                        path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                        start = endPoint;
                        controlPoint = controlPoint2;
                        typeBefore = 'c';
                    }
                    else {
                        check = 0;
                        break;
                    }
                }
                if (check == 0) break;
                if (data.points.size() % 3 != 0) break;
            }
        }
        else if (data.typePointPath == 'v') {
            for (const auto& point : data.points) {
                PointF endPoint(static_cast<float>(start.X), static_cast<float>(point.Y) + start.Y);
                path.AddLine(start, endPoint);
                start = endPoint;

            }
        }
        else if (data.typePointPath == 'h') {
            for (const auto& point : data.points) {
                PointF endPoint(static_cast<float>(point.X) + start.X, static_cast<float>(start.Y));
                path.AddLine(start, endPoint);
                start = endPoint;

            }
        }

        else if (data.typePointPath == 'S') {
            // Draw a smooth cubic Bezier curve
            // C + S
            if (data.points.size() >= 2) {
                if (typeBefore == 'C' || typeBefore == 'c') {
                    PointF controlPoint2(static_cast<float>(data.points[0].X), static_cast<float>(data.points[0].Y));
                    PointF endPoint(static_cast<float>(data.points[1].X), static_cast<float>(data.points[1].Y));
                    PointF controlPoint1;
                    controlPoint1.Y = controlPoint2.Y;
                    controlPoint1.X = start.X * 2 - controlPoint.X;
                    int count = path.GetPointCount();
                    path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint2;
                }
                // Q + S
                else if (typeBefore == 'Q' || typeBefore == 'q') {
                    PointF controlPoint2(static_cast<float>(data.points[0].X), static_cast<float>(data.points[0].Y));
                    PointF endPoint(static_cast<float>(data.points[1].X), static_cast<float>(data.points[1].Y));
                    PointF controlPoint1;
                    controlPoint1.Y = controlPoint2.Y;
                    controlPoint1.X = start.X * 2 - controlPoint.X;
                    controlPoint2.Y = (endPoint.Y + controlPoint2.Y) / 2;
                    controlPoint1.Y = (endPoint.Y + controlPoint1.Y) / 2;

                    path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                    //path.AddLine(endPoint, controlPoint2);
                    start = endPoint;
                    controlPoint = controlPoint2;
                }

                //T+S
                else if (typeBefore == 'T' || typeBefore == 't') {
                    PointF controlPoint2(static_cast<float>(data.points[0].X) + 10, static_cast<float>(data.points[0].Y) + 10);
                    PointF endPoint(static_cast<float>(data.points[1].X), static_cast<float>(data.points[1].Y));
                    PointF controlPoint1;
                    controlPoint1.X = controlPoint2.X + 10;
                    controlPoint1.Y = (endPoint.Y + controlPoint2.Y) / 2 + 8;

                    path.AddBezier(start, controlPoint1, controlPoint1, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint2;
                }

                // S 
                else {
                    PointF controlPoint2(static_cast<float>(data.points[0].X), static_cast<float>(data.points[0].Y) + 10);
                    PointF endPoint(static_cast<float>(data.points[1].X), static_cast<float>(data.points[1].Y));
                    PointF controlPoint1;
                    controlPoint1.X = controlPoint2.X;
                    controlPoint1.Y = (endPoint.Y + controlPoint2.Y) / 2 + 8;

                    path.AddBezier(start, controlPoint1, controlPoint1, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint2;
                }
                typeBefore = 'S';
            }
        }
        else if (data.typePointPath == 'Q') {
            //Draw a quadratic Bezier curve
            if (data.points.size() >= 2) {
                PointF controlPoint2(static_cast<float>(data.points[0].X), static_cast<float>(data.points[0].Y));
                PointF controlPoint1 = { (controlPoint2.X + start.X) / 2, (controlPoint2.Y + start.Y) / 2 };
                PointF endPoint(static_cast<float>(data.points[1].X), static_cast<float>(data.points[1].Y));
                PointF controlPoint3 = { controlPoint2.X ,(controlPoint2.Y * 4) };
                path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                // path.AddLine(start, controlPoint1);
                 //path.AddLine(endPoint, controlPoint2);

                start = endPoint;
                controlPoint = controlPoint2;
                control = controlPoint1;
                typeBefore = 'Q';
            }
        }
        else if (data.typePointPath == 'T') {
            // Draw a smooth quadratic Bezier curve

            if (data.points.size() >= 1) {
                // Q + T
                if (typeBefore == 'Q' || typeBefore == 'q') {
                    PointF endPoint(static_cast<float>(data.points[0].X), static_cast<float>(data.points[0].Y));
                    PointF controlPoint1;
                    PointF controlPoint2;
                    controlPoint1.X = start.X * 2 - controlPoint.X;
                    controlPoint1.Y = start.Y * 2 - controlPoint.Y;
                    controlPoint2.X = start.X * 2 - control.X;
                    controlPoint2.Y = start.Y * 2 - control.Y;
                    path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint2;
                    control = controlPoint1;
                    //typeBefore = 'T';
                }

                else if (typeBefore == 'C' || typeBefore == 'c') {
                    //wrong
                    PointF endPoint(static_cast<float>(data.points[0].X), static_cast<float>(data.points[0].Y));
                    PointF controlPoint1;
                    PointF controlPoint2;
                    controlPoint1.X = start.X * 2 - controlPoint.X;
                    controlPoint1.Y = start.Y * 2 - controlPoint.Y;
                    controlPoint2.X = start.X * 2 - control.X;
                    controlPoint2.Y = start.Y * 2 - control.Y;
                    path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint2;
                    control = controlPoint1;
                }

                // T
                else {
                    PointF endPoint(static_cast<float>(data.points[0].X), static_cast<float>(data.points[0].Y));
                    path.AddBezier(start, start, start, endPoint);
                    start = endPoint;
                    controlPoint = endPoint;
                    control = endPoint;
                    //typeBefore = 'T';
                }
                typeBefore = 'T';
            }
        }

        else if (data.typePointPath == 's') {
            // Draw a smooth cubic Bezier curve
            // C + S
            if (data.points.size() >= 2) {
                if (typeBefore == 'C' || typeBefore == 'c') {
                    PointF controlPoint2(static_cast<float>(data.points[0].X) + start.X, static_cast<float>(data.points[0].Y) + start.Y);
                    PointF endPoint(static_cast<float>(data.points[1].X) + start.X, static_cast<float>(data.points[1].Y) + start.Y);
                    PointF controlPoint1;
                    controlPoint1.Y = controlPoint2.Y;
                    controlPoint1.X = start.X * 2 - controlPoint.X;
                    int count = path.GetPointCount();
                    path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint2;
                }
                // Q + S
                else if (typeBefore == 'Q' || typeBefore == 'q') {
                    PointF controlPoint2(static_cast<float>(data.points[0].X) + start.X, static_cast<float>(data.points[0].Y) + start.Y);
                    PointF endPoint(static_cast<float>(data.points[1].X) + start.X, static_cast<float>(data.points[1].Y) + start.Y);
                    PointF controlPoint1;
                    controlPoint1.Y = controlPoint2.Y;
                    controlPoint1.X = start.X * 2 - controlPoint.X;
                    controlPoint2.Y = (endPoint.Y + controlPoint2.Y) / 2;
                    controlPoint1.Y = (endPoint.Y + controlPoint1.Y) / 2;

                    path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                    //path.AddLine(endPoint, controlPoint2);
                    start = endPoint;
                    controlPoint = controlPoint2;
                }

                //T+S
                else if (typeBefore == 'T' || typeBefore == 't') {
                    PointF controlPoint2(static_cast<float>(data.points[0].X) + 10 + start.X, static_cast<float>(data.points[0].Y) + 10 + start.Y);
                    PointF endPoint(static_cast<float>(data.points[1].X) + start.X, static_cast<float>(data.points[1].Y) + start.Y);
                    PointF controlPoint1;
                    controlPoint1.X = controlPoint2.X + 10;
                    controlPoint1.Y = (endPoint.Y + controlPoint2.Y) / 2 + 8;

                    path.AddBezier(start, controlPoint1, controlPoint1, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint2;
                }

                // S 
                else {
                    PointF controlPoint2(static_cast<float>(data.points[0].X) + start.X, static_cast<float>(data.points[0].Y) + 10 + start.Y);
                    PointF endPoint(static_cast<float>(data.points[1].X) + start.X, static_cast<float>(data.points[1].Y) + start.Y);
                    PointF controlPoint1;
                    controlPoint1.X = controlPoint2.X;
                    controlPoint1.Y = (endPoint.Y + controlPoint2.Y) / 2 + 8;

                    path.AddBezier(start, controlPoint1, controlPoint1, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint2;
                }
                typeBefore = 's';
            }
        }
        else if (data.typePointPath == 'q') {
            //Draw a quadratic Bezier curve
            if (data.points.size() >= 2) {
                PointF controlPoint2(static_cast<float>(data.points[0].X) + start.X, static_cast<float>(data.points[0].Y) + start.Y);
                PointF controlPoint1 = { (controlPoint2.X + start.X) / 2 + start.X, (controlPoint2.Y + start.Y) / 2 + start.Y };
                PointF endPoint(static_cast<float>(data.points[1].X) + start.X, static_cast<float>(data.points[1].Y) + start.Y);
                //PointF controlPoint3 = { controlPoint2.X ,(controlPoint2.Y * 4) };
                path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                // path.AddLine(start, controlPoint1);
                 //path.AddLine(endPoint, controlPoint2);

                start = endPoint;
                controlPoint = controlPoint2;
                control = controlPoint1;
                typeBefore = 'q';
            }
        }
        else if (data.typePointPath == 't') {
            // Draw a smooth quadratic Bezier curve

            if (data.points.size() >= 1) {
                // Q + T
                if (typeBefore == 'Q' || typeBefore == 'q') {
                    PointF endPoint(static_cast<float>(data.points[0].X + start.X), static_cast<float>(data.points[0].Y) + start.Y);
                    PointF controlPoint1;
                    PointF controlPoint2;
                    controlPoint1.X = start.X * 2 - controlPoint.X + start.X;
                    controlPoint1.Y = start.Y * 2 - controlPoint.Y + start.Y;
                    controlPoint2.X = start.X * 2 - control.X + start.X;
                    controlPoint2.Y = start.Y * 2 - control.Y + start.Y;
                    path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint2;
                    control = controlPoint1;
                    //typeBefore = 'T';
                }

                else if (typeBefore == 'C' || typeBefore == 'c') {
                    //wrong
                    PointF endPoint(static_cast<float>(data.points[0].X) + start.X, static_cast<float>(data.points[0].Y) + start.Y);
                    PointF controlPoint1;
                    PointF controlPoint2;
                    controlPoint1.X = start.X * 2 - controlPoint.X + start.X;
                    controlPoint1.Y = start.Y * 2 - controlPoint.Y + start.Y;
                    controlPoint2.X = start.X * 2 - control.X + start.X;
                    controlPoint2.Y = start.Y * 2 - control.Y + start.Y;
                    path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint2;
                    control = controlPoint1;
                }

                // T
                else {
                    PointF endPoint(static_cast<float>(data.points[0].X) + start.X, static_cast<float>(data.points[0].Y) + start.Y);
                    path.AddBezier(start, start, start, endPoint);
                    start = endPoint;
                    controlPoint = endPoint;
                    control = endPoint;
                    //typeBefore = 'T';
                }
                typeBefore = 't';
            }
        }

        else if (data.typePointPath == 'A') {
            REAL angle = data.xAxisRotation * (REAL)pi / 180.0f;
            REAL cosAngle = cos(angle);
            REAL sinAngle = sin(angle);
            REAL x1 = cosAngle * (start.X - data.x) / 2.0f + sinAngle * (start.Y - data.y) / 2.0f;
            REAL y1 = -sinAngle * (start.X - data.x) / 2.0f + cosAngle * (start.Y - data.y) / 2.0f;

            REAL rxSq = data.rx * data.rx;
            REAL rySq = data.ry * data.ry;
            REAL x1Sq = x1 * x1;
            REAL y1Sq = y1 * y1;

            // Tính toán các thông số của elip
            REAL radiiCheck = x1Sq / rxSq + y1Sq / rySq;
            if (radiiCheck > 1.0f)
            {
                data.rx *= sqrt(radiiCheck);
                data.ry *= sqrt(radiiCheck);
                rxSq = data.rx * data.rx;
                rySq = data.ry * data.ry;
            }

            REAL denom = rxSq * y1Sq + rySq * x1Sq;
            REAL sqrtDenom = sqrt((rxSq * rySq - denom) / denom);

            REAL cx1 = sqrtDenom * data.rx * y1 / data.ry;
            REAL cy1 = sqrtDenom * -data.ry * x1 / data.rx;

            REAL sx2 = (start.X + data.x) / 2.0f + cosAngle * cx1 - sinAngle * cy1;
            REAL sy2 = (start.Y + data.y) / 2.0f + sinAngle * cx1 + cosAngle * cy1;

            REAL thetaStart = atan2((y1 - cy1) / data.ry, (x1 - cx1) / data.rx);
            REAL thetaEnd = atan2((-y1 - cy1) / data.ry, (-x1 - cx1) / data.rx);

            REAL sweepAngle = thetaEnd - thetaStart;
            if (data.sweepFlag == 0 && sweepAngle > 0)
                sweepAngle -= 2 * (REAL)pi;
            else if (data.sweepFlag == 1 && sweepAngle < 0)
                sweepAngle += 2 * (REAL)pi;

            // Vẽ đường cong elip
            path.AddArc(sx2 - start.X, sy2 - 3 * start.Y, 2 * data.rx - 1/2 *start.X - 10, 2 * data.ry + start.Y, thetaStart * 180.0f / (REAL)pi, sweepAngle * 180.0f / (REAL)pi);
            
           
            start.X = data.x;
            start.Y = data.y;
            typeBefore = 'A';
        }

        else if (data.typePointPath == 'a') {
            data.x += start.X;
            data.y += start.Y;
            REAL angle = data.xAxisRotation * (REAL)pi / 180.0f;
            REAL cosAngle = cos(angle);
            REAL sinAngle = sin(angle);
            REAL x1 = cosAngle * (start.X - data.x) / 2.0f + sinAngle * (start.Y - data.y) / 2.0f;
            REAL y1 = -sinAngle * (start.X - data.x) / 2.0f + cosAngle * (start.Y - data.y) / 2.0f;

            REAL rxSq = data.rx * data.rx;
            REAL rySq = data.ry * data.ry;
            REAL x1Sq = x1 * x1;
            REAL y1Sq = y1 * y1;

            // Tính toán các thông số của elip
            REAL radiiCheck = x1Sq / rxSq + y1Sq / rySq;
            if (radiiCheck > 1.0f)
            {
                data.rx *= sqrt(radiiCheck);
                data.ry *= sqrt(radiiCheck);
                rxSq = data.rx * data.rx;
                rySq = data.ry * data.ry;
            }

            REAL denom = rxSq * y1Sq + rySq * x1Sq;
            REAL sqrtDenom = sqrt((rxSq * rySq - denom) / denom);

            REAL cx1 = sqrtDenom * data.rx * y1 / data.ry;
            REAL cy1 = sqrtDenom * -data.ry * x1 / data.rx;

            REAL sx2 = (start.X + data.x) / 2.0f + cosAngle * cx1 - sinAngle * cy1;
            REAL sy2 = (start.Y + data.y) / 2.0f + sinAngle * cx1 + cosAngle * cy1;

            REAL thetaStart = atan2((y1 - cy1) / data.ry, (x1 - cx1) / data.rx);
            REAL thetaEnd = atan2((-y1 - cy1) / data.ry, (-x1 - cx1) / data.rx);

            REAL sweepAngle = thetaEnd - thetaStart;
            if (data.sweepFlag == 0 && sweepAngle > 0)
                sweepAngle -= 2 * (REAL)pi;
            else if (data.sweepFlag == 1 && sweepAngle < 0)
                sweepAngle += 2 * (REAL)pi;

            // Vẽ đường cong elip
            path.AddArc(sx2 - start.X, sy2 - 3 * start.Y, 2 * data.rx - 1 / 2 * start.X - 10, 2 * data.ry + start.Y, thetaStart * 180.0f / (REAL)pi, sweepAngle * 180.0f / (REAL)pi);


            start.X = data.x;
            start.Y = data.y;
            typeBefore = 'a';
            }

        else if (data.typePointPath == 'Z' || data.typePointPath == 'z') {
            start = start2;
            path.CloseFigure();
          
        }
    }

    Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
    SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));

    graphics.FillPath(&brush, &path);
    graphics.DrawPath(&pen, &path);

    graphics.Restore(state);
}

void GroupSVG::drawSVG(Graphics& graphics) {
    for (auto& element : elements) {
        if (element.type == GroupOrShape::SHAPE) {
            element.shape->drawSVG(graphics);
        }
        else  if (element.type == GroupOrShape::GROUP) {
            element.group->drawSVG(graphics);
        }
    }
}
