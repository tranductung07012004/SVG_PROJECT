#pragma once
#include "FunTionRead.h"

class Rect {
private:
    string style = "";
    float fillOpacity = 0;
    float strokeOpacity = 0;
    string fill = "";
    string stroke = "";
    int strokeWidth = 0;
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    int rx = 0;
    int ry = 0;
    string transform = "";
public:
    void readLine(const string& line);
    void printRect();
    void draw();
};