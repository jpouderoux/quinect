#include "MyKinectListener.h"
#include "MainWindow.h"
#include <opencv/cv.h>


MyKinectListener::MyKinectListener(MainWindow* window) 
{
	_window = window;
	_DepthColor = new unsigned char[640*480*3];
	for ( int i = 0; i < 2048; i++ )
		_GammaMap[i] = (unsigned short)(float)(powf(i/2048.0f,3)*6*6*256);	

	_colorImg = cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 3 );
	_depthImg = cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 3 );

	_colorImgFrame = 0;
	_depthImgFrame = 0;
}

MyKinectListener::~MyKinectListener()
{
	delete [] _DepthColor;
	cvReleaseImage( &_colorImg );
	cvReleaseImage( &_depthImg );
}

void MyKinectListener::ParseDepth(Kinect::Kinect *K)
{
	int i = 0;
	for ( int y = 0; y < 480; y++ )
	{
		unsigned char *destrow = (unsigned char*)_depthImg->imageData + y*640*3;// + ((479-y)*(640))*3;
		//float *destdepth = mDepthBuffer + ((479-y)*(640));
		//float *maxdestdepth = mMaxDepthBuffer + ((479-y)*(640));
		//unsigned short *bitdepth = mBitDepthBuffer + ((479-y)*(640));
		for ( int x = 0; x < 640; x++ )
		{
			unsigned short Depth = K->mDepthBuffer[i];
			if (Kinect::Kinect_IsDepthValid(Depth))
			{}
			/**bitdepth ++ = Depth;
			float D = Kinect::Kinect_DepthValueToZ(Depth);
			*destdepth++ = D;
			if (D> *maxdestdepth )
			{
			*maxdestdepth = D;
			}					
			maxdestdepth++;
			}
			else
			{
			*bitdepth++ = -1;
			*destdepth++  = -100000;
			maxdestdepth++ ;
			}*/
			//destrow[3] =  0;
			int pval = _GammaMap[Depth];
			int lb = pval & 0xff;
			switch (pval>>8) 
			{
			case 0:
				destrow[2] = 255;
				destrow[1] = 255-lb;
				destrow[0] = 255-lb;
				break;
			case 1:
				destrow[2] = 255;
				destrow[1] = lb;
				destrow[0] = 0;
				break;
			case 2:
				destrow[2] = 255-lb;
				destrow[1] = 255;
				destrow[0] = 0;
				break;
			case 3:
				destrow[2] = 0;
				destrow[1] = 255;
				destrow[0] = lb;
				break;
			case 4:
				destrow[2] = 0;
				destrow[1] = 255-lb;
				destrow[0] = 255;
				break;
			case 5:
				destrow[2] = 0;
				destrow[1] = 0;
				destrow[0] = 255-lb;
				break;
			default:
				destrow[2] = 0;
				destrow[1] = 0;
				destrow[0] = 0;
				break;
			}
			destrow+=3;
			i++;
		};
	};
}

void MyKinectListener::ColorReceived(Kinect::Kinect *K) 
{
	lock();
	
	K->ParseColorBuffer();
	
	cvSetImageData( _colorImg, K->mColorBuffer, 640*3 );

	//memcpy( _colorImg->imageData, K->mColorBuffer, 640*480*3 );

	//cvCvtColor( _colorImg, _colorImg, CV_RGB2BGR );

	_colorImgFrame++;
	
	unlock();
}

void MyKinectListener::DepthReceived(Kinect::Kinect *K) 
{
	lock();

	K->ParseDepthBuffer();
		
	ParseDepth( K );

	_depthImgFrame++;

	unlock();
}	

bool MyKinectListener::getColorImage( IplImage* dest, unsigned int &frame )
{
	if ( _colorImgFrame == frame ) return false;

	lock();

	cvCopyImage( _colorImg, dest );
	
	frame = _colorImgFrame;

	unlock();

	return true;
}

bool MyKinectListener::getDepthImage( IplImage* dest, unsigned int &frame )
{
	if ( _depthImgFrame == frame ) return false;

	lock();

	cvCopyImage( _depthImg, dest );
	
	frame = _depthImgFrame;

	unlock();

	return true;
}
