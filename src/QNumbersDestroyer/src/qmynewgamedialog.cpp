#include <QtGui>
#include "qmynewgamedialog.h"
//
#define MAX_COLUMN_COUNT 50
#define MAXIMUM 200
//
QMyNewGameDialog::QMyNewGameDialog(QWidget* parent, Qt::WFlags f)
	: QDialog(parent, f)
{
	this->setWindowTitle(tr("New game options"));
	createControls();
	setLayouts();
	setConnects();
	qspColumnCount->setFocus();
	qspColumnCount->selectAll();
}
//
void QMyNewGameDialog::createControls()
{
	qlColumnCount=new QLabel(tr("Columns count"),this);
	qlColumnCount->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
	qspColumnCount=new QSpinBox(this);
	qspColumnCount->setRange(2,MAX_COLUMN_COUNT);

	qlFewNumbers=new QLabel(tr("Numbers count"),this);
	qlFewNumbers->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
	qspFewNumbers=new QSpinBox(this);
	qspFewNumbers->setRange(2,MAXIMUM);

	qgbColumnsAndNumbers=new QGroupBox(this);

	qdbbButtons=new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
				Qt::Horizontal,
				this);
}
//
void QMyNewGameDialog::setLayouts()
{
	QGridLayout* qglColumnsAndNumbers=new QGridLayout();
	qglColumnsAndNumbers->addWidget(qlColumnCount,0,0);
	qglColumnsAndNumbers->addWidget(qspColumnCount,0,1);
	qglColumnsAndNumbers->addWidget(qlFewNumbers,1,0);
	qglColumnsAndNumbers->addWidget(qspFewNumbers,1,1);

	qgbColumnsAndNumbers->setLayout(qglColumnsAndNumbers);

	QVBoxLayout* qvblMainLayout=new QVBoxLayout();
	qvblMainLayout->addWidget(qgbColumnsAndNumbers);
	qvblMainLayout->addWidget(qdbbButtons);

	this->setLayout(qvblMainLayout);
}
//
void QMyNewGameDialog::setConnects()
{
	connect(qdbbButtons,SIGNAL(accepted()),this,SLOT(accept()));
	connect(qdbbButtons,SIGNAL(rejected()),this,SLOT(reject()));

}
//

