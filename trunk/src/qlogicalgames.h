/***************************************************************************
 *   Copyright (C) 2008 by PanteR   *
 *   panter.dsd@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef QLOGICALGAMES_H
#define QLOGICALGAMES_H

#include <QMainWindow>
#include <QCloseEvent>
#include "./QNumbersDestroyer/mainwidget.h"

class QAction;
class QMenu;
class QSettings;

class QLogicalGames:public QMainWindow
{
Q_OBJECT
private:
	QNumbersDestroyer*				qndNumbersDestroyer;
	
	QSettings*									qsetAppSettings;
public:
	QLogicalGames();
	~QLogicalGames();

protected:
	void closeEvent(QCloseEvent *event);

private:
	void createWidgets();
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void readSettings();
	void writeSettings();

private slots:
};

#endif
