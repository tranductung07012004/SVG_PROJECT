#pragma once
#include "stdafx.h"
#include "rapidxml.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <string>
#include <fstream>
#include <regex>
#include "SVGDemo.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <locale>
#include <codecvt>
#include <unordered_map>.
#include <cmath>
#include "FunctionRead.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")



struct transformSVG {
    float translateX, translateY;
    float rotateAngle;
    float scaleX, scaleY;
    string transformType;
};
void parsetransformSVG(vector<transformSVG>& transformations, const string& input);


struct RGBSVG {
    double R = 0, G = 0, B = 0;
};
RGBSVG colorSVG(const string& s, bool &c);

struct pointMinMax {
    PointF pointMin = { DBL_MAX,DBL_MAX }, pointMax = { -DBL_MAX,-DBL_MAX };
};


class ShapeSVG {
protected:    
    RGBSVG fill;
    double fillOpacity = 1;
    RGBSVG stroke;    
    double strokeOpacity = 1;
    double strokeWidth = 1;
    vector<transformSVG> tfSVG;
    string style = "";
    bool inGroup = 0;
    double dx = 0, dy = 0;
public:
    virtual void parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) = 0;
    void parseDataSVG(string attribute, string data, bool& cf, bool& cs);
    virtual void getPointMINMAX(pointMinMax& ptMM) = 0;
    virtual void drawSVG(Graphics& graphics) = 0;
    void copyAttributes(const ShapeSVG& other);
    void CheckinGroup() {
        inGroup = 1;
    }
};


class RectSVG : public ShapeSVG {
private:
    PointF p = { 0,0 };
    double width = 0;
    double height = 0;
    double rx = 0;
    double ry = 0;

public:
    void parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) override;
    void drawSVG(Graphics& graphics) override;

    void TranslateRectangle(Graphics& graphics, float dx, float dy) {
        graphics.TranslateTransform(dx, dy);
    }
    void ScaleRectangle(Graphics& graphics, float x, float y) {
        graphics.ScaleTransform(x, y);
    }

    void RotateRect(Graphics& graphics, float angleDegrees) {

        graphics.TranslateTransform(0, 0);
        graphics.RotateTransform(angleDegrees);

    }
    void getPointMINMAX(pointMinMax&) override;
};

class TextSVG : public ShapeSVG {
private:
    PointF p = { 0,0 };
    double fontSize = 0;
    int fontWeight1 = 0;
    string fontWeight2 = "";
    string fontStyle = "";
    string fontFamily = "Times New Roman";
    string textAnchor = "";
    string textDecoration = "";
    string textTransform = "";
    string textContent = "";
public:
    void parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) override;
    void drawSVG(Graphics& graphics) override;

    void TranslateText(Graphics& graphics, float dx, float dy) {
        graphics.TranslateTransform(dx, dy);
    }
    void ScaleText(Graphics& graphics, float x, float y) {

        graphics.ScaleTransform(x, y);

    }
    void RotateText(Graphics& graphics, float angleDegrees) {

        graphics.TranslateTransform(0, 0);
        graphics.RotateTransform(angleDegrees);

    }
    void getPointMINMAX(pointMinMax&) override;
};

class CircleSVG : public ShapeSVG {
private:
    PointF c = { 0,0 };
    double r = 0;
public:
    void parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) override;
    void drawSVG(Graphics& graphics) override;

    void TranslateCircle(Graphics& graphics, float dx, float dy) {
        graphics.TranslateTransform(dx, dy);
    }
    void ScaleCircle(Graphics& graphics, float x, float y) {
        graphics.ScaleTransform(x, y);
    }

    void RotateCircle(Graphics& graphics, float angleDegrees) {

        graphics.TranslateTransform(0, 0);
        graphics.RotateTransform(angleDegrees);


    }
    void getPointMINMAX(pointMinMax&) override;
};

class EllipseSVG : public ShapeSVG {
private:
    PointF c = { 0,0 };
    double rx = 0, ry = 0;
public:
    void parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) override;
    void drawSVG(Graphics& graphics) override;

    void TranslateEllipse(Graphics& graphics, float dx, float dy) {
        graphics.TranslateTransform(dx, dy);
    }
    void ScaleEllipse(Graphics& graphics, float x, float y) {

        graphics.ScaleTransform(x, y);

    }

    void RotateEllipse(Graphics& graphics, float angleDegrees) {
        graphics.TranslateTransform(0, 0);
        graphics.RotateTransform(angleDegrees);
    }

    void getPointMINMAX(pointMinMax&) override;
};

class LineSVG : public ShapeSVG {
private:
    PointF p1 = { 0,0 }, p2 = { 0,0 };
public:
    void parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) override;
    void drawSVG(Graphics& graphics) override;
    void TranslateLine(Graphics& graphics, float dx, float dy) {
        graphics.TranslateTransform(dx, dy);
    }
    void ScaleLine(Graphics& graphics, float x, float y) {

        graphics.ScaleTransform(x, y);
    }

    void RotateLine(Graphics& graphics, float angleDegrees) {
        graphics.TranslateTransform(0, 0);
        graphics.RotateTransform(angleDegrees);
    }
    void getPointMINMAX(pointMinMax&) override;
};

vector<PointF> parsePointString(const string& input);

class PolygonSVG : public ShapeSVG {
private:
    vector<PointF> points;
public:
    void parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) override;
    void drawSVG(Graphics& graphics) override;

    void TranslatePolygon(Graphics& graphics, float dx, float dy) {
        graphics.TranslateTransform(dx, dy);

    }
    void ScalePolygon(Graphics& graphics, float x, float y) {
        graphics.ScaleTransform(x, y);
    }

    void RotatePolygon(Graphics& graphics, float angleDegrees) {
        graphics.TranslateTransform(0, 0);
        graphics.RotateTransform(angleDegrees);
    }
    void getPointMINMAX(pointMinMax&) override;
};

class PolylineSVG : public ShapeSVG {
private:

    vector<PointF> points;
public:
    void parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) override;
    void drawSVG(Graphics& graphics) override;

    void TranslatePolyline(Graphics& graphics, float dx, float dy) {
        graphics.TranslateTransform(dx, dy);

    }
    void ScalePolyline(Graphics& graphics, float x, float y) {
        graphics.ScaleTransform(x, y);
    }

    void RotatePolyline(Graphics& graphics, float angleDegrees) {
        graphics.TranslateTransform(0, 0);
        graphics.RotateTransform(angleDegrees);
    }
    void getPointMINMAX(pointMinMax&) override;
};


struct PointPathSVG {
    char typePointPath;
    vector<PointF> points;
    double rx = 0, ry = 0, x = 0, y = 0;
    double xAxisRotation = 0;
    bool largeArcFlag = 0, sweepFlag = 0;
};

class PathSVG : public ShapeSVG {
private:
    vector<PointPathSVG> PathData;
public:
    void parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) override;
    void drawSVG(Graphics& graphics) override;

    void TranslatePath(Graphics& graphics, float dx, float dy) {
        /*for (int i = 0; i < PathData.size(); i++) {
            for (int j = 0; j < PathData[i].points.size(); j++) {
                PathData[i].points[j].x += dx;
                PathData[i].points[j].y += dy;
            }
        }*/
        graphics.TranslateTransform(dx, dy);
    }
    void ScalePath(Graphics& graphics, float x, float y) {
        /*for (int i = 0; i < PathData.size(); i++) {
            for (int j = 0; j < PathData[i].points.size(); j++) {
                PathData[i].points[j].x *= x;
                PathData[i].points[j].y *= y;
            }
        }*/
        graphics.ScaleTransform(x, y);
    }

    void RotatePath(Graphics& graphics, float angleDegrees) {
        graphics.TranslateTransform(0, 0);
        graphics.RotateTransform(angleDegrees);
    }
    void getPointMINMAX(pointMinMax&) override;
};
vector<PointPathSVG> parsePathData(const string& input);

class GroupSVG : public ShapeSVG {
private:
    struct GroupOrShape {
        enum Type { GROUP, SHAPE };
        Type type;
        unique_ptr<ShapeSVG> shape;
        unique_ptr<GroupSVG> group;
    };
    vector<GroupOrShape> elements;
public:
    void parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) override;
    void drawSVG(Graphics& graphics) override;
    void getPointMINMAX(pointMinMax& ptMM) override;
};
