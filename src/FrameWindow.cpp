#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QTreeView>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTemporaryFile>
#include <QProgressBar>
#include <QObject>
#include <QPlastiqueStyle>
#include <QDialog>
#include <QComboBox>
#include <QToolBar>
#include <QMdiArea>
#include <QMovie>

#include "FrameWindow.h"
#include "Kinect-win32.h"
#include "Kinect-Utility.h"
#include "videowidget.h"
#include "videowidgetsurface.h"
#include <QVideoSurfaceFormat>
#include <opencv/cv.h>
#include "MyKinectListener.h"


FrameWindow::FrameWindow( const QString &title, MyKinectListener* listener ) : VideoWidget()
{
	_listener = listener;

	this->setWindowTitle( title );

	setBackgroundRole(QPalette::Base);
	setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	_surface = videoSurface();

	_frame = 0;
	
	_cvImage = cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 3 );
	_image = new QImage( (unsigned char*)_cvImage->imageData, 640, 480, 640*3, QImage::Format_RGB888 );
		
}

FrameWindow::~FrameWindow()
{
}

void FrameWindow::frame()
{
	presentImage();
}

void FrameWindow::presentImage()
{
    QVideoFrame frame( *_image );

    if (!frame.isValid()) return;

    QVideoSurfaceFormat currentFormat = _surface->surfaceFormat();

    if (frame.pixelFormat() != currentFormat.pixelFormat() || frame.size() != currentFormat.frameSize()) 
	{
        QVideoSurfaceFormat format(frame.size(), frame.pixelFormat());
        if (!_surface->start(format)) return;
    }

    if (!_surface->present(frame)) _surface->stop();
}

///////////////////////////////////////////////////////////////////////////////

ColorFrameWindow::ColorFrameWindow( const QString &title, MyKinectListener* listener ) : FrameWindow( title, listener )
{
}

ColorFrameWindow::~ColorFrameWindow()
{
}

void ColorFrameWindow::frame()
{
	if ( _listener->getColorImage( _cvImage, _frame ) ) FrameWindow::frame();
}

///////////////////////////////////////////////////////////////////////////////

DepthFrameWindow::DepthFrameWindow( const QString &title, MyKinectListener* listener ) : FrameWindow( title, listener )
{
}

DepthFrameWindow::~DepthFrameWindow()
{
}

void DepthFrameWindow::frame()
{
	if ( _listener->getDepthImage( _cvImage, _frame ) )	FrameWindow::frame();
}

///////////////////////////////////////////////////////////////////////////////
