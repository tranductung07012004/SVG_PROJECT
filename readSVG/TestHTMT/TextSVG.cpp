#include "TextSVG.h"

void Text::readLine(const string& line) {
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
            else if (key == "fill") {
                fill = value;
            }
            else if (key == "font-size") {
                fontSize = stoi(value);
            }
            else if (key == "font-family") {
                fontFamily = value;
            }
            else if (key == "font-weight") {
                fontWeight = stoi(value);
            }
            else if (key == "font-style") {
                fontStyle = value;
            }
            else if (key == "text-anchor") {
                textAnchor = value;
            }
            else if (key == "text-decoration") {
                textDecoration = value;
            }
            else if (key == "text-transform") {
                textTransform = value;
            }
        }
    }

    size_t openTagPos = line.find(">");
    size_t closeTagPos = line.find("</text>");
    if (openTagPos != string::npos && closeTagPos != string::npos) {
        textContent = line.substr(openTagPos + 1, closeTagPos - openTagPos - 1);
    }
}
void Text::printText() {
    cout << "x: " << x << ", y: " << y << "\n";
    cout << "fill: " << fill << "\n";
    cout << "font-size: " << fontSize << "\n";
    cout << "font-family: " << fontFamily << "\n";
    cout << "font-weight: " << fontWeight << "\n";
    cout << "font-style: " << fontStyle << "\n";
    cout << "text-anchor: " << textAnchor << "\n";
    cout << "text-decoration: " << textDecoration << "\n";
    cout << "text-transform: " << textTransform << "\n";
    cout << "text content: " << textContent << "\n";
}