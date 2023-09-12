/*
 * AINT 308 - C1
 * Rachel Ireland-Jones
 * March 2023
 *
 * Task 3 - Cross Correlation
*/

/* ---- Include ---- */
#include <iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/opencv_modules.hpp>

/* ---- Namespaces and Classes ---- */
using namespace std;
using namespace cv;

/* ---- Variables ---- */
string folderPath;
Mat PCB;
Mat Component;

Mat result;
Point minLoc, maxLoc;
double minVal,maxVal;

int totalComponents = 10;
int notPlaced = 0;

/* ---- Functions ---- */

void drawAroundComponent(){
    int x = Component.cols;
    int y = Component.rows;

    Point corner1(minLoc.x, minLoc.y);
    Point corner2(minLoc.x + x, minLoc.y + y);

    rectangle(PCB, corner1, corner2, Scalar(0, 0, 255), 3);
}

/* ---- main ---- */
int main()
{
    for(int m = 0; m < 4; m++)
       {
        // Find path to the folder with PCB and Components
           string Path = "../Task3/PCB Images/PCB" + to_string(m) + "/";
           int notFound = 0;

           //loop through component images
           for(int n=0; n<10; ++n)
           {
               //read PCB and component images
               PCB = imread(Path+"PCB.png");
               Component = imread(Path+"Component"+to_string(n)+".png");

               // Find the error of the component image across the PCB image
               // Then find minimum error point on the image
               matchTemplate(PCB, Component, result, TM_SQDIFF_NORMED);
               minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

               float err = minVal * 100;

               // If the minimum error across the image is over 1 - the component hasn't been found.
               if (err > 1){
                   cout << "Component " << n << " not found on PCB " << m << "\tError = " << err << endl;
                   rectangle(Component, Point(0,0), Point(Component.cols, Component.rows), Scalar(0, 0, 255), 3);
                   notFound++;
               }
               // Otherwise draw a box on the PCB image showing the component
               else{
                   cout << "Component "<< n << " found on PCB" << m << endl;
                   drawAroundComponent();
               }

               //display the results untill x is pressed
               while(waitKey(10)!='x')
               {
                   imshow("Target", Component);
                   imshow("PCB", PCB);
               }

           }
           cout << endl << "PCB " << m << ": " << notFound << " Component(s) not found." << endl << endl;
       }}
