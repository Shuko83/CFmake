
#include <QtGui>

#include "QsGroupRadio.h"


QsGroupRadio::QsGroupRadio(QWidget *parent)
	: QGroupBox(parent)
{
	_rbAssociated = 0;
}

QsGroupRadio::~QsGroupRadio()
{

}

void QsGroupRadio::mouseReleaseEvent(QMouseEvent * /*event*/)
{
	if (_rbAssociated->isChecked())
		_rbAssociated->setChecked(false);
	else
		_rbAssociated->setChecked(true);
}


bool QsGroupRadio::event(QEvent *event)
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
