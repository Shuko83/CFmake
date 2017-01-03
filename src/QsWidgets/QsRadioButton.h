

#ifndef QS_WIDGETS_QSRADIOBUTTON_H
#define QS_WIDGETS_QSRADIOBUTTON_H

#include <QRadioButton>
#include <QsWidgets_Global.h>


class QSWIDGETS_EXPORT QsRadioButton : public QRadioButton
{
	Q_OBJECT
		Q_PROPERTY(QString ClassCSS READ getClassCSS WRITE setClassCSS);

public:
	QsRadioButton(QWidget *parent = 0);
	virtual ~QsRadioButton();

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
