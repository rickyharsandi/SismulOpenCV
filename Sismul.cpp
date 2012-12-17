/*
 * Sismul.cpp
 *
 *  Created on: Dec 17, 2012
 *      Author: renodesper
 */




#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <cvaux.h>
#include <cvwimage.h>
#include <cxmisc.h>
#include <ml.h>

using namespace cv;

int main(int argc, char** argv) {
	cvNamedWindow("OpenCV Video", CV_WINDOW_AUTOSIZE);
	CvCapture* capture = cvCreateFileCapture("cursor.avi");
	IplImage* frame;
	while (1) {

		frame = cvQueryFrame(capture);
		if (!frame)
			break;
		cvShowImage("OpenCV Video", frame);

		char c = cvWaitKey(33);
		if (c == 27)
			break;
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("OpenCV Video");
}
