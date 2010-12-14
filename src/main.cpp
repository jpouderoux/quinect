#include <QApplication>
#include <QSplashScreen>
#include <QTranslator>
#include <QMainWindow>
#include <QLocale>
#include <QBitmap>
#include <QObject>
#include "MainWindow.h"

int main( int argc, char *argv[] )
{
	// load qt traduction files
	QTranslator qtTranslator;
	qtTranslator.load( "qt_" + QLocale::system().name() );

	// load our application traduction file
	QTranslator myappTranslator;
	myappTranslator.load( "Quinect_" + QLocale::system().name() );
    
	// create application and set translations
	QApplication app( argc, argv ); 
	
	Q_INIT_RESOURCE(resources);

	QPixmap splashImage( ":/images/splash" );
	QSplashScreen splash( splashImage );
	splash.setMask( QRegion( splashImage.mask() ) ); // transparency
    splash.show();
#if 0
	QPalette p (app.palette());
    p.setColor (QPalette::Background, QColor (48, 48, 48/*39, 39, 39*/)); // general background
    p.setColor (QPalette::WindowText, QColor (203, 203, 203)); // general foreground
    p.setColor (QPalette::Base, QColor (60, 60, 60)); // text entry widget
    p.setColor (QPalette::AlternateBase, QColor (238, 112, 0)); // Text used on 'Base'
    p.setColor (QPalette::Text, QColor (145, 145, 145)); // Text used on 'Base'
    p.setColor (QPalette::Button, QColor (48, 48, 48)); // general bg button color
    p.setColor (QPalette::ButtonText, QColor (203, 203, 203)); // general foreground button color
    p.setColor (QPalette::BrightText, QColor (238, 112, 0)); // to ensure contrast
    p.setColor (QPalette::Highlight, QColor (238, 112, 0)); // marked element
    p.setColor (QPalette::HighlightedText, QColor (31, 13, 0)); // marked element text
	app.setPalette(p);
#endif

	/*QFile file( "data/BRGMViewer.qss" );
	file.open( QFile::ReadOnly );
	QString styleSheet = QLatin1String( file.readAll() );
	app.setStyleSheet( styleSheet );*/

	//app.setWindowIcon( QIcon( QPixmap( ":/icons/brgm-128" ) ) );
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	app.installTranslator( &qtTranslator );
	app.installTranslator( &myappTranslator );

	// create and launch window
	MainWindow window;

	window.show();

	splash.finish(&window);

	// let the show begin...
	return app.exec();
}

#ifdef WIN32
#include <windows.h> 
#include <stdio.h>
#include <shellapi.h>

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	int argc;
	char **argv;

	LPWSTR *szArglist = CommandLineToArgvW( GetCommandLineW(), &argc );
	argv = ( char** )malloc( sizeof( char* )*argc );
	for ( int i = 0;i<argc;i++ )
	{
		argv[i] = ( char* )malloc( wcslen( szArglist[i] )+1 ); //+1 for \0
		sprintf( argv[i], "%ws", szArglist[i] );
	}
	LocalFree( szArglist );

	return main ( argc, argv ); 
}
#endif
