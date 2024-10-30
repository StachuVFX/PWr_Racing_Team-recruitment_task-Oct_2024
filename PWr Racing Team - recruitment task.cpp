#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//  Point struct
struct Point
{
    double x;
    double y;

    Point(double x, double y)
        : x(x), y(y)
    {
    }
};

//  SplitText function
std::vector<std::string> SplitText(std::string text, char delimiter)
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

int main()
{
    std::cout << "Surrounding points on a plane\n";

    //      Reading a file entered by the user and converting it to a vector with points
    //  Getting the file name from the user
    std::string fileName;
    std::cout << "Enter the points file name:\n";
    std::cin >> fileName;
    //  Opening the file
    std::string pointLine;
    std::ifstream fileRead(fileName);
    getline(fileRead, pointLine);
    int pointsAmount = stoi(pointLine);
    //  Creating a Point vector out of the lines in the file
    std::vector<Point> pointsVector;
    while (getline(fileRead, pointLine))
    {
        std::vector<std::string> pointLineVector = SplitText(pointLine, ' ');
        pointsVector.push_back(Point(stod(pointLineVector[0]), stod(pointLineVector[1])));
    }
    //  LOG: Point vector - works
    for (int i = 0; i < pointsVector.size(); i++)
    {
        std::cout << pointsVector[i].x << " " << pointsVector[i].y << std::endl;
    }

    fileRead.close();
    return 0;
}
