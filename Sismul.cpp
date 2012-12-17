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

bool BacaFileVideo(char* namafile);
int BacaFrameRate();
int BacaJumlahFrame();
int BacaLebar();
int BacaTinggi();
int BacaIndexFrame();
void PergiKeFrame(int);
void StartVideo();
void StartVideo(int awal, int akhir);
void StopVideo();
void UbahVideo(int awal, int akhir, char* lokasi);
void SelisihkanFrames(int awal,int akhir);

int totalframe;
CvCapture* pointerscreenshot;

int main(int argc, char** argv) {
	char* namafile;
	IplImage* frame;

	cvNamedWindow("OpenCV Video", CV_WINDOW_AUTOSIZE);
/*	CvCapture* capture = cvCreateFileCapture("cursor.avi");

	while (1) {

		frame = cvQueryFrame(capture);
		if (!frame)
			break;
		cvShowImage("OpenCV Video", frame);

		char c = cvWaitKey(33);
		if (c == 27)
			break;
	}
*/
	cvReleaseCapture(&capture);
	cvDestroyWindow("OpenCV Video");
}

bool BacaFileVideo(char* namafile)
{
	pointerscreenshot = cvCaptureFromFile(namafile);
	if(!namafile)
		return false;
	if(!pointerscreenshot)
		return false;

	totalframe = (int)cvGetCaptureProperty(pointerscreenshot,CV_CAP_PROP_FRAME_COUNT);
	return true;
}

int BacaFrameRate()
{
	return (int)cvGetCaptureProperty(pointerscreenshot,CV_CAP_PROP_FPS);
}

int BacaJumlahFrame()
{
	return totalframe;
}

int BacaLebar()
{
	return (int)cvGetCaptureProperty(pointerscreenshot, CV_CAP_PROP_FRAME_WIDTH);
}

int BacaTinggi()
{
	return (int)cvGetCaptureProperty(pointerscreenshot, CV_CAP_PROP_FRAME_HEIGHT);
}

int BacaIndexFrame()
{
	return (int)cvGetCaptureProperty(pointerscreenshot,CV_CAP_PROP_POS_FRAMES );
}
