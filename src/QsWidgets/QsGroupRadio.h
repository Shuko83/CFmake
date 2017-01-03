
#ifndef QS_WIDGETS_QSGROUPRADIO_H
#define QS_WIDGETS_QSGROUPRADIO_H

#include <QGroupBox>
#include <QRadioButton>
#include <QsWidgets_Global.h>

class QSWIDGETS_EXPORT QsGroupRadio : public QGroupBox
{
	Q_OBJECT
	Q_PROPERTY(QString ClassCSS READ getClassCSS WRITE setClassCSS);
	Q_PROPERTY(QString HelpText READ getHelpText WRITE setHelpText);

public:
	QsGroupRadio(QWidget *parent = 0);
	virtual ~QsGroupRadio();

	virtual void mouseReleaseEvent(QMouseEvent *event);

	QRadioButton * RbAssociated() const { return _rbAssociated; }
	void setRbAssociated(QRadioButton * val) { _rbAssociated = val; }

	QString getClassCSS() const { return _classCSS; }
	void setClassCSS(QString val) { _classCSS = val; }

	QString getHelpText() const { return _helpText; }
	void setHelpText(QString val) { _helpText = val; }

	virtual bool event(QEvent *event);

signals:
	void mouseEnter();
	void mouseLeave();

private:
	QString _classCSS;
	QString _helpText;
	QRadioButton * _rbAssociated;
};

#endif //QS_WIDGETS_QSGROUPRADIO_H
