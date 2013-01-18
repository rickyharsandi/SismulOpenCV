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
	while(true){
        	static IplImage *frame = NULL, *frame1 = NULL, *frame1_1C = NULL, *frame2_1C = NULL, *eig_image = NULL, *temp_image = NULL, *pyramid1 = NULL, *pyramid2 = NULL;

		//Mengambil frame (Frame 1) dari pointer capture dan cek ketersediaan frame
		frame = cvQueryFrame(capture);
		if (frame == NULL)
		{
			fprintf(stderr, "Error: Frame 1 tidak ditemukan\n");
			return -1;
		}

		//Mengalokasikan memori untuk frame yang diambil
		allocateOnDemand(&temp_image, frame_size, IPL_DEPTH_8U, 3);
		
		//Membuat backup dari Frame 1
		allocateOnDemand(&frame1, frame_size, IPL_DEPTH_8U, 3);
		cvConvertImage(frame, frame1);
		
		//Mengalokasikan memori untuk frame dengan 1 channel
		allocateOnDemand(&frame1_1C, frame_size, IPL_DEPTH_8U, 1);

		//Mengubah frame dari BGR menjadi HSV
		cvCvtColor(frame, temp_image, CV_BGR2HSV);
		cvInRangeS(temp_image, hsv_min, hsv_max, frame1_1C);
		cvSet(temp_image, cvScalar(0, 0, 0, 0));
		cvCopy(frame, temp_image, frame1_1C);
		cvConvertImage(temp_image, frame1_1C);

		//Mengambil frame selanjutnya (Frame 2) dari pointer capture dan cek ketersediaan frame
		frame = cvQueryFrame(capture);
		if (frame == NULL)
		{
			fprintf(stderr, "Error: Frame 2 tidak ditemukan\n");
			return -1;
		}

		//Mengalokasikan memori untuk frame dengan 1 channel
		allocateOnDemand(&frame2_1C, frame_size, IPL_DEPTH_8U, 1);

		//Mengubah frame dari BGR menjadi HSV
		cvCvtColor(frame, temp_image, CV_BGR2HSV);
		cvInRangeS(temp_image, hsv_min, hsv_max, frame2_1C);
		cvSet(temp_image, cvScalar(0, 0, 0, 0));
		cvCopy(frame, temp_image, frame2_1C);
		cvConvertImage(temp_image, frame2_1C);

		//Mengalokasikan memori untuk Pendeteksi Fitur (Gerakan) Shi dan Tomasi
		allocateOnDemand(&eig_image, frame_size, IPL_DEPTH_32F, 3);
		allocateOnDemand(&temp_image, frame_size, IPL_DEPTH_32F, 3);
		
		//Keluar dari looping program menggunakan tombol escape
		if ((cvWaitKey(10) & 255) == 27)
			break;
	}

	//Membersihkan memori yang telah digunakan
	cvReleaseCapture(&capture);
	cvReleaseImage(&grayframe0);
	cvReleaseImage(&grayframe1);
	cvReleaseImage(&graymovingframe);
	cvReleaseImage(&frame1);

	return 0;
}
