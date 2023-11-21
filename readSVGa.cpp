#include "stdafx.h"
#include "ReadSVG.h"


void parsetransformSVG(vector<transformSVG>& transformations, const string& input) {
    regex transformRegex(R"((\w+)\s*\(\s*([+-]?\d*\.?\d+)\s*(?:,\s*([+-]?\d*\.?\d+))?\s*\))");

    // Iterator to iterate over matches
    sregex_iterator it(input.begin(), input.end(), transformRegex);
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


bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

string removeCommas(const std::string& input) {
    std::string result;
    for (char c : input) {
        if (c == ',') {
            result += ' ';
        }
        else {
            result += c;
        }
    }
    return result;
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
            if (previousIsDigit) {
                result += '\n';
            }
            result += c;
            previousIsDigit = false;
        }
    }

    return result;
}

vector<PointPathSVG> parsePathData(const string& input) {

    string sPath = removeCommas(input);

    sPath = formatSVGPath(sPath);
    std::transform(sPath.begin(), sPath.end(), sPath.begin(), ::toupper);
    vector<PointPathSVG> DataPath;
    istringstream ss(sPath);

    string line;
    while (getline(ss, line, '\n')) {
        istringstream iss(line);
        PointPathSVG path;
        char type;
        double x, y;

        iss >> type;
        if (type == 'Z') break;
        path.typePointPath = type;

        // Use && instead of ||
        if (type != 'H' && type != 'V' && type != 'A') {
            while (iss >> x >> y) {
                PointSVG point;
                point.x = x;
                point.y = y;
                path.points.push_back(point);

            }
            if (type == 'Q') {
                fstream fo("1.txt");
                for (const auto& pointf : path.points)
                    fo << pointf.x << " " << pointf.y << endl;
            }
        }
        else if (type == 'H' || type == 'V') {
            while (iss >> x) {
                PointSVG point;
                if (type == 'H') {
                    point.x = x;
                    point.y = 0;
                }
                else {
                    point.x = 0;
                    point.y = x;
                }
                path.points.push_back(point);
            }
        }
        else if (type == 'A') {
            iss >> path.rx >> path.ry >> path.xAxisRotation >> path.largeArcFlag >> path.sweepFlag >> path.x >> path.y;
        }

        DataPath.push_back(path);
    }

    return DataPath;
}

void parseStyle(const string& s, SVGElement& element) {
    vector<string> stylePairs;
    stringstream ss(s);
    string pair;
    while (getline(ss, pair, ';')) {
        stylePairs.push_back(pair);
    }

    for (const string& stylePair : stylePairs) {
        size_t colonPos = stylePair.find(':');
        if (colonPos != string::npos) {
            string key = stylePair.substr(0, colonPos);
            string value = stylePair.substr(colonPos + 1);

            key.erase(0, key.find_first_not_of(" \t\n\r\f\v"));
            key.erase(key.find_last_not_of(" \t\n\r\f\v") + 1);

            value.erase(0, value.find_first_not_of(" \t\n\r\f\v"));
            value.erase(value.find_last_not_of(" \t\n\r\f\v") + 1);

            std::transform(key.begin(), key.end(), key.begin(), ::tolower);
            std::transform(value.begin(), value.end(), value.begin(), ::tolower);
            element.attributes[key] = value;
        }
    }
}

void parseSVGNode(xml_node<>* node, vector<SVGElement>& elements) {
    for (xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
        SVGElement element;
        element.type = child->name();

        for (xml_attribute<>* attr = child->first_attribute(); attr; attr = attr->next_attribute()) {
            string attrName = attr->name();
            transform(attrName.begin(), attrName.end(), attrName.begin(), ::tolower);
            string attrVal = attr->value();
            transform(attrVal.begin(), attrVal.end(), attrVal.begin(), ::tolower);
            element.attributes[attrName] = attrVal;
            if (attrName == "style") parseStyle(attrVal, element);
        }

        // Check if the element is of type "text" and extract its content
        if (element.type == "text") {
            element.textContent = child->value();
        }

        // If the element is a group ("<g>"), parse its children separately
        if (element.type == "g") {
            parseGroupNode(child, element);
        }

        elements.push_back(element);
    }
}

// Add this function to handle the <g> element
void parseGroupNode(xml_node<>* node, SVGElement& groupElement) {
    for (xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
        SVGElement element;
        element.type = child->name();

        for (xml_attribute<>* attr = child->first_attribute(); attr; attr = attr->next_attribute()) {
            string attrName = attr->name();
            transform(attrName.begin(), attrName.end(), attrName.begin(), ::tolower);
            string attrVal = attr->value();
            transform(attrVal.begin(), attrVal.end(), attrVal.begin(), ::tolower);
            element.attributes[attrName] = attrVal;
            if (attrName == "style") parseStyle(attrVal, element);
        }

        // Check if the element is of type "text" and extract its content
        if (element.type == "text") {
            element.textContent = child->value();
        }
        if (element.type == "g") {
            parseGroupNode(child, element);
        }
        groupElement.children.push_back(element);
    }
}
vector<SVGElement> parseSVG(const string& filename) {
    vector<SVGElement> result;

    // Open the SVG file
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file '" << filename << "'" << endl;
        return result;  // Return an empty vector on error
    }

    // Read the content of the SVG file into a string
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    string content = buffer.str();

    // Parse the SVG content using RapidXML
    xml_document<> doc;
    doc.parse<0>(&content[0]);

    // Start parsing the SVG elements
    xml_node<>* svgNode = doc.first_node("svg");
    if (svgNode) {
        parseSVGNode(svgNode, result);
    }
    else {
        cerr << "Error: Missing 'svg' element in the SVG file" << endl;
    }

    return result;
}

string remove_spaces(const string& input_string) {
    string result = input_string;
    result.erase(remove_if(result.begin(), result.end(), ::isspace), result.end());
    return result;
}


RGBSVG colorSVG(const string& s) {
    RGBSVG color;
    string s1 = s;

    transform(s1.begin(), s1.end(), s1.begin(), ::tolower);

    regex pattern("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");
    smatch match;

    if (regex_search(s1, match, pattern) && match.size() == 4) {
        color.R = stod(match[1].str());
        color.G = stod(match[2].str());
        color.B = stod(match[3].str());
    }
    else {
        s1 = remove_spaces(s1);
        unordered_map<string, RGBSVG> colorMap = {
            {"aliceblue", {240, 248, 255}},
            {"antiquewhite", {250, 235, 215}},
            {"aqua", {0, 255, 255}},
            {"aquamarine", {127, 255, 212}},
            {"azure", {240, 255, 255}},
            {"beige", {245, 245, 220}},
            {"bisque", {255, 228, 196}},
            {"black", {0, 0, 0}},
            {"blanchedalmond", {255, 235, 205}},
            {"blue", {0, 0, 255}},
            {"blueviolet", {138, 43, 226}},
            {"brown", {165, 42, 42}},
            {"burlywood", {222, 184, 135}},
            {"cadetblue", {95, 158, 160}},
            {"chartreuse", {127, 255, 0}},
            {"chocolate", {210, 105, 30}},
            {"coral", {255, 127, 80}},
            {"cornflowerblue", {100, 149, 237}},
            {"cornsilk", {255, 248, 220}},
            {"crimson", {220, 20, 60}},
            {"cyan", {0, 255, 255}},
            {"darkblue", {0, 0, 139}},
            {"darkcyan", {0, 139, 139}},
            {"darkgoldenrod", {184, 134, 11}},
            {"darkgray", {169, 169, 169}},
            {"darkgreen", {0, 100, 0}},
            {"darkkhaki", {189, 183, 107}},
            {"darkmagenta", {139, 0, 139}},
            {"darkolivegreen", {85, 107, 47}},
            {"darkorange", {255, 140, 0}},
            {"darkorchid", {153, 50, 204}},
            {"darkred", {139, 0, 0}},
            {"darksalmon", {233, 150, 122}},
            {"darkseagreen", {143, 188, 143}},
            {"darkslateblue", {72, 61, 139}},
            {"darkslategray", {47, 79, 79}},
            {"darkturquoise", {0, 206, 209}},
            {"darkviolet", {148, 0, 211}},
            {"deeppink", {255, 20, 147}},
            {"deepskyblue", {0, 191, 255}},
            {"dimgray", {105, 105, 105}},
            {"dodgerblue", {30, 144, 255}},
            {"firebrick", {178, 34, 34}},
            {"floralwhite", {255, 250, 240}},
            {"forestgreen", {34, 139, 34}},
            {"fuchsia", {255, 0, 255}},
            {"gainsboro", {220, 220, 220}},
            {"ghostwhite", {248, 248, 255}},
            {"gold", {255, 215, 0}},
            {"goldenrod", {218, 165, 32}},
            {"gray", {128, 128, 128}},
            {"green", {0, 128, 0}},
            {"greenyellow", {173, 255, 47}},
            {"honeydew", {240, 255, 240}},
            {"hotpink", {255, 105, 180}},
            {"indianred", {205, 92, 92}},
            {"indigo", {75, 0, 130}},
            {"ivory", {255, 255, 240}},
            {"khaki", {240, 230, 140}},
            {"lavender", {230, 230, 250}},
            {"lavenderblush", {255, 240, 245}},
            {"lawngreen", {124, 252, 0}},
            {"lemonchiffon", {255, 250, 205}},
            {"lightblue", {173, 216, 230}},
            {"lightcoral", {240, 128, 128}},
            {"lightcyan", {224, 255, 255}},
            {"lightgoldenrodyellow", {250, 250, 210}},
            {"lightgray", {211, 211, 211}},
            {"lightgreen", {144, 238, 144}},
            {"lightpink", {255, 182, 193}},
            {"lightsalmon", {255, 160, 122}},
            {"lightseagreen", {32, 178, 170}},
            {"lightskyblue", {135, 206, 250}},
            {"lightslategray", {119, 136, 153}},
            {"lightsteelblue", {176, 196, 222}},
            {"lightyellow", {255, 255, 224}},
            {"lime", {0, 255, 0}},
            {"limegreen", {50, 205, 50}},
            {"linen", {250, 240, 230}},
            {"magenta", {255, 0, 255}},
            {"maroon", {128, 0, 0}},
            {"mediumaquamarine", {102, 205, 170}},
            {"mediumblue", {0, 0, 205}},
            {"mediumorchid", {186, 85, 211}},
            {"mediumpurple", {147, 112, 219}},
            {"mediumseagreen", {60, 179, 113}},
            {"mediumslateblue", {123, 104, 238}},
            {"mediumspringgreen", {0, 250, 154}},
            {"mediumturquoise", {72, 209, 204}},
            {"mediumvioletred", {199, 21, 133}},
            {"midnightblue", {25, 25, 112}},
            {"mintcream", {245, 255, 250}},
            {"mistyrose", {255, 228, 225}},
            {"moccasin", {255, 228, 181}},
            {"navajowhite", {255, 222, 173}},
            {"navy", {0, 0, 128}},
            {"oldlace", {253, 245, 230}},
            {"olive", {128, 128, 0}},
            {"olivedrab", {107, 142, 35}},
            {"orange", {255, 165, 0}},
            {"orangered", {255, 69, 0}},
            {"orchid", {218, 112, 214}},
            {"palegoldenrod", {238, 232, 170}},
            {"palegreen", {152, 251, 152}},
            {"paleturquoise", {175, 238, 238}},
            {"palevioletred", {219, 112, 147}},
            {"papayawhip", {255, 239, 213}},
            {"peachpuff", {255, 218, 185}},
            {"peru", {205, 133, 63}},
            {"pink", {255, 192, 203}},
            {"plum", {221, 160, 221}},
            {"powderblue", {176, 224, 230}},
            {"purple", {128, 0, 128}},
            {"rebeccapurple", {102, 51, 153}},
            {"red", {255, 0, 0}},
            {"rosybrown", {188, 143, 143}},
            {"royalblue", {65, 105, 225}},
            {"saddlebrown", {139, 69, 19}},
            {"salmon", {250, 128, 114}},
            {"sandybrown", {244, 164, 96}},
            {"seagreen", {46, 139, 87}},
            {"seashell", {255, 245, 238}},
            {"sienna", {160, 82, 45}},
            {"silver", {192, 192, 192}},
            {"skyblue", {135, 206, 235}},
            {"slateblue", {106, 90, 205}},
            {"slategray", {112, 128, 144}},
            {"snow", {255, 250, 250}},
            {"springgreen", {0, 255, 127}},
            {"steelblue", {70, 130, 180}},
            {"tan", {210, 180, 140}},
            {"teal", {0, 128, 128}},
            {"thistle", {216, 191, 216}},
            {"tomato", {255, 99, 71}},
            {"turquoise", {64, 224, 208}},
            {"violet", {238, 130, 238}},
            {"wheat", {245, 222, 179}},
            {"white", {255, 255, 255}},
            {"whitesmoke", {245, 245, 245}},
            {"yellow", {255, 255, 0}},
            {"yellowgreen", {154, 205, 50}}

        };
        if (colorMap.find(s1) != colorMap.end()) {
            color = colorMap[s1];
        }
    }
    return color;
}

vector<PointSVG> parsePointString(const string& input) {
    vector<PointSVG> points;
    istringstream ss(input);
    string token;

    while (getline(ss, token, ' ')) {
        PointSVG point;
        size_t commaPos = token.find(',');
        if (commaPos != string::npos) {
            point.x = stod(token.substr(0, commaPos));
            point.y = stod(token.substr(commaPos + 1));
            points.push_back(point);
        }
    }
    return points;
}



void RectSVG::parseShapeSVG(const SVGElement& element) {
    bool cStroke = 0, cFill = 0;
    for (const auto& attr : element.attributes) {
        if (attr.first == "x") {
            p.x = stod(attr.second);
        }
        else if (attr.first == "y") {
            p.y = stod(attr.second);
        }
        else if (attr.first == "width") {
            width = stod(attr.second);
        }
        else if (attr.first == "height") {
            height = stod(attr.second);
        }
        else if (attr.first == "rx") {
            rx = stod(attr.second);
        }
        else if (attr.first == "ry") {
            ry = stod(attr.second);
        }
        else if (attr.first == "fill") {
            fill = colorSVG(remove_spaces(attr.second));
            if (remove_spaces(attr.second) == "none" || remove_spaces(attr.second) == "transparent") cFill = 1;
        }
        else if (attr.first == "fill-opacity") {
            fillOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke") {
            stroke = colorSVG(attr.second);
            if (remove_spaces(attr.second) == "none" || remove_spaces(attr.second) == "transparent") cStroke = 1;
        }
        else if (attr.first == "stroke-opacity") {
            strokeOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke-width") {
            strokeWidth = stod(attr.second);
        }
        else if (attr.first == "transform") {
            parsetransformSVG(tfSVG, attr.second);
        }
        else if (attr.first == "style") {
            style = attr.second;
        }
    }
    if (cStroke == 1) strokeOpacity = 0;
    if (cFill == 1) fillOpacity = 0;
}

void TextSVG::parseShapeSVG(const SVGElement& element) {
    bool cStroke = 0, cFill = 0;
    for (const auto& attr : element.attributes) {
        if (attr.first == "x") {
            p.x = stod(attr.second);
        }
        else if (attr.first == "y") {
            p.y = stod(attr.second);
        }
        else if (attr.first == "font-size") {
            fontSize = stod(attr.second);
        }
        else if (attr.first == "font-weight") {
            try {
                fontWeight1 = stoi(attr.second);
            }
            catch (const std::invalid_argument&) {
                fontWeight2 = attr.second;
            }
        }
        else if (attr.first == "font-style") {
            fontStyle = attr.second;
            std::transform(fontStyle.begin(), fontStyle.end(), fontStyle.begin(), ::tolower);
        }
        else if (attr.first == "font-family") {
            fontFamily = attr.second;
            std::transform(fontFamily.begin(), fontFamily.end(), fontFamily.begin(), ::tolower);
        }
        else if (attr.first == "text-anchor") {
            textAnchor = attr.second;
            std::transform(textAnchor.begin(), textAnchor.end(), textAnchor.begin(), ::tolower);
        }
        else if (attr.first == "text-decoration") {
            textDecoration = attr.second;
            std::transform(textDecoration.begin(), textDecoration.end(), textDecoration.begin(), ::tolower);
        }
        else if (attr.first == "text-transform") {
            textTransform = attr.second;
            std::transform(textTransform.begin(), textTransform.end(), textTransform.begin(), ::tolower);
        }
        else if (attr.first == "fill") {
            fill = colorSVG(attr.second);
            if (remove_spaces(attr.second) == "none" || remove_spaces(attr.second) == "transparent") cFill = 1;
        }
        else if (attr.first == "fill-opacity") {
            fillOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke") {
            stroke = colorSVG(attr.second);
            if (remove_spaces(attr.second) == "none" || remove_spaces(attr.second) == "transparent") { cStroke = 1; }
            else checkStroke = 1;
        }
        else if (attr.first == "stroke-opacity") {
            strokeOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke-width") {
            strokeWidth = stod(attr.second);
        }
        else if (attr.first == "transform") {
            parsetransformSVG(tfSVG, attr.second);
        }
    }

    // Store the text content from the SVG element
    textContent = element.textContent;
    if (cStroke == 1) strokeOpacity = 0;
    if (cFill == 1) fillOpacity = 0;
}

void CircleSVG::parseShapeSVG(const SVGElement& element) {
    bool cStroke = 0, cFill = 0;
    for (const auto& attr : element.attributes) {
        if (attr.first == "stroke-width") {
            strokeWidth = stod(attr.second);
        }
        else if (attr.first == "cx") {
            c.x = stod(attr.second);
        }
        else if (attr.first == "cy") {
            c.y = stod(attr.second);
        }
        else if (attr.first == "r") {
            r = stod(attr.second);
        }
        else if (attr.first == "fill") {
            fill = colorSVG(attr.second);
            if (remove_spaces(attr.second) == "none" || remove_spaces(attr.second) == "transparent") cFill = 1;
        }
        else if (attr.first == "fill-opacity") {
            fillOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke") {
            stroke = colorSVG(attr.second);
            if (remove_spaces(attr.second) == "none" || remove_spaces(attr.second) == "transparent") cStroke = 1;
        }
        else if (attr.first == "stroke-opacity") {
            strokeOpacity = stod(attr.second);
        }
        else if (attr.first == "transform") {
            parsetransformSVG(tfSVG, attr.second);
        }
        else if (attr.first == "style") {
            style = attr.second;
        }
    }
    if (cStroke == 1) strokeOpacity = 0;
    if (cFill == 1) fillOpacity = 0;
}

void EllipseSVG::parseShapeSVG(const SVGElement& element) {
    bool cStroke = 0, cFill = 0;
    for (const auto& attr : element.attributes) {
        if (attr.first == "cx") {
            c.x = stod(attr.second);
        }
        else if (attr.first == "cy") {
            c.y = stod(attr.second);
        }
        else if (attr.first == "rx") {
            rx = stod(attr.second);
        }
        else if (attr.first == "ry") {
            ry = stod(attr.second);
        }
        else if (attr.first == "fill") {
            fill = colorSVG(attr.second);
            if (remove_spaces(attr.second) == "none" || remove_spaces(attr.second) == "transparent") cFill = 1;

        }
        else if (attr.first == "fill-opacity") {
            fillOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke") {
            stroke = colorSVG(attr.second);
            if (remove_spaces(attr.second) == "none" || remove_spaces(attr.second) == "transparent") cStroke = 1;
        }
        else if (attr.first == "stroke-opacity") {
            strokeOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke-width") {
            strokeWidth = stod(attr.second);
        }
        else if (attr.first == "transform") {
            parsetransformSVG(tfSVG, attr.second);
        }
        else if (attr.first == "style") {
            style = attr.second;
        }
    }
    if (cStroke == 1) strokeOpacity = 0;
    if (cFill == 1) fillOpacity = 0;
}

void LineSVG::parseShapeSVG(const SVGElement& element) {
    bool cStroke = 0;

    for (const auto& attr : element.attributes) {
        if (attr.first == "x1") {
            p1.x = stod(attr.second);
        }
        else if (attr.first == "y1") {
            p1.y = stod(attr.second);
        }
        else if (attr.first == "x2") {
            p2.x = stod(attr.second);
        }
        else if (attr.first == "y2") {
            p2.y = stod(attr.second);
        }
        else if (attr.first == "stroke") {
            stroke = colorSVG(attr.second);
            if (remove_spaces(attr.second) == "none" || remove_spaces(attr.second) == "transparent") cStroke = 1;
        }
        else if (attr.first == "stroke-opacity") {
            strokeOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke-width") {
            strokeWidth = stod(attr.second);
        }
        else if (attr.first == "transform") {
            parsetransformSVG(tfSVG, attr.second);
        }
        else if (attr.first == "style") {
            style = attr.second;
        }
    }
    if (cStroke == 1) strokeOpacity = 0;
}

void PolygonSVG::parseShapeSVG(const SVGElement& element) {
    bool cStroke = 0, cFill = 0;
    for (const auto& attr : element.attributes) {
        if (attr.first == "points") {
            points = parsePointString(attr.second);
        }
        else if (attr.first == "fill") {
            fill = colorSVG(attr.second);
            if (remove_spaces(attr.second) == "none" || remove_spaces(attr.second) == "transparent") cFill = 1;
        }
        else if (attr.first == "fill-opacity") {
            fillOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke") {
            stroke = colorSVG(attr.second);
            if (remove_spaces(attr.second) == "none" || remove_spaces(attr.second) == "transparent") cStroke = 1;
        }
        else if (attr.first == "stroke-opacity") {
            strokeOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke-width") {
            strokeWidth = stod(attr.second);
        }
        else if (attr.first == "transform") {
            parsetransformSVG(tfSVG, attr.second);
        }
        else if (attr.first == "style") {
            style = attr.second;
        }
    }
    if (cStroke == 1) strokeOpacity = 0;
    if (cFill == 1) fillOpacity = 0;
}
void PolylineSVG::parseShapeSVG(const SVGElement& element) {
    bool cStroke = 0, cFill = 0;
    for (const auto& attr : element.attributes) {
        if (attr.first == "points") {
            points = parsePointString(attr.second);
        }
        else if (attr.first == "fill") {
            fill = colorSVG(attr.second);
            if (remove_spaces(attr.second) == "none" || remove_spaces(attr.second) == "transparent") cFill = 1;

        }
        else if (attr.first == "fill-opacity") {
            fillOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke") {
            stroke = colorSVG(attr.second);
            if (remove_spaces(attr.second) == "none" || remove_spaces(attr.second) == "transparent") cFill = 1;
        }
        else if (attr.first == "stroke-opacity") {
            strokeOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke-width") {
            strokeWidth = stod(attr.second);
        }
        else if (attr.first == "transform") {
            parsetransformSVG(tfSVG, attr.second);
        }
        else if (attr.first == "style") {
            style = attr.second;
        }
    }
    if (cStroke == 1) strokeOpacity = 0;
    if (cFill == 1) fillOpacity = 0;
}

void PathSVG::parseShapeSVG(const SVGElement& element) {
    bool cStroke = 0, cFill = 0;

    for (const auto& attr : element.attributes) {
        if (attr.first == "d") {
            PathData = parsePathData(attr.second);
        }
        else if (attr.first == "fill") {
            fill = colorSVG(attr.second);
            if (remove_spaces(attr.second) == "none" || remove_spaces(attr.second) == "transparent") cFill = 1;

        }
        else if (attr.first == "fill-opacity") {
            fillOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke") {
            stroke = colorSVG(attr.second);
            if (remove_spaces(attr.second) == "none" || remove_spaces(attr.second) == "transparent") cStroke = 1;

        }
        else if (attr.first == "stroke-opacity") {
            strokeOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke-width") {
            strokeWidth = stod(attr.second);
        }
        else if (attr.first == "transform") {
            parsetransformSVG(tfSVG, attr.second);
        }
        else if (attr.first == "style") {
            style = attr.second;
        }
    }
    if (cStroke == 1) strokeOpacity = 0;
    if (cFill == 1) fillOpacity = 0;
}

void ShapeSVG::copyAttributes(const ShapeSVG& other) {
    fillOpacity = other.fillOpacity;
    strokeOpacity = other.strokeOpacity;
    fill = other.fill;
    stroke = other.stroke;
    strokeWidth = other.strokeWidth;
    style = other.style;
    if (other.tfSVG.size() != 0)
        for (const auto& tf : other.tfSVG)
            tfSVG.push_back(tf);
}

void GroupSVG::parseShapeSVG(const SVGElement& element) {
    bool cStroke = 0, cFill = 0;

    for (const auto& attr : element.attributes) {
        if (attr.first == "fill") {
            fill = colorSVG(attr.second);
            if (remove_spaces(attr.second) == "none" || remove_spaces(attr.second) == "transparent") cFill = 1;

        }
        else if (attr.first == "fill-opacity") {
            fillOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke") {
            stroke = colorSVG(attr.second);
            if (remove_spaces(attr.second) == "none" || remove_spaces(attr.second) == "transparent") cStroke = 1;
        }
        else if (attr.first == "stroke-opacity") {
            strokeOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke-width") {
            strokeWidth = stod(attr.second);
        }
        else if (attr.first == "transform") {
            parsetransformSVG(tfSVG, attr.second);
        }
        else if (attr.first == "style") {
            style = attr.second;
        }
    }
    if (cStroke == 1) strokeOpacity = 0;
    if (cFill == 1) fillOpacity = 0;
    for (const SVGElement& childElement : element.children) {
        GroupOrShape elementToAdd;
        if (childElement.type == "g") {
            elementToAdd.type = GroupOrShape::GROUP;
            elementToAdd.group = make_unique<GroupSVG>();
            elementToAdd.group->parseShapeSVG(childElement);
        }
        else {
            elementToAdd.type = GroupOrShape::SHAPE;
            if (childElement.type == "rect") {
                elementToAdd.shape = make_unique<RectSVG>();
            }
            else if (childElement.type == "text") {
                elementToAdd.shape = make_unique<TextSVG>();
            }
            else if (childElement.type == "circle") {
                elementToAdd.shape = make_unique<CircleSVG>();
            }
            else if (childElement.type == "ellipse") {
                elementToAdd.shape = make_unique<EllipseSVG>();
            }
            else if (childElement.type == "line") {
                elementToAdd.shape = make_unique<LineSVG>();
            }
            else if (childElement.type == "polygon") {
                elementToAdd.shape = make_unique<PolygonSVG>();
            }
            else if (childElement.type == "polyline") {
                elementToAdd.shape = make_unique<PolylineSVG>();
            }
            else if (childElement.type == "path") {
                elementToAdd.shape = make_unique<PathSVG>();
            }
            else {
                continue;
            }
            elementToAdd.shape->copyAttributes(*this);
            elementToAdd.shape->parseShapeSVG(childElement);
        }

        elements.push_back(std::move(elementToAdd));
    }
}

void CircleSVG::drawSVG(Graphics& graphics) {
    //Graphics graphics(hdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    //graphics.ScaleTransform(2.0f, 2.0f);
    //float zoomFactor = 1.0;
   // Matrix scalingMatrix(zoomFactor, 0, 0, zoomFactor, 0, 0); // Create a scaling matrix
    //graphics.SetTransform(&scalingMatrix);


    for (const auto& tf : tfSVG) {
        if (tf.transformType == "translate") {

            this->TranslateCircle(graphics, tf.translateX, tf.translateY);

        }
        if (tf.transformType == "scale") {
            this->ScaleCircle(graphics, tf.scaleX, tf.scaleY);
        }
    }
    Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
    RectF ellipseRect(c.x - r, c.y - r, r * 2, r * 2);

    SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));

    graphics.FillEllipse(&brush, ellipseRect);
    graphics.DrawEllipse(&pen, ellipseRect);
    // graphics.ResetTransform();
}

void EllipseSVG::drawSVG(Graphics& graphics) {
    //Graphics graphics(hdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    //graphics.ScaleTransform(2.0f, 2.0f);
    //float zoomFactor = 1.0;
   // Matrix scalingMatrix(zoomFactor, 0, 0, zoomFactor, 0, 0); // Create a scaling matrix
    //graphics.SetTransform(&scalingMatrix);


    for (const auto& tf : tfSVG) {
        if (tf.transformType == "translate") {

            this->TranslateEllipse(graphics, tf.translateX, tf.translateY);

        }
        if (tf.transformType == "scale") {
            this->ScaleEllipse(graphics, tf.scaleX, tf.scaleY);
        }
    }
    Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
    RectF ellipseRect(c.x - rx, c.y - ry, rx * 2, ry * 2);
    SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));
    graphics.FillEllipse(&brush, ellipseRect);
    graphics.DrawEllipse(&pen, ellipseRect);

    //graphics.ResetTransform();
}

void LineSVG::drawSVG(Graphics& graphics) {
    //Graphics graphics(hdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    //graphics.ScaleTransform(2.0f, 2.0f);
    //float zoomFactor = 1.0;
   // Matrix scalingMatrix(zoomFactor, 0, 0, zoomFactor, 0, 0); // Create a scaling matrix
    //graphics.SetTransform(&scalingMatrix);

    for (const auto& tf : tfSVG) {
        if (tf.transformType == "translate") {

            this->TranslateLine(graphics, tf.translateX, tf.translateY);

        }
        if (tf.transformType == "scale") {
            this->ScaleLine(graphics, tf.scaleX, tf.scaleY);
        }
    }
    Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
    PointF point1((REAL)p1.x, (REAL)p1.y);
    PointF point2(p2.x, p2.y);
    graphics.DrawLine(&pen, point1, point2);
    // graphics.ResetTransform();
}

void PolygonSVG::drawSVG(Graphics& graphics) {
    //Graphics graphics(hdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    // graphics.ScaleTransform(2.0f, 2.0f);
     //float zoomFactor = 1.0;
    // Matrix scalingMatrix(zoomFactor, 0, 0, zoomFactor, 0, 0); // Create a scaling matrix
     //graphics.SetTransform(&scalingMatrix);



    int size = points.size();
    PointF* point = new PointF[size];

    for (int i = 0; i < size; i++) {
        point[i].X = points[i].x;
        point[i].Y = points[i].y;
    }
    for (const auto& tf : tfSVG) {
        if (tf.transformType == "translate") {

            this->TranslatePolygon(graphics, tf.translateX, tf.translateY);
            for (int i = 0; i < size; i++) {
                point[i].X = points[i].x;
                point[i].Y = points[i].y;
            }

        }
        if (tf.transformType == "scale") {
            this->ScalePolygon(graphics, tf.scaleX, tf.scaleY);
            for (int i = 0; i < size; i++) {
                point[i].X = points[i].x;
                point[i].Y = points[i].y;
            }
        }
    }
    Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);


    SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));

    graphics.FillPolygon(&brush, point, size);
    graphics.DrawPolygon(&pen, point, size);

    delete[] point;
    //graphics.ResetTransform();
}

void PolylineSVG::drawSVG(Graphics& graphics) {
    //Graphics graphics(hdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    int size = points.size();
    PointF* point = new PointF[size];

    for (int i = 0; i < size; i++) {
        point[i].X = points[i].x;
        point[i].Y = points[i].y;
    }
    for (const auto& tf : tfSVG) {
        if (tf.transformType == "translate") {

            this->TranslatePolyline(graphics, tf.translateX, tf.translateY);
            for (int i = 0; i < size; i++) {
                point[i].X = points[i].x;
                point[i].Y = points[i].y;
            }
        }
        if (tf.transformType == "scale") {
            this->ScalePolyline(graphics, tf.scaleX, tf.scaleY);
            for (int i = 0; i < size; i++) {
                point[i].X = points[i].x;
                point[i].Y = points[i].y;
            }
        }
    }

    SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));
    graphics.FillPolygon(&brush, point, size);
    Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
    graphics.DrawLines(&pen, point, size);

    delete[] point;
    // graphics.ResetTransform();
}

void RectSVG::drawSVG(Graphics& graphics) {
    //Graphics graphics(hdc);


    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    for (const auto& tf : tfSVG) {
        if (tf.transformType == "translate") {

            this->TranslateRectangle(graphics, tf.translateX, tf.translateY);

        }
        if (tf.transformType == "scale") {
            this->ScaleRectangle(graphics, tf.scaleX, tf.scaleY);
        }
    }
    Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
    //graphics.ScaleTransform(2.0f, 2.0f);
    //float zoomFactor = 1.0;
   // Matrix scalingMatrix(zoomFactor, 0, 0, zoomFactor, 0, 0); // Create a scaling matrix
    //graphics.SetTransform(&scalingMatrix);

   //Matrix matrix;
   //// PointF p;
   // //p.X = (ptMM.pointMin.x + ptMM.pointMax.x) / 2;
   //// p.Y = (ptMM.pointMin.y + ptMM.pointMax.y) / 2;
   // //int width = ptMM.pointMax.x - ptMM.pointMin.x;
   // //int height = ptMM.pointMax.y - ptMM.pointMin.y;
   //matrix.RotateAt(180.0f, PointF(500, 200)); // Rotation angle: 45 degrees, Rotation center: (150, 100)
   //graphics.SetTransform(&matrix);
   //// graphics.DrawRectangle(&pen, (int)ptMM.pointMin.x, (int)ptMM.pointMin.y, width, height);
   //graphics.DrawRectangle(&pen, rx, ry, width, height);

    SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));
    graphics.FillRectangle(&brush, (int)p.x, (int)p.y, width, height);
    graphics.DrawRectangle(&pen, (int)p.x, (int)p.y, width, height);

    //graphics.ResetTransform();

}

void TextSVG::drawSVG(Graphics& graphics) {
    //Graphics graphics(hdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);


    for (const auto& tf : tfSVG) {
        if (tf.transformType == "translate") {

            this->TranslateText(graphics, tf.translateX, tf.translateY);

        }
        if (tf.transformType == "scale") {
            this->ScaleText(graphics, tf.scaleX, tf.scaleY);
        }
    }
    Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
    Pen pen1(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B));
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring ws = converter.from_bytes(fontFamily);
    FontFamily fontFamily(ws.c_str());

    int font1 = FontStyleRegular;
    if (fontWeight2 == "bold" || fontWeight2 == "Bold" || fontWeight1 >= 550) {
        font1 = FontStyleBold;
    }
    else if (fontStyle == "italic" || fontStyle == "Italic") {
        font1 = FontStyleItalic;
    }
    else if ((fontWeight2 == "bold" || fontWeight2 == "Bold" || fontWeight1 >= 550) && (fontStyle == "italic" || fontStyle == "Italic")) {
        font1 = FontStyleBoldItalic;
    }
    else if (textDecoration == "underline" || textDecoration == "Underline") {
        font1 = FontStyleUnderline;
    }
    else if (textDecoration == "line-through") {
        font1 = FontStyleStrikeout;
    }
    Font font(&fontFamily, fontSize, font1, UnitPixel);
    //draw string in group
    PointF point(static_cast<float>(p.x) - fontSize - 6, static_cast<float>(p.y) - fontSize + 5);
    //draw string outside group
    PointF point2(static_cast<float>(p.x), static_cast<float>(p.y) - fontSize + fontSize / 10);
    SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));
    wstring wstr = converter.from_bytes(textContent);
    PointF point1(static_cast<float>(p.x) - fontSize - 6, static_cast<float>(p.y) - fontSize - 2);

    GraphicsPath path;
    path.AddString(wstr.c_str(), -1, &fontFamily, font1, fontSize, point1, NULL);
    /*if (fontSize >= 59) {
        path.AddString(wstr.c_str(), -1, &fontFamily, font1, fontSize, point, NULL);
    }*/
    if (strokeWidth == 1) {
        pen1.SetWidth(3);
    }
    else pen1.SetWidth(strokeWidth + 3);


    graphics.DrawString(wstr.c_str(), -1, &font, point, &brush);
    if (checkStroke == 1)
        graphics.DrawPath(&pen1, &path);

    //graphics.ResetTransform();
    //FontFamily fontfam(L"Arial");
    //Font font(L"Arial", 37);
    //SolidBrush textBrush(Color(255, 0, 0, 255)); // Màu chữ đen

    //// Tạo đối tượng GraphicsPath
    //GraphicsPath path;

    //// Thiết lập thuộc tính "outline" cho chữ
    //path.AddString(
    //    L"Hello World",
    //    -1,                 // NULL-terminated string
    //    &fontfam,
    //    FontStyleRegular,
    //    48,
    //    PointF(50.0f, 50.0f),
    //    NULL);

    //// Vẽ chữ với thuộc tính "outline"
    //Pen pen(Color(255, 255, 0, 0), 5.0f); // Màu viền đỏ và độ rộng viền
    //graphics.DrawPath(&pen, &path);
    //graphics.DrawString(L"Hello World", -1, &font, PointF(50.0f, 50.0f), &textBrush);
}
void PathSVG::RotatePath(Graphics& graphics, float angleInDegrees) {
    // Calculate rotation matrix
    Matrix matrix;
    matrix.Rotate(angleInDegrees);

    // Apply rotation to the graphics object
    graphics.SetTransform(&matrix);
}
void PathSVG::drawSVG(Graphics& graphics) {
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    for (const auto& tf : tfSVG) {
        if (tf.transformType == "translate") {
            this->TranslatePath(graphics, tf.translateX, tf.translateY);
        }
        if (tf.transformType == "scale") {
            this->ScalePath(graphics, tf.scaleX, tf.scaleY);
        }
        else if (tf.transformType == "rotate") {
            this->RotatePath(graphics, tf.rotateAngle); 
        }
    }

    PointF start = { -3.4e38,-3.4e38 };
    PointF controlPoint;
    PointF control;
    GraphicsPath path;
    char typeBefore = NULL;

    for (const auto& data : PathData) {
        if (data.typePointPath == 'M') {
            // Move to the starting point
            path.StartFigure();
            PointF startPoint(static_cast<float>(data.points[0].x), static_cast<float>(data.points[0].y));
            start = startPoint;
            //path.AddLine(startPoint, startPoint);
        }
        else if (data.typePointPath == 'L' || data.typePointPath == 'l') {
            // Draw a line
            for (const auto& point : data.points) {
                PointF endPoint(static_cast<float>(point.x), static_cast<float>(point.y));
                path.AddLine(start, endPoint);
                start = endPoint;
            }
        }
        else if (data.typePointPath == 'C' || data.typePointPath == 'c') {
            // Draw a Bezier curve
            if (data.points.size() >= 3) {
                PointF controlPoint1(static_cast<float>(data.points[0].x), static_cast<float>(data.points[0].y));
                PointF controlPoint2(static_cast<float>(data.points[1].x), static_cast<float>(data.points[1].y));
                PointF endPoint(static_cast<float>(data.points[2].x), static_cast<float>(data.points[2].y));
                path.AddBezier(start, controlPoint1, controlPoint2, endPoint);
                start = endPoint;
                controlPoint = controlPoint2;
                typeBefore = 'C';
            }
        }

        else if (data.typePointPath == 'V') {
            for (const auto& point : data.points) {
                PointF endPoint(static_cast<float>(start.X), static_cast<float>(point.y));
                path.AddLine(start, endPoint);
                start = endPoint;
            }
        }

        else if (data.typePointPath == 'H') {
            for (const auto& point : data.points) {
                PointF endPoint(static_cast<float>(point.x), static_cast<float>(start.Y));
                path.AddLine(start, endPoint);
                start = endPoint;
            }
        }

        else if (data.typePointPath == 'Z') {

            path.CloseFigure();
        }
    }

    Pen pen(Color(strokeOpacity * 255, stroke.R, stroke.G, stroke.B), strokeWidth);
    SolidBrush brush(Color(fillOpacity * 255, fill.R, fill.G, fill.B));

    graphics.FillPath(&brush, &path);
    graphics.DrawPath(&pen, &path);


}

void GroupSVG::drawSVG(Graphics& graphics) {
    for (const auto& element : elements) {
        if (element.type == GroupOrShape::GROUP) {
            element.group->drawSVG(graphics);
        }
        else if (element.type == GroupOrShape::SHAPE) {
            element.shape->drawSVG(graphics);
        }
    }
}


void RectSVG::getPointMINMAX(pointMinMax& pMM) {
    if (pMM.pointMin.x > this->p.x) pMM.pointMin.x = this->p.x;
    if (pMM.pointMin.y > this->p.y) pMM.pointMin.y = this->p.y;
    if (pMM.pointMax.x < this->p.x + this->width) pMM.pointMax.x = this->p.x + this->width;
    if (pMM.pointMax.y < this->p.y + this->height) pMM.pointMax.y = this->p.y + this->height;
}

void TextSVG::getPointMINMAX(pointMinMax& pMM) {
    if (pMM.pointMin.x > this->p.x) pMM.pointMin.x = this->p.x;
    if (pMM.pointMin.y > this->p.y - this->fontSize) pMM.pointMin.y = this->p.y - this->fontSize;
    if (pMM.pointMax.x < this->p.x + this->textContent.size() * this->fontSize) pMM.pointMax.x = this->p.x + this->textContent.size() * this->fontSize;
    if (pMM.pointMax.y < this->p.y) pMM.pointMax.y = this->p.y;
}

void CircleSVG::getPointMINMAX(pointMinMax& pMM) {
    if (pMM.pointMin.x > this->c.x - this->r) pMM.pointMin.x = this->c.x - this->r;
    if (pMM.pointMin.y > this->c.y - this->r) pMM.pointMin.y = this->c.y - this->r;
    if (pMM.pointMax.x < this->c.x + this->r) pMM.pointMax.x = this->c.x + this->r;
    if (pMM.pointMax.y < this->c.y + this->r) pMM.pointMax.y = this->c.y + this->r;
}

void EllipseSVG::getPointMINMAX(pointMinMax& pMM) {
    if (pMM.pointMin.x > this->c.x - this->rx) pMM.pointMin.x = this->c.x - this->rx;
    if (pMM.pointMin.y > this->c.y - this->ry) pMM.pointMin.y = this->c.y - this->ry;
    if (pMM.pointMax.x < (this->c.x - this->rx) + 2 * rx) pMM.pointMax.x = (this->c.x - this->rx) + 2 * rx;
    if (pMM.pointMax.y < (this->c.y - this->ry) + 2 * ry) pMM.pointMax.y = (this->c.y - this->ry) + 2 * ry;
}

void LineSVG::getPointMINMAX(pointMinMax& pMM) {
    if (pMM.pointMin.x > this->p1.x) pMM.pointMin.x = this->p1.x;
    if (pMM.pointMin.y > this->p1.y) pMM.pointMin.y = this->p1.y;
    if (pMM.pointMax.x < this->p1.x) pMM.pointMax.x = this->p1.x;
    if (pMM.pointMax.y < this->p1.y) pMM.pointMax.y = this->p1.y;
    if (pMM.pointMin.x > this->p2.x) pMM.pointMin.x = this->p2.x;
    if (pMM.pointMin.y > this->p2.y) pMM.pointMin.y = this->p2.y;
    if (pMM.pointMax.x < this->p2.x) pMM.pointMax.x = this->p2.x;
    if (pMM.pointMax.y < this->p2.y) pMM.pointMax.y = this->p2.y;
}

void PolygonSVG::getPointMINMAX(pointMinMax& pMM) {
    for (int i = 0; i < this->points.size(); i++)
    {
        if (pMM.pointMin.x > this->points[i].x) pMM.pointMin.x = this->points[i].x;
        if (pMM.pointMin.y > this->points[i].y) pMM.pointMin.y = this->points[i].y;
        if (pMM.pointMax.x < this->points[i].x) pMM.pointMax.x = this->points[i].x;
        if (pMM.pointMax.y < this->points[i].y) pMM.pointMax.y = this->points[i].y;
    }
}

void PolylineSVG::getPointMINMAX(pointMinMax& pMM) {
    for (int i = 0; i < this->points.size(); i++)
    {
        if (pMM.pointMin.x > this->points[i].x) pMM.pointMin.x = this->points[i].x;
        if (pMM.pointMin.y > this->points[i].y) pMM.pointMin.y = this->points[i].y;
        if (pMM.pointMax.x < this->points[i].x) pMM.pointMax.x = this->points[i].x;
        if (pMM.pointMax.y < this->points[i].y) pMM.pointMax.y = this->points[i].y;
    }
}

void PathSVG::getPointMINMAX(pointMinMax& pMM) {
    for (const auto& pointPath : PathData)
    {
        if (pointPath.typePointPath != 'A')
            for (const auto& point : pointPath.points) {
                if (pMM.pointMin.x > point.x) pMM.pointMin.x = point.x;
                if (pMM.pointMin.y > point.y) pMM.pointMin.y = point.y;
                if (pMM.pointMax.x < point.x) pMM.pointMax.x = point.x;
                if (pMM.pointMax.y < point.y) pMM.pointMax.y = point.y;
            }
        else {
            if (pMM.pointMin.x > pointPath.x - pointPath.rx) pMM.pointMin.x = pointPath.x - pointPath.rx;
            if (pMM.pointMin.y > pointPath.y - pointPath.ry) pMM.pointMin.y = pointPath.y - pointPath.ry;
            if (pMM.pointMax.x < (pointPath.x - pointPath.rx) + 2 * pointPath.rx) pMM.pointMax.x = (pointPath.x - pointPath.rx) + 2 * pointPath.rx;
            if (pMM.pointMax.y < (pointPath.y - pointPath.ry) + 2 * pointPath.ry) pMM.pointMax.y = (pointPath.y - pointPath.ry) + 2 * pointPath.ry;
        }
    }
}

void GroupSVG::getPointMINMAX(pointMinMax& ptMM) {
    for (const auto& element : elements) {
        if (element.type == GroupOrShape::SHAPE) {
            element.shape->getPointMINMAX(ptMM);
        }
        else if (element.type == GroupOrShape::GROUP) {
            element.group->getPointMINMAX(ptMM);
        }
    }
}
