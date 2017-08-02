#ifndef WIDGETITEMDELEGATE_H
#define WIDGETITEMDELEGATE_H

#include <QWidget>
#include <QEvent>
#include <QTGui>


namespace Ui {class WidgetItemDelegate;};

class WidgetItemDelegate : public QWidget
{
	Q_OBJECT

public:
	WidgetItemDelegate(QWidget *parent = 0);
	~WidgetItemDelegate();

	void setText(QString val );
	QString getText();

	void enterEvent (QEvent*);
	void leaveEvent (QEvent*);
	void mouseMoveEvent(QMouseEvent*e);

private slots:
		void deleteClicked();
signals :
		void deleteAsked();
private:
	Ui::WidgetItemDelegate *ui;
};

#endif // WIDGETITEMDELEGATE_H
