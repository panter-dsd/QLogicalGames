#include "qmyundo.h"
#include <QString>
QMyUndo::QMyUndo()
{
	qlUndoList=new QList<UndoOperation>();
}
//
QMyUndo::~QMyUndo()
{
	qlUndoList->clear();
}
//
void QMyUndo::put(UndoOperation uo)
{
	qlUndoList->append(uo);
}
//
UndoOperation QMyUndo::get()
{
	if (qlUndoList->isEmpty())
		return UndoOperation();
	UndoOperation oper=qlUndoList->last();
	qlUndoList->removeLast();
	return oper;
}
//
bool QMyUndo::undoAvailable()
{
	if (qlUndoList->isEmpty())
		return false;
	else
		return true;
}
//
void QMyUndo::clear()
{
	qlUndoList->clear();
}
//

