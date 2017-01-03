

#ifndef QS_WIDGETS_QSLINEEDIT_H
#define QS_WIDGETS_QSLINEEDIT_H

#include <QLineEdit>
#include <QsWidgets_Global.h>

class QSWIDGETS_EXPORT QsLineEdit : public QLineEdit
{
    Q_OBJECT
	Q_PROPERTY (QString ClassCSS READ getClassCSS WRITE setClassCSS);
	Q_PROPERTY (QString HelpText READ getHelpText WRITE setHelpText);

public:
    QsLineEdit(QWidget *parent = 0);
	virtual ~QsLineEdit();

	QString getClassCSS() const { return _classCSS; }
	void setClassCSS(QString val) { _classCSS = val; }

	QString getHelpText() const { return _helpText; }
	void setHelpText(QString val) { _helpText = val; }

	virtual bool event(QEvent *event);
	void keyPressEvent (QKeyEvent* event);

private:
	QString _classCSS;
	QString _helpText;

signals:
	void mouseEnter();
	void mouseLeave();
	void mousePress();
	void focusOut();
	void enterKeyPress();
};

#endif
