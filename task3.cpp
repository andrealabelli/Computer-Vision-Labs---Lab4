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
    threshold(gray_img, gray_img, 200, 230, cv::THRESH_BINARY);  //Using a threshold with range between 200 and 230 in order to remove other lines due to the vegetation and the right guard-rail
    GaussianBlur(gray_img, blurred_img, Size(5, 5), 0);   //Blurring the image with a 5x5 kernel size
    
    //Edge detection
    Canny(blurred_img, canny_img, 200, 275, 3);
    namedWindow("Canny", WINDOW_KEEPRATIO);
    imshow("Canny", canny_img);

    //Standard Hough Line Transform
    vector<Vec4i> lines; //Hold the results of the detection
    HoughLinesP(canny_img, lines, 1, CV_PI/180, 50); //Runs the actual detection
   
    // Find the two strongest lines
    Vec4i line0(0, 0, 0, 0); 
    Vec4i line1(0, 0, 0, 0); 
    Vec4i line2(0, 0, 0, 0); 
    double max0 = 0, max1 = 0, max2 = 0;

    for (int i = 0; i < lines.size(); i++){
       
        Vec4i l = lines[i];
        double len = sqrt(pow(l[2] - l[0], 2) + pow(l[3] - l[1], 2)); 
        if (len > max0){
            line2 = line1;
            max2 = max1;
            line1 = line0;
            max1 = max0;
            line0 = l;
            max0 = len;
        }else if (len > max1){
            line2 = line1;
            max2 = max1;
            line1 = l;
            max1 = len;
        }else if (len > max2){
            line2 = l;
            max2 = len;
        }
    }

    Point pt1 = Point(line1[0], line1[1]);
    Point pt2 = Point(line1[2], line1[3]);
    Point pt3 = Point(line2[0], line2[1]);
    Point pt4 = Point(line2[2], line2[3]);

    // Calculate the intersection point
    double slope1 = (double)(pt2.y - pt1.y) / (double)(pt2.x - pt1.x);
    double y1 = pt1.y - slope1 * pt1.x;
    double slope2 = (double)(pt4.y - pt3.y) / (double)(pt4.x - pt3.x);
    double y2 = pt3.y - slope2 * pt3.x;

    double x_intersect = (double)(y2 - y1) / (double)(slope1 - slope2);
    double y_intersect = (double)(slope1 * x_intersect + y1);
    Point point(x_intersect, y_intersect);

    // Calculate the intersection with the 'origin'
    double x1 = (filled_img.rows - y1) / slope1;
    double x2 = (filled_img.rows - y2) / slope2;

    // Fill the area between the lines in red
    vector<Point> roi = {Point(x1, filled_img.rows), point, point, Point(x2, filled_img.rows)};
    fillPoly(filled_img, roi, Scalar(0, 0, 255));

    //Show result
    namedWindow("Filled Area", WINDOW_KEEPRATIO);
    imshow("Filled Area", filled_img);

    waitKey(0);

    return 0;
}