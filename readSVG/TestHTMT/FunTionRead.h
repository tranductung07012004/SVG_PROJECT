#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

struct Point {
	int x, y;
};

string removeSpacesBetweenQuotes(const string& input);
string extractPointsValue(string& input);