#include <QtGui>
#include "qmyinformationpanel.h"
//
QMyInformationPanel::QMyInformationPanel(QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent,f)
{
	createControls();
	setLayouts();
}
//
void QMyInformationPanel::createControls()
{
	qgbTimeBox=new QGroupBox(tr("Game time"),this);
	qlTimeLabel=new QLabel("00:00:00",qgbTimeBox);
	qlTimeLabel->setAlignment(Qt::AlignHCenter);

	qgbSchetHodov=new QGroupBox(tr("Score"),this);
	qlKillItemsCount=new QLabel(tr("Destroyed numbers count"),qgbSchetHodov);
	qlcdnKillItemsCount=new QLCDNumber(qgbSchetHodov);
	qlHelpCount=new QLabel(tr("Get help count"),qgbSchetHodov);
	qlcdnHelpCount=new QLCDNumber(qgbSchetHodov);
	qlUndoCount=new QLabel(tr("Undo count"),qgbSchetHodov);
	qlcdnUndoCount=new QLCDNumber(qgbSchetHodov);

	qgbGameParams=new QGroupBox(tr("Game params"),this);
	qlColumnCount=new QLabel(tr("Columns count"),qgbGameParams);
	qlcdnColumnCount=new QLCDNumber(qgbGameParams);
	qlRowCount=new QLabel(tr("Rows count"),qgbGameParams);
	qlcdnRowCount=new QLCDNumber(qgbGameParams);
	qlItemsCount=new QLabel(tr("Numbers count"),qgbGameParams);
	qlcdnItemsCount=new QLCDNumber(qgbGameParams);
}
//
void QMyInformationPanel::setLayouts()
{
//Time
	QVBoxLayout* qvblTimeLayout=new QVBoxLayout();
	qvblTimeLayout->addWidget(qlTimeLabel);

	qgbTimeBox->setLayout(qvblTimeLayout);
//Schet
	QGridLayout* qglSchetLayout=new QGridLayout();
	qglSchetLayout->addWidget(qlKillItemsCount,0,0);
	qglSchetLayout->addWidget(qlcdnKillItemsCount,0,1);
	qglSchetLayout->addWidget(qlHelpCount,1,0);
	qglSchetLayout->addWidget(qlcdnHelpCount,1,1);
	qglSchetLayout->addWidget(qlUndoCount,2,0);
	qglSchetLayout->addWidget(qlcdnUndoCount,2,1);

	qgbSchetHodov->setLayout(qglSchetLayout);
//Game params
	QGridLayout* qglGameParamsLayout=new QGridLayout();
	qglGameParamsLayout->addWidget(qlColumnCount,0,0);
	qglGameParamsLayout->addWidget(qlcdnColumnCount,0,1);
	qglGameParamsLayout->addWidget(qlRowCount,1,0);
	qglGameParamsLayout->addWidget(qlcdnRowCount,1,1);
	qglGameParamsLayout->addWidget(qlItemsCount,2,0);
	qglGameParamsLayout->addWidget(qlcdnItemsCount,2,1);

	qgbGameParams->setLayout(qglGameParamsLayout);
//Main
	QVBoxLayout* qvblMailLayout=new QVBoxLayout();
	qvblMailLayout->addWidget(qgbTimeBox);
	qvblMailLayout->addWidget(qgbSchetHodov);
	qvblMailLayout->addWidget(qgbGameParams);

	this->setLayout(qvblMailLayout);
}
//
void QMyInformationPanel::slotSetTime(const int msec)
{
	qlTimeLabel->setText(QTime(0,0).addMSecs(msec).toString("hh:mm:ss"));
}
//
void QMyInformationPanel::slotSetParams(const int columnCount,const int rowCount,const int itemsCount)
{
	qlcdnColumnCount->display(columnCount);
	qlcdnRowCount->display(rowCount);
	qlcdnItemsCount->display(itemsCount);
}
//
void QMyInformationPanel::slotSetSchetHodov(const int killItemsCount,const int helpCount,const int undoCount)
{
	qlcdnKillItemsCount->display(killItemsCount);
	qlcdnHelpCount->display(helpCount);
	qlcdnUndoCount->display(undoCount);
}
//
