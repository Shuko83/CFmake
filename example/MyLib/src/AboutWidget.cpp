/**
* Include QT
*/
#include <QFileInfo>

/**
* Include Local
*/
#include "AboutWidget.h"
#include "qdir.h"

//-------------------------------------------------------------------------
AboutWidget::AboutWidget( QWidget * parent ) : QDialog( parent )
{
    ui.setupUi( this );
    setWindowFlags( Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint );
    
    QString versionName; 
    
    ui.L_starlinxVersion->setText( versionName );
    
    if( QCoreApplication::applicationName() == QString( "StreamWorkEditor2" ) )
    {
        QObject::connect( &_releaseNoteWatcher, &QFileSystemWatcher::fileChanged, this, [ = ]( QString file ) { reload(); } );
    }
    
    setWindowIcon( QIcon( ":/SpecificToolBar/About_LIGHT" ) );
    
    connect( ui.PB_Ok, &QPushButton::clicked, this, &QDialog::accept );
    
    reload();
    
}

//-----------------------------------------------------------------------
AboutWidget::~AboutWidget()
{
}

//-----------------------------------------------------------------------
QWidget * AboutWidget::GetWidget()
{
    return ( this );
}

//-----------------------------------------------------------------------
void AboutWidget::reload()
{
    loadFile();
}

//-----------------------------------------------------------------------
void AboutWidget::loadFile()
{
    //To be able to reload the releaseNote in execution ...
    if( QCoreApplication::applicationName() == QString( "StreamWorkEditor2" ) )
    {
        QString debugFileName;
    }
}

//-----------------------------------------------------------------------
void AboutWidget::onService( bool available )
{
    if( available )
    {
        setWindowFlags( Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint );
    }
    else
    {
        setParent( nullptr );
        setWindowFlags( Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint );
    }
}
