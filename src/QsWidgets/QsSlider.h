

#ifndef QS_WIDGETS_QSSLIDER_H
#define QS_WIDGETS_QSSLIDER_H

#include <QSlider>
#include <QsWidgets_Global.h>


class QSWIDGETS_EXPORT QsSlider : public QSlider
{
	Q_OBJECT
	Q_PROPERTY (QString ClassCSS READ getClassCSS WRITE setClassCSS);

public:
	QsSlider(QWidget *parent = 0);
	virtual ~QsSlider();

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
