

#ifndef QS_WIDGETS_QSSPINBOX_H
#define QS_WIDGETS_QSSPINBOX_H

#include <QSpinBox>
#include <QsWidgets_Global.h>


class QSWIDGETS_EXPORT QsSpinBox : public QSpinBox
{
    Q_OBJECT
	Q_PROPERTY (QString ClassCSS READ getClassCSS WRITE setClassCSS);
    Q_PROPERTY (QString HelpText READ getHelpText WRITE setHelpText);

public:
    QsSpinBox(QWidget *parent = 0);
	virtual ~QsSpinBox();

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
