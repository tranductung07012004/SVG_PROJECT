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

vector<transformSVG> parsetransformSVG(const string& filename);
string formatSVGPath(const string& path);
vector<SVGElement> parseSVG(const string& filename);
void parseStyle(const string& s, SVGElement& element);

struct PointSVG {
    double x=0, y=0;
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
    void drawSVG(Graphics&) override;
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
public:
    void parseShapeSVG(const SVGElement& element) override;
    void drawSVG(Graphics&) override;
    void getPointMINMAX(pointMinMax&) override;
};

class CircleSVG : public ShapeSVG {
private:
    PointSVG c;
    double r = 0;
public:
    void parseShapeSVG(const SVGElement& element) override;
    void drawSVG(Graphics&) override;
    void getPointMINMAX(pointMinMax&) override;
};

class EllipseSVG : public ShapeSVG {
private:
    PointSVG c;
    double rx = 0, ry = 0;
public:
    void parseShapeSVG(const SVGElement& element) override;
    void drawSVG(Graphics&) override;
    void getPointMINMAX(pointMinMax&) override;

};

class LineSVG : public ShapeSVG {
private:
    PointSVG p1, p2;
    double width = 0, height = 0, rx = 0, ry = 0;
public:
    void parseShapeSVG(const SVGElement& element) override;
    void drawSVG(Graphics&) override;
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
    void drawSVG(Graphics&) override;
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
    void drawSVG(Graphics&) override;
    void getPointMINMAX(pointMinMax&) override;
};

class PathSVG : public ShapeSVG {
private:
    vector<PointPathSVG> PathData;
public:
    void parseShapeSVG(const SVGElement& element) override;
    void drawSVG(Graphics&) override;
    void getPointMINMAX(pointMinMax&) override;
};

class GroupSVG : public ShapeSVG {
private:
    vector<unique_ptr<ShapeSVG>> shapes;
    vector<unique_ptr<GroupSVG>> groups;

public:
    void parseShapeSVG(const SVGElement& element) override;
    void drawSVG(Graphics&) override;
    void getPointMINMAX(pointMinMax& ptMM) override;
};
