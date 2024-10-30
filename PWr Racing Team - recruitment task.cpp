#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

//  Point struct (mine)
struct Point
{
    double x, y;

    Point(double x, double y)
        : x(x), y(y)
    {
    }
};

//  SplitText function (mine)
std::vector<std::string> SplitText(std::string& text, char delimiter)
{
    std::vector<std::string> splitTextVector;
    std::vector<int> delimitersIndexes;
    for (int i = 0; i < text.size(); i++)
    {
        if (text[i] == delimiter)
            delimitersIndexes.push_back(i);
    }
    splitTextVector.push_back(text.substr(0, delimitersIndexes[0]));
    for (int i = 0; i < delimitersIndexes.size() - 1; i++)
    {
        splitTextVector.push_back(text.substr(delimitersIndexes[i], delimitersIndexes[i + 1] - delimitersIndexes[i]));
    }
    splitTextVector.push_back(text.substr(delimitersIndexes[delimitersIndexes.size() - 1], text.size() - delimitersIndexes[delimitersIndexes.size() - 1]));

    return splitTextVector;
}

// Funkcja pomocnicza do obliczenia iloczynu wektorowego (ChatGPT)
int crossProduct(const Point& O, const Point& A, const Point& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Funkcja porównująca do sortowania względem kąta biegunowego (ChatGPT)
bool polarOrder(const Point& p0, const Point& p1, const Point& p2) {
    int order = crossProduct(p0, p1, p2);
    if (order == 0) // jeśli są współliniowe, wybieramy punkt bliższy
        return (p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y) <
        (p2.x - p0.x) * (p2.x - p0.x) + (p2.y - p0.y) * (p2.y - p0.y);
    return (order > 0);
}

//  Convex hull (Graham algorythm) (ChatGPT)
std::vector<Point> ConvexHull_Graham(std::vector<Point>& points)
{
    // Krok 1: znajdź punkt o najmniejszej współrzędnej y
    Point p0 = *std::min_element(points.begin(), points.end(), [](Point a, Point b) {
        return a.y < b.y || (a.y == b.y && a.x < b.x);
        });

    // Krok 2: Posortuj punkty względem kąta biegunowego w stosunku do p0
    std::sort(points.begin(), points.end(), [&p0](Point a, Point b) {
        return polarOrder(p0, a, b);
        });

    // Krok 3: Budowanie otoczki wypukłej
    std::vector<Point> hull;
    for (const auto& point : points) {
        // Usuń punkty tworzące zakręt w prawo
        while (hull.size() > 1 && crossProduct(hull[hull.size() - 2], hull.back(), point) <= 0) {
            hull.pop_back();
        }
        hull.push_back(point);
    }

    return hull;
}

//  Main function (mine)
int main()
{
    std::cout << "\n - Surrounding points on a plane - \n";

    //      Reading a file entered by the user and converting it to a vector with points
    //  Getting the file name from the user
    std::string fileName;
    std::cout << "\nEnter the points file name:\n";
    std::cin >> fileName;
    //  Opening the file
    std::string pointLine;
    std::ifstream fileRead(fileName);   //  for now will throw up if file doesn't exist, needs checking :(
    getline(fileRead, pointLine);
    int pointsAmount = stoi(pointLine);
    //  Creating a Point vector out of the lines in the file
    std::vector<Point> pointsVector;
    while (getline(fileRead, pointLine))
    {
        std::vector<std::string> pointLineVector = SplitText(pointLine, ' ');
        pointsVector.push_back(Point(stod(pointLineVector[0]), stod(pointLineVector[1])));
    }
    fileRead.close();
    //  LOG: Point vector - works
    std::cout << "\nIntput points:\n" << "[";
    for (int i = 0; i < pointsVector.size(); i++)
    {
        if (i > 0)
            std::cout << ", ";
        std::cout << "(" << pointsVector[i].x << ", " << pointsVector[i].y << ")";
    }
    std::cout << "]\n";

    //      Getting the surrounding points using Graham algorithm for convex hull
    std::vector<Point> surroundingPointsVector = ConvexHull_Graham(pointsVector);
    //  LOG: Surrounding points vector - works!!!
    std::cout << "\nOutput points:\n" << "[";
    for (int i = 0; i < surroundingPointsVector.size(); i++)
    {
        if (i > 0)
            std::cout << ", ";
        std::cout << "(" << surroundingPointsVector[i].x << ", " << surroundingPointsVector[i].y << ")";
    }
    std::cout << "]\n";

    return 0;
}
