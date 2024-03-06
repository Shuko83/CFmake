/*
 * Classe widget console
 */

#include <QtWidgets>

#include <QTimer>
#include <QKeyEvent>
#include <QShowEvent>
#include <QWidget>
#include <QLabel>
#include <QStringList>

#ifndef __SwConsoleWidget_H
#define __SwConsoleWidget_H


class _SwConsoleWidget :
	public QWidget
{
	Q_OBJECT
private:
	//font height
	int _fHeight;
	//font height
	int _maxWidth;
	//max message
	int _maxMessages;
	//liste des messages
	QStringList _messages;
	//Content has changed
	bool _hasChanged;
public:
	_SwConsoleWidget(QWidget *parent = 0);
	~_SwConsoleWidget(void);

	static QWidget * buildScrollableConsole(QWidget *parent,_SwConsoleWidget ** console);

	void addMessage(QString message);
	void setMaxMessages(int val) { _maxMessages = val; }
protected:
  virtual void paintEvent ( QPaintEvent * event );
  
public slots:
	void refreshContent();

};

#endif
