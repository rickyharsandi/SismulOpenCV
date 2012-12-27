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

 int main() {
   CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
   if ( !capture ) {
     fprintf( stderr, "ERROR: capture is NULL \n" );
     getchar();
     return -1;
   }

   cvNamedWindow( "OpenCV - Sistem Multimedia", CV_WINDOW_AUTOSIZE );

   while ( 1 ) {

     IplImage* frame = cvQueryFrame( capture );
     if ( !frame ) {
       fprintf( stderr, "ERROR: frame is null...\n" );
       getchar();
       break;
     }
     cvShowImage( "OpenCV - Sistem Multimedia", frame );

     if ( (cvWaitKey(10) & 255) == 27 ) break;
   }

   cvReleaseCapture( &capture );
   cvDestroyWindow( "OpenCV - Sistem Multimedia" );
   return 0;
 }
