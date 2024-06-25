/**
@file _SwGuiCompGridLayoutCell.cpp
@brief ComposantGridLayoutCell
@author Big
*/

#include "_SwGuiCompGridLayoutCell.h"
#include "Properties/ISwProperty.h"

#define WIDGET_NAME "Widget%1"

//-----------------------------------------------------------------------
_SwGuiCompGridLayoutCell::_SwGuiCompGridLayoutCell( int index, ISwProperties * propertiesAccess, ISwInterfaces_Consumer * consumerAccess) : QObject()
{
    QString name = QString( WIDGET_NAME ).arg( index );
    setObjectName( name );
    _propertiesAccess = propertiesAccess;
    _consumerAccess = consumerAccess;
    _isBuild = false;
    _glayout = 0;
    _hWidget = 0;
    _alignment = 0;
    _row = 1;
    _column = 1;
    _rowSpan = 1;
    _columnSpan = 1;
    propertiesAccess->CreatePropertiesForQObject( this, name, true );
    consumerAccess->RegisterConsumedInterface<QWidget>( name, &_hWidget );
}

//-----------------------------------------------------------------------
_SwGuiCompGridLayoutCell::~_SwGuiCompGridLayoutCell()
{
    if( _isBuild )
        destroy();
    QList<ISwProperty *> properties = _propertiesAccess->GetProperties();
    for( int i = 0; i < properties.count(); i++ )
    {
        ISwProperty * p = properties[i];
        if( p->GetRealName().startsWith( objectName() ) )
        {
            _propertiesAccess->DestroyProperty( p->GetRealName() );
        }
    }
    _consumerAccess->UnregisterConsumedInterface( objectName() );
}
//-----------------------------------------------------------------------
void _SwGuiCompGridLayoutCell::setGridLayout( QGridLayout * glayout )
{
    if( _glayout == glayout )
        return;
    if( _isBuild )
        destroy();
    _glayout = glayout;
    if( _glayout != 0 )
    {
        build();
    }
}
//-----------------------------------------------------------------------
unsigned int _SwGuiCompGridLayoutCell::getRow()
{
    return _row;
}

//-----------------------------------------------------------------------
void _SwGuiCompGridLayoutCell::setRow( unsigned int row )
{
    _row = row;
    rebuild();
}

//-----------------------------------------------------------------------
unsigned int _SwGuiCompGridLayoutCell::getColumn()
{
    return _column;
}

//-----------------------------------------------------------------------
void _SwGuiCompGridLayoutCell::setColumn( unsigned int column )
{
    _column = column;
    rebuild();
}

//-----------------------------------------------------------------------
unsigned int _SwGuiCompGridLayoutCell::getRowSpan()
{
    return _rowSpan;
}

//-----------------------------------------------------------------------
void _SwGuiCompGridLayoutCell::setRowSpan( unsigned int rowSpan )
{
    _rowSpan = rowSpan;
    rebuild();
}

//-----------------------------------------------------------------------
unsigned int _SwGuiCompGridLayoutCell::getColumnSpan()
{
    return _columnSpan;
}

//-----------------------------------------------------------------------
void _SwGuiCompGridLayoutCell::setColumnSpan( unsigned int columnSpan )
{
    _columnSpan = columnSpan;
    rebuild();
}

//-----------------------------------------------------------------------
Qt::Alignment _SwGuiCompGridLayoutCell::getAlignment()
{
    return _alignment;
}

//-----------------------------------------------------------------------
void _SwGuiCompGridLayoutCell::setAlignment( Qt::Alignment alignment )
{
    _alignment = alignment;
    rebuild();
}
//-------------------------------------------------------------------------
//Fin Getter setter property
//-------------------------------------------------------------------------

//-----------------------------------------------------------------------
void _SwGuiCompGridLayoutCell::eventBeforeInterfaceAvailability( QString interface_name, SwComponent_Class * provider_host )
{
	if (interface_name == objectName() && _hWidget != 0)
	{
		destroy();
	}
}

//-----------------------------------------------------------------------
void _SwGuiCompGridLayoutCell::eventAfterInterfaceAvailability( QString interface_name, SwComponent_Class * provider_host )
{
	if (interface_name == objectName() && _hWidget != 0)
	{
		build();
	}
}
//-----------------------------------------------------------------------
void _SwGuiCompGridLayoutCell::rebuild()
{
    if( !_isBuild )
        return;
    destroy();
    build();
}
//-----------------------------------------------------------------------
void _SwGuiCompGridLayoutCell::build()
{
    if( _glayout == 0 )
        return;
    if( _hWidget == 0 )
        return;
    //qDebug("Build cell");
     _glayout->addWidget( _hWidget, _row, _column, _rowSpan, _columnSpan, _alignment );
    _isBuild = true;
}
//-----------------------------------------------------------------------
void _SwGuiCompGridLayoutCell::destroy()
{
    if( !_isBuild )
        return;
    // qDebug("Destroy cell");
    if( _hWidget )
		_hWidget->setParent( nullptr );
    _isBuild = false;
}
