#pragma once
#include "FunTionRead.h"

class Ellipse {
private:
    float fillOpacity = 0;
    float strokeOpacity = 0;
    string fill = "";
    string stroke = "";
    int strokeWidth = 0;
    int cx = 0;
    int cy = 0;
    int rx = 0, ry = 0;
    string transform = "";
public:
    void readLine(const string& line);
    void printEllipse();
    void draw();
};