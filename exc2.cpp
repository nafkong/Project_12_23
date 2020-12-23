#include<iostream>
#include<opencv2/opencv.hpp>
#include <algorithm>

using namespace cv;
using namespace std;

int calcHOG(Mat roiMag, float* hist,Mat roiAgl)
{
	for (int n = 0; n < roiMag.rows; n++) {
		for (int m = 0; m < roiMag.cols; m++) {
			hist[(int)(roiAgl.at<float>(n, m) / 45)]++;
		}
	}
	return 0;
}

int main()
{
	Mat srcMat, ROI;
	srcMat = imread("picture.png", 0);
	ROI = imread("picture_ROI.png", 0);
	imshow("1", ROI);
	int cnt = 0;
	int nAngle = 8;
	int reCode = 0;
	int bins = (srcMat.cols / ROI.cols) * (srcMat.rows / ROI.rows) * nAngle;
	int nX = srcMat.cols-ROI.cols;
	int nY = srcMat.rows-ROI.rows;
	Mat gx, gy, mag_ROI, angle_ROI;
	Mat mag, angle;
	int distance[10000] = { 0 };

	Sobel(srcMat, gx, CV_32F, 1, 0, 1);
	Sobel(srcMat, gy, CV_32F, 0, 1, 1);
	cartToPolar(gx, gy, mag, angle, true);
	Sobel(ROI, gx, CV_32F, 1, 0, 1);
	Sobel(ROI, gy, CV_32F, 0, 1, 1);
	cartToPolar(gx, gy, mag_ROI, angle_ROI, true);
	float hist1[100] = { 0 };
	for (int i = 0; i < nY; i++) {
		for (int j = 0; j < nX; j++) {
			Mat roiMat, roiMag, roiAgl;
			Rect roi;
			roi.height = ROI.rows;
			roi.width = ROI.cols;
			roi.x = j;
			roi.y = i;
			roiMat = srcMat(roi);
			roiMag = mag(roi);
			roiAgl = angle(roi);
			int head = (i * nX + j) * nAngle;
			float hist2[100] = { 0 };
			calcHOG(ROI, hist1, angle_ROI);
			calcHOG(roiMat, hist2, roiAgl);
			int result = 0;
			for (int n = 0; n < bins; n++) {
				result = pow((hist1[n] - hist2[n]), 2);
				result += result;
			}
			distance[i * nY + j] = result;
		}
	}
	int i_find=0;
	for (int i = 0; i < bins; i++) {
		if (distance[i] == *min_element(distance, distance + bins))
			i_find = i;
//		cout << distance[i] << endl;
	}
	int i = i_find / nX;
	int j = i_find % nX;
	Rect roi;
	roi.height = ROI.rows;
	roi.width = ROI.cols;
	roi.x = j;
	roi.y = i;
	rectangle(srcMat, roi, Scalar(0, 0, 255), 1, 8, 0);
	imshow("srcMat", srcMat);
	waitKey(0);
}