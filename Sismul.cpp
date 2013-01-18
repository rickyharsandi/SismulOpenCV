/*
 * Sismul.cpp
 *
 *  Created on: Dec 17, 2012
 *      Author: renodesper
 *
 *      - Aus Firdaus		10109013
 *      - Gilang Rahmat R	10109023
 *      - Boy Sandy G. A.	10109030
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
		
		//Menyimpan fitur gerakan dari frame 1 pada array
		CvPoint2D32f frame1_features[NUMBER_OF_FEATURES];

		int nof;
		nof = NUMBER_OF_FEATURES;

		//Mencari fitur yang bisa dideteksi berdasarkan sudut (corner) lalu menyimpan fiturnya pada array
		cvGoodFeaturesToTrack(frame1_1C, eig_image, temp_image, frame1_features, &nof, .05, .001, NULL);
		CvPoint2D32f frame2_features[NUMBER_OF_FEATURES];

		char optical_flow_found_feature[NUMBER_OF_FEATURES];
		float optical_flow_feature_error[NUMBER_OF_FEATURES];

		CvSize optical_flow_window = cvSize(3, 3);
		CvTermCriteria optical_flow_termination_criteria = cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .1);

		allocateOnDemand(&pyramid1, frame_size, IPL_DEPTH_8U, 1);
		allocateOnDemand(&pyramid2, frame_size, IPL_DEPTH_8U, 1);

		//Menjalankan algoritma piramida Lucas Kanade
		cvCalcOpticalFlowPyrLK(frame1_1C, frame2_1C, pyramid1, pyramid2, frame1_features, frame2_features,
							   nof, optical_flow_window, 5, optical_flow_found_feature,
							   optical_flow_feature_error, optical_flow_termination_criteria, 0);

		double angle;
		double hypotenuse;
		double xt = 0, yt = 0;

		for(int i = 0; i < nof; i++)
		{
			if (optical_flow_found_feature[i] == 0)
				continue;

			CvPoint a,b;
			a.x = (int) frame1_features[i].x;
			a.y = (int) frame1_features[i].y;
			b.x = (int) frame2_features[i].x;
			b.y = (int) frame2_features[i].y;

			xt += (a.x - b.x);
			yt += (a.y - b.y);

			angle = atan2( (double) a.y - b.y, (double) a.x - b.x );
			hypotenuse = sqrt(square(a.y - b.y) + square(a.x - b.x));

			if (hypotenuse < 10 || hypotenuse > 100)
				continue;

			int line_thickness;
			line_thickness = 1;

			//Menggambar lingkaran sebagai titik awal gerakan
			cvCircle(frame1, a, 2, CV_RGB(0, 255, 0), line_thickness, CV_AA);
			cvCircle(frame1, b, 2, CV_RGB(0, 255, 0), line_thickness, CV_AA);

			//Menggambar garis untuk menunjukan pergerakan
			cvLine(frame1, a, b, CV_RGB(0, 0, 255), line_thickness, CV_AA);

			//Menggambar garis untuk menunjukan kepala panah
			a.x = (int) (b.x + 9 * cos(angle + PI / 4));
			a.y = (int) (b.y + 9 * sin(angle + PI / 4));
			cvLine(frame1, a, b, CV_RGB(0, 255, 0), line_thickness, CV_AA);

			//Menggambar garis untuk menunjukan kepala panah
			a.x = (int) (b.x + 9 * cos(angle - PI / 4));
			a.y = (int) (b.y + 9 * sin(angle - PI / 4));
			cvLine(frame1, a, b, CV_RGB(0, 255, 0), line_thickness, CV_AA);
		}

		//Khusus mendeteksi gerakan dari titik pusat frame (tengah)
		CvPoint c, d;
		c.x = frame_size.width / 2;
		c.y = frame_size.height / 2;
		d.x = c.x + xt;
		d.y = c.y + yt;

		angle = atan2(yt, xt);
		hypotenuse = sqrt(square(yt) + square(xt));

		int line_thickness;
		line_thickness = 1;

		d.x = (int) (c.x - hypotenuse * cos(angle));
		d.y = (int) (c.y - hypotenuse * sin(angle));

		//Menggambar garis untuk menunjukan titik pusat frame
		cvLine(frame1, c, d, CV_RGB(255, 0, 0), line_thickness, CV_AA, 0);

		xt = (abs(xt) > MIN_VECT_COMP) ? (50 * (xt > 0 ? -1 : 1)) : 0;
		yt = (abs(yt) > MIN_VECT_COMP) ? (50 * (yt > 0 ? -1 : 1)) : 0;

		d.x = c.x + xt;
		d.y = c.y + yt;

		line_thickness = 3;

		//Menggambar lingkaran sebagai titik awal gerakan dan garis untuk menunjukan pergerakan
		cvCircle(frame1, c, 2, CV_RGB(255, 0, 0), line_thickness, CV_AA);
		cvLine(frame1, c, d, CV_RGB(255, 0, 0), line_thickness, CV_AA, 0);

		//Menggambar garis untuk menunjukan kepala panah
		c.x = (int) (d.x + 9 * cos(angle + PI / 4));
		c.y = (int) (d.y + 9 * sin(angle + PI / 4));
		cvLine(frame1, c, d, CV_RGB(255, 0, 0), line_thickness, CV_AA, 0);

		//Menggambar garis untuk menunjukan kepala panah
		c.x = (int) (d.x + 9 * cos(angle - PI / 4));
		c.y = (int) (d.y + 9 * sin(angle - PI / 4));
		cvLine(frame1, c, d, CV_RGB(255, 0, 0), line_thickness, CV_AA, 0);

		printf("%f %f %d\n", xt, yt, nof);

		cvShowImage("Optical Flow", frame1);
		cvShowImage("Frame 1", frame1_1C);
		cvShowImage("Frame 2", frame2_1C);

		//Keluar dari looping program menggunakan tombol escape
		if ((cvWaitKey(10) & 255) == 27)
			break;
	}

	//Membersihkan memori yang telah digunakan
	cvReleaseCapture(&capture);
	cvDestroyWindow("Optical Flow");
	cvDestroyWindow("Frame 1");
	cvDestroyWindow("Frame 2");

	return 0;
}
