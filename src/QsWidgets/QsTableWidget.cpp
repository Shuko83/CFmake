
#include <QtGui>

#include "QsTableWidget.h"

QsTableWidget::QsTableWidget(QWidget *parent)	: QTableWidget(parent)
{

}

QsTableWidget::~QsTableWidget()
{

}

bool QsTableWidget::event(QEvent *event)
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
	return QTableWidget::event(event);
}
