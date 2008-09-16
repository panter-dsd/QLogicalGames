#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include "mainwindow.h"
//
#define ApplicationVersion  "0.0.1.1"
//
int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	app.setApplicationName(QApplication::tr("QNumbersDestroyer"));
	app.setApplicationVersion(ApplicationVersion);

	QTranslator translator;
	translator.load(app.applicationDirPath()+"/../share/languages/qnumbersdestroyer_"+QLocale::system().name()+".qm");
	qDebug((app.applicationDirPath()+"./../share/languages/qnumbersdestroyer_"+QLocale::system().name()+".qm").toLocal8Bit());
	app.installTranslator(&translator);

	QTranslator translatorQt;
	translatorQt.load(app.applicationDirPath()+"/../share/languages/qt_"+QLocale::system().name()+".qm");
	app.installTranslator(&translatorQt);
	MainWindow win;
	win.setWindowTitle(app.applicationName()+" "+app.applicationVersion());
	win.show();
	app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	return app.exec();
}
