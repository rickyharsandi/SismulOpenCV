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

#define NUMBER_OF_FEATURES 400
#define MIN_VECT_COMP 2000
#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982

using namespace cv;
using namespace std;

//Fungsi pangkat
inline static double square(int a)
{
	return a * a;
}

//Fungsi untuk mengalokasikan memori
inline static void allocateOnDemand(IplImage **img, CvSize size, int depth, int channels) {
	if (*img != NULL)
		return;

	*img = cvCreateImage(size, depth, channels);
	if (*img == NULL) {
		fprintf(stderr, "Error: Tidak bisa mengalokasikan gambar\n");
		exit(-1);
	}
}

int  main(){
	//Mendeklarasikan pointer capture dari kamera
	CvCapture* capture;
	capture = cvCaptureFromCAM(CV_CAP_ANY);

	//Mengecek ketersediaan kamera
	if (!capture){
		fprintf(stderr, "ERROR: Kamera tidak terdeteksi\n");
	}

	//Membaca ukuran frame (height dan width) dari pointer capture
	CvSize frame_size;
	frame_size.height = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	frame_size.width = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);

	//Mendeklarasikan window
	cvNamedWindow("Frame 1", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Frame 2", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Optical Flow", CV_WINDOW_AUTOSIZE);

	//Mendeklarasikan nilai minimal dan maksimal HSV
	CvScalar hsv_min = cvScalar(0, 30, 80, 0);
	CvScalar hsv_max = cvScalar(50, 100, 255, 0);

	//Menampilkan gambar yang ditangkap dari kamera
	while(1){
		//Mengekstrak frame 2 ke frame1
		frame1 = cvQueryFrame(capture);

		//Konversi dari RGB ke Gray
		cvCvtColor(frame0,grayframe0,CV_RGB2GRAY);
		cvCvtColor(frame1,grayframe1,CV_RGB2GRAY);

		//Menghitung perbedaan antar-frame
		cvAbsDiff(grayframe0,grayframe1,graymovingframe);
		cvAbsDiff(frame0,frame1,movingframe);

		//Menampilkan gambar asli
		cvShowImage("Original Image",frame1);

		//Menampilkan perbedaan antar-frame dari gambar-gambar Gray
		cvShowImage("Gray Image Difference ",graymovingframe);

		//Menampilkan perbedaan antar-frame dari gambar-gambar RGB
		cvShowImage("RGB Image Difference ",movingframe);

		//Keluar dari looping program menggunakan tombol escape
		if ((cvWaitKey(10) & 255) == 27)
			break;

		//Mengkopi frame1 ke frame0
		cvCopy(frame1, frame0);
	}

	//Membersihkan memori yang telah digunakan
	cvReleaseCapture(&capture);
	cvReleaseImage(&grayframe0);
	cvReleaseImage(&grayframe1);
	cvReleaseImage(&graymovingframe);
	cvReleaseImage(&frame1);

	return 0;
}
