#pragma once
#include "stdafx.h"
#include "rapidxml.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <string>
#include <fstream>
#include <regex>
#include "SVGDemo.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <locale>
#include <codecvt>
#include <unordered_map>
#include <cmath>
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")


struct SVGElement {
    string type;
    map<string, string> attributes;
    string textContent;
    vector<SVGElement> children;
    bool parseYes = 0;
};

vector<SVGElement> parseSVG(const string& filename, double& width, double& height, double& minX, double& minY, double& maxX, double& maxY);
void parseStyle(const string& s, SVGElement& element);
void parseSVGNode(xml_node<>* node, vector<SVGElement>& elements);
void parseGroupNode(xml_node<>* node, SVGElement& groupElement);
void parseGradientNode(xml_node<>* node, SVGElement& gradientElement);
void parseDefsNode(xml_node<>* node, SVGElement& defsElement);