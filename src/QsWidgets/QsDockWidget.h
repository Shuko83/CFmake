

#ifndef QS_WIDGETS_QSDOCKWIDGET_H
#define QS_WIDGETS_QSDOCKWIDGET_H

#include <QDockWidget>
#include <QsWidgets_Global.h>

class QSWIDGETS_EXPORT QsDockWidget : public QDockWidget
{
    Q_OBJECT
	Q_PROPERTY (QString ClassCSS READ getClassCSS WRITE setClassCSS);

public:
    QsDockWidget(QWidget *parent = 0);
	virtual ~QsDockWidget();

	QString getClassCSS() const { return _classCSS; }
	void setClassCSS(QString val) { _classCSS = val; }
private:
	QString _classCSS;
};
	
#endif
