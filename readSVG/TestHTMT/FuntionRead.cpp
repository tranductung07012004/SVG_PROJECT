#include "FunTionRead.h"

string removeSpacesBetweenQuotes(const string& input) {
    string result;
    bool insideQuotes = false;

    for (char c : input) {
        if (c == '"') {
            insideQuotes = !insideQuotes;
        }
        if (insideQuotes && c == ' ') {
            continue;
        }
        result += c;
    }

    return result;
}

string extractPointsValue(string& input) {
    string pointsValue;

    size_t startPos = input.find("points=\"");

    if (startPos != string::npos) {
        startPos += 8;
        size_t endPos = input.find("\"", startPos);

        if (endPos != string::npos) {
            pointsValue = input.substr(startPos, endPos - startPos);
            input = input.erase(startPos, endPos - startPos);
        }
    }
    return pointsValue;
}