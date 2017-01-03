

#ifndef QS_WIDGETS_QSFRAMEBASE_H
#define QS_WIDGETS_QSFRAMEBASE_H

#include <QFrame>
#include <QsWidgets_Global.h>


class QSWIDGETS_EXPORT QsFrameBase : public QFrame
{
    Q_OBJECT
	Q_PROPERTY (QString ClassCSS READ getClassCSS WRITE setClassCSS);

public:
    QsFrameBase(QWidget *parent = 0);
	virtual ~QsFrameBase();

	QString getClassCSS() const { return _classCSS; }
	void setClassCSS(QString val) { _classCSS = val; }
private:
	QString _classCSS;
};

#endif
