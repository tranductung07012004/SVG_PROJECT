#pragma once
#include "FunTionRead.h"

class Line {
private:
    string style = "";
    float fillOpacity = 0;
    float strokeOpacity = 0;
    string fill = "";
    string stroke = "";
    int strokeWidth = 0, x1 = 0, y1 = 0, x2 = 0, y2 = 0, width = 0, height = 0, rx = 0, ry = 0;
    string transform = "";
public:
    void readLine(const string& line);
    void printLine();
    void draw();
};