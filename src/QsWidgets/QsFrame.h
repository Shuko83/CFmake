

#ifndef QS_WIDGETS_QSFRAME_H
#define QS_WIDGETS_QSFRAME_H

#include <QFrame>
#include <QsWidgets_Global.h>



class QSWIDGETS_EXPORT QsFrame : public QFrame
{
    Q_OBJECT
	Q_PROPERTY (QString ClassCSS READ getClassCSS WRITE setClassCSS);
	Q_PROPERTY (QString HelpText READ getHelpText WRITE setHelpText);

public:
    QsFrame(QWidget *parent = 0);
	virtual ~QsFrame();

	QString getClassCSS() const { return _classCSS; }
	void setClassCSS(QString val) { _classCSS = val; }

	QString getHelpText() const { return _helpText; }
	void setHelpText(QString val) { _helpText = val; }

	virtual bool event(QEvent *event);
private:
	QString _classCSS;
	QString _helpText;

signals:
	void mouseEnter();
	void mouseLeave();
};

#endif
