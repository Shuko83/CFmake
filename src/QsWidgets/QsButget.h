

#ifndef QS_WIDGETS_QSBUTGET_H
#define QS_WIDGETS_QSBUTGET_H

#include <QPushButton>
#include <QsWidgets_Global.h>

class QSWIDGETS_EXPORT QsButget : public QPushButton
{
    Q_OBJECT
	Q_PROPERTY (QString ClassCSS READ getClassCSS WRITE setClassCSS);

public:
    QsButget(QWidget *parent = 0);
	virtual ~QsButget();
	QString getClassCSS() const { return _classCSS; }
	void setClassCSS(QString val) { _classCSS = val; }
private:
	QString _classCSS;
};

#endif
