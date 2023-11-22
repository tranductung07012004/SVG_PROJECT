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
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

class Math {
public:
    static double RadiansToDegrees(double radians) {
        return radians * (180.0 / 3.14159);
    }
};

struct SVGElement {
    string type;
    map<string, string> attributes;
    string textContent;
    vector<SVGElement> children;
};

struct transformSVG {
    float translateX, translateY;
    float rotateAngle;
    float scaleX, scaleY;
    string transformType;
};

void parsetransformSVG(vector<transformSVG>& transformations, const string& input);

string formatSVGPath(const string& path);
vector<SVGElement> parseSVG(const string& filename);
void parseStyle(const string& s, SVGElement& element);

struct PointSVG {
    double x = 0, y = 0;
};

struct RGBSVG {
    double R = 0, G = 0, B = 0;
};

struct PointPathSVG {
    char typePointPath;
    vector<PointSVG> points;
    double rx = 0, ry = 0, x = 0, y = 0;
    double xAxisRotation = 0;
    bool largeArcFlag = 0, sweepFlag = 0;
};

RGBSVG colorSVG(const string& s);

vector<PointSVG> parsePointString(const string& input);
void parseSVGNode(xml_node<>* node, vector<SVGElement>& elements);
void parseGroupNode(xml_node<>* node, SVGElement& groupElement);
vector<PointPathSVG> parsePathData(const string& input);


struct pointMinMax {
    PointSVG pointMin = { DBL_MAX,DBL_MAX }, pointMax = { -DBL_MAX,-DBL_MAX };
};


class ShapeSVG {
protected:
    double fillOpacity = 1;
    double strokeOpacity = 1;
    RGBSVG fill;
    RGBSVG stroke;
    double strokeWidth = 0;
    vector<transformSVG> tfSVG;
    string style = "";
public:
    virtual void parseShapeSVG(const SVGElement& element) = 0;
    virtual void getPointMINMAX(pointMinMax& ptMM) = 0;
    virtual void drawSVG(Graphics& graphics) = 0;
    void copyAttributes(const ShapeSVG& other);
    void pushpathTrasform(const ShapeSVG& other);

};


class RectSVG : public ShapeSVG {
private:
    PointSVG p;
    double width = 0;
    double height = 0;
    double rx = 0;
    double ry = 0;

public:
    void parseShapeSVG(const SVGElement& element) override;
    void drawSVG(Graphics& graphics) override;

    void TranslateRectangle(Graphics& graphics, float dx, float dy) {
        p.x += dx;
        p.y += dy;
    }
    void ScaleRectangle(Graphics& graphics, float x, float y) {
        p.x *= x;
        p.y *= y;
        width *= x;
        height *= y;
        strokeWidth *= x;
    }
    void ScaleRectangle(Graphics& graphics, float d) {
        p.x *= d;
        p.y *= d;
        width *= d;
        height *= d;
        strokeWidth *= d;
    }
    void RotateRect(Graphics& graphics, float angleDegrees) {
      
        graphics.TranslateTransform(0, 0);
        graphics.RotateTransform(angleDegrees);
        
    }
    void getPointMINMAX(pointMinMax&) override;
};

class TextSVG : public ShapeSVG {
private:
    PointSVG p;
    double fontSize = 0;
    int fontWeight1 = 0;
    string fontWeight2 = "";
    string fontStyle = "";
    string fontFamily = "Times New Roman";
    string textAnchor = "";
    string textDecoration = "";
    string textTransform = "";
    string textContent = "";
    bool checkStroke = 0;
public:
    void parseShapeSVG(const SVGElement& element) override;
    void drawSVG(Graphics& graphics) override;

    void TranslateText(Graphics& graphics, float dx, float dy) {
        float a = p.x + dx;
        float b = p.y + dy;
        p.x = a + (fontSize - p.x);
        p.y = b - fontSize;
    }
    void ScaleText(Graphics& graphics, float x, float y) {
        float a = p.x * x;
        float b = p.y * y;
        p.x = a + (x * fontSize - p.x);
        p.y = b - y * fontSize;
        fontSize *= x;

    }
    void RotateText(Graphics& graphics, float angleDegrees) {
       
        graphics.TranslateTransform(0, 0);
        graphics.RotateTransform(angleDegrees);
     
    }

    void getPointMINMAX(pointMinMax&) override;
};

class CircleSVG : public ShapeSVG {
private:
    PointSVG c;
    double r = 0;
public:
    void parseShapeSVG(const SVGElement& element) override;
    void drawSVG(Graphics& graphics) override;

    void TranslateCircle(Graphics& graphics, float dx, float dy) {
        c.x += dx;
        c.y += dy;
    }
    void ScaleCircle(Graphics& graphics, float x, float y) {
        c.x *= x;
        c.y *= y;
        r *= x;

        strokeWidth *= x;
    }
    void ScaleCircle(Graphics& graphics, float d) {
        c.x *= d;
        c.y *= d;
        r *= d;

        strokeWidth *= d;
    }
    void RotateCircle(Graphics& graphics, float angleDegrees) {
      
        graphics.TranslateTransform(0, 0);
        graphics.RotateTransform(angleDegrees);
      

    }
    void getPointMINMAX(pointMinMax&) override;
};

class EllipseSVG : public ShapeSVG {
private:
    PointSVG c;
    double rx = 0, ry = 0;
public:
    void parseShapeSVG(const SVGElement& element) override;
    void drawSVG(Graphics& graphics) override;

    void TranslateEllipse(Graphics& graphics, float dx, float dy) {
        c.x += dx;
        c.y += dy;
    }
    void ScaleEllipse(Graphics& graphics, float x, float y) {
        c.x *= x;
        c.y *= y;
        rx *= x;
        ry *= y;
        strokeWidth *= y;
    }
    void ScaleEllipse(Graphics& graphics, float d) {
        c.x *= d;
        c.y *= d;
        rx *= d;
        ry *= d;
        strokeWidth *= d;
    }
    void RotateEllipse(Graphics& graphics, float angleDegrees) {
        graphics.TranslateTransform(0, 0);
        graphics.RotateTransform(angleDegrees);
    }

    void getPointMINMAX(pointMinMax&) override;
};

class LineSVG : public ShapeSVG {
private:
    PointSVG p1, p2;
    double width = 0, height = 0, rx = 0, ry = 0;
public:
    void parseShapeSVG(const SVGElement& element) override;
    void drawSVG(Graphics& graphics) override;

    void TranslateLine(Graphics& graphics, float dx, float dy) {
        p1.x += dx;
        p1.y += dy;
        p2.x += dx;
        p2.y += dy;
    }
    void ScaleLine(Graphics& graphics, float x, float y) {
        p1.x *= x;
        p1.y *= y;
        p2.x *= x;
        p2.y *= y;
        strokeWidth *= x;
    }
    void ScaleLine(Graphics& graphics, float d) {
        p1.x *= d;
        p1.y *= d;
        p2.x *= d;
        p2.y *= d;
        strokeWidth *= d;
    }
    void RotateLine(Graphics& graphics, float angleDegrees) {
        graphics.TranslateTransform(0, 0);
        graphics.RotateTransform(angleDegrees);
    }
    void getPointMINMAX(pointMinMax&) override;
};

class PolygonSVG : public ShapeSVG {
private:
    vector<PointSVG> points;
public:
    PolygonSVG() {
        strokeOpacity = 0;
    }
    void parseShapeSVG(const SVGElement& element) override;
    void drawSVG(Graphics& graphics) override;

    void TranslatePolygon(Graphics& graphics, float dx, float dy) {
        for (int i = 0; i < points.size(); i++) {
            points[i].x += dx;
            points[i].y += dy;
        }

    }
    void ScalePolygon(Graphics& graphics, float x, float y) {
        for (int i = 0; i < points.size(); i++) {
            points[i].x *= x;
            points[i].y *= y;
        }
        strokeWidth *= x;
    }
    void ScalePolygon(Graphics& graphics, float d) {
        for (int i = 0; i < points.size(); i++) {
            points[i].x *= d;
            points[i].y *= d;
        }
        strokeWidth *= d;
    }
    void RotatePolygon(Graphics& graphics, float angleDegrees) {
        graphics.TranslateTransform(0, 0);
        graphics.RotateTransform(angleDegrees);
    }
    void getPointMINMAX(pointMinMax&) override;
};

class PolylineSVG : public ShapeSVG {
private:

    vector<PointSVG> points;
public:
    PolylineSVG() {
        strokeOpacity = 0;
    }
    void parseShapeSVG(const SVGElement& element) override;
    void drawSVG(Graphics& graphics) override;

    void TranslatePolyline(Graphics& graphics, float dx, float dy) {
        for (int i = 0; i < points.size(); i++) {
            points[i].x += dx;
            points[i].y += dy;
        }

    }
    void ScalePolyline(Graphics& graphics, float x, float y) {
        for (int i = 0; i < points.size(); i++) {
            points[i].x *= x;
            points[i].y *= y;
        }
        strokeWidth *= x;
    }
    void ScalePolyline(Graphics& graphics, float d) {
        for (int i = 0; i < points.size(); i++) {
            points[i].x *= d;
            points[i].y *= d;
        }
        strokeWidth *= d;
    }
    void RotatePolyline(Graphics& graphics, float angleDegrees) {
        graphics.TranslateTransform(0, 0);
        graphics.RotateTransform(angleDegrees);
    }
    void getPointMINMAX(pointMinMax&) override;
};

class PathSVG : public ShapeSVG {
private:
    vector<PointPathSVG> PathData;
public:
    void parseShapeSVG(const SVGElement& element) override;
    void drawSVG(Graphics& graphics) override;

    void TranslatePath(Graphics& graphics, float dx, float dy) {
        for (int i = 0; i < PathData.size(); i++) {
            for (int j = 0; j < PathData[i].points.size(); j++) {
                PathData[i].points[j].x += dx;
                PathData[i].points[j].y += dy;
            }
        }
    }
    void ScalePath(Graphics& graphics, float x, float y) {
        for (int i = 0; i < PathData.size(); i++) {
            for (int j = 0; j < PathData[i].points.size(); j++) {
                PathData[i].points[j].x *= x;
                PathData[i].points[j].y *= y;
            }
        }
    }
    void ScalePath(Graphics& graphics, float d) {
        for (int i = 0; i < PathData.size(); i++) {
            for (int j = 0; j < PathData[i].points.size(); j++) {
                PathData[i].points[j].x *= d;
                PathData[i].points[j].y *= d;
            }
        }
    }
    void RotatePath(Graphics& graphics, float angleDegrees) {
        graphics.TranslateTransform(0, 0);
        graphics.RotateTransform(angleDegrees);
    }
    void getPointMINMAX(pointMinMax&) override;
};

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
    void parseShapeSVG(const SVGElement& element) override;
    void drawSVG(Graphics& graphics) override;
    void getPointMINMAX(pointMinMax& ptMM) override;

};
