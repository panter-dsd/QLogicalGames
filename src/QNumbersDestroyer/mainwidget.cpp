#include <QtGui>
#include <QtSql>
#include "mainwidget.h"
//
QNumbersDestroyer::QNumbersDestroyer( QMainWindow* parent,QSettings* settings, Qt::WFlags f)
	: QWidget(parent, f)
{
	qmwParent=parent;
	qsetAppSettings=settings;
	qsdBase = QSqlDatabase::addDatabase("QSQLITE");
	qsCurrentFile="";
	createControls();
	loadSettings();
	setLayouts();
	createActions();
	setConnects();
	createMenus();
	createToolBars();
	qsiPreviousItem=0;
	qmuUndo=new QMyUndo();
	bStopAutoGame=true;
	isGame=false;
	iTimeID=0;
	nullInformation();
	slotUpdateActions();
}
//
QNumbersDestroyer::~QNumbersDestroyer()
{
	saveSettings();
}
//
void QNumbersDestroyer::createControls()
{
	qtvTable=new QTableView(this);
	qtvTable->setCornerButtonEnabled(false);
	qtvTable->setSelectionMode(QAbstractItemView::SingleSelection);
	qtvTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
//
	qmipInformationPanel=new QMyInformationPanel(this);

	qsimModel=new QStandardItemModel(this);
	qtvTable->setModel(qsimModel);
//	qtvTable->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
//	qtvTable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	qsbStatusBar=qmwParent->statusBar();
}
//
void QNumbersDestroyer::createActions()
{
	actionNewGame=new QAction(tr("New game"),this);
	actionNewGame->setShortcut(QKeySequence::New);
	actionNewGame->setStatusTip(tr("Create a new game"));
	actionNewGame->setIcon(QIcon(":/share/images/actionNewGame.png"));
	this->addAction(actionNewGame);
//
	actionOpenGame=new QAction(tr("Open game"),this);
	actionOpenGame->setShortcut(QKeySequence::Open);
	actionOpenGame->setStatusTip(tr("Open saved game"));
	actionOpenGame->setIcon(QIcon(":/share/images/actionOpenGame.png"));
	this->addAction(actionOpenGame);
//
	actionSaveGame=new QAction(tr("Save game"),this);
	actionSaveGame->setShortcut(QKeySequence::Open);
	actionSaveGame->setStatusTip(tr("Save game in the file"));
	actionSaveGame->setIcon(QIcon(":/share/images/actionSaveGame.png"));
	this->addAction(actionSaveGame);
//
	actionSaveAsGame=new QAction(tr("Save game as..."),this);
	actionSaveAsGame->setShortcut(QKeySequence::Open);
	actionSaveAsGame->setStatusTip(tr("Save game as..."));
	actionSaveAsGame->setIcon(QIcon(":/share/images/actionSaveAsGame.png"));
	this->addAction(actionSaveAsGame);
//
	actionUpdateGame=new QAction(tr("More numbers"),this);
	actionUpdateGame->setShortcut(Qt::Key_F3);
	actionUpdateGame->setStatusTip(tr("Adding more numbers"));
	actionUpdateGame->setIcon(QIcon(":/share/images/actionUpdateGame.png"));
	this->addAction(actionUpdateGame);
	actionExit=new QAction(tr("Exit"),this);
	actionExit->setShortcut(Qt::ALT + Qt::Key_X);
	actionExit->setStatusTip(tr("Exit from game"));
	actionExit->setIcon(QIcon(":/share/images/actionExit.png"));
	this->addAction(actionExit);
	actionAbout=new QAction(tr("About..."),this);
	actionAbout->setStatusTip(tr("About this application"));
	actionAbout->setIcon(QIcon(":/share/images/actionAbout.png"));
	this->addAction(actionAbout);
	actionAboutQt=new QAction(tr("About Qt..."),this);
	actionAboutQt->setStatusTip(tr("About Qt"));
	actionAboutQt->setIcon(QIcon(":/share/images/actionAboutQt.png"));
	this->addAction(actionAboutQt);

	actionUndo=new QAction(tr("Undo"),this);
	actionUndo->setShortcut(QKeySequence::Undo);
	actionUndo->setStatusTip(tr("Undo changes"));
	actionUndo->setIcon(QIcon(":/share/images/actionUndo.png"));
	this->addAction(actionUndo);

	actionGetHelp=new QAction(tr("Get a tip"),this);
	actionGetHelp->setStatusTip(tr("Get a tip"));
	actionGetHelp->setIcon(QIcon(":/share/images/actionGetHelp.png"));
	this->addAction(actionGetHelp);

	actionBeginAutoGame=new QAction(tr("Autogame"),this);
	actionBeginAutoGame->setStatusTip(tr("Begin autogame"));
	actionBeginAutoGame->setIcon(QIcon(":/share/images/actionBeginAutoGame.png"));
	this->addAction(actionBeginAutoGame);

	actionStopAutoGame=new QAction(tr("Stop autogame"),this);
	actionStopAutoGame->setStatusTip(tr("Stop autogame"));
	actionStopAutoGame->setIcon(QIcon(":/share/images/actionStopAutoGame.png"));
	this->addAction(actionStopAutoGame);
}
//
void QNumbersDestroyer::setLayouts()
{
	QHBoxLayout* qhblMainLayout=new QHBoxLayout();
	qhblMainLayout->addWidget(qtvTable);
	qhblMainLayout->addWidget(qmipInformationPanel);

	this->setLayout(qhblMainLayout);
}
//
void QNumbersDestroyer::setConnects()
{
	connect(qtvTable,SIGNAL(clicked(const QModelIndex&)),this,SLOT(slotCellClicked(const QModelIndex&)));

	connect(actionUpdateGame,SIGNAL(triggered()),this,SLOT(slotUpdateGame()));
	connect(actionNewGame,SIGNAL(triggered()),this,SLOT(slotNewGame()));
	connect(actionAboutQt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
	connect(actionExit,SIGNAL(triggered()),this,SLOT(close()));
	connect(actionAbout,SIGNAL(triggered()),this,SLOT(slotAbout()));
	connect(actionOpenGame,SIGNAL(triggered()),this,SLOT(slotOpenGame()));
	connect(actionSaveGame,SIGNAL(triggered()),this,SLOT(slotSaveGame()));
	connect(actionSaveAsGame,SIGNAL(triggered()),this,SLOT(slotSaveAsGame()));
	connect(actionUndo,SIGNAL(triggered()),this,SLOT(slotUndo()));
	connect(actionGetHelp,SIGNAL(triggered()),this,SLOT(slotGetHelp()));
	connect(actionBeginAutoGame,SIGNAL(triggered()),this,SLOT(slotAutoGame()));
	connect(actionStopAutoGame,SIGNAL(triggered()),this,SLOT(slotStopAutoGame()));
//Information
	connect(this,
				SIGNAL(signalGameTime(const int)),
				qmipInformationPanel,
				SLOT(slotSetTime(const int)));
	connect(this,
				SIGNAL(signalGameParams(const int,const int,const int)),
				qmipInformationPanel,
				SLOT(slotSetParams(const int,const int,const int)));
	connect(this,
				SIGNAL(signalGameSchetHodov(const int,const int,const int)),
				qmipInformationPanel,
				SLOT(slotSetSchetHodov(const int,const int,const int)));
}
//
void QNumbersDestroyer::loadSettings()
{
}
//
void QNumbersDestroyer::saveSettings()
{
}
//
void QNumbersDestroyer::createGame(int columnsCount, int numbersCount) //Создание игры
{
	qsimModel->clear();
	nullInformation();
	qsimModel->setColumnCount(columnsCount);
	qsimModel->setRowCount(1);
	int row=0;
	int column=0;
	QString qsTmp;
	int i=1;
	do
	{
		if (column==qsimModel->columnCount())
		{
			row++;
			qsimModel->setRowCount(row+1);
			column=0;
		}
		if (qsTmp.isEmpty())
			qsTmp.setNum(i);
		iItemsCount++;
		QStandardItem* item=new QStandardItem(qsTmp.at(0));
		item->setTextAlignment(Qt::AlignCenter);
		item->setBackground(QBrush(Qt::cyan));
		qsimModel->setItem(row,column++,item);
		qsTmp.remove(0,1);
		if (qsTmp.isEmpty())
			i++;
	}
	while(i<=numbersCount);
	qtvTable->setCurrentIndex (QModelIndex());
	qtvTable->resizeRowsToContents();
	qtvTable->resizeColumnsToContents();
	setLastItem();
	qtGameTime.start();
	iTimeID=this->startTimer(1000);
	isGame=true;
	slotUpdateInformation();
	slotUpdateActions();
}
//
void QNumbersDestroyer::checkndel(QStandardItem* current, QStandardItem* previous)
{
	qsiPreviousItem=0;
	qtvTable->clearFocus();
	qtvTable->clearSelection();
	qtvTable->setCurrentIndex(QModelIndex());
	QList<QStandardItem*> itemList=itemsForDelete(*current);
	bool isDel=false;
	for (int i=0; i<itemList.count(); i++)
	{
		if (itemList.at(i)==previous)
		{
			isDel=true;
			break;
		}
	}
	if (!isDel)
	{
		itemList=itemsForDelete(*previous);
		for (int i=0; i<itemList.count(); i++)
		{
			if (itemList.at(i)==current)
			{
				isDel=true;
				break;
			}
		}
	}
	if (isDel)
	{
		//Undo
		UndoOperation uo;
		uo.uoperation=DELETE_ITEMS;
		uo.iFirstRow=current->row();
		uo.iFirstColumn=current->column();
		uo.iFirstNumber=current->text().toInt();
		uo.iSecondRow=previous->row();
		uo.iSecondColumn=previous->column();
		uo.iSecondNumber=previous->text().toInt();
		qmuUndo->put(uo);
		//End undo
		current->setText("");
		current->setBackground(QBrush(Qt::black));
		previous->setText("");
		previous->setBackground(QBrush(Qt::black));
		//Search for empty rows
		if (current->row()==previous->row())
		{
			if (isRowEmpty(current->row()))
			{
				//Undo
				UndoOperation uo;
				uo.uoperation=DELETE_LINE;
				uo.iFirstRow=current->row();
				qmuUndo->put(uo);
				//End undo
				qsimModel->removeRow(current->row());
			}
		}
		else
		{
			if (isRowEmpty(current->row()))
			{
				//Undo
				UndoOperation uo;
				uo.uoperation=DELETE_LINE;
				uo.iFirstRow=current->row();
				qmuUndo->put(uo);
				//End undo
				qsimModel->removeRow(current->row());
			}
			if (isRowEmpty(previous->row()))
			{
				//Undo
				UndoOperation uo;
				uo.uoperation=DELETE_LINE;
				uo.iFirstRow=previous->row();
				qmuUndo->put(uo);
				//End undo
				qsimModel->removeRow(previous->row());
			}
		}
		//information
		iKillItemsCount+=2;
		iItemsCount-=2;
		if (qsimModel->rowCount()==0)
			victory();
		slotUpdateInformation();
		slotUpdateActions();
	}
}
//
void QNumbersDestroyer::slotCellClicked (const QModelIndex& qmiIndex)
{
	if (qsimModel->itemFromIndex(qmiIndex)->text().isEmpty())
	{
		if (qsiPreviousItem==0)
			qtvTable->setCurrentIndex (QModelIndex());
		else
			qtvTable->setCurrentIndex (qsimModel->indexFromItem(qsiPreviousItem));
		return;
	}
	if (qsiPreviousItem==0)
	{
		qsiPreviousItem=qsimModel->itemFromIndex(qmiIndex);
	}
	else
	{
		checkndel(qsimModel->itemFromIndex(qmiIndex),qsiPreviousItem);
	}
}
//
void QNumbersDestroyer::slotUpdateGame()
{
	if (qsimModel->rowCount()==0)
		return;
	setLastItem();
	int lastRow=qsiLastItem->row();
	int lastColumn=qsiLastItem->column();
	UndoOperation uo;
	uo.uoperation=ADD_ROWS;
	uo.iFirstRow=lastRow;
	uo.iFirstColumn=lastColumn;
	uo.iFirstNumber=iItemsCount;
	qmuUndo->put(uo);
	int rowCount=qsimModel->rowCount();
	int row=qsiLastItem->row();
	int column=qsiLastItem->column();
	qtvTable->setUpdatesEnabled(false);
	QStandardItem* qsiItem;
	for (int i=0; i<rowCount; i++)
	{
		QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
		for (int j=0; j<qsimModel->columnCount(); j++)
		{
			if (((i==lastRow) && (j>lastColumn)) || (i>lastRow))
			{
				qtvTable->resizeRowsToContents();
				qtvTable->resizeColumnsToContents();
				setLastItem();
				qtvTable->setUpdatesEnabled(true);
				slotUpdateInformation();
				slotUpdateActions();
				return;
			}
			if (qsimModel->item(i,j)->text().isEmpty())
				continue;
			iItemsCount++;
			qsiItem=new QStandardItem(qsimModel->item(i,j)->text());
			qsiItem->setTextAlignment(Qt::AlignCenter);
			qsiItem->setBackground(QBrush(Qt::cyan));
			column++;
			if (column==qsimModel->columnCount())
			{
				column=0;
				qsimModel->setRowCount(qsimModel->rowCount()+1);
				row++;
			}
			qsimModel->setItem(row,column,qsiItem);
		}
	}
	qtvTable->resizeRowsToContents();
	qtvTable->resizeColumnsToContents();
	qtvTable->setUpdatesEnabled(true);
	setLastItem();
	slotUpdateInformation();
	slotUpdateActions();
}
//
bool QNumbersDestroyer::isRowEmpty(int iRow)
{
	for (int i=0; i<qsimModel->columnCount(); i++)
	{
		if (qsimModel->item(iRow,i) && (!qsimModel->item(iRow,i)->text().isEmpty()))
			return false;
	}
	return true;
}
//
void QNumbersDestroyer::setLastItem()
{
	for (int i=qsimModel->rowCount(); i>=0; i--)
	{
		for (int j=qsimModel->columnCount(); j>=0; j--)
		{
			if (qsimModel->item(i,j))
			{
				qsiLastItem=qsimModel->item(i,j);
				return;
			}
		}
	}
}
//
void QNumbersDestroyer::victory()
{
	qsimModel->clear();
	nullInformation();
	QMessageBox* message=new QMessageBox(QMessageBox::NoIcon,
				QApplication::applicationName(),
				tr("Your are win!!!"),
				QMessageBox::Ok,
				this);
	message->exec();
	delete message;
}
//
void QNumbersDestroyer::slotNewGame()
{
	QMyNewGameDialog* qdNewGameDialog=new QMyNewGameDialog(this);
	qdNewGameDialog->setColumnsCount(qsetAppSettings->value("Game/columnCount",9).toInt());
	qdNewGameDialog->setNumbersCount(qsetAppSettings->value("Game/numbersCount",27).toInt());
	if (qdNewGameDialog->exec())
	{
		createGame(qdNewGameDialog->getColumnsCount(),qdNewGameDialog->getNumbersCount());
		qsetAppSettings->setValue("Game/columnCount",
					qdNewGameDialog->getColumnsCount());
		qsetAppSettings->setValue("Game/numbersCount",
					qdNewGameDialog->getNumbersCount());
	}
	delete qdNewGameDialog;
}
//
void QNumbersDestroyer::createMenus()
{
	qmbMainMenu=qmwParent->menuBar();

	QMenu* qmGame=qmbMainMenu->addMenu(tr("Game"));
	qmGame->addAction(actionNewGame);
	qmGame->addAction(actionOpenGame);
	qmGame->addAction(actionSaveGame);
	qmGame->addAction(actionSaveAsGame);
	qmGame->addSeparator();
	qmGame->addAction(actionUpdateGame);
	qmGame->addAction(actionUndo);
	qmGame->addSeparator();
	qmGame->addAction(actionGetHelp);
	qmGame->addAction(actionBeginAutoGame);
	qmGame->addAction(actionStopAutoGame);
	qmGame->addSeparator();
	qmGame->addAction(actionExit);

	QMenu* qmHelp=qmbMainMenu->addMenu(tr("Help"));
	qmHelp->addAction(actionAbout);
	qmHelp->addAction(actionAboutQt);
}
//
void QNumbersDestroyer::createToolBars()
{
	qtbGameToolbar=new QToolBar(tr("Game"));
	qtbGameToolbar->setObjectName("Game");
	qtbGameToolbar->addAction(actionNewGame);
	qtbGameToolbar->addAction(actionOpenGame);
	qtbGameToolbar->addAction(actionSaveGame);
	qtbGameToolbar->addAction(actionSaveAsGame);
	qtbGameToolbar->addSeparator();
	qtbGameToolbar->addAction(actionUpdateGame);
	qtbGameToolbar->addAction(actionUndo);
	qtbGameToolbar->addSeparator();
	qtbGameToolbar->addAction(actionGetHelp);
	qtbGameToolbar->addAction(actionBeginAutoGame);
	qtbGameToolbar->addAction(actionStopAutoGame);
	qtbGameToolbar->addSeparator();
	qtbGameToolbar->addAction(actionExit);
	qmwParent->addToolBar(qtbGameToolbar);
}
//
void QNumbersDestroyer::slotAbout()
{
	QMessageBox::about(this,tr("About application..."),
						tr("<h2>") + QApplication::applicationName()+
						" "+QApplication::applicationVersion() +tr ("</h2>")+
						tr("<p>Author: PanteR"
							"<p>e-mail: panter.dsd@gmail.com"
							"<p>phone: 89094119675"
							"<p>License: GNU GPL v2"));
}
//
//
void QNumbersDestroyer::slotOpenGame()
{
	if (okToContinue())
	{
		QString qsFileName;
		qsFileName = QFileDialog::getOpenFileName(this,
										tr("Open file"),
										qsetAppSettings->value("PATH/OpenPath",QString()).toString(),
										tr("Game files (*.qnd)"));
		if (!qsFileName.isEmpty())
		{
			qsetAppSettings->setValue("PATH/OpenPath",QFileInfo(qsFileName).absolutePath());
			openGame(qsFileName);
		}
	}
}
//
bool QNumbersDestroyer::openGame(const QString& qsFileName)
{
	nullInformation();
	qsdBase.setDatabaseName(qsFileName);
	if (!qsdBase.open())
	{
		QMessageBox::critical(0, tr("Error opening game"),qsdBase.lastError().text());
		return false;
	}
	qsimModel->clear();
	QCoreApplication::processEvents();
	QSqlQuery* qsqQuery=new QSqlQuery("");
	qsqQuery->prepare("SELECT DATA FROM params WHERE NAME=:NAME");
//
	qsqQuery->bindValue("NAME","ROW_COUNT");
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error opening game"), qsqQuery->lastError().text());
		return false;
	}
	qsqQuery->next();
	qsimModel->setRowCount(qsqQuery->value(0).toInt());
//
	qsqQuery->bindValue("NAME","COLUMN_COUNT");
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error opening game"), qsqQuery->lastError().text());
		return false;
	}
	qsqQuery->next();
	qsimModel->setColumnCount(qsqQuery->value(0).toInt());
//
	qsqQuery->bindValue("NAME","LAST_ROW");
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error opening game"), qsqQuery->lastError().text());
		return false;
	}
	qsqQuery->next();
	int lastRow=qsqQuery->value(0).toInt();
//
	qsqQuery->bindValue("NAME","LAST_COLUMN");
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error opening game"), qsqQuery->lastError().text());
		return false;
	}
	qsqQuery->next();
	int lastColumn=qsqQuery->value(0).toInt();
//Information
	qsqQuery->bindValue("NAME","MSEC_OFFSET");
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error opening game"), qsqQuery->lastError().text());
		return false;
	}
	if (qsqQuery->next())
		iMSecOffset=qsqQuery->value(0).toInt();
	else
		iMSecOffset=0;
//
	qsqQuery->bindValue("NAME","KILL_ITEMS_COUNT");
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error opening game"), qsqQuery->lastError().text());
		return false;
	}
	if (qsqQuery->next())
		iKillItemsCount=qsqQuery->value(0).toInt();
	else
		iKillItemsCount=0;
//
	qsqQuery->bindValue("NAME","HELP_COUNT");
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error opening game"), qsqQuery->lastError().text());
		return false;
	}
	if (qsqQuery->next())
		iHelpCount=qsqQuery->value(0).toInt();
	else
		iHelpCount=0;
//
	qsqQuery->bindValue("NAME","UNDO_COUNT");
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error opening game"), qsqQuery->lastError().text());
		return false;
	}
	if (qsqQuery->next())
		iUndoCount=qsqQuery->value(0).toInt();
	else
		iUndoCount=0;
//
	qsqQuery->prepare("SELECT count() FROM game");
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error opening game"), qsqQuery->lastError().text());
		return false;
	}
	qsqQuery->next();
	iItemsCount=qsqQuery->value(0).toInt();
	qsqQuery->prepare("SELECT * FROM game");
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error opening game"), qsqQuery->lastError().text());
		return false;
	}
	QStandardItem* item;
	QProgressDialog* qpdProgress=new QProgressDialog(tr("Loading..."),
			tr("Break"),
			0,
			iItemsCount,
			this);
	qpdProgress->setModal(true);
	qtvTable->setUpdatesEnabled(false);

	while (qsqQuery->next())
	{
		qpdProgress->setValue(qpdProgress->value()+1);
		QCoreApplication::processEvents();
		if (qpdProgress->wasCanceled())
		{
			qsimModel->clear();
			qtvTable->setUpdatesEnabled(true);
			delete qpdProgress;
			delete qsqQuery;
			qsdBase.close();
			qtvTable->setUpdatesEnabled(true);
			return false;
		}
		item=new QStandardItem(qsqQuery->value(2).toString());
		item->setTextAlignment(Qt::AlignCenter);
		item->setBackground(QBrush(Qt::cyan));
		qsimModel->setItem(qsqQuery->value(0).toInt(),qsqQuery->value(1).toInt(),item);
	}
//
	for (int i=0; i<qsimModel->rowCount(); i++)
		for (int j=0; j<qsimModel->columnCount(); j++)
		{
/*			qpdProgress->setValue(qpdProgress->value()+1);
			QCoreApplication::processEvents();
			if (qpdProgress->wasCanceled())
			{
				qsimModel->clear();
				qtvTable->setUpdatesEnabled(true);
				delete qpdProgress;
				delete qsqQuery;
				qsdBase.close();
				qtvTable->setUpdatesEnabled(true);
				return false;
			}
*/			if (((i==lastRow) && (j>lastColumn)) || (i>lastRow))
				break;
			if (!qsimModel->item(i,j))
			{
				item=new QStandardItem("");
				item->setTextAlignment(Qt::AlignCenter);
				item->setBackground(QBrush(Qt::black));
				qsimModel->setItem(i,j,item);
			}
		}
//
	delete qpdProgress;
	qsqQuery->clear();
	delete qsqQuery;
	qsdBase.close();
	qtvTable->resizeColumnsToContents();
	qtvTable->resizeRowsToContents();
	qtvTable->setUpdatesEnabled(true);
	qsCurrentFile=qsFileName;
	qtGameTime.start();
	iTimeID=this->startTimer(1000);
	isGame=true;
	slotUpdateActions();
	slotUpdateInformation();
	return true;
}
//
bool QNumbersDestroyer::slotSaveGame()
{
	if (qsimModel->rowCount()==0)
		return false;
	if (qsCurrentFile.isEmpty())
	{
		return slotSaveAsGame();
	}
	return saveGame(qsCurrentFile);
}
//
//
bool QNumbersDestroyer::slotSaveAsGame()
{
	if (qsimModel->rowCount()==0)
		return false;
	QString qsFileName;
	qsFileName = QFileDialog::getSaveFileName(this,
							tr("Save file"),
							qsetAppSettings->value("PATH/OpenPath",QString()).toString(),
							tr("Game files (*.qnd)"));
	if (qsFileName.isEmpty())
		return false;
	if (QFileInfo(qsFileName).suffix().isEmpty())
	{
		qsFileName+=".qnd";
	}
	qsetAppSettings->setValue("PATH/OpenPath",QFileInfo(qsFileName).absolutePath());
	return saveGame(qsFileName);
}
//
bool QNumbersDestroyer::saveGame(const QString& qsFileName)
{
	qsdBase.setDatabaseName(qsFileName);
	if (!qsdBase.open())
	{
		QMessageBox::critical(0, tr("Error saving game"),qsdBase.lastError().text());
		return false;
	}
	qsdBase.transaction();
	QSqlQuery* qsqQuery=new QSqlQuery("");
	qsqQuery->prepare("create table params (NAME VARCHAR(30),DATA VARCHAR(50));");
	if (!qsqQuery->exec())
	{
		qsqQuery->prepare("DELETE FROM params");
		if (!qsqQuery->exec())
		{
			QMessageBox::critical(0, tr("Error saving game"), qsqQuery->lastError().text());
			qsdBase.rollback();
			return false;
		}
	}
	qsqQuery->clear();
	qsqQuery->prepare("create table game (ROW_ INTEGER,COLUMN_ INTEGER,DATA INTEGER);");
	if (!qsqQuery->exec())
	{
		qsqQuery->prepare("DELETE FROM game");
		if (!qsqQuery->exec())
		{
			QMessageBox::critical(0, tr("Error saving game"), qsqQuery->lastError().text());
			qsdBase.rollback();
			return false;
		}
	}
	qsqQuery->clear();
//
	qsqQuery->prepare("INSERT INTO params (NAME,DATA) VALUES (:NAME,:DATA)");
	qsqQuery->bindValue(":NAME","ROW_COUNT");
	qsqQuery->bindValue(":DATA",qsimModel->rowCount());
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error saving game"), qsqQuery->lastError().text());
		qsdBase.rollback();
		return false;
	}
	qsqQuery->bindValue(":NAME","COLUMN_COUNT");
	qsqQuery->bindValue(":DATA",qsimModel->columnCount());
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error saving game"), qsqQuery->lastError().text());
		qsdBase.rollback();
		return false;
	}
	setLastItem();
	qsqQuery->bindValue(":NAME","LAST_ROW");
	qsqQuery->bindValue(":DATA",qsiLastItem->row());
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error saving game"), qsqQuery->lastError().text());
		qsdBase.rollback();
		return false;
	}
	qsqQuery->bindValue(":NAME","LAST_COLUMN");
	qsqQuery->bindValue(":DATA",qsiLastItem->column());
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error saving game"), qsqQuery->lastError().text());
		qsdBase.rollback();
		return false;
	}
//information
	qsqQuery->bindValue(":NAME","MSEC_OFFSET");
	qsqQuery->bindValue(":DATA",qtGameTime.elapsed()+iMSecOffset);
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error saving game"), qsqQuery->lastError().text());
		qsdBase.rollback();
		return false;
	}
	qsqQuery->bindValue(":NAME","KILL_ITEMS_COUNT");
	qsqQuery->bindValue(":DATA",iKillItemsCount);
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error saving game"), qsqQuery->lastError().text());
		qsdBase.rollback();
		return false;
	}
	qsqQuery->bindValue(":NAME","HELP_COUNT");
	qsqQuery->bindValue(":DATA",iHelpCount);
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error saving game"), qsqQuery->lastError().text());
		qsdBase.rollback();
		return false;
	}
	qsqQuery->bindValue(":NAME","UNDO_COUNT");
	qsqQuery->bindValue(":DATA",iUndoCount);
	if (!qsqQuery->exec())
	{
		QMessageBox::critical(0, tr("Error saving game"), qsqQuery->lastError().text());
		qsdBase.rollback();
		return false;
	}
	qsqQuery->clear();
//
	QProgressDialog* qpdProgress=new QProgressDialog(tr("Saving..."),
			tr("Break"),
			0,
			qsimModel->rowCount()*qsimModel->columnCount(),
			this);
	qpdProgress->setModal(true);
	qsqQuery->prepare("INSERT INTO game (ROW_,COLUMN_,DATA) VALUES (:ROW_,:COLUMN_,:DATA)");
	for (int i=0; i<qsimModel->rowCount(); i++)
	{
		for (int j=0; j<qsimModel->columnCount(); j++)
		{
			qpdProgress->setValue(qpdProgress->value()+1);
			QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
			if (qpdProgress->wasCanceled())
			{
				delete qpdProgress;
				delete qsqQuery;
				qsdBase.rollback();
				qsdBase.close();
				return false;
			}
			if ((!qsimModel->item(i,j)) || (qsimModel->item(i,j)->text().isEmpty()))
				continue;
			qsqQuery->bindValue(":ROW_",i);
			qsqQuery->bindValue(":COLUMN_",j);
			qsqQuery->bindValue(":DATA",qsimModel->item(i,j)->text().toInt());
			if (!qsqQuery->exec())
			{
				QMessageBox::critical(0, tr("Error saving game"), qsqQuery->lastError().text());
				qsdBase.rollback();
				return false;
			}
		}
	}
	qsdBase.commit();
	qsqQuery->clear();
	delete qsqQuery;
	delete qpdProgress;
	qsdBase.close();
//
	qsCurrentFile=qsFileName;
	return true;
}
//
bool QNumbersDestroyer::okToContinue()
{
	if (isGame)
	{
		int res=QMessageBox::warning(this,
								QApplication::applicationName(),
								tr("Do you want save this game?"),
								QMessageBox::Yes | QMessageBox::Default,
								QMessageBox::No,
								QMessageBox::Cancel);
		if (res==QMessageBox::Yes)
			return slotSaveGame();
		if (res==QMessageBox::Cancel)
			return false;
	}
	return true;
}
//
void QNumbersDestroyer::slotUpdateInformation()
{
//Update first numbers
	for (int i=0; i<qsimModel->columnCount(); i++)
	{
		for (int j=0; j<qsimModel->rowCount(); j++)
		{
			if ((qsimModel->item(j,i)) && (!qsimModel->item(j,i)->text().isEmpty()))
			{
				QStandardItem* qsiHeaderItem=new QStandardItem(qsimModel->item(j,i)->text());
				qsimModel->setHorizontalHeaderItem(i,qsiHeaderItem);
				break;
			}
		}
	}
//End update first numbers

//Game params
	emit signalGameParams(qsimModel->columnCount(),
				qsimModel->rowCount(),
				iItemsCount);
//Chet hodov
	emit signalGameSchetHodov(iKillItemsCount,
				iHelpCount,
				iUndoCount);

}
//
void QNumbersDestroyer::slotUpdateActions()
{
	actionUndo->setEnabled(qmuUndo->undoAvailable() && bStopAutoGame);
	actionUndo->setToolTip(tr("%1 undo changes").arg(qmuUndo->undoCount()));
	actionBeginAutoGame->setEnabled(isGame && bStopAutoGame);
	actionStopAutoGame->setEnabled(isGame && !bStopAutoGame);
	actionSaveGame->setEnabled(isGame && bStopAutoGame);
	actionSaveAsGame->setEnabled(isGame && bStopAutoGame);
	actionUpdateGame->setEnabled(isGame && bStopAutoGame);
	actionGetHelp->setEnabled(isGame && bStopAutoGame);
	actionNewGame->setEnabled(bStopAutoGame);
	actionOpenGame->setEnabled(bStopAutoGame);
}
//
void QNumbersDestroyer::slotUndo()
{
	UndoOperation uo=qmuUndo->get();
	while (uo.uoperation==DELETE_LINE)
	{
		iUndoCount++;
		qsimModel->insertRow(uo.iFirstRow);
		QStandardItem* item;
		for (int i=0; i<qsimModel->columnCount(); i++)
		{
			item=new QStandardItem("");
			item->setTextAlignment(Qt::AlignCenter);
			item->setBackground(QBrush(Qt::black));
			qsimModel->setItem(uo.iFirstRow,i,item);
		}
		qtvTable->resizeRowToContents(uo.iFirstRow);
		uo=qmuUndo->get();
	}
	if (uo.uoperation==ADD_ROWS)
	{
		qsimModel->removeRows(uo.iFirstRow+1,qsimModel->rowCount()-uo.iFirstRow-1);
		for (int i=uo.iFirstColumn+1; i<qsimModel->columnCount(); i++)
		{
			delete qsimModel->item(uo.iFirstRow,i);
		}
		iItemsCount=uo.iFirstNumber;
	}
	if (uo.uoperation==DELETE_ITEMS)
	{
		QStandardItem* item;
		item=new QStandardItem(QString().setNum(uo.iFirstNumber));
		item->setTextAlignment(Qt::AlignCenter);
		item->setBackground(QBrush(Qt::cyan));
		qsimModel->setItem(uo.iFirstRow,uo.iFirstColumn,item);
		item=new QStandardItem(QString().setNum(uo.iSecondNumber));
		item->setTextAlignment(Qt::AlignCenter);
		item->setBackground(QBrush(Qt::cyan));
		qsimModel->setItem(uo.iSecondRow,uo.iSecondColumn,item);
		iKillItemsCount-=2;
		iItemsCount+=2;
	}
	iUndoCount++;
	slotUpdateActions();
	slotUpdateInformation();
}
//
QList<QStandardItem*> QNumbersDestroyer::itemsForDelete(const QStandardItem& item)
{
	QList<QStandardItem*> itemList;
	QStandardItem* tmpItem;
	bool bTmp;
//Top & Bottom
	bTmp=true;
	for (int i=item.row()-1; i>=0; i--)
	{
		tmpItem=qsimModel->item(i,item.column());
		if (tmpItem && (!tmpItem->text().isEmpty()))
		{
			bTmp=false;
			if ((tmpItem->text()==item.text()) || ((tmpItem->text().toInt()+item.text().toInt())==10))
			{
				itemList.append(tmpItem);
			}
			break;
		}
	}
	if (bTmp)
	{
		for (int i=qsimModel->rowCount(); i>item.row()+1; i--)
		{
			tmpItem=qsimModel->item(i,item.column());
			if (tmpItem && (!tmpItem->text().isEmpty()))
			{
				if ((tmpItem->text()==item.text()) || ((tmpItem->text().toInt()+item.text().toInt())==10))
				{
					itemList.append(tmpItem);
				}
				break;
			}
		}
	}
//Left & Top_Right
	bTmp=true;
	for (int i=item.column()-1; i>=0; i--)
	{
		tmpItem=qsimModel->item(item.row(),i);
		if (tmpItem && (!tmpItem->text().isEmpty()))
		{
			bTmp=false;
			break;
		}
	}
	if (bTmp)
	{
		for (int i=qsimModel->columnCount(); i>=0; i--)
		{
			tmpItem=qsimModel->item(item.row()-1,i);
			if (tmpItem && (!tmpItem->text().isEmpty()))
			{
				if ((tmpItem->text()==item.text()) || ((tmpItem->text().toInt()+item.text().toInt())==10))
				{
					itemList.append(tmpItem);
				}
				break;
			}
		}
	}
//Left & right
	bTmp=true;
	for (int i=item.column()-1; i>=0; i--)
	{
		tmpItem=qsimModel->item(item.row(),i);
		if (tmpItem && (!tmpItem->text().isEmpty()))
		{
			bTmp=false;
			if ((tmpItem->text()==item.text()) || ((tmpItem->text().toInt()+item.text().toInt())==10))
			{
				itemList.append(tmpItem);
			}
			break;
		}
	}
	if (bTmp)
	{
		for (int i=qsimModel->columnCount(); i>item.column()+1; i--)
		{
			tmpItem=qsimModel->item(item.row(),i);
			if (tmpItem && (!tmpItem->text().isEmpty()))
			{
				if ((tmpItem->text()==item.text()) || ((tmpItem->text().toInt()+item.text().toInt())==10))
				{
					itemList.append(tmpItem);
				}
				break;
			}
		}
	}
//Bottom
	for (int i=item.row()+1; i<qsimModel->rowCount(); i++)
	{
		tmpItem=qsimModel->item(i,item.column());
		if (tmpItem && (!tmpItem->text().isEmpty()))
		{
			if ((tmpItem->text()==item.text()) || ((tmpItem->text().toInt()+item.text().toInt())==10))
			{
				itemList.append(tmpItem);
			}
			break;
		}
	}
//Right
	for (int i=item.column()+1; i<qsimModel->columnCount(); i++)
	{
		tmpItem=qsimModel->item(item.row(),i);
		if (tmpItem && (!tmpItem->text().isEmpty()))
		{
			if ((tmpItem->text()==item.text()) || ((tmpItem->text().toInt()+item.text().toInt())==10))
			{
				itemList.append(tmpItem);
			}
			break;
		}
	}
	return itemList;
}
//
void QNumbersDestroyer::slotGetHelp()
{
	QList<QStandardItem*> itemList;
	for (int i=0; i<qsimModel->rowCount(); i++)
	{
		for (int j=0; j<qsimModel->columnCount(); j++)
		{
			QStandardItem* item=qsimModel->item(i,j);
			if (item && (!item->text().isEmpty()))
			{
				itemList=itemsForDelete(*qsimModel->item(i,j));
				if (!itemList.isEmpty())
				{
					iHelpCount++;
					deleteItems(item,itemList.at(0));
					return;
				}
			}
		}
	}
	QMessageBox::information(this,
				qApp->applicationName(),
				tr("No numbers for destroying"));
}
//
void QNumbersDestroyer::deleteItems(QStandardItem* current, QStandardItem* previous)
{
	int iWaitTime=qsetAppSettings->value("Game/WaitTime",50).toInt();
	int iBlinkCount=qsetAppSettings->value("Game/BlinkCount",3).toInt();

	qtvTable->scrollTo(qsimModel->indexFromItem(current));
	blinkItem(current,iWaitTime,iBlinkCount);
	qtvTable->scrollTo(qsimModel->indexFromItem(previous));
	blinkItem(previous,iWaitTime,iBlinkCount);
	checkndel(current,previous);
}
//
void QNumbersDestroyer::slotAutoGame()
{
	if (qsimModel->rowCount()<=0)
		return;
	bStopAutoGame=false;
	slotUpdateActions();
	QCoreApplication::processEvents();

	QList<QStandardItem*> itemList;
	bool isEnd;
	do
	{
		isEnd=true;
		for (int i=0; i<qsimModel->rowCount(); i++)
		{
			for (int j=0; j<qsimModel->columnCount(); j++)
			{
				QCoreApplication::processEvents();
				if (bStopAutoGame)
				{
					slotUpdateActions();
					return;
				}
				QStandardItem* item=qsimModel->item(i,j);
				if (item && (!item->text().isEmpty()))
				{
					itemList=itemsForDelete(*qsimModel->item(i,j));
					if (!itemList.isEmpty())
					{
						iHelpCount++;
						deleteItems(item,itemList.at(0));
						isEnd=false;
					}
				}
			}
		}
	}
	while (!isEnd);
	if (qsimModel->rowCount()>0)
	{
		slotUpdateGame();
		QCoreApplication::processEvents();
		if (!bStopAutoGame)
			slotAutoGame();
	}
}
//
void QNumbersDestroyer::timerEvent(QTimerEvent *event)
{
	emit signalGameTime(qtGameTime.elapsed()+iMSecOffset);
}
//
void QNumbersDestroyer::nullInformation()
{
	iMSecOffset=0;
	iKillItemsCount=0;
	iHelpCount=0;
	iUndoCount=0;
	iItemsCount=0;
	if (iTimeID!=0)
	{
		this->killTimer(iTimeID);
		iTimeID=0;
	}
	emit signalGameTime(0);
	qmuUndo->clear();
	isGame=false;
	bStopAutoGame=true;
	slotUpdateInformation();
	slotUpdateActions();
}
//
void QNumbersDestroyer::blinkItem(QStandardItem* current, const int iWaitTime, const int iBlinkCount)
{
	for (int i=0; i<iBlinkCount*2; i++)
	{
		if (current->background()==QBrush(Qt::yellow))
			current->setBackground(QBrush(Qt::red));
		else
			current->setBackground(QBrush(Qt::yellow));
		QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
		#ifdef Q_WS_WIN
			Sleep(iWaitTime);
		#else
			usleep( iWaitTime * 1000 );
		#endif
	}
}
//
