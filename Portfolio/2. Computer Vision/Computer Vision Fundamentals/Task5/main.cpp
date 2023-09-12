//James Rogers Mar 2022 (c) Plymouth University
#include<iostream>
#include<fstream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

static Scalar red = Scalar(0, 0, 255);
static Scalar green = Scalar(0, 255, 0);
static Scalar blue = Scalar(255, 0, 0);
static Scalar white = Scalar(255, 255, 255);
static Scalar black = Scalar(0, 0, 0);

string title;
int i = 0;
Mat l_filtered;
Mat masked_edges;
int xPos = 1820;
int xNeg = 0;
Point centreLine[2];

typedef struct{
    float interceptC;
    float gradientM;

} strLine;

int selectVid(){
    cout << "Press a number between 0 and 4 to choose a video to display." << endl;
    Mat sel = imread("../Task5/video_selection.png");
    imshow("Select your video", sel);
    int x;
    while(1)
    {
        int k = waitKey(0);

        if (k == '0'){       // wait for ESC key to exit
            x = 0;
            break;
        }
        else if (k == '1'){
            x = 1;
            break;
        }
        else if (k == '2'){
            x = 2;
            break;
        }
        else if (k == '3'){
            x = 3;
            break;
        }
        else{

        }
    }
    destroyAllWindows();
    cout << "Video " << x << endl;
    title = "Video " + to_string(x);
    return x;
}

//a drawing function that can draw a line based on rho and theta values.
//useful for drawing lines from the hough line detector.
void lineRT(Mat &Src, Vec2f L, Scalar color, int thickness){
    Point pt1, pt2;
    double a = cos(static_cast<double>(L[1]));
    double b = sin(static_cast<double>(L[1]));
    double x0 = a*static_cast<double>(L[0]);
    double y0 = b*static_cast<double>(L[0]);
    pt1.x = cvRound(x0 + 10000*(-b));
    pt1.y = cvRound(y0 + 10000*(a));
    pt2.x = cvRound(x0 - 10000*(-b));
    pt2.y = cvRound(y0 - 10000*(a));
    line(Src, pt1, pt2, color, thickness, LINE_AA);
}

//Find point (x,y) where two parameterized lines intersect :p Returns 0 if lines are parallel
Point houghIntersect(Vec2f lineA, Vec2f lineB) {
    float r1 = lineA[0];
    float t1 = lineA[1];
    float r2 = lineB[0];
    float t2 = lineB[1];

    float d=cosf(t1)*sinf(t2)-sinf(t1)*cosf(t2);        //determinative (rearranged matrix for inverse)

    Point p;
    if(d!=0.0f) {
        p.x=(int)((sinf(t2)*r1-sinf(t1)*r2)/d);
        p.y=(int)((-cosf(t2)*r1+cosf(t1)*r2)/d);
        return(p);
    }
    else { //lines are parallel and will NEVER intersect!
        p = Point(0,0);
        return(p);
    }
}

Point findClosePoint(Vec2f line){
    Point pt1, pt2;
    Point closePnt, farPnt;
    double a = cos(static_cast<double>(line[1]));
    double b = sin(static_cast<double>(line[1]));
    double x0 = a*static_cast<double>(line[0]);
    double y0 = b*static_cast<double>(line[0]);
    pt1.x = cvRound(x0 + 10000*(-b));
    pt1.y = cvRound(y0 + 10000*(a));
    pt2.x = cvRound(x0 - 10000*(-b));
    pt2.y = cvRound(y0 - 10000*(a));

    closePnt =  (pt1.y > pt2.y) ? pt1 : pt2;
    return closePnt;
}

Vec2f pointsToRhoTheta(Point p1, Point p2){
    double x1 = p1.x;
    double y1 = p1.y;

    double x2 = p2.x;
    double y2 = p2.y;

    double cvTheta=0;
    double cvRho=0;

    double grad=0;
    if (x2-x1!=0)
    {
        grad=(y2-y1)/(x2-x1);
    }
    else
    {
        grad=99999;
    }
    double xSlope, ySlope, m;

    m=y1-grad*x1;
    xSlope=-m/(grad+(1/grad));
    ySlope=m/(pow(grad,2)+1);

    if (atan2((ySlope),(xSlope))<0)
    {
        cvTheta=atan2((ySlope),(xSlope))+CV_PI;
        cvRho=-sqrt(pow(xSlope,2)+pow(ySlope,2));
    }
    else
    {
        cvTheta=atan2((ySlope),(xSlope));
        cvRho=sqrt(pow(xSlope,2)+pow(ySlope,2));
    }

    Vec2f lineRhoTheta;
    lineRhoTheta[0]=cvRho;
    lineRhoTheta[1]=cvTheta;

    return lineRhoTheta;
}

Mat defineROI(Mat f, Point vert[1][4]){

    Point BL = Point(0, 719);
    Point BR = Point(1219, 719);
    Vec2f base = pointsToRhoTheta(BL, BR); //bottom line

    Mat CannyFrame(f.size(), CV_8U);
    Canny(masked_edges, CannyFrame, 100, 255);

    Mat ROICalc(f.size(), CV_8U);
    vector<Vec2f> all_lines;
    vector<Vec2f> lines;

    HoughLines(CannyFrame, all_lines, 1, M_PI/180.0, 170, 0, 0);

    int num = all_lines.size();

    int minAngle = 60;
    int maxAngle = 120;

    for (int i = 0; i < (int)num; i++) {
        float theta = all_lines[i][1] * 180/M_PI;
        if (!(minAngle < theta && theta < maxAngle) && (10 < theta)) {
            lines.push_back(all_lines[i]);
        }
    }
    num = lines.size();
    if (num > 1)
    {
        int centre = 1280/2;
        int negX = 0;
        int posX = 1280;
        int leftIdx;
        int rightIdx;

        for (int a = 0; a < num; a++){
            Point BaseLineIntersect = houghIntersect(lines[a], base);
            if (negX < BaseLineIntersect.x && BaseLineIntersect.x < centre){
                leftIdx = a;
                negX = BaseLineIntersect.x;
            }
            else if(posX > BaseLineIntersect.x && BaseLineIntersect.x > centre){
                rightIdx = a;
                posX = BaseLineIntersect.x;
            }
        }

        Point baseA = houghIntersect(lines[leftIdx], base);
        Point baseB = houghIntersect(lines[rightIdx], base);

        vert[0][0] = Point(centreLine[1].x-5,  centreLine[1].y);
        vert[0][1] = Point(centreLine[1].x+5,  centreLine[1].y);
        vert[0][2] = Point(baseB.x+25, 719);
        vert[0][3] = Point(baseA.x-25,  719);

        centreLine[0] = Point(posX - ((posX-negX)/2), 719);
        centreLine[1] = houghIntersect(lines[leftIdx], lines[rightIdx]);
        centreLine[1].y += 15;

        const Point* ppt[1] = { vert[0] };
        int npt[] {4};

        fillPoly(ROICalc, ppt, npt, 1, Scalar(255,255,255), LINE_8);
    }
    return ROICalc;
}
Mat thresh(Mat input, int valueA, int valueB){
    Mat output;
    threshold(input, output, valueA, valueB, THRESH_BINARY);
    return output;
}

int main()
{
    while(i < 4)
    {
        int v = selectVid();

        //Open video file
        VideoCapture CarVideo("../Task5/DashCam" + to_string(v) + ".mp4");
        if(!CarVideo.isOpened()){
            cout<<"Error opening video"<<endl;
            return -1;
        }

        //main program loop
        while(true)
        {
            // 1. Open the next frame from the video file
            Mat Frame_BGR;
            CarVideo.read(Frame_BGR);
            if(Frame_BGR.empty()){
                break;
            }

            // 2. Convert video from BGR into HSL (hue, sat, lightness)
            Mat Frame_HSL;
            cvtColor(Frame_BGR, Frame_HSL, COLOR_BGR2HLS);

            // 3. Perform edge detection on the lightness channel
            vector<Mat> hsv_channels(3);
            split(Frame_BGR, hsv_channels);
            Mat l_channel = hsv_channels[2];

            Mat l_thresh;
            Mat l_gauss;
            threshold(l_channel, l_thresh, 170, 255, THRESH_BINARY);
            GaussianBlur(l_thresh, l_gauss, Size(3,3), 0);

            // SOBEL EDGE DETECTION
            Sobel(l_gauss, l_filtered, CV_8U, 1, 0, 3);

            // 4. Perform binary thresholding on the saturation channel of the video frame
            Mat sat_channel = hsv_channels[1];
            Mat sat_thresh = thresh(sat_channel, 180, 245);
            imshow("sat", sat_thresh);

            // 5. Perform binary thresholding on the R (red) channel of the original BGR video frame.
            vector<Mat> bgr_channels(3);
            split(Frame_BGR, bgr_channels);
            Mat red_channel = bgr_channels[2];
            Mat red_thresh = thresh(red_channel, 190, 255);
            imshow("red", red_thresh);

            // 7. Perform bitwise calcs to select only edges which are within the chosen area
            bitwise_and(sat_thresh, red_thresh, masked_edges); // combine saturation and red levels thresholds
            GaussianBlur(masked_edges, masked_edges, Size(5, 5), 0); //blur the saturation mask

            // 6. Find the area infront of the car.
            Point vert[1][4];
            Mat ROI(Frame_BGR.size(), CV_8U);
            ROI = defineROI(Frame_BGR, vert);

            Mat final_edges(Frame_BGR.size(), CV_8U);
            bitwise_and(masked_edges, ROI, final_edges); // combine with the ROI mask
            line(Frame_BGR, centreLine[0], centreLine[1], green, 2);

            // 9. Detect the contours on the binary mask and draw on original image
            vector<vector<Point>> contours;
            vector<vector<Point>> good_contours;
            findContours(final_edges, contours, RETR_TREE, CHAIN_APPROX_NONE);
            int maxArea = 1000;
            for (int i = 0; i < (int)contours.size(); i++)
            {
                double area = contourArea(contours[i]);

                if (area > maxArea)
                {
                    good_contours.push_back(contours.at(i));
                }
            }
            drawContours(Frame_BGR, good_contours, -1, Scalar(40, 40, 40), 1);
            vector<Point> pts = {vert[0][0], vert[0][1], vert[0][2], vert[0][3]};

            for (int r = 0; r < Frame_BGR.rows; ++r)
            {
                for (int c = 0; c < Frame_BGR.cols; ++c)
                {
                    Vec3b &framePix = Frame_BGR.at<Vec3b>(r,c);
                    // Blending
                    int overlayPix = ROI.at<uchar>(r,c);
                    if (overlayPix > 250){
                        framePix[1] = 240;
                    }
                }
            }


            imshow(title, Frame_BGR);
            while(waitKey(100)!='x'){
            }
        }
        i++;
    }
}

