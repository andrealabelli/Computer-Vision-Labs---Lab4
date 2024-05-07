#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    // Read input image
    Mat input_img = imread("C:/Users/andre/OneDrive/Desktop/Lab_4/street_scene.png");
    Mat gray_img, binary_img, morph_img;

    if (!input_img.data)
    {
        printf(" No image found in the path! \n");
        return -1;
    } 

    cvtColor(input_img, gray_img, COLOR_BGR2GRAY);
    threshold(gray_img, binary_img, 250, 255, cv::THRESH_BINARY);

    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));  //Getting the rectangle of the dotted and solid lines
    morphologyEx(binary_img, morph_img, MORPH_CLOSE, kernel);  // "MORPH_CLOSE" does a dilation followed by an erosion on the element selected; with this I obtained the best result

    namedWindow("White markings", WINDOW_KEEPRATIO);
    imshow("White markings", morph_img);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(morph_img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); 

    for (int i = 0; i < contours.size(); i++) {
    Scalar color = Scalar(0, 0, 255);
    drawContours(input_img, contours, i, color, 2);
    }

    namedWindow("White markings Detection", WINDOW_KEEPRATIO);
    imshow("White markings Detection", input_img);

    waitKey(0);

    return 0;

}