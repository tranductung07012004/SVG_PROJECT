#include "PolylineSVG.h"

void Polyline::readLine(const string& line) {
    string tmp = line;
    string linepoint = extractPointsValue(tmp);
    string line1 = removeSpacesBetweenQuotes(tmp);
    istringstream iss(line1);
    string token;

    while (iss >> token) {
        size_t equalPos = token.find('=');
        if (equalPos != string::npos) {
            string key = token.substr(0, equalPos);
            string value = token.substr(equalPos + 2, token.size() - equalPos - 3);
            if (key == "fill") {
                fill = value;
            }
            else if (key == "fill-opacity") {
                fillOpacity = stof(value);
            }
            else if (key == "stroke") {
                stroke = value;
            }
            else if (key == "stroke-opacity") {
                strokeOpacity = stof(value);
            }
            else if (key == "stroke-width") {
                strokeWidth = stoi(value);
            }

        }
    }
    istringstream in(linepoint);
    string token1;

    while (getline(in, token1, ' ')) {
        Point p;
        istringstream pointStream(token1);
        char comma;
        pointStream >> p.x >> comma >> p.y;
        points.push_back(p);
    }
}

void Polyline::printPolyline() {
    cout << "fill: " << fill << "\n";
    cout << "fill-opacity: " << fillOpacity << "\n";
    cout << "stroke: " << stroke << "\n";
    cout << "stroke-opacity: " << strokeOpacity << "\n";
    cout << "stroke-width: " << strokeWidth << "\n";
    cout << "Points: ";
    for (int i = 0; i < points.size(); i++) {
        cout << "(" << points[i].x << ", " << points[i].y << ") ";
    }
    cout << "\n";
}