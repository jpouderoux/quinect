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

#include "MainWindow.h"
#include "Kinect-win32.h"
#include "Kinect-Utility.h"
#include "MyKinectListener.h"
#include "videowidget.h"
#include "videowidgetsurface.h"
#include <QVideoSurfaceFormat>

static Kinect::KinectFinder* KF;
static Kinect::Kinect *K = 0;

MainWindow::MainWindow( QWidget* parent, Qt::WFlags flags ) : QMainWindow( parent, flags )
{
	setStyle( new QPlastiqueStyle() );

	setupUi(this);
	
	QMdiArea *zoneCentrale = new QMdiArea;
	setCentralWidget(zoneCentrale);

	this->setWindowTitle("Quinect");

	_statusAccelerometers = new QLabel("");
	_statusAccelerometers->setMinimumWidth(200);
	statusBar()->addPermanentWidget(_statusAccelerometers);

	_myKinectListener = new MyKinectListener( this );
	
	{
		FrameWindow* f = new ColorFrameWindow( tr( "Color camera image" ), _myKinectListener );
		QMdiSubWindow* w = zoneCentrale->addSubWindow( f );
		_frames.push_back( f );
		_windows.push_back( w );
	}	
	{
		FrameWindow* f = new DepthFrameWindow( tr( "Depth image" ), _myKinectListener );
		QMdiSubWindow* w = zoneCentrale->addSubWindow( f );
		_frames.push_back( f );
		_windows.push_back( w );
	}


	connect( actionExit, SIGNAL( triggered() ), this, SLOT( close() ) );
	connect( actionStart, SIGNAL( triggered() ), this, SLOT( start() ) );

	connect( actionHelp, SIGNAL( triggered() ), this, SLOT( help() ) );
	connect( actionAbout, SIGNAL( triggered() ), this, SLOT( about() ) );

	connect( &_timer, SIGNAL( timeout() ), this, SLOT( tick() ) );

	statusBar()->showMessage( tr( "Ready." ) );

	_started = false;

	if ( !KF ) KF = new Kinect::KinectFinder();

	if (KF->GetKinectCount() < 1)
		statusBar()->showMessage( "Unable to find Kinect devices... Is one connected?" );
	else
	{
		K = KF->GetKinect();
		if (K == 0) 
			statusBar()->showMessage( "error getting Kinect..." );
		else
		{
			K->Stop();
			K->AddListener(_myKinectListener);
		}
	}
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent* c)
{
	if ( KF ) start();
	delete KF;
}

void MainWindow::help()
{
	QMessageBox::about( this, tr( "Help" ), tr( "Sorry, the help is not available yet." ) );
}

void MainWindow::about()
{
	QMessageBox::about( this, tr( "About Quinect" ), tr( "Quinect\n Version 1.0.3\n(c) 2010, Joachim Pouderoux" ) );
}

void MainWindow::start()
{
	if ( !K ) return;

	if ( !_started )
	{			
		_timer.start( 25 );
		statusBar()->showMessage( tr( "Kinect started." ) );

		QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/pause"), QSize(), QIcon::Normal, QIcon::Off);
        actionStart->setIcon(icon);

		K->Run();
		K->SetMotorPosition( 1.0 );
		K->SetLedMode( Kinect::Led_Green );
		float x,y,z;
		for (int i = 0 ;i<10;i++)
		{
			if (K->GetAcceleroData(&x,&y,&z))
			{
				QString str;
				str.sprintf( "Accelerometers: %.2lf %.2lf %.2lf", x, y, z );
				_statusAccelerometers->setText( str );
			}
			Sleep(5);
		}		

		_started = true;
	}
	else 
	{
		_timer.stop();

		K->SetLedMode( Kinect::Led_Off );
		K->Stop();		
		
		_statusAccelerometers->setText( "" );
		statusBar()->showMessage( tr( "Kinect stopped." ) );

		QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/play"), QSize(), QIcon::Normal, QIcon::Off);
        actionStart->setIcon(icon);

		_started = false;	
	}
}

void MainWindow::tick()
{
	static int accel = 0;
	accel++;
	float x,y,z;
	if ( accel == 10 )
	if ( K->GetAcceleroData( &x, &y, &z) )
	{
		accel = 0;
		QString str;
		str.sprintf( "Accelerometers: %.2lf %.2lf %.2lf", x, y, z );
		_statusAccelerometers->setText( str );
	}

	for ( unsigned int i = 0; i < _frames.size(); i++ )
		_frames[i]->frame();
}

