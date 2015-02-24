#include "WidgetItemDelegate.h"
#include "ui_WidgetItemDelegate.h"
#include <Qdebug>


WidgetItemDelegate::WidgetItemDelegate(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::WidgetItemDelegate();
	ui->setupUi(this);
	connect(ui->PB_delete,SIGNAL(clicked()),this,SLOT(deleteClicked()));
	ui->PB_delete->hide();
	ui->L_key->hide();
	setStyleSheet("background:transparent");
	setMouseTracking ( true);
}

WidgetItemDelegate::~WidgetItemDelegate()
{
	delete ui;
}

//-------------------------------------------------------------------------
void WidgetItemDelegate::setText( QString val )
{
	ui->L_key->setText(val);
}

//-------------------------------------------------------------------------
void WidgetItemDelegate::deleteClicked()
{
	emit deleteAsked();
}

//-------------------------------------------------------------------------
void WidgetItemDelegate::enterEvent( QEvent*event )
{
	ui->PB_delete->show();
	event->accept();
}

//-------------------------------------------------------------------------
void WidgetItemDelegate::leaveEvent( QEvent*event )
{
	ui->PB_delete->hide();
	event->accept();
}

//-------------------------------------------------------------------------
QString WidgetItemDelegate::getText()
{
	return ui->L_key->text();
}

//-------------------------------------------------------------------------
void WidgetItemDelegate::mouseMoveEvent( QMouseEvent*e )
{
	QWidget::mouseMoveEvent(e);

}
