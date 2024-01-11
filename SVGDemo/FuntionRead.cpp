#include "stdafx.h"
#include "FunctionRead.h"

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



void parseGradientNode(xml_node<>* node, SVGElement& gradientElement) {
    for (xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
        SVGElement element;
        string s = child->name();
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        element.type = s;

        if (element.type == "stop") {
            for (xml_attribute<>* attr = child->first_attribute(); attr; attr = attr->next_attribute()) {
                string attrName = attr->name();
                transform(attrName.begin(), attrName.end(), attrName.begin(), ::tolower);
                string attrVal = attr->value();
                if (element.attributes[attrName] == "")
                    element.attributes[attrName] = attrVal;
                if (attrName == "style") parseStyle(attrVal, element);
            }
            gradientElement.children.push_back(element);
        }
    }
}

void parseDefsNode(xml_node<>* node, SVGElement& defsElement) {
    for (xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
        SVGElement element;
        string s = child->name();
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        element.type = s;


        // Parse attributes of the child element
        for (xml_attribute<>* attr = child->first_attribute(); attr; attr = attr->next_attribute()) {
            string attrName = attr->name();
            transform(attrName.begin(), attrName.end(), attrName.begin(), ::tolower);
            string attrVal = attr->value();
            if (element.attributes[attrName] == "")
                element.attributes[attrName] = attrVal;
            if (attrName == "style") parseStyle(attrVal, element);
        }

        // Check if the child element is of type "text" and extract its content
        if (element.type == "text") {
            element.textContent = child->value();
        }

        // Recursively handle nested elements
        if (element.type == "radialgradient" || element.type == "lineargradient" || element.type == "linearGradient" || element.type == "radialGradient") {
            parseGradientNode(child, element);
        }
        if (element.type == "g") {
            parseGroupNode(child, element);
        }

        // Add the parsed element to the list of children of the defsElement
        defsElement.children.push_back(element);
    }
}

void parseSVGNode(xml_node<>* node, vector<SVGElement>& elements) {
    for (xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
        SVGElement element;
        string s= child->name();
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        element.type =s;

        for (xml_attribute<>* attr = child->first_attribute(); attr; attr = attr->next_attribute()) {
            string attrName = attr->name();
            transform(attrName.begin(), attrName.end(), attrName.begin(), ::tolower);
            string attrVal = attr->value();
            if (element.attributes[attrName] == "")
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
        if (element.type == "radialgradient" || element.type == "lineargradient" || element.type == "linearGradient" || element.type == "radialGradient") {
            parseGradientNode(child, element);
        }
        if (element.type == "defs") {
            parseDefsNode(child, element);
        }
        elements.push_back(element);
    }
}




void parseGroupNode(xml_node<>* node, SVGElement& groupElement) {
    for (xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
        SVGElement element;
        string s = child->name();
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        element.type = s;
        for (xml_attribute<>* attr = child->first_attribute(); attr; attr = attr->next_attribute()) {
            string attrName = attr->name();
            transform(attrName.begin(), attrName.end(), attrName.begin(), ::tolower);
            string attrVal = attr->value();
            if (element.attributes[attrName] == "")
                element.attributes[attrName] = attrVal;
            if (attrName == "style") parseStyle(attrVal, element);
        }

        // Check if the element is of type "text" and extract its content
        if (element.type == "text") {
            element.textContent = child->value();
        }
        if (element.type == "radialgradient" || element.type == "lineargradient" || element.type == "linearGradient" || element.type == "radialGradient") {
            parseGradientNode(child, element);
        }
        if (element.type == "g") {
            parseGroupNode(child, element);
        }
        if (element.type == "defs") {
            parseDefsNode(child, element);
        }
        groupElement.children.push_back(element);
    }
}
vector<SVGElement> parseSVG(const string& filename, double& width, double& height, double& minX, double& minY, double& maxX, double& maxY) {
    vector<SVGElement> result;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file '" << filename << "'" << endl;
        return result;  // Return an empty vector on error
    }
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    string content = buffer.str();

    xml_document<> doc;
    doc.parse<0>(&content[0]);

    xml_node<>* svgNode = doc.first_node("svg");
    if (svgNode) {
        for (xml_attribute<>* attr = svgNode->first_attribute(); attr; attr = attr->next_attribute()) {
            string attrName = attr->name();
            transform(attrName.begin(), attrName.end(), attrName.begin(), ::tolower);
            string attrValue = attr->value();

            if (attrName == "width") {
                width = stod(attrValue);
            }
            else if (attrName == "height") {
                height = stod(attrValue);
            }
            else if (attrName == "viewbox") {
                string result;
                for (char c : attrValue) {
                    if (c == '\n' || c == ',') {
                        result += ' ';
                    }
                    else result += c;
                }
                istringstream viewBoxStream(result);
                viewBoxStream >> minX >> minY >> maxX >> maxY;
            }
        }

        // Parse the rest of the SVG elements
        parseSVGNode(svgNode, result);
    }
    else {
        cerr << "Error: Missing 'svg' element in the SVG file" << endl;
    }
    return result;
}