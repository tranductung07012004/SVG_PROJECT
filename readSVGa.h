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
using namespace std;
using namespace rapidxml;

struct SVGElement {
    string type;
    map<string, string> attributes;
    string textContent;
};

vector<SVGElement> parseSVG(const string& filename);
void printSVGElements(const vector<SVGElement>& elements);

struct PointSVG {
    double x, y;
};

struct RGBSVG {
    double R = 0, G = 0, B = 0;
};

RGBSVG colorSVG(const string& s);

vector<PointSVG> parsePointString(const string& input);

class ShapeSVG {
protected:
    double fillOpacity = 0;
    double strokeOpacity = 0;
    RGBSVG fill;
    RGBSVG stroke;
    double strokeWidth = 0;
    string transform = "";
    string style = "";
};

class RectSVG : public ShapeSVG {
private:
    double x = 0;
    double y = 0;
    double width = 0;
    double height = 0;
    double rx = 0;
    double ry = 0;

public:
    void parseShapeSVG(const SVGElement& element);
    void drawRectSVG(HDC);

};

class TextSVG : public ShapeSVG {
private:
    double x = 0;
    double y = 0;
    double fontSize = 0;
    double fontWeight = 0;
    string fontStyle = "";
    string fontFamily = "Times New Roman";
    string textAnchor = "";
    string textDecoration = "";
    string textTransform = "";
    string textContent = "";
public:
    void parseShapeSVG(const SVGElement& element);
    void drawTextSVG(HDC);
};

class CircleSVG : public ShapeSVG {
private:
    double strokeWidth = 0;
    double cx = 0;
    double cy = 0;
    double r = 0;
public:
    void parseShapeSVG(const SVGElement& element);
    void drawCircleSVG(HDC);
};

class EllipseSVG : public ShapeSVG {
private:
    double cx = 0;
    double cy = 0;
    double rx = 0, ry = 0;
public:
    void parseShapeSVG(const SVGElement& element);
    void drawEllipseSVG(HDC);

};

class LineSVG : public ShapeSVG {
private:
    double x1 = 0, y1 = 0, x2 = 0, y2 = 0, width = 0, height = 0, rx = 0, ry = 0;
public:
    void parseShapeSVG(const SVGElement& element);
    void drawLineSVG(HDC);
};

class PolygonSVG : public ShapeSVG {
private:
    vector<PointSVG> points;
public:
    void parseShapeSVG(const SVGElement& element);
    void drawPolygonSVG(HDC);
};

class PolylineSVG : public ShapeSVG {
private:
    vector<PointSVG> points;
public:
    void parseShapeSVG(const SVGElement& element);
    void drawPolylineSVG(HDC);
};