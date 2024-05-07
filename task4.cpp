#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    Mat input_img = imread("C:/Users/andre/OneDrive/Desktop/Lab_4/street_scene.png");
    Mat gray_img, canny_img, blurred_img;
    Mat filled_img = input_img.clone();

    if (!input_img.data)
    {
        printf(" No image found in the path! \n");
        return -1;
    }

    cvtColor(input_img, gray_img, COLOR_BGR2GRAY);
    GaussianBlur(gray_img, blurred_img, Size(5, 5), 0);   //Blurring the image with a 5x5 kernel size
    
    vector<Vec3f> circles;
    HoughCircles(blurred_img, circles, HOUGH_GRADIENT, 1, blurred_img.rows/16, 100, 30, 1, 30);

    for(int i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        
        //Circle outline
        int radius = c[2];
        circle(filled_img, center, radius, Scalar(0, 255, 0), FILLED, LINE_AA);  //Using "FILLED" I can completely detect the circle not only on the outline
    }

    //Show results
    namedWindow("Circle detection", WINDOW_KEEPRATIO);
    imshow("Circle detection", filled_img);

    waitKey(0);
    return 0;
}