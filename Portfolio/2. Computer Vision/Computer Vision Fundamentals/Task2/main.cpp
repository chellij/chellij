/*
 * AINT 308 - C1
 * Rachel Ireland-Jones
 * March 2023
 *
 * Task 2 - Colour Tracker
*/

/* ---- Include ---- */
#include <iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/opencv_modules.hpp>

/* ---- Namespaces and Classes ---- */
using namespace std;
using namespace cv;

/* ---- Variables ---- */
VideoCapture InputStream;
Point Pivot;
Vec3b UpperBound;
Vec3b LowerBound;
Vec3b PivotLBound;
Vec3b PivotUBound;
ofstream DataFile;

//string video = "Original";
string video = "ExtendedTesting";

/* ---- Functions ----*/
float calculateAngle(Point pointA, Point pointB){
    // calculate the difference in x and y from pointA to pointB
    float x_diff = (pointA.x) - (pointB.x);
    float y_diff = (pointA.y) - (pointB.y);

    // calculate the angle between the points
    float theta = atan2(x_diff, y_diff) * 180.0f/M_PI;
    return theta;
}

/* ---- main ---- */
int main()
{
    if (video == "Original") {
        //Load in the video as an input stream
        InputStream = VideoCapture("C:/AINT308Lib/Data/Task2 Video.mp4");

        // Set pivot position     changed from (592,52)
        Pivot = Point(590,40);

        // Upper and lower colour boundries for the target
        LowerBound = Vec3b(65, 100, 100);
        UpperBound = Vec3b(85, 255, 255);

        //Open output file for angle data
        DataFile = ofstream("../Task2/Data_Original.csv");
    }
    else if(video == "ExtendedTesting"){
        // Load in the video as an input stream
        InputStream = VideoCapture("../Task2/AINT308_C1_Task2_Extended.mp4");

        // Set upper and lower colour boundries for the pivot point
        PivotLBound = Vec3b(160, 70, 150);
        PivotUBound = Vec3b(180, 100, 255);

        // Set upper and lower colour boundries for the target
        LowerBound = Vec3b(0, 100, 60);
        UpperBound = Vec3b(10, 150, 255);

        //Open output file for angle data
        DataFile = ofstream("../Task2/Data_Extended.csv");
    }


    // loop through video frames
    while(true){

        // load next frame
        Mat Frame;
        InputStream.read(Frame);

        // if frame is empty then the video has ended, so break the loop
        if(Frame.empty()){
            break;
        }

        // Video is very high resolution, reduce it to 720p to run faster
        resize(Frame,Frame,Size(1280,720));

        // convert the frame to HSV colour system (not using _FULL flag so 180 deg)
        Mat FrameHSV;
        cvtColor(Frame, FrameHSV, COLOR_BGR2HSV);
        //imshow("HSV", FrameHSV);

        // filter the HSV frame to only show the pendulum target colour (produces a boolean matrix)
        Mat FrameFiltered;
        inRange(FrameHSV, LowerBound, UpperBound, FrameFiltered);
        //imshpw("Filtered", FrameFiltered);

        // calculate the moment (average) of this filtered frame
        Moments m = moments(FrameFiltered, true);

        // calulate the centrePoint of the moment
        Point centrePoint(m.m10/m.m00, m.m01/m.m00);

        // draw a circle arond the centrePoint
        circle(Frame, centrePoint, 15, Scalar(255, 0, 0), 2);

        if (video == "ExtendedTesting"){

            // filter the HSV frame to show only the pivot target colour
            Mat PivotFiltered;
            inRange(FrameHSV, PivotLBound, PivotUBound, PivotFiltered);

            Mat element = getStructuringElement(MORPH_RECT, Size(6,6));
            morphologyEx(PivotFiltered, PivotFiltered, MORPH_CLOSE, element, Point(-1, -1), 2);

            // calculate the moment (avergage) of this frame
            Moments pivot_m = moments(PivotFiltered, true);

            // caclulate the pivot point from the moment
            Pivot = Point(pivot_m.m10/pivot_m.m00, pivot_m.m01/pivot_m.m00);

            // draw the line from the pivot to the centrePoint
            line(Frame, Pivot, centrePoint, Scalar(0, 0, 255), 3);

            // draw a circle indicating the pivot point
            circle(Frame, Pivot, 15, Scalar(255, 0, 0), 2);
            //imshow("Filtered", PivotFiltered);
        }

        // draw a line from the centrePoint to the Pivot point
        line(Frame, centrePoint, Pivot, Scalar(0, 0, 255), 3);

        // calculate the angle between pivot and pendulum
        float angle = calculateAngle(centrePoint, Pivot);
        cout << angle << endl;

        // send the angle to the output file
        DataFile << angle << endl;

        // Display the frame
        imshow("Video", Frame);
        waitKey(10);
    }
    DataFile.close(); //close output file
}
