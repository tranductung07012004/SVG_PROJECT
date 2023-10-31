#pragma once
#include "FunTionRead.h"

class Polygon {
private:
    float fillOpacity = 0;
    float strokeOpacity = 0;
    string fill = "";
    string stroke = "";
    int strokeWidth = 0;
    vector<Point> points;
public:
    void readLine(const string& line);
    void printPolygon();
};