#include "MyWidget.h"

//-------------------------------------------------------------------------
MyWidget::MyWidget( QWidget * parent ) : QDialog( parent )
{
    ui.setupUi( this );
    setWindowFlags( Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint );
    
    QString versionName; 
    
    ui.L_Version->setText( versionName );
    
    setWindowIcon( QIcon( ":/Eye.png" ) );
    
    connect( ui.PB_Ok, &QPushButton::clicked, this, &QDialog::accept );
}

//-----------------------------------------------------------------------
MyWidget::~MyWidget()
{
}
