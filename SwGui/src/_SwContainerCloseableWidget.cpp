
#include "_SwContainerCloseableWidget.h"


//-----------------------------------------------------------------------
_SwContainerCloseableWidget::_SwContainerCloseableWidget(QWidget *parent) :QWidget(parent)
{
	_content = 0;
	_layout = new QVBoxLayout(this);
	_layout->setContentsMargins(0, 0, 0, 0);
	setLayout(_layout);
}

//-----------------------------------------------------------------------
_SwContainerCloseableWidget::~_SwContainerCloseableWidget()
{

}

//-----------------------------------------------------------------------
void _SwContainerCloseableWidget::setContainedWidget(QWidget *w)
{
	if ( _content != 0 )
	{
		//Suppression du precedent
		_layout->removeWidget(_content);
		_content->setParent(nullptr);
		_content = 0;
	}
	if ( w != 0 )
	{
		//Enregistrement du nouveau
		resize(w->size());
		_layout->addWidget(w);
		_content = w;
	}
}

//-----------------------------------------------------------------------
void _SwContainerCloseableWidget::closeEvent(QCloseEvent * event)
{
	QWidget::closeEvent(event);
	emit onClose();
}
