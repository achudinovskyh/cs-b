#ifndef SILCOUNTER_H
#define SILCOUNTER_H
#include <iostream>
#include <QImage>
#include <QColor>
#include <QPoint>
#include <fstream>
#include <map>
#include <queue>
#include <string>

using namespace std;

class SilCounter
{
    // method that read the image and create two dimensional array of colors from this image
    void createImageMatrixOfColour(string* filePath,int** &imageMatrix, int& width, int& height);

    // method that determines the colors of silhouettes and background
    void identifyColors(int** imageMatrix, int width, int height,int& backgroundColour);

    // method that print image to text file for debugging
    void printImageMatrix(int** imageMatrix, int width, int height);

    // method that repaint silhouette in color of background
    void fillSilhouette(QPoint& point,int** imageMatrix,int width, int height ,int backgroundColor);

public:

    // method that performs all the steps to calculate silhouettes
    int countSilhouettes(string*);

    SilCounter();
};
#endif // SILCOUNTER_H
