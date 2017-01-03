

#ifndef QS_WIDGETS_QSTABWIDGET_H
#define QS_WIDGETS_QSTABWIDGET_H

#include <QTabWidget>
#include <QsWidgets_Global.h>

class QSWIDGETS_EXPORT QsTabWidget : public QTabWidget
{
	Q_OBJECT
		Q_PROPERTY(QString ClassCSS READ getClassCSS WRITE setClassCSS);

public:
	QsTabWidget(QWidget *parent = 0);
	virtual ~QsTabWidget();

	QString getClassCSS() const { return _classCSS; }
	void setClassCSS(QString val) { _classCSS = val; }

	QTabBar* getTabBar() { return tabBar(); }

	virtual bool event(QEvent *event);

private:
	QString _classCSS;

signals:
	void mouseEnter();
	void mouseLeave();
};

#endif
