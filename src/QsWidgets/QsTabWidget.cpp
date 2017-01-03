
#include <QtGui>

#include "QsTabWidget.h"

QsTabWidget::QsTabWidget(QWidget *parent) : QTabWidget(parent)
{

}

QsTabWidget::~QsTabWidget()
{

}

bool QsTabWidget::event(QEvent *event)
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
	return QTabWidget::event(event);
}
