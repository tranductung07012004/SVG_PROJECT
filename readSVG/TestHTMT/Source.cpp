#include "ReadSVG.h"

int main() {
    string filePath = "sample.svg";
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Failed to open the file." << endl;
        return 0;
    }

    string line;
    while (getline(file, line)) {
        if (line.find("<line") != string::npos) {
            Line myLine;
            myLine.readLine(line);
            myLine.printLine();
            cout << endl;
        }
        else if (line.find("<polygon") != string::npos) {
            Polygon myPolygon;
            myPolygon.readLine(line);
            myPolygon.printPolygon();
            cout << endl;
        }
        else if (line.find("<rect") != string::npos) {
            Rect myRect;
            myRect.readLine(line);
            myRect.printRect();
            cout << endl;
        }
        else if (line.find("<circle") != string::npos) {
            Circle myCircle;
            myCircle.readLine(line);
            myCircle.printCircle();
            cout << endl;

        }
        else if (line.find("<ellipse") != string::npos) {
            Ellipse myEllipse;
            myEllipse.readLine(line);
            myEllipse.printEllipse();
        }
        else if (line.find("<polyline") != string::npos) {
            Polyline myPolyline;
            myPolyline.readLine(line);
            myPolyline.printPolyline();
            cout << endl;
        }
        else if (line.find("<text") != string::npos) {
            Text myText;
            myText.readLine(line);
            myText.printText();
            cout << endl;
        }
    }

    return 0;
}