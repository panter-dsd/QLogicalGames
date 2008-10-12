#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//
class QWidget;
class QTableView;
class QStandardItem;
class QAction;
class QSettings;
class QStandardItemModel;
class QModelIndex;
class QMainWindow;
class QStatusBar;
class QToolBar;
class QMenuBar;
//
#include <QWidget>
#include <QSqlDatabase>
#include <QTime>

#include "qmynewgamedialog.h"
#include "qmyinformationpanel.h"
#include "qmyundo.h"
//
class QNumbersDestroyer : public QWidget
{
Q_OBJECT
private:
	QMainWindow*							qmwParent;
	QTableView*								qtvTable;
	QMyInformationPanel*				qmipInformationPanel;
	QStandardItemModel*				qsimModel;
	QStandardItem*							qsiPreviousItem;
	QStandardItem*							qsiLastItem;

	QToolBar*									qtbGameToolbar;
	QMenuBar*									qmbMainMenu;
	QStatusBar*								qsbStatusBar;
//Actions
	QAction*										actionNewGame;
	QAction*										actionOpenGame;
	QAction*										actionSaveGame;
	QAction*										actionSaveAsGame;
	QAction*										actionUpdateGame;
	QAction*										actionExit;
	QAction*										actionAbout;
	QAction*										actionAboutQt;
	QAction*										actionUndo;
	QAction*										actionGetHelp;
	QAction*										actionBeginAutoGame;
	QAction*										actionStopAutoGame;
//End actions
	QSettings*									qsetAppSettings;
	QString										qsCurrentFile;
	QSqlDatabase								qsdBase;

	QMyUndo*									qmuUndo;
	bool										bStopAutoGame;
	bool										isGame;
//Information
	QTime										qtGameTime;
	int											iTimeID;
	int											iMSecOffset;
	int											iKillItemsCount;
	int											iHelpCount;
	int											iUndoCount;
	int											iItemsCount;

public:
	QNumbersDestroyer( QMainWindow * parent,QSettings* settings, Qt::WFlags f = 0 );
	~QNumbersDestroyer();
protected:
	void timerEvent(QTimerEvent *event);
private:
	void createControls();
	void createActions();
	void setLayouts();
	void setConnects();
	void createGame(int columnsCount, int numbersCount);
	bool isRowEmpty(int iRow);
	void setLastItem();
	void victory();
	void loadSettings();
	void saveSettings();
	void createMenus();
	void createToolBars();
	bool openGame(const QString& qsFileName);
	bool saveGame(const QString& qsFileName);
	bool okToContinue();
	QList<QStandardItem*> itemsForDelete(const QStandardItem& item);
	void deleteItems(QStandardItem* current, QStandardItem* previous);
	void nullInformation();
	void blinkItem(QStandardItem* current, const int iWaitTime, const int iBlinkCount);
private slots:
	void checkndel(QStandardItem* current, QStandardItem* previous);
	void slotCellClicked (const QModelIndex& qmiIndex);
	void slotUpdateGame();
	void slotNewGame();
	void slotAbout();
	void slotOpenGame();
	bool slotSaveGame();
	bool slotSaveAsGame();
	void slotUpdateInformation();
	void slotUpdateActions();
	void slotUndo();
	void slotGetHelp();
	void slotAutoGame();
	void slotStopAutoGame() {bStopAutoGame=true;};
signals:
	void signalGameTime(const int msec);
	void signalGameParams(const int columnCount,const int rowCount,const int itemsCount);
	void signalGameSchetHodov(const int killItemsCount,const int helpCount,const int undoCount);
};
#endif

