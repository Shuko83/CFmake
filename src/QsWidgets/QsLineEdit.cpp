
#include <QtGui>

#include "QsLineEdit.h"

QsLineEdit::QsLineEdit(QWidget *parent) : QLineEdit(parent)
{

}

QsLineEdit::~QsLineEdit()
{

}


bool QsLineEdit::event(QEvent *event)
{
	switch (event->type())
	{
	case QEvent::Enter:
		emit mouseEnter();
		break;

	case QEvent::Leave:
		emit mouseLeave();
		break;

	case QEvent::MouseButtonPress:
		emit mousePress();
		break;

	case QEvent::FocusOut:
		emit focusOut();
		break;
	}
	return QLineEdit::event(event);
}

void QsLineEdit::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Enter)
		emit enterKeyPress();

	QLineEdit::keyPressEvent(event);
}
