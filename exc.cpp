#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() 
{
	Mat frame, tempMat, refMat, resultMat, dispMat;
	int cnt = 0;
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;
	VideoCapture cap(0);
	while(1)
	{
		cap >> frame;
//		cvtColor(frame, frame, COLOR_BGR2GRAY);
		if (cnt == 0)
		{
			Rect2d r;
			r = selectROI(frame, true);
			tempMat = frame(r);
			tempMat.copyTo(refMat);
			destroyAllWindows();
		}
		matchTemplate(frame, refMat, resultMat, TM_SQDIFF);
		normalize(resultMat,resultMat,0,1,NORM_MINMAX,-1,Mat());
		minMaxLoc(resultMat, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
		matchLoc = minLoc;
		frame.copyTo(dispMat);
		rectangle(dispMat, matchLoc, Point(matchLoc.x + refMat.cols, matchLoc.y + refMat.rows), 1, 8, 0);
		cnt++;
		//imshow("tempMat", tempMat);
		//imshow("refMat", refMat);
		imshow("dispMat", dispMat);
		waitKey(30);
	}
}