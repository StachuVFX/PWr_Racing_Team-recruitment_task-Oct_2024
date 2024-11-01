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

//  Line struct (mine)
struct Line
{
    double a, b, x1, x2, y1, y2;
    bool isVertical;
    
    //  Constructor: creates a line in slope-intercept form out of two points
    Line(Point p1, Point p2)
        : x1(p1.x), x2(p2.x), y1(p1.y), y2(p2.y)
    {
        if (isVertical = (x2 - x1) == 0)
        {
            a = x1;
            b = 0;
        }
        else
        {
            a = (y2 - y1) / (x2 - x1);
            b = y1 - a * x1;
        }
    }

    //  Checks if there is a side without points (returns: up - 1, down - 2, none - 0)
    int checkSides(std::vector<Point>& points)
    {
        int upSide = 0, downSide = 0;
        for (int i = 0; i < points.size(); i++)
        {
            if (isVertical)
            {
                if (points[i].x > a)
                    upSide++;
                else if (points[i].x < a)
                    downSide++;
            }
            else
            {
                if (points[i].y > a * points[i].x + b)
                    upSide++;
                else if (points[i].y < a * points[i].x + b)
                    downSide++;
            }
        }
        if (!upSide)
            return 1;
        else if (!downSide)
            return 2;
        
        return 0;
    }
};

//  SplitText function (mine)
std::vector<std::string> SplitText(std::string& text, char delimiter)
{
    std::vector<std::string> splitTextVector;
    //  I already forgot what I did here lmao
    //  I'm checking for all appearances of the delimiter
    std::vector<int> delimitersIndexes;
    for (int i = 0; i < text.size(); i++)
    {
        if (text[i] == delimiter)
            delimitersIndexes.push_back(i);
    }
    //  then I'm making substrings according to the delimiter appearances
    splitTextVector.push_back(text.substr(0, delimitersIndexes[0]));
    for (int i = 0; i < delimitersIndexes.size() - 1; i++)
    {
        splitTextVector.push_back(text.substr(delimitersIndexes[i], delimitersIndexes[i + 1] - delimitersIndexes[i]));
    }
    splitTextVector.push_back(text.substr(delimitersIndexes[delimitersIndexes.size() - 1], text.size() - delimitersIndexes[delimitersIndexes.size() - 1]));
    //  Actually I didn't have to do that for more than 1 appearance of the delimiter, but I'm ambitious lol

    return splitTextVector;
}

// Helper function to calculate the cross product (ChatGPT)
int crossProduct(const Point& O, const Point& A, const Point& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Comparison function for sorting based on polar angle (ChatGPT)
bool polarOrder(const Point& p0, const Point& p1, const Point& p2) {
    int order = crossProduct(p0, p1, p2);
    if (order == 0) // if points are collinear, choose the closer point
        return (p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y) <
        (p2.x - p0.x) * (p2.x - p0.x) + (p2.y - p0.y) * (p2.y - p0.y);
    return (order > 0);
}

//  Convex hull function (Graham algorithm) (ChatGPT)
std::vector<Point> ConvexHull_Graham(std::vector<Point>& points)
{
    // Step 1: find the point with the smallest y-coordinate
    Point p0 = *std::min_element(points.begin(), points.end(), [](Point a, Point b) {
        return a.y < b.y || (a.y == b.y && a.x < b.x);
        });

    // Step 2: Sort points by polar angle relative to p0
    std::sort(points.begin(), points.end(), [&p0](Point a, Point b) {
        return polarOrder(p0, a, b);
        });

    // Step 3: Build the convex hull
    std::vector<Point> hull;
    for (const auto& point : points) {
        // Remove points that make a right turn
        while (hull.size() > 1 && crossProduct(hull[hull.size() - 2], hull.back(), point) <= 0) {
            hull.pop_back();
        }
        hull.push_back(point);
    }

    return hull;
}

//  My surrounding points (convex hull) function
std::vector<Point> ConvexHull_Stachu(std::vector<Point>& points)
{
    std::vector<int> surroundingPointsIndices;
    for (int i = 0; i < points.size(); i++)
    {
        surroundingPointsIndices.push_back(0);
    }

    //  Finding lines that don't have any points on one of the sides
    for (int i = 0; i < points.size() - 1; i++)
    {
        for (int j = i + 1; j < points.size(); j++)
        {
            Line line(points[i], points[j]);
            if (line.checkSides(points))
            {
                surroundingPointsIndices[i]++;
                surroundingPointsIndices[j]++;
            }
        }
    }

    std::vector<Point> surroundingPoints;
    for (int i = 0; i < surroundingPointsIndices.size(); i++)
    {
        if (surroundingPointsIndices[i])
            surroundingPoints.push_back(points[i]);
    }

    return surroundingPoints;
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
    std::ifstream fileRead(fileName);   //  for now it will throw up if file doesn't exist, needs checking :(
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
    //  LOG: Surrounding points vector (Graham) - works!!!
    std::cout << "\nOutput points (Graham):\n" << "[";
    for (int i = 0; i < surroundingPointsVector.size(); i++)
    {
        if (i > 0)
            std::cout << ", ";
        std::cout << "(" << surroundingPointsVector[i].x << ", " << surroundingPointsVector[i].y << ")";
    }
    std::cout << "]\n";

    //      Getting the surrounding points using my own algorithm for convex hull
    std::vector<Point> surroundingPointsVector_Stachu = ConvexHull_Stachu(pointsVector);
    //  LOG: Surrounding points vector (Stachu) - works, but needs testing
    std::cout << "\nOutput points (Stachu):\n" << "[";
    for (int i = 0; i < surroundingPointsVector_Stachu.size(); i++)
    {
        if (i > 0)
            std::cout << ", ";
        std::cout << "(" << surroundingPointsVector_Stachu[i].x << ", " << surroundingPointsVector_Stachu[i].y << ")";
    }
    std::cout << "]\n";

    return 0;
}
