#ifndef QMYINFORMATIONPANEL_H
#define QMYINFORMATIONPANEL_H
//
class QGroupBox;
class QLabel;
class QTime;
class QLCDNumber;
#include <QWidget>
//
class QMyInformationPanel : public QWidget
{
Q_OBJECT
private:
	QGroupBox*					qgbTimeBox;
	QLabel*						qlTimeLabel;
	
	QGroupBox*					qgbSchetHodov;
	QLabel*						qlKillItemsCount;
	QLCDNumber*					qlcdnKillItemsCount;
	QLabel*						qlHelpCount;
	QLCDNumber*					qlcdnHelpCount;
	QLabel*						qlUndoCount;
	QLCDNumber*					qlcdnUndoCount;
	
	QGroupBox*					qgbGameParams;
	QLabel*						qlColumnCount;
	QLCDNumber*					qlcdnColumnCount;
	QLabel*						qlRowCount;
	QLCDNumber*					qlcdnRowCount;
	QLabel*						qlItemsCount;
	QLCDNumber*					qlcdnItemsCount;

public:
	QMyInformationPanel(QWidget* parent = 0, Qt::WindowFlags f = 0);
private:
	void createControls();
	void setLayouts();
public slots:
	void slotSetTime(const int msec);
	void slotSetParams(const int columnCount,const int rowCount,const int itemsCount);
	void slotSetSchetHodov(const int killItemsCount,const int helpCount,const int undoCount);
};
#endif
