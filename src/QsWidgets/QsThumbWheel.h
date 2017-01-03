

#ifndef QS_WIDGETS_QSTHUMBWHEEL_H
#define QS_WIDGETS_QSTHUMBWHEEL_H

#include <QtThumbWheel>
#include <QsWidgets_Global.h>


class QSWIDGETS_EXPORT QsThumbWheel : public QtThumbWheel
{
	Q_OBJECT
		Q_PROPERTY(QString ClassCSS READ getClassCSS WRITE setClassCSS);

public:
	QsThumbWheel(QWidget *parent = 0);
	virtual ~QsThumbWheel();
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
