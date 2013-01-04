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

int  main(){
	CvCapture* capture;
	capture = cvCaptureFromCAM(CV_CAP_ANY);

	//Mengecek ketersediaan kamera
	if (!capture){
		fprintf(stderr, "ERROR: Kamera tidak terdeteksi\n");
	}

	IplImage* frame0;
	IplImage* frame1;
	IplImage* movingframe;
	IplImage* grayframe0;
	IplImage* grayframe1;
	IplImage* graymovingframe;

	//Mengekstrak frame dari video
	frame1 = cvQueryFrame(capture);

	//Mengecek ketersediaan frame
	if(!frame1)
		return -1;

	//Meng-clone frame dari frame1 ke frame0
	frame0= cvCloneImage(frame1);

	//Mengambil properti-properti frame
	int column = frame1->height;
	int row = frame1->width;
	int depth = frame1->depth;

	//Mengalokasikan memori untuk gambar
	movingframe = cvCreateImage(cvSize(row,column),depth,3);
	grayframe0 = cvCreateImage(cvSize(row,column),depth,1);
	grayframe1 = cvCreateImage(cvSize(row,column),depth,1);
	graymovingframe = cvCreateImage(cvSize(row,column),depth,1);

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
