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


    Gdiplus::PointF origin(p.X - 0.2 * fontSize, p.Y - 0.9 * fontSize);
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
    Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
    SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));
    graphics.FillPolygon(&brush, point, size);
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

    SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));
    graphics.FillPolygon(&brush, point, size);
    Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
    graphics.DrawLines(&pen, point, size);

    delete[] point;
    graphics.Restore(state);
}

void PathSVG::drawSVG(Graphics& graphics) {
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
    PointF start = { -3.4e38,-3.4e38 };
    PointF start1 = start;
    PointF start2 = { 0,0 };
    PointF controlPoint;
    PointF control;
    GraphicsPath path;
    char typeBefore = NULL;

    for (const auto& data : PathData) {
        if (data.typePointPath == 'M') {
            if (data.points.size() == 1) {
                // Move to the starting point
                path.StartFigure();
                PointF startPoint(static_cast<float>(data.points[0].X), static_cast<float>(data.points[0].Y));
                start = startPoint;
                start2 = startPoint;
            }
            else  {
                // Move to the starting point
                path.StartFigure();
                PointF startPoint(static_cast<float>(data.points[0].X), static_cast<float>(data.points[0].Y));
                start = startPoint;
                for (const auto& point : data.points) {
                    PointF endPoint(static_cast<float>(point.X), static_cast<float>(point.Y));
                    path.AddLine(start, endPoint);
                    start = endPoint;
                    start2 = startPoint;
                }
            }
        }
        
        
        else if (data.typePointPath == 'L') {
            // Draw a line
            for (const auto& point : data.points) {
                PointF endPoint(static_cast<float>(point.X), static_cast<float>(point.Y));
                path.AddLine(start, endPoint);
                start = endPoint;
            }
        }
        
        else if (data.typePointPath == 'C') {
            // Draw a Bezier curve
            if (data.points.size() >= 3) {
                PointF controlPoint1(static_cast<float>(data.points[0].X), static_cast<float>(data.points[0].Y));
                PointF controlPoint2(static_cast<float>(data.points[1].X), static_cast<float>(data.points[1].Y));
                PointF endPoint(static_cast<float>(data.points[2].X), static_cast<float>(data.points[2].Y));
                path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                start = endPoint;
                controlPoint = controlPoint2;
                typeBefore = 'C';
            }
        }
        
        else if (data.typePointPath == 'V') {
            for (const auto& point : data.points) {
                PointF endPoint(static_cast<float>(start.X), static_cast<float>(point.Y));
                path.AddLine(start, endPoint);
                start = endPoint;
            }
        }
        

        else if (data.typePointPath == 'H' ) {
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
                // Move to the starting point
                path.StartFigure();
                PointF startPoint(static_cast<float>(data.points[0].X) + start.X, static_cast<float>(data.points[0].Y) + start.Y);
                start = startPoint;
                start2 = startPoint;
            }
            else {
                // Move to the starting point
                path.StartFigure();
                PointF startPoint(static_cast<float>(data.points[0].X) + start.X, static_cast<float>(data.points[0].Y) + start.Y);
                start = startPoint;
                for (int i = 1; i < data.points.size(); i++) {
                    PointF endPoint(static_cast<float>(data.points[i].X) + start.X, static_cast<float>(data.points[i].Y) + start.Y);
                    path.AddLine(start, endPoint);
                    start = endPoint;
                    start2 = startPoint;
                }
            }
        }
        else if (data.typePointPath == 'l') {
            for (const auto& point : data.points) {
                PointF endPoint(static_cast<float>(point.X) + start.X, static_cast<float>(point.Y) + start.Y);
                path.AddLine(start, endPoint);
                start = endPoint;
            }
        }
        else if (data.typePointPath == 'c') {

            if (data.points.size() >= 3) {
                PointF controlPoint1(static_cast<float>(data.points[0].X) + start.X, static_cast<float>(data.points[0].Y) + start.Y);
                PointF controlPoint2(static_cast<float>(data.points[1].X) + start.X, static_cast<float>(data.points[1].Y) + start.Y);
                PointF endPoint(static_cast<float>(data.points[2].X) + start.X, static_cast<float>(data.points[2].Y) + start.Y);
                path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                start = endPoint;
                controlPoint = controlPoint2;
                typeBefore = 'C';
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

        else if (data.typePointPath == 'Z' || data.typePointPath == 'z') {
            //start = start2;

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