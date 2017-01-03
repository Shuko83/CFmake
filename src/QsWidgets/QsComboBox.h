

#ifndef QS_WIDGETS_QSComboBOX_H
#define QS_WIDGETS_QSComboBOX_H

#include <QComboBox>
#include <QsWidgets_Global.h>


class QSWIDGETS_EXPORT QsComboBox : public QComboBox
{
    Q_OBJECT
	Q_PROPERTY (QString ClassCSS READ getClassCSS WRITE setClassCSS);
	Q_PROPERTY (QString HelpText READ getHelpText WRITE setHelpText);

public:
    QsComboBox(QWidget *parent = 0);
	virtual ~QsComboBox();

	QString getClassCSS() const { return _classCSS; }
	void setClassCSS(QString val) { _classCSS = val; }

	QString getHelpText() const { return _helpText; }
	void setHelpText(QString val) { _helpText = val; }

	virtual bool event(QEvent *event);
	bool eventFilter(QObject *obj, QEvent *event);

private:
	QString _classCSS;
	QString _helpText;

signals:
	void mouseEnter();
	void mouseLeave();
	void comboClosed();
};
	
#endif
