

#ifndef QS_WIDGETS_QSLABELTITLE_H
#define QS_WIDGETS_QSLABELTITLE_H

#include <QLabel>
#include <QsWidgets_Global.h>


class QSWIDGETS_EXPORT QsLabelTitle : public QLabel
{
    Q_OBJECT
	Q_PROPERTY (QString ClassCSS READ getClassCSS WRITE setClassCSS);

public:
    QsLabelTitle(QWidget *parent = 0);
	virtual ~QsLabelTitle();

	QString getClassCSS() const { return _classCSS; }
	void setClassCSS(QString val) { _classCSS = val; }
private:
	QString _classCSS;
};

#endif
