#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void Canny_treshold(int, void*);

Mat gray_img, canny_img, blurred_img;

//Canny edge detection variables
int lower_treshold;
int maxlow_treshold = 150;   //Setting this value to detect the strongest differences with sliding the threshold

int main(int argc, char** argv)
{
    Mat input_img = imread("C:/Users/andre/OneDrive/Desktop/Lab_4/street_scene.png");
    

    if (!input_img.data)
    {
        printf(" No image found in the path! \n");
        return -1;
    } 
    
    cvtColor(input_img, gray_img, COLOR_BGR2GRAY);
    GaussianBlur(gray_img, blurred_img, Size(5, 5), 0);   //Blurring the image with a 5x5 kernel size

    namedWindow("Original Img", WINDOW_KEEPRATIO);
    namedWindow("Gray Img", WINDOW_KEEPRATIO);
    namedWindow("Gaussian Img", WINDOW_KEEPRATIO);
    namedWindow("Canny Img", WINDOW_KEEPRATIO);
    
    createTrackbar("Threshold","Canny Img", &lower_treshold, maxlow_treshold, Canny_treshold);  //Create the trackbar for threshold
    Canny_treshold(0,0);

    imshow("Original Img", input_img);
    imshow("Gray Img", gray_img);
    imshow("Gaussian Img", blurred_img);
    imshow("Canny Img", canny_img);

    waitKey(0);

    return 0;
}

void Canny_treshold(int, void*)
{
    Canny(blurred_img, canny_img, lower_treshold, maxlow_treshold);
    imshow("Canny Img", canny_img);
}