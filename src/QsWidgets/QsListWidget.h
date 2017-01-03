
#ifndef QS_WIDGETS_QSLISTWIDGET_H
#define QS_WIDGETS_QSLISTWIDGET_H

#include <QListWidget>
#include <QsWidgets_Global.h>

class QSWIDGETS_EXPORT QsListWidget : public QListWidget
{
    Q_OBJECT
	Q_PROPERTY (QString ClassCSS READ getClassCSS WRITE setClassCSS);


public:
    QsListWidget(QWidget *parent = 0);
	virtual ~QsListWidget();

	QString getClassCSS() const { return _classCSS; }
	void setClassCSS(QString val) { _classCSS = val; }
private:
	QString _classCSS;
};

#endif
