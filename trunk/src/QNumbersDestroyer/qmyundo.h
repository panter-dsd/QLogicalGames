#ifndef QMYUNDO_H
#define QMYUNDO_H
//
#include <QList>
//
enum UndoOperations {DELETE_LINE,DELETE_ITEMS,ADD_ROWS};
struct UndoOperation
{
	UndoOperations uoperation;
	int iFirstRow;
	int iFirstColumn;
	int iFirstNumber;
	int iSecondRow;
	int iSecondColumn;
	int iSecondNumber;
};
class QMyUndo
{
private:
	QList<UndoOperation>*				qlUndoList;
public:
	QMyUndo();
	virtual ~QMyUndo();
	void put(UndoOperation uo);
	UndoOperation get();
	bool undoAvailable();
	int undoCount() {return qlUndoList->count();}
	void clear();
};

#endif // QMYUNDO_H
