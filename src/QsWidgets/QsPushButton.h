

#ifndef QS_WIDGETS_QSPUSHBUTTON_H
#define QS_WIDGETS_QSPUSHBUTTON_H

#include <QPushButton>
#include <QsWidgets_Global.h>

#include <QtUiPlugin/QDesignerExportWidget>

class QSWIDGETS_EXPORT QsPushButton : public QPushButton
{
	Q_OBJECT
	Q_PROPERTY (QString ClassCSS READ getClassCSS WRITE setClassCSS);
	Q_PROPERTY (QString HelpText READ getHelpText WRITE setHelpText);
		
public:
		
	QsPushButton(QWidget *parent = 0);
	virtual ~QsPushButton();
		
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
