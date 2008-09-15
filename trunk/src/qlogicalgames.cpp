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

#include <QtGui>
#include "qlogicalgames.h"

#include <QTextEdit>
#include <QTextStream>
#include <QCloseEvent>
#include <QFileDialog>

QLogicalGames::QLogicalGames()
{
      createActions();
      createMenus();
      createToolBars();
      createStatusBar();

      readSettings();
}

void QLogicalGames::closeEvent(QCloseEvent *event)
{
      if (maybeSave()) {
            writeSettings();
            event->accept();
      } else {
            event->ignore();
      }
}

void QLogicalGames::newFile()
{
}

void QLogicalGames::open()
{
}

bool QLogicalGames::save()
{
}

bool QLogicalGames::saveAs()
{
}

void QLogicalGames::about()
{
}

void QLogicalGames::documentWasModified()
{
}

void QLogicalGames::createActions()
{
}

void QLogicalGames::createMenus()
{
}

void QLogicalGames::createToolBars()
{
}

void QLogicalGames::createStatusBar()
{
}

void QLogicalGames::readSettings()
{
}

void QLogicalGames::writeSettings()
{
}

bool QLogicalGames::maybeSave()
{
}

void QLogicalGames::loadFile(const QString &fileName)
{
}

bool QLogicalGames::saveFile(const QString &fileName)
{
}

void QLogicalGames::setCurrentFile(const QString &fileName)
{
}

QLogicalGames::~QLogicalGames()
{

}

