
#include <QtGui>

#include "QsComboBox.h"
#include <QAbstractItemView>


QsComboBox::QsComboBox(QWidget *parent) : QComboBox(parent)
{
	view()->installEventFilter(this);
}

QsComboBox::~QsComboBox()
{

}

bool QsComboBox::event(QEvent *event)
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
	return QComboBox::event(event);
}

bool QsComboBox::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::Hide)
	{
		QAbstractItemView * item = qobject_cast<QAbstractItemView*>(obj);

		if (item  && item == view())
			emit comboClosed();
	}
	return QObject::eventFilter(obj, event);
}

