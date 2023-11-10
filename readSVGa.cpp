#include "readSVGa.h"

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

vector<SVGElement> parseSVG(const string& filename) {
    vector<SVGElement> elements;

    ifstream file(filename);
    if (!file) {
        cerr << "Failed to open the SVG file." << endl;
        return elements;
    }

    // Read the content of the SVG file into a string
    string xmlContent;
    string line;
    while (getline(file, line)) {
        xmlContent += line + ' ';
    }

    // Parse the XML content using RapidXML
    xml_document<> doc;
    doc.parse<0>(&xmlContent[0]);

    // Traverse the XML document and extract data from different SVG elements
    xml_node<>* svgNode = doc.first_node("svg");
    if (svgNode) {
        for (xml_node<>* node = svgNode->first_node(); node; node = node->next_sibling()) {
            SVGElement element;
            element.type = node->name();

            for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
                string attrName = attr->name();
                std::transform(attrName.begin(), attrName.end(), attrName.begin(), ::tolower);
                string attrVal = attr->value();
                std::transform(attrVal.begin(), attrVal.end(), attrVal.begin(), ::tolower);
                element.attributes[attrName] = attrVal;
                if (attrName == "style") parseStyle(attrVal, element);
            }

            // Check if the element is of type "text" and extract its content
            if (element.type == "text") {
                element.textContent = node->value();
            }
            elements.push_back(element);
        }
    }

    file.close();
    return elements;
}


void printSVGElements(const vector<SVGElement>& elements) {
    for (const SVGElement& element : elements) {
        cout << "Element type: " << element.type << endl;
        cout << "Attributes:" << endl;

        for (const auto& attribute : element.attributes) {
            cout << attribute.first << " = " << attribute.second << endl;
        }

        if (!element.textContent.empty()) {
            cout << "Text Content: " << element.textContent << endl;
        }

        cout << "-----------------------------" << endl;
    }
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
    istringstream iss(input);
    string pair;

    while (getline(iss, pair, ' ')) {
        istringstream pairStream(pair);
        double x, y;

        if (getline(pairStream, pair, ',') && getline(pairStream, pair, ',')) {
            x = stod(pair);

            if (getline(pairStream, pair, ',')) {
                y = stod(pair);
                PointSVG point = { x, y };
                points.push_back(point);
            }
        }
    }

    return points;
}

void RectSVG::parseShapeSVG(const SVGElement& element) {
    for (const auto& attr : element.attributes) {
        if (attr.first == "x") {
            x = stod(attr.second);
        }
        else if (attr.first == "y") {
            y = stod(attr.second);
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
            fill = colorSVG(attr.second);
        }
        else if (attr.first == "fill-opacity") {
            fillOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke") {
            stroke = colorSVG(attr.second);
        }
        else if (attr.first == "stroke-opacity") {
            strokeOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke-width") {
            strokeWidth = stod(attr.second);
        }
        else if (attr.first == "transform") {
            transform = attr.second;
        }
        else if (attr.first == "style") {
            style = attr.second;
        }
    }
}

void TextSVG::parseShapeSVG(const SVGElement& element) {
    for (const auto& attr : element.attributes) {
        if (attr.first == "x") {
            x = stod(attr.second);
        }
        else if (attr.first == "y") {
            y = stod(attr.second);
        }
        else if (attr.first == "font-size") {
            fontSize = stod(attr.second);
        }
        else if (attr.first == "font-weight") {
            fontWeight = stod(attr.second);
        }
        else if (attr.first == "font-style") {
            fontStyle = attr.second;
        }
        else if (attr.first == "font-family") {
            fontFamily = attr.second;
        }
        else if (attr.first == "text-anchor") {
            textAnchor = attr.second;
        }
        else if (attr.first == "text-decoration") {
            textDecoration = attr.second;
        }
        else if (attr.first == "text-transform") {
            textTransform = attr.second;
        }
    }

    // Store the text content from the SVG element
    textContent = element.textContent;
}

void CircleSVG::parseShapeSVG(const SVGElement& element) {
    for (const auto& attr : element.attributes) {
        if (attr.first == "stroke-width") {
            strokeWidth = stod(attr.second);
        }
        else if (attr.first == "cx") {
            cx = stod(attr.second);
        }
        else if (attr.first == "cy") {
            cy = stod(attr.second);
        }
        else if (attr.first == "r") {
            r = stod(attr.second);
        }
        else if (attr.first == "fill") {
            fill = colorSVG(attr.second);
        }
        else if (attr.first == "fill-opacity") {
            fillOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke") {
            stroke = colorSVG(attr.second);
        }
        else if (attr.first == "stroke-opacity") {
            strokeOpacity = stod(attr.second);
        }
        else if (attr.first == "transform") {
            transform = attr.second;
        }
        else if (attr.first == "style") {
            style = attr.second;
        }
    }
}

void EllipseSVG::parseShapeSVG(const SVGElement& element) {
    for (const auto& attr : element.attributes) {
        if (attr.first == "cx") {
            cx = stod(attr.second);
        }
        else if (attr.first == "cy") {
            cy = stod(attr.second);
        }
        else if (attr.first == "rx") {
            rx = stod(attr.second);
        }
        else if (attr.first == "ry") {
            ry = stod(attr.second);
        }
        else if (attr.first == "fill") {
            fill = colorSVG(attr.second);
        }
        else if (attr.first == "fill-opacity") {
            fillOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke") {
            stroke = colorSVG(attr.second);
        }
        else if (attr.first == "stroke-opacity") {
            strokeOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke-width") {
            strokeWidth = stod(attr.second);
        }
        else if (attr.first == "transform") {
            transform = attr.second;
        }
        else if (attr.first == "style") {
            style = attr.second;
        }
    }
}

void LineSVG::parseShapeSVG(const SVGElement& element) {
    for (const auto& attr : element.attributes) {
        if (attr.first == "x1") {
            x1 = stod(attr.second);
        }
        else if (attr.first == "y1") {
            y1 = stod(attr.second);
        }
        else if (attr.first == "x2") {
            x2 = stod(attr.second);
        }
        else if (attr.first == "y2") {
            y2 = stod(attr.second);
        }
        else if (attr.first == "stroke") {
            stroke = colorSVG(attr.second);
        }
        else if (attr.first == "stroke-opacity") {
            strokeOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke-width") {
            strokeWidth = stod(attr.second);
        }
        else if (attr.first == "transform") {
            transform = attr.second;
        }
        else if (attr.first == "style") {
            style = attr.second;
        }
    }
}

void PolygonSVG::parseShapeSVG(const SVGElement& element) {
    for (const auto& attr : element.attributes) {
        if (attr.first == "points") {
            points = parsePointString(attr.second);
        }
        else if (attr.first == "fill") {
            fill = colorSVG(attr.second);
        }
        else if (attr.first == "fill-opacity") {
            fillOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke") {
            stroke = colorSVG(attr.second);
        }
        else if (attr.first == "stroke-opacity") {
            strokeOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke-width") {
            strokeWidth = stod(attr.second);
        }
        else if (attr.first == "transform") {
            transform = attr.second;
        }
        else if (attr.first == "style") {
            style = attr.second;
        }
    }
}
void PolylineSVG::parseShapeSVG(const SVGElement& element) {
    for (const auto& attr : element.attributes) {
        if (attr.first == "points") {
            points = parsePointString(attr.second);
        }
        else if (attr.first == "fill") {
            fill = colorSVG(attr.second);
        }
        else if (attr.first == "fill-opacity") {
            fillOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke") {
            stroke = colorSVG(attr.second);
        }
        else if (attr.first == "stroke-opacity") {
            strokeOpacity = stod(attr.second);
        }
        else if (attr.first == "stroke-width") {
            strokeWidth = stod(attr.second);
        }
        else if (attr.first == "transform") {
            transform = attr.second;
        }
        else if (attr.first == "style") {
            style = attr.second;
        }
    }
}

void RectSVG::getPointMINMAX(pointMinMax& pMM) {
    if (pMM.pointMin.x > this->x) pMM.pointMin.x = this->x;
    if (pMM.pointMin.y > this->y) pMM.pointMin.y = this->y;
    if (pMM.pointMax.x < this->x + this->width) pMM.pointMax.x = this->x + this->width;
    if (pMM.pointMax.y < this->y + this->height) pMM.pointMax.y = this->y + this->height;
}
}

void TextSVG::getPointMINMAX(pointMinMax& pMM) {
    if (pMM.pointMin.x > this->x) pMM.pointMin.x = this->x;
    if (pMM.pointMin.y > this->y - this->fontSize) pMM.pointMin.y = this->y - this->fontSize;
    if (pMM.pointMax.x < this->x + this->textContent.size()*this->fontSize) pMM.pointMax.x = this->x + this->textContent.size() * this->fontSize;
    if (pMM.pointMax.y < this->y) pMM.pointMax.y = this->y;
}

void CircleSVG::getPointMINMAX(pointMinMax& pMM) {
    if (pMM.pointMin.x > this->cx - this->r) pMM.pointMin.x = this->cx - this->r;
    if (pMM.pointMin.y > this->cy - this->r) pMM.pointMin.y = this->cy - this->r;
    if (pMM.pointMax.x < this->cx + this->r) pMM.pointMax.x = this->cx + this->r;
    if (pMM.pointMax.y < this->cy + this->r) pMM.pointMax.y = this->cy + this->r;
}

void EllipseSVG::getPointMINMAX(pointMinMax& pMM) {
    if (pMM.pointMin.x > this->cx - this->rx) pMM.pointMin.x = this->cx - this->rx;
    if (pMM.pointMin.y > this->cy - this->ry) pMM.pointMin.y = this->cy - this->ry;
    if (pMM.pointMax.x < (this->cx - this->rx) + 2 * rx) pMM.pointMax.x = (this->cx - this->rx) + 2 * rx;
    if (pMM.pointMax.y < (this->cy - this->ry) + 2 * ry) pMM.pointMax.y = (this->cy - this->ry) + 2 * ry;
}

void LineSVG::getPointMINMAX(pointMinMax& pMM) {
    if (pMM.pointMin.x > this->x1) pMM.pointMin.x = this->x1;
    if (pMM.pointMin.y > this->y1) pMM.pointMin.y = this->y1;
    if (pMM.pointMax.x < this->x1) pMM.pointMax.x = this->x1;
    if (pMM.pointMax.y < this->y1) pMM.pointMax.y = this->y1;
    if (pMM.pointMin.x > this->x2) pMM.pointMin.x = this->x2;
    if (pMM.pointMin.y > this->y2) pMM.pointMin.y = this->y2;
    if (pMM.pointMax.x < this->x2) pMM.pointMax.x = this->x2;
    if (pMM.pointMax.y < this->y2) pMM.pointMax.y = this->y2;
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

/*
#include "readSVG.h"

int main() {
    const string filename = "sample.svg";
    vector<SVGElement> elements = parseSVG(filename);
    pointMinMax ptMM;
    printSVGElements(elements);
    for (const SVGElement& element : elements)
    {
        if (element.type == "rect") {
            RectSVG rectElement;
            rectElement.parseShapeSVG(element);
            rectElement.getPointMINMAX(ptMM);
        }
        if (element.type == "text") {
            TextSVG textElement;
            textElement.parseShapeSVG(element);
            textElement.getPointMINMAX(ptMM);
        }
        if (element.type == "circle") {
            CircleSVG circleElement;
            circleElement.parseShapeSVG(element);
            circleElement.getPointMINMAX(ptMM);
        }
        if (element.type == "ellipse") {
            EllipseSVG ellipseElement;
            ellipseElement.parseShapeSVG(element);
            ellipseElement.getPointMINMAX(ptMM);
        }
        if (element.type == "line") {
            LineSVG lineElement;
            lineElement.parseShapeSVG(element);
            lineElement.getPointMINMAX(ptMM);
        }
        if (element.type == "polygon") {
            PolygonSVG polygonElement;
            polygonElement.parseShapeSVG(element);
            polygonElement.getPointMINMAX(ptMM);
        }
        if (element.type == "polyline") {
            PolylineSVG polylineElement;
            polylineElement.parseShapeSVG(element);
            polylineElement.getPointMINMAX(ptMM);
        }
    }
    return 0;
}
*/
