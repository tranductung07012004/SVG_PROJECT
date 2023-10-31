#include "CircleSVG.h"

void Circle::readLine(const string& line) {
    string line1 = removeSpacesBetweenQuotes(line);
    istringstream iss(line1);
    string token;
    while (iss >> token) {
        size_t equalPos = token.find('=');
        if (equalPos != string::npos) {
            string key = token.substr(0, equalPos);
            string value = token.substr(equalPos + 2, token.size() - equalPos - 3);
            if (key == "cx") {
                cx = stoi(value);
            }
            else if (key == "cy") {
                cy = stoi(value);
            }
            else if (key == "r") {
                r = stoi(value);
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
            else if (key == "stroke-opacity") {
                strokeOpacity = stof(value);
            }
            else if (key == "fill-opacity") {
                fillOpacity = stof(value);
            }
        }
    }
}

void Circle::printCircle() {
    cout << "cx: " << cx << ", cy: " << cy << "\n";
    cout << "r: " << r << "\n";
    cout << "fill: " << fill << "\n";
    cout << "stroke: " << stroke << "\n";
    cout << "stroke-width: " << strokeWidth << "\n";
    cout << "stroke-opacity: " << strokeOpacity << "\n";
    cout << "fill-opacity: " << fillOpacity << "\n";
}