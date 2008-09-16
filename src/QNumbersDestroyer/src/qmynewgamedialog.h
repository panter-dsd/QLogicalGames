#ifndef QMYNEWGAMEDIALOG_H
#define QMYNEWGAMEDIALOG_H
//
class QWidget;
class QLabel;
class QGroupBox;
class QDialogButtonBox;
//
#include <QDialog>
#include <QSpinBox>
//
class QMyNewGameDialog : public QDialog
{
Q_OBJECT
private:
	QLabel*							qlColumnCount;
	QSpinBox*						qspColumnCount;
	QLabel*							qlFewNumbers;
	QSpinBox*						qspFewNumbers;
	QGroupBox*					qgbColumnsAndNumbers;
	QDialogButtonBox*		qdbbButtons;
public:
	QMyNewGameDialog(QWidget* parent=0, Qt::WFlags f=Qt::WindowSystemMenuHint);
	int getNumbersCount()							{return qspFewNumbers->value();}
	int getColumnsCount()							{return qspColumnCount->value();}
	void setNumbersCount(int numbers)		{qspFewNumbers->setValue(numbers);}
	void setColumnsCount(int columns)		{qspColumnCount->setValue(columns);}
	void setNumbersAndColumnsCount(int numbers, int columns) {
		setNumbersCount(numbers);
		setColumnsCount(columns);
	}
private:
	void createControls();
	void setLayouts();
	void setConnects();
};
#endif
