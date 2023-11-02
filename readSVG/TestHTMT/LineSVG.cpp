#include "LineSVG.h"

void Line::readLine(const string& line) {
    string line1 = removeSpacesBetweenQuotes(line);
    istringstream iss(line1);
    string token;
    while (iss >> token) {
        size_t equalPos = token.find('=');
        if (equalPos != string::npos) {
            string key = token.substr(0, equalPos);
            string value = token.substr(equalPos + 2, token.size() - equalPos - 3);
            if (key == "x1") {
                x1 = stoi(value);
            }
            else if (key == "y1") {
                y1 = stoi(value);
            }
            else if (key == "x2") {
                x2 = stoi(value);
            }
            else if (key == "y2") {
                y2 = stoi(value);
            }
            else if (key == "stroke") {
                stroke = value;
            }
            else if (key == "stroke-width") {
                strokeWidth = stoi(value);
            }
            else if (key == "stroke-opacity") {
                strokeOpacity = stof(value);
            }
        }
    }
}
