#pragma once

#include <QWidgetAction>

class TitleWidgetAction : public QWidgetAction
{

public:	
	TitleWidgetAction(QObject *parent, QString title = "");
	~TitleWidgetAction() = default;

protected:
	QWidget* createWidget(QWidget *parent) override;	

	QString _title;
	QWidget* _widget;

};