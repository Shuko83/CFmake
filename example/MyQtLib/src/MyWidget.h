#pragma once

#include <QDialog>

#include "ui_MyWidget.h"

class MyWidget : public QDialog
{
    Q_OBJECT;
    
public:
    MyWidget( QWidget * parent = nullptr );
    
    ~MyWidget();
    
private:
    Ui::MyWidgetClass ui;
};
