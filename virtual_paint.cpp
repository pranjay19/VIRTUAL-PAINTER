//Including all the header files required for the project.


#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


#include <iostream>
#include <string>
#include <sstream>// the main use is for the reading and the writing of the files.

using namespace cv;
using namespace std;


// read write image
/*
int main(int argc, const char** argv) {
	//read images
	Mat color = imread("Resources/lambo.png");
	Mat gray = imread("Resources/lambo.png", 0);

	//write images
	imwrite("lambogray.png", gray);

	//get same pixel with opencv function

	//to access rows and columns of image
	int myRow = color.cols - 1;
	int myCol = color.rows - 1;
	//to access ech pixel of image
	Vec3b pixel = color.at<Vec3b>(myRow, myCol);
	cout << "Pixel value (B,G,R): (" << (int)pixel[0] << "," << (int)pixel[1] << "," << (int)pixel[2] << ")" << endl;

	//show images
	imshow("lambo BGR", color);
	imshow("lambo gray", gray);

	//wait for any key press
	waitKey(0);
	return 0;
}
*/

//// video capturing
/*
int main() {
	VideoCapture cap;
	cap.open(0);
	for (;;){
		Mat frame;
		cap >> frame;
		imshow("video", frame);
		waitKey(1);
	}


}

*/
/*

///////////////  Detecting colours with the help of Trackbars. //////////////////////
Mat imgHSV, mask;
int hmin = 0, smin = 110, vmin = 153;
int hmax = 19, smax = 240, vmax = 255;

VideoCapture cap(0);
Mat img;
	
int main(){
		

		namedWindow("Trackbars", (640, 200));
		createTrackbar("Hue Min", "Trackbars", &hmin, 179);
		createTrackbar("Hue Max", "Trackbars", &hmax, 179);
		createTrackbar("Sat Min", "Trackbars", &smin, 255);
		createTrackbar("Sat Max", "Trackbars", &smax, 255);
		createTrackbar("Val Min", "Trackbars", &vmin, 255);
		createTrackbar("Val Max", "Trackbars", &vmax, 255);

		while (true) {

			cap.read(img);

			cvtColor(img, imgHSV, COLOR_BGR2HSV);



			Scalar lower(hmin, smin, vmin);
			Scalar upper(hmax, smax, vmax);
			
			inRange(imgHSV, lower, upper, mask);

			imshow("Image", img);
			
			imshow("Image Mask", mask);
			waitKey(1);
		}
		
}
	
	
	*/




/////////////////  Project 1 - Virtual Painter //////////////////////

Mat img;

VideoCapture cap(0);

vector<vector<int>> newPoints;  // to store all points

/////////////////////  COLOR VALUES ////////////////////////////////
vector<vector<int>> myColors{ {142,179,128,179,255,255}, // RED
								{63,255,24,97,255,255},//green
								};// Green
vector<Scalar> myColorValues{ {0,0,255},		// RED
								{0,255,0} ,//green
								};//brown
////////////////////////////////////////////////////////////////////
//Template class for 2D points specified by its coordinates x and y.
Point getContours(Mat image) {


	vector<vector<Point>> contours;

	vector<Vec4i> hierarchy;

	findContours(image, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);// (Here the image is the source, there are the detected contours, contains information about the image topology, defines the contour retrieval mode,defines the contour approximation method).

																				 //drawContours(img, contours, -1, Scalar(255, 0, 255), 2);

																				 // By use of RETR_EXTERNAL :-Retrieves only the extreme outer contours. As you can see only three contours are created i.e the external one .No Parent, Child relation ship is given.All External contours are at same hierarchy(Level)

																				 //CHAIN_APPROX_SIMPLE:- length of contours stored is 4 i.e only corner four points are stored in array.

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	Point myPoint(0, 0);

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);

		cout << area << endl;

		string objectType;

		if (area > 100)
		{
			
			float peri = arcLength(contours[i], true);

			approxPolyDP(contours[i], conPoly[i], 0.01 * peri, true);


			cout << conPoly[i].size() << endl;

			boundRect[i] = boundingRect(conPoly[i]);

			myPoint.x = boundRect[i].x + boundRect[i].width / 2;

			myPoint.y = boundRect[i].y;

			drawContours(img, conPoly, i, Scalar(0, 255, 0), 2);

			//rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
		}
	}
	return myPoint;
}


vector<vector<int>> findColor(Mat img)
{

	Mat imgHSV;

	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	for (int i = 0; i < myColors.size(); i++)
	{

		Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);

		Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);

		Mat mask;

		inRange(imgHSV, lower, upper, mask);

		

		Point myPoint = getContours(mask);

		if (myPoint.x != 0) {

			newPoints.push_back({ myPoint.x,myPoint.y,i });
			
		}
	}
	return newPoints;
}

void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues)
{

	for (int i = 0; i < newPoints.size(); i++)
	{
		
		circle(img, Point(newPoints[i][0], newPoints[i][1]), 6, myColorValues[newPoints[i][2]], FILLED);
	}
}
int event;

void main() {

	

		
	

	

	while (true) {

		cap.read(img);

		newPoints = findColor(img);

		drawOnCanvas(newPoints, myColorValues);
		// using the below mentioned code, we can add text on the real-time image.
		// org is used for justifying the coordinates in x and y axis time space.
		//using the putText function we can add the text.
		//
		Point org(100, 30);
		
		putText(img, "WELCOME TO VIRTUAL PAINTER", org,FONT_HERSHEY_SCRIPT_COMPLEX, 1,Scalar(0,255,0), 2, LINE_AA);
		
		
		Point org2(350, 50);
		
		putText(img, "By:- PRANJAY GULERIA", org2,FONT_HERSHEY_COMPLEX_SMALL, 0.5,Scalar(0, 0, 255), 1, LINE_AA);

		
		

		imshow("Image", img);

		
		
		waitKey(1);
	}
}









