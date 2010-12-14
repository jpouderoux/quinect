#ifndef FRAMEWINDOW_H_
#define FRAMEWINDOW_H_

#include <QStandardItemModel>
#include <QStandardItem>
#include <QSortFilterProxyModel>
#include <QTreeView>
#include <QMenu>
#include <QModelIndex>
#include <QMessageBox>
#include <QLineEdit>
#include <QLabel>
#include <QTimer>
#include <QMdiSubWindow>
#include <QMovie>
#include <QMutex>
#include "ui_MainWindow.h"
#include "globals.h"
#include "videowidget.h"
#include "videowidgetsurface.h"
#include <opencv/cv.h>

class MyKinectListener;

class FrameWindow: public VideoWidget
{
	Q_OBJECT
public:
	FrameWindow( const QString &title, MyKinectListener* );
	virtual ~FrameWindow();

public:
	virtual void frame();

private:
	void presentImage();

public:
	MyKinectListener* _listener;

    QAbstractVideoSurface *_surface;
	
	QImage *_image;
	IplImage *_cvImage;

	unsigned int _frame;
};

class ColorFrameWindow: public FrameWindow
{
	Q_OBJECT
public:
	ColorFrameWindow( const QString &title, MyKinectListener* );
	virtual ~ColorFrameWindow();

public:
	virtual void frame();

public:
};

class DepthFrameWindow: public FrameWindow
{
	Q_OBJECT
public:
	DepthFrameWindow( const QString &title, MyKinectListener* );
	virtual ~DepthFrameWindow();

public:
	virtual void frame();

public:
};

#endif
