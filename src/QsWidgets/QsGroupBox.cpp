
#include <QtGui>

#include "QsGroupBox.h"


QsGroupBox::QsGroupBox(QWidget *parent)
	: QGroupBox(parent)
{

}

QsGroupBox::~QsGroupBox()
{

}

bool QsGroupBox::event(QEvent *event)
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
	return QGroupBox::event(event);
}
