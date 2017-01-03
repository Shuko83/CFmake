

#ifndef QS_WIDGETS_QSCHECKBOX_H
#define QS_WIDGETS_QSCHECKBOX_H

#include <QCheckBox>
#include <QsWidgets_Global.h>


class QSWIDGETS_EXPORT QsCheckBox : public QCheckBox
{
    Q_OBJECT
	Q_PROPERTY (QString ClassCSS READ getClassCSS WRITE setClassCSS);
	Q_PROPERTY (QString HelpText READ getHelpText WRITE setHelpText);

public:
    QsCheckBox(QWidget *parent = 0);
	virtual ~QsCheckBox();

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
