#include <cv.h>
#include <highgui.h>
int main()
{
	CvCapture *capt= cvCaptureFromCAM(0);
	IplImage *frame = cvQueryFrame(capt);
	IplImage *binary = cvCreateImage(cvGetSize(frame),8,1);
    IplImage *pic = cvLoadImage("pic.jpg");
    IplImage *sub = cvCreateImage(cvGetSize(pic),8,3);
    IplImage *marker = cvCreateImage(cvGetSize(frame),8,3);

    CvMemStorage *storage = cvCreateMemStorage(0);
	CvMemStorage *miao = cvCreateMemStorage(0);

	CvSeq *first=NULL;
	CvSeq *CONTOrni=NULL;

	cvNamedWindow("Video",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("bin",CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("marker",CV_WINDOW_AUTOSIZE);
    CvPoint *pt[4];
    CvPoint2D32f src[4],dst[4];
    int i;

    CvMat *matrice = cvCreateMat(3,3,CV_32FC1);

	while(1)
	{
		frame = cvQueryFrame(capt);
		cvFlip(frame,frame,1);
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
            cvSetZero(marker);
            cvZero(sub);
            for(first=CONTOrni; first!=NULL;first=first->h_next)
		    {

                if(first->v_next && first->total == 4 && first->v_next->total == 4 && cvCheckContourConvexity(CONTOrni) == 0)
                {
                    cvDrawContours(frame,first,CV_RGB(255,0,0),CV_RGB(0,0,0),0,3,8,cvPoint(0,0));
                    for(i=0; i < 4; i++)
                    {
                        CvPoint *p = CV_GET_SEQ_ELEM(CvPoint,first,i);
                        cvCircle(frame,*p,5,CV_RGB(0,255,100),1,8,0);
                        src[i].x = pic->width;
                        src[i].y = pic->height;

                        dst[i].x = (float)p->x;
                        dst[i].y = (float)p->y;
                    }

                }

		    }
        }
        //cvGetPerspectiveTransform (src, dst, matrice);
        //cvWarpPerspective (pic,sub, matrice);

		cvShowImage("Video",frame);
		cvShowImage("bin",binary);
		char lol = cvWaitKey(15);
		if(lol == 'q')
			break;
	}
}

