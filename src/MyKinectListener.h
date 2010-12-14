#pragma once

#include "Kinect-win32.h"
#include "Kinect-Utility.h"

#include <conio.h>
#include <windows.h>
#include <math.h>
#include "globals.h"
#include <opencv/cv.h>

#include <QMutex>

class MainWindow;

class MyKinectListener : public QObject, public Kinect::KinectListener
{
	Q_OBJECT
public:
	MyKinectListener(MainWindow* window);

	virtual ~MyKinectListener();

	virtual void KinectDisconnected(Kinect::Kinect *K) {}

	virtual void DepthReceived(Kinect::Kinect*);

	virtual void ColorReceived(Kinect::Kinect*);

	virtual void AudioReceived(Kinect::Kinect* K) {}

	void lock() { _mutex.lock(); }

	void unlock() { _mutex.unlock(); }

	bool getColorImage( IplImage*, unsigned int &frame );

	bool getDepthImage( IplImage*, unsigned int &frame );
	
private:
	void ParseDepth( Kinect::Kinect* );		

public:
	MainWindow* _window;

	unsigned char *_DepthColor;
	unsigned short _GammaMap[2048];

	IplImage *_colorImg;
	IplImage *_depthImg;

	unsigned int _colorImgFrame;
	unsigned int _depthImgFrame;

	QMutex _mutex;
};
