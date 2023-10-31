#pragma once
#include "FunTionRead.h"

class Text {
private:
    string style = "";
    float fillOpacity = 0;
    string fill = "";
    int strokeWidth = 0;
    int x = 0;
    int y = 0;
    string transform = "";
    int fontSize = 0;
    int fontWeight = 0;
    string fontStyle = "";
    string fontFamily = "";
    string textAnchor = "";
    string textDecoration = "";
    string textTransform = "";
    string textContent = "";
public:
    void readLine(const string& line);
    void printText();
    void draw();
};