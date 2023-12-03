#include "stdafx.h"
#include "classSVG.h"
#include "DataRead.h"
#include "FunctionRead.h"
void parsetransformSVG(vector<transformSVG>& transformations, const string& input) {
    string result;
    for (char c : input) {
        if (c != '\n') {
            result += c;
        }
    }
    transform(result.begin(), result.end(), result.begin(), ::towlower);
    regex transformRegex(R"((\w+)\s*\(\s*([+-]?\d*\.?\d+)\s*(?:[, ]\s*([+-]?\d*\.?\d+))?\s*\))");

    // Iterator to iterate over matches
    sregex_iterator it(result.begin(), result.end(), transformRegex);
    sregex_iterator end;

    while (it != end) {
        smatch match = *it;

        transformSVG transform;
        transform.transformType = match[1];

        // Set the values based on the transformation type
        if (transform.transformType == "translate") {
            transform.translateX = stof(match[2]);
            transform.translateY = match[3].matched ? stof(match[3]) : 0.0f;
        }
        else if (transform.transformType == "rotate") {
            transform.rotateAngle = stof(match[2]);
        }
        else if (transform.transformType == "scale") {
            transform.scaleX = stof(match[2]);
            transform.scaleY = match[3].matched ? stof(match[3]) : transform.scaleX;
        }

        // Add the transformation to the vector
        transformations.push_back(transform);

        ++it;
    }
}
string remove_spaces(const string& input_string) {
    string result = input_string;
    result.erase(remove_if(result.begin(), result.end(), ::isspace), result.end());
    return result;
}

RGBSVG colorSVG(const string& s, bool& c) {
    RGBSVG color;
    string s1 = s;

    transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
    s1 = remove_spaces(s1);
    if (s1 == "none" || s1 == "transparent") {
        c = 0;
        return color;
    }
    // Check for hexadecimal color code like "#000000" or "#FFF"
    regex hexPattern("#([a-fA-F0-9]{6}|[a-fA-F0-9]{3})");
    smatch hexMatch;

    if (regex_search(s1, hexMatch, hexPattern)) {
        string hexValue = hexMatch[1].str();

        // Convert 3-digit hex to 6-digit hex
        if (hexValue.size() == 3) {
            hexValue = string(2, hexValue[0]) + string(2, hexValue[1]) + string(2, hexValue[2]);
        }

        // Parse hex values to RGB
        color.R = stoul(hexValue.substr(0, 2), nullptr, 16);
        color.G = stoul(hexValue.substr(2, 2), nullptr, 16);
        color.B = stoul(hexValue.substr(4, 2), nullptr, 16);

    }
    else {
        // Check for RGB color format like "rgb(255, 127, 0)"        
        regex rgbPattern("rgb\\((-?\\d+),\\s*(-?\\d+),\\s*(-?\\d+)\\)");
        smatch rgbMatch;
        if (regex_search(s1, rgbMatch, rgbPattern) && rgbMatch.size() == 4) {

            color.R = stod(rgbMatch[1].str());
            color.G = stod(rgbMatch[2].str());
            color.B = stod(rgbMatch[3].str());
        }
        else {
            // Check if the color name is in the predefined colorMap
            if (colorMap.find(s1) != colorMap.end()) {
                color = colorMap[s1];
            }
            else {
                c = false;  // Set c to 0 if the color is not found in colorMap
                return color;
            }
        }
    }
    if (color.R > 255) color.R = 255;
    if (color.G > 255) color.G = 255;
    if (color.B > 255) color.B = 255;
    if (color.R < 0) color.R = 0;
    if (color.G < 0) color.G = 0;
    if (color.B < 0) color.B = 0;
    c = 1;
    return color;
}

string removeCommas(const std::string& input) {
    std::string result;
    for (char c : input) {
        if (c == ',' || c == '\n') {
            result += ' ';
        }
        else {
            result += c;
        }
    }
    return result;
}

vector<PointF> parsePointString(const string& input) {
    std::vector<PointF> points;
    string s = removeCommas(input);
    std::istringstream iss(s);
    REAL x, y;

    // Read pairs of coordinates
    while (iss >> x >> y) {
        points.push_back({ x, y });
    }
    return points;
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isChar(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}


string formatSVGPath(string& path) {
    string result;
    bool previousIsDigit = false;

    for (char c : path) {
        if (isDigit(c) || c == '-' || c == '.') {
            if (!previousIsDigit) {
                result += ' ';
            }
            result += c;
            previousIsDigit = true;
        }
        else if (c == ' ') {
            result += c;
        }
        else {

            result += '\n';

            result += c;
            previousIsDigit = false;
        }
    }
    return result;
}

vector<PointPathSVG> parsePathData(const string& input) {
    string sPath = removeCommas(input);
    //transform(sPath.begin(), sPath.end(), sPath.begin(), ::towupper)
    sPath = formatSVGPath(sPath);
    vector<PointPathSVG> DataPath;
    istringstream ss(sPath);

    string line;
    while (getline(ss, line, '\n')) {
        istringstream iss(line);
        PointPathSVG path;
        char type;
        double x, y;

        iss >> type;
        path.typePointPath = type;

        // Use && instead of ||
        if (type != 'H' && type != 'V' && type != 'A' && type != 'h' && type != 'v' && type != 'a') {
            while (iss >> x ) {
                PointF point;
                point.X = x;
                y = -3.4e38;
                iss >> y;
                point.Y = y;
                path.points.push_back(point);
            }
        }
        else if (type == 'H' || type == 'V' || type == 'h' || type == 'v') {
            while (iss >> x) {
                PointF point;
                if (type == 'H' || type == 'h') {
                    point.X = x;
                    point.Y = 0;
                }
                else {
                    point.X = 0;
                    point.Y = x;
                }
                path.points.push_back(point);
            }
        }
        else if (type == 'A' || type == 'a') {
            iss >> path.rx >> path.ry >> path.xAxisRotation >> path.largeArcFlag >> path.sweepFlag >> path.x >> path.y;
        }

        DataPath.push_back(path);
    }
    return DataPath;
}
