/*
 * AINT 308 - C1
 * Rachel Ireland-Jones
 * March 2023
 *
 * Task 1 - Colour Sorter
*/

/* ---- Include ---- */
#include <iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/opencv_modules.hpp>

/* ---- Namespaces and Classes ---- */
using namespace std;
using namespace cv;

/* ---- Variables ---- */
string stored[30];
string colours[] = {"red", "green", "blue", "undefined"};
int R;
int G;
int B;
int deadpixels;

int h = 640;
int w = 480;

static Vec3b red(0, 0, 255);
static Vec3b green(0, 255, 0);
static Vec3b blue(255, 0, 0);
static Vec3b white(255, 255, 255);

/* ---- Functions ----*/
void readPixel(Mat im, int col, int row)
{
    Vec3b PixelValue = im.at<Vec3b>(col,row);

    int b = PixelValue[0];
    int g = PixelValue[1];
    int r = PixelValue[2];

    int min = 20;
    int max = 135;

    if ((r < min && b < min && g < min) || (r > max && g > max && b > max)){
        im.at<Vec3b>(col,row) = white;
        deadpixels++;
    }
    else if(r > g + 45 && r > b + 30){
        im.at<Vec3b>(col,row) = red;
        R++;
    }
    else if(g > r + 30 && g > b + 20){
        im.at<Vec3b>(col,row) = green;
        G++;
    }
    else if(b > g + 50 && b > r + 50){
        im.at<Vec3b>(col,row) = blue;
        B++;
    }
    else{
        im.at<Vec3b>(col,row) = white;
        deadpixels++;
    }
}

string calculateColour(){
    string colour;

    int thresh = (w * h) * 0.02;

    if (R + G + B > thresh)
    {
        if(R > B && R > G)
        {
            colour = colours[0];
        }
        else if(G > B && G > R)
        {
            colour = colours[1];
        }
        else if(B > G && B > R)
        {
            colour = colours[2];
        }
    }
    else{
        colour = colours[3];
    }

    cout << R << "\t";
    cout << G << "\t";
    cout << B << "\t";
    cout << deadpixels << endl;

    return colour;
}

void displayCarsofColour(int j, string pathFolder){

    // if any images still up, close them
    destroyAllWindows();

    // update colour being shown
    string c = colours[j];

    cout << "Cars that are " << c << ": ";

    // loop through all 30 images
    for(int i = 0; i < 30; i++)
    {
        // if the car is the correct colour
        if (stored[i] == c)
        {
            // display the car image once again
            string title = "Car " + to_string(i);
            string path = pathFolder + to_string(i) + ".png";
            Mat car = imread(path);

            // make it small so not crowding screen
            resize(car, car, Size(320, 240), INTER_LINEAR);
            imshow(title, car);

            // print the car number
            cout << i << ", ";
        }
    }
    cout << endl;
    waitKey(0);
}

/* ---- main ---- */
int main(){

    // define the folder path
    string PathToFolder = "../Task1/Car Images2/";

    // loop through the 30 car images
    for(int n=0; n<30; ++n){

        // reset photo total rgb values;
        R = 0;
        G = 0;
        B = 0;

        // generate the image file using n and the folder path
        string PathToImage = PathToFolder+to_string(n)+".png";

        // load car image at the file path location
        Mat Car=imread(PathToImage);

        // resize the car image
        resize(Car, Car, Size(h, w), INTER_LINEAR);
        imshow("car", Car);

        GaussianBlur(Car, Car, Size(11, 11), 0);

        // loop through image, reading the pixel values
        for (int hh = 0; hh < Car.rows; hh++){
            for(int ww = 0; ww < Car.cols; ww++){

                // private function that reads the pixel and determines the most prominant colour, incrementing R, G or B
                readPixel(Car, hh, ww);
            }
        }

        // checks values of R, G and B and calculates the colour across the whole image

        stored[n] = calculateColour();
        cout << "Car " << n << " Analysed: " << stored[n] << endl;

        while(waitKey(10)!= 'x'){
            imshow("Recoloured", Car);
        }
    }

    // once all photos have been analysed - show images of the required colour
    // haven't implemented a way to choose colour, so showns all of them in three sections
    for (int j = 0; j < 4; j++){
        // (red, green, blue, undefined)
        displayCarsofColour(j, PathToFolder);
    }

}
