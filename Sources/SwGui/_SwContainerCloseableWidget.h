/*
 * Classe container closeable widget
 */
#include <QWidget>

#include <QVBoxLayout>

#ifndef __SwContainerCloseableWidget_H
#define __SwContainerCloseableWidget_H


class _SwContainerCloseableWidget :
	public QWidget
{
	Q_OBJECT
	
public:
	_SwContainerCloseableWidget(QWidget *parent = 0);
	~_SwContainerCloseableWidget();

	void setContainedWidget(QWidget *w);
protected:
    virtual void closeEvent ( QCloseEvent * event );
  
signals:
	void onClose();


private:
	QVBoxLayout * _layout;
	QWidget * _content;
};

#endif
