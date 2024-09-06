/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the tools applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qtvariantproperty.h"
#include "qtpropertymanager.h"
#include "qteditorfactory.h"

#include <QVariant>
#include <QIcon>
#include <QDate>
#include <QLocale>

#if defined(Q_CC_MSVC)
    #pragma warning(disable: 4786) /* MS VS 6: truncating debug info after 255 characters */
#endif

QT_BEGIN_NAMESPACE

class QtEnumPropertyType
{
};


class QtFlagPropertyType
{
};


class QtGroupPropertyType
{
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE( QtEnumPropertyType )
Q_DECLARE_METATYPE( QtFlagPropertyType )
Q_DECLARE_METATYPE( QtGroupPropertyType )

QT_BEGIN_NAMESPACE

/*!
    Returns the type id for an enum property.

    Note that the property's value type can be retrieved using the
    valueType() function (which is QVariant::Int for the enum property
    type).

    \sa propertyType(), valueType()
*/
int QtVariantPropertyManager::enumTypeId()
{
    return qMetaTypeId< QtEnumPropertyType >();
}

/*!
    Returns the type id for a flag property.

    Note that the property's value type can be retrieved using the
    valueType() function (which is QVariant::Int for the flag property
    type).

    \sa propertyType(), valueType()
*/
int QtVariantPropertyManager::flagTypeId()
{
    return qMetaTypeId< QtFlagPropertyType >();
}

/*!
    Returns the type id for a group property.

    Note that the property's value type can be retrieved using the
    valueType() function (which is QVariant::Invalid for the group
    property type, since it doesn't provide any value).

    \sa propertyType(), valueType()
*/
int QtVariantPropertyManager::groupTypeId()
{
    return qMetaTypeId< QtGroupPropertyType >();
}

/*!
    Returns the type id for a icon map attribute.

    Note that the property's attribute type can be retrieved using the
    attributeType() function.

    \sa attributeType(), QtEnumPropertyManager::enumIcons()
*/
int QtVariantPropertyManager::iconMapTypeId()
{
    return qMetaTypeId< QtIconMap >();
}

typedef QMap< const QtProperty *, QtProperty * > PropertyMap;
Q_GLOBAL_STATIC( PropertyMap, propertyToWrappedProperty )

static QtProperty * wrappedProperty( QtProperty * property )
{
    return propertyToWrappedProperty()->value( property, 0 );
}

class QtVariantPropertyPrivate
{
public:
    QtVariantPropertyPrivate( QtVariantPropertyManager * m ) : manager( m ) {}
    
    QtVariantPropertyManager * manager;
};

/*!
    \class QtVariantProperty
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtVariantProperty class is a convenience class handling
    QVariant based properties.

    QtVariantProperty provides additional API: A property's type,
    value type, attribute values and current value can easily be
    retrieved using the propertyType(), valueType(), attributeValue()
    and value() functions respectively. In addition, the attribute
    values and the current value can be set using the corresponding
    setValue() and setAttribute() functions.

    For example, instead of writing:

    \snippet doc/src/snippets/code/tools_shared_qtpropertybrowser_qtvariantproperty.cpp 0

    you can write:

    \snippet doc/src/snippets/code/tools_shared_qtpropertybrowser_qtvariantproperty.cpp 1

    QtVariantProperty instances can only be created by the
    QtVariantPropertyManager class.

    \sa QtProperty, QtVariantPropertyManager, QtVariantEditorFactory
*/

/*!
    Creates a variant property using the given \a manager.

    Do not use this constructor to create variant property instances;
    use the QtVariantPropertyManager::addProperty() function
    instead.  This constructor is used internally by the
    QtVariantPropertyManager::createProperty() function.

    \sa QtVariantPropertyManager
*/
QtVariantProperty::QtVariantProperty( QtVariantPropertyManager * manager )
    : QtProperty( manager ), d_ptr( new QtVariantPropertyPrivate( manager ) )
{
}

/*!
    Destroys this property.

    \sa QtProperty::~QtProperty()
*/
QtVariantProperty::~QtVariantProperty()
{
}

/*!
    Returns the property's current value.

    \sa valueType(), setValue()
*/
QVariant QtVariantProperty::value() const
{
    return d_ptr->manager->value( this );
}

/*!
    Returns this property's value for the specified \a attribute.

    QtVariantPropertyManager provides a couple of related functions:
    \l{QtVariantPropertyManager::attributes()}{attributes()} and
    \l{QtVariantPropertyManager::attributeType()}{attributeType()}.

    \sa setAttribute()
*/
QVariant QtVariantProperty::attributeValue( const QString & attribute ) const
{
    return d_ptr->manager->attributeValue( this, attribute );
}

/*!
    Returns the type of this property's value.

    \sa propertyType()
*/
int QtVariantProperty::valueType() const
{
    return d_ptr->manager->valueType( this );
}

/*!
    Returns this property's type.

    QtVariantPropertyManager provides several related functions:
    \l{QtVariantPropertyManager::enumTypeId()}{enumTypeId()},
    \l{QtVariantPropertyManager::flagTypeId()}{flagTypeId()} and
    \l{QtVariantPropertyManager::groupTypeId()}{groupTypeId()}.

    \sa valueType()
*/
int QtVariantProperty::propertyType() const
{
    return d_ptr->manager->propertyType( this );
}

/*!
    Sets the value of this property to \a value.

    The specified \a value must be of the type returned by
    valueType(), or of a type that can be converted to valueType()
    using the QVariant::canConvert() function; otherwise this function
    does nothing.

    \sa value()
*/
void QtVariantProperty::setValue( const QVariant & value )
{
    d_ptr->manager->setValue( this, value );
}

/*!
    Sets the \a attribute of property to \a value.

    QtVariantPropertyManager provides the related
    \l{QtVariantPropertyManager::setAttribute()}{setAttribute()}
    function.

    \sa attributeValue()
*/
void QtVariantProperty::setAttribute( const QString & attribute, const QVariant & value )
{
    d_ptr->manager->setAttribute( this, attribute, value );
}

class QtVariantPropertyManagerPrivate
{
    QtVariantPropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtVariantPropertyManager )
public:
    QtVariantPropertyManagerPrivate();
    
    bool _creatingProperty;
    bool _creatingSubProperties;
    bool _destroyingSubProperties;
    int _propertyType;
    
    void slotValueChanged( QtProperty * property, int val );
    void slotRangeChanged( QtProperty * property, int min, int max );
    void slotSingleStepChanged( QtProperty * property, int step );
    void slotValueChanged( QtProperty * property, unsigned int val );
    void slotRangeChanged( QtProperty * property, unsigned int min, unsigned int max );
    void slotValueChanged( QtProperty * property, long long val );
    void slotRangeChanged( QtProperty * property, long long min, long long max );
    void slotValueChanged( QtProperty * property, unsigned long long val );
    void slotRangeChanged( QtProperty * property, unsigned long long min, unsigned long long max );
    void slotValueChanged( QtProperty * property, double val );
    void slotRangeChanged( QtProperty * property, double min, double max );
    void slotSingleStepChanged( QtProperty * property, double step );
    void slotDecimalsChanged( QtProperty * property, int prec );
    void slotValueChanged( QtProperty * property, bool val );
    void slotValueChanged( QtProperty * property, const QString & val );
    void slotRegExpChanged( QtProperty * property, const QRegExp & regExp );
    void slotLengthChanged( QtProperty * property, int length );
    void slotValueChanged( QtProperty * property, const QDate & val );
    void slotRangeChanged( QtProperty * property, const QDate & min, const QDate & max );
    void slotValueChanged( QtProperty * property, const QTime & val );
    void slotValueChanged( QtProperty * property, const QDateTime & val );
    void slotValueChanged( QtProperty * property, const QKeySequence & val );
    void slotValueChanged( QtProperty * property, const QChar & val );
    void slotValueChanged( QtProperty * property, const QLocale & val );
    void slotValueChanged( QtProperty * property, const QPoint & val );
    void slotValueChanged( QtProperty * property, const QPointF & val );
    void slotValueChanged( QtProperty * property, const QSize & val );
    void slotRangeChanged( QtProperty * property, const QSize & min, const QSize & max );
    void slotValueChanged( QtProperty * property, const QSizeF & val );
    void slotRangeChanged( QtProperty * property, const QSizeF & min, const QSizeF & max );
    void slotValueChanged( QtProperty * property, const QRect & val );
    void slotConstraintChanged( QtProperty * property, const QRect & val );
    void slotValueChanged( QtProperty * property, const QRectF & val );
    void slotConstraintChanged( QtProperty * property, const QRectF & val );
    void slotValueChanged( QtProperty * property, const QColor & val );
    void slotEnumChanged( QtProperty * property, int val );
    void slotEnumNamesChanged( QtProperty * property, const QStringList & enumNames );
    void slotEnumIconsChanged( QtProperty * property, const QMap< int, QIcon > & enumIcons );
    void slotValueChanged( QtProperty * property, const QSizePolicy & val );
    void slotValueChanged( QtProperty * property, const QFont & val );
    void slotValueChanged( QtProperty * property, const QCursor & val );
    void slotFlagChanged( QtProperty * property, int val );
    void slotFlagNamesChanged( QtProperty * property, const QStringList & flagNames );
    void slotPropertyInserted( QtProperty * property, QtProperty * parent, QtProperty * after );
    void slotPropertyRemoved( QtProperty * property, QtProperty * parent );
    
    void valueChanged( QtProperty * property, const QVariant & val );
    
    int internalPropertyToType( QtProperty * property ) const;
    QtVariantProperty * createSubProperty( QtVariantProperty * parent, QtVariantProperty * after,
                                           QtProperty * internal );
    void removeSubProperty( QtVariantProperty * property );
    
    QMap< int, QtAbstractPropertyManager * > _typeToPropertyManager;
    QMap< int, QMap< QString, int > > _typeToAttributeToAttributeType;
    
    QMap< const QtProperty *, QPair< QtVariantProperty *, int > > _propertyToType;
    
    QMap< int, int > _typeToValueType;
    
    
    QMap< QtProperty *, QtVariantProperty * > _internalToProperty;
    
    const QString _constraintAttribute;
    const QString _singleStepAttribute;
    const QString _decimalsAttribute;
    const QString _enumIconsAttribute;
    const QString _enumNamesAttribute;
    const QString _flagNamesAttribute;
    const QString _maximumAttribute;
    const QString _minimumAttribute;
    const QString _regExpAttribute;
    const QString _lengthAttribute;
};

QtVariantPropertyManagerPrivate::QtVariantPropertyManagerPrivate() :
    _constraintAttribute( QLatin1String( "constraint" ) ),
    _singleStepAttribute( QLatin1String( "singleStep" ) ),
    _decimalsAttribute( QLatin1String( "decimals" ) ),
    _enumIconsAttribute( QLatin1String( "enumIcons" ) ),
    _enumNamesAttribute( QLatin1String( "enumNames" ) ),
    _flagNamesAttribute( QLatin1String( "flagNames" ) ),
    _maximumAttribute( QLatin1String( "maximum" ) ),
    _minimumAttribute( QLatin1String( "minimum" ) ),
    _regExpAttribute( QLatin1String( "regExp" ) ),
    _lengthAttribute( QLatin1String( "length" ) )
{
}

int QtVariantPropertyManagerPrivate::internalPropertyToType( QtProperty * property ) const
{
    int type = 0;
    QtAbstractPropertyManager * internPropertyManager = property->propertyManager();
    if( qobject_cast< QtIntPropertyManager * >( internPropertyManager ) )
        type = QVariant::Int;
    else if( qobject_cast< QtUIntPropertyManager * >( internPropertyManager ) )
        type = QVariant::UInt;
    else if( qobject_cast< QtLongLongPropertyManager * >( internPropertyManager ) )
        type = QVariant::LongLong;
    else if( qobject_cast< QtUnsignedLongLongPropertyManager * >( internPropertyManager ) )
        type = QVariant::ULongLong;
    else if( qobject_cast< QtEnumPropertyManager * >( internPropertyManager ) )
        type = QtVariantPropertyManager::enumTypeId();
    else if( qobject_cast< QtBoolPropertyManager * >( internPropertyManager ) )
        type = QVariant::Bool;
    else if( qobject_cast< QtDoublePropertyManager * >( internPropertyManager ) )
        type = QVariant::Double;
    return type;
}

QtVariantProperty * QtVariantPropertyManagerPrivate::createSubProperty( QtVariantProperty * parent,
        QtVariantProperty * after, QtProperty * internal )
{
    int type = internalPropertyToType( internal );
    if( !type )
        return 0;
        
    bool wasCreatingSubProperties = _creatingSubProperties;
    _creatingSubProperties = true;
    
    QtVariantProperty * varChild = q_ptr->addProperty( type, internal->propertyName() );
    
    _creatingSubProperties = wasCreatingSubProperties;
    
    varChild->setPropertyName( internal->propertyName() );
    varChild->setToolTip( internal->toolTip() );
    varChild->setStatusTip( internal->statusTip() );
    varChild->setWhatsThis( internal->whatsThis() );
    
    parent->insertSubProperty( varChild, after );
    
    _internalToProperty[internal] = varChild;
    propertyToWrappedProperty()->insert( varChild, internal );
    return varChild;
}

void QtVariantPropertyManagerPrivate::removeSubProperty( QtVariantProperty * property )
{
    QtProperty * internChild = wrappedProperty( property );
    bool wasDestroyingSubProperties = _destroyingSubProperties;
    _destroyingSubProperties = true;
    delete property;
    _destroyingSubProperties = wasDestroyingSubProperties;
    _internalToProperty.remove( internChild );
    propertyToWrappedProperty()->remove( property );
}

void QtVariantPropertyManagerPrivate::slotPropertyInserted( QtProperty * property,
        QtProperty * parent, QtProperty * after )
{
    if( _creatingProperty )
        return;
        
    QtVariantProperty * varParent = _internalToProperty.value( parent, 0 );
    if( !varParent )
        return;
        
    QtVariantProperty * varAfter = 0;
    if( after )
    {
        varAfter = _internalToProperty.value( after, 0 );
        if( !varAfter )
            return;
    }
    
    createSubProperty( varParent, varAfter, property );
}

void QtVariantPropertyManagerPrivate::slotPropertyRemoved( QtProperty * property, QtProperty * parent )
{
    Q_UNUSED( parent )
    
    QtVariantProperty * varProperty = _internalToProperty.value( property, 0 );
    if( !varProperty )
        return;
        
    removeSubProperty( varProperty );
}

void QtVariantPropertyManagerPrivate::valueChanged( QtProperty * property, const QVariant & val )
{
    QtVariantProperty * varProp = _internalToProperty.value( property, 0 );
    if( !varProp )
        return;
    emit q_ptr->valueChanged( varProp, val );
    emit q_ptr->propertyChanged( varProp );
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, int val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotRangeChanged( QtProperty * property, int min, int max )
{
    if( QtVariantProperty * varProp = _internalToProperty.value( property, 0 ) )
    {
        emit q_ptr->attributeChanged( varProp, _minimumAttribute, QVariant( min ) );
        emit q_ptr->attributeChanged( varProp, _maximumAttribute, QVariant( max ) );
    }
}

void QtVariantPropertyManagerPrivate::slotSingleStepChanged( QtProperty * property, int step )
{
    if( QtVariantProperty * varProp = _internalToProperty.value( property, 0 ) )
        emit q_ptr->attributeChanged( varProp, _singleStepAttribute, QVariant( step ) );
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, unsigned int val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotRangeChanged( QtProperty * property, unsigned int min, unsigned int max )
{
    if( QtVariantProperty * varProp = _internalToProperty.value( property, 0 ) )
    {
        emit q_ptr->attributeChanged( varProp, _minimumAttribute, QVariant( min ) );
        emit q_ptr->attributeChanged( varProp, _maximumAttribute, QVariant( max ) );
    }
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, long long val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotRangeChanged( QtProperty * property, long long min, long long max )
{
    if( QtVariantProperty * varProp = _internalToProperty.value( property, 0 ) )
    {
        emit q_ptr->attributeChanged( varProp, _minimumAttribute, QVariant( min ) );
        emit q_ptr->attributeChanged( varProp, _maximumAttribute, QVariant( max ) );
    }
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, unsigned long long val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotRangeChanged( QtProperty * property, unsigned long long min, unsigned long long max )
{
    if( QtVariantProperty * varProp = _internalToProperty.value( property, 0 ) )
    {
        emit q_ptr->attributeChanged( varProp, _minimumAttribute, QVariant( min ) );
        emit q_ptr->attributeChanged( varProp, _maximumAttribute, QVariant( max ) );
    }
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, double val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotRangeChanged( QtProperty * property, double min, double max )
{
    if( QtVariantProperty * varProp = _internalToProperty.value( property, 0 ) )
    {
        emit q_ptr->attributeChanged( varProp, _minimumAttribute, QVariant( min ) );
        emit q_ptr->attributeChanged( varProp, _maximumAttribute, QVariant( max ) );
    }
}

void QtVariantPropertyManagerPrivate::slotSingleStepChanged( QtProperty * property, double step )
{
    if( QtVariantProperty * varProp = _internalToProperty.value( property, 0 ) )
        emit q_ptr->attributeChanged( varProp, _singleStepAttribute, QVariant( step ) );
}

void QtVariantPropertyManagerPrivate::slotDecimalsChanged( QtProperty * property, int prec )
{
    if( QtVariantProperty * varProp = _internalToProperty.value( property, 0 ) )
        emit q_ptr->attributeChanged( varProp, _decimalsAttribute, QVariant( prec ) );
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, bool val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, const QString & val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotRegExpChanged( QtProperty * property, const QRegExp & regExp )
{
    if( QtVariantProperty * varProp = _internalToProperty.value( property, 0 ) )
        emit q_ptr->attributeChanged( varProp, _regExpAttribute, QVariant( regExp ) );
}

void QtVariantPropertyManagerPrivate::slotLengthChanged( QtProperty * property, int length )
{
    if( QtVariantProperty * varProp = _internalToProperty.value( property, 0 ) )
        emit q_ptr->attributeChanged( varProp, _lengthAttribute, QVariant( length ) );
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, const QDate & val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotRangeChanged( QtProperty * property, const QDate & min, const QDate & max )
{
    if( QtVariantProperty * varProp = _internalToProperty.value( property, 0 ) )
    {
        emit q_ptr->attributeChanged( varProp, _minimumAttribute, QVariant( min ) );
        emit q_ptr->attributeChanged( varProp, _maximumAttribute, QVariant( max ) );
    }
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, const QTime & val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, const QDateTime & val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, const QKeySequence & val )
{
    QVariant v;
    v.setValue( val );
    valueChanged( property, v );
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, const QChar & val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, const QLocale & val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, const QPoint & val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, const QPointF & val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, const QSize & val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotRangeChanged( QtProperty * property, const QSize & min, const QSize & max )
{
    if( QtVariantProperty * varProp = _internalToProperty.value( property, 0 ) )
    {
        emit q_ptr->attributeChanged( varProp, _minimumAttribute, QVariant( min ) );
        emit q_ptr->attributeChanged( varProp, _maximumAttribute, QVariant( max ) );
    }
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, const QSizeF & val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotRangeChanged( QtProperty * property, const QSizeF & min, const QSizeF & max )
{
    if( QtVariantProperty * varProp = _internalToProperty.value( property, 0 ) )
    {
        emit q_ptr->attributeChanged( varProp, _minimumAttribute, QVariant( min ) );
        emit q_ptr->attributeChanged( varProp, _maximumAttribute, QVariant( max ) );
    }
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, const QRect & val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotConstraintChanged( QtProperty * property, const QRect & constraint )
{
    if( QtVariantProperty * varProp = _internalToProperty.value( property, 0 ) )
        emit q_ptr->attributeChanged( varProp, _constraintAttribute, QVariant( constraint ) );
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, const QRectF & val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotConstraintChanged( QtProperty * property, const QRectF & constraint )
{
    if( QtVariantProperty * varProp = _internalToProperty.value( property, 0 ) )
        emit q_ptr->attributeChanged( varProp, _constraintAttribute, QVariant( constraint ) );
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, const QColor & val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotEnumNamesChanged( QtProperty * property, const QStringList & enumNames )
{
    if( QtVariantProperty * varProp = _internalToProperty.value( property, 0 ) )
        emit q_ptr->attributeChanged( varProp, _enumNamesAttribute, QVariant( enumNames ) );
}

void QtVariantPropertyManagerPrivate::slotEnumIconsChanged( QtProperty * property, const QMap< int, QIcon > & enumIcons )
{
    if( QtVariantProperty * varProp = _internalToProperty.value( property, 0 ) )
    {
        QVariant v;
        v.setValue( enumIcons );
        emit q_ptr->attributeChanged( varProp, _enumIconsAttribute, v );
    }
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, const QSizePolicy & val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, const QFont & val )
{
    valueChanged( property, QVariant( val ) );
}

void QtVariantPropertyManagerPrivate::slotValueChanged( QtProperty * property, const QCursor & val )
{
    #ifndef QT_NO_CURSOR
    valueChanged( property, QVariant( val ) );
    #endif
}

void QtVariantPropertyManagerPrivate::slotFlagNamesChanged( QtProperty * property, const QStringList & flagNames )
{
    if( QtVariantProperty * varProp = _internalToProperty.value( property, 0 ) )
        emit q_ptr->attributeChanged( varProp, _flagNamesAttribute, QVariant( flagNames ) );
}

/*!
    \class QtVariantPropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtVariantPropertyManager class provides and manages QVariant based properties.

    QtVariantPropertyManager provides the addProperty() function which
    creates QtVariantProperty objects. The QtVariantProperty class is
    a convenience class handling QVariant based properties inheriting
    QtProperty. A QtProperty object created by a
    QtVariantPropertyManager instance can be converted into a
    QtVariantProperty object using the variantProperty() function.

    The property's value can be retrieved using the value(), and set
    using the setValue() slot. In addition the property's type, and
    the type of its value, can be retrieved using the propertyType()
    and valueType() functions respectively.

    A property's type is a QVariant::Type enumerator value, and
    usually a property's type is the same as its value type. But for
    some properties the types differ, for example for enums, flags and
    group types in which case QtVariantPropertyManager provides the
    enumTypeId(), flagTypeId() and groupTypeId() functions,
    respectively, to identify their property type (the value types are
    QVariant::Int for the enum and flag types, and QVariant::Invalid
    for the group type).

    Use the isPropertyTypeSupported() function to check if a particular
    property type is supported. The currently supported property types
    are:

    \table
    \header
        \li Property Type
        \li Property Type Id
    \row
        \li int
        \li QVariant::Int
    \row
        \li double
        \li QVariant::Double
    \row
        \li bool
        \li QVariant::Bool
    \row
        \li QString
        \li QVariant::String
    \row
        \li QDate
        \li QVariant::Date
    \row
        \li QTime
        \li QVariant::Time
    \row
        \li QDateTime
        \li QVariant::DateTime
    \row
        \li QKeySequence
        \li QVariant::KeySequence
    \row
        \li QChar
        \li QVariant::Char
    \row
        \li QLocale
        \li QVariant::Locale
    \row
        \li QPoint
        \li QVariant::Point
    \row
        \li QPointF
        \li QVariant::PointF
    \row
        \li QSize
        \li QVariant::Size
    \row
        \li QSizeF
        \li QVariant::SizeF
    \row
        \li QRect
        \li QVariant::Rect
    \row
        \li QRectF
        \li QVariant::RectF
    \row
        \li QColor
        \li QVariant::Color
    \row
        \li QSizePolicy
        \li QVariant::SizePolicy
    \row
        \li QFont
        \li QVariant::Font
    \row
        \li QCursor
        \li QVariant::Cursor
    \row
        \li enum
        \li enumTypeId()
    \row
        \li flag
        \li flagTypeId()
    \row
        \li group
        \li groupTypeId()
    \endtable

    Each property type can provide additional attributes,
    e.g. QVariant::Int and QVariant::Double provides minimum and
    maximum values. The currently supported attributes are:

    \table
    \header
        \li Property Type
        \li Attribute Name
        \li Attribute Type
    \row
        \li \c int
        \li minimum
        \li QVariant::Int
    \row
        \li
        \li maximum
        \li QVariant::Int
    \row
        \li
        \li singleStep
        \li QVariant::Int
    \row
        \li \c double
        \li minimum
        \li QVariant::Double
    \row
        \li
        \li maximum
        \li QVariant::Double
    \row
        \li
        \li singleStep
        \li QVariant::Double
    \row
        \li
        \li decimals
        \li QVariant::Int
    \row
        \li QString
        \li regExp
        \li QVariant::RegExp
    \row
        \li QDate
        \li minimum
        \li QVariant::Date
    \row
        \li
        \li maximum
        \li QVariant::Date
    \row
        \li QPointF
        \li decimals
        \li QVariant::Int
    \row
        \li QSize
        \li minimum
        \li QVariant::Size
    \row
        \li
        \li maximum
        \li QVariant::Size
    \row
        \li QSizeF
        \li minimum
        \li QVariant::SizeF
    \row
        \li
        \li maximum
        \li QVariant::SizeF
    \row
        \li
        \li decimals
        \li QVariant::Int
    \row
        \li QRect
        \li constraint
        \li QVariant::Rect
    \row
        \li QRectF
        \li constraint
        \li QVariant::RectF
    \row
        \li
        \li decimals
        \li QVariant::Int
    \row
        \li \c enum
        \li enumNames
        \li QVariant::StringList
    \row
        \li
        \li enumIcons
        \li iconMapTypeId()
    \row
        \li \c flag
        \li flagNames
        \li QVariant::StringList
    \endtable

    The attributes for a given property type can be retrieved using
    the attributes() function. Each attribute has a value type which
    can be retrieved using the attributeType() function, and a value
    accessible through the attributeValue() function. In addition, the
    value can be set using the setAttribute() slot.

    QtVariantManager also provides the valueChanged() signal which is
    emitted whenever a property created by this manager change, and
    the attributeChanged() signal which is emitted whenever an
    attribute of such a property changes.

    \sa QtVariantProperty, QtVariantEditorFactory
*/

/*!
    \fn void QtVariantPropertyManager::valueChanged(QtProperty *property, const QVariant &value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the
    new \a value as parameters.

    \sa setValue()
*/

/*!
    \fn void QtVariantPropertyManager::attributeChanged(QtProperty *property,
                const QString &attribute, const QVariant &value)

    This signal is emitted whenever an attribute of a property created
    by this manager changes its value, passing a pointer to the \a
    property, the \a attribute and the new \a value as parameters.

    \sa setAttribute()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtVariantPropertyManager::QtVariantPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtVariantPropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
    
    d_ptr->_creatingProperty = false;
    d_ptr->_creatingSubProperties = false;
    d_ptr->_destroyingSubProperties = false;
    d_ptr->_propertyType = 0;
    
    // IntPropertyManager
    QtIntPropertyManager * intPropertyManager = new QtIntPropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::Int] = intPropertyManager;
    d_ptr->_typeToAttributeToAttributeType[QVariant::Int][d_ptr->_minimumAttribute] = QVariant::Int;
    d_ptr->_typeToAttributeToAttributeType[QVariant::Int][d_ptr->_maximumAttribute] = QVariant::Int;
    d_ptr->_typeToAttributeToAttributeType[QVariant::Int][d_ptr->_singleStepAttribute] = QVariant::Int;
    d_ptr->_typeToValueType[QVariant::Int] = QVariant::Int;
    connect( intPropertyManager, SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotValueChanged( QtProperty *, int ) ) );
    connect( intPropertyManager, SIGNAL( rangeChanged( QtProperty *, int, int ) ),
             this, SLOT( slotRangeChanged( QtProperty *, int, int ) ) );
    connect( intPropertyManager, SIGNAL( singleStepChanged( QtProperty *, int ) ),
             this, SLOT( slotSingleStepChanged( QtProperty *, int ) ) );
    // UIntPropertyManager
    QtUIntPropertyManager * uintPropertyManager = new QtUIntPropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::UInt] = uintPropertyManager;
    d_ptr->_typeToAttributeToAttributeType[QVariant::UInt][d_ptr->_minimumAttribute] = QVariant::UInt;
    d_ptr->_typeToAttributeToAttributeType[QVariant::UInt][d_ptr->_maximumAttribute] = QVariant::UInt;
    d_ptr->_typeToValueType[QVariant::UInt] = QVariant::UInt;
    connect( uintPropertyManager, SIGNAL( valueChanged( QtProperty *, unsigned int ) ),
             this, SLOT( slotValueChanged( QtProperty *, unsigned int ) ) );
    connect( uintPropertyManager, SIGNAL( rangeChanged( QtProperty *, unsigned int, unsigned int ) ),
             this, SLOT( slotRangeChanged( QtProperty *, unsigned int, unsigned int ) ) );
    // LongLongPropertyManager
    QtLongLongPropertyManager * longlongPropertyManager = new QtLongLongPropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::LongLong] = longlongPropertyManager;
    d_ptr->_typeToAttributeToAttributeType[QVariant::LongLong][d_ptr->_minimumAttribute] = QVariant::LongLong;
    d_ptr->_typeToAttributeToAttributeType[QVariant::LongLong][d_ptr->_maximumAttribute] = QVariant::LongLong;
    d_ptr->_typeToValueType[QVariant::LongLong] = QVariant::LongLong;
    connect( longlongPropertyManager, SIGNAL( valueChanged( QtProperty *, long long ) ),
             this, SLOT( slotValueChanged( QtProperty *, long long ) ) );
    connect( longlongPropertyManager, SIGNAL( rangeChanged( QtProperty *, long long, long long ) ),
             this, SLOT( slotRangeChanged( QtProperty *, long long, long long ) ) );
    // UnsignedLongLongPropertyManager
    QtUnsignedLongLongPropertyManager * ulonglongPropertyManager = new QtUnsignedLongLongPropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::ULongLong] = ulonglongPropertyManager;
    d_ptr->_typeToAttributeToAttributeType[QVariant::ULongLong][d_ptr->_minimumAttribute] = QVariant::ULongLong;
    d_ptr->_typeToAttributeToAttributeType[QVariant::ULongLong][d_ptr->_maximumAttribute] = QVariant::ULongLong;
    d_ptr->_typeToValueType[QVariant::ULongLong] = QVariant::ULongLong;
    connect( ulonglongPropertyManager, SIGNAL( valueChanged( QtProperty *, unsigned long long ) ),
             this, SLOT( slotValueChanged( QtProperty *, unsigned long long ) ) );
    connect( ulonglongPropertyManager, SIGNAL( rangeChanged( QtProperty *, unsigned long long, unsigned long long ) ),
             this, SLOT( slotRangeChanged( QtProperty *, unsigned long long, unsigned long long ) ) );
    // DoublePropertyManager
    QtDoublePropertyManager * doublePropertyManager = new QtDoublePropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::Double] = doublePropertyManager;
    d_ptr->_typeToAttributeToAttributeType[QVariant::Double][d_ptr->_minimumAttribute] =
        QVariant::Double;
    d_ptr->_typeToAttributeToAttributeType[QVariant::Double][d_ptr->_maximumAttribute] =
        QVariant::Double;
    d_ptr->_typeToAttributeToAttributeType[QVariant::Double][d_ptr->_singleStepAttribute] =
        QVariant::Double;
    d_ptr->_typeToAttributeToAttributeType[QVariant::Double][d_ptr->_decimalsAttribute] =
        QVariant::Int;
    d_ptr->_typeToValueType[QVariant::Double] = QVariant::Double;
    connect( doublePropertyManager, SIGNAL( valueChanged( QtProperty *, double ) ),
             this, SLOT( slotValueChanged( QtProperty *, double ) ) );
    connect( doublePropertyManager, SIGNAL( rangeChanged( QtProperty *, double, double ) ),
             this, SLOT( slotRangeChanged( QtProperty *, double, double ) ) );
    connect( doublePropertyManager, SIGNAL( singleStepChanged( QtProperty *, double ) ),
             this, SLOT( slotSingleStepChanged( QtProperty *, double ) ) );
    connect( doublePropertyManager, SIGNAL( decimalsChanged( QtProperty *, int ) ),
             this, SLOT( slotDecimalsChanged( QtProperty *, int ) ) );
    // BoolPropertyManager
    QtBoolPropertyManager * boolPropertyManager = new QtBoolPropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::Bool] = boolPropertyManager;
    d_ptr->_typeToValueType[QVariant::Bool] = QVariant::Bool;
    connect( boolPropertyManager, SIGNAL( valueChanged( QtProperty *, bool ) ),
             this, SLOT( slotValueChanged( QtProperty *, bool ) ) );
    // StringPropertyManager
    QtStringPropertyManager * stringPropertyManager = new QtStringPropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::String] = stringPropertyManager;
    d_ptr->_typeToValueType[QVariant::String] = QVariant::String;
    d_ptr->_typeToAttributeToAttributeType[QVariant::String][d_ptr->_regExpAttribute] =
        QVariant::RegExp;
    d_ptr->_typeToAttributeToAttributeType[QVariant::String][d_ptr->_lengthAttribute] =
        QVariant::Int;
    connect( stringPropertyManager, SIGNAL( valueChanged( QtProperty *, QString ) ),
             this, SLOT( slotValueChanged( QtProperty *, QString ) ) );
    connect( stringPropertyManager, SIGNAL( regExpChanged( QtProperty *, QRegExp ) ),
             this, SLOT( slotRegExpChanged( QtProperty *, QRegExp ) ) );
    connect( stringPropertyManager, SIGNAL( lengthChanged( QtProperty *, int ) ),
             this, SLOT( slotLengthChanged( QtProperty *, int ) ) );
    // DatePropertyManager
    QtDatePropertyManager * datePropertyManager = new QtDatePropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::Date] = datePropertyManager;
    d_ptr->_typeToValueType[QVariant::Date] = QVariant::Date;
    d_ptr->_typeToAttributeToAttributeType[QVariant::Date][d_ptr->_minimumAttribute] =
        QVariant::Date;
    d_ptr->_typeToAttributeToAttributeType[QVariant::Date][d_ptr->_maximumAttribute] =
        QVariant::Date;
    connect( datePropertyManager, SIGNAL( valueChanged( QtProperty *, QDate ) ),
             this, SLOT( slotValueChanged( QtProperty *, QDate ) ) );
    connect( datePropertyManager, SIGNAL( rangeChanged( QtProperty *, QDate, QDate ) ),
             this, SLOT( slotRangeChanged( QtProperty *, QDate, QDate ) ) );
    // TimePropertyManager
    QtTimePropertyManager * timePropertyManager = new QtTimePropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::Time] = timePropertyManager;
    d_ptr->_typeToValueType[QVariant::Time] = QVariant::Time;
    connect( timePropertyManager, SIGNAL( valueChanged( QtProperty *, QTime ) ),
             this, SLOT( slotValueChanged( QtProperty *, QTime ) ) );
    // DateTimePropertyManager
    QtDateTimePropertyManager * dateTimePropertyManager = new QtDateTimePropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::DateTime] = dateTimePropertyManager;
    d_ptr->_typeToValueType[QVariant::DateTime] = QVariant::DateTime;
    connect( dateTimePropertyManager, SIGNAL( valueChanged( QtProperty *, QDateTime ) ),
             this, SLOT( slotValueChanged( QtProperty *, QDateTime ) ) );
    // KeySequencePropertyManager
    QtKeySequencePropertyManager * keySequencePropertyManager = new QtKeySequencePropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::KeySequence] = keySequencePropertyManager;
    d_ptr->_typeToValueType[QVariant::KeySequence] = QVariant::KeySequence;
    connect( keySequencePropertyManager, SIGNAL( valueChanged( QtProperty *, QKeySequence ) ),
             this, SLOT( slotValueChanged( QtProperty *, QKeySequence ) ) );
    // CharPropertyManager
    QtCharPropertyManager * charPropertyManager = new QtCharPropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::Char] = charPropertyManager;
    d_ptr->_typeToValueType[QVariant::Char] = QVariant::Char;
    connect( charPropertyManager, SIGNAL( valueChanged( QtProperty *, QChar ) ),
             this, SLOT( slotValueChanged( QtProperty *, QChar ) ) );
    // LocalePropertyManager
    QtLocalePropertyManager * localePropertyManager = new QtLocalePropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::Locale] = localePropertyManager;
    d_ptr->_typeToValueType[QVariant::Locale] = QVariant::Locale;
    connect( localePropertyManager, SIGNAL( valueChanged( QtProperty *, QLocale ) ),
             this, SLOT( slotValueChanged( QtProperty *, QLocale ) ) );
    connect( localePropertyManager->subEnumPropertyManager(), SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotValueChanged( QtProperty *, int ) ) );
    connect( localePropertyManager, SIGNAL( propertyInserted( QtProperty *, QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyInserted( QtProperty *, QtProperty *, QtProperty * ) ) );
    connect( localePropertyManager, SIGNAL( propertyRemoved( QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyRemoved( QtProperty *, QtProperty * ) ) );
    // PointPropertyManager
    QtPointPropertyManager * pointPropertyManager = new QtPointPropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::Point] = pointPropertyManager;
    d_ptr->_typeToValueType[QVariant::Point] = QVariant::Point;
    connect( pointPropertyManager, SIGNAL( valueChanged( QtProperty *, QPoint ) ),
             this, SLOT( slotValueChanged( QtProperty *, QPoint ) ) );
    connect( pointPropertyManager->subIntPropertyManager(), SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotValueChanged( QtProperty *, int ) ) );
    connect( pointPropertyManager, SIGNAL( propertyInserted( QtProperty *, QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyInserted( QtProperty *, QtProperty *, QtProperty * ) ) );
    connect( pointPropertyManager, SIGNAL( propertyRemoved( QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyRemoved( QtProperty *, QtProperty * ) ) );
    // PointFPropertyManager
    QtPointFPropertyManager * pointFPropertyManager = new QtPointFPropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::PointF] = pointFPropertyManager;
    d_ptr->_typeToValueType[QVariant::PointF] = QVariant::PointF;
    d_ptr->_typeToAttributeToAttributeType[QVariant::PointF][d_ptr->_decimalsAttribute] =
        QVariant::Int;
    connect( pointFPropertyManager, SIGNAL( valueChanged( QtProperty *, QPointF ) ),
             this, SLOT( slotValueChanged( QtProperty *, QPointF ) ) );
    connect( pointFPropertyManager, SIGNAL( decimalsChanged( QtProperty *, int ) ),
             this, SLOT( slotDecimalsChanged( QtProperty *, int ) ) );
    connect( pointFPropertyManager->subDoublePropertyManager(), SIGNAL( valueChanged( QtProperty *, double ) ),
             this, SLOT( slotValueChanged( QtProperty *, double ) ) );
    connect( pointFPropertyManager, SIGNAL( propertyInserted( QtProperty *, QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyInserted( QtProperty *, QtProperty *, QtProperty * ) ) );
    connect( pointFPropertyManager, SIGNAL( propertyRemoved( QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyRemoved( QtProperty *, QtProperty * ) ) );
    // SizePropertyManager
    QtSizePropertyManager * sizePropertyManager = new QtSizePropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::Size] = sizePropertyManager;
    d_ptr->_typeToValueType[QVariant::Size] = QVariant::Size;
    d_ptr->_typeToAttributeToAttributeType[QVariant::Size][d_ptr->_minimumAttribute] =
        QVariant::Size;
    d_ptr->_typeToAttributeToAttributeType[QVariant::Size][d_ptr->_maximumAttribute] =
        QVariant::Size;
    connect( sizePropertyManager, SIGNAL( valueChanged( QtProperty *, QSize ) ),
             this, SLOT( slotValueChanged( QtProperty *, QSize ) ) );
    connect( sizePropertyManager, SIGNAL( rangeChanged( QtProperty *, QSize, QSize ) ),
             this, SLOT( slotRangeChanged( QtProperty *, QSize, QSize ) ) );
    connect( sizePropertyManager->subIntPropertyManager(), SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotValueChanged( QtProperty *, int ) ) );
    connect( sizePropertyManager->subIntPropertyManager(), SIGNAL( rangeChanged( QtProperty *, int, int ) ),
             this, SLOT( slotRangeChanged( QtProperty *, int, int ) ) );
    connect( sizePropertyManager, SIGNAL( propertyInserted( QtProperty *, QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyInserted( QtProperty *, QtProperty *, QtProperty * ) ) );
    connect( sizePropertyManager, SIGNAL( propertyRemoved( QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyRemoved( QtProperty *, QtProperty * ) ) );
    // SizeFPropertyManager
    QtSizeFPropertyManager * sizeFPropertyManager = new QtSizeFPropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::SizeF] = sizeFPropertyManager;
    d_ptr->_typeToValueType[QVariant::SizeF] = QVariant::SizeF;
    d_ptr->_typeToAttributeToAttributeType[QVariant::SizeF][d_ptr->_minimumAttribute] =
        QVariant::SizeF;
    d_ptr->_typeToAttributeToAttributeType[QVariant::SizeF][d_ptr->_maximumAttribute] =
        QVariant::SizeF;
    d_ptr->_typeToAttributeToAttributeType[QVariant::SizeF][d_ptr->_decimalsAttribute] =
        QVariant::Int;
    connect( sizeFPropertyManager, SIGNAL( valueChanged( QtProperty *, QSizeF ) ),
             this, SLOT( slotValueChanged( QtProperty *, QSizeF ) ) );
    connect( sizeFPropertyManager, SIGNAL( rangeChanged( QtProperty *, QSizeF, QSizeF ) ),
             this, SLOT( slotRangeChanged( QtProperty *, QSizeF, QSizeF ) ) );
    connect( sizeFPropertyManager, SIGNAL( decimalsChanged( QtProperty *, int ) ),
             this, SLOT( slotDecimalsChanged( QtProperty *, int ) ) );
    connect( sizeFPropertyManager->subDoublePropertyManager(), SIGNAL( valueChanged( QtProperty *, double ) ),
             this, SLOT( slotValueChanged( QtProperty *, double ) ) );
    connect( sizeFPropertyManager->subDoublePropertyManager(), SIGNAL( rangeChanged( QtProperty *, double, double ) ),
             this, SLOT( slotRangeChanged( QtProperty *, double, double ) ) );
    connect( sizeFPropertyManager, SIGNAL( propertyInserted( QtProperty *, QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyInserted( QtProperty *, QtProperty *, QtProperty * ) ) );
    connect( sizeFPropertyManager, SIGNAL( propertyRemoved( QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyRemoved( QtProperty *, QtProperty * ) ) );
    // RectPropertyManager
    QtRectPropertyManager * rectPropertyManager = new QtRectPropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::Rect] = rectPropertyManager;
    d_ptr->_typeToValueType[QVariant::Rect] = QVariant::Rect;
    d_ptr->_typeToAttributeToAttributeType[QVariant::Rect][d_ptr->_constraintAttribute] =
        QVariant::Rect;
    connect( rectPropertyManager, SIGNAL( valueChanged( QtProperty *, QRect ) ),
             this, SLOT( slotValueChanged( QtProperty *, QRect ) ) );
    connect( rectPropertyManager, SIGNAL( constraintChanged( QtProperty *, QRect ) ),
             this, SLOT( slotConstraintChanged( QtProperty *, QRect ) ) );
    connect( rectPropertyManager->subIntPropertyManager(), SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotValueChanged( QtProperty *, int ) ) );
    connect( rectPropertyManager->subIntPropertyManager(), SIGNAL( rangeChanged( QtProperty *, int, int ) ),
             this, SLOT( slotRangeChanged( QtProperty *, int, int ) ) );
    connect( rectPropertyManager, SIGNAL( propertyInserted( QtProperty *, QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyInserted( QtProperty *, QtProperty *, QtProperty * ) ) );
    connect( rectPropertyManager, SIGNAL( propertyRemoved( QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyRemoved( QtProperty *, QtProperty * ) ) );
    // RectFPropertyManager
    QtRectFPropertyManager * rectFPropertyManager = new QtRectFPropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::RectF] = rectFPropertyManager;
    d_ptr->_typeToValueType[QVariant::RectF] = QVariant::RectF;
    d_ptr->_typeToAttributeToAttributeType[QVariant::RectF][d_ptr->_constraintAttribute] =
        QVariant::RectF;
    d_ptr->_typeToAttributeToAttributeType[QVariant::RectF][d_ptr->_decimalsAttribute] =
        QVariant::Int;
    connect( rectFPropertyManager, SIGNAL( valueChanged( QtProperty *, QRectF ) ),
             this, SLOT( slotValueChanged( QtProperty *, QRectF ) ) );
    connect( rectFPropertyManager, SIGNAL( constraintChanged( QtProperty *, QRectF ) ),
             this, SLOT( slotConstraintChanged( QtProperty *, QRectF ) ) );
    connect( rectFPropertyManager, SIGNAL( decimalsChanged( QtProperty *, int ) ),
             this, SLOT( slotDecimalsChanged( QtProperty *, int ) ) );
    connect( rectFPropertyManager->subDoublePropertyManager(), SIGNAL( valueChanged( QtProperty *, double ) ),
             this, SLOT( slotValueChanged( QtProperty *, double ) ) );
    connect( rectFPropertyManager->subDoublePropertyManager(), SIGNAL( rangeChanged( QtProperty *, double, double ) ),
             this, SLOT( slotRangeChanged( QtProperty *, double, double ) ) );
    connect( rectFPropertyManager, SIGNAL( propertyInserted( QtProperty *, QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyInserted( QtProperty *, QtProperty *, QtProperty * ) ) );
    connect( rectFPropertyManager, SIGNAL( propertyRemoved( QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyRemoved( QtProperty *, QtProperty * ) ) );
    // ColorPropertyManager
    QtColorPropertyManager * colorPropertyManager = new QtColorPropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::Color] = colorPropertyManager;
    d_ptr->_typeToValueType[QVariant::Color] = QVariant::Color;
    connect( colorPropertyManager, SIGNAL( valueChanged( QtProperty *, QColor ) ),
             this, SLOT( slotValueChanged( QtProperty *, QColor ) ) );
    connect( colorPropertyManager->subIntPropertyManager(), SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotValueChanged( QtProperty *, int ) ) );
    connect( colorPropertyManager, SIGNAL( propertyInserted( QtProperty *, QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyInserted( QtProperty *, QtProperty *, QtProperty * ) ) );
    connect( colorPropertyManager, SIGNAL( propertyRemoved( QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyRemoved( QtProperty *, QtProperty * ) ) );
    // EnumPropertyManager
    int enumId = enumTypeId();
    QtEnumPropertyManager * enumPropertyManager = new QtEnumPropertyManager( this );
    d_ptr->_typeToPropertyManager[enumId] = enumPropertyManager;
    d_ptr->_typeToValueType[enumId] = QVariant::Int;
    d_ptr->_typeToAttributeToAttributeType[enumId][d_ptr->_enumNamesAttribute] =
        QVariant::StringList;
    d_ptr->_typeToAttributeToAttributeType[enumId][d_ptr->_enumIconsAttribute] =
        iconMapTypeId();
    connect( enumPropertyManager, SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotValueChanged( QtProperty *, int ) ) );
    connect( enumPropertyManager, SIGNAL( enumNamesChanged( QtProperty *, QStringList ) ),
             this, SLOT( slotEnumNamesChanged( QtProperty *, QStringList ) ) );
    connect( enumPropertyManager, SIGNAL( enumIconsChanged( QtProperty *, QMap< int, QIcon > ) ),
             this, SLOT( slotEnumIconsChanged( QtProperty *, QMap< int, QIcon > ) ) );
    // SizePolicyPropertyManager
    QtSizePolicyPropertyManager * sizePolicyPropertyManager = new QtSizePolicyPropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::SizePolicy] = sizePolicyPropertyManager;
    d_ptr->_typeToValueType[QVariant::SizePolicy] = QVariant::SizePolicy;
    connect( sizePolicyPropertyManager, SIGNAL( valueChanged( QtProperty *, QSizePolicy ) ),
             this, SLOT( slotValueChanged( QtProperty *, QSizePolicy ) ) );
    connect( sizePolicyPropertyManager->subIntPropertyManager(), SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotValueChanged( QtProperty *, int ) ) );
    connect( sizePolicyPropertyManager->subIntPropertyManager(), SIGNAL( rangeChanged( QtProperty *, int, int ) ),
             this, SLOT( slotRangeChanged( QtProperty *, int, int ) ) );
    connect( sizePolicyPropertyManager->subEnumPropertyManager(), SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotValueChanged( QtProperty *, int ) ) );
    connect( sizePolicyPropertyManager->subEnumPropertyManager(),
             SIGNAL( enumNamesChanged( QtProperty *, QStringList ) ),
             this, SLOT( slotEnumNamesChanged( QtProperty *, QStringList ) ) );
    connect( sizePolicyPropertyManager, SIGNAL( propertyInserted( QtProperty *, QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyInserted( QtProperty *, QtProperty *, QtProperty * ) ) );
    connect( sizePolicyPropertyManager, SIGNAL( propertyRemoved( QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyRemoved( QtProperty *, QtProperty * ) ) );
    // FontPropertyManager
    QtFontPropertyManager * fontPropertyManager = new QtFontPropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::Font] = fontPropertyManager;
    d_ptr->_typeToValueType[QVariant::Font] = QVariant::Font;
    connect( fontPropertyManager, SIGNAL( valueChanged( QtProperty *, QFont ) ),
             this, SLOT( slotValueChanged( QtProperty *, QFont ) ) );
    connect( fontPropertyManager->subIntPropertyManager(), SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotValueChanged( QtProperty *, int ) ) );
    connect( fontPropertyManager->subIntPropertyManager(), SIGNAL( rangeChanged( QtProperty *, int, int ) ),
             this, SLOT( slotRangeChanged( QtProperty *, int, int ) ) );
    connect( fontPropertyManager->subEnumPropertyManager(), SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotValueChanged( QtProperty *, int ) ) );
    connect( fontPropertyManager->subEnumPropertyManager(),
             SIGNAL( enumNamesChanged( QtProperty *, QStringList ) ),
             this, SLOT( slotEnumNamesChanged( QtProperty *, QStringList ) ) );
    connect( fontPropertyManager->subBoolPropertyManager(), SIGNAL( valueChanged( QtProperty *, bool ) ),
             this, SLOT( slotValueChanged( QtProperty *, bool ) ) );
    connect( fontPropertyManager, SIGNAL( propertyInserted( QtProperty *, QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyInserted( QtProperty *, QtProperty *, QtProperty * ) ) );
    connect( fontPropertyManager, SIGNAL( propertyRemoved( QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyRemoved( QtProperty *, QtProperty * ) ) );
    // CursorPropertyManager
    QtCursorPropertyManager * cursorPropertyManager = new QtCursorPropertyManager( this );
    d_ptr->_typeToPropertyManager[QVariant::Cursor] = cursorPropertyManager;
    d_ptr->_typeToValueType[QVariant::Cursor] = QVariant::Cursor;
    connect( cursorPropertyManager, SIGNAL( valueChanged( QtProperty *, QCursor ) ),
             this, SLOT( slotValueChanged( QtProperty *, QCursor ) ) );
    // FlagPropertyManager
    int flagId = flagTypeId();
    QtFlagPropertyManager * flagPropertyManager = new QtFlagPropertyManager( this );
    d_ptr->_typeToPropertyManager[flagId] = flagPropertyManager;
    d_ptr->_typeToValueType[flagId] = QVariant::Int;
    d_ptr->_typeToAttributeToAttributeType[flagId][d_ptr->_flagNamesAttribute] =
        QVariant::StringList;
    connect( flagPropertyManager, SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotValueChanged( QtProperty *, int ) ) );
    connect( flagPropertyManager, SIGNAL( flagNamesChanged( QtProperty *, QStringList ) ),
             this, SLOT( slotFlagNamesChanged( QtProperty *, QStringList ) ) );
    connect( flagPropertyManager->subBoolPropertyManager(), SIGNAL( valueChanged( QtProperty *, bool ) ),
             this, SLOT( slotValueChanged( QtProperty *, bool ) ) );
    connect( flagPropertyManager, SIGNAL( propertyInserted( QtProperty *, QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyInserted( QtProperty *, QtProperty *, QtProperty * ) ) );
    connect( flagPropertyManager, SIGNAL( propertyRemoved( QtProperty *, QtProperty * ) ),
             this, SLOT( slotPropertyRemoved( QtProperty *, QtProperty * ) ) );
    // FlagPropertyManager
    int groupId = groupTypeId();
    QtGroupPropertyManager * groupPropertyManager = new QtGroupPropertyManager( this );
    d_ptr->_typeToPropertyManager[groupId] = groupPropertyManager;
    d_ptr->_typeToValueType[groupId] = QVariant::Invalid;
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtVariantPropertyManager::~QtVariantPropertyManager()
{
    clear();
}

/*!
    Returns the given \a property converted into a QtVariantProperty.

    If the \a property was not created by this variant manager, the
    function returns 0.

    \sa createProperty()
*/
QtVariantProperty * QtVariantPropertyManager::variantProperty( const QtProperty * property ) const
{
    const QMap< const QtProperty *, QPair< QtVariantProperty *, int > >::const_iterator it = d_ptr->_propertyToType.constFind( property );
    if( it == d_ptr->_propertyToType.constEnd() )
        return 0;
    return it.value().first;
}

/*!
    Returns true if the given \a propertyType is supported by this
    variant manager; otherwise false.

    \sa propertyType()
*/
bool QtVariantPropertyManager::isPropertyTypeSupported( int propertyType ) const
{
    if( d_ptr->_typeToValueType.contains( propertyType ) )
        return true;
    return false;
}

/*!
    Creates and returns a variant property of the given \a propertyType
    with the given \a name.

    If the specified \a propertyType is not supported by this variant
    manager, this function returns 0.

    Do not use the inherited
    QtAbstractPropertyManager::addProperty() function to create a
    variant property (that function will always return 0 since it will
    not be clear what type the property should have).

    \sa isPropertyTypeSupported()
*/
QtVariantProperty * QtVariantPropertyManager::addProperty( int propertyType, const QString & name )
{
    if( !isPropertyTypeSupported( propertyType ) )
        return 0;
        
    bool wasCreating = d_ptr->_creatingProperty;
    d_ptr->_creatingProperty = true;
    d_ptr->_propertyType = propertyType;
    QtProperty * property = QtAbstractPropertyManager::addProperty( name );
    d_ptr->_creatingProperty = wasCreating;
    d_ptr->_propertyType = 0;
    
    if( !property )
        return 0;
        
    return variantProperty( property );
}

/*!
    Returns the given \a property's value.

    If the given \a property is not managed by this manager, this
    function returns an invalid variant.

    \sa setValue()
*/
QVariant QtVariantPropertyManager::value( const QtProperty * property ) const
{
    QtProperty * internProp = propertyToWrappedProperty()->value( property, 0 );
    if( internProp == 0 )
        return QVariant();
        
    QtAbstractPropertyManager * manager = internProp->propertyManager();
    if( QtIntPropertyManager * intManager = qobject_cast< QtIntPropertyManager * >( manager ) )
        return intManager->value( internProp );
    else if( QtUIntPropertyManager * uintManager = qobject_cast< QtUIntPropertyManager * >( manager ) )
        return uintManager->value( internProp );
    else if( QtLongLongPropertyManager * longlongManager = qobject_cast< QtLongLongPropertyManager * >( manager ) )
        return longlongManager->value( internProp );
    else if( QtUnsignedLongLongPropertyManager * ulonglongManager = qobject_cast< QtUnsignedLongLongPropertyManager * >( manager ) )
        return ulonglongManager->value( internProp );
    else if( QtDoublePropertyManager * doubleManager = qobject_cast< QtDoublePropertyManager * >( manager ) )
        return doubleManager->value( internProp );
    else if( QtBoolPropertyManager * boolManager = qobject_cast< QtBoolPropertyManager * >( manager ) )
        return boolManager->value( internProp );
    else if( QtStringPropertyManager * stringManager = qobject_cast< QtStringPropertyManager * >( manager ) )
        return stringManager->value( internProp );
    else if( QtDatePropertyManager * dateManager = qobject_cast< QtDatePropertyManager * >( manager ) )
        return dateManager->value( internProp );
    else if( QtTimePropertyManager * timeManager = qobject_cast< QtTimePropertyManager * >( manager ) )
        return timeManager->value( internProp );
    else if( QtDateTimePropertyManager * dateTimeManager = qobject_cast< QtDateTimePropertyManager * >( manager ) )
        return dateTimeManager->value( internProp );
    else if( QtKeySequencePropertyManager * keySequenceManager = qobject_cast< QtKeySequencePropertyManager * >( manager ) )
        return QVariant::fromValue( keySequenceManager->value( internProp ) );
    else if( QtCharPropertyManager * charManager = qobject_cast< QtCharPropertyManager * >( manager ) )
        return charManager->value( internProp );
    else if( QtLocalePropertyManager * localeManager = qobject_cast< QtLocalePropertyManager * >( manager ) )
        return localeManager->value( internProp );
    else if( QtPointPropertyManager * pointManager = qobject_cast< QtPointPropertyManager * >( manager ) )
        return pointManager->value( internProp );
    else if( QtPointFPropertyManager * pointFManager = qobject_cast< QtPointFPropertyManager * >( manager ) )
        return pointFManager->value( internProp );
    else if( QtSizePropertyManager * sizeManager = qobject_cast< QtSizePropertyManager * >( manager ) )
        return sizeManager->value( internProp );
    else if( QtSizeFPropertyManager * sizeFManager = qobject_cast< QtSizeFPropertyManager * >( manager ) )
        return sizeFManager->value( internProp );
    else if( QtRectPropertyManager * rectManager = qobject_cast< QtRectPropertyManager * >( manager ) )
        return rectManager->value( internProp );
    else if( QtRectFPropertyManager * rectFManager = qobject_cast< QtRectFPropertyManager * >( manager ) )
        return rectFManager->value( internProp );
    else if( QtColorPropertyManager * colorManager = qobject_cast< QtColorPropertyManager * >( manager ) )
        return colorManager->value( internProp );
    else if( QtEnumPropertyManager * enumManager = qobject_cast< QtEnumPropertyManager * >( manager ) )
        return enumManager->value( internProp );
    else if( QtSizePolicyPropertyManager * sizePolicyManager =
                 qobject_cast< QtSizePolicyPropertyManager * >( manager ) )
        return sizePolicyManager->value( internProp );
    else if( QtFontPropertyManager * fontManager = qobject_cast< QtFontPropertyManager * >( manager ) )
    {
        return fontManager->value( internProp );
        #ifndef QT_NO_CURSOR
    }
    else if( QtCursorPropertyManager * cursorManager = qobject_cast< QtCursorPropertyManager * >( manager ) )
    {
        return cursorManager->value( internProp );
        #endif
    }
    else if( QtFlagPropertyManager * flagManager = qobject_cast< QtFlagPropertyManager * >( manager ) )
        return flagManager->value( internProp );
    return QVariant();
}

/*!
    Returns the given \a property's value type.

    \sa propertyType()
*/
int QtVariantPropertyManager::valueType( const QtProperty * property ) const
{
    int propType = propertyType( property );
    return valueType( propType );
}

/*!
    \overload

    Returns the value type associated with the given \a propertyType.
*/
int QtVariantPropertyManager::valueType( int propertyType ) const
{
    if( d_ptr->_typeToValueType.contains( propertyType ) )
        return d_ptr->_typeToValueType[propertyType];
    return 0;
}

/*!
    Returns the given \a property's type.

    \sa valueType()
*/
int QtVariantPropertyManager::propertyType( const QtProperty * property ) const
{
    const QMap< const QtProperty *, QPair< QtVariantProperty *, int > >::const_iterator it = d_ptr->_propertyToType.constFind( property );
    if( it == d_ptr->_propertyToType.constEnd() )
        return 0;
    return it.value().second;
}

/*!
    Returns the given \a property's value for the specified \a
    attribute

    If the given \a property was not created by \e this manager, or if
    the specified \a attribute does not exist, this function returns
    an invalid variant.

    \sa attributes(), attributeType(), setAttribute()
*/
QVariant QtVariantPropertyManager::attributeValue( const QtProperty * property, const QString & attribute ) const
{
    int propType = propertyType( property );
    if( !propType )
        return QVariant();
        
    QMap< int, QMap< QString, int > >::ConstIterator it =
        d_ptr->_typeToAttributeToAttributeType.find( propType );
    if( it == d_ptr->_typeToAttributeToAttributeType.constEnd() )
        return QVariant();
        
    QMap< QString, int > attributes = it.value();
    QMap< QString, int >::ConstIterator itAttr = attributes.find( attribute );
    if( itAttr == attributes.constEnd() )
        return QVariant();
        
    QtProperty * internProp = propertyToWrappedProperty()->value( property, 0 );
    if( internProp == 0 )
        return QVariant();
        
    QtAbstractPropertyManager * manager = internProp->propertyManager();
    if( QtIntPropertyManager * intManager = qobject_cast< QtIntPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_maximumAttribute )
            return intManager->maximum( internProp );
        if( attribute == d_ptr->_minimumAttribute )
            return intManager->minimum( internProp );
        if( attribute == d_ptr->_singleStepAttribute )
            return intManager->singleStep( internProp );
        return QVariant();
    }
    else if( QtUIntPropertyManager * uintManager = qobject_cast< QtUIntPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_maximumAttribute )
            return uintManager->maximum( internProp );
        if( attribute == d_ptr->_minimumAttribute )
            return uintManager->minimum( internProp );
        return QVariant();
    }
    else if( QtLongLongPropertyManager * longlongManager = qobject_cast< QtLongLongPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_maximumAttribute )
            return longlongManager->maximum( internProp );
        if( attribute == d_ptr->_minimumAttribute )
            return longlongManager->minimum( internProp );
        return QVariant();
    }
    else if( QtUnsignedLongLongPropertyManager * ulonglongManager = qobject_cast< QtUnsignedLongLongPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_maximumAttribute )
            return ulonglongManager->maximum( internProp );
        if( attribute == d_ptr->_minimumAttribute )
            return ulonglongManager->minimum( internProp );
        return QVariant();
    }
    else if( QtDoublePropertyManager * doubleManager = qobject_cast< QtDoublePropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_maximumAttribute )
            return doubleManager->maximum( internProp );
        if( attribute == d_ptr->_minimumAttribute )
            return doubleManager->minimum( internProp );
        if( attribute == d_ptr->_singleStepAttribute )
            return doubleManager->singleStep( internProp );
        if( attribute == d_ptr->_decimalsAttribute )
            return doubleManager->decimals( internProp );
        return QVariant();
    }
    else if( QtStringPropertyManager * stringManager = qobject_cast< QtStringPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_regExpAttribute )
            return stringManager->regExp( internProp );
        if( attribute == d_ptr->_lengthAttribute )
            return stringManager->length( internProp );
        return QVariant();
    }
    else if( QtDatePropertyManager * dateManager = qobject_cast< QtDatePropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_maximumAttribute )
            return dateManager->maximum( internProp );
        if( attribute == d_ptr->_minimumAttribute )
            return dateManager->minimum( internProp );
        return QVariant();
    }
    else if( QtPointFPropertyManager * pointFManager = qobject_cast< QtPointFPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_decimalsAttribute )
            return pointFManager->decimals( internProp );
        return QVariant();
    }
    else if( QtSizePropertyManager * sizeManager = qobject_cast< QtSizePropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_maximumAttribute )
            return sizeManager->maximum( internProp );
        if( attribute == d_ptr->_minimumAttribute )
            return sizeManager->minimum( internProp );
        return QVariant();
    }
    else if( QtSizeFPropertyManager * sizeFManager = qobject_cast< QtSizeFPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_maximumAttribute )
            return sizeFManager->maximum( internProp );
        if( attribute == d_ptr->_minimumAttribute )
            return sizeFManager->minimum( internProp );
        if( attribute == d_ptr->_decimalsAttribute )
            return sizeFManager->decimals( internProp );
        return QVariant();
    }
    else if( QtRectPropertyManager * rectManager = qobject_cast< QtRectPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_constraintAttribute )
            return rectManager->constraint( internProp );
        return QVariant();
    }
    else if( QtRectFPropertyManager * rectFManager = qobject_cast< QtRectFPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_constraintAttribute )
            return rectFManager->constraint( internProp );
        if( attribute == d_ptr->_decimalsAttribute )
            return rectFManager->decimals( internProp );
        return QVariant();
    }
    else if( QtEnumPropertyManager * enumManager = qobject_cast< QtEnumPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_enumNamesAttribute )
            return enumManager->enumNames( internProp );
        if( attribute == d_ptr->_enumIconsAttribute )
        {
            QVariant v;
            v.setValue( enumManager->enumIcons( internProp ) );
            return v;
        }
        return QVariant();
    }
    else if( QtFlagPropertyManager * flagManager = qobject_cast< QtFlagPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_flagNamesAttribute )
            return flagManager->flagNames( internProp );
        return QVariant();
    }
    return QVariant();
}

/*!
    Returns a list of the given \a propertyType 's attributes.

    \sa attributeValue(), attributeType()
*/
QStringList QtVariantPropertyManager::attributes( int propertyType ) const
{
    QMap< int, QMap< QString, int > >::ConstIterator it =
        d_ptr->_typeToAttributeToAttributeType.find( propertyType );
    if( it == d_ptr->_typeToAttributeToAttributeType.constEnd() )
        return QStringList();
    return it.value().keys();
}

/*!
    Returns the type of the specified \a attribute of the given \a
    propertyType.

    If the given \a propertyType is not supported by \e this manager,
    or if the given \a propertyType does not possess the specified \a
    attribute, this function returns QVariant::Invalid.

    \sa attributes(), valueType()
*/
int QtVariantPropertyManager::attributeType( int propertyType, const QString & attribute ) const
{
    QMap< int, QMap< QString, int > >::ConstIterator it =
        d_ptr->_typeToAttributeToAttributeType.find( propertyType );
    if( it == d_ptr->_typeToAttributeToAttributeType.constEnd() )
        return 0;
        
    QMap< QString, int > attributes = it.value();
    QMap< QString, int >::ConstIterator itAttr = attributes.find( attribute );
    if( itAttr == attributes.constEnd() )
        return 0;
    return itAttr.value();
}

/*!
    \fn void QtVariantPropertyManager::setValue(QtProperty *property, const QVariant &value)

    Sets the value of the given \a property to \a value.

    The specified \a value must be of a type returned by valueType(),
    or of type that can be converted to valueType() using the
    QVariant::canConvert() function, otherwise this function does
    nothing.

    \sa value(), QtVariantProperty::setValue(), valueChanged()
*/
void QtVariantPropertyManager::setValue( QtProperty * property, const QVariant & val )
{
    int propType = val.userType();
    if( !propType )
        return;
        
    int valType = valueType( property );
    
    if( propType != valType && !val.canConvert( static_cast< QVariant::Type >( valType ) ) )
        return;
        
    QtProperty * internProp = propertyToWrappedProperty()->value( property, 0 );
    if( internProp == 0 )
        return;
        
        
    QtAbstractPropertyManager * manager = internProp->propertyManager();
    if( QtIntPropertyManager * intManager = qobject_cast< QtIntPropertyManager * >( manager ) )
    {
        intManager->setValue( internProp, qvariant_cast< int >( val ) );
        return;
    }
    else if( QtUIntPropertyManager * uintManager = qobject_cast< QtUIntPropertyManager * >( manager ) )
    {
        uintManager->setValue( internProp, qvariant_cast< unsigned int >( val ) );
        return;
    }
    else if( QtLongLongPropertyManager * longlongManager = qobject_cast< QtLongLongPropertyManager * >( manager ) )
    {
        longlongManager->setValue( internProp, qvariant_cast< long long >( val ) );
        return;
    }
    else if( QtUnsignedLongLongPropertyManager * ulonglongManager = qobject_cast< QtUnsignedLongLongPropertyManager * >( manager ) )
    {
        ulonglongManager->setValue( internProp, qvariant_cast< unsigned long long >( val ) );
        return;
    }
    else if( QtDoublePropertyManager * doubleManager = qobject_cast< QtDoublePropertyManager * >( manager ) )
    {
        doubleManager->setValue( internProp, qvariant_cast< double >( val ) );
        return;
    }
    else if( QtBoolPropertyManager * boolManager = qobject_cast< QtBoolPropertyManager * >( manager ) )
    {
        boolManager->setValue( internProp, qvariant_cast< bool >( val ) );
        return;
    }
    else if( QtStringPropertyManager * stringManager = qobject_cast< QtStringPropertyManager * >( manager ) )
    {
        stringManager->setValue( internProp, qvariant_cast< QString >( val ) );
        return;
    }
    else if( QtDatePropertyManager * dateManager = qobject_cast< QtDatePropertyManager * >( manager ) )
    {
        dateManager->setValue( internProp, qvariant_cast< QDate >( val ) );
        return;
    }
    else if( QtTimePropertyManager * timeManager = qobject_cast< QtTimePropertyManager * >( manager ) )
    {
        timeManager->setValue( internProp, qvariant_cast< QTime >( val ) );
        return;
    }
    else if( QtDateTimePropertyManager * dateTimeManager = qobject_cast< QtDateTimePropertyManager * >( manager ) )
    {
        dateTimeManager->setValue( internProp, qvariant_cast< QDateTime >( val ) );
        return;
    }
    else if( QtKeySequencePropertyManager * keySequenceManager = qobject_cast< QtKeySequencePropertyManager * >( manager ) )
    {
        keySequenceManager->setValue( internProp, qvariant_cast< QKeySequence >( val ) );
        return;
    }
    else if( QtCharPropertyManager * charManager = qobject_cast< QtCharPropertyManager * >( manager ) )
    {
        charManager->setValue( internProp, qvariant_cast< QChar >( val ) );
        return;
    }
    else if( QtLocalePropertyManager * localeManager = qobject_cast< QtLocalePropertyManager * >( manager ) )
    {
        localeManager->setValue( internProp, qvariant_cast< QLocale >( val ) );
        return;
    }
    else if( QtPointPropertyManager * pointManager = qobject_cast< QtPointPropertyManager * >( manager ) )
    {
        pointManager->setValue( internProp, qvariant_cast< QPoint >( val ) );
        return;
    }
    else if( QtPointFPropertyManager * pointFManager = qobject_cast< QtPointFPropertyManager * >( manager ) )
    {
        pointFManager->setValue( internProp, qvariant_cast< QPointF >( val ) );
        return;
    }
    else if( QtSizePropertyManager * sizeManager = qobject_cast< QtSizePropertyManager * >( manager ) )
    {
        sizeManager->setValue( internProp, qvariant_cast< QSize >( val ) );
        return;
    }
    else if( QtSizeFPropertyManager * sizeFManager = qobject_cast< QtSizeFPropertyManager * >( manager ) )
    {
        sizeFManager->setValue( internProp, qvariant_cast< QSizeF >( val ) );
        return;
    }
    else if( QtRectPropertyManager * rectManager = qobject_cast< QtRectPropertyManager * >( manager ) )
    {
        rectManager->setValue( internProp, qvariant_cast< QRect >( val ) );
        return;
    }
    else if( QtRectFPropertyManager * rectFManager = qobject_cast< QtRectFPropertyManager * >( manager ) )
    {
        rectFManager->setValue( internProp, qvariant_cast< QRectF >( val ) );
        return;
    }
    else if( QtColorPropertyManager * colorManager = qobject_cast< QtColorPropertyManager * >( manager ) )
    {
        colorManager->setValue( internProp, qvariant_cast< QColor >( val ) );
        return;
    }
    else if( QtEnumPropertyManager * enumManager = qobject_cast< QtEnumPropertyManager * >( manager ) )
    {
        enumManager->setValue( internProp, qvariant_cast< int >( val ) );
        return;
    }
    else if( QtSizePolicyPropertyManager * sizePolicyManager =
                 qobject_cast< QtSizePolicyPropertyManager * >( manager ) )
    {
        sizePolicyManager->setValue( internProp, qvariant_cast< QSizePolicy >( val ) );
        return;
    }
    else if( QtFontPropertyManager * fontManager = qobject_cast< QtFontPropertyManager * >( manager ) )
    {
        fontManager->setValue( internProp, qvariant_cast< QFont >( val ) );
        return;
        #ifndef QT_NO_CURSOR
    }
    else if( QtCursorPropertyManager * cursorManager = qobject_cast< QtCursorPropertyManager * >( manager ) )
    {
        cursorManager->setValue( internProp, qvariant_cast< QCursor >( val ) );
        return;
        #endif
    }
    else if( QtFlagPropertyManager * flagManager = qobject_cast< QtFlagPropertyManager * >( manager ) )
    {
        flagManager->setValue( internProp, qvariant_cast< int >( val ) );
        return;
    }
}

/*!
    Sets the value of the specified \a attribute of the given \a
    property, to \a value.

    The new \a value's type must be of the type returned by
    attributeType(), or of a type that can be converted to
    attributeType() using the QVariant::canConvert() function,
    otherwise this function does nothing.

    \sa attributeValue(), QtVariantProperty::setAttribute(), attributeChanged()
*/
void QtVariantPropertyManager::setAttribute( QtProperty * property,
        const QString & attribute, const QVariant & value )
{
    QVariant oldAttr = attributeValue( property, attribute );
    if( !oldAttr.isValid() )
        return;
        
    int attrType = value.userType();
    if( !attrType )
        return;
        
    if( attrType != attributeType( propertyType( property ), attribute ) &&
        !value.canConvert( ( QVariant::Type )attrType ) )
        return;
        
    QtProperty * internProp = propertyToWrappedProperty()->value( property, 0 );
    if( internProp == 0 )
        return;
        
    QtAbstractPropertyManager * manager = internProp->propertyManager();
    if( QtIntPropertyManager * intManager = qobject_cast< QtIntPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_maximumAttribute )
            intManager->setMaximum( internProp, qvariant_cast< int >( value ) );
        else if( attribute == d_ptr->_minimumAttribute )
            intManager->setMinimum( internProp, qvariant_cast< int >( value ) );
        else if( attribute == d_ptr->_singleStepAttribute )
            intManager->setSingleStep( internProp, qvariant_cast< int >( value ) );
        return;
    }
    else if( QtUIntPropertyManager * uintManager = qobject_cast< QtUIntPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_maximumAttribute )
            uintManager->setMaximum( internProp, qvariant_cast< unsigned int >( value ) );
        else if( attribute == d_ptr->_minimumAttribute )
            uintManager->setMinimum( internProp, qvariant_cast< unsigned int >( value ) );
        return;
    }
    else if( QtLongLongPropertyManager * longlongManager = qobject_cast< QtLongLongPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_maximumAttribute )
            longlongManager->setMaximum( internProp, qvariant_cast< long long >( value ) );
        else if( attribute == d_ptr->_minimumAttribute )
            longlongManager->setMinimum( internProp, qvariant_cast< long long >( value ) );
        return;
    }
    else if( QtUnsignedLongLongPropertyManager * ulonglongManager = qobject_cast< QtUnsignedLongLongPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_maximumAttribute )
            ulonglongManager->setMaximum( internProp, qvariant_cast< unsigned long long >( value ) );
        else if( attribute == d_ptr->_minimumAttribute )
            ulonglongManager->setMinimum( internProp, qvariant_cast< unsigned long long >( value ) );
        return;
    }
    else if( QtDoublePropertyManager * doubleManager = qobject_cast< QtDoublePropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_maximumAttribute )
            doubleManager->setMaximum( internProp, qvariant_cast< double >( value ) );
        if( attribute == d_ptr->_minimumAttribute )
            doubleManager->setMinimum( internProp, qvariant_cast< double >( value ) );
        if( attribute == d_ptr->_singleStepAttribute )
            doubleManager->setSingleStep( internProp, qvariant_cast< double >( value ) );
        if( attribute == d_ptr->_decimalsAttribute )
            doubleManager->setDecimals( internProp, qvariant_cast< int >( value ) );
        return;
    }
    else if( QtStringPropertyManager * stringManager = qobject_cast< QtStringPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_regExpAttribute )
            stringManager->setRegExp( internProp, qvariant_cast< QRegExp >( value ) );
        if( attribute == d_ptr->_lengthAttribute )
            stringManager->setLength( internProp, qvariant_cast< int >( value ) );
        return;
    }
    else if( QtDatePropertyManager * dateManager = qobject_cast< QtDatePropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_maximumAttribute )
            dateManager->setMaximum( internProp, qvariant_cast< QDate >( value ) );
        if( attribute == d_ptr->_minimumAttribute )
            dateManager->setMinimum( internProp, qvariant_cast< QDate >( value ) );
        return;
    }
    else if( QtPointFPropertyManager * pointFManager = qobject_cast< QtPointFPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_decimalsAttribute )
            pointFManager->setDecimals( internProp, qvariant_cast< int >( value ) );
        return;
    }
    else if( QtSizePropertyManager * sizeManager = qobject_cast< QtSizePropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_maximumAttribute )
            sizeManager->setMaximum( internProp, qvariant_cast< QSize >( value ) );
        if( attribute == d_ptr->_minimumAttribute )
            sizeManager->setMinimum( internProp, qvariant_cast< QSize >( value ) );
        return;
    }
    else if( QtSizeFPropertyManager * sizeFManager = qobject_cast< QtSizeFPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_maximumAttribute )
            sizeFManager->setMaximum( internProp, qvariant_cast< QSizeF >( value ) );
        if( attribute == d_ptr->_minimumAttribute )
            sizeFManager->setMinimum( internProp, qvariant_cast< QSizeF >( value ) );
        if( attribute == d_ptr->_decimalsAttribute )
            sizeFManager->setDecimals( internProp, qvariant_cast< int >( value ) );
        return;
    }
    else if( QtRectPropertyManager * rectManager = qobject_cast< QtRectPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_constraintAttribute )
            rectManager->setConstraint( internProp, qvariant_cast< QRect >( value ) );
        return;
    }
    else if( QtRectFPropertyManager * rectFManager = qobject_cast< QtRectFPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_constraintAttribute )
            rectFManager->setConstraint( internProp, qvariant_cast< QRectF >( value ) );
        if( attribute == d_ptr->_decimalsAttribute )
            rectFManager->setDecimals( internProp, qvariant_cast< int >( value ) );
        return;
    }
    else if( QtEnumPropertyManager * enumManager = qobject_cast< QtEnumPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_enumNamesAttribute )
            enumManager->setEnumNames( internProp, qvariant_cast< QStringList >( value ) );
        if( attribute == d_ptr->_enumIconsAttribute )
            enumManager->setEnumIcons( internProp, qvariant_cast< QtIconMap >( value ) );
        return;
    }
    else if( QtFlagPropertyManager * flagManager = qobject_cast< QtFlagPropertyManager * >( manager ) )
    {
        if( attribute == d_ptr->_flagNamesAttribute )
            flagManager->setFlagNames( internProp, qvariant_cast< QStringList >( value ) );
        return;
    }
}

/*!
    \internal
*/
bool QtVariantPropertyManager::hasValue( const QtProperty * property ) const
{
    if( propertyType( property ) == groupTypeId() )
        return false;
    return true;
}

/*!
    \internal
*/
QString QtVariantPropertyManager::valueText( const QtProperty * property ) const
{
    const QtProperty * internProp = propertyToWrappedProperty()->value( property, 0 );
    return internProp ? internProp->valueText() : QString();
}

/*!
    \internal
*/
QIcon QtVariantPropertyManager::valueIcon( const QtProperty * property ) const
{
    const QtProperty * internProp = propertyToWrappedProperty()->value( property, 0 );
    return internProp ? internProp->valueIcon() : QIcon();
}

/*!
    \internal
*/
void QtVariantPropertyManager::initializeProperty( QtProperty * property )
{
    QtVariantProperty * varProp = variantProperty( property );
    if( !varProp )
        return;
        
    QMap< int, QtAbstractPropertyManager * >::ConstIterator it =
        d_ptr->_typeToPropertyManager.find( d_ptr->_propertyType );
    if( it != d_ptr->_typeToPropertyManager.constEnd() )
    {
        QtProperty * internProp = 0;
        if( !d_ptr->_creatingSubProperties )
        {
            QtAbstractPropertyManager * manager = it.value();
            internProp = manager->addProperty();
            d_ptr->_internalToProperty[internProp] = varProp;
        }
        propertyToWrappedProperty()->insert( varProp, internProp );
        if( internProp )
        {
            QList< QtProperty * > children = internProp->subProperties();
            QListIterator< QtProperty * > itChild( children );
            QtVariantProperty * lastProperty = 0;
            while( itChild.hasNext() )
            {
                QtVariantProperty * prop = d_ptr->createSubProperty( varProp, lastProperty, itChild.next() );
                lastProperty = prop ? prop : lastProperty;
            }
        }
    }
}

/*!
    \internal
*/
void QtVariantPropertyManager::uninitializeProperty( QtProperty * property )
{
    const QMap< const QtProperty *, QPair< QtVariantProperty *, int > >::iterator type_it = d_ptr->_propertyToType.find( property );
    if( type_it == d_ptr->_propertyToType.end() )
        return;
        
    PropertyMap::iterator it = propertyToWrappedProperty()->find( property );
    if( it != propertyToWrappedProperty()->end() )
    {
        QtProperty * internProp = it.value();
        if( internProp )
        {
            d_ptr->_internalToProperty.remove( internProp );
            if( !d_ptr->_destroyingSubProperties )
                delete internProp;
        }
        propertyToWrappedProperty()->erase( it );
    }
    d_ptr->_propertyToType.erase( type_it );
}

/*!
    \internal
*/
QtProperty * QtVariantPropertyManager::createProperty()
{
    if( !d_ptr->_creatingProperty )
        return 0;
        
    QtVariantProperty * property = new QtVariantProperty( this );
    d_ptr->_propertyToType.insert( property, qMakePair( property, d_ptr->_propertyType ) );
    
    return property;
}

/////////////////////////////

class QtVariantEditorFactoryPrivate
{
    QtVariantEditorFactory * q_ptr;
    Q_DECLARE_PUBLIC( QtVariantEditorFactory )
public:

    QtSpinBoxFactory      *     _spinBoxFactory;
    QtDoubleSpinBoxFactory   *  _doubleSpinBoxFactory;
    QtCheckBoxFactory     *     _checkBoxFactory;
    QtLineEditFactory     *     _lineEditFactory;
    QtDateEditFactory     *     _dateEditFactory;
    QtTimeEditFactory     *     _timeEditFactory;
    QtDateTimeEditFactory   *   _dateTimeEditFactory;
    QtKeySequenceEditorFactory * _keySequenceEditorFactory;
    QtCharEditorFactory    *    _charEditorFactory;
    QtEnumEditorFactory    *    _comboBoxFactory;
    QtCursorEditorFactory   *   _cursorEditorFactory;
    QtColorEditorFactory    *   _colorEditorFactory;
    QtFontEditorFactory    *    _fontEditorFactory;
    
    QMap< QtAbstractEditorFactoryBase *, int > _factoryToType;
    QMap< int, QtAbstractEditorFactoryBase * > _typeToFactory;
};

/*!
    \class QtVariantEditorFactory
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtVariantEditorFactory class provides widgets for properties
    created by QtVariantPropertyManager objects.

    The variant factory provides the following widgets for the
    specified property types:

    \table
    \header
        \li Property Type
        \li Widget
    \row
        \li \c int
        \li QSpinBox
    \row
        \li \c double
        \li QDoubleSpinBox
    \row
        \li \c bool
        \li QCheckBox
    \row
        \li QString
        \li QLineEdit
    \row
        \li QDate
        \li QDateEdit
    \row
        \li QTime
        \li QTimeEdit
    \row
        \li QDateTime
        \li QDateTimeEdit
    \row
        \li QKeySequence
        \li customized editor
    \row
        \li QChar
        \li customized editor
    \row
        \li \c enum
        \li QComboBox
    \row
        \li QCursor
        \li QComboBox
    \endtable

    Note that QtVariantPropertyManager supports several additional property
    types for which the QtVariantEditorFactory class does not provide
    editing widgets, e.g. QPoint and QSize. To provide widgets for other
    types using the variant approach, derive from the QtVariantEditorFactory
    class.

    \sa QtAbstractEditorFactory, QtVariantPropertyManager
*/

/*!
    Creates a factory with the given \a parent.
*/
QtVariantEditorFactory::QtVariantEditorFactory( QObject * parent )
    : QtAbstractEditorFactory< QtVariantPropertyManager >( parent ), d_ptr( new QtVariantEditorFactoryPrivate() )
{
    d_ptr->q_ptr = this;
    
    d_ptr->_spinBoxFactory = new QtSpinBoxFactory( this );
    d_ptr->_factoryToType[d_ptr->_spinBoxFactory] = QVariant::Int;
    d_ptr->_typeToFactory[QVariant::Int] = d_ptr->_spinBoxFactory;
    
    d_ptr->_doubleSpinBoxFactory = new QtDoubleSpinBoxFactory( this );
    d_ptr->_factoryToType[d_ptr->_doubleSpinBoxFactory] = QVariant::Double;
    d_ptr->_typeToFactory[QVariant::Double] = d_ptr->_doubleSpinBoxFactory;
    
    d_ptr->_checkBoxFactory = new QtCheckBoxFactory( this );
    d_ptr->_factoryToType[d_ptr->_checkBoxFactory] = QVariant::Bool;
    d_ptr->_typeToFactory[QVariant::Bool] = d_ptr->_checkBoxFactory;
    
    d_ptr->_lineEditFactory = new QtLineEditFactory( this );
    d_ptr->_factoryToType[d_ptr->_lineEditFactory] = QVariant::String;
    d_ptr->_typeToFactory[QVariant::String] = d_ptr->_lineEditFactory;
    
    d_ptr->_dateEditFactory = new QtDateEditFactory( this );
    d_ptr->_factoryToType[d_ptr->_dateEditFactory] = QVariant::Date;
    d_ptr->_typeToFactory[QVariant::Date] = d_ptr->_dateEditFactory;
    
    d_ptr->_timeEditFactory = new QtTimeEditFactory( this );
    d_ptr->_factoryToType[d_ptr->_timeEditFactory] = QVariant::Time;
    d_ptr->_typeToFactory[QVariant::Time] = d_ptr->_timeEditFactory;
    
    d_ptr->_dateTimeEditFactory = new QtDateTimeEditFactory( this );
    d_ptr->_factoryToType[d_ptr->_dateTimeEditFactory] = QVariant::DateTime;
    d_ptr->_typeToFactory[QVariant::DateTime] = d_ptr->_dateTimeEditFactory;
    
    d_ptr->_keySequenceEditorFactory = new QtKeySequenceEditorFactory( this );
    d_ptr->_factoryToType[d_ptr->_keySequenceEditorFactory] = QVariant::KeySequence;
    d_ptr->_typeToFactory[QVariant::KeySequence] = d_ptr->_keySequenceEditorFactory;
    
    d_ptr->_charEditorFactory = new QtCharEditorFactory( this );
    d_ptr->_factoryToType[d_ptr->_charEditorFactory] = QVariant::Char;
    d_ptr->_typeToFactory[QVariant::Char] = d_ptr->_charEditorFactory;
    
    d_ptr->_cursorEditorFactory = new QtCursorEditorFactory( this );
    d_ptr->_factoryToType[d_ptr->_cursorEditorFactory] = QVariant::Cursor;
    d_ptr->_typeToFactory[QVariant::Cursor] = d_ptr->_cursorEditorFactory;
    
    d_ptr->_colorEditorFactory = new QtColorEditorFactory( this );
    d_ptr->_factoryToType[d_ptr->_colorEditorFactory] = QVariant::Color;
    d_ptr->_typeToFactory[QVariant::Color] = d_ptr->_colorEditorFactory;
    
    d_ptr->_fontEditorFactory = new QtFontEditorFactory( this );
    d_ptr->_factoryToType[d_ptr->_fontEditorFactory] = QVariant::Font;
    d_ptr->_typeToFactory[QVariant::Font] = d_ptr->_fontEditorFactory;
    
    d_ptr->_comboBoxFactory = new QtEnumEditorFactory( this );
    const int enumId = QtVariantPropertyManager::enumTypeId();
    d_ptr->_factoryToType[d_ptr->_comboBoxFactory] = enumId;
    d_ptr->_typeToFactory[enumId] = d_ptr->_comboBoxFactory;
}

/*!
    Destroys this factory, and all the widgets it has created.
*/
QtVariantEditorFactory::~QtVariantEditorFactory()
{
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtVariantEditorFactory::connectPropertyManager( QtVariantPropertyManager * manager )
{
    QList< QtIntPropertyManager * > intPropertyManagers = manager->findChildren< QtIntPropertyManager * >();
    QListIterator< QtIntPropertyManager * > itInt( intPropertyManagers );
    while( itInt.hasNext() )
        d_ptr->_spinBoxFactory->addPropertyManager( itInt.next() );
        
    QList< QtDoublePropertyManager * > doublePropertyManagers = manager->findChildren< QtDoublePropertyManager * >();
    QListIterator< QtDoublePropertyManager * > itDouble( doublePropertyManagers );
    while( itDouble.hasNext() )
        d_ptr->_doubleSpinBoxFactory->addPropertyManager( itDouble.next() );
        
    QList< QtBoolPropertyManager * > boolPropertyManagers = manager->findChildren< QtBoolPropertyManager * >();
    QListIterator< QtBoolPropertyManager * > itBool( boolPropertyManagers );
    while( itBool.hasNext() )
        d_ptr->_checkBoxFactory->addPropertyManager( itBool.next() );
        
    QList< QtStringPropertyManager * > stringPropertyManagers = manager->findChildren< QtStringPropertyManager * >();
    QListIterator< QtStringPropertyManager * > itString( stringPropertyManagers );
    while( itString.hasNext() )
        d_ptr->_lineEditFactory->addPropertyManager( itString.next() );
        
    QList< QtDatePropertyManager * > datePropertyManagers = manager->findChildren< QtDatePropertyManager * >();
    QListIterator< QtDatePropertyManager * > itDate( datePropertyManagers );
    while( itDate.hasNext() )
        d_ptr->_dateEditFactory->addPropertyManager( itDate.next() );
        
    QList< QtTimePropertyManager * > timePropertyManagers = manager->findChildren< QtTimePropertyManager * >();
    QListIterator< QtTimePropertyManager * > itTime( timePropertyManagers );
    while( itTime.hasNext() )
        d_ptr->_timeEditFactory->addPropertyManager( itTime.next() );
        
    QList< QtDateTimePropertyManager * > dateTimePropertyManagers = manager->findChildren< QtDateTimePropertyManager * >();
    QListIterator< QtDateTimePropertyManager * > itDateTime( dateTimePropertyManagers );
    while( itDateTime.hasNext() )
        d_ptr->_dateTimeEditFactory->addPropertyManager( itDateTime.next() );
        
    QList< QtKeySequencePropertyManager * > keySequencePropertyManagers = manager->findChildren< QtKeySequencePropertyManager * >();
    QListIterator< QtKeySequencePropertyManager * > itKeySequence( keySequencePropertyManagers );
    while( itKeySequence.hasNext() )
        d_ptr->_keySequenceEditorFactory->addPropertyManager( itKeySequence.next() );
        
    QList< QtCharPropertyManager * > charPropertyManagers = manager->findChildren< QtCharPropertyManager * >();
    QListIterator< QtCharPropertyManager * > itChar( charPropertyManagers );
    while( itChar.hasNext() )
        d_ptr->_charEditorFactory->addPropertyManager( itChar.next() );
        
    QList< QtLocalePropertyManager * > localePropertyManagers = manager->findChildren< QtLocalePropertyManager * >();
    QListIterator< QtLocalePropertyManager * > itLocale( localePropertyManagers );
    while( itLocale.hasNext() )
        d_ptr->_comboBoxFactory->addPropertyManager( itLocale.next()->subEnumPropertyManager() );
        
    QList< QtPointPropertyManager * > pointPropertyManagers = manager->findChildren< QtPointPropertyManager * >();
    QListIterator< QtPointPropertyManager * > itPoint( pointPropertyManagers );
    while( itPoint.hasNext() )
        d_ptr->_spinBoxFactory->addPropertyManager( itPoint.next()->subIntPropertyManager() );
        
    QList< QtPointFPropertyManager * > pointFPropertyManagers = manager->findChildren< QtPointFPropertyManager * >();
    QListIterator< QtPointFPropertyManager * > itPointF( pointFPropertyManagers );
    while( itPointF.hasNext() )
        d_ptr->_doubleSpinBoxFactory->addPropertyManager( itPointF.next()->subDoublePropertyManager() );
        
    QList< QtSizePropertyManager * > sizePropertyManagers = manager->findChildren< QtSizePropertyManager * >();
    QListIterator< QtSizePropertyManager * > itSize( sizePropertyManagers );
    while( itSize.hasNext() )
        d_ptr->_spinBoxFactory->addPropertyManager( itSize.next()->subIntPropertyManager() );
        
    QList< QtSizeFPropertyManager * > sizeFPropertyManagers = manager->findChildren< QtSizeFPropertyManager * >();
    QListIterator< QtSizeFPropertyManager * > itSizeF( sizeFPropertyManagers );
    while( itSizeF.hasNext() )
        d_ptr->_doubleSpinBoxFactory->addPropertyManager( itSizeF.next()->subDoublePropertyManager() );
        
    QList< QtRectPropertyManager * > rectPropertyManagers = manager->findChildren< QtRectPropertyManager * >();
    QListIterator< QtRectPropertyManager * > itRect( rectPropertyManagers );
    while( itRect.hasNext() )
        d_ptr->_spinBoxFactory->addPropertyManager( itRect.next()->subIntPropertyManager() );
        
    QList< QtRectFPropertyManager * > rectFPropertyManagers = manager->findChildren< QtRectFPropertyManager * >();
    QListIterator< QtRectFPropertyManager * > itRectF( rectFPropertyManagers );
    while( itRectF.hasNext() )
        d_ptr->_doubleSpinBoxFactory->addPropertyManager( itRectF.next()->subDoublePropertyManager() );
        
    QList< QtColorPropertyManager * > colorPropertyManagers = manager->findChildren< QtColorPropertyManager * >();
    QListIterator< QtColorPropertyManager * > itColor( colorPropertyManagers );
    while( itColor.hasNext() )
    {
        QtColorPropertyManager * colorManager = itColor.next();
        d_ptr->_colorEditorFactory->addPropertyManager( colorManager );
        d_ptr->_spinBoxFactory->addPropertyManager( colorManager->subIntPropertyManager() );
    }
    
    QList< QtEnumPropertyManager * > enumPropertyManagers = manager->findChildren< QtEnumPropertyManager * >();
    QListIterator< QtEnumPropertyManager * > itEnum( enumPropertyManagers );
    while( itEnum.hasNext() )
        d_ptr->_comboBoxFactory->addPropertyManager( itEnum.next() );
        
    QList< QtSizePolicyPropertyManager * > sizePolicyPropertyManagers = manager->findChildren< QtSizePolicyPropertyManager * >();
    QListIterator< QtSizePolicyPropertyManager * > itSizePolicy( sizePolicyPropertyManagers );
    while( itSizePolicy.hasNext() )
    {
        QtSizePolicyPropertyManager * sizePolicyManager = itSizePolicy.next();
        d_ptr->_spinBoxFactory->addPropertyManager( sizePolicyManager->subIntPropertyManager() );
        d_ptr->_comboBoxFactory->addPropertyManager( sizePolicyManager->subEnumPropertyManager() );
    }
    
    QList< QtFontPropertyManager * > fontPropertyManagers = manager->findChildren< QtFontPropertyManager * >();
    QListIterator< QtFontPropertyManager * > itFont( fontPropertyManagers );
    while( itFont.hasNext() )
    {
        QtFontPropertyManager * fontManager = itFont.next();
        d_ptr->_fontEditorFactory->addPropertyManager( fontManager );
        d_ptr->_spinBoxFactory->addPropertyManager( fontManager->subIntPropertyManager() );
        d_ptr->_comboBoxFactory->addPropertyManager( fontManager->subEnumPropertyManager() );
        d_ptr->_checkBoxFactory->addPropertyManager( fontManager->subBoolPropertyManager() );
    }
    
    QList< QtCursorPropertyManager * > cursorPropertyManagers = manager->findChildren< QtCursorPropertyManager * >();
    QListIterator< QtCursorPropertyManager * > itCursor( cursorPropertyManagers );
    while( itCursor.hasNext() )
        d_ptr->_cursorEditorFactory->addPropertyManager( itCursor.next() );
        
    QList< QtFlagPropertyManager * > flagPropertyManagers = manager->findChildren< QtFlagPropertyManager * >();
    QListIterator< QtFlagPropertyManager * > itFlag( flagPropertyManagers );
    while( itFlag.hasNext() )
        d_ptr->_checkBoxFactory->addPropertyManager( itFlag.next()->subBoolPropertyManager() );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget * QtVariantEditorFactory::createEditor( QtVariantPropertyManager * manager, QtProperty * property,
        QWidget * parent )
{
    const int propType = manager->propertyType( property );
    QtAbstractEditorFactoryBase * factory = d_ptr->_typeToFactory.value( propType, 0 );
    if( !factory )
        return 0;
    return factory->createEditor( wrappedProperty( property ), parent );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtVariantEditorFactory::disconnectPropertyManager( QtVariantPropertyManager * manager )
{
    QList< QtIntPropertyManager * > intPropertyManagers = manager->findChildren< QtIntPropertyManager * >();
    QListIterator< QtIntPropertyManager * > itInt( intPropertyManagers );
    while( itInt.hasNext() )
        d_ptr->_spinBoxFactory->removePropertyManager( itInt.next() );
        
    QList< QtDoublePropertyManager * > doublePropertyManagers = manager->findChildren< QtDoublePropertyManager * >();
    QListIterator< QtDoublePropertyManager * > itDouble( doublePropertyManagers );
    while( itDouble.hasNext() )
        d_ptr->_doubleSpinBoxFactory->removePropertyManager( itDouble.next() );
        
    QList< QtBoolPropertyManager * > boolPropertyManagers = manager->findChildren< QtBoolPropertyManager * >();
    QListIterator< QtBoolPropertyManager * > itBool( boolPropertyManagers );
    while( itBool.hasNext() )
        d_ptr->_checkBoxFactory->removePropertyManager( itBool.next() );
        
    QList< QtStringPropertyManager * > stringPropertyManagers = manager->findChildren< QtStringPropertyManager * >();
    QListIterator< QtStringPropertyManager * > itString( stringPropertyManagers );
    while( itString.hasNext() )
        d_ptr->_lineEditFactory->removePropertyManager( itString.next() );
        
    QList< QtDatePropertyManager * > datePropertyManagers = manager->findChildren< QtDatePropertyManager * >();
    QListIterator< QtDatePropertyManager * > itDate( datePropertyManagers );
    while( itDate.hasNext() )
        d_ptr->_dateEditFactory->removePropertyManager( itDate.next() );
        
    QList< QtTimePropertyManager * > timePropertyManagers = manager->findChildren< QtTimePropertyManager * >();
    QListIterator< QtTimePropertyManager * > itTime( timePropertyManagers );
    while( itTime.hasNext() )
        d_ptr->_timeEditFactory->removePropertyManager( itTime.next() );
        
    QList< QtDateTimePropertyManager * > dateTimePropertyManagers = manager->findChildren< QtDateTimePropertyManager * >();
    QListIterator< QtDateTimePropertyManager * > itDateTime( dateTimePropertyManagers );
    while( itDateTime.hasNext() )
        d_ptr->_dateTimeEditFactory->removePropertyManager( itDateTime.next() );
        
    QList< QtKeySequencePropertyManager * > keySequencePropertyManagers = manager->findChildren< QtKeySequencePropertyManager * >();
    QListIterator< QtKeySequencePropertyManager * > itKeySequence( keySequencePropertyManagers );
    while( itKeySequence.hasNext() )
        d_ptr->_keySequenceEditorFactory->removePropertyManager( itKeySequence.next() );
        
    QList< QtCharPropertyManager * > charPropertyManagers = manager->findChildren< QtCharPropertyManager * >();
    QListIterator< QtCharPropertyManager * > itChar( charPropertyManagers );
    while( itChar.hasNext() )
        d_ptr->_charEditorFactory->removePropertyManager( itChar.next() );
        
    QList< QtLocalePropertyManager * > localePropertyManagers = manager->findChildren< QtLocalePropertyManager * >();
    QListIterator< QtLocalePropertyManager * > itLocale( localePropertyManagers );
    while( itLocale.hasNext() )
        d_ptr->_comboBoxFactory->removePropertyManager( itLocale.next()->subEnumPropertyManager() );
        
    QList< QtPointPropertyManager * > pointPropertyManagers = manager->findChildren< QtPointPropertyManager * >();
    QListIterator< QtPointPropertyManager * > itPoint( pointPropertyManagers );
    while( itPoint.hasNext() )
        d_ptr->_spinBoxFactory->removePropertyManager( itPoint.next()->subIntPropertyManager() );
        
    QList< QtPointFPropertyManager * > pointFPropertyManagers = manager->findChildren< QtPointFPropertyManager * >();
    QListIterator< QtPointFPropertyManager * > itPointF( pointFPropertyManagers );
    while( itPointF.hasNext() )
        d_ptr->_doubleSpinBoxFactory->removePropertyManager( itPointF.next()->subDoublePropertyManager() );
        
    QList< QtSizePropertyManager * > sizePropertyManagers = manager->findChildren< QtSizePropertyManager * >();
    QListIterator< QtSizePropertyManager * > itSize( sizePropertyManagers );
    while( itSize.hasNext() )
        d_ptr->_spinBoxFactory->removePropertyManager( itSize.next()->subIntPropertyManager() );
        
    QList< QtSizeFPropertyManager * > sizeFPropertyManagers = manager->findChildren< QtSizeFPropertyManager * >();
    QListIterator< QtSizeFPropertyManager * > itSizeF( sizeFPropertyManagers );
    while( itSizeF.hasNext() )
        d_ptr->_doubleSpinBoxFactory->removePropertyManager( itSizeF.next()->subDoublePropertyManager() );
        
    QList< QtRectPropertyManager * > rectPropertyManagers = manager->findChildren< QtRectPropertyManager * >();
    QListIterator< QtRectPropertyManager * > itRect( rectPropertyManagers );
    while( itRect.hasNext() )
        d_ptr->_spinBoxFactory->removePropertyManager( itRect.next()->subIntPropertyManager() );
        
    QList< QtRectFPropertyManager * > rectFPropertyManagers = manager->findChildren< QtRectFPropertyManager * >();
    QListIterator< QtRectFPropertyManager * > itRectF( rectFPropertyManagers );
    while( itRectF.hasNext() )
        d_ptr->_doubleSpinBoxFactory->removePropertyManager( itRectF.next()->subDoublePropertyManager() );
        
    QList< QtColorPropertyManager * > colorPropertyManagers = manager->findChildren< QtColorPropertyManager * >();
    QListIterator< QtColorPropertyManager * > itColor( colorPropertyManagers );
    while( itColor.hasNext() )
    {
        QtColorPropertyManager * colorManager = itColor.next();
        d_ptr->_colorEditorFactory->removePropertyManager( colorManager );
        d_ptr->_spinBoxFactory->removePropertyManager( colorManager->subIntPropertyManager() );
    }
    
    QList< QtEnumPropertyManager * > enumPropertyManagers = manager->findChildren< QtEnumPropertyManager * >();
    QListIterator< QtEnumPropertyManager * > itEnum( enumPropertyManagers );
    while( itEnum.hasNext() )
        d_ptr->_comboBoxFactory->removePropertyManager( itEnum.next() );
        
    QList< QtSizePolicyPropertyManager * > sizePolicyPropertyManagers = manager->findChildren< QtSizePolicyPropertyManager * >();
    QListIterator< QtSizePolicyPropertyManager * > itSizePolicy( sizePolicyPropertyManagers );
    while( itSizePolicy.hasNext() )
    {
        QtSizePolicyPropertyManager * sizePolicyManager = itSizePolicy.next();
        d_ptr->_spinBoxFactory->removePropertyManager( sizePolicyManager->subIntPropertyManager() );
        d_ptr->_comboBoxFactory->removePropertyManager( sizePolicyManager->subEnumPropertyManager() );
    }
    
    QList< QtFontPropertyManager * > fontPropertyManagers = manager->findChildren< QtFontPropertyManager * >();
    QListIterator< QtFontPropertyManager * > itFont( fontPropertyManagers );
    while( itFont.hasNext() )
    {
        QtFontPropertyManager * fontManager = itFont.next();
        d_ptr->_fontEditorFactory->removePropertyManager( fontManager );
        d_ptr->_spinBoxFactory->removePropertyManager( fontManager->subIntPropertyManager() );
        d_ptr->_comboBoxFactory->removePropertyManager( fontManager->subEnumPropertyManager() );
        d_ptr->_checkBoxFactory->removePropertyManager( fontManager->subBoolPropertyManager() );
    }
    
    QList< QtCursorPropertyManager * > cursorPropertyManagers = manager->findChildren< QtCursorPropertyManager * >();
    QListIterator< QtCursorPropertyManager * > itCursor( cursorPropertyManagers );
    while( itCursor.hasNext() )
        d_ptr->_cursorEditorFactory->removePropertyManager( itCursor.next() );
        
    QList< QtFlagPropertyManager * > flagPropertyManagers = manager->findChildren< QtFlagPropertyManager * >();
    QListIterator< QtFlagPropertyManager * > itFlag( flagPropertyManagers );
    while( itFlag.hasNext() )
        d_ptr->_checkBoxFactory->removePropertyManager( itFlag.next()->subBoolPropertyManager() );
}

QT_END_NAMESPACE

#include "moc_qtvariantproperty.cpp"
