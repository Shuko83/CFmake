
#ifndef QS_WIDGETS_QSWIDGET_H
#define QS_WIDGETS_QSWIDGET_H

#include <QWidget>
#include <QsWidgets_Global.h>

class QSWIDGETS_EXPORT QsWidget : public QWidget
{
    Q_OBJECT
	Q_PROPERTY (QString ClassCSS READ getClassCSS WRITE setClassCSS);


public:
    QsWidget(QWidget *parent = 0);
	virtual ~QsWidget();

	QString getClassCSS() const { return _classCSS; }
	void setClassCSS(QString val) { _classCSS = val; }
private:
	QString _classCSS;
};

#endif
