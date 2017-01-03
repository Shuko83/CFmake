
#include <QtGui>

#include "QsTextEdit.h"

QsTextEdit::QsTextEdit(QWidget *parent)	: QTextEdit(parent)
{

}

QsTextEdit::~QsTextEdit()
{

}

bool QsTextEdit::event(QEvent *event)
{
	switch (event->type())
	{
	case QEvent::Enter:
		emit mouseEnter();
		break;

	case QEvent::Leave:
		emit mouseLeave();
		break;
	}
	return QTextEdit::event(event);
}
