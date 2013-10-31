#include <cv.h>
#include <highgui.h>
int main()
{
	CvCapture *capt= cvCaptureFromCAM(0);
	IplImage *frame = cvQueryFrame(capt);
	IplImage *binary = cvCreateImage(cvGetSize(frame),8,1);
    IplImage *pic = cvLoadImage("Tux.jpg");
//    CvCapture *vid = cvCaptureFromAVI("video.avi");
  //  IplImage *pic = cvQueryFrame(vid);
    IplImage *blank = cvCreateImage(cvGetSize(pic),8,3);
    IplImage *neg = cvCreateImage(cvGetSize(frame),8,3);
    IplImage *cpy = cvCreateImage(cvGetSize(frame),8,3);
    CvMemStorage *storage = cvCreateMemStorage(0);
	CvMemStorage *miao = cvCreateMemStorage(0);

	CvSeq *first=NULL;
	CvSeq *CONTOrni=NULL;

	cvNamedWindow("AR",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Binaria",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Originale",CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("marker",CV_WINDOW_AUTOSIZE);
    CvPoint corners[4];
    CvPoint2D32f src[4],dst[4];
    int i,quatt=0;
    src[3].x= 0;
    src[3].y= 0;
    src[0].y= 0;
    src[2].x= 0;

    CvMat *matrice = cvCreateMat(3,3,CV_32FC1);

	while(1)
	{
		cvFlip(frame,frame,1);
        cvShowImage("Originale",frame);
        //cvFlip(pic,pic,1);
		cvCvtColor(frame,binary,CV_BGR2GRAY);
		cvSmooth(binary,binary,CV_GAUSSIAN,5,3);
        cvAdaptiveThreshold(binary,binary,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY_INV,71,15);
        //cvThreshold(binary,binary,0,255,CV_THRESH_BINARY | CV_THRESH_OTSU);
        cvThreshold(binary,binary,0,255,CV_THRESH_BINARY | CV_THRESH_OTSU);
        //cvCanny(binary,binary,0,255);
        cvSmooth(binary,binary,CV_GAUSSIAN,7,3);
        //cvDilate(binary,binary,NULL,3);
	//    cvNot(binary,binary);
        cvFindContours(binary,storage,&first,sizeof(CvContour),CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));
		if(first != NULL)
        {
            CONTOrni = cvApproxPoly(first,sizeof(CvContour),miao,CV_POLY_APPROX_DP,19.,1);
            for(first=CONTOrni; first!=NULL;first=first->h_next)
		    {

                if(first->v_next && first->total == 4 && first->v_next->total == 4 && cvCheckContourConvexity(CONTOrni) == 0)
                {
                    cvDrawContours(frame,first,CV_RGB(255,0,0),CV_RGB(0,0,0),0,3,8,cvPoint(0,0));
                    for(i=0; i < 4; i++)
                    {
                        CvPoint *p = CV_GET_SEQ_ELEM(CvPoint,first,i);
                        corners[i] = *p;
                        cvCircle(frame,*p,5,CV_RGB(0,255,100),1,8,0);
                    }
                    quatt=1;
                }

		    }
            if(quatt == 1)
            {
                cvZero(blank);
                cvNot(blank,blank);

                src[0].x= (float) pic->width;
                src[1].x= (float) pic->width;
                src[1].y= (float) pic->height;
                src[2].y= (float) pic->height;

                //Angoli del quadrato
                dst[0].x= corners[0].x;
                dst[0].y= corners[0].y;

                dst[1].x= corners[1].x;
                dst[1].y= corners[1].y;

                dst[2].x= corners[2].x;
                dst[2].y= corners[2].y;

                dst[3].x= corners[3].x;
                dst[3].y= corners[3].y;

                cvGetPerspectiveTransform (src, dst, matrice);

                cvZero(neg);
                cvZero(cpy);

                cvWarpPerspective(pic,neg, matrice);
                cvWarpPerspective(blank,cpy,matrice);
                cvNot(cpy,cpy);

                cvAnd(cpy,frame,cpy);
                cvOr(cpy,neg,frame);
            }
        }
		cvShowImage("AR",frame);
		cvShowImage("Binaria",binary);
        quatt = 0;
		char lol = cvWaitKey(15);
		if(lol == 'q')
			break;
        frame = cvQueryFrame(capt);
	}
}
