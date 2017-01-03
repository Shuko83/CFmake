

#ifndef QS_WIDGETS_QSTABLEWIDGET_H
#define QS_WIDGETS_QSTABLEWIDGET_H

#include <QTableWidget>
#include <QsWidgets_Global.h>

class QSWIDGETS_EXPORT QsTableWidget : public QTableWidget
{
	Q_OBJECT
	Q_PROPERTY (QString ClassCSS READ getClassCSS WRITE setClassCSS);

public:
	QsTableWidget(QWidget *parent = 0);
	virtual ~QsTableWidget();

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
