#pragma once


#include "SwAssistedComponent.h"
#include "qdialog.h"
#include "qboxlayout.h"
#include "Main/Services/Management/SwServiceManager_Helper.h"
#include "ISwServiceMainWindow.h"
#include "qevent.h"

#define QWIDGET_TO_QDIALOG_COMPONENT "_SwGuiCompQWidgetToQDialog"

class DialogContainer : public QDialog
{
public :
    DialogContainer()
    {
        _layout = new QVBoxLayout( this );
        _layout->setContentsMargins( 0, 0, 0, 0 );
        setLayout( _layout );
        setMinimumSize( 100, 100 );
        _savePositionWhenHidden = false;
        _dialogSavedPosition = QPoint( 0, 0 );
    }
    
    void setContentWidget( QWidget * wid )
    {
        if( wid != 0 )
        {
            //Enregistrement du noluveau
            resize( wid->size() );
            setMinimumSize( wid->minimumSize() );
            setWindowTitle( wid->windowTitle() );
            setWindowIcon( wid->windowIcon() );
            connect( wid, &QWidget::windowTitleChanged, [ = ]( const QString & value ) { setWindowTitle( value ); } );
            _layout->addWidget( wid );
        }
    }
    
    void keyPressEvent( QKeyEvent * evt )
    {
        if( evt->key() == Qt::Key_Enter || evt->key() == Qt::Key_Return || evt->key() == Qt::Key_Escape )
            return;
        QDialog::keyPressEvent( evt );
    }
    
    virtual void setVisible( bool visible ) override
    {
        if( _savePositionWhenHidden && !visible )
        {
            _dialogSavedPosition = mapToGlobal( pos() + QPoint( -geometry().x(), -geometry().y() ) );
        }
        
        QDialog::setVisible( visible );
        
        if( _savePositionWhenHidden && visible )
            move( _dialogSavedPosition );
    }
    
    void setSavePositionWhenHidden( bool value )
    {
        _savePositionWhenHidden = value;
    }
    
private:
    QVBoxLayout * _layout;
    
    bool _savePositionWhenHidden;
    QPoint _dialogSavedPosition;
};


/**
*   @class _SwGuiCompQWidgetToQDialog
*   @brief Charger de convertir un widget en QDialog parenté
*/
class _SwGuiCompQWidgetToQDialog : public StreamWork::SwFoundation::SwAssistedComponent
{
    Q_OBJECT;
    Q_PROPERTY( bool EnableMaximize READ getEnableMaximize WRITE setEnableMaximize );
    bool getEnableMaximize() const { return _enableMaximize; }
    void setEnableMaximize( bool val ) { _enableMaximize = val; }
    
    Q_PROPERTY( bool SaveDialogPosition READ getSaveDialogPosition WRITE setSaveDialogPosition );
    bool getSaveDialogPosition() const { return _saveDialogPosition; }
    void setSaveDialogPosition( bool val ) { _saveDialogPosition = val; _container.setSavePositionWhenHidden( _saveDialogPosition ); }
    
public:

    /** @brief : Constructeur */
    _SwGuiCompQWidgetToQDialog();
    
    /** @brief : Destructeur */
    virtual ~_SwGuiCompQWidgetToQDialog();
    
    /**
    * @brief    : Initialisation du composant
    */
    virtual void initializeComponent() throw(StreamWork::SwCore::SwException );
    
    virtual void interfaceAvailable( QString interfaceName );
    
    virtual void interfaceUnavailable( QString interfaceName );
    
    void onService( bool available );
        
protected:
    DialogContainer _container;
    QWidget * _widget;
    
    bool _enableMaximize;
    bool _saveDialogPosition;
    
    QAction * _action;
    SwServiceManager_Helper<StreamWork::Service::ISwServiceMainWindow> _helper;
};
