#pragma once
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
void draw_rotatedRect(Mat &img, RotatedRect mr, Scalar color, int thickness = 2)
{
	Point2f pts[4];
	mr.points(pts);
	for (int i = 0; i < 4; ++i) {
		line(img, pts[i], pts[(i + 1) % 4], color, thickness);
	}
}
