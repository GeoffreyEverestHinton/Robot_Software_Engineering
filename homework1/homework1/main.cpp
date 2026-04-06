#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main()
{
	Mat img = imread("agi.jpg");
	namedWindow("Picture", WINDOW_NORMAL);
	imshow("Picture", img);
	waitKey(0);
	return 0;
}
