#include "silcounter.h"

/*
 *  Method that read the image and create two dimensional array of colors from this image
 *
 *  @param  filePath Path to image file
 *  @param  imageMatrix 2d array that contains color's
 *  @param  width The width of the image
 *  @param  height The height of the image
 */
void SilCounter::createImageMatrixOfColour(string& filePath, int** &imageMatrix, int &width, int &height)
{
    // creating QImage from image that was set by user
    QImage* image = new QImage(&filePath[0]);

    // geting width and height from image
    width = (*image).width();
    height = (*image).height();


    // creating 2d array that will store color of image pixels
    imageMatrix = new int*[height];
    for(int i = 0; i < height; i++){
        imageMatrix[i] = new int [width];
    }

    // cheking if this image is a PNG file
    bool png = false;
    string pngCheck = filePath.substr(filePath.length() - 3,filePath.length());
    if(pngCheck == "png" || pngCheck == "PNG"){
        png = true;
    }

    // geting alpha component if it is a PNG, or lightness, from each pixel of the image
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(png){
                QRgb tempColor = (*image).pixel(j,i);
                int alpha = qAlpha(tempColor);
                imageMatrix[i][j] = alpha;
            }else{
                QColor tempColor  = (*image).pixel(j,i);
                int alpha = tempColor.lightness();
                imageMatrix[i][j] = alpha;
            }
        }
    }

    delete image;
}

/*
 *  Method that determines the colors of silhouettes and background
 *
 *  @param  imageMatrix 2d array that contains color's
 *  @param  width The width of the image
 *  @param  height The height of the image
 *  @param  backgroundColor Variable that indicate background color
 */
void SilCounter::identifyColors(int** imageMatrix, int width, int height,int& backgroundColor)
{
    // map with key color and value amount of pixels with this color
    MyMap<int,int> colorFreq;

    // walk through 2d array and fill the map
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            colorFreq[imageMatrix[i][j]]++;
        }
    }

    // variables that will contain two most frequent colors
    int mostFreqFirst[2] = {0, 0};
    int mostFreqSecond[2] = {0, 0};

    // searching for two most frequent colors
    for(MyMap<int,int>::iterator it = colorFreq.begin(); it != colorFreq.end(); it++){
        if(it.node->value > mostFreqFirst[1]){
            mostFreqFirst[0] = it.node->key;
            mostFreqFirst[1] = it.node->value;
        }
    }
    for(MyMap<int,int>::iterator it = colorFreq.begin(); it != colorFreq.end(); it++){
        if((it.node->key > mostFreqSecond[1]) && (it.node->key != mostFreqFirst[0] )){
            mostFreqSecond[0] = it.node->key;
            mostFreqSecond[1] = it.node->value;
        }
    }

    // find average between most frequent colors
    int averageValue = (mostFreqFirst[0] + mostFreqSecond[0]) / 2;

    // set all colors that is lower then average to qrequent color with lower index to 0 and vice versa
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            imageMatrix[i][j] < averageValue ? imageMatrix[i][j] = 0 : imageMatrix[i][j] = 1;
        }
    }

    // check the perimeter of the image to identify pixels with what value are more often
    // it will be a background color
    int zeroCounter = 0;
    int oneCounter = 0;

    // checking upper and lower bounds
    for(int i = 0; i < width; i++){
        imageMatrix[0][i] ? oneCounter++ : zeroCounter++;

        imageMatrix[height-1][i] ? oneCounter++ : zeroCounter++;
    }

    // checking sides
    for(int i = 0; i < height; i++){
        imageMatrix[i][0] ? oneCounter++ : zeroCounter++;

        imageMatrix[i][width-1] ? oneCounter++ : zeroCounter++;
    }
    // set background color
    if(oneCounter > zeroCounter ){
        backgroundColor = 1;
    }
}

/*
 *  Method that print image to text file for debugging
 *
 *  @param  imageMatrix 2d array that contains color's
 *  @param  width The width of the image
 *  @param  height The height of the image
 */
void SilCounter::printImageMatrix(int** imageMatrix, int width, int height)
{
    string filePath = "";
    cout << "Enter file path to print the image matrix there.\n";
    cin >> filePath;

    ofstream of(&filePath[0]);

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++) {
            if(imageMatrix[i][j] < 10){
                of << "  " << imageMatrix[i][j] << " ";
            }else if(imageMatrix[i][j] < 100){
                of << " " << imageMatrix[i][j] << " ";
            }else{
                of << imageMatrix[i][j] << " ";
            }
        }
        of << endl;
    }
    of.close();
}

/*
 *  Repaint silhouette in color of background
 *
 *  @param  point Extreme point of silhouette
 *  @param  imageMatrix 2d array that contains color's
 *  @param  width The width of the image
 *  @param  height The height of the image
 *  @param  backgroundColor Variable that indicate background color
 */
void SilCounter::fillSilhouette(QPoint &point, int **imageMatrix, int width, int height, int backgroundColor)
{
    // queue that store all point neighbours, with silhouette color
    MyQueue<QPoint> queueOfPoints;

    // repaint point in background color to prevent it from re-adding to the queue from cycle
    imageMatrix[point.y()][point.x()] = backgroundColor;
    queueOfPoints.push(point);\

    // walk throw all point neigbours with silhouette color
    while(queueOfPoints.size()){

        // temporary variable for poin from queue
        QPoint temp = queueOfPoints.front();
        int x = temp.x();
        int y = temp.y();
        queueOfPoints.pop();\

        // walk throw all point neigbours with silhouette color repaint them and adding to the queue
        for(int i = -1; i <= 1; i++){
            for(int j = -1; j <= 1; j++){
                if( (0 <= x + i) && (x + i < width) && (0 <= y + j) && (y + j < height) && (imageMatrix[y + j][x + i] != backgroundColor)){
                    imageMatrix[y + j][x + i] = backgroundColor;
                    queueOfPoints.push(QPoint(x + i,y + j));
                }
            }
        }
    }
}

/*
 *  Counts the amount of silhouettes in the image
 *
 *  @param  path to image file
 *  @return  result the amount of silhouettes
 */
int SilCounter::countSilhouettes(string& filePath)
{
    // pointer for 2d array that will store color's of image
    int** imageMatrix;

    // variables for wigth and height
    int width = 0;
    int height = 0;

    // method that read the image and create two dimensional array of colors from this image
    createImageMatrixOfColour(filePath,imageMatrix,width,height);

    int backgroundColor = 0;

    // method that determines the colors of silhouettes and background
    identifyColors(imageMatrix,width,height,backgroundColor);

    int silhouettesCount = 0;
    // walk through 2d array and when find silhouette repaint all it's pixel
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(imageMatrix[i][j] != backgroundColor){
                QPoint point;
                point.setX(j);
                point.setY(i);

                // method that repaint silhouette in color of background
                fillSilhouette(point,imageMatrix,width,height,backgroundColor);

                // when repaint is done, count it
                silhouettesCount++;
            }
        }
    }

    // deleting 2d array
    for(int i = 0; i < height; i++){
        delete[] imageMatrix[i];
    }
    delete[] imageMatrix;

    return silhouettesCount;
}

SilCounter::SilCounter()
{

}

