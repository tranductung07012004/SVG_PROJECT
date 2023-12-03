#include "stdafx.h"
#include "classSVG.h"



void ShapeSVG::parseDataSVG(string attribute, string data, bool& cf, bool& cs) {
    if (attribute == "fill") {
        fill = colorSVG(data, cf);
    }
    else if (attribute == "fill-opacity") {
        fillOpacity = stod(data);
    }
    else if (attribute == "stroke") {
        stroke = colorSVG(data, cs);
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

void RectSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) {
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
        else this->parseDataSVG(attr.first, attr.second, cFill, cStroke);
    }
    if (cStroke == 0) strokeOpacity = 0;
    if (cFill == 0) fillOpacity = 0;

}
void TextSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) {
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
        else this->parseDataSVG(attr.first, attr.second, cFill, cStroke);

    }
    string s = element.textContent;
    string result;
    for (char c : s) {
        if (c != '\n') {
            result += c;
        }
    }
    textContent = result;
    if (cFill == 0) fillOpacity = 0;
    if (cStroke == 0) strokeOpacity = 0;

}
void CircleSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) {
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
        else this->parseDataSVG(attr.first, attr.second, cFill, cStroke);;

    }
    if (cStroke == 0) strokeOpacity = 0;
    if (cFill == 0) fillOpacity = 0;
}
void EllipseSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) {
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
        else this->parseDataSVG(attr.first, attr.second, cFill, cStroke);;
    }
    if (cStroke == 0) strokeOpacity = 0;
    if (cFill == 0) fillOpacity = 0;

}
void LineSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) {
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
        else this->parseDataSVG(attr.first, attr.second, cFill, cStroke);;
    }
    if (cStroke == 0) strokeOpacity = 0;
    if (cFill == 0) fillOpacity = 0;
}
void PolygonSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) {
    for (const auto& attr : element.attributes) {
        if (attr.first == "points") {
            points = parsePointString(attr.second);
        }
        else this->parseDataSVG(attr.first, attr.second, cFill, cStroke);;
    }
    if (cStroke == 0) strokeOpacity = 0;
    if (cFill == 0) fillOpacity = 0;
}
void PolylineSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) {
    for (const auto& attr : element.attributes) {
        if (attr.first == "points") {
            points = parsePointString(attr.second);
        }
        else this->parseDataSVG(attr.first, attr.second, cFill, cStroke);

    }
    if (cStroke == 0) strokeOpacity = 0;
    if (cFill == 0) fillOpacity = 0;
}
fstream fo("input.txt");
void PathSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) {
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
        else this->parseDataSVG(attr.first, attr.second, cFill, cStroke);;
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

//void GroupSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) {
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


void GroupSVG::parseShapeSVG(const SVGElement& element, bool cFill, bool cStroke) {
    for (const SVGElement& childElement : element.children) {
        GroupOrShape elementToAdd;
        if (childElement.type == "g") {
            elementToAdd.type = GroupOrShape::GROUP;
            elementToAdd.group = make_unique<GroupSVG>();
            SVGElement childElement1 = addSVGElement(element, childElement);
            elementToAdd.group->parseShapeSVG(childElement1, 1, 0);
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
            elementToAdd.shape->parseShapeSVG(childElement1, 1, 0);
        }

        elements.push_back(std::move(elementToAdd));
    }
}
