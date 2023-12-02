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
void parseSVGNode(xml_node<>* node, vector<SVGElement>& elements) {
    for (xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
        SVGElement element;
        element.type = child->name();

        for (xml_attribute<>* attr = child->first_attribute(); attr; attr = attr->next_attribute()) {
            string attrName = attr->name();
            transform(attrName.begin(), attrName.end(), attrName.begin(), ::tolower);
            string attrVal = attr->value();
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
void parseGroupNode(xml_node<>* node, SVGElement& groupElement) {
    for (xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
        SVGElement element;
        element.type = child->name();

        for (xml_attribute<>* attr = child->first_attribute(); attr; attr = attr->next_attribute()) {
            string attrName = attr->name();
            transform(attrName.begin(), attrName.end(), attrName.begin(), ::tolower);
            string attrVal = attr->value();
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
vector<SVGElement> parseSVG(const string& filename, double& width, double& height) {
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
        if (xml_attribute<>* widthAttr = svgNode->first_attribute("width")) {
            width = stod(widthAttr->value());
        }
        if (xml_attribute<>* heightAttr = svgNode->first_attribute("height")) {
            height = stod(heightAttr->value());
        }

        // Parse the rest of the SVG elements
        parseSVGNode(svgNode, result);
    }
    if (svgNode) {
        // Extract width and height attributes
        if (xml_attribute<>* widthAttr = svgNode->first_attribute("width")) {
            width = stod(widthAttr->value());
        }
        if (xml_attribute<>* heightAttr = svgNode->first_attribute("height")) {
            height = stod(heightAttr->value());
        }

        // Parse the rest of the SVG elements
        parseSVGNode(svgNode, result);
    }
    else {
        cerr << "Error: Missing 'svg' element in the SVG file" << endl;
    }
    return result;
}
