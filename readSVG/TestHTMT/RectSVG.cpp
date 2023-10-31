#include "RectSVG.h"
void Rect::readLine(const string& line) {
    string line1 = removeSpacesBetweenQuotes(line);
    istringstream iss(line1);
    string token;
    while (iss >> token) {
        size_t equalPos = token.find('=');
        if (equalPos != string::npos) {
            string key = token.substr(0, equalPos);
            string value = token.substr(equalPos + 2, token.size() - equalPos - 3);

            if (key == "x") {
                x = stoi(value);
            }
            else if (key == "y") {
                y = stoi(value);
            }
            else if (key == "width") {
                width = stoi(value);
            }
            else if (key == "height") {
                height = stoi(value);
            }
            else if (key == "style") {
                style = value;
            }
            else if (key == "fill-opacity") {
                fillOpacity = stof(value);
            }
            else if (key == "fill") {
                fill = value;
            }
            else if (key == "stroke") {
                stroke = value;
            }
            else if (key == "stroke-width") {
                strokeWidth = stoi(value);
            }
            else if (key == "transform") {
                transform = value;
            }
            else if (key == "rx") {
                height = stoi(value);
            }
            else if (key == "ry") {
                height = stoi(value);
            }
            else if (key == "stroke-opacity") {
                height = stof(value);
            }
        }
    }
}

void Rect::printRect() {
    cout << "x: " << x << ", y: " << y << ", width: " << width << ", height: " << height << "\n";
    cout << "style: " << style << "\n";
    cout << "fill-opacity: " << fillOpacity << "\n";
    cout << "fill: " << fill << "\n";
    cout << "stroke: " << stroke << "\n";
    cout << "stroke-width: " << strokeWidth << "\n";
    cout << "transform: " << transform << "\n";
}