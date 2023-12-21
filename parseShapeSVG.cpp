#include "stdafx.h"
#include "classSVG.h"

fstream fop1("Text2.txt");

void printSVGElement(const SVGElement& element) {
    fop1 << "Type: " << element.type << endl;
    fop1 << "Attributes:" << endl;
    for (const auto& pair : element.attributes) {
        fop1 << "  " << pair.first << ": " << pair.second << endl;
    }
    fop1 << "Text Content: " << element.textContent << endl;
    fop1 << "Children:" << endl;
    for (const auto& child : element.children) {
        printSVGElement(child);
    }
    fop1 << "end Children" << endl << endl;

}

void parseStopSVG(vector<Stop>& stops, const SVGElement& input) {
    Stop stop;
    for (const auto& pair : input.attributes) {
        if (pair.first == "offset") {

            stop.offset = stod(pair.second);
            size_t found = pair.second.find('%');
            if (found != std::string::npos) {
                stop.offset /= 100;
            }
        }
        else if (pair.first == "stop-color") {
            bool cf;
            stop.stopColor = colorSVG(pair.second, cf);
        }
        else if (pair.first == "stop-opacity") {
            stop.stopOpacity = stod(pair.second);
        }
    }
    stops.push_back(stop);
}
void parseGradientSVG(vector<Gradient>& gradients, const SVGElement& input) {
    // Check if the id already exists
    const string id = "url(#" + input.attributes.at("id") + ")";
    auto existingGradient = std::find_if(gradients.begin(), gradients.end(),
        [&](const Gradient& g) { return g.id == id; });

    if (existingGradient != gradients.end()) {
        return;
    }

    Gradient gradient;
    gradient.id = id;
    string s = input.type;

    gradient.typeGradient = s;

    if (gradient.typeGradient == "lineargradient" || gradient.typeGradient == "linearGradient") {
        for (const auto& attr : input.attributes)
            if (attr.first == "x1") {
                gradient.x1 = stod(attr.second);
                size_t found = attr.second.find('%');
                if (found != std::string::npos) {
                    gradient.x1 /= 100;
                }
            }
            else if (attr.first == "y1") {
                gradient.y1 = stod(attr.second);
                size_t found = attr.second.find('%');
                if (found != std::string::npos) {
                    gradient.y1 /= 100;
                }
            }
            else if (attr.first == "x2") {
                gradient.x2 = stod(attr.second);
                size_t found = attr.second.find('%');
                if (found != std::string::npos) {
                    gradient.x1 /= 100;
                }
            }
            else if (attr.first == "y2") {
                gradient.y2 = stod(attr.second);
                size_t found = attr.second.find('%');
                if (found != std::string::npos) {
                    gradient.y2 /= 100;
                }
            }
    }
    else if (gradient.typeGradient == "radialgradient" || gradient.typeGradient == "radialGradient") {
        for (const auto& attr : input.attributes)
            if (attr.first == "cx") {
                gradient.cx = stod(attr.second);
                size_t found = attr.second.find('%');
                if (found != std::string::npos) {
                    gradient.cx /= 100;
                }
            }
            else if (attr.first == "cy") {
                gradient.cy = stod(attr.second);
                size_t found = attr.second.find('%');
                if (found != std::string::npos) {
                    gradient.cy /= 100;
                }
            }
            else if (attr.first == "r") {
                gradient.r = stod(attr.second);
                size_t found = attr.second.find('%');
                if (found != std::string::npos) {
                    gradient.r /= 100;
                }
            }
            else if (attr.first == "fx") {
                gradient.fx = stod(attr.second);
                size_t found = attr.second.find('%');
                if (found != std::string::npos) {
                    gradient.fx /= 100;
                }
            }
            else if (attr.first == "fy") {
                gradient.fy = stod(attr.second);
                size_t found = attr.second.find('%');
                if (found != std::string::npos) {
                    gradient.fy /= 100;
                }
            }
    }
    for (const auto& attr : input.attributes)
    {
        if (attr.first == "xlink:href")
        {
            gradient.xlink = "url(" + attr.second + ")";
        }

    }
    for (const auto& gradientc : gradients) {
        if (gradient.xlink == gradientc.id)
            gradient.stops = gradientc.stops;
    }
    for (const SVGElement& child : input.children) {
        if (child.type == "stop") {
            Stop stop;
            parseStopSVG(gradient.stops, child);
        }
    }

    gradients.push_back(gradient);
}
void parseGradientSVG2(vector<Gradient>& gradients,  SVGElement& input) {
    // Check if the id already exists
    const string id = "url(#" + input.attributes.at("id") + ")";
    Gradient gradient;
    gradient.id = id;
    string s = input.type;

    gradient.typeGradient = s;

    for (const auto& attr : input.attributes)
    {
        if (attr.first == "xlink:href")
        {
            gradient.xlink = "url(" + attr.second + ")";
        }
    }
    for (const auto& gradientc : gradients) {
        if (gradient.xlink == gradientc.id)
            gradient.stops = gradientc.stops;
    }
    for (const SVGElement& child : input.children) {
        if (child.type == "stop") {
            Stop stop;
            parseStopSVG(gradient.stops, child);
        }
    }
    for ( auto& gradientc : gradients) {
        if (gradient.id == gradientc.id && input.parseYes == 0) {
            gradientc.stops = gradient.stops;
            input.parseYes = 1;
            break;
        }
            
    }    
}
fstream fop("Text1.txt");
void printGradientSVG(const vector<Gradient>& gradients) {
    for (const auto& gradient : gradients) {
        fop << "Gradient ID: " << gradient.id << endl;
        fop << "Gradient Type: " << gradient.typeGradient << endl;
        if (gradient.typeGradient == "lineargradient" || gradient.typeGradient == "linearGradient") {
            fop << "x1: " << gradient.x1 << ", y1: " << gradient.y1 << endl;
            fop << "x2: " << gradient.x2 << ", y2: " << gradient.y2 << endl;
        }
        else if (gradient.typeGradient == "radialgradient" || gradient.typeGradient == "radialGradient") {
            fop << "cx: " << gradient.cx << ", cy: " << gradient.cy << endl;
            fop << "r: " << gradient.r << ", fx: " << gradient.fx << ", fy: " << gradient.fy << endl;
        }
        fop << "Stops:" << endl;
        for (const auto& stop : gradient.stops) {
            fop << "  Offset: " << stop.offset << ", Color: " << stop.stopColor.R << " " << stop.stopColor.G << " " << stop.stopColor.B << ", Opacity: " << stop.stopOpacity << endl;
        }
        fop << endl;
    }
}

string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

bool isUrl(const std::string& str) {
    std::regex pattern("^url\\(#[^)]+\\)$");
    return std::regex_match(str, pattern);
}
void ShapeSVG::parseDataSVG(string attribute, string data, bool& cf, bool& cs, vector<Gradient>& Gradients) {
    if (attribute == "fill") {
        string s = trim(data);
        string result;
        for (char c : s) {
            if (c != '\n' || c != '\t' || c != ' ') {
                result += c;
            }
        }
        s = result;
        if (isUrl(s)) {
            hasGradientFill = 1;
            bool found = false;
            for (const auto& gradient : Gradients) {
                if (gradient.id == s) {
                    found = true;
                    cf = 1;
                    Gfill = gradient;
                    fill = Gfill.stops[0].stopColor;
                    break;
                }
            }
        }
        else
            fill = colorSVG(s, cf);
    }
    else if (attribute == "fill-opacity") {
        fillOpacity = stod(data);
    }
    else if (attribute == "stroke") {
        string s = trim(data);
        string result;
        for (char c : s) {
            if (c != '\n' || c != '\t' || c != ' ') {
                result += c;
            }
        }
        s = result;
        if (isUrl(s)) {
            hasGradientStroke = 1;
            bool found = false;
            for (const auto& gradient : Gradients) {
                if (gradient.id == s) {
                    found = true;
                    cs = 1;
                    Gstroke = gradient;
                    stroke = Gstroke.stops[0].stopColor;
                    break;
                }
            }
        }
        else
            stroke = colorSVG(s, cs);
    }
    else if (attribute == "stroke-opacity") {
        strokeOpacity = stod(data);
    }
    else if (attribute == "stroke-width") {
        strokeWidth = stod(data);
    }
    else if (attribute == "transform") {
        parsetransformSVG(tfSVG, data);
    }
    else if (attribute == "style") {
        style = data;
    }
    else if (attribute == "dx") {
        dx = stod(data);
    }
    else if (attribute == "dy") {
        dy = stod(data);
    }
}

void RectSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke, vector<Gradient>& Gradients) {
    for (const auto& attr : element.attributes) {
        if (attr.first == "x") {
            p.X = stod(attr.second);
        }
        else if (attr.first == "y") {
            p.Y = stod(attr.second);
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
        else this->parseDataSVG(attr.first, attr.second, cFill, cStroke, Gradients);
    }
    if (cStroke == 0) strokeOpacity = 0;
    if (cFill == 0) fillOpacity = 0;
}

string removeLeadingSpaces(const string& inputString) {
    size_t index = 0;
    while (index < inputString.length() && isspace(inputString[index])) {
        index++;
    }

    return inputString.substr(index);
}

void TextSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke, vector<Gradient>& Gradients) {
    for (const auto& attr : element.attributes) {
        if (attr.first == "x") {
            p.X = stod(attr.second);
        }
        else if (attr.first == "y") {
            p.Y = stod(attr.second);
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
            string s = fontFamily;
            std::transform(fontFamily.begin(), fontFamily.end(), fontFamily.begin(), ::tolower);
            LPCWSTR lpcwstr = L"";
            if (!s.empty()) {
                int size_needed = MultiByteToWideChar(CP_UTF8, 0, &s[0], (int)s.size(), NULL, 0);
                std::wstring wstrTo(size_needed, 0);
                MultiByteToWideChar(CP_UTF8, 0, &s[0], (int)s.size(), &wstrTo[0], size_needed);
                lpcwstr = wstrTo.c_str();
            }
            if (!AddFontResource(lpcwstr)) fontFamily = "Times New Roman";
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
        else this->parseDataSVG(attr.first, attr.second, cFill, cStroke, Gradients);

    }
    string s = element.textContent;
    string result;
    for (char c : s) {
        if (c != '\n' || c != '\t') {
            result += c;
        }
    }
    result = removeLeadingSpaces(result);
    textContent = result;
    if (cFill == 0) fillOpacity = 0;
    if (cStroke == 0) strokeOpacity = 0;

}
void CircleSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke, vector<Gradient>& Gradients) {
    for (const auto& attr : element.attributes) {
        if (attr.first == "stroke-width") {
            strokeWidth = stod(attr.second);
        }
        else if (attr.first == "cx") {
            c.X = stod(attr.second);
        }
        else if (attr.first == "cy") {
            c.Y = stod(attr.second);
        }
        else if (attr.first == "r") {
            r = stod(attr.second);
        }
        else this->parseDataSVG(attr.first, attr.second, cFill, cStroke, Gradients);;

    }
    if (cStroke == 0) strokeOpacity = 0;
    if (cFill == 0) fillOpacity = 0;
}
void EllipseSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke, vector<Gradient>& Gradients) {
    for (const auto& attr : element.attributes) {
        if (attr.first == "cx") {
            c.X = stod(attr.second);
        }
        else if (attr.first == "cy") {
            c.Y = stod(attr.second);
        }
        else if (attr.first == "rx") {
            rx = stod(attr.second);
        }
        else if (attr.first == "ry") {
            ry = stod(attr.second);
        }
        else this->parseDataSVG(attr.first, attr.second, cFill, cStroke, Gradients);;
    }
    if (cStroke == 0) strokeOpacity = 0;
    if (cFill == 0) fillOpacity = 0;

}
void LineSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke, vector<Gradient>& Gradients) {
    for (const auto& attr : element.attributes) {
        if (attr.first == "x1") {
            p1.X = stod(attr.second);
        }
        else if (attr.first == "y1") {
            p1.Y = stod(attr.second);
        }
        else if (attr.first == "x2") {
            p2.X = stod(attr.second);
        }
        else if (attr.first == "y2") {
            p2.Y = stod(attr.second);
        }
        else this->parseDataSVG(attr.first, attr.second, cFill, cStroke, Gradients);;
    }
    if (cStroke == 0) strokeOpacity = 0;
    if (cFill == 0) fillOpacity = 0;
}
void PolygonSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke, vector<Gradient>& Gradients) {
    for (const auto& attr : element.attributes) {
        if (attr.first == "points") {
            points = parsePointString(attr.second);
        }
        else this->parseDataSVG(attr.first, attr.second, cFill, cStroke, Gradients);;
    }
    if (cStroke == 0) strokeOpacity = 0;
    if (cFill == 0) fillOpacity = 0;
}
void PolylineSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke, vector<Gradient>& Gradients) {
    for (const auto& attr : element.attributes) {
        if (attr.first == "points") {
            points = parsePointString(attr.second);
        }
        else this->parseDataSVG(attr.first, attr.second, cFill, cStroke, Gradients);

    }
    if (cStroke == 0) strokeOpacity = 0;
    if (cFill == 0) fillOpacity = 0;
}
fstream fo("input.txt");
void PathSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke, vector<Gradient>& Gradients) {
    for (const auto& attr : element.attributes) {
        if (attr.first == "d") {
            PathData = parsePathData(attr.second);
            for (const auto& path : PathData) {
                fo << "Type: " << path.typePointPath << ", Points: ";
                for (const auto& point : path.points) {
                    fo << "(" << point.X << ", " << point.Y << ") ";
                }
                fo << std::endl;
            }
        }
        else this->parseDataSVG(attr.first, attr.second, cFill, cStroke, Gradients);;
    }
    if (cStroke == 0) strokeOpacity = 0;
    if (cFill == 0) fillOpacity = 0;
}

void ShapeSVG::copyAttributes(const ShapeSVG& other) {
    //fillOpacity = other.fillOpacity;
    //strokeOpacity = other.strokeOpacity;
    //fill = other.fill;
    //stroke = other.stroke;
    //strokeWidth = other.strokeWidth;
    //style = other.style;
    //if (other.tfSVG.size() != 0)
    for (const auto& tf : other.tfSVG)
        tfSVG.push_back(tf);
}

SVGElement addSVGElement(const SVGElement& e1, const SVGElement& e2) {
    SVGElement result;

    // Combine type
    result.type = e2.type;

    // Combine attributes
    result.attributes = e2.attributes;
    for (const auto& attr : e1.attributes) {
        // If attribute doesn't exist in e2 and is not "transform", add it
        if (result.attributes.find(attr.first) == result.attributes.end() && attr.first != "transform") {
            result.attributes[attr.first] = attr.second;
        }
        // If attribute is "transform", append the values
        else if (attr.first == "transform") {
            string s = attr.second + " " + result.attributes[attr.first];
            result.attributes[attr.first] = s;
        }
        else if (attr.first == "d") {
            string s = attr.second + " " + result.attributes[attr.first];
            result.attributes[attr.first] = s;
        }
    }

    // Combine text content
    result.textContent = e2.textContent;

    // Combine children (take children from e2)
    result.children = e2.children;

    return result;
}

//void GroupSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke, vector<Gradient>& Gradient) {
//    for (const auto& attr : element.attributes) {
//        this->parseDataSVG(attr.first, attr.second, cFill, cStroke);
//    }
//    //if (cStroke == 0) strokeOpacity = 0;
//    //if (cFill == 0) fillOpacity = 0;
//    for (const SVGElement& childElement : element.children) {
//        GroupOrShape elementToAdd;
//        if (childElement.type == "g") {
//            elementToAdd.type = GroupOrShape::GROUP;
//            elementToAdd.group = make_unique<GroupSVG>();
//            elementToAdd.group->copyAttributes(*this);
//            elementToAdd.group->parseShapeSVG(childElement, cFill, cStroke);
//        }
//        else {
//            elementToAdd.type = GroupOrShape::SHAPE;
//            if (childElement.type == "rect") {
//                elementToAdd.shape = make_unique<RectSVG>();
//            }
//            else if (childElement.type == "text") {
//                elementToAdd.shape = make_unique<TextSVG>();
//            }
//            else if (childElement.type == "circle") {
//                elementToAdd.shape = make_unique<CircleSVG>();
//            }
//            else if (childElement.type == "ellipse") {
//                elementToAdd.shape = make_unique<EllipseSVG>();
//            }
//            else if (childElement.type == "line") {
//                elementToAdd.shape = make_unique<LineSVG>();
//            }
//            else if (childElement.type == "polygon") {
//                elementToAdd.shape = make_unique<PolygonSVG>();
//            }
//            else if (childElement.type == "polyline") {
//                elementToAdd.shape = make_unique<PolylineSVG>();
//            }
//            else if (childElement.type == "path") {
//                elementToAdd.shape = make_unique<PathSVG>();
//            }
//            else {
//                continue;
//            }
//            elementToAdd.shape->copyAttributes(*this);
//            elementToAdd.shape->CheckinGroup();
//            elementToAdd.shape->parseShapeSVG(childElement, cFill, cStroke);
//        }
//
//        elements.push_back(std::move(elementToAdd));
//    }
//}


void GroupSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke, vector<Gradient>& Gradient) {
    for (const SVGElement& childElement : element.children) {
        GroupOrShape elementToAdd;
        if (childElement.type == "g") {
            elementToAdd.type = GroupOrShape::GROUP;
            elementToAdd.group = make_unique<GroupSVG>();
            SVGElement childElement1 = addSVGElement(element, childElement);
            elementToAdd.group->parseShapeSVG(childElement1, 1, 0, Gradient);
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
            elementToAdd.shape->CheckinGroup();
            SVGElement childElement1 = addSVGElement(element, childElement);
            elementToAdd.shape->parseShapeSVG(childElement1, 1, 0, Gradient);
        }

        elements.push_back(std::move(elementToAdd));
    }
}
