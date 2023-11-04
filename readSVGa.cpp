#include "readSVGa.h"


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
                element.attributes[attr->name()] = attr->value();
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

    // Regular expression pattern to match the RGB values
    regex pattern("rgb\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");
    smatch match;

    if (regex_search(s, match, pattern) && match.size() == 4) {
        color.R = stod(match[1].str());
        color.G = stod(match[2].str());
        color.B = stod(match[3].str());
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

/*
#include "readSVG.h"

int main() {
    const string filename = "sample.svg";
    vector<SVGElement> elements = parseSVG(filename);

    // Print the elements
    printSVGElements(elements);
    for (const SVGElement& element : elements)
    {
        if (element.type == "rect") {
            RectSVG rectElement;
            rectElement.parseShapeSVG(element);
        }
        if (element.type == "text") {
            TextSVG textElement;
            textElement.parseShapeSVG(element);
        }
        if (element.type == "circle") {
            CircleSVG circleElement;
            circleElement.parseShapeSVG(element);
        }
        if (element.type == "ellipse") {
            EllipseSVG ellipseElement;
            ellipseElement.parseShapeSVG(element);
        }
        if (element.type == "line") {
            LineSVG lineElement;
            lineElement.parseShapeSVG(element);
        }
        if (element.type == "polygon") {
            PolygonSVG polygonElement;
            polygonElement.parseShapeSVG(element);
        }
        if (element.type == "polyline") {
            PolylineSVG polylineElement;
            polylineElement.parseShapeSVG(element);
        }
    }
    return 0;
}
*/