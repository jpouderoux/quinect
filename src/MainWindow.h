#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>
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
#include "FrameWindow.h"

class QAbstractVideoSurface;
class VideoWidget;
class MyKinectListener;

class MainWindow: public QMainWindow, public Ui_MainWindow
{
	Q_OBJECT
public:
	MainWindow( QWidget* parent = NULL, Qt::WFlags flags = 0 );
	virtual ~MainWindow();

	void frame();
	
public slots:
	void help();
	void about();
	void start();
	void tick();

private:
	void presentImage(QAbstractVideoSurface* surface, const QImage &);
	
protected:
	void closeEvent(QCloseEvent* c);

public:
	bool _started;

	 MyKinectListener* _myKinectListener;

	QLabel* _statusAccelerometers;
	QTimer _timer;

	std::vector<FrameWindow*> _frames;
	std::vector<QMdiSubWindow*> _windows;
};

#endif
