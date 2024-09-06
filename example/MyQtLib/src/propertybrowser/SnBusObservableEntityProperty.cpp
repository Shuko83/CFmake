#include "SnBusObservableEntityProperty.h"

#include <QSpinBox>
#include <QDoubleSpinBox>

QT_BEGIN_NAMESPACE

using namespace sunset::widgets;

class SnBusArrayPropertyType
{
};

class SnBusEntityPropertyType
{
};

class SnBusExchangePropertyType
{
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE( SnBusArrayPropertyType )
Q_DECLARE_METATYPE( SnBusEntityPropertyType )
Q_DECLARE_METATYPE( SnBusExchangePropertyType )

SnBusObservableEntityPropertyManager::SnBusObservableEntityPropertyManager( QObject * parent )
    : QtVariantPropertyManager( parent )
{
}

SnBusObservableEntityPropertyManager::~SnBusObservableEntityPropertyManager()
{

}

int SnBusObservableEntityPropertyManager::entityTypeId()
{
    return qMetaTypeId< SnBusEntityPropertyType >();
}

int SnBusObservableEntityPropertyManager::arrayTypeId()
{
    return qMetaTypeId< SnBusArrayPropertyType >();
}

int SnBusObservableEntityPropertyManager::exchangeTypeId()
{
    return qMetaTypeId< SnBusExchangePropertyType >();
}

QString SnBusObservableEntityPropertyManager::prefix( const QtProperty * property ) const
{
    if( !_propertyToData.contains( property ) )
        return QString();
    return _propertyToData[property].prefix;
}

QString SnBusObservableEntityPropertyManager::suffix( const QtProperty * property ) const
{
    if( !_propertyToData.contains( property ) )
        return QString();
    return _propertyToData[property].suffix;
}

void SnBusObservableEntityPropertyManager::setPrefix( QtProperty * property, const QString & prefix )
{
    if( !_propertyToData.contains( property ) )
        return;

    SnBusObservableEntityPropertyManager::PrefixSuffixData data = _propertyToData[property];
    if( data.prefix == prefix )
        return;

    data.prefix = prefix;
    _propertyToData[property] = data;

    emit propertyChanged( property );
    emit prefixChanged( property, prefix );
}

void SnBusObservableEntityPropertyManager::setSuffix( QtProperty * property, const QString & suffix )
{
    if( !_propertyToData.contains( property ) )
        return;

    SnBusObservableEntityPropertyManager::PrefixSuffixData data = _propertyToData[property];
    if( data.suffix == suffix )
        return;

    data.suffix = suffix;
    _propertyToData[property] = data;

    emit propertyChanged( property );
    emit suffixChanged( property, suffix );
}

bool SnBusObservableEntityPropertyManager::isPropertyTypeSupported( int propertyType ) const
{
    if( propertyType == exchangeTypeId() || propertyType == entityTypeId() || propertyType == arrayTypeId() )
        return true;
    return QtVariantPropertyManager::isPropertyTypeSupported( propertyType );
}

int SnBusObservableEntityPropertyManager::valueType( int propertyType ) const
{
    if( propertyType == exchangeTypeId() || propertyType == entityTypeId() || propertyType == arrayTypeId() )
        return QVariant::Invalid;
    return QtVariantPropertyManager::valueType( propertyType );
}

void SnBusObservableEntityPropertyManager::setValueText( const QtProperty * property, QString valueText )
{
    if( propertyType( property ) == entityTypeId() || propertyType( property ) == arrayTypeId() )
        _propertyToValueText[property] = valueText;
}

QString SnBusObservableEntityPropertyManager::valueText( const QtProperty * property ) const
{
    QString text;
    if( _propertyToValueText.contains( property ) )
        text = _propertyToValueText[property];
    else
        text = QtVariantPropertyManager::valueText( property );

    if( !_propertyToData.contains( property ) )
        return text;

    SnBusObservableEntityPropertyManager::PrefixSuffixData data = _propertyToData[property];
    text = data.prefix + text + data.suffix;

    return text;
}

bool SnBusObservableEntityPropertyManager::hasValue( const QtProperty * property ) const
{
    if( propertyType( property ) == exchangeTypeId() )
        return false;
    return QtVariantPropertyManager::hasValue( property );
}

void SnBusObservableEntityPropertyManager::setValue( QtProperty * property, const QVariant & val )
{
    QtVariantPropertyManager::setValue( property, val );
}

void SnBusObservableEntityPropertyManager::initializeProperty( QtProperty * property )
{
    _propertyToData[property] = SnBusObservableEntityPropertyManager::PrefixSuffixData();
    QtVariantPropertyManager::initializeProperty( property );
}

void SnBusObservableEntityPropertyManager::uninitializeProperty( QtProperty * property )
{
    _propertyToValueText.remove( property );
    _propertyToData.remove( property );
    QtVariantPropertyManager::uninitializeProperty( property );
}

SnBusObservableEntityPropertyFactory::SnBusObservableEntityPropertyFactory( QObject * parent )
    : QtAbstractEditorFactory< SnBusObservableEntityPropertyManager >( parent )
{
    _originalFactory = new QtVariantEditorFactory( this );
}

SnBusObservableEntityPropertyFactory::~SnBusObservableEntityPropertyFactory()
{
}

void SnBusObservableEntityPropertyFactory::connectPropertyManager( SnBusObservableEntityPropertyManager * manager )
{
    _originalFactory->addPropertyManager( manager );
    connect( manager, SIGNAL( prefixChanged( QtProperty *, const QString & ) ), this, SLOT( slotPrefixChanged( QtProperty *, const QString & ) ) );
    connect( manager, SIGNAL( suffixChanged( QtProperty *, const QString & ) ), this, SLOT( slotSuffixChanged( QtProperty *, const QString & ) ) );
}

QWidget * SnBusObservableEntityPropertyFactory::createEditor( SnBusObservableEntityPropertyManager * manager, QtProperty * property,
        QWidget * parent )
{
    QtAbstractEditorFactoryBase * base = _originalFactory;
    QWidget * w = base->createEditor( property, parent );
    if( !w )
        return 0;

    if( w->inherits( "QSpinBox" ) )
    {
        QSpinBox * spinBox = qobject_cast< QSpinBox * >( w );
        spinBox->setPrefix( manager->prefix( property ) );
        spinBox->setSuffix( manager->suffix( property ) );
        _createdEditors[property].append( spinBox );
        _editorToProperty[spinBox] = property;
    }
    else if( w->inherits( "QDoubleSpinBox" ) )
    {
        QDoubleSpinBox * doubleSpinBox = qobject_cast< QDoubleSpinBox * >( w );
        doubleSpinBox->setPrefix( manager->prefix( property ) );
        doubleSpinBox->setSuffix( manager->suffix( property ) );
        _createdEditors[property].append( doubleSpinBox );
        _editorToProperty[doubleSpinBox] = property;
    }
    else if( w->inherits( "sunset::widgets::SnAbstractSpinBox" ) )
    {
    }

    return w;
}

void SnBusObservableEntityPropertyFactory::disconnectPropertyManager( SnBusObservableEntityPropertyManager * manager )
{
    _originalFactory->removePropertyManager( manager );
    disconnect( manager, SIGNAL( prefixChanged( QtProperty *, const QString & ) ), this, SLOT( slotPrefixChanged( QtProperty *, const QString & ) ) );
    disconnect( manager, SIGNAL( suffixChanged( QtProperty *, const QString & ) ), this, SLOT( slotSuffixChanged( QtProperty *, const QString & ) ) );
}

void SnBusObservableEntityPropertyFactory::slotPrefixChanged( QtProperty * property, const QString & prefix )
{
    if( !_createdEditors.contains( property ) )
        return;

    SnBusObservableEntityPropertyManager * manager = propertyManager( property );
    if( !manager )
        return;

    QList< QWidget * > editors = _createdEditors[property];
    QListIterator< QWidget * > itEditor( editors );
    while( itEditor.hasNext() )
    {
        QWidget * editor = itEditor.next();
        if( editor->inherits( "QSpinBox" ) )
            qobject_cast< QSpinBox * >( editor )->setPrefix( prefix );
        else if( editor->inherits( "QDoubleSpinBox" ) )
            qobject_cast< QDoubleSpinBox * >( editor )->setPrefix( prefix );
    }
}

void SnBusObservableEntityPropertyFactory::slotSuffixChanged( QtProperty * property, const QString & suffix )
{
    if( !_createdEditors.contains( property ) )
        return;

    SnBusObservableEntityPropertyManager * manager = propertyManager( property );
    if( !manager )
        return;

    QList< QWidget * > editors = _createdEditors[property];
    QListIterator< QWidget * > itEditor( editors );
    while( itEditor.hasNext() )
    {
        QWidget * editor = itEditor.next();
        if( editor->inherits( "QSpinBox" ) )
            qobject_cast< QSpinBox * >( editor )->setSuffix( suffix );
        else if( editor->inherits( "QDoubleSpinBox" ) )
            qobject_cast< QDoubleSpinBox * >( editor )->setSuffix( suffix );
    }
}

void SnBusObservableEntityPropertyFactory::slotEditorDestroyed( QObject * object )
{
    QMap< QWidget *, QtProperty * >::ConstIterator itEditor = _editorToProperty.constBegin();
    while( itEditor != _editorToProperty.constEnd() )
    {
        if( itEditor.key() == object )
        {
            QWidget * editor = itEditor.key();
            QtProperty * property = itEditor.value();
            _editorToProperty.remove( editor );
            _createdEditors[property].removeAll( editor );
            if( _createdEditors[property].isEmpty() )
                _createdEditors.remove( property );
            return;
        }
        itEditor++;
    }
}
