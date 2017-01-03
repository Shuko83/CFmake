
#ifndef QS_WIDGETS_QSSCROLLAREA_H
#define QS_WIDGETS_QSSCROLLAREA_H

#include <QScrollArea>
#include <QsWidgets_Global.h>

class QSWIDGETS_EXPORT QsScrollArea : public QScrollArea
{
	Q_OBJECT
	Q_PROPERTY (QString ClassCSS READ getClassCSS WRITE setClassCSS);


public:
	QsScrollArea(QWidget *parent = 0);
	virtual ~QsScrollArea();

	QString getClassCSS() const { return _classCSS; }
	void setClassCSS(QString val) { _classCSS = val; }

	virtual bool event(QEvent *event);

private:
	QString _classCSS;

signals:
	void mouseEnter();
	void mouseLeave();
};

#endif
