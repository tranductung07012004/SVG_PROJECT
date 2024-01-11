#include "stdafx.h"
#include "classSVG.h"

void bezier_arc_svg(double x0, double y0, double rx, double ry, double angle, bool large_arc_flag,
    bool sweep_flag, double x2, double y2, double& start_angle, double& sweep_angle, double& cx, double& cy)
{
    double pi = atan(1) * 4;
    bool m_radii_ok = true;

    if (rx < 0.0) rx = -rx;
    if (ry < 0.0) ry = -rx;

    // Calculate the middle point between the current and the final points

    double dx2 = (x0 - x2) / 2.0;
    double dy2 = (y0 - y2) / 2.0;

    double cos_a = std::cos(angle);
    double sin_a = std::sin(angle);

    // Calculate (x1, y1)

    double x1 = cos_a * dx2 + sin_a * dy2;
    double y1 = -sin_a * dx2 + cos_a * dy2;

    // Ensure radii are large enough

    double prx = rx * rx;
    double pry = ry * ry;
    double px1 = x1 * x1;
    double py1 = y1 * y1;

    // Check that radii are large enough

    double radii_check = px1 / prx + py1 / pry;
    if (radii_check > 1.0)
    {
        rx = std::sqrt(radii_check) * rx;
        ry = std::sqrt(radii_check) * ry;
        prx = rx * rx;
        pry = ry * ry;
        if (radii_check > 10.0) m_radii_ok = false;
    }

    // Calculate (cx1, cy1)

    double sign = (large_arc_flag == sweep_flag) ? -1.0 : 1.0;
    double sq = (prx * pry - prx * py1 - pry * px1) / (prx * py1 + pry * px1);
    double coef = sign * std::sqrt((sq < 0) ? 0 : sq);
    double cx1 = coef * ((rx * y1) / ry);
    double cy1 = coef * -((ry * x1) / rx);

    // Calculate (cx, cy) from (cx1, cy1)

    double sx2 = (x0 + x2) / 2.0;
    double sy2 = (y0 + y2) / 2.0;
    cx = sx2 + (cos_a * cx1 - sin_a * cy1);
    cy = sy2 + (sin_a * cx1 + cos_a * cy1);

    // Calculate the start_angle (angle1) and the sweep_angle (dangle)

    double ux = (x1 - cx1) / rx;
    double uy = (y1 - cy1) / ry;
    double vx = (-x1 - cx1) / rx;
    double vy = (-y1 - cy1) / ry;
    double p, n;

    // Calculate the angle start

    n = std::sqrt(ux * ux + uy * uy);
    p = ux; // (1 * ux) + (0 * uy)
    sign = (uy < 0) ? -1.0 : 1.0;
    double v = p / n;
    if (v < -1.0) v = -1.0;
    if (v > 1.0) v = 1.0;
    start_angle = sign * std::acos(v);

    // Calculate the sweep angle

    n = std::sqrt((ux * ux + uy * uy) * (vx * vx + vy * vy));
    p = ux * vx + uy * vy;
    sign = (ux * vy - uy * vx < 0) ? -1.0 : 1.0;
    v = p / n;
    if (v < -1.0) v = -1.0;
    if (v > 1.0) v = 1.0;
    sweep_angle = sign * std::acos(v);
    if (!sweep_flag && sweep_angle > 0)
    {
        sweep_angle -= pi * 2.0;
    }
    else
        if (sweep_flag && sweep_angle < 0)
        {
            sweep_angle += pi * 2.0;
        }
}

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
    if (hasGradientFill) {
        if (Gfill.typeGradient == "linearGradient") {
            Color colors[100];
            REAL positions[100];
            for (int i = 0; i < Gfill.stops.size(); i++) {
                colors[i] = Color(Gfill.stops[i].stopOpacity * 255, Gfill.stops[i].stopColor.R, Gfill.stops[i].stopColor.G, Gfill.stops[i].stopColor.B);
                positions[i] = Gfill.stops[i].offset;
            }
            //Color colors[] = { Color(255, 255, 0, 0), Color(255, 255, 255, 0), Color(255, 0, 255, 0) }; // Mảng màu
            //REAL positions[] = { 0.0f, 0.5f, 1.0f }; // Mảng vị trí tương ứng với mỗi màu
            LinearGradientBrush fillBrush(
                Point(p.X, p.Y),      // Start point
                Point((p.X + width), (p.Y + height)),    // End point
                Color(Gfill.stops[0].stopOpacity * 255 * fillOpacity, Gfill.stops[0].stopColor.R, Gfill.stops[0].stopColor.G, Gfill.stops[0].stopColor.B),
                Color(Gfill.stops[Gfill.stops.size() - 1].stopOpacity * 255 * fillOpacity, Gfill.stops[Gfill.stops.size() - 1].stopColor.R, Gfill.stops[Gfill.stops.size() - 1].stopColor.G, Gfill.stops[Gfill.stops.size() - 1].stopColor.B)
            );

            fillBrush.SetInterpolationColors(colors, positions, Gfill.stops.size() - 1);
            graphics.FillRectangle(&fillBrush, (int)p.X, (int)p.Y, width, height);
        }
        else if (Gfill.typeGradient == "radialgradient") {
            Color colors[100];
            REAL positions[100];
            GraphicsPath path;
            for (int i = 0; i < Gfill.stops.size(); i++) {
                colors[i] = Color(Gfill.stops[i].stopOpacity * 255, Gfill.stops[i].stopColor.R, Gfill.stops[i].stopColor.G, Gfill.stops[i].stopColor.B);
                positions[i] = Gfill.stops[i].offset;
            }
            PathGradientBrush gradientBrush(&path);

            //REAL positions[] = { 0.0f, 10.0f };
            int count1 = Gfill.stops.size();


            gradientBrush.SetSurroundColors(colors, &count1);
            gradientBrush.SetInterpolationColors(colors, positions, Gfill.stops.size());
            // Vẽ hình dạng gradient lên màn hình
            graphics.FillRectangle(&gradientBrush, (int)p.X, (int)p.Y, width, height);
        }
    }

    else {

        SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));
        graphics.FillRectangle(&brush, (int)p.X, (int)p.Y, width, height);
    }
    if (hasGradientStroke) {
        if (Gstroke.typeGradient == "linearGradient") {
            Color colors[100];
            REAL positions[100];
            for (int i = 0; i < Gstroke.stops.size(); i++) {
                colors[i] = Color(Gstroke.stops[i].stopOpacity * 255, Gstroke.stops[i].stopColor.R, Gstroke.stops[i].stopColor.G, Gstroke.stops[i].stopColor.B);
                positions[i] = Gstroke.stops[i].offset;
            }
            LinearGradientBrush strokeBrush(
                Point(p.X, p.Y),      // Start point
                Point((p.X + width), (p.Y + height)),    // End point
                Color(Gstroke.stops[0].stopOpacity * 255 * fillOpacity, Gstroke.stops[0].stopColor.R, Gstroke.stops[0].stopColor.G, Gstroke.stops[0].stopColor.B),
                Color(Gstroke.stops[Gstroke.stops.size() - 1].stopOpacity * 255 * fillOpacity, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.R, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.G, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.B)
            );
            strokeBrush.SetInterpolationColors(colors, positions, Gstroke.stops.size() - 1);
            Pen strokePen(&strokeBrush, strokeWidth);
            graphics.DrawRectangle(&strokePen, (int)p.X, (int)p.Y, width, height);
        }
        else if (Gstroke.typeGradient == "radialgradient") {
            Color colors[100];
            REAL positions[100];
            GraphicsPath path;
            for (int i = 0; i < Gstroke.stops.size(); i++) {
                colors[i] = Color(Gstroke.stops[i].stopOpacity * 255, Gstroke.stops[i].stopColor.R, Gstroke.stops[i].stopColor.G, Gstroke.stops[i].stopColor.B);
                positions[i] = Gstroke.stops[i].offset;
            }
            PathGradientBrush strokeBrush(&path);

            // Thiết lập màu sắc cho gradient

            //REAL positions[] = { 0.0f, 10.0f };
            int count1 = Gfill.stops.size();
            //gradientBrush.SetSurroundColors(colors, &count1);
            strokeBrush.SetSurroundColors(colors, &count1);
            strokeBrush.SetInterpolationColors(colors, positions, Gfill.stops.size());
            Pen strokePen(&strokeBrush, strokeWidth);
            // Vẽ hình dạng gradient lên màn hình
            graphics.DrawRectangle(&strokePen, (int)p.X, (int)p.Y, width, height);
        }
    }

    else {
        Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
        graphics.DrawRectangle(&pen, (int)p.X, (int)p.Y, width, height);
    }

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


    if (hasGradientFill) {
        if (Gfill.typeGradient == "linearGradient") {
            Color colors[100];
            REAL positions[100];
            for (int i = 0; i < Gfill.stops.size(); i++) {
                colors[i] = Color(Gfill.stops[i].stopOpacity * 255, Gfill.stops[i].stopColor.R, Gfill.stops[i].stopColor.G, Gfill.stops[i].stopColor.B);
                positions[i] = Gfill.stops[i].offset;
            }
            LinearGradientBrush fillBrush(
                Point(p.X, this->p.Y - this->fontSize),      // Start point
                Point(this->p.X + this->textContent.size() * this->fontSize, p.Y),    // End point
                Color(Gfill.stops[0].stopOpacity * 255 * fillOpacity, Gfill.stops[0].stopColor.R, Gfill.stops[0].stopColor.G, Gfill.stops[0].stopColor.B),
                Color(Gfill.stops[Gfill.stops.size() - 1].stopOpacity * 255 * fillOpacity, Gfill.stops[Gfill.stops.size() - 1].stopColor.R, Gfill.stops[Gfill.stops.size() - 1].stopColor.G, Gfill.stops[Gfill.stops.size() - 1].stopColor.B)
            );
            fillBrush.SetInterpolationColors(colors, positions, Gfill.stops.size() - 1);
            graphics.FillPath(&fillBrush, &path);
        }
        else if (Gfill.typeGradient == "radialgradient") {
            Color colors[100];
            REAL positions[100];
            GraphicsPath path;
            for (int i = 0; i < Gfill.stops.size(); i++) {
                colors[i] = Color(Gfill.stops[i].stopOpacity * 255, Gfill.stops[i].stopColor.R, Gfill.stops[i].stopColor.G, Gfill.stops[i].stopColor.B);
                positions[i] = Gfill.stops[i].offset;
            }
            PathGradientBrush gradientBrush(&path);


            //REAL positions[] = { 0.0f, 10.0f };
            int count1 = Gfill.stops.size();


            gradientBrush.SetSurroundColors(colors, &count1);
            gradientBrush.SetInterpolationColors(colors, positions, Gfill.stops.size());
            // Vẽ hình dạng gradient lên màn hình
            graphics.FillPath(&gradientBrush, &path);
        }
    }
    else {
        SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));
        graphics.FillPath(&brush, &path);
    }
    if (hasGradientStroke) {
        if (Gstroke.typeGradient == "linearGradient") {
            Color colors[100];
            REAL positions[100];
            for (int i = 0; i < Gstroke.stops.size(); i++) {
                colors[i] = Color(Gstroke.stops[i].stopOpacity * 255, Gstroke.stops[i].stopColor.R, Gstroke.stops[i].stopColor.G, Gstroke.stops[i].stopColor.B);
                positions[i] = Gstroke.stops[i].offset;
            }
            LinearGradientBrush strokeBrush(
                Point(p.X, this->p.Y - this->fontSize),      // Start point
                Point(this->p.X + this->textContent.size() * this->fontSize, p.Y),    // End point
                Color(Gstroke.stops[0].stopOpacity * 255 * fillOpacity, Gstroke.stops[0].stopColor.R, Gstroke.stops[0].stopColor.G, Gstroke.stops[0].stopColor.B),
                Color(Gstroke.stops[Gstroke.stops.size() - 1].stopOpacity * 255 * fillOpacity, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.R, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.G, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.B)
            );
            strokeBrush.SetInterpolationColors(colors, positions, Gfill.stops.size() - 1);
            Pen strokePen(&strokeBrush, strokeWidth);
            graphics.DrawPath(&strokePen, &path);
        }
        else if (Gstroke.typeGradient == "radialgradient") {
            Color colors[100];
            REAL positions[100];
            GraphicsPath path;
            for (int i = 0; i < Gstroke.stops.size(); i++) {
                colors[i] = Color(Gstroke.stops[i].stopOpacity * 255, Gstroke.stops[i].stopColor.R, Gstroke.stops[i].stopColor.G, Gstroke.stops[i].stopColor.B);
                positions[i] = Gstroke.stops[i].offset;
            }
            PathGradientBrush strokeBrush(&path);

            // Thiết lập màu sắc cho gradient

            //REAL positions[] = { 0.0f, 10.0f };
            int count1 = Gfill.stops.size();
            //gradientBrush.SetSurroundColors(colors, &count1);
            strokeBrush.SetSurroundColors(colors, &count1);
            strokeBrush.SetInterpolationColors(colors, positions, Gfill.stops.size());
            Pen strokePen(&strokeBrush, strokeWidth);
            // Vẽ hình dạng gradient lên màn hình
            graphics.DrawPath(&strokePen, &path);
        }
    }

    else {
        Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
        graphics.DrawPath(&pen, &path);
    }
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
    RectF ellipseRect(c.X - r, c.Y - r, r * 2, r * 2);

    if (hasGradientFill) {
        if (Gfill.typeGradient == "linearGradient") {
            Color colors[100];
            REAL positions[100];
            for (int i = 0; i < Gfill.stops.size(); i++) {
                colors[i] = Color(Gfill.stops[i].stopOpacity * 255, Gfill.stops[i].stopColor.R, Gfill.stops[i].stopColor.G, Gfill.stops[i].stopColor.B);
                positions[i] = Gfill.stops[i].offset;
            }
            LinearGradientBrush fillBrush(
                Point(this->c.X - this->r, this->c.Y - this->r),      // Start point
                Point(this->c.X + this->r, this->c.Y + this->r),    // End point
                Color(Gfill.stops[0].stopOpacity * 255 * fillOpacity, Gfill.stops[0].stopColor.R, Gfill.stops[0].stopColor.G, Gfill.stops[0].stopColor.B),
                Color(Gfill.stops[Gfill.stops.size() - 1].stopOpacity * 255 * fillOpacity, Gfill.stops[Gfill.stops.size() - 1].stopColor.R, Gfill.stops[Gfill.stops.size() - 1].stopColor.G, Gfill.stops[Gfill.stops.size() - 1].stopColor.B)
            );
            fillBrush.SetInterpolationColors(colors, positions, Gfill.stops.size() - 1);
            graphics.FillEllipse(&fillBrush, ellipseRect);
        }
        else if (Gfill.typeGradient == "radialgradient") {
            Color colors[100];
            REAL positions[100];
            GraphicsPath path;
            for (int i = 0; i < Gfill.stops.size(); i++) {
                colors[i] = Color(Gfill.stops[i].stopOpacity * 255, Gfill.stops[i].stopColor.R, Gfill.stops[i].stopColor.G, Gfill.stops[i].stopColor.B);
                positions[i] = Gfill.stops[i].offset;
            }
            PathGradientBrush gradientBrush(&path);


            //REAL positions[] = { 0.0f, 10.0f };
            int count1 = Gfill.stops.size();


            gradientBrush.SetSurroundColors(colors, &count1);
            gradientBrush.SetInterpolationColors(colors, positions, Gfill.stops.size());
            // Vẽ hình dạng gradient lên màn hình
            graphics.FillEllipse(&gradientBrush, ellipseRect);
        }
    }
    else {
        SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));
        graphics.FillEllipse(&brush, ellipseRect);
    }
    if (hasGradientStroke) {
        if (Gstroke.typeGradient == "linearGradient") {
            Color colors[100];
            REAL positions[100];
            for (int i = 0; i < Gstroke.stops.size(); i++) {
                colors[i] = Color(Gstroke.stops[i].stopOpacity * 255, Gstroke.stops[i].stopColor.R, Gstroke.stops[i].stopColor.G, Gstroke.stops[i].stopColor.B);
                positions[i] = Gstroke.stops[i].offset;
            }
            LinearGradientBrush strokeBrush(
                Point(this->c.X - this->r, this->c.Y - this->r),      // Start point
                Point(this->c.X + this->r, this->c.Y + this->r),    // End point
                Color(Gstroke.stops[0].stopOpacity * 255 * fillOpacity, Gstroke.stops[0].stopColor.R, Gstroke.stops[0].stopColor.G, Gstroke.stops[0].stopColor.B),
                Color(Gstroke.stops[Gstroke.stops.size() - 1].stopOpacity * 255 * fillOpacity, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.R, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.G, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.B)
            );
            strokeBrush.SetInterpolationColors(colors, positions, Gstroke.stops.size() - 1);

            Pen strokePen(&strokeBrush, strokeWidth);

            graphics.DrawEllipse(&strokePen, ellipseRect);
        }
        else if (Gstroke.typeGradient == "radialgradient") {
            Color colors[100];
            REAL positions[100];
            GraphicsPath path;
            for (int i = 0; i < Gstroke.stops.size(); i++) {
                colors[i] = Color(Gstroke.stops[i].stopOpacity * 255, Gstroke.stops[i].stopColor.R, Gstroke.stops[i].stopColor.G, Gstroke.stops[i].stopColor.B);
                positions[i] = Gstroke.stops[i].offset;
            }
            PathGradientBrush strokeBrush(&path);

            // Thiết lập màu sắc cho gradient

            //REAL positions[] = { 0.0f, 10.0f };
            int count1 = Gfill.stops.size();
            //gradientBrush.SetSurroundColors(colors, &count1);
            strokeBrush.SetSurroundColors(colors, &count1);
            strokeBrush.SetInterpolationColors(colors, positions, Gfill.stops.size());
            Pen strokePen(&strokeBrush, strokeWidth);
            // Vẽ hình dạng gradient lên màn hình
            graphics.DrawEllipse(&strokePen, ellipseRect);
        }
    }

    else {
        Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
        graphics.DrawEllipse(&pen, ellipseRect);
    }
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
    RectF ellipseRect(c.X - rx, c.Y - ry, rx * 2, ry * 2);
    if (hasGradientFill) {
        if (Gfill.typeGradient == "linearGradient") {
            Color colors[100];
            REAL positions[100];
            for (int i = 0; i < Gfill.stops.size(); i++) {
                colors[i] = Color(Gfill.stops[i].stopOpacity * 255, Gfill.stops[i].stopColor.R, Gfill.stops[i].stopColor.G, Gfill.stops[i].stopColor.B);
                positions[i] = Gfill.stops[i].offset;
            }
            LinearGradientBrush fillBrush(
                Point(this->c.X - this->rx, this->c.Y - this->ry),      // Start point
                Point((this->c.X - this->rx) + 2 * rx, (this->c.Y - this->ry) + 2 * ry),    // End point
                Color(Gfill.stops[0].stopOpacity * 255 * fillOpacity, Gfill.stops[0].stopColor.R, Gfill.stops[0].stopColor.G, Gfill.stops[0].stopColor.B),
                Color(Gfill.stops[Gfill.stops.size() - 1].stopOpacity * 255 * fillOpacity, Gfill.stops[Gfill.stops.size() - 1].stopColor.R, Gfill.stops[Gfill.stops.size() - 1].stopColor.G, Gfill.stops[Gfill.stops.size() - 1].stopColor.B)
            );
            fillBrush.SetInterpolationColors(colors, positions, Gfill.stops.size() - 1);

            graphics.FillEllipse(&fillBrush, ellipseRect);
        }
        else if (Gfill.typeGradient == "radialgradient") {
            Color colors[100];
            REAL positions[100];
            GraphicsPath path;
            for (int i = 0; i < Gfill.stops.size(); i++) {
                colors[i] = Color(Gfill.stops[i].stopOpacity * 255, Gfill.stops[i].stopColor.R, Gfill.stops[i].stopColor.G, Gfill.stops[i].stopColor.B);
                positions[i] = Gfill.stops[i].offset;
            }
            PathGradientBrush gradientBrush(&path);


            //REAL positions[] = { 0.0f, 10.0f };
            int count1 = Gfill.stops.size();


            gradientBrush.SetSurroundColors(colors, &count1);
            gradientBrush.SetInterpolationColors(colors, positions, Gfill.stops.size());
            // Vẽ hình dạng gradient lên màn hình
            graphics.FillEllipse(&gradientBrush, ellipseRect);
        }
    }
    else {
        SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));
        graphics.FillEllipse(&brush, ellipseRect);
    }
    if (hasGradientStroke) {
        if (Gstroke.typeGradient == "radialGradient") {
            Color colors[100];
            REAL positions[100];
            for (int i = 0; i < Gstroke.stops.size(); i++) {
                colors[i] = Color(Gstroke.stops[i].stopOpacity * 255, Gstroke.stops[i].stopColor.R, Gstroke.stops[i].stopColor.G, Gstroke.stops[i].stopColor.B);
                positions[i] = Gstroke.stops[i].offset;
            }
            LinearGradientBrush strokeBrush(
                Point(this->c.X - this->rx, this->c.Y - this->ry),      // Start point
                Point((this->c.X - this->rx) + 2 * rx, (this->c.Y - this->ry) + 2 * ry),    // End point
                Color(Gstroke.stops[0].stopOpacity * 255 * fillOpacity, Gstroke.stops[0].stopColor.R, Gstroke.stops[0].stopColor.G, Gstroke.stops[0].stopColor.B),
                Color(Gstroke.stops[Gstroke.stops.size() - 1].stopOpacity * 255 * fillOpacity, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.R, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.G, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.B)
            );
            strokeBrush.SetInterpolationColors(colors, positions, Gstroke.stops.size() - 1);

            Pen strokePen(&strokeBrush, strokeWidth);

            graphics.DrawEllipse(&strokePen, ellipseRect);
        }
        else if (Gstroke.typeGradient == "radialgradient") {
            Color colors[100];
            REAL positions[100];
            GraphicsPath path;
            for (int i = 0; i < Gstroke.stops.size(); i++) {
                colors[i] = Color(Gstroke.stops[i].stopOpacity * 255, Gstroke.stops[i].stopColor.R, Gstroke.stops[i].stopColor.G, Gstroke.stops[i].stopColor.B);
                positions[i] = Gstroke.stops[i].offset;
            }
            PathGradientBrush strokeBrush(&path);

            // Thiết lập màu sắc cho gradient

            //REAL positions[] = { 0.0f, 10.0f };
            int count1 = Gfill.stops.size();
            //gradientBrush.SetSurroundColors(colors, &count1);
            strokeBrush.SetSurroundColors(colors, &count1);
            strokeBrush.SetInterpolationColors(colors, positions, Gfill.stops.size());
            Pen strokePen(&strokeBrush, strokeWidth);
            // Vẽ hình dạng gradient lên màn hình
            graphics.DrawEllipse(&strokePen, ellipseRect);
        }
    }

    else {
        Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
        graphics.DrawEllipse(&pen, ellipseRect);
    }
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
    PointF point1((REAL)p1.X, (REAL)p1.Y);
    PointF point2(p2.X, p2.Y);
    pointMinMax p;
    this->getPointMINMAX(p);
    if (hasGradientStroke) {
        if (Gstroke.typeGradient == "linearGradient") {
            Color colors[100];
            REAL positions[100];
            for (int i = 0; i < Gstroke.stops.size(); i++) {
                colors[i] = Color(Gstroke.stops[i].stopOpacity * 255, Gstroke.stops[i].stopColor.R, Gstroke.stops[i].stopColor.G, Gstroke.stops[i].stopColor.B);
                positions[i] = Gstroke.stops[i].offset;
            }
            LinearGradientBrush strokeBrush(
                Point(p.pointMin.X, p.pointMin.Y),      // Start point
                Point(p.pointMax.X, p.pointMax.Y),    // End point
                Color(Gstroke.stops[0].stopOpacity * 255 * fillOpacity, Gstroke.stops[0].stopColor.R, Gstroke.stops[0].stopColor.G, Gstroke.stops[0].stopColor.B),
                Color(Gstroke.stops[Gstroke.stops.size() - 1].stopOpacity * 255 * fillOpacity, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.R, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.G, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.B)
            );
            strokeBrush.SetInterpolationColors(colors, positions, Gfill.stops.size() - 1);

            Pen strokePen(&strokeBrush, strokeWidth);

            graphics.DrawLine(&strokePen, point1, point2);
        }
        else if (Gstroke.typeGradient == "radialgradient") {
            Color colors[100];
            REAL positions[100];
            GraphicsPath path;
            for (int i = 0; i < Gstroke.stops.size(); i++) {
                colors[i] = Color(Gstroke.stops[i].stopOpacity * 255, Gstroke.stops[i].stopColor.R, Gstroke.stops[i].stopColor.G, Gstroke.stops[i].stopColor.B);
                positions[i] = Gstroke.stops[i].offset;
            }
            PathGradientBrush strokeBrush(&path);

            // Thiết lập màu sắc cho gradient

            //REAL positions[] = { 0.0f, 10.0f };
            int count1 = Gfill.stops.size();
            //gradientBrush.SetSurroundColors(colors, &count1);
            strokeBrush.SetSurroundColors(colors, &count1);
            strokeBrush.SetInterpolationColors(colors, positions, Gfill.stops.size());
            Pen strokePen(&strokeBrush, strokeWidth);
            // Vẽ hình dạng gradient lên màn hình
            graphics.DrawLine(&strokePen, point1, point2);
        }
    }

    else {
        Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
        graphics.DrawLine(&pen, point1, point2);
    }

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

    pointMinMax p;
    this->getPointMINMAX(p);
    if (hasGradientFill) {
        if (Gfill.typeGradient == "linearGradient") {
            Color colors[100];
            REAL positions[100];
            for (int i = 0; i < Gfill.stops.size(); i++) {
                colors[i] = Color(Gfill.stops[i].stopOpacity * 255, Gfill.stops[i].stopColor.R, Gfill.stops[i].stopColor.G, Gfill.stops[i].stopColor.B);
                positions[i] = Gfill.stops[i].offset;
            }
            LinearGradientBrush fillBrush(
                Point(p.pointMin.X, p.pointMin.Y),      // Start point
                Point(p.pointMax.X, p.pointMax.Y),    // End point
                Color(Gfill.stops[0].stopOpacity * 255 * fillOpacity, Gfill.stops[0].stopColor.R, Gfill.stops[0].stopColor.G, Gfill.stops[0].stopColor.B),
                Color(Gfill.stops[Gfill.stops.size() - 1].stopOpacity * 255 * fillOpacity, Gfill.stops[Gfill.stops.size() - 1].stopColor.R, Gfill.stops[Gfill.stops.size() - 1].stopColor.G, Gfill.stops[Gfill.stops.size() - 1].stopColor.B)
            );
            fillBrush.SetInterpolationColors(colors, positions, Gfill.stops.size() - 1);

            graphics.FillPolygon(&fillBrush, point, size, FillModeWinding);
        }
        else if (Gfill.typeGradient == "radialgradient") {
            Color colors[100];
            REAL positions[100];
            GraphicsPath path;
            for (int i = 0; i < Gfill.stops.size(); i++) {
                colors[i] = Color(Gfill.stops[i].stopOpacity * 255, Gfill.stops[i].stopColor.R, Gfill.stops[i].stopColor.G, Gfill.stops[i].stopColor.B);
                positions[i] = Gfill.stops[i].offset;
            }
            PathGradientBrush gradientBrush(&path);


            //REAL positions[] = { 0.0f, 10.0f };
            int count1 = Gfill.stops.size();


            gradientBrush.SetSurroundColors(colors, &count1);
            gradientBrush.SetInterpolationColors(colors, positions, Gfill.stops.size());
            // Vẽ hình dạng gradient lên màn hình
            graphics.FillPolygon(&gradientBrush, point, size, FillModeWinding);
        }
    }
    else {
        SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));
        graphics.FillPolygon(&brush, point, size, FillModeWinding);
    }
    if (hasGradientStroke) {
        if (Gstroke.typeGradient == "radialGradient") {
            Color colors[100];
            REAL positions[100];
            for (int i = 0; i < Gstroke.stops.size(); i++) {
                colors[i] = Color(Gstroke.stops[i].stopOpacity * 255, Gstroke.stops[i].stopColor.R, Gstroke.stops[i].stopColor.G, Gstroke.stops[i].stopColor.B);
                positions[i] = Gstroke.stops[i].offset;
            }
            LinearGradientBrush strokeBrush(
                Point(p.pointMin.X, p.pointMin.Y),      // Start point
                Point(p.pointMax.X, p.pointMax.Y),    // End point
                Color(Gstroke.stops[0].stopOpacity * 255 * fillOpacity, Gstroke.stops[0].stopColor.R, Gstroke.stops[0].stopColor.G, Gstroke.stops[0].stopColor.B),
                Color(Gstroke.stops[Gstroke.stops.size() - 1].stopOpacity * 255 * fillOpacity, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.R, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.G, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.B)
            );
            strokeBrush.SetInterpolationColors(colors, positions, Gstroke.stops.size() - 1);

            Pen strokePen(&strokeBrush, strokeWidth);

            graphics.DrawPolygon(&strokePen, point, size);
        }
        else if (Gstroke.typeGradient == "radialgradient") {
            Color colors[100];
            REAL positions[100];
            GraphicsPath path;
            for (int i = 0; i < Gstroke.stops.size(); i++) {
                colors[i] = Color(Gstroke.stops[i].stopOpacity * 255, Gstroke.stops[i].stopColor.R, Gstroke.stops[i].stopColor.G, Gstroke.stops[i].stopColor.B);
                positions[i] = Gstroke.stops[i].offset;
            }
            PathGradientBrush strokeBrush(&path);

            // Thiết lập màu sắc cho gradient

            //REAL positions[] = { 0.0f, 10.0f };
            int count1 = Gfill.stops.size();
            //gradientBrush.SetSurroundColors(colors, &count1);
            strokeBrush.SetSurroundColors(colors, &count1);
            strokeBrush.SetInterpolationColors(colors, positions, Gfill.stops.size());
            Pen strokePen(&strokeBrush, strokeWidth);
            // Vẽ hình dạng gradient lên màn hình
            graphics.DrawPolygon(&strokePen, point, size);
        }
    }

    else {
        Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
        graphics.DrawPolygon(&pen, point, size);
    }

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


    pointMinMax p;
    this->getPointMINMAX(p);
    if (hasGradientFill) {
        if (Gfill.typeGradient == "linearGradient") {
            Color colors[100];
            REAL positions[100];
            for (int i = 0; i < Gfill.stops.size(); i++) {
                colors[i] = Color(Gfill.stops[i].stopOpacity * 255, Gfill.stops[i].stopColor.R, Gfill.stops[i].stopColor.G, Gfill.stops[i].stopColor.B);
                positions[i] = Gfill.stops[i].offset;
            }
            LinearGradientBrush fillBrush(
                Point(p.pointMin.X, p.pointMin.Y),      // Start point
                Point(p.pointMax.X, p.pointMax.Y),    // End point
                Color(Gfill.stops[0].stopOpacity * 255 * fillOpacity, Gfill.stops[0].stopColor.R, Gfill.stops[0].stopColor.G, Gfill.stops[0].stopColor.B),
                Color(Gfill.stops[Gfill.stops.size() - 1].stopOpacity * 255 * fillOpacity, Gfill.stops[Gfill.stops.size() - 1].stopColor.R, Gfill.stops[Gfill.stops.size() - 1].stopColor.G, Gfill.stops[Gfill.stops.size() - 1].stopColor.B)
            );
            fillBrush.SetInterpolationColors(colors, positions, Gfill.stops.size() - 1);
            graphics.FillPolygon(&fillBrush, point, size);
        }
        else if (Gfill.typeGradient == "radialgradient") {
            Color colors[100];
            REAL positions[100];
            GraphicsPath path;
            for (int i = 0; i < Gfill.stops.size(); i++) {
                colors[i] = Color(Gfill.stops[i].stopOpacity * 255, Gfill.stops[i].stopColor.R, Gfill.stops[i].stopColor.G, Gfill.stops[i].stopColor.B);
                positions[i] = Gfill.stops[i].offset;
            }
            PathGradientBrush gradientBrush(&path);


            //REAL positions[] = { 0.0f, 10.0f };
            int count1 = Gfill.stops.size();


            gradientBrush.SetSurroundColors(colors, &count1);
            gradientBrush.SetInterpolationColors(colors, positions, Gfill.stops.size());
            // Vẽ hình dạng gradient lên màn hình
            graphics.FillPolygon(&gradientBrush, point, size);
        }
    }
    else {
        SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));
        graphics.FillPolygon(&brush, point, size);
    }
    if (hasGradientStroke) {
        if (Gstroke.typeGradient == "linearGradient") {
            Color colors[100];
            REAL positions[100];
            for (int i = 0; i < Gstroke.stops.size(); i++) {
                colors[i] = Color(Gstroke.stops[i].stopOpacity * 255, Gstroke.stops[i].stopColor.R, Gstroke.stops[i].stopColor.G, Gstroke.stops[i].stopColor.B);
                positions[i] = Gstroke.stops[i].offset;
            }
            LinearGradientBrush strokeBrush(
                Point(p.pointMin.X, p.pointMin.Y),      // Start point
                Point(p.pointMax.X, p.pointMax.Y),    // End point
                Color(Gstroke.stops[0].stopOpacity * 255 * fillOpacity, Gstroke.stops[0].stopColor.R, Gstroke.stops[0].stopColor.G, Gstroke.stops[0].stopColor.B),
                Color(Gstroke.stops[Gstroke.stops.size() - 1].stopOpacity * 255 * fillOpacity, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.R, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.G, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.B)
            );
            strokeBrush.SetInterpolationColors(colors, positions, Gstroke.stops.size() - 1);

            Pen strokePen(&strokeBrush, strokeWidth);

            graphics.DrawLines(&strokePen, point, size);
        }
        else if (Gstroke.typeGradient == "radialgradient") {
            Color colors[100];
            REAL positions[100];
            GraphicsPath path;
            for (int i = 0; i < Gstroke.stops.size(); i++) {
                colors[i] = Color(Gstroke.stops[i].stopOpacity * 255, Gstroke.stops[i].stopColor.R, Gstroke.stops[i].stopColor.G, Gstroke.stops[i].stopColor.B);
                positions[i] = Gstroke.stops[i].offset;
            }
            PathGradientBrush strokeBrush(&path);

            // Thiết lập màu sắc cho gradient

            //REAL positions[] = { 0.0f, 10.0f };
            int count1 = Gfill.stops.size();
            //gradientBrush.SetSurroundColors(colors, &count1);
            strokeBrush.SetSurroundColors(colors, &count1);
            strokeBrush.SetInterpolationColors(colors, positions, Gfill.stops.size());
            Pen strokePen(&strokeBrush, strokeWidth);
            // Vẽ hình dạng gradient lên màn hình
            graphics.DrawLines(&strokePen, point, size);
        }
    }

    else {
        Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
        graphics.DrawLines(&pen, point, size);
    }
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
    GraphicsPath path1;
    GraphicsPath path2;
    char typeBefore = NULL;
    int isCurve = 0;
    bool check = 1;
    Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
    SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));
    for (auto& data : PathData) {
        if (data.typePointPath == 'M') {
            if (data.points.size() == 1) {
                if (data.points[0].Y != -FLT_MAX) {
                    path.StartFigure();
                    PointF startPoint(static_cast<float>(data.points[0].X), static_cast<float>(data.points[0].Y));
                    start = startPoint;
                    start2 = startPoint;
                }
                else break;
            }
            else {
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
            isCurve = 0;
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
            isCurve = 0;
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
            isCurve = 0;
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
            isCurve = 0;
        }
        else if (data.typePointPath == 'H') {
            for (const auto& point : data.points) {
                PointF endPoint(static_cast<float>(point.X), static_cast<float>(start.Y));
                path.AddLine(start, endPoint);
                start = endPoint;
            }
            isCurve = 0;
        }
        else if (data.typePointPath == 'h') {
            for (const auto& point : data.points) {
                PointF endPoint(static_cast<float>(point.X) + start.X, static_cast<float>(start.Y));
                path.AddLine(start, endPoint);
                start = endPoint;
            }
            isCurve = 0;
        }
        else if (data.typePointPath == 'V') {
            for (const auto& point : data.points) {
                PointF endPoint(static_cast<float>(start.X), static_cast<float>(point.Y));
                path.AddLine(start, endPoint);
                start = endPoint;
            }
            isCurve = 0;
        }
        else if (data.typePointPath == 'v') {
            for (const auto& point : data.points) {
                PointF endPoint(static_cast<float>(start.X), static_cast<float>(point.Y) + start.Y);
                path.AddLine(start, endPoint);
                start = endPoint;
            }
            isCurve = 0;
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
                    control = controlPoint1;
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
                    }
                    else {
                        check = 0;
                        break;
                    }
                }
                if (check == 0) break;
                if (data.points.size() % 3 != 0) break;
            }
            isCurve = 2;
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
                    }
                    else {
                        check = 0;
                        break;
                    }
                }
                if (check == 0) break;
                if (data.points.size() % 3 != 0) break;
            }
            isCurve = 2;
        }
        else if (data.typePointPath == 'S') {
            // Draw a smooth cubic Bezier curve
            if (data.points.size() < 2) break;
            if (data.points.size() == 2) {
                if (isCurve == 2) {
                    PointF controlPoint2(static_cast<float>(data.points[0].X), static_cast<float>(data.points[0].Y));
                    PointF endPoint(static_cast<float>(data.points[1].X), static_cast<float>(data.points[1].Y));
                    PointF controlPoint1;
                    controlPoint1.Y = start.Y * 2 - controlPoint.Y;
                    controlPoint1.X = start.X * 2 - controlPoint.X;
                    int count = path.GetPointCount();
                    path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint2;
                    isCurve = 2;
                }
                else {
                    PointF controlPoint2(static_cast<float>(data.points[0].X), static_cast<float>(data.points[0].Y) + 10);
                    PointF endPoint(static_cast<float>(data.points[1].X), static_cast<float>(data.points[1].Y));
                    PointF controlPoint1;
                    controlPoint1.X = start.X;
                    controlPoint1.Y = start.Y;

                    path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint2;
                    isCurve = 2;
                }
            }
            else if (data.points.size() > 2) {
                for (int i = 0; i < (data.points.size()) / 2 * 2; i += 2)
                    if (isCurve == 2) {
                        PointF controlPoint2(static_cast<float>(data.points[i].X), static_cast<float>(data.points[i].Y));
                        PointF endPoint(static_cast<float>(data.points[i + 1].X), static_cast<float>(data.points[i + 1].Y));
                        PointF controlPoint1;
                        controlPoint1.X = start.X * 2 - controlPoint.X;
                        controlPoint1.Y = start.Y * 2 - controlPoint.Y;
                        int count = path.GetPointCount();
                        path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                        start = endPoint;
                        controlPoint = controlPoint2;
                        isCurve = 2;
                    }
                    else {
                        PointF controlPoint2(static_cast<float>(data.points[i].X), static_cast<float>(data.points[i].Y) + 10);
                        PointF endPoint(static_cast<float>(data.points[i + 1].X), static_cast<float>(data.points[i + 1].Y));
                        PointF controlPoint1;
                        controlPoint1.X = start.X;
                        controlPoint1.Y = start.Y;
                        path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                        start = endPoint;
                        controlPoint = controlPoint2;
                        isCurve = 2;
                    }
                if (data.points.size() % 2 != 0) break;
            }
        }
        else if (data.typePointPath == 's') {
            // Draw a smooth cubic Bezier curve
            if (data.points.size() < 2) break;
            if (data.points.size() == 2) {
                if (isCurve == 2) {
                    PointF controlPoint2(static_cast<float>(data.points[0].X) + start.X, static_cast<float>(data.points[0].Y) + start.Y);
                    PointF endPoint(static_cast<float>(data.points[1].X) + start.X, static_cast<float>(data.points[1].Y) + start.Y);
                    PointF controlPoint1;
                    controlPoint1.Y = start.Y * 2 - controlPoint.Y;
                    controlPoint1.X = start.X * 2 - controlPoint.X;
                    int count = path.GetPointCount();
                    path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint2;
                    isCurve = 2;
                }
                else {
                    PointF controlPoint2(static_cast<float>(data.points[0].X) + start.X, static_cast<float>(data.points[0].Y) + start.Y);
                    PointF endPoint(static_cast<float>(data.points[1].X) + start.X, static_cast<float>(data.points[1].Y) + start.Y);
                    PointF controlPoint1;
                    controlPoint1.X = start.X;
                    controlPoint1.Y = start.Y;
                    path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint2;
                    isCurve = 2;
                }
            }
            else if (data.points.size() > 2) {
                for (int i = 0; i < (data.points.size()) / 2 * 2; i += 2)
                    if (isCurve == 2) {
                        PointF controlPoint2(static_cast<float>(data.points[i].X) + start.X, static_cast<float>(data.points[i].Y) + start.Y);
                        PointF endPoint(static_cast<float>(data.points[i + 1].X) + start.X, static_cast<float>(data.points[i + 1].Y) + start.Y);
                        PointF controlPoint1;
                        controlPoint1.X = start.X * 2 - controlPoint.X;
                        controlPoint1.Y = start.Y * 2 - controlPoint.Y;
                        int count = path.GetPointCount();
                        path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                        start = endPoint;
                        controlPoint = controlPoint2;
                        isCurve = 2;
                    }
                    else {
                        PointF controlPoint2(static_cast<float>(data.points[i].X) + start.X, static_cast<float>(data.points[i].Y) + start.Y);
                        PointF endPoint(static_cast<float>(data.points[i + 1].X) + start.X, static_cast<float>(data.points[i + 1].Y) + start.Y);
                        PointF controlPoint1;
                        controlPoint1.X = start.X;
                        controlPoint1.Y = start.Y;
                        path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                        start = endPoint;
                        controlPoint = controlPoint2;
                        isCurve = 2;
                    }
                if (data.points.size() % 2 != 0) break;
            }
        }
        else if (data.typePointPath == 'Q') {
            if (data.points.size() < 2) break;
            else if (data.points.size() >= 2) {
                for (int i = 0; i < (data.points.size()) / 2 * 2; i += 2) {
                    PointF controlPoint2(static_cast<float>(data.points[i].X), static_cast<float>(data.points[i].Y));
                    PointF endPoint(static_cast<float>(data.points[i + 1].X), static_cast<float>(data.points[i + 1].Y));
                    //path.AddBezier(start, controlPoint2, controlPoint2, endPoint);
                    PointF bezierControl = PointF((start.X + 2 * controlPoint2.X) / 3, (start.Y + 2 * controlPoint2.Y) / 3);
                    PointF bezierControl2 = PointF((endPoint.X + 2 * controlPoint2.X) / 3, (endPoint.Y + 2 * controlPoint2.Y) / 3);
                    path.AddBezier(start, bezierControl, bezierControl2, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint2;
                    isCurve = 1;
                }
            }
            if (data.points.size() % 2 != 0) break;
        }
        else if (data.typePointPath == 'q') {
            if (data.points.size() < 2) break;
            else if (data.points.size() >= 2) {
                for (int i = 0; i < (data.points.size()) / 2 * 2; i += 2) {
                    PointF controlPoint2(static_cast<float>(data.points[i].X) + start.X, static_cast<float>(data.points[i].Y) + start.Y);
                    PointF endPoint(static_cast<float>(data.points[i + 1].X) + start.X, static_cast<float>(data.points[i + 1].Y) + start.Y);
                    PointF bezierControl = PointF((start.X + 2 * controlPoint2.X) / 3, (start.Y + 2 * controlPoint2.Y) / 3);
                    PointF bezierControl2 = PointF((endPoint.X + 2 * controlPoint2.X) / 3, (endPoint.Y + 2 * controlPoint2.Y) / 3);
                    path.AddBezier(start, bezierControl, bezierControl2, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint2;
                    isCurve = 1;
                }
            }
            if (data.points.size() % 2 != 0) break;
        }
        else if (data.typePointPath == 'T') {
            // Draw a smooth quadratic Bezier curve
            for (int i = 0; i < (data.points.size()); i++) {
                if (isCurve == 1) {
                    {
                        PointF endPoint(static_cast<float>(data.points[i].X), static_cast<float>(data.points[i].Y));
                        PointF controlPoint2;
                        controlPoint2.X = start.X * 2 - controlPoint.X;
                        controlPoint2.Y = start.Y * 2 - controlPoint.Y;
                        PointF bezierControl = PointF((start.X + 2 * controlPoint2.X) / 3, (start.Y + 2 * controlPoint2.Y) / 3);
                        PointF bezierControl2 = PointF((endPoint.X + 2 * controlPoint2.X) / 3, (endPoint.Y + 2 * controlPoint2.Y) / 3);
                        path.AddBezier(start, bezierControl, bezierControl2, endPoint);
                        start = endPoint;
                        controlPoint = controlPoint2;
                        isCurve = 1;
                    }
                }
                else {
                    PointF endPoint(static_cast<float>(data.points[i].X), static_cast<float>(data.points[i].Y));
                    PointF controlPoint1;
                    controlPoint1.X = start.X;
                    controlPoint1.Y = start.Y;
                    path.AddBezier(start, controlPoint1, controlPoint1, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint1;
                    control = controlPoint1;
                    isCurve = 1;
                }
            }

        }
        else if (data.typePointPath == 't') {
            // Draw a smooth quadratic Bezier curve
            for (int i = 0; i < (data.points.size()); i++) {
                if (isCurve == 1) {
                    PointF endPoint(static_cast<float>(data.points[i].X + start.X), static_cast<float>(data.points[i].Y + start.Y));
                    PointF controlPoint2;
                    controlPoint2.X = start.X * 2 - controlPoint.X;
                    controlPoint2.Y = start.Y * 2 - controlPoint.Y;
                    PointF bezierControl = PointF((start.X + 2 * controlPoint2.X) / 3, (start.Y + 2 * controlPoint2.Y) / 3);
                    PointF bezierControl2 = PointF((endPoint.X + 2 * controlPoint2.X) / 3, (endPoint.Y + 2 * controlPoint2.Y) / 3);
                    path.AddBezier(start, bezierControl, bezierControl2, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint2;
                    control = controlPoint2;
                    isCurve = 1;
                }
                else {
                    PointF endPoint(static_cast<float>(data.points[i].X + start.X), static_cast<float>(data.points[i].Y + start.Y));
                    PointF controlPoint1;
                    controlPoint1.X = start.X;
                    controlPoint1.Y = start.Y;
                    path.AddBezier(start, controlPoint1, controlPoint1, endPoint);
                    start = endPoint;
                    controlPoint = controlPoint1;
                    control = controlPoint1;
                    isCurve = 1;
                }
            }
        }
        else if (data.typePointPath == 'A') {
            for (auto& Ash : data.As) {
                double cx, cy;
                double start_angle, sweep_angle;

                bezier_arc_svg(start.X, start.Y, Ash.rx, Ash.ry, Ash.xAxisRotation, Ash.largeArcFlag,
                    Ash.sweepFlag, Ash.x, Ash.y, start_angle, sweep_angle, cx, cy);


                RectF bounds(cx - Ash.rx, cy - Ash.ry, 2 * Ash.rx, 2 * Ash.ry);

                // Vẽ đoạn cung
                start_angle = start_angle * (180.0f / pi);
                sweep_angle = sweep_angle * (180.0f / pi);
                path.AddArc(bounds, start_angle, sweep_angle);
                start.X = Ash.x;
                start.Y = Ash.y;
                isCurve = 0;
            }
        }

        else if (data.typePointPath == 'a') {
            for (auto& Ash : data.As) {
                Ash.x += start.X;
                Ash.y += start.Y;
                double cx, cy;
                double start_angle, sweep_angle;

                bezier_arc_svg(start.X, start.Y, Ash.rx, Ash.ry, Ash.xAxisRotation, Ash.largeArcFlag,
                    Ash.sweepFlag, Ash.x, Ash.y, start_angle, sweep_angle, cx, cy);


                RectF bounds(cx - Ash.rx, cy - Ash.ry, 2 * Ash.rx, 2 * Ash.ry);

                // Vẽ đoạn cung
                start_angle = start_angle * (180.0f / pi);
                sweep_angle = sweep_angle * (180.0f / pi);
                path.AddArc(bounds, start_angle, sweep_angle);
                start.X = Ash.x;
                start.Y = Ash.y;
                isCurve = 0;
            }
        }
        else if (data.typePointPath == 'Z' || data.typePointPath == 'z') {
            start = start2;
            path.CloseFigure();
        }

    }
    pointMinMax p;
    this->getPointMINMAX(p);
    if (hasGradientFill) {
        if (Gfill.typeGradient == "lineargradient") {
            Color colors[100];
            REAL positions[100];
            for (int i = 0; i < Gfill.stops.size(); i++) {
                colors[i] = Color(Gfill.stops[i].stopOpacity * 255, Gfill.stops[i].stopColor.R, Gfill.stops[i].stopColor.G, Gfill.stops[i].stopColor.B);
                positions[i] = Gfill.stops[i].offset;
            }
            LinearGradientBrush fillBrush(
                Point(p.pointMin.X, p.pointMin.Y),      // Start point
                Point(p.pointMax.X, p.pointMax.Y),    // End point
                Color(Gfill.stops[0].stopOpacity * 255 * fillOpacity, Gfill.stops[0].stopColor.R, Gfill.stops[0].stopColor.G, Gfill.stops[0].stopColor.B),
                Color(Gfill.stops[Gfill.stops.size() - 1].stopOpacity * 255 * fillOpacity, Gfill.stops[Gfill.stops.size() - 1].stopColor.R, Gfill.stops[Gfill.stops.size() - 1].stopColor.G, Gfill.stops[Gfill.stops.size() - 1].stopColor.B)
            );
            fillBrush.SetInterpolationColors(colors, positions, Gfill.stops.size());
            graphics.FillPath(&fillBrush, &path);
        }
        else if (Gfill.typeGradient == "radialgradient") {
            Color colors[100];
            REAL positions[100];
            for (int i = 0; i < Gfill.stops.size(); i++) {
                colors[i] = Color(Gfill.stops[i].stopOpacity * 255, Gfill.stops[i].stopColor.R, Gfill.stops[i].stopColor.G, Gfill.stops[i].stopColor.B);
                positions[i] = Gfill.stops[i].offset;
            }
            PathGradientBrush gradientBrush(&path);


            //REAL positions[] = { 0.0f, 10.0f };
            int count1 = Gfill.stops.size();


            gradientBrush.SetSurroundColors(colors, &count1);
            gradientBrush.SetInterpolationColors(colors, positions, Gfill.stops.size());
            // Vẽ hình dạng gradient lên màn hình
            graphics.FillPath(&gradientBrush, &path);
        }
    }
    else {
        SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));
        graphics.FillPath(&brush, &path);
    }
    if (hasGradientStroke) {
        if (Gstroke.typeGradient == "lineargradient") {
            Color colors[100];
            REAL positions[100];
            for (int i = 0; i < Gstroke.stops.size(); i++) {
                colors[i] = Color(Gstroke.stops[i].stopOpacity * 255, Gstroke.stops[i].stopColor.R, Gstroke.stops[i].stopColor.G, Gstroke.stops[i].stopColor.B);
                positions[i] = Gstroke.stops[i].offset;
            }
            LinearGradientBrush strokeBrush(
                Point(p.pointMin.X, p.pointMin.Y),      // Start point
                Point(p.pointMax.X, p.pointMax.Y),    // End point
                Color(Gstroke.stops[0].stopOpacity * 255 * fillOpacity, Gstroke.stops[0].stopColor.R, Gstroke.stops[0].stopColor.G, Gstroke.stops[0].stopColor.B),
                Color(Gstroke.stops[Gstroke.stops.size() - 1].stopOpacity * 255 * fillOpacity, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.R, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.G, Gstroke.stops[Gstroke.stops.size() - 1].stopColor.B)
            );
            strokeBrush.SetInterpolationColors(colors, positions, Gstroke.stops.size());
            Pen strokePen(&strokeBrush, strokeWidth);

            graphics.DrawPath(&strokePen, &path);

        }
        else if (Gstroke.typeGradient == "radialgradient") {
            Color colors[100];
            REAL positions[100];
            for (int i = 0; i < Gstroke.stops.size(); i++) {
                colors[i] = Color(Gstroke.stops[i].stopOpacity * 255, Gstroke.stops[i].stopColor.R, Gstroke.stops[i].stopColor.G, Gstroke.stops[i].stopColor.B);
                positions[i] = Gstroke.stops[i].offset;
            }
            PathGradientBrush strokeBrush(&path);

            // Thiết lập màu sắc cho gradient

            //REAL positions[] = { 0.0f, 10.0f };
            int count1 = Gfill.stops.size();
            //gradientBrush.SetSurroundColors(colors, &count1);
            strokeBrush.SetSurroundColors(colors, &count1);
            strokeBrush.SetInterpolationColors(colors, positions, Gfill.stops.size());
            Pen strokePen(&strokeBrush, strokeWidth);
            // Vẽ hình dạng gradient lên màn hình
            graphics.DrawPath(&strokePen, &path);
        }
    }

    else {
        Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
        graphics.DrawPath(&pen, &path);
    }
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