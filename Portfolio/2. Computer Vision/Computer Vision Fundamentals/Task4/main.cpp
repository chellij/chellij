//James Rogers Nov 2020 (c) Plymouth University

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

ofstream DataFile;
Mat calibMat[4];
float avgBF = 0;
Ptr<StereoSGBM> sgbm;
Scalar white = Scalar(255, 255, 255);
Size imSize = {640, 480};
int numberOfDisparities = 256;
int numberOfKnownImages = 13;
int SADWindowSize = 7;
Rect ROI = Rect(Point(370, 235), Point(390, 255));
const int KnownImages[] = {30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150};

float calcAvg(int cnt, float arr[]){
    //==================================== CALULATE AVERAGE OF A FLOAT ARRAY ===============================
    // This funtion inputs a float array and outputs a float value.

    float total = 0;
    for(int i = 0; i < cnt; i++) {
        total = total+arr[i];
    }
    float average = total/cnt;

    return average;
}
int calibration(string intr, string extr, Mat m[]){
    //================================================Load Calibration Files===============================================
    //This code loads in the intrinsics.xml and extrinsics.xml calibration files, and creates: map11, map12, map21, map22.
    //These four matrices are used to distort the camera images to apply the lense correction.
    Rect roi1, roi2;
    Mat Q;

    FileStorage fs(intr, FileStorage::READ);
    if(!fs.isOpened()){
        printf("Failed to open file %s\n", intr.c_str());
        return -1;
    }

    Mat M1, D1, M2, D2;
    fs["M1"] >> M1;
    fs["D1"] >> D1;
    fs["M2"] >> M2;
    fs["D2"] >> D2;

    fs.open(extr, FileStorage::READ);
    if(!fs.isOpened())
    {
        printf("Failed to open file %s\n", extr.c_str());
        return -1;
    }
    Mat R, T, R1, P1, R2, P2;
    fs["R"] >> R;
    fs["T"] >> T;

    stereoRectify( M1, D1, M2, D2, imSize, R, T, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, -1, imSize, &roi1, &roi2 );

    Mat map11, map12, map21, map22;
    initUndistortRectifyMap(M1, D1, R1, P1, imSize, CV_16SC2, map11, map12);
    initUndistortRectifyMap(M2, D2, R2, P2, imSize, CV_16SC2, map21, map22);

    m[0] = map11;
    m[1] = map12;
    m[2] = map21;
    m[3] = map22;

    return 0;
}
Ptr<StereoSGBM> stereoSGBM(){
    //===============================================Stereo SGBM Settings==================================================
    //This sets up the block matcher, which is used to create the disparity map. The various settings can be changed to
    //obtain different results. Note that some settings will crash the program.
    //SADWindowSize and numberOfDisparities are global variables

    Ptr<StereoSGBM> sgbm = StereoSGBM::create(0,16,3);
    sgbm->setBlockSize(SADWindowSize);
    sgbm->setPreFilterCap(63);
    sgbm->setP1(8*3*SADWindowSize*SADWindowSize);
    sgbm->setP2(32*3*SADWindowSize*SADWindowSize);
    sgbm->setMinDisparity(0);
    sgbm->setNumDisparities(numberOfDisparities);
    sgbm->setUniquenessRatio(10);
    sgbm->setSpeckleWindowSize(50); //100
    sgbm->setSpeckleRange(32);
    sgbm->setDisp12MaxDiff(1);
    sgbm->setMode(StereoSGBM::MODE_SGBM);
    return sgbm;
}
void setup(){
    //==================================== SETUP AND CALIBRATE ================
    // This funtion inputs the array of 4 matrices (map), the defined stereo matching algorithm (stereoMatch)
    // and the distance from the camera for this known image.

    // Define paths to XML files (made in StereoCalibration project)
    string intrinsic_filename = "../StereoCalibration/intrinsics.xml";
    string extrinsic_filename = "../StereoCalibration/extrinsics.xml";

    // Run calibration
    calibration(intrinsic_filename, extrinsic_filename, calibMat);

    // Initialise SGBM object
    sgbm = stereoSGBM();
}
Mat createDispMat(string pathL, string pathR, Mat map[], Ptr<StereoSGBM> sgbm){
    //==================================== CREATE 16 BIT DISPARITY MAP ================
    // The function calculates disparity and uses this in combination with the known distance to calculate the
    // constant BF. In the equation, B is the distance between two cameras and f is the focal length of camera
    // This value is calculated in multiple points on the target so that an average can be taken.

    Mat Left =imread(pathL);
    Mat Right=imread(pathR);

    if (Left.empty()){
        cout << pathL << " not found." << endl;
    }
    if (Right.empty()){
        cout << pathR << " not found." << endl;
    }

    // Distort image to correct for lens/positional distortion
    remap(Left, Left, map[0], map[1], INTER_LINEAR);
    remap(Right, Right, map[2], map[3], INTER_LINEAR);

    // Match left and right images to create disparity image
    Mat disp16bit;
    sgbm->compute(Left, Right, disp16bit);
    return disp16bit;
}
void calcBF(){
    // ======= CALCULATE BF VALUE ==================================================
    cout << "Stepping through the known images and calculating disparity:" << endl;
    DataFile << "Distance, BF Value, Disparity" << endl;

    float bfArray[numberOfKnownImages];
    for(int imageIdx = 0; imageIdx < numberOfKnownImages; imageIdx++)
    {
        int cm = KnownImages[imageIdx];

        string pathL = "../Task4/Distance Targets/left" +to_string(cm)+"cm.jpg";
        string pathR = "../Task4/Distance Targets/right"+to_string(cm)+"cm.jpg";

        Mat disp16bit = createDispMat(pathL, pathR, calibMat, sgbm);            // create 16-bit disparity map for calculations

        Mat disp8bit;
        disp16bit.convertTo(disp8bit, CV_8U, 255/(numberOfDisparities*16.));    // convert to 8-bit for display only

        float disparity = mean(disp16bit(ROI))[0];                              // calculate the average value in ROI
        rectangle(disp8bit, ROI, white, 2);                                     // draw the ROI on 8-bit to check location is suitable

        bfArray[imageIdx] = disparity * cm;                                     // calculate bf value and save to array
        DataFile << cm << ", " << bfArray[imageIdx] << ", " << disparity << endl ;

        imshow("Disparity Map", disp8bit);
        waitKey(500);
    }
    avgBF = calcAvg(13, bfArray);                                           // find average BF value
    cout << "BF = " << avgBF << endl << endl;
    DataFile << "," << avgBF << endl;
}
void ErrorCalc(){
    // ======= TEST KNOWN IMAGES WITH DISTANCE MAP =================================
    DataFile << "Known Distances:" << endl;
    DataFile << "Actual Distance, Avg Disparity, Estimated Distance, Percentage Error(Abs)" << endl;
    cout << "Stepping through the known images and calculating distance for error calculations:" << endl;
    float maxPercentageError = 0;
    float maxAbsoluteError = 0;
    for(int actualDistance = 30; actualDistance <= 150; actualDistance=actualDistance+10){
        string pathL = "../Task4/Distance Targets/left" +to_string(actualDistance)+"cm.jpg";
        string pathR = "../Task4/Distance Targets/right"+to_string(actualDistance)+"cm.jpg";

        Mat disp16bit = createDispMat(pathL, pathR, calibMat, sgbm);            // create 16-bit disparity map for calculations

        // Create distance image
        Mat distanceMap(imSize, CV_8U, 255);
        for (int i = 0; i < (int)imSize.width; i++)
        {
            for(int j = 0; j < (int)imSize.height; j++)
            {
                int disparity = (int)disp16bit.at<ushort>(j, i);
                int calcDistance = avgBF/disparity;
                distanceMap.at<uchar>(j, i) = calcDistance;
            }
        }

        // Find average disparity and distance from ROI
        float averageDisparity = mean(disp16bit(ROI))[0];
        float estimatedDistance = mean(distanceMap(ROI))[0];
        rectangle(distanceMap, ROI, white, 2);

        // Calculate Error
        float absoluteError = abs(estimatedDistance-actualDistance);
        float relativeError = absoluteError/actualDistance;
        float percentageError = (relativeError * 100);

        // Update max error
        if (percentageError > maxPercentageError){
            maxPercentageError = percentageError;
        }
        if (absoluteError > maxAbsoluteError){
            maxAbsoluteError = absoluteError;
        }

        cout << "Known Target (" << actualDistance << "cm) " << " estimated distance: " << format("%.2f", estimatedDistance)<< "cm" << endl;
        DataFile << actualDistance << "," << averageDisparity << ", " << estimatedDistance << ", " << percentageError << endl;

        imshow("Error Calculation", distanceMap);
        waitKey(500);
    }

    cout << endl << "Maximum percentage error in known images identified as +-" << format("%.2f", maxPercentageError) << "% " << endl;
    cout << "Largest error (cm) in known images identified as +-" << format("%.2f", maxAbsoluteError) << "cm " << endl << endl;
}
void DistCalc(){
    // ======= FIND DISTANCES FROM UNKNOWN IMAGES WITH DISTANCE MAP ========================================
    DataFile << endl << "Unknown Distances:" << endl;
    cout << "Stepping through the unknown images and calculating distance:" << endl;
    DataFile << "ImageNo, Avg Disparity, Estimated Distance" << endl;
    for(int ImageNum = 0; ImageNum < 8; ImageNum++)
    {
        string pathL = "../Task4/Unknown Targets/left" +to_string(ImageNum)+".jpg";
        string pathR = "../Task4/Unknown Targets/right"+to_string(ImageNum)+".jpg";

        Mat disp16bit = createDispMat(pathL, pathR, calibMat, sgbm);            // create 16-bit disparity map for calculations

        // Create distance image
        Mat distanceMap(imSize, CV_8U, 255);
        for (int i = 0; i < (int)imSize.width; i++)
        {
            for(int j = 0; j < (int)imSize.height; j++)
            {
                int disparity = (int)disp16bit.at<ushort>(j, i);
                int calcDistance = avgBF/disparity;
                distanceMap.at<uchar>(j, i) = calcDistance;
            }
        }

        // Find average disparity and distance from ROI
        float averageDisparity = mean(disp16bit(ROI))[0];
        float estimatedDistance = mean(distanceMap(ROI))[0];
        rectangle(distanceMap, ROI, white, 2);

        DataFile << ImageNum << ", " << averageDisparity << ", " <<  estimatedDistance << endl;
        cout << "Unknown Target " << ImageNum << " estimated distance: " << format("%.2f", estimatedDistance) << "cm" << endl;

        imshow("Finding Unknown Distance", distanceMap);
        waitKey(500);
    }
}

// ================================================================================================
int main()
{
    string csv = "../Task4/data.csv";
    DataFile = ofstream(csv); // Define CSV output
    setup();

    while(DataFile.is_open()){
        cout << "---------------" << endl <<
                "Choose one of the following options by pressing the corresponding number." << endl <<
                "0 - Quit" << endl <<
                "1 - Run BF Calculation" << endl <<
                "2 - Run Error Calculation" << endl <<
                "3 - Run Distance Estimation" << endl << endl;

        int k;
        cin >> k;

        if (k == 1){
            cout << "Option 1: Finding Disparity and BF value for Known Targets" << endl;
            calcBF();
            destroyAllWindows();
        }
        else if(k == 2){
            if (avgBF != 0){
                cout << "Option 2: Running Distance Estimation on Known Targets" << endl;
                ErrorCalc();
                destroyAllWindows();
            }
            else{
                cout << "BF value not calculated, cannot continue. Please run option W first." << endl;
            }
        }
        else if(k == 3){
            if (avgBF != 0){
                cout << "Option 3: Running Distance Estimation on Unknown Targets" << endl;
                DistCalc();
                destroyAllWindows();
            }
            else{
                cout << "BF value not calculated, cannot continue. Please run option W first." << endl;
            }
        }
        else if(k == 0){
            cout << "Quitting" << endl;
            DataFile.close();
        }
    }
}





