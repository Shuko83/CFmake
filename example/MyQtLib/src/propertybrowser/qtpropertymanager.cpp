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

#include "qtpropertymanager.h"
#include "qtpropertybrowserutils_p.h"

#include <QDateTime>
#include <QLocale>
#include <QMap>
#include <QTimer>
#include <QIcon>
#include <QMetaEnum>
#include <QFontDatabase>
#include <QStyleOption>
#include <QStyle>
#include <QApplication>
#include <QPainter>
#include <QLabel>

#include <limits.h>
#include <float.h>

#if defined(Q_CC_MSVC)
    #pragma warning(disable: 4786) /* MS VS 6: truncating debug info after 255 characters */
#endif

QT_BEGIN_NAMESPACE

template < class PrivateData, class Value >
static void setSimpleMinimumData( PrivateData * data, const Value & minVal )
{
    data->minVal = minVal;
    if( data->maxVal < data->minVal )
        data->maxVal = data->minVal;
        
    if( data->val < data->minVal )
        data->val = data->minVal;
}

template < class PrivateData, class Value >
static void setSimpleMaximumData( PrivateData * data, const Value & maxVal )
{
    data->maxVal = maxVal;
    if( data->minVal > data->maxVal )
        data->minVal = data->maxVal;
        
    if( data->val > data->maxVal )
        data->val = data->maxVal;
}

template < class PrivateData, class Value >
static void setSizeMinimumData( PrivateData * data, const Value & newMinVal )
{
    data->minVal = newMinVal;
    if( data->maxVal.width() < data->minVal.width() )
        data->maxVal.setWidth( data->minVal.width() );
    if( data->maxVal.height() < data->minVal.height() )
        data->maxVal.setHeight( data->minVal.height() );
        
    if( data->val.width() < data->minVal.width() )
        data->val.setWidth( data->minVal.width() );
    if( data->val.height() < data->minVal.height() )
        data->val.setHeight( data->minVal.height() );
}

template < class PrivateData, class Value >
static void setSizeMaximumData( PrivateData * data, const Value & newMaxVal )
{
    data->maxVal = newMaxVal;
    if( data->minVal.width() > data->maxVal.width() )
        data->minVal.setWidth( data->maxVal.width() );
    if( data->minVal.height() > data->maxVal.height() )
        data->minVal.setHeight( data->maxVal.height() );
        
    if( data->val.width() > data->maxVal.width() )
        data->val.setWidth( data->maxVal.width() );
    if( data->val.height() > data->maxVal.height() )
        data->val.setHeight( data->maxVal.height() );
}

template < class SizeValue >
static SizeValue qBoundSize( const SizeValue & minVal, const SizeValue & val, const SizeValue & maxVal )
{
    SizeValue croppedVal = val;
    if( minVal.width() > val.width() )
        croppedVal.setWidth( minVal.width() );
    else if( maxVal.width() < val.width() )
        croppedVal.setWidth( maxVal.width() );
        
    if( minVal.height() > val.height() )
        croppedVal.setHeight( minVal.height() );
    else if( maxVal.height() < val.height() )
        croppedVal.setHeight( maxVal.height() );
        
    return croppedVal;
}

// Match the exact signature of qBound for VS 6.
QSize qBound( QSize minVal, QSize val, QSize maxVal )
{
    return qBoundSize( minVal, val, maxVal );
}

QSizeF qBound( QSizeF minVal, QSizeF val, QSizeF maxVal )
{
    return qBoundSize( minVal, val, maxVal );
}

namespace {

    namespace {
        template < class Value >
        void orderBorders( Value & minVal, Value & maxVal )
        {
            if( minVal > maxVal )
                std::swap( minVal, maxVal );
        }
        
        template < class Value >
        static void orderSizeBorders( Value & minVal, Value & maxVal )
        {
            Value fromSize = minVal;
            Value toSize = maxVal;
            if( fromSize.width() > toSize.width() )
            {
                fromSize.setWidth( maxVal.width() );
                toSize.setWidth( minVal.width() );
            }
            if( fromSize.height() > toSize.height() )
            {
                fromSize.setHeight( maxVal.height() );
                toSize.setHeight( minVal.height() );
            }
            minVal = fromSize;
            maxVal = toSize;
        }
        
        void orderBorders( QSize & minVal, QSize & maxVal )
        {
            orderSizeBorders( minVal, maxVal );
        }
        
        void orderBorders( QSizeF & minVal, QSizeF & maxVal )
        {
            orderSizeBorders( minVal, maxVal );
        }
        
    }
}
////////

template < class Value, class PrivateData >
static Value getData( const QMap< const QtProperty *, PrivateData > & propertyMap,
                      Value PrivateData::*data,
                      const QtProperty * property, const Value & defaultValue = Value() )
{
    typedef QMap< const QtProperty *, PrivateData > PropertyToData;
    typedef typename PropertyToData::const_iterator PropertyToDataConstIterator;
    const PropertyToDataConstIterator it = propertyMap.constFind( property );
    if( it == propertyMap.constEnd() )
        return defaultValue;
    return it.value().*data;
}

template < class Value, class PrivateData >
static Value getValue( const QMap< const QtProperty *, PrivateData > & propertyMap,
                       const QtProperty * property, const Value & defaultValue = Value() )
{
    return getData< Value >( propertyMap, &PrivateData::val, property, defaultValue );
}

template < class Value, class PrivateData >
static Value getMinimum( const QMap< const QtProperty *, PrivateData > & propertyMap,
                         const QtProperty * property, const Value & defaultValue = Value() )
{
    return getData< Value >( propertyMap, &PrivateData::minVal, property, defaultValue );
}

template < class Value, class PrivateData >
static Value getMaximum( const QMap< const QtProperty *, PrivateData > & propertyMap,
                         const QtProperty * property, const Value & defaultValue = Value() )
{
    return getData< Value >( propertyMap, &PrivateData::maxVal, property, defaultValue );
}

template < class ValueChangeParameter, class Value, class PropertyManager >
static void setSimpleValue( QMap< const QtProperty *, Value > & propertyMap,
                            PropertyManager * manager,
                            void ( PropertyManager::*propertyChangedSignal )( QtProperty * ),
                            void ( PropertyManager::*valueChangedSignal )( QtProperty *, ValueChangeParameter ),
                            QtProperty * property, const Value & val )
{
    typedef QMap< const QtProperty *, Value > PropertyToData;
    typedef typename PropertyToData::iterator PropertyToDataIterator;
    const PropertyToDataIterator it = propertyMap.find( property );
    if( it == propertyMap.end() )
        return;
        
    if( it.value() == val )
        return;
        
    it.value() = val;
    
    emit( manager->*propertyChangedSignal )( property );
    emit( manager->*valueChangedSignal )( property, val );
}

template < class ValueChangeParameter, class PropertyManagerPrivate, class PropertyManager, class Value >
static void setValueInRange( PropertyManager * manager, PropertyManagerPrivate * managerPrivate,
                             void ( PropertyManager::*propertyChangedSignal )( QtProperty * ),
                             void ( PropertyManager::*valueChangedSignal )( QtProperty *, ValueChangeParameter ),
                             QtProperty * property, const Value & val,
                             void ( PropertyManagerPrivate::*setSubPropertyValue )( QtProperty *, ValueChangeParameter ) )
{
    typedef typename PropertyManagerPrivate::Data PrivateData;
    typedef QMap< const QtProperty *, PrivateData > PropertyToData;
    typedef typename PropertyToData::iterator PropertyToDataIterator;
    const PropertyToDataIterator it = managerPrivate->_values.find( property );
    if( it == managerPrivate->_values.end() )
        return;
        
    PrivateData & data = it.value();
    
    if( data.val == val )
        return;
        
    const Value oldVal = data.val;
    
    data.val = qBound( data.minVal, val, data.maxVal );
    
    if( data.val == oldVal )
        return;
        
    if( setSubPropertyValue )
        ( managerPrivate->*setSubPropertyValue )( property, data.val );
        
    emit( manager->*propertyChangedSignal )( property );
    emit( manager->*valueChangedSignal )( property, data.val );
}

template < class ValueChangeParameter, class PropertyManagerPrivate, class PropertyManager, class Value >
static void setBorderValues( PropertyManager * manager, PropertyManagerPrivate * managerPrivate,
                             void ( PropertyManager::*propertyChangedSignal )( QtProperty * ),
                             void ( PropertyManager::*valueChangedSignal )( QtProperty *, ValueChangeParameter ),
                             void ( PropertyManager::*rangeChangedSignal )( QtProperty *, ValueChangeParameter, ValueChangeParameter ),
                             QtProperty * property, const Value & minVal, const Value & maxVal,
                             void ( PropertyManagerPrivate::*setSubPropertyRange )( QtProperty *,
                                     ValueChangeParameter, ValueChangeParameter, ValueChangeParameter ) )
{
    typedef typename PropertyManagerPrivate::Data PrivateData;
    typedef QMap< const QtProperty *, PrivateData > PropertyToData;
    typedef typename PropertyToData::iterator PropertyToDataIterator;
    const PropertyToDataIterator it = managerPrivate->_values.find( property );
    if( it == managerPrivate->_values.end() )
        return;
        
    Value fromVal = minVal;
    Value toVal = maxVal;
    orderBorders( fromVal, toVal );
    
    PrivateData & data = it.value();
    
    if( data.minVal == fromVal && data.maxVal == toVal )
        return;
        
    const Value oldVal = data.val;
    
    data.setMinimumValue( fromVal );
    data.setMaximumValue( toVal );
    
    emit( manager->*rangeChangedSignal )( property, data.minVal, data.maxVal );
    
    if( setSubPropertyRange )
        ( managerPrivate->*setSubPropertyRange )( property, data.minVal, data.maxVal, data.val );
        
    if( data.val == oldVal )
        return;
        
    emit( manager->*propertyChangedSignal )( property );
    emit( manager->*valueChangedSignal )( property, data.val );
}

template < class ValueChangeParameter, class PropertyManagerPrivate, class PropertyManager, class Value, class PrivateData >
static void setBorderValue( PropertyManager * manager, PropertyManagerPrivate * managerPrivate,
                            void ( PropertyManager::*propertyChangedSignal )( QtProperty * ),
                            void ( PropertyManager::*valueChangedSignal )( QtProperty *, ValueChangeParameter ),
                            void ( PropertyManager::*rangeChangedSignal )( QtProperty *, ValueChangeParameter, ValueChangeParameter ),
                            QtProperty * property,
                            Value( PrivateData::*getRangeVal )() const,
                            void ( PrivateData::*setRangeVal )( ValueChangeParameter ), const Value & borderVal,
                            void ( PropertyManagerPrivate::*setSubPropertyRange )( QtProperty *,
                                    ValueChangeParameter, ValueChangeParameter, ValueChangeParameter ) )
{
    typedef QMap< const QtProperty *, PrivateData > PropertyToData;
    typedef typename PropertyToData::iterator PropertyToDataIterator;
    const PropertyToDataIterator it = managerPrivate->_values.find( property );
    if( it == managerPrivate->_values.end() )
        return;
        
    PrivateData & data = it.value();
    
    if( ( data.*getRangeVal )() == borderVal )
        return;
        
    const Value oldVal = data.val;
    
    ( data.*setRangeVal )( borderVal );
    
    emit( manager->*rangeChangedSignal )( property, data.minVal, data.maxVal );
    
    if( setSubPropertyRange )
        ( managerPrivate->*setSubPropertyRange )( property, data.minVal, data.maxVal, data.val );
        
    if( data.val == oldVal )
        return;
        
    emit( manager->*propertyChangedSignal )( property );
    emit( manager->*valueChangedSignal )( property, data.val );
}

template < class ValueChangeParameter, class PropertyManagerPrivate, class PropertyManager, class Value, class PrivateData >
static void setMinimumValue( PropertyManager * manager, PropertyManagerPrivate * managerPrivate,
                             void ( PropertyManager::*propertyChangedSignal )( QtProperty * ),
                             void ( PropertyManager::*valueChangedSignal )( QtProperty *, ValueChangeParameter ),
                             void ( PropertyManager::*rangeChangedSignal )( QtProperty *, ValueChangeParameter, ValueChangeParameter ),
                             QtProperty * property, const Value & minVal )
{
    void ( PropertyManagerPrivate::*setSubPropertyRange )( QtProperty *,
            ValueChangeParameter, ValueChangeParameter, ValueChangeParameter ) = 0;
    setBorderValue< ValueChangeParameter, PropertyManagerPrivate, PropertyManager, Value, PrivateData >( manager, managerPrivate,
            propertyChangedSignal, valueChangedSignal, rangeChangedSignal,
            property, &PropertyManagerPrivate::Data::minimumValue, &PropertyManagerPrivate::Data::setMinimumValue, minVal, setSubPropertyRange );
}

template < class ValueChangeParameter, class PropertyManagerPrivate, class PropertyManager, class Value, class PrivateData >
static void setMaximumValue( PropertyManager * manager, PropertyManagerPrivate * managerPrivate,
                             void ( PropertyManager::*propertyChangedSignal )( QtProperty * ),
                             void ( PropertyManager::*valueChangedSignal )( QtProperty *, ValueChangeParameter ),
                             void ( PropertyManager::*rangeChangedSignal )( QtProperty *, ValueChangeParameter, ValueChangeParameter ),
                             QtProperty * property, const Value & maxVal )
{
    void ( PropertyManagerPrivate::*setSubPropertyRange )( QtProperty *,
            ValueChangeParameter, ValueChangeParameter, ValueChangeParameter ) = 0;
    setBorderValue< ValueChangeParameter, PropertyManagerPrivate, PropertyManager, Value, PrivateData >( manager, managerPrivate,
            propertyChangedSignal, valueChangedSignal, rangeChangedSignal,
            property, &PropertyManagerPrivate::Data::maximumValue, &PropertyManagerPrivate::Data::setMaximumValue, maxVal, setSubPropertyRange );
}

class QtMetaEnumWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QSizePolicy::Policy policy READ policy )
public:
    QSizePolicy::Policy policy() const { return QSizePolicy::Ignored; }
private:
    QtMetaEnumWrapper( QObject * parent ) : QObject( parent ) {}
};

class QtMetaEnumProvider
{
public:
    QtMetaEnumProvider();
    
    QStringList policyEnumNames() const { return _policyEnumNames; }
    QStringList languageEnumNames() const { return _languageEnumNames; }
    QStringList countryEnumNames( QLocale::Language language ) const { return _countryEnumNames.value( language ); }
    
    QSizePolicy::Policy indexToSizePolicy( int index ) const;
    int sizePolicyToIndex( QSizePolicy::Policy policy ) const;
    
    void indexToLocale( int languageIndex, int countryIndex, QLocale::Language * language, QLocale::Country * country ) const;
    void localeToIndex( QLocale::Language language, QLocale::Country country, int * languageIndex, int * countryIndex ) const;
    
private:
    void initLocale();
    
    QStringList _policyEnumNames;
    QStringList _languageEnumNames;
    QMap< QLocale::Language, QStringList > _countryEnumNames;
    QMap< int, QLocale::Language > _indexToLanguage;
    QMap< QLocale::Language, int > _languageToIndex;
    QMap< int, QMap< int, QLocale::Country > > _indexToCountry;
    QMap< QLocale::Language, QMap< QLocale::Country, int > > _countryToIndex;
    QMetaEnum _policyEnum;
};

static QList< QLocale::Country > sortCountries( const QList< QLocale::Country > & countries )
{
    QMultiMap< QString, QLocale::Country > nameToCountry;
    QListIterator< QLocale::Country > itCountry( countries );
    while( itCountry.hasNext() )
    {
        QLocale::Country country = itCountry.next();
        nameToCountry.insert( QLocale::countryToString( country ), country );
    }
    return nameToCountry.values();
}

void QtMetaEnumProvider::initLocale()
{
    QMultiMap< QString, QLocale::Language > nameToLanguage;
    QLocale::Language language = QLocale::C;
    while( language <= QLocale::LastLanguage )
    {
        QLocale locale( language );
        if( locale.language() == language )
            nameToLanguage.insert( QLocale::languageToString( language ), language );
        language = ( QLocale::Language )( ( uint )language + 1 ); // ++language
    }
    
    const QLocale system = QLocale::system();
    if( !nameToLanguage.contains( QLocale::languageToString( system.language() ) ) )
        nameToLanguage.insert( QLocale::languageToString( system.language() ), system.language() );
        
    QList< QLocale::Language > languages = nameToLanguage.values();
    QListIterator< QLocale::Language > itLang( languages );
    while( itLang.hasNext() )
    {
        QLocale::Language lang = itLang.next();
        QList< QLocale::Country > countries;
        countries = QLocale::countriesForLanguage( lang );
        if( countries.isEmpty() && lang == system.language() )
            countries << system.country();
            
        if( !countries.isEmpty() && !_languageToIndex.contains( lang ) )
        {
            countries = sortCountries( countries );
            int langIdx = _languageEnumNames.count();
            _indexToLanguage[langIdx] = lang;
            _languageToIndex[lang] = langIdx;
            QStringList countryNames;
            QListIterator< QLocale::Country > it( countries );
            int countryIdx = 0;
            while( it.hasNext() )
            {
                QLocale::Country country = it.next();
                countryNames << QLocale::countryToString( country );
                _indexToCountry[langIdx][countryIdx] = country;
                _countryToIndex[lang][country] = countryIdx;
                ++countryIdx;
            }
            _languageEnumNames << QLocale::languageToString( lang );
            _countryEnumNames[lang] = countryNames;
        }
    }
}

QtMetaEnumProvider::QtMetaEnumProvider()
{
    QMetaProperty p;
    
    p = QtMetaEnumWrapper::staticMetaObject.property(
            QtMetaEnumWrapper::staticMetaObject.propertyOffset() + 0 );
    _policyEnum = p.enumerator();
    const int keyCount = _policyEnum.keyCount();
    for( int i = 0; i < keyCount; i++ )
        _policyEnumNames << QLatin1String( _policyEnum.key( i ) );
        
    initLocale();
}

QSizePolicy::Policy QtMetaEnumProvider::indexToSizePolicy( int index ) const
{
    return static_cast< QSizePolicy::Policy >( _policyEnum.value( index ) );
}

int QtMetaEnumProvider::sizePolicyToIndex( QSizePolicy::Policy policy ) const
{
    const int keyCount = _policyEnum.keyCount();
    for( int i = 0; i < keyCount; i++ )
        if( indexToSizePolicy( i ) == policy )
            return i;
    return -1;
}

void QtMetaEnumProvider::indexToLocale( int languageIndex, int countryIndex, QLocale::Language * language, QLocale::Country * country ) const
{
    QLocale::Language l = QLocale::C;
    QLocale::Country c = QLocale::AnyCountry;
    if( _indexToLanguage.contains( languageIndex ) )
    {
        l = _indexToLanguage[languageIndex];
        if( _indexToCountry.contains( languageIndex ) && _indexToCountry[languageIndex].contains( countryIndex ) )
            c = _indexToCountry[languageIndex][countryIndex];
    }
    if( language )
        *language = l;
    if( country )
        *country = c;
}

void QtMetaEnumProvider::localeToIndex( QLocale::Language language, QLocale::Country country, int * languageIndex, int * countryIndex ) const
{
    int l = -1;
    int c = -1;
    if( _languageToIndex.contains( language ) )
    {
        l = _languageToIndex[language];
        if( _countryToIndex.contains( language ) && _countryToIndex[language].contains( country ) )
            c = _countryToIndex[language][country];
    }
    
    if( languageIndex )
        *languageIndex = l;
    if( countryIndex )
        *countryIndex = c;
}

Q_GLOBAL_STATIC( QtMetaEnumProvider, metaEnumProvider )

// QtGroupPropertyManager

/*!
    \class QtGroupPropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtGroupPropertyManager provides and manages group properties.

    This class is intended to provide a grouping element without any value.

    \sa QtAbstractPropertyManager
*/

/*!
    Creates a manager with the given \a parent.
*/
QtGroupPropertyManager::QtGroupPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent )
{

}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtGroupPropertyManager::~QtGroupPropertyManager()
{

}

/*!
    \reimp
*/
bool QtGroupPropertyManager::hasValue( const QtProperty * property ) const
{
    Q_UNUSED( property )
    return false;
}

/*!
    \reimp
*/
void QtGroupPropertyManager::initializeProperty( QtProperty * property )
{
    Q_UNUSED( property )
}

/*!
    \reimp
*/
void QtGroupPropertyManager::uninitializeProperty( QtProperty * property )
{
    Q_UNUSED( property )
}

// QtIntPropertyManager

class QtIntPropertyManagerPrivate
{
    QtIntPropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtIntPropertyManager )
public:

    struct Data
    {
        Data() : val( 0 ), minVal( -INT_MAX ), maxVal( INT_MAX ), singleStep( 1 ) {}
        int val;
        int minVal;
        int maxVal;
        int singleStep;
        int minimumValue() const { return minVal; }
        int maximumValue() const { return maxVal; }
        void setMinimumValue( int newMinVal ) { setSimpleMinimumData( this, newMinVal ); }
        void setMaximumValue( int newMaxVal ) { setSimpleMaximumData( this, newMaxVal ); }
    };
    
    typedef QMap< const QtProperty *, Data > PropertyValueMap;
    PropertyValueMap _values;
};

/*!
    \class QtIntPropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtIntPropertyManager provides and manages int properties.

    An int property has a current value, and a range specifying the
    valid values. The range is defined by a minimum and a maximum
    value.

    The property's value and range can be retrieved using the value(),
    minimum() and maximum() functions, and can be set using the
    setValue(), setMinimum() and setMaximum() slots. Alternatively,
    the range can be defined in one go using the setRange() slot.

    In addition, QtIntPropertyManager provides the valueChanged() signal which
    is emitted whenever a property created by this manager changes,
    and the rangeChanged() signal which is emitted whenever such a
    property changes its range of valid values.

    \sa QtAbstractPropertyManager, QtSpinBoxFactory, QtSliderFactory, QtScrollBarFactory
*/

/*!
    \fn void QtIntPropertyManager::valueChanged(QtProperty *property, int value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the new
    \a value as parameters.

    \sa setValue()
*/

/*!
    \fn void QtIntPropertyManager::rangeChanged(QtProperty *property, int minimum, int maximum)

    This signal is emitted whenever a property created by this manager
    changes its range of valid values, passing a pointer to the
    \a property and the new \a minimum and \a maximum values.

    \sa setRange()
*/

/*!
    \fn void QtIntPropertyManager::singleStepChanged(QtProperty *property, int step)

    This signal is emitted whenever a property created by this manager
    changes its single step property, passing a pointer to the
    \a property and the new \a step value

    \sa setSingleStep()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtIntPropertyManager::QtIntPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtIntPropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtIntPropertyManager::~QtIntPropertyManager()
{
    clear();
}

/*!
    Returns the given \a property's value.

    If the given property is not managed by this manager, this
    function returns 0.

    \sa setValue()
*/
int QtIntPropertyManager::value( const QtProperty * property ) const
{
    return getValue< int >( d_ptr->_values, property, 0 );
}

/*!
    Returns the given \a property's minimum value.

    \sa setMinimum(), maximum(), setRange()
*/
int QtIntPropertyManager::minimum( const QtProperty * property ) const
{
    return getMinimum< int >( d_ptr->_values, property, 0 );
}

/*!
    Returns the given \a property's maximum value.

    \sa setMaximum(), minimum(), setRange()
*/
int QtIntPropertyManager::maximum( const QtProperty * property ) const
{
    return getMaximum< int >( d_ptr->_values, property, 0 );
}

/*!
    Returns the given \a property's step value.

    The step is typically used to increment or decrement a property value while pressing an arrow key.

    \sa setSingleStep()
*/
int QtIntPropertyManager::singleStep( const QtProperty * property ) const
{
    return getData< int >( d_ptr->_values, &QtIntPropertyManagerPrivate::Data::singleStep, property, 0 );
}

/*!
    \reimp
*/
QString QtIntPropertyManager::valueText( const QtProperty * property ) const
{
    const QtIntPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
    return QString::number( it.value().val );
}

/*!
    \fn void QtIntPropertyManager::setValue(QtProperty *property, int value)

    Sets the value of the given \a property to \a value.

    If the specified \a value is not valid according to the given \a
    property's range, the \a value is adjusted to the nearest valid
    value within the range.

    \sa value(), setRange(), valueChanged()
*/
void QtIntPropertyManager::setValue( QtProperty * property, int val )
{
    void ( QtIntPropertyManagerPrivate::*setSubPropertyValue )( QtProperty *, int ) = 0;
    setValueInRange< int, QtIntPropertyManagerPrivate, QtIntPropertyManager, int >( this, d_ptr.data(),
            &QtIntPropertyManager::propertyChanged,
            &QtIntPropertyManager::valueChanged,
            property, val, setSubPropertyValue );
}

/*!
    Sets the minimum value for the given \a property to \a minVal.

    When setting the minimum value, the maximum and current values are
    adjusted if necessary (ensuring that the range remains valid and
    that the current value is within the range).

    \sa minimum(), setRange(), rangeChanged()
*/
void QtIntPropertyManager::setMinimum( QtProperty * property, int minVal )
{
    setMinimumValue< int, QtIntPropertyManagerPrivate, QtIntPropertyManager, int, QtIntPropertyManagerPrivate::Data >( this, d_ptr.data(),
            &QtIntPropertyManager::propertyChanged,
            &QtIntPropertyManager::valueChanged,
            &QtIntPropertyManager::rangeChanged,
            property, minVal );
}

/*!
    Sets the maximum value for the given \a property to \a maxVal.

    When setting maximum value, the minimum and current values are
    adjusted if necessary (ensuring that the range remains valid and
    that the current value is within the range).

    \sa maximum(), setRange(), rangeChanged()
*/
void QtIntPropertyManager::setMaximum( QtProperty * property, int maxVal )
{
    setMaximumValue< int, QtIntPropertyManagerPrivate, QtIntPropertyManager, int, QtIntPropertyManagerPrivate::Data >( this, d_ptr.data(),
            &QtIntPropertyManager::propertyChanged,
            &QtIntPropertyManager::valueChanged,
            &QtIntPropertyManager::rangeChanged,
            property, maxVal );
}

/*!
    \fn void QtIntPropertyManager::setRange(QtProperty *property, int minimum, int maximum)

    Sets the range of valid values.

    This is a convenience function defining the range of valid values
    in one go; setting the \a minimum and \a maximum values for the
    given \a property with a single function call.

    When setting a new range, the current value is adjusted if
    necessary (ensuring that the value remains within range).

    \sa setMinimum(), setMaximum(), rangeChanged()
*/
void QtIntPropertyManager::setRange( QtProperty * property, int minVal, int maxVal )
{
    void ( QtIntPropertyManagerPrivate::*setSubPropertyRange )( QtProperty *, int, int, int ) = 0;
    setBorderValues< int, QtIntPropertyManagerPrivate, QtIntPropertyManager, int >( this, d_ptr.data(),
            &QtIntPropertyManager::propertyChanged,
            &QtIntPropertyManager::valueChanged,
            &QtIntPropertyManager::rangeChanged,
            property, minVal, maxVal, setSubPropertyRange );
}

/*!
    Sets the step value for the given \a property to \a step.

    The step is typically used to increment or decrement a property value while pressing an arrow key.

    \sa singleStep()
*/
void QtIntPropertyManager::setSingleStep( QtProperty * property, int step )
{
    const QtIntPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    QtIntPropertyManagerPrivate::Data data = it.value();
    
    if( step < 0 )
        step = 0;
        
    if( data.singleStep == step )
        return;
        
    data.singleStep = step;
    
    it.value() = data;
    
    emit singleStepChanged( property, data.singleStep );
}

/*!
    \reimp
*/
void QtIntPropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = QtIntPropertyManagerPrivate::Data();
}

/*!
    \reimp
*/
void QtIntPropertyManager::uninitializeProperty( QtProperty * property )
{
    d_ptr->_values.remove( property );
}

// QtUIntPropertyManager

class QtUIntPropertyManagerPrivate
{
    QtUIntPropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtUIntPropertyManager )
public:

    struct Data
    {
        Data() : val( 0 ), minVal( 0 ), maxVal( UINT_MAX ), singleStep( 1 ) {}
        unsigned int val;
        unsigned int minVal;
        unsigned int maxVal;
        unsigned int singleStep;
        unsigned int minimumValue() const { return minVal; }
        unsigned int maximumValue() const { return maxVal; }
        void setMinimumValue( unsigned int newMinVal ) { setSimpleMinimumData( this, newMinVal ); }
        void setMaximumValue( unsigned int newMaxVal ) { setSimpleMaximumData( this, newMaxVal ); }
    };
    
    typedef QMap< const QtProperty *, Data > PropertyValueMap;
    PropertyValueMap _values;
};


QtUIntPropertyManager::QtUIntPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtUIntPropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
}

QtUIntPropertyManager::~QtUIntPropertyManager()
{
    clear();
}


unsigned int QtUIntPropertyManager::value( const QtProperty * property ) const
{
    return getValue< unsigned int >( d_ptr->_values, property, 0 );
}

unsigned int QtUIntPropertyManager::minimum( const QtProperty * property ) const
{
    return getMinimum< unsigned int >( d_ptr->_values, property, 0 );
}

unsigned int QtUIntPropertyManager::maximum( const QtProperty * property ) const
{
    return getMaximum< unsigned int >( d_ptr->_values, property, 0 );
}

QString QtUIntPropertyManager::valueText( const QtProperty * property ) const
{
    const QtUIntPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
    return QString::number( it.value().val );
}

void QtUIntPropertyManager::setValue( QtProperty * property, unsigned int val )
{
    void ( QtUIntPropertyManagerPrivate::*setSubPropertyValue )( QtProperty *, unsigned int ) = 0;
    setValueInRange< unsigned int, QtUIntPropertyManagerPrivate, QtUIntPropertyManager, unsigned int >( this, d_ptr.data(),
            &QtUIntPropertyManager::propertyChanged,
            &QtUIntPropertyManager::valueChanged,
            property, val, setSubPropertyValue );
}

void QtUIntPropertyManager::setMinimum( QtProperty * property, unsigned int minVal )
{
    setMinimumValue< unsigned int, QtUIntPropertyManagerPrivate, QtUIntPropertyManager, unsigned int, QtUIntPropertyManagerPrivate::Data >( this, d_ptr.data(),
            &QtUIntPropertyManager::propertyChanged,
            &QtUIntPropertyManager::valueChanged,
            &QtUIntPropertyManager::rangeChanged,
            property, minVal );
}

void QtUIntPropertyManager::setMaximum( QtProperty * property, unsigned int maxVal )
{
    setMaximumValue< unsigned int, QtUIntPropertyManagerPrivate, QtUIntPropertyManager, unsigned int, QtUIntPropertyManagerPrivate::Data >( this, d_ptr.data(),
            &QtUIntPropertyManager::propertyChanged,
            &QtUIntPropertyManager::valueChanged,
            &QtUIntPropertyManager::rangeChanged,
            property, maxVal );
}

void QtUIntPropertyManager::setRange( QtProperty * property, unsigned int minVal, unsigned int maxVal )
{
    void ( QtUIntPropertyManagerPrivate::*setSubPropertyRange )( QtProperty *, unsigned int, unsigned int, unsigned int ) = 0;
    setBorderValues< unsigned int, QtUIntPropertyManagerPrivate, QtUIntPropertyManager, unsigned int >( this, d_ptr.data(),
            &QtUIntPropertyManager::propertyChanged,
            &QtUIntPropertyManager::valueChanged,
            &QtUIntPropertyManager::rangeChanged,
            property, minVal, maxVal, setSubPropertyRange );
}

void QtUIntPropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = QtUIntPropertyManagerPrivate::Data();
}

void QtUIntPropertyManager::uninitializeProperty( QtProperty * property )
{
    d_ptr->_values.remove( property );
}

// QtLongLongPropertyManager

class QtLongLongPropertyManagerPrivate
{
    QtLongLongPropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtLongLongPropertyManager )
public:

    struct Data
    {
        Data() : val( 0 ), minVal( 0 ), maxVal( UINT_MAX ), singleStep( 1 ) {}
        long long val;
        long long minVal;
        long long maxVal;
        long long singleStep;
        long long minimumValue() const { return minVal; }
        long long maximumValue() const { return maxVal; }
        void setMinimumValue( long long newMinVal ) { setSimpleMinimumData( this, newMinVal ); }
        void setMaximumValue( long long newMaxVal ) { setSimpleMaximumData( this, newMaxVal ); }
    };
    
    typedef QMap< const QtProperty *, Data > PropertyValueMap;
    PropertyValueMap _values;
};


QtLongLongPropertyManager::QtLongLongPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtLongLongPropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
}

QtLongLongPropertyManager::~QtLongLongPropertyManager()
{
    clear();
}

long long QtLongLongPropertyManager::value( const QtProperty * property ) const
{
    return getValue< long long >( d_ptr->_values, property, 0 );
}

long long QtLongLongPropertyManager::minimum( const QtProperty * property ) const
{
    return getMinimum< long long >( d_ptr->_values, property, 0 );
}

long long QtLongLongPropertyManager::maximum( const QtProperty * property ) const
{
    return getMaximum< long long >( d_ptr->_values, property, 0 );
}

QString QtLongLongPropertyManager::valueText( const QtProperty * property ) const
{
    const QtLongLongPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
    return QString::number( it.value().val );
}

void QtLongLongPropertyManager::setValue( QtProperty * property, long long val )
{
    void ( QtLongLongPropertyManagerPrivate::*setSubPropertyValue )( QtProperty *, long long ) = 0;
    setValueInRange< long long, QtLongLongPropertyManagerPrivate, QtLongLongPropertyManager, long long >( this, d_ptr.data(),
            &QtLongLongPropertyManager::propertyChanged,
            &QtLongLongPropertyManager::valueChanged,
            property, val, setSubPropertyValue );
}

void QtLongLongPropertyManager::setMinimum( QtProperty * property, long long minVal )
{
    setMinimumValue< long long, QtLongLongPropertyManagerPrivate, QtLongLongPropertyManager, long long, QtLongLongPropertyManagerPrivate::Data >( this, d_ptr.data(),
            &QtLongLongPropertyManager::propertyChanged,
            &QtLongLongPropertyManager::valueChanged,
            &QtLongLongPropertyManager::rangeChanged,
            property, minVal );
}

void QtLongLongPropertyManager::setMaximum( QtProperty * property, long long maxVal )
{
    setMaximumValue< long long, QtLongLongPropertyManagerPrivate, QtLongLongPropertyManager, long long, QtLongLongPropertyManagerPrivate::Data >( this, d_ptr.data(),
            &QtLongLongPropertyManager::propertyChanged,
            &QtLongLongPropertyManager::valueChanged,
            &QtLongLongPropertyManager::rangeChanged,
            property, maxVal );
}

void QtLongLongPropertyManager::setRange( QtProperty * property, long long minVal, long long maxVal )
{
    void ( QtLongLongPropertyManagerPrivate::*setSubPropertyRange )( QtProperty *, long long, long long, long long ) = 0;
    setBorderValues< long long, QtLongLongPropertyManagerPrivate, QtLongLongPropertyManager, long long >( this, d_ptr.data(),
            &QtLongLongPropertyManager::propertyChanged,
            &QtLongLongPropertyManager::valueChanged,
            &QtLongLongPropertyManager::rangeChanged,
            property, minVal, maxVal, setSubPropertyRange );
}

void QtLongLongPropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = QtLongLongPropertyManagerPrivate::Data();
}

void QtLongLongPropertyManager::uninitializeProperty( QtProperty * property )
{
    d_ptr->_values.remove( property );
}

// QtUnsignedLongLongPropertyManager

class QtUnsignedLongLongPropertyManagerPrivate
{
    QtUnsignedLongLongPropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtUnsignedLongLongPropertyManager )
public:

    struct Data
    {
        Data() : val( 0 ), minVal( 0 ), maxVal( UINT_MAX ), singleStep( 1 ) {}
        unsigned long long val;
        unsigned long long minVal;
        unsigned long long maxVal;
        unsigned long long singleStep;
        unsigned long long minimumValue() const { return minVal; }
        unsigned long long maximumValue() const { return maxVal; }
        void setMinimumValue( unsigned long long newMinVal ) { setSimpleMinimumData( this, newMinVal ); }
        void setMaximumValue( unsigned long long newMaxVal ) { setSimpleMaximumData( this, newMaxVal ); }
    };
    
    typedef QMap< const QtProperty *, Data > PropertyValueMap;
    PropertyValueMap _values;
};


QtUnsignedLongLongPropertyManager::QtUnsignedLongLongPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtUnsignedLongLongPropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
}

QtUnsignedLongLongPropertyManager::~QtUnsignedLongLongPropertyManager()
{
    clear();
}

unsigned long long QtUnsignedLongLongPropertyManager::value( const QtProperty * property ) const
{
    return getValue< unsigned long long >( d_ptr->_values, property, 0 );
}

unsigned long long QtUnsignedLongLongPropertyManager::minimum( const QtProperty * property ) const
{
    return getMinimum< unsigned long long >( d_ptr->_values, property, 0 );
}

unsigned long long QtUnsignedLongLongPropertyManager::maximum( const QtProperty * property ) const
{
    return getMaximum< unsigned long long >( d_ptr->_values, property, 0 );
}

QString QtUnsignedLongLongPropertyManager::valueText( const QtProperty * property ) const
{
    const QtUnsignedLongLongPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
    return QString::number( it.value().val );
}

void QtUnsignedLongLongPropertyManager::setValue( QtProperty * property, unsigned long long val )
{
    void ( QtUnsignedLongLongPropertyManagerPrivate::*setSubPropertyValue )( QtProperty *, unsigned long long ) = 0;
    setValueInRange< unsigned long long, QtUnsignedLongLongPropertyManagerPrivate, QtUnsignedLongLongPropertyManager, unsigned long long >( this, d_ptr.data(),
            &QtUnsignedLongLongPropertyManager::propertyChanged,
            &QtUnsignedLongLongPropertyManager::valueChanged,
            property, val, setSubPropertyValue );
}

void QtUnsignedLongLongPropertyManager::setMinimum( QtProperty * property, unsigned long long minVal )
{
    setMinimumValue< unsigned long long, QtUnsignedLongLongPropertyManagerPrivate, QtUnsignedLongLongPropertyManager, unsigned long long, QtUnsignedLongLongPropertyManagerPrivate::Data >( this, d_ptr.data(),
            &QtUnsignedLongLongPropertyManager::propertyChanged,
            &QtUnsignedLongLongPropertyManager::valueChanged,
            &QtUnsignedLongLongPropertyManager::rangeChanged,
            property, minVal );
}

void QtUnsignedLongLongPropertyManager::setMaximum( QtProperty * property, unsigned long long maxVal )
{
    setMaximumValue< unsigned long long, QtUnsignedLongLongPropertyManagerPrivate, QtUnsignedLongLongPropertyManager, unsigned long long, QtUnsignedLongLongPropertyManagerPrivate::Data >( this, d_ptr.data(),
            &QtUnsignedLongLongPropertyManager::propertyChanged,
            &QtUnsignedLongLongPropertyManager::valueChanged,
            &QtUnsignedLongLongPropertyManager::rangeChanged,
            property, maxVal );
}

void QtUnsignedLongLongPropertyManager::setRange( QtProperty * property, unsigned long long minVal, unsigned long long maxVal )
{
    void ( QtUnsignedLongLongPropertyManagerPrivate::*setSubPropertyRange )( QtProperty *, unsigned long long, unsigned long long, unsigned long long ) = 0;
    setBorderValues< unsigned long long, QtUnsignedLongLongPropertyManagerPrivate, QtUnsignedLongLongPropertyManager, unsigned long long >( this, d_ptr.data(),
            &QtUnsignedLongLongPropertyManager::propertyChanged,
            &QtUnsignedLongLongPropertyManager::valueChanged,
            &QtUnsignedLongLongPropertyManager::rangeChanged,
            property, minVal, maxVal, setSubPropertyRange );
}

void QtUnsignedLongLongPropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = QtUnsignedLongLongPropertyManagerPrivate::Data();
}

void QtUnsignedLongLongPropertyManager::uninitializeProperty( QtProperty * property )
{
    d_ptr->_values.remove( property );
}

// QtDoublePropertyManager

class QtDoublePropertyManagerPrivate
{
    QtDoublePropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtDoublePropertyManager )
public:

    struct Data
    {
        Data() : val( 0 ), minVal( -DBL_MAX ), maxVal( DBL_MAX ), singleStep( 1 ), decimals( 2 ) {}
        double val;
        double minVal;
        double maxVal;
        double singleStep;
        int decimals;
        double minimumValue() const { return minVal; }
        double maximumValue() const { return maxVal; }
        void setMinimumValue( double newMinVal ) { setSimpleMinimumData( this, newMinVal ); }
        void setMaximumValue( double newMaxVal ) { setSimpleMaximumData( this, newMaxVal ); }
    };
    
    typedef QMap< const QtProperty *, Data > PropertyValueMap;
    PropertyValueMap _values;
};

/*!
    \class QtDoublePropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtDoublePropertyManager provides and manages double properties.

    A double property has a current value, and a range specifying the
    valid values. The range is defined by a minimum and a maximum
    value.

    The property's value and range can be retrieved using the value(),
    minimum() and maximum() functions, and can be set using the
    setValue(), setMinimum() and setMaximum() slots.
    Alternatively, the range can be defined in one go using the
    setRange() slot.

    In addition, QtDoublePropertyManager provides the valueChanged() signal
    which is emitted whenever a property created by this manager
    changes, and the rangeChanged() signal which is emitted whenever
    such a property changes its range of valid values.

    \sa QtAbstractPropertyManager, QtDoubleSpinBoxFactory
*/

/*!
    \fn void QtDoublePropertyManager::valueChanged(QtProperty *property, double value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the new
    \a value as parameters.

    \sa setValue()
*/

/*!
    \fn void QtDoublePropertyManager::rangeChanged(QtProperty *property, double minimum, double maximum)

    This signal is emitted whenever a property created by this manager
    changes its range of valid values, passing a pointer to the
    \a property and the new \a minimum and \a maximum values

    \sa setRange()
*/

/*!
    \fn void QtDoublePropertyManager::decimalsChanged(QtProperty *property, int prec)

    This signal is emitted whenever a property created by this manager
    changes its precision of value, passing a pointer to the
    \a property and the new \a prec value

    \sa setDecimals()
*/

/*!
    \fn void QtDoublePropertyManager::singleStepChanged(QtProperty *property, double step)

    This signal is emitted whenever a property created by this manager
    changes its single step property, passing a pointer to the
    \a property and the new \a step value

    \sa setSingleStep()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtDoublePropertyManager::QtDoublePropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtDoublePropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
}

/*!
    Destroys  this manager, and all the properties it has created.
*/
QtDoublePropertyManager::~QtDoublePropertyManager()
{
    clear();
}

/*!
    Returns the given \a property's value.

    If the given property is not managed by this manager, this
    function returns 0.

    \sa setValue()
*/
double QtDoublePropertyManager::value( const QtProperty * property ) const
{
    return getValue< double >( d_ptr->_values, property, 0.0 );
}

/*!
    Returns the given \a property's minimum value.

    \sa maximum(), setRange()
*/
double QtDoublePropertyManager::minimum( const QtProperty * property ) const
{
    return getMinimum< double >( d_ptr->_values, property, 0.0 );
}

/*!
    Returns the given \a property's maximum value.

    \sa minimum(), setRange()
*/
double QtDoublePropertyManager::maximum( const QtProperty * property ) const
{
    return getMaximum< double >( d_ptr->_values, property, 0.0 );
}

/*!
    Returns the given \a property's step value.

    The step is typically used to increment or decrement a property value while pressing an arrow key.

    \sa setSingleStep()
*/
double QtDoublePropertyManager::singleStep( const QtProperty * property ) const
{
    return getData< double >( d_ptr->_values, &QtDoublePropertyManagerPrivate::Data::singleStep, property, 0 );
}

/*!
    Returns the given \a property's precision, in decimals.

    \sa setDecimals()
*/
int QtDoublePropertyManager::decimals( const QtProperty * property ) const
{
    return getData< int >( d_ptr->_values, &QtDoublePropertyManagerPrivate::Data::decimals, property, 0 );
}

/*!
    \reimp
*/
QString QtDoublePropertyManager::valueText( const QtProperty * property ) const
{
    const QtDoublePropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
    return QString::number( it.value().val, 'f', it.value().decimals );
}

/*!
    \fn void QtDoublePropertyManager::setValue(QtProperty *property, double value)

    Sets the value of the given \a property to \a value.

    If the specified \a value is not valid according to the given
    \a property's range, the \a value is adjusted to the nearest valid value
    within the range.

    \sa value(), setRange(), valueChanged()
*/
void QtDoublePropertyManager::setValue( QtProperty * property, double val )
{
    void ( QtDoublePropertyManagerPrivate::*setSubPropertyValue )( QtProperty *, double ) = 0;
    setValueInRange< double, QtDoublePropertyManagerPrivate, QtDoublePropertyManager, double >( this, d_ptr.data(),
            &QtDoublePropertyManager::propertyChanged,
            &QtDoublePropertyManager::valueChanged,
            property, val, setSubPropertyValue );
}

/*!
    Sets the step value for the given \a property to \a step.

    The step is typically used to increment or decrement a property value while pressing an arrow key.

    \sa singleStep()
*/
void QtDoublePropertyManager::setSingleStep( QtProperty * property, double step )
{
    const QtDoublePropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    QtDoublePropertyManagerPrivate::Data data = it.value();
    
    if( step < 0 )
        step = 0;
        
    if( data.singleStep == step )
        return;
        
    data.singleStep = step;
    
    it.value() = data;
    
    emit singleStepChanged( property, data.singleStep );
}

/*!
    \fn void QtDoublePropertyManager::setDecimals(QtProperty *property, int prec)

    Sets the precision of the given \a property to \a prec.

    The valid decimal range is 0-13. The default is 2.

    \sa decimals()
*/
void QtDoublePropertyManager::setDecimals( QtProperty * property, int prec )
{
    const QtDoublePropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    QtDoublePropertyManagerPrivate::Data data = it.value();
    
    if( prec > 13 )
        prec = 13;
    else if( prec < 0 )
        prec = 0;
        
    if( data.decimals == prec )
        return;
        
    data.decimals = prec;
    
    it.value() = data;
    
    emit decimalsChanged( property, data.decimals );
}

/*!
    Sets the minimum value for the given \a property to \a minVal.

    When setting the minimum value, the maximum and current values are
    adjusted if necessary (ensuring that the range remains valid and
    that the current value is within in the range).

    \sa minimum(), setRange(), rangeChanged()
*/
void QtDoublePropertyManager::setMinimum( QtProperty * property, double minVal )
{
    setMinimumValue< double, QtDoublePropertyManagerPrivate, QtDoublePropertyManager, double, QtDoublePropertyManagerPrivate::Data >( this, d_ptr.data(),
            &QtDoublePropertyManager::propertyChanged,
            &QtDoublePropertyManager::valueChanged,
            &QtDoublePropertyManager::rangeChanged,
            property, minVal );
}

/*!
    Sets the maximum value for the given \a property to \a maxVal.

    When setting the maximum value, the minimum and current values are
    adjusted if necessary (ensuring that the range remains valid and
    that the current value is within in the range).

    \sa maximum(), setRange(), rangeChanged()
*/
void QtDoublePropertyManager::setMaximum( QtProperty * property, double maxVal )
{
    setMaximumValue< double, QtDoublePropertyManagerPrivate, QtDoublePropertyManager, double, QtDoublePropertyManagerPrivate::Data >( this, d_ptr.data(),
            &QtDoublePropertyManager::propertyChanged,
            &QtDoublePropertyManager::valueChanged,
            &QtDoublePropertyManager::rangeChanged,
            property, maxVal );
}

/*!
    \fn void QtDoublePropertyManager::setRange(QtProperty *property, double minimum, double maximum)

    Sets the range of valid values.

    This is a convenience function defining the range of valid values
    in one go; setting the \a minimum and \a maximum values for the
    given \a property with a single function call.

    When setting a new range, the current value is adjusted if
    necessary (ensuring that the value remains within range).

    \sa setMinimum(), setMaximum(), rangeChanged()
*/
void QtDoublePropertyManager::setRange( QtProperty * property, double minVal, double maxVal )
{
    void ( QtDoublePropertyManagerPrivate::*setSubPropertyRange )( QtProperty *, double, double, double ) = 0;
    setBorderValues< double, QtDoublePropertyManagerPrivate, QtDoublePropertyManager, double >( this, d_ptr.data(),
            &QtDoublePropertyManager::propertyChanged,
            &QtDoublePropertyManager::valueChanged,
            &QtDoublePropertyManager::rangeChanged,
            property, minVal, maxVal, setSubPropertyRange );
}

/*!
    \reimp
*/
void QtDoublePropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = QtDoublePropertyManagerPrivate::Data();
}

/*!
    \reimp
*/
void QtDoublePropertyManager::uninitializeProperty( QtProperty * property )
{
    d_ptr->_values.remove( property );
}

// QtStringPropertyManager

class QtStringPropertyManagerPrivate
{
    QtStringPropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtStringPropertyManager )
public:

    struct Data
    {
        Data() : regExp( QString( QLatin1Char( '*' ) ), Qt::CaseSensitive, QRegExp::Wildcard ), length( 32767 )
        {
        }
        QString val;
        QRegExp regExp;
        int length;
    };
    
    typedef QMap< const QtProperty *, Data > PropertyValueMap;
    QMap< const QtProperty *, Data > _values;
};

/*!
    \class QtStringPropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtStringPropertyManager provides and manages QString properties.

    A string property's value can be retrieved using the value()
    function, and set using the setValue() slot.

    The current value can be checked against a regular expression. To
    set the regular expression use the setRegExp() slot, use the
    regExp() function to retrieve the currently set expression.

    In addition, QtStringPropertyManager provides the valueChanged() signal
    which is emitted whenever a property created by this manager
    changes, and the regExpChanged() signal which is emitted whenever
    such a property changes its currently set regular expression.

    \sa QtAbstractPropertyManager, QtLineEditFactory
*/

/*!
    \fn void QtStringPropertyManager::valueChanged(QtProperty *property, const QString &value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the
    new \a value as parameters.

    \sa setValue()
*/

/*!
    \fn void QtStringPropertyManager::regExpChanged(QtProperty *property, const QRegExp &regExp)

    This signal is emitted whenever a property created by this manager
    changes its currenlty set regular expression, passing a pointer to
    the \a property and the new \a regExp as parameters.

    \sa setRegExp()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtStringPropertyManager::QtStringPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtStringPropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtStringPropertyManager::~QtStringPropertyManager()
{
    clear();
}

/*!
    Returns the given \a property's value.

    If the given property is not managed by this manager, this
    function returns an empty string.

    \sa setValue()
*/
QString QtStringPropertyManager::value( const QtProperty * property ) const
{
    return getValue< QString >( d_ptr->_values, property );
}

/*!
    Returns the given \a property's currently set regular expression.

    If the given \a property is not managed by this manager, this
    function returns an empty expression.

    \sa setRegExp()
*/
QRegExp QtStringPropertyManager::regExp( const QtProperty * property ) const
{
    return getData< QRegExp >( d_ptr->_values, &QtStringPropertyManagerPrivate::Data::regExp, property, QRegExp() );
}

int QtStringPropertyManager::length( const QtProperty * property ) const
{
    return getData< int >( d_ptr->_values, &QtStringPropertyManagerPrivate::Data::length, property, 32767 );
}

/*!
    \reimp
*/
QString QtStringPropertyManager::valueText( const QtProperty * property ) const
{
    const QtStringPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
    return it.value().val;
}

/*!
    \fn void QtStringPropertyManager::setValue(QtProperty *property, const QString &value)

    Sets the value of the given \a property to \a value.

    If the specified \a value doesn't match the given \a property's
    regular expression, this function does nothing.

    \sa value(), setRegExp(), valueChanged()
*/
void QtStringPropertyManager::setValue( QtProperty * property, const QString & val )
{
    const QtStringPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    QtStringPropertyManagerPrivate::Data data = it.value();
    
    if( data.val == val )
        return;
        
    if( data.regExp.isValid() && !data.regExp.exactMatch( val ) )
        return;
        
    data.val = val;
    
    it.value() = data;
    
    emit propertyChanged( property );
    emit valueChanged( property, data.val );
}

/*!
    Sets the regular expression of the given \a property to \a regExp.

    \sa regExp(), setValue(), regExpChanged()
*/
void QtStringPropertyManager::setRegExp( QtProperty * property, const QRegExp & regExp )
{
    const QtStringPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    QtStringPropertyManagerPrivate::Data data = it.value() ;
    
    if( data.regExp == regExp )
        return;
        
    data.regExp = regExp;
    
    it.value() = data;
    
    emit regExpChanged( property, data.regExp );
}

void QtStringPropertyManager::setLength( QtProperty * property, int length )
{
    const QtStringPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    QtStringPropertyManagerPrivate::Data data = it.value();
    
    if( data.length == length )
        return;
        
    data.length = length;
    
    it.value() = data;
    
    emit lengthChanged( property, data.length );
}

/*!
    \reimp
*/
void QtStringPropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = QtStringPropertyManagerPrivate::Data();
}

/*!
    \reimp
*/
void QtStringPropertyManager::uninitializeProperty( QtProperty * property )
{
    d_ptr->_values.remove( property );
}

// QtBoolPropertyManager
//     Return an icon containing a check box indicator
static QIcon drawCheckBox( bool value )
{
    QStyleOptionButton opt;
    opt.state |= value ? QStyle::State_On : QStyle::State_Off;
    opt.state |= QStyle::State_Enabled;
    const QStyle * style = QApplication::style();
    // Figure out size of an indicator and make sure it is not scaled down in a list view item
    // by making the pixmap as big as a list view icon and centering the indicator in it.
    // (if it is smaller, it can't be helped)
    const int indicatorWidth = style->pixelMetric( QStyle::PM_IndicatorWidth, &opt );
    const int indicatorHeight = style->pixelMetric( QStyle::PM_IndicatorHeight, &opt );
    const int listViewIconSize = indicatorWidth;
    const int pixmapWidth = indicatorWidth;
    const int pixmapHeight = qMax( indicatorHeight, listViewIconSize );
    
    opt.rect = QRect( 0, 0, indicatorWidth, indicatorHeight );
    QPixmap pixmap = QPixmap( pixmapWidth, pixmapHeight );
    pixmap.fill( Qt::transparent );
    {
        // Center?
        const int xoff = ( pixmapWidth  > indicatorWidth )  ? ( pixmapWidth  - indicatorWidth )  / 2 : 0;
        const int yoff = ( pixmapHeight > indicatorHeight ) ? ( pixmapHeight - indicatorHeight ) / 2 : 0;
        QPainter painter( &pixmap );
        painter.translate( xoff, yoff );
        style->drawPrimitive( QStyle::PE_IndicatorCheckBox, &opt, &painter );
    }
    return QIcon( pixmap );
}

class QtBoolPropertyManagerPrivate
{
    QtBoolPropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtBoolPropertyManager )
public:
    QtBoolPropertyManagerPrivate();
    
    QMap< const QtProperty *, bool > _values;
    const QIcon _checkedIcon;
    const QIcon _uncheckedIcon;
};

QtBoolPropertyManagerPrivate::QtBoolPropertyManagerPrivate() :
    _checkedIcon( drawCheckBox( true ) ),
    _uncheckedIcon( drawCheckBox( false ) )
{
}

/*!
    \class QtBoolPropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtBoolPropertyManager class provides and manages boolean properties.

    The property's value can be retrieved using the value() function,
    and set using the setValue() slot.

    In addition, QtBoolPropertyManager provides the valueChanged() signal
    which is emitted whenever a property created by this manager
    changes.

    \sa QtAbstractPropertyManager, QtCheckBoxFactory
*/

/*!
    \fn void QtBoolPropertyManager::valueChanged(QtProperty *property, bool value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the
    new \a value as parameters.
*/

/*!
    Creates a manager with the given \a parent.
*/
QtBoolPropertyManager::QtBoolPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtBoolPropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtBoolPropertyManager::~QtBoolPropertyManager()
{
    clear();
}

/*!
    Returns the given \a property's value.

    If the given \a property is not managed by \e this manager, this
    function returns false.

    \sa setValue()
*/
bool QtBoolPropertyManager::value( const QtProperty * property ) const
{
    return d_ptr->_values.value( property, false );
}

/*!
    \reimp
*/
QString QtBoolPropertyManager::valueText( const QtProperty * property ) const
{
    const QMap< const QtProperty *, bool >::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
        
    static const QString trueText = tr( "True" );
    static const QString falseText = tr( "False" );
    return it.value() ? trueText : falseText;
}

/*!
    \reimp
*/
QIcon QtBoolPropertyManager::valueIcon( const QtProperty * property ) const
{
    const QMap< const QtProperty *, bool >::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QIcon();
        
    return it.value() ? d_ptr->_checkedIcon : d_ptr->_uncheckedIcon;
}

/*!
    \fn void QtBoolPropertyManager::setValue(QtProperty *property, bool value)

    Sets the value of the given \a property to \a value.

    \sa value()
*/
void QtBoolPropertyManager::setValue( QtProperty * property, bool val )
{
    setSimpleValue< bool, bool, QtBoolPropertyManager >( d_ptr->_values, this,
            &QtBoolPropertyManager::propertyChanged,
            &QtBoolPropertyManager::valueChanged,
            property, val );
}

/*!
    \reimp
*/
void QtBoolPropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = false;
}

/*!
    \reimp
*/
void QtBoolPropertyManager::uninitializeProperty( QtProperty * property )
{
    d_ptr->_values.remove( property );
}

// QtDatePropertyManager

class QtDatePropertyManagerPrivate
{
    QtDatePropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtDatePropertyManager )
public:
    explicit QtDatePropertyManagerPrivate( QtDatePropertyManager * q );
    
    struct Data
    {
        Data() : val( QDate::currentDate() ), minVal( QDate( 1752, 9, 14 ) ),
            maxVal( QDate( 7999, 12, 31 ) ) {}
        QDate val;
        QDate minVal;
        QDate maxVal;
        QDate minimumValue() const { return minVal; }
        QDate maximumValue() const { return maxVal; }
        void setMinimumValue( const QDate & newMinVal ) { setSimpleMinimumData( this, newMinVal ); }
        void setMaximumValue( const QDate & newMaxVal ) { setSimpleMaximumData( this, newMaxVal ); }
    };
    
    QString _format;
    
    typedef QMap< const QtProperty *, Data > PropertyValueMap;
    QMap< const QtProperty *, Data > _values;
};

QtDatePropertyManagerPrivate::QtDatePropertyManagerPrivate( QtDatePropertyManager * q ) :
    q_ptr( q ),
    _format( QtPropertyBrowserUtils::dateFormat() )
{
}

/*!
    \class QtDatePropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtDatePropertyManager provides and manages QDate properties.

    A date property has a current value, and a range specifying the
    valid dates. The range is defined by a minimum and a maximum
    value.

    The property's values can be retrieved using the minimum(),
    maximum() and value() functions, and can be set using the
    setMinimum(), setMaximum() and setValue() slots. Alternatively,
    the range can be defined in one go using the setRange() slot.

    In addition, QtDatePropertyManager provides the valueChanged() signal
    which is emitted whenever a property created by this manager
    changes, and the rangeChanged() signal which is emitted whenever
    such a property changes its range of valid dates.

    \sa QtAbstractPropertyManager, QtDateEditFactory, QtDateTimePropertyManager
*/

/*!
    \fn void QtDatePropertyManager::valueChanged(QtProperty *property, const QDate &value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the new
    \a value as parameters.

    \sa setValue()
*/

/*!
    \fn void QtDatePropertyManager::rangeChanged(QtProperty *property, const QDate &minimum, const QDate &maximum)

    This signal is emitted whenever a property created by this manager
    changes its range of valid dates, passing a pointer to the \a
    property and the new \a minimum and \a maximum dates.

    \sa setRange()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtDatePropertyManager::QtDatePropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtDatePropertyManagerPrivate( this ) )
{
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtDatePropertyManager::~QtDatePropertyManager()
{
    clear();
}

/*!
    Returns the given \a property's value.

    If the given \a property is not managed by \e this manager, this
    function returns an invalid date.

    \sa setValue()
*/
QDate QtDatePropertyManager::value( const QtProperty * property ) const
{
    return getValue< QDate >( d_ptr->_values, property );
}

/*!
    Returns the given \a  property's  minimum date.

    \sa maximum(), setRange()
*/
QDate QtDatePropertyManager::minimum( const QtProperty * property ) const
{
    return getMinimum< QDate >( d_ptr->_values, property );
}

/*!
    Returns the given \a property's maximum date.

    \sa minimum(), setRange()
*/
QDate QtDatePropertyManager::maximum( const QtProperty * property ) const
{
    return getMaximum< QDate >( d_ptr->_values, property );
}

/*!
    \reimp
*/
QString QtDatePropertyManager::valueText( const QtProperty * property ) const
{
    const QtDatePropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
    return it.value().val.toString( d_ptr->_format );
}

/*!
    \fn void QtDatePropertyManager::setValue(QtProperty *property, const QDate &value)

    Sets the value of the given \a property to \a value.

    If the specified \a value is not a valid date according to the
    given \a property's range, the value is adjusted to the nearest
    valid value within the range.

    \sa value(), setRange(), valueChanged()
*/
void QtDatePropertyManager::setValue( QtProperty * property, const QDate & val )
{
    void ( QtDatePropertyManagerPrivate::*setSubPropertyValue )( QtProperty *, const QDate & ) = 0;
    setValueInRange< const QDate &, QtDatePropertyManagerPrivate, QtDatePropertyManager, const QDate >( this, d_ptr.data(),
            &QtDatePropertyManager::propertyChanged,
            &QtDatePropertyManager::valueChanged,
            property, val, setSubPropertyValue );
}

/*!
    Sets the minimum value for the given \a property to \a minVal.

    When setting the minimum value, the maximum and current values are
    adjusted if necessary (ensuring that the range remains valid and
    that the current value is within in the range).

    \sa minimum(), setRange()
*/
void QtDatePropertyManager::setMinimum( QtProperty * property, const QDate & minVal )
{
    setMinimumValue< const QDate &, QtDatePropertyManagerPrivate, QtDatePropertyManager, QDate, QtDatePropertyManagerPrivate::Data >( this, d_ptr.data(),
            &QtDatePropertyManager::propertyChanged,
            &QtDatePropertyManager::valueChanged,
            &QtDatePropertyManager::rangeChanged,
            property, minVal );
}

/*!
    Sets the maximum value for the given \a property to \a maxVal.

    When setting the maximum value, the minimum and current
    values are adjusted if necessary (ensuring that the range remains
    valid and that the current value is within in the range).

    \sa maximum(), setRange()
*/
void QtDatePropertyManager::setMaximum( QtProperty * property, const QDate & maxVal )
{
    setMaximumValue< const QDate &, QtDatePropertyManagerPrivate, QtDatePropertyManager, QDate, QtDatePropertyManagerPrivate::Data >( this, d_ptr.data(),
            &QtDatePropertyManager::propertyChanged,
            &QtDatePropertyManager::valueChanged,
            &QtDatePropertyManager::rangeChanged,
            property, maxVal );
}

/*!
    \fn void QtDatePropertyManager::setRange(QtProperty *property, const QDate &minimum, const QDate &maximum)

    Sets the range of valid dates.

    This is a convenience function defining the range of valid dates
    in one go; setting the \a minimum and \a maximum values for the
    given \a property with a single function call.

    When setting a new date range, the current value is adjusted if
    necessary (ensuring that the value remains in date range).

    \sa setMinimum(), setMaximum(), rangeChanged()
*/
void QtDatePropertyManager::setRange( QtProperty * property, const QDate & minVal, const QDate & maxVal )
{
    void ( QtDatePropertyManagerPrivate::*setSubPropertyRange )( QtProperty *, const QDate &,
            const QDate &, const QDate & ) = 0;
    setBorderValues< const QDate &, QtDatePropertyManagerPrivate, QtDatePropertyManager, QDate >( this, d_ptr.data(),
            &QtDatePropertyManager::propertyChanged,
            &QtDatePropertyManager::valueChanged,
            &QtDatePropertyManager::rangeChanged,
            property, minVal, maxVal, setSubPropertyRange );
}

/*!
    \reimp
*/
void QtDatePropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = QtDatePropertyManagerPrivate::Data();
}

/*!
    \reimp
*/
void QtDatePropertyManager::uninitializeProperty( QtProperty * property )
{
    d_ptr->_values.remove( property );
}

// QtTimePropertyManager

class QtTimePropertyManagerPrivate
{
    QtTimePropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtTimePropertyManager )
public:
    explicit QtTimePropertyManagerPrivate( QtTimePropertyManager * q );
    
    const QString _format;
    
    typedef QMap< const QtProperty *, QTime > PropertyValueMap;
    PropertyValueMap _values;
};

QtTimePropertyManagerPrivate::QtTimePropertyManagerPrivate( QtTimePropertyManager * q ) :
    q_ptr( q ),
    _format( QtPropertyBrowserUtils::timeFormat() )
{
}

/*!
    \class QtTimePropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtTimePropertyManager provides and manages QTime properties.

    A time property's value can be retrieved using the value()
    function, and set using the setValue() slot.

    In addition, QtTimePropertyManager provides the valueChanged() signal
    which is emitted whenever a property created by this manager
    changes.

    \sa QtAbstractPropertyManager, QtTimeEditFactory
*/

/*!
    \fn void QtTimePropertyManager::valueChanged(QtProperty *property, const QTime &value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the
    new \a value as parameters.

    \sa setValue()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtTimePropertyManager::QtTimePropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtTimePropertyManagerPrivate( this ) )
{
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtTimePropertyManager::~QtTimePropertyManager()
{
    clear();
}

/*!
    Returns the given \a property's value.

    If the given property is not managed by this manager, this
    function returns an invalid time object.

    \sa setValue()
*/
QTime QtTimePropertyManager::value( const QtProperty * property ) const
{
    return d_ptr->_values.value( property, QTime() );
}

/*!
    \reimp
*/
QString QtTimePropertyManager::valueText( const QtProperty * property ) const
{
    const QtTimePropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
    return it.value().toString( d_ptr->_format );
}

/*!
    \fn void QtTimePropertyManager::setValue(QtProperty *property, const QTime &value)

    Sets the value of the given \a property to \a value.

    \sa value(), valueChanged()
*/
void QtTimePropertyManager::setValue( QtProperty * property, const QTime & val )
{
    setSimpleValue< const QTime &, QTime, QtTimePropertyManager >( d_ptr->_values, this,
            &QtTimePropertyManager::propertyChanged,
            &QtTimePropertyManager::valueChanged,
            property, val );
}

/*!
    \reimp
*/
void QtTimePropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = QTime::currentTime();
}

/*!
    \reimp
*/
void QtTimePropertyManager::uninitializeProperty( QtProperty * property )
{
    d_ptr->_values.remove( property );
}

// QtDateTimePropertyManager

class QtDateTimePropertyManagerPrivate
{
    QtDateTimePropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtDateTimePropertyManager )
public:
    explicit QtDateTimePropertyManagerPrivate( QtDateTimePropertyManager * q );
    
    const QString _format;
    
    typedef QMap< const QtProperty *, QDateTime > PropertyValueMap;
    PropertyValueMap _values;
};

QtDateTimePropertyManagerPrivate::QtDateTimePropertyManagerPrivate( QtDateTimePropertyManager * q ) :
    q_ptr( q ),
    _format( QtPropertyBrowserUtils::dateTimeFormat() )
{
}

/*! \class QtDateTimePropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtDateTimePropertyManager provides and manages QDateTime properties.

    A date and time property has a current value which can be
    retrieved using the value() function, and set using the setValue()
    slot. In addition, QtDateTimePropertyManager provides the
    valueChanged() signal which is emitted whenever a property created
    by this manager changes.

    \sa QtAbstractPropertyManager, QtDateTimeEditFactory, QtDatePropertyManager
*/

/*!
    \fn void QtDateTimePropertyManager::valueChanged(QtProperty *property, const QDateTime &value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the new
    \a value as parameters.
*/

/*!
    Creates a manager with the given \a parent.
*/
QtDateTimePropertyManager::QtDateTimePropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtDateTimePropertyManagerPrivate( this ) )
{
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtDateTimePropertyManager::~QtDateTimePropertyManager()
{
    clear();
}

/*!
    Returns the given \a property's value.

    If the given \a property is not managed by this manager, this
    function returns an invalid QDateTime object.

    \sa setValue()
*/
QDateTime QtDateTimePropertyManager::value( const QtProperty * property ) const
{
    return d_ptr->_values.value( property, QDateTime() );
}

/*!
    \reimp
*/
QString QtDateTimePropertyManager::valueText( const QtProperty * property ) const
{
    const QtDateTimePropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
    return it.value().toString( d_ptr->_format );
}

/*!
    \fn void QtDateTimePropertyManager::setValue(QtProperty *property, const QDateTime &value)

    Sets the value of the given \a property to \a value.

    \sa value(), valueChanged()
*/
void QtDateTimePropertyManager::setValue( QtProperty * property, const QDateTime & val )
{
    setSimpleValue< const QDateTime &, QDateTime, QtDateTimePropertyManager >( d_ptr->_values, this,
            &QtDateTimePropertyManager::propertyChanged,
            &QtDateTimePropertyManager::valueChanged,
            property, val );
}

/*!
    \reimp
*/
void QtDateTimePropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = QDateTime::currentDateTime();
}

/*!
    \reimp
*/
void QtDateTimePropertyManager::uninitializeProperty( QtProperty * property )
{
    d_ptr->_values.remove( property );
}

// QtKeySequencePropertyManager

class QtKeySequencePropertyManagerPrivate
{
    QtKeySequencePropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtKeySequencePropertyManager )
public:

    QString _format;
    
    typedef QMap< const QtProperty *, QKeySequence > PropertyValueMap;
    PropertyValueMap _values;
};

/*! \class QtKeySequencePropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtKeySequencePropertyManager provides and manages QKeySequence properties.

    A key sequence's value can be retrieved using the value()
    function, and set using the setValue() slot.

    In addition, QtKeySequencePropertyManager provides the valueChanged() signal
    which is emitted whenever a property created by this manager
    changes.

    \sa QtAbstractPropertyManager
*/

/*!
    \fn void QtKeySequencePropertyManager::valueChanged(QtProperty *property, const QKeySequence &value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the new
    \a value as parameters.
*/

/*!
    Creates a manager with the given \a parent.
*/
QtKeySequencePropertyManager::QtKeySequencePropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtKeySequencePropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtKeySequencePropertyManager::~QtKeySequencePropertyManager()
{
    clear();
}

/*!
    Returns the given \a property's value.

    If the given \a property is not managed by this manager, this
    function returns an empty QKeySequence object.

    \sa setValue()
*/
QKeySequence QtKeySequencePropertyManager::value( const QtProperty * property ) const
{
    return d_ptr->_values.value( property, QKeySequence() );
}

/*!
    \reimp
*/
QString QtKeySequencePropertyManager::valueText( const QtProperty * property ) const
{
    const QtKeySequencePropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
    return it.value().toString( QKeySequence::NativeText );
}

/*!
    \fn void QtKeySequencePropertyManager::setValue(QtProperty *property, const QKeySequence &value)

    Sets the value of the given \a property to \a value.

    \sa value(), valueChanged()
*/
void QtKeySequencePropertyManager::setValue( QtProperty * property, const QKeySequence & val )
{
    setSimpleValue< const QKeySequence &, QKeySequence, QtKeySequencePropertyManager >( d_ptr->_values, this,
            &QtKeySequencePropertyManager::propertyChanged,
            &QtKeySequencePropertyManager::valueChanged,
            property, val );
}

/*!
    \reimp
*/
void QtKeySequencePropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = QKeySequence();
}

/*!
    \reimp
*/
void QtKeySequencePropertyManager::uninitializeProperty( QtProperty * property )
{
    d_ptr->_values.remove( property );
}

// QtCharPropertyManager

class QtCharPropertyManagerPrivate
{
    QtCharPropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtCharPropertyManager )
public:

    typedef QMap< const QtProperty *, QChar > PropertyValueMap;
    PropertyValueMap _values;
};

/*! \class QtCharPropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtCharPropertyManager provides and manages QChar properties.

    A char's value can be retrieved using the value()
    function, and set using the setValue() slot.

    In addition, QtCharPropertyManager provides the valueChanged() signal
    which is emitted whenever a property created by this manager
    changes.

    \sa QtAbstractPropertyManager
*/

/*!
    \fn void QtCharPropertyManager::valueChanged(QtProperty *property, const QChar &value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the new
    \a value as parameters.
*/

/*!
    Creates a manager with the given \a parent.
*/
QtCharPropertyManager::QtCharPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtCharPropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtCharPropertyManager::~QtCharPropertyManager()
{
    clear();
}

/*!
    Returns the given \a property's value.

    If the given \a property is not managed by this manager, this
    function returns an null QChar object.

    \sa setValue()
*/
QChar QtCharPropertyManager::value( const QtProperty * property ) const
{
    return d_ptr->_values.value( property, QChar() );
}

/*!
    \reimp
*/
QString QtCharPropertyManager::valueText( const QtProperty * property ) const
{
    const QtCharPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
    const QChar c = it.value();
    return c.isNull() ? QString() : QString( c );
}

/*!
    \fn void QtCharPropertyManager::setValue(QtProperty *property, const QChar &value)

    Sets the value of the given \a property to \a value.

    \sa value(), valueChanged()
*/
void QtCharPropertyManager::setValue( QtProperty * property, const QChar & val )
{
    setSimpleValue< const QChar &, QChar, QtCharPropertyManager >( d_ptr->_values, this,
            &QtCharPropertyManager::propertyChanged,
            &QtCharPropertyManager::valueChanged,
            property, val );
}

/*!
    \reimp
*/
void QtCharPropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = QChar();
}

/*!
    \reimp
*/
void QtCharPropertyManager::uninitializeProperty( QtProperty * property )
{
    d_ptr->_values.remove( property );
}

// QtLocalePropertyManager

class QtLocalePropertyManagerPrivate
{
    QtLocalePropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtLocalePropertyManager )
public:

    QtLocalePropertyManagerPrivate();
    
    void slotEnumChanged( QtProperty * property, int value );
    void slotPropertyDestroyed( QtProperty * property );
    
    typedef QMap< const QtProperty *, QLocale > PropertyValueMap;
    PropertyValueMap _values;
    
    QtEnumPropertyManager * _enumPropertyManager;
    
    QMap< const QtProperty *, QtProperty * > _propertyToLanguage;
    QMap< const QtProperty *, QtProperty * > _propertyToCountry;
    
    QMap< const QtProperty *, QtProperty * > _languageToProperty;
    QMap< const QtProperty *, QtProperty * > _countryToProperty;
};

QtLocalePropertyManagerPrivate::QtLocalePropertyManagerPrivate()
{
}

void QtLocalePropertyManagerPrivate::slotEnumChanged( QtProperty * property, int value )
{
    if( QtProperty * langProp = _languageToProperty.value( property, 0 ) )
    {
        const QLocale loc = _values[langProp];
        QLocale::Language newLanguage = loc.language();
        QLocale::Country newCountry = loc.country();
        metaEnumProvider()->indexToLocale( value, 0, &newLanguage, 0 );
        QLocale newLoc( newLanguage, newCountry );
        q_ptr->setValue( langProp, newLoc );
    }
    else if( QtProperty * countryProp = _countryToProperty.value( property, 0 ) )
    {
        const QLocale loc = _values[countryProp];
        QLocale::Language newLanguage = loc.language();
        QLocale::Country newCountry = loc.country();
        metaEnumProvider()->indexToLocale( _enumPropertyManager->value( _propertyToLanguage.value( countryProp ) ), value, &newLanguage, &newCountry );
        QLocale newLoc( newLanguage, newCountry );
        q_ptr->setValue( countryProp, newLoc );
    }
}

void QtLocalePropertyManagerPrivate::slotPropertyDestroyed( QtProperty * property )
{
    if( QtProperty * subLangProp = _languageToProperty.value( property, 0 ) )
    {
        _propertyToLanguage[subLangProp] = 0;
        _languageToProperty.remove( property );
    }
    else if( QtProperty * subCountryProp = _countryToProperty.value( property, 0 ) )
    {
        _propertyToCountry[subCountryProp] = 0;
        _countryToProperty.remove( property );
    }
}

/*!
    \class QtLocalePropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtLocalePropertyManager provides and manages QLocale properties.

    A locale property has nested \e language and \e country
    subproperties. The top-level property's value can be retrieved
    using the value() function, and set using the setValue() slot.

    The subproperties are created by QtEnumPropertyManager object.
    These submanager can be retrieved using the subEnumPropertyManager()
    function. In order to provide editing widgets for the subproperties
    in a property browser widget, this manager must be associated with editor factory.

    In addition, QtLocalePropertyManager provides the valueChanged()
    signal which is emitted whenever a property created by this
    manager changes.

    \sa QtAbstractPropertyManager, QtEnumPropertyManager
*/

/*!
    \fn void QtLocalePropertyManager::valueChanged(QtProperty *property, const QLocale &value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the
    new \a value as parameters.

    \sa setValue()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtLocalePropertyManager::QtLocalePropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtLocalePropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
    
    d_ptr->_enumPropertyManager = new QtEnumPropertyManager( this );
    connect( d_ptr->_enumPropertyManager, SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotEnumChanged( QtProperty *, int ) ) );
             
    connect( d_ptr->_enumPropertyManager, SIGNAL( propertyDestroyed( QtProperty * ) ),
             this, SLOT( slotPropertyDestroyed( QtProperty * ) ) );
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtLocalePropertyManager::~QtLocalePropertyManager()
{
    clear();
}

/*!
    Returns the manager that creates the nested \e language
    and \e country subproperties.

    In order to provide editing widgets for the mentioned subproperties
    in a property browser widget, this manager must be associated with
    an editor factory.

    \sa QtAbstractPropertyBrowser::setFactoryForManager()
*/
QtEnumPropertyManager * QtLocalePropertyManager::subEnumPropertyManager() const
{
    return d_ptr->_enumPropertyManager;
}

/*!
    Returns the given \a property's value.

    If the given property is not managed by this manager, this
    function returns the default locale.

    \sa setValue()
*/
QLocale QtLocalePropertyManager::value( const QtProperty * property ) const
{
    return d_ptr->_values.value( property, QLocale() );
}

/*!
    \reimp
*/
QString QtLocalePropertyManager::valueText( const QtProperty * property ) const
{
    const QtLocalePropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
        
    const QLocale loc = it.value();
    
    int langIdx = 0;
    int countryIdx = 0;
    const QtMetaEnumProvider * me = metaEnumProvider();
    me->localeToIndex( loc.language(), loc.country(), &langIdx, &countryIdx );
    if( langIdx < 0 )
    {
        qWarning( "QtLocalePropertyManager::valueText: Unknown language %d", loc.language() );
        return tr( "<Invalid>" );
    }
    const QString languageName = me->languageEnumNames().at( langIdx );
    if( countryIdx < 0 )
    {
        qWarning( "QtLocalePropertyManager::valueText: Unknown country %d for %s", loc.country(), qUtf8Printable( languageName ) );
        return languageName;
    }
    const QString countryName = me->countryEnumNames( loc.language() ).at( countryIdx );
    return tr( "%1, %2" ).arg( languageName, countryName );
}

/*!
    \fn void QtLocalePropertyManager::setValue(QtProperty *property, const QLocale &value)

    Sets the value of the given \a property to \a value. Nested
    properties are updated automatically.

    \sa value(), valueChanged()
*/
void QtLocalePropertyManager::setValue( QtProperty * property, const QLocale & val )
{
    const QtLocalePropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    const QLocale loc = it.value();
    if( loc == val )
        return;
        
    it.value() = val;
    
    int langIdx = 0;
    int countryIdx = 0;
    metaEnumProvider()->localeToIndex( val.language(), val.country(), &langIdx, &countryIdx );
    if( loc.language() != val.language() )
    {
        d_ptr->_enumPropertyManager->setValue( d_ptr->_propertyToLanguage.value( property ), langIdx );
        d_ptr->_enumPropertyManager->setEnumNames( d_ptr->_propertyToCountry.value( property ),
                metaEnumProvider()->countryEnumNames( val.language() ) );
    }
    d_ptr->_enumPropertyManager->setValue( d_ptr->_propertyToCountry.value( property ), countryIdx );
    
    emit propertyChanged( property );
    emit valueChanged( property, val );
}

/*!
    \reimp
*/
void QtLocalePropertyManager::initializeProperty( QtProperty * property )
{
    QLocale val;
    d_ptr->_values[property] = val;
    
    int langIdx = 0;
    int countryIdx = 0;
    metaEnumProvider()->localeToIndex( val.language(), val.country(), &langIdx, &countryIdx );
    
    QtProperty * languageProp = d_ptr->_enumPropertyManager->addProperty();
    languageProp->setPropertyName( tr( "Language" ) );
    d_ptr->_enumPropertyManager->setEnumNames( languageProp, metaEnumProvider()->languageEnumNames() );
    d_ptr->_enumPropertyManager->setValue( languageProp, langIdx );
    d_ptr->_propertyToLanguage[property] = languageProp;
    d_ptr->_languageToProperty[languageProp] = property;
    property->addSubProperty( languageProp );
    
    QtProperty * countryProp = d_ptr->_enumPropertyManager->addProperty();
    countryProp->setPropertyName( tr( "Country" ) );
    d_ptr->_enumPropertyManager->setEnumNames( countryProp, metaEnumProvider()->countryEnumNames( val.language() ) );
    d_ptr->_enumPropertyManager->setValue( countryProp, countryIdx );
    d_ptr->_propertyToCountry[property] = countryProp;
    d_ptr->_countryToProperty[countryProp] = property;
    property->addSubProperty( countryProp );
}

/*!
    \reimp
*/
void QtLocalePropertyManager::uninitializeProperty( QtProperty * property )
{
    QtProperty * languageProp = d_ptr->_propertyToLanguage[property];
    if( languageProp )
    {
        d_ptr->_languageToProperty.remove( languageProp );
        delete languageProp;
    }
    d_ptr->_propertyToLanguage.remove( property );
    
    QtProperty * countryProp = d_ptr->_propertyToCountry[property];
    if( countryProp )
    {
        d_ptr->_countryToProperty.remove( countryProp );
        delete countryProp;
    }
    d_ptr->_propertyToCountry.remove( property );
    
    d_ptr->_values.remove( property );
}

// QtPointPropertyManager

class QtPointPropertyManagerPrivate
{
    QtPointPropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtPointPropertyManager )
public:

    void slotIntChanged( QtProperty * property, int value );
    void slotPropertyDestroyed( QtProperty * property );
    
    typedef QMap< const QtProperty *, QPoint > PropertyValueMap;
    PropertyValueMap _values;
    
    QtIntPropertyManager * _intPropertyManager;
    
    QMap< const QtProperty *, QtProperty * > _propertyToX;
    QMap< const QtProperty *, QtProperty * > _propertyToY;
    
    QMap< const QtProperty *, QtProperty * > _xToProperty;
    QMap< const QtProperty *, QtProperty * > _yToProperty;
};

void QtPointPropertyManagerPrivate::slotIntChanged( QtProperty * property, int value )
{
    if( QtProperty * xprop = _xToProperty.value( property, 0 ) )
    {
        QPoint p = _values[xprop];
        p.setX( value );
        q_ptr->setValue( xprop, p );
    }
    else if( QtProperty * yprop = _yToProperty.value( property, 0 ) )
    {
        QPoint p = _values[yprop];
        p.setY( value );
        q_ptr->setValue( yprop, p );
    }
}

void QtPointPropertyManagerPrivate::slotPropertyDestroyed( QtProperty * property )
{
    if( QtProperty * pointXProp = _xToProperty.value( property, 0 ) )
    {
        _propertyToX[pointXProp] = 0;
        _xToProperty.remove( property );
    }
    else if( QtProperty * pointYProp = _yToProperty.value( property, 0 ) )
    {
        _propertyToY[pointYProp] = 0;
        _yToProperty.remove( property );
    }
}

/*! \class QtPointPropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtPointPropertyManager provides and manages QPoint properties.

    A point property has nested \e x and \e y subproperties. The
    top-level property's value can be retrieved using the value()
    function, and set using the setValue() slot.

    The subproperties are created by a QtIntPropertyManager object. This
    manager can be retrieved using the subIntPropertyManager() function. In
    order to provide editing widgets for the subproperties in a
    property browser widget, this manager must be associated with an
    editor factory.

    In addition, QtPointPropertyManager provides the valueChanged() signal which
    is emitted whenever a property created by this manager changes.

    \sa QtAbstractPropertyManager, QtIntPropertyManager, QtPointFPropertyManager
*/

/*!
    \fn void QtPointPropertyManager::valueChanged(QtProperty *property, const QPoint &value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the
    new \a value as parameters.

    \sa setValue()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtPointPropertyManager::QtPointPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtPointPropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
    
    d_ptr->_intPropertyManager = new QtIntPropertyManager( this );
    connect( d_ptr->_intPropertyManager, SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotIntChanged( QtProperty *, int ) ) );
    connect( d_ptr->_intPropertyManager, SIGNAL( propertyDestroyed( QtProperty * ) ),
             this, SLOT( slotPropertyDestroyed( QtProperty * ) ) );
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtPointPropertyManager::~QtPointPropertyManager()
{
    clear();
}

/*!
    Returns the manager that creates the nested \e x and \e y
    subproperties.

    In order to provide editing widgets for the subproperties in a
    property browser widget, this manager must be associated with an
    editor factory.

    \sa QtAbstractPropertyBrowser::setFactoryForManager()
*/
QtIntPropertyManager * QtPointPropertyManager::subIntPropertyManager() const
{
    return d_ptr->_intPropertyManager;
}

/*!
    Returns the given \a property's value.

    If the given \a property is not managed by this manager, this
    function returns a point with coordinates (0, 0).

    \sa setValue()
*/
QPoint QtPointPropertyManager::value( const QtProperty * property ) const
{
    return d_ptr->_values.value( property, QPoint() );
}

/*!
    \reimp
*/
QString QtPointPropertyManager::valueText( const QtProperty * property ) const
{
    const QtPointPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
    const QPoint v = it.value();
    return tr( "(%1, %2)" ).arg( QString::number( v.x() ) )
           .arg( QString::number( v.y() ) );
}

/*!
    \fn void QtPointPropertyManager::setValue(QtProperty *property, const QPoint &value)

    Sets the value of the given \a property to \a value. Nested
    properties are updated automatically.

    \sa value(), valueChanged()
*/
void QtPointPropertyManager::setValue( QtProperty * property, const QPoint & val )
{
    const QtPointPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    if( it.value() == val )
        return;
        
    it.value() = val;
    d_ptr->_intPropertyManager->setValue( d_ptr->_propertyToX[property], val.x() );
    d_ptr->_intPropertyManager->setValue( d_ptr->_propertyToY[property], val.y() );
    
    emit propertyChanged( property );
    emit valueChanged( property, val );
}

/*!
    \reimp
*/
void QtPointPropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = QPoint( 0, 0 );
    
    QtProperty * xProp = d_ptr->_intPropertyManager->addProperty();
    xProp->setPropertyName( tr( "X" ) );
    d_ptr->_intPropertyManager->setValue( xProp, 0 );
    d_ptr->_propertyToX[property] = xProp;
    d_ptr->_xToProperty[xProp] = property;
    property->addSubProperty( xProp );
    
    QtProperty * yProp = d_ptr->_intPropertyManager->addProperty();
    yProp->setPropertyName( tr( "Y" ) );
    d_ptr->_intPropertyManager->setValue( yProp, 0 );
    d_ptr->_propertyToY[property] = yProp;
    d_ptr->_yToProperty[yProp] = property;
    property->addSubProperty( yProp );
}

/*!
    \reimp
*/
void QtPointPropertyManager::uninitializeProperty( QtProperty * property )
{
    QtProperty * xProp = d_ptr->_propertyToX[property];
    if( xProp )
    {
        d_ptr->_xToProperty.remove( xProp );
        delete xProp;
    }
    d_ptr->_propertyToX.remove( property );
    
    QtProperty * yProp = d_ptr->_propertyToY[property];
    if( yProp )
    {
        d_ptr->_yToProperty.remove( yProp );
        delete yProp;
    }
    d_ptr->_propertyToY.remove( property );
    
    d_ptr->_values.remove( property );
}

// QtPointFPropertyManager

class QtPointFPropertyManagerPrivate
{
    QtPointFPropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtPointFPropertyManager )
public:

    struct Data
    {
        Data() : decimals( 2 ) {}
        QPointF val;
        int decimals;
    };
    
    void slotDoubleChanged( QtProperty * property, double value );
    void slotPropertyDestroyed( QtProperty * property );
    
    typedef QMap< const QtProperty *, Data > PropertyValueMap;
    PropertyValueMap _values;
    
    QtDoublePropertyManager * _doublePropertyManager;
    
    QMap< const QtProperty *, QtProperty * > _propertyToX;
    QMap< const QtProperty *, QtProperty * > _propertyToY;
    
    QMap< const QtProperty *, QtProperty * > _xToProperty;
    QMap< const QtProperty *, QtProperty * > _yToProperty;
};

void QtPointFPropertyManagerPrivate::slotDoubleChanged( QtProperty * property, double value )
{
    if( QtProperty * xProp = _xToProperty.value( property, 0 ) )
    {
        QPointF p = _values[xProp].val;
        p.setX( value );
        q_ptr->setValue( xProp, p );
    }
    else if( QtProperty * yProp = _yToProperty.value( property, 0 ) )
    {
        QPointF p = _values[yProp].val;
        p.setY( value );
        q_ptr->setValue( yProp, p );
    }
}

void QtPointFPropertyManagerPrivate::slotPropertyDestroyed( QtProperty * property )
{
    if( QtProperty * pointXProp  = _xToProperty.value( property, 0 ) )
    {
        _propertyToX[pointXProp] = 0;
        _xToProperty.remove( property );
    }
    else if( QtProperty * pointYProp = _yToProperty.value( property, 0 ) )
    {
        _propertyToY[pointYProp] = 0;
        _yToProperty.remove( property );
    }
}

/*! \class QtPointFPropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtPointFPropertyManager provides and manages QPointF properties.

    A point property has nested \e x and \e y subproperties. The
    top-level property's value can be retrieved using the value()
    function, and set using the setValue() slot.

    The subproperties are created by a QtDoublePropertyManager object. This
    manager can be retrieved using the subDoublePropertyManager() function. In
    order to provide editing widgets for the subproperties in a
    property browser widget, this manager must be associated with an
    editor factory.

    In addition, QtPointFPropertyManager provides the valueChanged() signal which
    is emitted whenever a property created by this manager changes.

    \sa QtAbstractPropertyManager, QtDoublePropertyManager, QtPointPropertyManager
*/

/*!
    \fn void QtPointFPropertyManager::valueChanged(QtProperty *property, const QPointF &value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the
    new \a value as parameters.

    \sa setValue()
*/

/*!
    \fn void QtPointFPropertyManager::decimalsChanged(QtProperty *property, int prec)

    This signal is emitted whenever a property created by this manager
    changes its precision of value, passing a pointer to the
    \a property and the new \a prec value

    \sa setDecimals()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtPointFPropertyManager::QtPointFPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtPointFPropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
    
    d_ptr->_doublePropertyManager = new QtDoublePropertyManager( this );
    connect( d_ptr->_doublePropertyManager, SIGNAL( valueChanged( QtProperty *, double ) ),
             this, SLOT( slotDoubleChanged( QtProperty *, double ) ) );
    connect( d_ptr->_doublePropertyManager, SIGNAL( propertyDestroyed( QtProperty * ) ),
             this, SLOT( slotPropertyDestroyed( QtProperty * ) ) );
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtPointFPropertyManager::~QtPointFPropertyManager()
{
    clear();
}

/*!
    Returns the manager that creates the nested \e x and \e y
    subproperties.

    In order to provide editing widgets for the subproperties in a
    property browser widget, this manager must be associated with an
    editor factory.

    \sa QtAbstractPropertyBrowser::setFactoryForManager()
*/
QtDoublePropertyManager * QtPointFPropertyManager::subDoublePropertyManager() const
{
    return d_ptr->_doublePropertyManager;
}

/*!
    Returns the given \a property's value.

    If the given \a property is not managed by this manager, this
    function returns a point with coordinates (0, 0).

    \sa setValue()
*/
QPointF QtPointFPropertyManager::value( const QtProperty * property ) const
{
    return getValue< QPointF >( d_ptr->_values, property );
}

/*!
    Returns the given \a property's precision, in decimals.

    \sa setDecimals()
*/
int QtPointFPropertyManager::decimals( const QtProperty * property ) const
{
    return getData< int >( d_ptr->_values, &QtPointFPropertyManagerPrivate::Data::decimals, property, 0 );
}

/*!
    \reimp
*/
QString QtPointFPropertyManager::valueText( const QtProperty * property ) const
{
    const QtPointFPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
    const QPointF v = it.value().val;
    const int dec =  it.value().decimals;
    return tr( "(%1, %2)" ).arg( QString::number( v.x(), 'f', dec ) )
           .arg( QString::number( v.y(), 'f', dec ) );
}

/*!
    \fn void QtPointFPropertyManager::setValue(QtProperty *property, const QPointF &value)

    Sets the value of the given \a property to \a value. Nested
    properties are updated automatically.

    \sa value(), valueChanged()
*/
void QtPointFPropertyManager::setValue( QtProperty * property, const QPointF & val )
{
    const QtPointFPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    if( it.value().val == val )
        return;
        
    it.value().val = val;
    d_ptr->_doublePropertyManager->setValue( d_ptr->_propertyToX[property], val.x() );
    d_ptr->_doublePropertyManager->setValue( d_ptr->_propertyToY[property], val.y() );
    
    emit propertyChanged( property );
    emit valueChanged( property, val );
}

/*!
    \fn void QtPointFPropertyManager::setDecimals(QtProperty *property, int prec)

    Sets the precision of the given \a property to \a prec.

    The valid decimal range is 0-13. The default is 2.

    \sa decimals()
*/
void QtPointFPropertyManager::setDecimals( QtProperty * property, int prec )
{
    const QtPointFPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    QtPointFPropertyManagerPrivate::Data data = it.value();
    
    if( prec > 13 )
        prec = 13;
    else if( prec < 0 )
        prec = 0;
        
    if( data.decimals == prec )
        return;
        
    data.decimals = prec;
    d_ptr->_doublePropertyManager->setDecimals( d_ptr->_propertyToX[property], prec );
    d_ptr->_doublePropertyManager->setDecimals( d_ptr->_propertyToY[property], prec );
    
    it.value() = data;
    
    emit decimalsChanged( property, data.decimals );
}

/*!
    \reimp
*/
void QtPointFPropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = QtPointFPropertyManagerPrivate::Data();
    
    QtProperty * xProp = d_ptr->_doublePropertyManager->addProperty();
    xProp->setPropertyName( tr( "X" ) );
    d_ptr->_doublePropertyManager->setDecimals( xProp, decimals( property ) );
    d_ptr->_doublePropertyManager->setValue( xProp, 0 );
    d_ptr->_propertyToX[property] = xProp;
    d_ptr->_xToProperty[xProp] = property;
    property->addSubProperty( xProp );
    
    QtProperty * yProp = d_ptr->_doublePropertyManager->addProperty();
    yProp->setPropertyName( tr( "Y" ) );
    d_ptr->_doublePropertyManager->setDecimals( yProp, decimals( property ) );
    d_ptr->_doublePropertyManager->setValue( yProp, 0 );
    d_ptr->_propertyToY[property] = yProp;
    d_ptr->_yToProperty[yProp] = property;
    property->addSubProperty( yProp );
}

/*!
    \reimp
*/
void QtPointFPropertyManager::uninitializeProperty( QtProperty * property )
{
    QtProperty * xProp = d_ptr->_propertyToX[property];
    if( xProp )
    {
        d_ptr->_xToProperty.remove( xProp );
        delete xProp;
    }
    d_ptr->_propertyToX.remove( property );
    
    QtProperty * yProp = d_ptr->_propertyToY[property];
    if( yProp )
    {
        d_ptr->_yToProperty.remove( yProp );
        delete yProp;
    }
    d_ptr->_propertyToY.remove( property );
    
    d_ptr->_values.remove( property );
}

// QtSizePropertyManager

class QtSizePropertyManagerPrivate
{
    QtSizePropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtSizePropertyManager )
public:

    void slotIntChanged( QtProperty * property, int value );
    void slotPropertyDestroyed( QtProperty * property );
    void setValue( QtProperty * property, const QSize & val );
    void setRange( QtProperty * property,
                   const QSize & minVal, const QSize & maxVal, const QSize & val );
                   
    struct Data
    {
        Data() : val( QSize( 0, 0 ) ), minVal( QSize( 0, 0 ) ), maxVal( QSize( INT_MAX, INT_MAX ) ) {}
        QSize val;
        QSize minVal;
        QSize maxVal;
        QSize minimumValue() const { return minVal; }
        QSize maximumValue() const { return maxVal; }
        void setMinimumValue( const QSize & newMinVal ) { setSizeMinimumData( this, newMinVal ); }
        void setMaximumValue( const QSize & newMaxVal ) { setSizeMaximumData( this, newMaxVal ); }
    };
    
    typedef QMap< const QtProperty *, Data > PropertyValueMap;
    PropertyValueMap _values;
    
    QtIntPropertyManager * _intPropertyManager;
    
    QMap< const QtProperty *, QtProperty * > _propertyToW;
    QMap< const QtProperty *, QtProperty * > _propertyToH;
    
    QMap< const QtProperty *, QtProperty * > _wToProperty;
    QMap< const QtProperty *, QtProperty * > _hToProperty;
};

void QtSizePropertyManagerPrivate::slotIntChanged( QtProperty * property, int value )
{
    if( QtProperty * wProp = _wToProperty.value( property, 0 ) )
    {
        QSize s = _values[wProp].val;
        s.setWidth( value );
        q_ptr->setValue( wProp, s );
    }
    else if( QtProperty * hProp = _hToProperty.value( property, 0 ) )
    {
        QSize s = _values[hProp].val;
        s.setHeight( value );
        q_ptr->setValue( hProp, s );
    }
}

void QtSizePropertyManagerPrivate::slotPropertyDestroyed( QtProperty * property )
{
    if( QtProperty * pointWProp = _wToProperty.value( property, 0 ) )
    {
        _propertyToW[pointWProp] = 0;
        _wToProperty.remove( property );
    }
    else if( QtProperty * pointHProp = _hToProperty.value( property, 0 ) )
    {
        _propertyToH[pointHProp] = 0;
        _hToProperty.remove( property );
    }
}

void QtSizePropertyManagerPrivate::setValue( QtProperty * property, const QSize & val )
{
    _intPropertyManager->setValue( _propertyToW.value( property ), val.width() );
    _intPropertyManager->setValue( _propertyToH.value( property ), val.height() );
}

void QtSizePropertyManagerPrivate::setRange( QtProperty * property,
        const QSize & minVal, const QSize & maxVal, const QSize & val )
{
    QtProperty * wProperty = _propertyToW.value( property );
    QtProperty * hProperty = _propertyToH.value( property );
    _intPropertyManager->setRange( wProperty, minVal.width(), maxVal.width() );
    _intPropertyManager->setValue( wProperty, val.width() );
    _intPropertyManager->setRange( hProperty, minVal.height(), maxVal.height() );
    _intPropertyManager->setValue( hProperty, val.height() );
}

/*!
    \class QtSizePropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtSizePropertyManager provides and manages QSize properties.

    A size property has nested \e width and \e height
    subproperties. The top-level property's value can be retrieved
    using the value() function, and set using the setValue() slot.

    The subproperties are created by a QtIntPropertyManager object. This
    manager can be retrieved using the subIntPropertyManager() function. In
    order to provide editing widgets for the subproperties in a
    property browser widget, this manager must be associated with an
    editor factory.

    A size property also has a range of valid values defined by a
    minimum size and a maximum size. These sizes can be retrieved
    using the minimum() and the maximum() functions, and set using the
    setMinimum() and setMaximum() slots. Alternatively, the range can
    be defined in one go using the setRange() slot.

    In addition, QtSizePropertyManager provides the valueChanged() signal
    which is emitted whenever a property created by this manager
    changes, and the rangeChanged() signal which is emitted whenever
    such a property changes its range of valid sizes.

    \sa QtAbstractPropertyManager, QtIntPropertyManager, QtSizeFPropertyManager
*/

/*!
    \fn void QtSizePropertyManager::valueChanged(QtProperty *property, const QSize &value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the new
    \a value as parameters.

    \sa setValue()
*/

/*!
    \fn void QtSizePropertyManager::rangeChanged(QtProperty *property, const QSize &minimum, const QSize &maximum)

    This signal is emitted whenever a property created by this manager
    changes its range of valid sizes, passing a pointer to the \a
    property and the new \a minimum and \a maximum sizes.

    \sa setRange()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtSizePropertyManager::QtSizePropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtSizePropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
    
    d_ptr->_intPropertyManager = new QtIntPropertyManager( this );
    connect( d_ptr->_intPropertyManager, SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotIntChanged( QtProperty *, int ) ) );
    connect( d_ptr->_intPropertyManager, SIGNAL( propertyDestroyed( QtProperty * ) ),
             this, SLOT( slotPropertyDestroyed( QtProperty * ) ) );
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtSizePropertyManager::~QtSizePropertyManager()
{
    clear();
}

/*!
    Returns the manager that creates the nested \e width and \e height
    subproperties.

    In order to provide editing widgets for the \e width and \e height
    properties in a property browser widget, this manager must be
    associated with an editor factory.

    \sa QtAbstractPropertyBrowser::setFactoryForManager()
*/
QtIntPropertyManager * QtSizePropertyManager::subIntPropertyManager() const
{
    return d_ptr->_intPropertyManager;
}

/*!
    Returns the given \a property's value.

    If the given \a property is not managed by this manager, this
    function returns an invalid size

    \sa setValue()
*/
QSize QtSizePropertyManager::value( const QtProperty * property ) const
{
    return getValue< QSize >( d_ptr->_values, property );
}

/*!
    Returns the given \a property's minimum size value.

    \sa setMinimum(), maximum(), setRange()
*/
QSize QtSizePropertyManager::minimum( const QtProperty * property ) const
{
    return getMinimum< QSize >( d_ptr->_values, property );
}

/*!
    Returns the given \a property's maximum size value.

    \sa setMaximum(), minimum(), setRange()
*/
QSize QtSizePropertyManager::maximum( const QtProperty * property ) const
{
    return getMaximum< QSize >( d_ptr->_values, property );
}

/*!
    \reimp
*/
QString QtSizePropertyManager::valueText( const QtProperty * property ) const
{
    const QtSizePropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
    const QSize v = it.value().val;
    return tr( "%1 x %2" ).arg( QString::number( v.width() ) )
           .arg( QString::number( v.height() ) );
}

/*!
    \fn void QtSizePropertyManager::setValue(QtProperty *property, const QSize &value)

    Sets the value of the given \a property to \a value.

    If the specified \a value is not valid according to the given \a
    property's size range, the \a value is adjusted to the nearest
    valid value within the size range.

    \sa value(), setRange(), valueChanged()
*/
void QtSizePropertyManager::setValue( QtProperty * property, const QSize & val )
{
    setValueInRange< const QSize &, QtSizePropertyManagerPrivate, QtSizePropertyManager, const QSize >( this, d_ptr.data(),
            &QtSizePropertyManager::propertyChanged,
            &QtSizePropertyManager::valueChanged,
            property, val, &QtSizePropertyManagerPrivate::setValue );
}

/*!
    Sets the minimum size value for the given \a property to \a minVal.

    When setting the minimum size value, the maximum and current
    values are adjusted if necessary (ensuring that the size range
    remains valid and that the current value is within the range).

    \sa minimum(), setRange(), rangeChanged()
*/
void QtSizePropertyManager::setMinimum( QtProperty * property, const QSize & minVal )
{
    setBorderValue< const QSize &, QtSizePropertyManagerPrivate, QtSizePropertyManager, QSize, QtSizePropertyManagerPrivate::Data >( this, d_ptr.data(),
            &QtSizePropertyManager::propertyChanged,
            &QtSizePropertyManager::valueChanged,
            &QtSizePropertyManager::rangeChanged,
            property,
            &QtSizePropertyManagerPrivate::Data::minimumValue,
            &QtSizePropertyManagerPrivate::Data::setMinimumValue,
            minVal, &QtSizePropertyManagerPrivate::setRange );
}

/*!
    Sets the maximum size value for the given \a property to \a maxVal.

    When setting the maximum size value, the minimum and current
    values are adjusted if necessary (ensuring that the size range
    remains valid and that the current value is within the range).

    \sa maximum(), setRange(), rangeChanged()
*/
void QtSizePropertyManager::setMaximum( QtProperty * property, const QSize & maxVal )
{
    setBorderValue< const QSize &, QtSizePropertyManagerPrivate, QtSizePropertyManager, QSize, QtSizePropertyManagerPrivate::Data >( this, d_ptr.data(),
            &QtSizePropertyManager::propertyChanged,
            &QtSizePropertyManager::valueChanged,
            &QtSizePropertyManager::rangeChanged,
            property,
            &QtSizePropertyManagerPrivate::Data::maximumValue,
            &QtSizePropertyManagerPrivate::Data::setMaximumValue,
            maxVal, &QtSizePropertyManagerPrivate::setRange );
}

/*!
    \fn void QtSizePropertyManager::setRange(QtProperty *property, const QSize &minimum, const QSize &maximum)

    Sets the range of valid values.

    This is a convenience function defining the range of valid values
    in one go; setting the \a minimum and \a maximum values for the
    given \a property with a single function call.

    When setting a new range, the current value is adjusted if
    necessary (ensuring that the value remains within the range).

    \sa setMinimum(), setMaximum(), rangeChanged()
*/
void QtSizePropertyManager::setRange( QtProperty * property, const QSize & minVal, const QSize & maxVal )
{
    setBorderValues< const QSize &, QtSizePropertyManagerPrivate, QtSizePropertyManager, QSize >( this, d_ptr.data(),
            &QtSizePropertyManager::propertyChanged,
            &QtSizePropertyManager::valueChanged,
            &QtSizePropertyManager::rangeChanged,
            property, minVal, maxVal, &QtSizePropertyManagerPrivate::setRange );
}

/*!
    \reimp
*/
void QtSizePropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = QtSizePropertyManagerPrivate::Data();
    
    QtProperty * wProp = d_ptr->_intPropertyManager->addProperty();
    wProp->setPropertyName( tr( "Width" ) );
    d_ptr->_intPropertyManager->setValue( wProp, 0 );
    d_ptr->_intPropertyManager->setMinimum( wProp, 0 );
    d_ptr->_propertyToW[property] = wProp;
    d_ptr->_wToProperty[wProp] = property;
    property->addSubProperty( wProp );
    
    QtProperty * hProp = d_ptr->_intPropertyManager->addProperty();
    hProp->setPropertyName( tr( "Height" ) );
    d_ptr->_intPropertyManager->setValue( hProp, 0 );
    d_ptr->_intPropertyManager->setMinimum( hProp, 0 );
    d_ptr->_propertyToH[property] = hProp;
    d_ptr->_hToProperty[hProp] = property;
    property->addSubProperty( hProp );
}

/*!
    \reimp
*/
void QtSizePropertyManager::uninitializeProperty( QtProperty * property )
{
    QtProperty * wProp = d_ptr->_propertyToW[property];
    if( wProp )
    {
        d_ptr->_wToProperty.remove( wProp );
        delete wProp;
    }
    d_ptr->_propertyToW.remove( property );
    
    QtProperty * hProp = d_ptr->_propertyToH[property];
    if( hProp )
    {
        d_ptr->_hToProperty.remove( hProp );
        delete hProp;
    }
    d_ptr->_propertyToH.remove( property );
    
    d_ptr->_values.remove( property );
}

// QtSizeFPropertyManager

class QtSizeFPropertyManagerPrivate
{
    QtSizeFPropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtSizeFPropertyManager )
public:

    void slotDoubleChanged( QtProperty * property, double value );
    void slotPropertyDestroyed( QtProperty * property );
    void setValue( QtProperty * property, const QSizeF & val );
    void setRange( QtProperty * property,
                   const QSizeF & minVal, const QSizeF & maxVal, const QSizeF & val );
                   
    struct Data
    {
        Data() : val( QSizeF( 0, 0 ) ), minVal( QSizeF( 0, 0 ) ), maxVal( QSizeF( INT_MAX, INT_MAX ) ), decimals( 2 ) {}
        QSizeF val;
        QSizeF minVal;
        QSizeF maxVal;
        int decimals;
        QSizeF minimumValue() const { return minVal; }
        QSizeF maximumValue() const { return maxVal; }
        void setMinimumValue( const QSizeF & newMinVal ) { setSizeMinimumData( this, newMinVal ); }
        void setMaximumValue( const QSizeF & newMaxVal ) { setSizeMaximumData( this, newMaxVal ); }
    };
    
    typedef QMap< const QtProperty *, Data > PropertyValueMap;
    PropertyValueMap _values;
    
    QtDoublePropertyManager * _doublePropertyManager;
    
    QMap< const QtProperty *, QtProperty * > _propertyToW;
    QMap< const QtProperty *, QtProperty * > _propertyToH;
    
    QMap< const QtProperty *, QtProperty * > _wToProperty;
    QMap< const QtProperty *, QtProperty * > _hToProperty;
};

void QtSizeFPropertyManagerPrivate::slotDoubleChanged( QtProperty * property, double value )
{
    if( QtProperty * wProp = _wToProperty.value( property, 0 ) )
    {
        QSizeF s = _values[wProp].val;
        s.setWidth( value );
        q_ptr->setValue( wProp, s );
    }
    else if( QtProperty * hProp = _hToProperty.value( property, 0 ) )
    {
        QSizeF s = _values[hProp].val;
        s.setHeight( value );
        q_ptr->setValue( hProp, s );
    }
}

void QtSizeFPropertyManagerPrivate::slotPropertyDestroyed( QtProperty * property )
{
    if( QtProperty * pointWProp = _wToProperty.value( property, 0 ) )
    {
        _propertyToW[pointWProp] = 0;
        _wToProperty.remove( property );
    }
    else if( QtProperty * pointHProp = _hToProperty.value( property, 0 ) )
    {
        _propertyToH[pointHProp] = 0;
        _hToProperty.remove( property );
    }
}

void QtSizeFPropertyManagerPrivate::setValue( QtProperty * property, const QSizeF & val )
{
    _doublePropertyManager->setValue( _propertyToW.value( property ), val.width() );
    _doublePropertyManager->setValue( _propertyToH.value( property ), val.height() );
}

void QtSizeFPropertyManagerPrivate::setRange( QtProperty * property,
        const QSizeF & minVal, const QSizeF & maxVal, const QSizeF & val )
{
    _doublePropertyManager->setRange( _propertyToW[property], minVal.width(), maxVal.width() );
    _doublePropertyManager->setValue( _propertyToW[property], val.width() );
    _doublePropertyManager->setRange( _propertyToH[property], minVal.height(), maxVal.height() );
    _doublePropertyManager->setValue( _propertyToH[property], val.height() );
}

/*!
    \class QtSizeFPropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtSizeFPropertyManager provides and manages QSizeF properties.

    A size property has nested \e width and \e height
    subproperties. The top-level property's value can be retrieved
    using the value() function, and set using the setValue() slot.

    The subproperties are created by a QtDoublePropertyManager object. This
    manager can be retrieved using the subDoublePropertyManager() function. In
    order to provide editing widgets for the subproperties in a
    property browser widget, this manager must be associated with an
    editor factory.

    A size property also has a range of valid values defined by a
    minimum size and a maximum size. These sizes can be retrieved
    using the minimum() and the maximum() functions, and set using the
    setMinimum() and setMaximum() slots. Alternatively, the range can
    be defined in one go using the setRange() slot.

    In addition, QtSizeFPropertyManager provides the valueChanged() signal
    which is emitted whenever a property created by this manager
    changes, and the rangeChanged() signal which is emitted whenever
    such a property changes its range of valid sizes.

    \sa QtAbstractPropertyManager, QtDoublePropertyManager, QtSizePropertyManager
*/

/*!
    \fn void QtSizeFPropertyManager::valueChanged(QtProperty *property, const QSizeF &value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the new
    \a value as parameters.

    \sa setValue()
*/

/*!
    \fn void QtSizeFPropertyManager::rangeChanged(QtProperty *property, const QSizeF &minimum, const QSizeF &maximum)

    This signal is emitted whenever a property created by this manager
    changes its range of valid sizes, passing a pointer to the \a
    property and the new \a minimum and \a maximum sizes.

    \sa setRange()
*/

/*!
    \fn void QtSizeFPropertyManager::decimalsChanged(QtProperty *property, int prec)

    This signal is emitted whenever a property created by this manager
    changes its precision of value, passing a pointer to the
    \a property and the new \a prec value

    \sa setDecimals()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtSizeFPropertyManager::QtSizeFPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtSizeFPropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
    
    d_ptr->_doublePropertyManager = new QtDoublePropertyManager( this );
    connect( d_ptr->_doublePropertyManager, SIGNAL( valueChanged( QtProperty *, double ) ),
             this, SLOT( slotDoubleChanged( QtProperty *, double ) ) );
    connect( d_ptr->_doublePropertyManager, SIGNAL( propertyDestroyed( QtProperty * ) ),
             this, SLOT( slotPropertyDestroyed( QtProperty * ) ) );
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtSizeFPropertyManager::~QtSizeFPropertyManager()
{
    clear();
}

/*!
    Returns the manager that creates the nested \e width and \e height
    subproperties.

    In order to provide editing widgets for the \e width and \e height
    properties in a property browser widget, this manager must be
    associated with an editor factory.

    \sa QtAbstractPropertyBrowser::setFactoryForManager()
*/
QtDoublePropertyManager * QtSizeFPropertyManager::subDoublePropertyManager() const
{
    return d_ptr->_doublePropertyManager;
}

/*!
    Returns the given \a property's value.

    If the given \a property is not managed by this manager, this
    function returns an invalid size

    \sa setValue()
*/
QSizeF QtSizeFPropertyManager::value( const QtProperty * property ) const
{
    return getValue< QSizeF >( d_ptr->_values, property );
}

/*!
    Returns the given \a property's precision, in decimals.

    \sa setDecimals()
*/
int QtSizeFPropertyManager::decimals( const QtProperty * property ) const
{
    return getData< int >( d_ptr->_values, &QtSizeFPropertyManagerPrivate::Data::decimals, property, 0 );
}

/*!
    Returns the given \a property's minimum size value.

    \sa setMinimum(), maximum(), setRange()
*/
QSizeF QtSizeFPropertyManager::minimum( const QtProperty * property ) const
{
    return getMinimum< QSizeF >( d_ptr->_values, property );
}

/*!
    Returns the given \a property's maximum size value.

    \sa setMaximum(), minimum(), setRange()
*/
QSizeF QtSizeFPropertyManager::maximum( const QtProperty * property ) const
{
    return getMaximum< QSizeF >( d_ptr->_values, property );
}

/*!
    \reimp
*/
QString QtSizeFPropertyManager::valueText( const QtProperty * property ) const
{
    const QtSizeFPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
    const QSizeF v = it.value().val;
    const int dec = it.value().decimals;
    return tr( "%1 x %2" ).arg( QString::number( v.width(), 'f', dec ) )
           .arg( QString::number( v.height(), 'f', dec ) );
}

/*!
    \fn void QtSizeFPropertyManager::setValue(QtProperty *property, const QSizeF &value)

    Sets the value of the given \a property to \a value.

    If the specified \a value is not valid according to the given \a
    property's size range, the \a value is adjusted to the nearest
    valid value within the size range.

    \sa value(), setRange(), valueChanged()
*/
void QtSizeFPropertyManager::setValue( QtProperty * property, const QSizeF & val )
{
    setValueInRange< const QSizeF &, QtSizeFPropertyManagerPrivate, QtSizeFPropertyManager, QSizeF >( this, d_ptr.data(),
            &QtSizeFPropertyManager::propertyChanged,
            &QtSizeFPropertyManager::valueChanged,
            property, val, &QtSizeFPropertyManagerPrivate::setValue );
}

/*!
    \fn void QtSizeFPropertyManager::setDecimals(QtProperty *property, int prec)

    Sets the precision of the given \a property to \a prec.

    The valid decimal range is 0-13. The default is 2.

    \sa decimals()
*/
void QtSizeFPropertyManager::setDecimals( QtProperty * property, int prec )
{
    const QtSizeFPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    QtSizeFPropertyManagerPrivate::Data data = it.value();
    
    if( prec > 13 )
        prec = 13;
    else if( prec < 0 )
        prec = 0;
        
    if( data.decimals == prec )
        return;
        
    data.decimals = prec;
    d_ptr->_doublePropertyManager->setDecimals( d_ptr->_propertyToW[property], prec );
    d_ptr->_doublePropertyManager->setDecimals( d_ptr->_propertyToH[property], prec );
    
    it.value() = data;
    
    emit decimalsChanged( property, data.decimals );
}

/*!
    Sets the minimum size value for the given \a property to \a minVal.

    When setting the minimum size value, the maximum and current
    values are adjusted if necessary (ensuring that the size range
    remains valid and that the current value is within the range).

    \sa minimum(), setRange(), rangeChanged()
*/
void QtSizeFPropertyManager::setMinimum( QtProperty * property, const QSizeF & minVal )
{
    setBorderValue< const QSizeF &, QtSizeFPropertyManagerPrivate, QtSizeFPropertyManager, QSizeF, QtSizeFPropertyManagerPrivate::Data >( this, d_ptr.data(),
            &QtSizeFPropertyManager::propertyChanged,
            &QtSizeFPropertyManager::valueChanged,
            &QtSizeFPropertyManager::rangeChanged,
            property,
            &QtSizeFPropertyManagerPrivate::Data::minimumValue,
            &QtSizeFPropertyManagerPrivate::Data::setMinimumValue,
            minVal, &QtSizeFPropertyManagerPrivate::setRange );
}

/*!
    Sets the maximum size value for the given \a property to \a maxVal.

    When setting the maximum size value, the minimum and current
    values are adjusted if necessary (ensuring that the size range
    remains valid and that the current value is within the range).

    \sa maximum(), setRange(), rangeChanged()
*/
void QtSizeFPropertyManager::setMaximum( QtProperty * property, const QSizeF & maxVal )
{
    setBorderValue< const QSizeF &, QtSizeFPropertyManagerPrivate, QtSizeFPropertyManager, QSizeF, QtSizeFPropertyManagerPrivate::Data >( this, d_ptr.data(),
            &QtSizeFPropertyManager::propertyChanged,
            &QtSizeFPropertyManager::valueChanged,
            &QtSizeFPropertyManager::rangeChanged,
            property,
            &QtSizeFPropertyManagerPrivate::Data::maximumValue,
            &QtSizeFPropertyManagerPrivate::Data::setMaximumValue,
            maxVal, &QtSizeFPropertyManagerPrivate::setRange );
}

/*!
    \fn void QtSizeFPropertyManager::setRange(QtProperty *property, const QSizeF &minimum, const QSizeF &maximum)

    Sets the range of valid values.

    This is a convenience function defining the range of valid values
    in one go; setting the \a minimum and \a maximum values for the
    given \a property with a single function call.

    When setting a new range, the current value is adjusted if
    necessary (ensuring that the value remains within the range).

    \sa setMinimum(), setMaximum(), rangeChanged()
*/
void QtSizeFPropertyManager::setRange( QtProperty * property, const QSizeF & minVal, const QSizeF & maxVal )
{
    setBorderValues< const QSizeF &, QtSizeFPropertyManagerPrivate, QtSizeFPropertyManager, QSizeF >( this, d_ptr.data(),
            &QtSizeFPropertyManager::propertyChanged,
            &QtSizeFPropertyManager::valueChanged,
            &QtSizeFPropertyManager::rangeChanged,
            property, minVal, maxVal, &QtSizeFPropertyManagerPrivate::setRange );
}

/*!
    \reimp
*/
void QtSizeFPropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = QtSizeFPropertyManagerPrivate::Data();
    
    QtProperty * wProp = d_ptr->_doublePropertyManager->addProperty();
    wProp->setPropertyName( tr( "Width" ) );
    d_ptr->_doublePropertyManager->setDecimals( wProp, decimals( property ) );
    d_ptr->_doublePropertyManager->setValue( wProp, 0 );
    d_ptr->_doublePropertyManager->setMinimum( wProp, 0 );
    d_ptr->_propertyToW[property] = wProp;
    d_ptr->_wToProperty[wProp] = property;
    property->addSubProperty( wProp );
    
    QtProperty * hProp = d_ptr->_doublePropertyManager->addProperty();
    hProp->setPropertyName( tr( "Height" ) );
    d_ptr->_doublePropertyManager->setDecimals( hProp, decimals( property ) );
    d_ptr->_doublePropertyManager->setValue( hProp, 0 );
    d_ptr->_doublePropertyManager->setMinimum( hProp, 0 );
    d_ptr->_propertyToH[property] = hProp;
    d_ptr->_hToProperty[hProp] = property;
    property->addSubProperty( hProp );
}

/*!
    \reimp
*/
void QtSizeFPropertyManager::uninitializeProperty( QtProperty * property )
{
    QtProperty * wProp = d_ptr->_propertyToW[property];
    if( wProp )
    {
        d_ptr->_wToProperty.remove( wProp );
        delete wProp;
    }
    d_ptr->_propertyToW.remove( property );
    
    QtProperty * hProp = d_ptr->_propertyToH[property];
    if( hProp )
    {
        d_ptr->_hToProperty.remove( hProp );
        delete hProp;
    }
    d_ptr->_propertyToH.remove( property );
    
    d_ptr->_values.remove( property );
}

// QtRectPropertyManager

class QtRectPropertyManagerPrivate
{
    QtRectPropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtRectPropertyManager )
public:

    void slotIntChanged( QtProperty * property, int value );
    void slotPropertyDestroyed( QtProperty * property );
    void setConstraint( QtProperty * property, const QRect & constraint, const QRect & val );
    
    struct Data
    {
        Data() : val( 0, 0, 0, 0 ) {}
        QRect val;
        QRect constraint;
    };
    
    typedef QMap< const QtProperty *, Data > PropertyValueMap;
    PropertyValueMap _values;
    
    QtIntPropertyManager * _intPropertyManager;
    
    QMap< const QtProperty *, QtProperty * > _propertyToX;
    QMap< const QtProperty *, QtProperty * > _propertyToY;
    QMap< const QtProperty *, QtProperty * > _propertyToW;
    QMap< const QtProperty *, QtProperty * > _propertyToH;
    
    QMap< const QtProperty *, QtProperty * > _xToProperty;
    QMap< const QtProperty *, QtProperty * > _yToProperty;
    QMap< const QtProperty *, QtProperty * > _wToProperty;
    QMap< const QtProperty *, QtProperty * > _hToProperty;
};

void QtRectPropertyManagerPrivate::slotIntChanged( QtProperty * property, int value )
{
    if( QtProperty * xProp = _xToProperty.value( property, 0 ) )
    {
        QRect r = _values[xProp].val;
        r.moveLeft( value );
        q_ptr->setValue( xProp, r );
    }
    else if( QtProperty * yProp = _yToProperty.value( property ) )
    {
        QRect r = _values[yProp].val;
        r.moveTop( value );
        q_ptr->setValue( yProp, r );
    }
    else if( QtProperty * wProp = _wToProperty.value( property, 0 ) )
    {
        Data data = _values[wProp];
        QRect r = data.val;
        r.setWidth( value );
        if( !data.constraint.isNull() && data.constraint.x() + data.constraint.width() < r.x() + r.width() )
            r.moveLeft( data.constraint.left() + data.constraint.width() - r.width() );
        q_ptr->setValue( wProp, r );
    }
    else if( QtProperty * hProp = _hToProperty.value( property, 0 ) )
    {
        Data data = _values[hProp];
        QRect r = data.val;
        r.setHeight( value );
        if( !data.constraint.isNull() && data.constraint.y() + data.constraint.height() < r.y() + r.height() )
            r.moveTop( data.constraint.top() + data.constraint.height() - r.height() );
        q_ptr->setValue( hProp, r );
    }
}

void QtRectPropertyManagerPrivate::slotPropertyDestroyed( QtProperty * property )
{
    if( QtProperty * pointXProp = _xToProperty.value( property, 0 ) )
    {
        _propertyToX[pointXProp] = 0;
        _xToProperty.remove( property );
    }
    else if( QtProperty * pointYProp = _yToProperty.value( property, 0 ) )
    {
        _propertyToY[pointYProp] = 0;
        _yToProperty.remove( property );
    }
    else if( QtProperty * pointWProp = _wToProperty.value( property, 0 ) )
    {
        _propertyToW[pointWProp] = 0;
        _wToProperty.remove( property );
    }
    else if( QtProperty * pointHProp = _hToProperty.value( property, 0 ) )
    {
        _propertyToH[pointHProp] = 0;
        _hToProperty.remove( property );
    }
}

void QtRectPropertyManagerPrivate::setConstraint( QtProperty * property,
        const QRect & constraint, const QRect & val )
{
    const bool isNull = constraint.isNull();
    const int left   = isNull ? INT_MIN : constraint.left();
    const int right  = isNull ? INT_MAX : constraint.left() + constraint.width();
    const int top    = isNull ? INT_MIN : constraint.top();
    const int bottom = isNull ? INT_MAX : constraint.top() + constraint.height();
    const int width  = isNull ? INT_MAX : constraint.width();
    const int height = isNull ? INT_MAX : constraint.height();
    
    _intPropertyManager->setRange( _propertyToX[property], left, right );
    _intPropertyManager->setRange( _propertyToY[property], top, bottom );
    _intPropertyManager->setRange( _propertyToW[property], 0, width );
    _intPropertyManager->setRange( _propertyToH[property], 0, height );
    
    _intPropertyManager->setValue( _propertyToX[property], val.x() );
    _intPropertyManager->setValue( _propertyToY[property], val.y() );
    _intPropertyManager->setValue( _propertyToW[property], val.width() );
    _intPropertyManager->setValue( _propertyToH[property], val.height() );
}

/*!
    \class QtRectPropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtRectPropertyManager provides and manages QRect properties.

    A rectangle property has nested \e x, \e y, \e width and \e height
    subproperties. The top-level property's value can be retrieved
    using the value() function, and set using the setValue() slot.

    The subproperties are created by a QtIntPropertyManager object. This
    manager can be retrieved using the subIntPropertyManager() function. In
    order to provide editing widgets for the subproperties in a
    property browser widget, this manager must be associated with an
    editor factory.

    A rectangle property also has a constraint rectangle which can be
    retrieved using the constraint() function, and set using the
    setConstraint() slot.

    In addition, QtRectPropertyManager provides the valueChanged() signal
    which is emitted whenever a property created by this manager
    changes, and the constraintChanged() signal which is emitted
    whenever such a property changes its constraint rectangle.

    \sa QtAbstractPropertyManager, QtIntPropertyManager, QtRectFPropertyManager
*/

/*!
    \fn void QtRectPropertyManager::valueChanged(QtProperty *property, const QRect &value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the new
    \a value as parameters.

    \sa setValue()
*/

/*!
    \fn void QtRectPropertyManager::constraintChanged(QtProperty *property, const QRect &constraint)

    This signal is emitted whenever property changes its constraint
    rectangle, passing a pointer to the \a property and the new \a
    constraint rectangle as parameters.

    \sa setConstraint()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtRectPropertyManager::QtRectPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtRectPropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
    
    d_ptr->_intPropertyManager = new QtIntPropertyManager( this );
    connect( d_ptr->_intPropertyManager, SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotIntChanged( QtProperty *, int ) ) );
    connect( d_ptr->_intPropertyManager, SIGNAL( propertyDestroyed( QtProperty * ) ),
             this, SLOT( slotPropertyDestroyed( QtProperty * ) ) );
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtRectPropertyManager::~QtRectPropertyManager()
{
    clear();
}

/*!
    Returns the manager that creates the nested \e x, \e y, \e width
    and \e height subproperties.

    In order to provide editing widgets for the mentioned
    subproperties in a property browser widget, this manager must be
    associated with an editor factory.

    \sa QtAbstractPropertyBrowser::setFactoryForManager()
*/
QtIntPropertyManager * QtRectPropertyManager::subIntPropertyManager() const
{
    return d_ptr->_intPropertyManager;
}

/*!
    Returns the given \a property's value.

    If the given \a property is not managed by this manager, this
    function returns an invalid rectangle.

    \sa setValue(), constraint()
*/
QRect QtRectPropertyManager::value( const QtProperty * property ) const
{
    return getValue< QRect >( d_ptr->_values, property );
}

/*!
    Returns the given \a property's constraining rectangle. If returned value is null QRect it means there is no constraint applied.

    \sa value(), setConstraint()
*/
QRect QtRectPropertyManager::constraint( const QtProperty * property ) const
{
    return getData< QRect >( d_ptr->_values, &QtRectPropertyManagerPrivate::Data::constraint, property, QRect() );
}

/*!
    \reimp
*/
QString QtRectPropertyManager::valueText( const QtProperty * property ) const
{
    const QtRectPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
    const QRect v = it.value().val;
    return tr( "[(%1, %2), %3 x %4]" ).arg( QString::number( v.x() ) )
           .arg( QString::number( v.y() ) )
           .arg( QString::number( v.width() ) )
           .arg( QString::number( v.height() ) );
}

/*!
    \fn void QtRectPropertyManager::setValue(QtProperty *property, const QRect &value)

    Sets the value of the given \a property to \a value. Nested
    properties are updated automatically.

    If the specified \a value is not inside the given \a property's
    constraining rectangle, the value is adjusted accordingly to fit
    within the constraint.

    \sa value(), setConstraint(), valueChanged()
*/
void QtRectPropertyManager::setValue( QtProperty * property, const QRect & val )
{
    const QtRectPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    QtRectPropertyManagerPrivate::Data data = it.value();
    
    QRect newRect = val.normalized();
    if( !data.constraint.isNull() && !data.constraint.contains( newRect ) )
    {
        const QRect r1 = data.constraint;
        const QRect r2 = newRect;
        newRect.setLeft( qMax( r1.left(), r2.left() ) );
        newRect.setRight( qMin( r1.right(), r2.right() ) );
        newRect.setTop( qMax( r1.top(), r2.top() ) );
        newRect.setBottom( qMin( r1.bottom(), r2.bottom() ) );
        if( newRect.width() < 0 || newRect.height() < 0 )
            return;
    }
    
    if( data.val == newRect )
        return;
        
    data.val = newRect;
    
    it.value() = data;
    d_ptr->_intPropertyManager->setValue( d_ptr->_propertyToX[property], newRect.x() );
    d_ptr->_intPropertyManager->setValue( d_ptr->_propertyToY[property], newRect.y() );
    d_ptr->_intPropertyManager->setValue( d_ptr->_propertyToW[property], newRect.width() );
    d_ptr->_intPropertyManager->setValue( d_ptr->_propertyToH[property], newRect.height() );
    
    emit propertyChanged( property );
    emit valueChanged( property, data.val );
}

/*!
    Sets the given \a property's constraining rectangle to \a
    constraint.

    When setting the constraint, the current value is adjusted if
    necessary (ensuring that the current rectangle value is inside the
    constraint). In order to reset the constraint pass a null QRect value.

    \sa setValue(), constraint(), constraintChanged()
*/
void QtRectPropertyManager::setConstraint( QtProperty * property, const QRect & constraint )
{
    const QtRectPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    QtRectPropertyManagerPrivate::Data data = it.value();
    
    QRect newConstraint = constraint.normalized();
    if( data.constraint == newConstraint )
        return;
        
    const QRect oldVal = data.val;
    
    data.constraint = newConstraint;
    
    if( !data.constraint.isNull() && !data.constraint.contains( oldVal ) )
    {
        QRect r1 = data.constraint;
        QRect r2 = data.val;
        
        if( r2.width() > r1.width() )
            r2.setWidth( r1.width() );
        if( r2.height() > r1.height() )
            r2.setHeight( r1.height() );
        if( r2.left() < r1.left() )
            r2.moveLeft( r1.left() );
        else if( r2.right() > r1.right() )
            r2.moveRight( r1.right() );
        if( r2.top() < r1.top() )
            r2.moveTop( r1.top() );
        else if( r2.bottom() > r1.bottom() )
            r2.moveBottom( r1.bottom() );
            
        data.val = r2;
    }
    
    it.value() = data;
    
    emit constraintChanged( property, data.constraint );
    
    d_ptr->setConstraint( property, data.constraint, data.val );
    
    if( data.val == oldVal )
        return;
        
    emit propertyChanged( property );
    emit valueChanged( property, data.val );
}

/*!
    \reimp
*/
void QtRectPropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = QtRectPropertyManagerPrivate::Data();
    
    QtProperty * xProp = d_ptr->_intPropertyManager->addProperty();
    xProp->setPropertyName( tr( "X" ) );
    d_ptr->_intPropertyManager->setValue( xProp, 0 );
    d_ptr->_propertyToX[property] = xProp;
    d_ptr->_xToProperty[xProp] = property;
    property->addSubProperty( xProp );
    
    QtProperty * yProp = d_ptr->_intPropertyManager->addProperty();
    yProp->setPropertyName( tr( "Y" ) );
    d_ptr->_intPropertyManager->setValue( yProp, 0 );
    d_ptr->_propertyToY[property] = yProp;
    d_ptr->_yToProperty[yProp] = property;
    property->addSubProperty( yProp );
    
    QtProperty * wProp = d_ptr->_intPropertyManager->addProperty();
    wProp->setPropertyName( tr( "Width" ) );
    d_ptr->_intPropertyManager->setValue( wProp, 0 );
    d_ptr->_intPropertyManager->setMinimum( wProp, 0 );
    d_ptr->_propertyToW[property] = wProp;
    d_ptr->_wToProperty[wProp] = property;
    property->addSubProperty( wProp );
    
    QtProperty * hProp = d_ptr->_intPropertyManager->addProperty();
    hProp->setPropertyName( tr( "Height" ) );
    d_ptr->_intPropertyManager->setValue( hProp, 0 );
    d_ptr->_intPropertyManager->setMinimum( hProp, 0 );
    d_ptr->_propertyToH[property] = hProp;
    d_ptr->_hToProperty[hProp] = property;
    property->addSubProperty( hProp );
}

/*!
    \reimp
*/
void QtRectPropertyManager::uninitializeProperty( QtProperty * property )
{
    QtProperty * xProp = d_ptr->_propertyToX[property];
    if( xProp )
    {
        d_ptr->_xToProperty.remove( xProp );
        delete xProp;
    }
    d_ptr->_propertyToX.remove( property );
    
    QtProperty * yProp = d_ptr->_propertyToY[property];
    if( yProp )
    {
        d_ptr->_yToProperty.remove( yProp );
        delete yProp;
    }
    d_ptr->_propertyToY.remove( property );
    
    QtProperty * wProp = d_ptr->_propertyToW[property];
    if( wProp )
    {
        d_ptr->_wToProperty.remove( wProp );
        delete wProp;
    }
    d_ptr->_propertyToW.remove( property );
    
    QtProperty * hProp = d_ptr->_propertyToH[property];
    if( hProp )
    {
        d_ptr->_hToProperty.remove( hProp );
        delete hProp;
    }
    d_ptr->_propertyToH.remove( property );
    
    d_ptr->_values.remove( property );
}

// QtRectFPropertyManager

class QtRectFPropertyManagerPrivate
{
    QtRectFPropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtRectFPropertyManager )
public:

    void slotDoubleChanged( QtProperty * property, double value );
    void slotPropertyDestroyed( QtProperty * property );
    void setConstraint( QtProperty * property, const QRectF & constraint, const QRectF & val );
    
    struct Data
    {
        Data() : val( 0, 0, 0, 0 ), decimals( 2 ) {}
        QRectF val;
        QRectF constraint;
        int decimals;
    };
    
    typedef QMap< const QtProperty *, Data > PropertyValueMap;
    PropertyValueMap _values;
    
    QtDoublePropertyManager * _doublePropertyManager;
    
    QMap< const QtProperty *, QtProperty * > _propertyToX;
    QMap< const QtProperty *, QtProperty * > _propertyToY;
    QMap< const QtProperty *, QtProperty * > _propertyToW;
    QMap< const QtProperty *, QtProperty * > _propertyToH;
    
    QMap< const QtProperty *, QtProperty * > _xToProperty;
    QMap< const QtProperty *, QtProperty * > _yToProperty;
    QMap< const QtProperty *, QtProperty * > _wToProperty;
    QMap< const QtProperty *, QtProperty * > _hToProperty;
};

void QtRectFPropertyManagerPrivate::slotDoubleChanged( QtProperty * property, double value )
{
    if( QtProperty * xProp = _xToProperty.value( property, 0 ) )
    {
        QRectF r = _values[xProp].val;
        r.moveLeft( value );
        q_ptr->setValue( xProp, r );
    }
    else if( QtProperty * yProp = _yToProperty.value( property, 0 ) )
    {
        QRectF r = _values[yProp].val;
        r.moveTop( value );
        q_ptr->setValue( yProp, r );
    }
    else if( QtProperty * wProp = _wToProperty.value( property, 0 ) )
    {
        Data data = _values[wProp];
        QRectF r = data.val;
        r.setWidth( value );
        if( !data.constraint.isNull() && data.constraint.x() + data.constraint.width() < r.x() + r.width() )
            r.moveLeft( data.constraint.left() + data.constraint.width() - r.width() );
        q_ptr->setValue( wProp, r );
    }
    else if( QtProperty * hProp = _hToProperty.value( property, 0 ) )
    {
        Data data = _values[hProp];
        QRectF r = data.val;
        r.setHeight( value );
        if( !data.constraint.isNull() && data.constraint.y() + data.constraint.height() < r.y() + r.height() )
            r.moveTop( data.constraint.top() + data.constraint.height() - r.height() );
        q_ptr->setValue( hProp, r );
    }
}

void QtRectFPropertyManagerPrivate::slotPropertyDestroyed( QtProperty * property )
{
    if( QtProperty * pointXProp = _xToProperty.value( property, 0 ) )
    {
        _propertyToX[pointXProp] = 0;
        _xToProperty.remove( property );
    }
    else if( QtProperty * pointYProp = _yToProperty.value( property, 0 ) )
    {
        _propertyToY[pointYProp] = 0;
        _yToProperty.remove( property );
    }
    else if( QtProperty * pointWProp = _wToProperty.value( property, 0 ) )
    {
        _propertyToW[pointWProp] = 0;
        _wToProperty.remove( property );
    }
    else if( QtProperty * pointHProp = _hToProperty.value( property, 0 ) )
    {
        _propertyToH[pointHProp] = 0;
        _hToProperty.remove( property );
    }
}

void QtRectFPropertyManagerPrivate::setConstraint( QtProperty * property,
        const QRectF & constraint, const QRectF & val )
{
    const bool isNull = constraint.isNull();
    const float left   = isNull ? FLT_MIN : constraint.left();
    const float right  = isNull ? FLT_MAX : constraint.left() + constraint.width();
    const float top    = isNull ? FLT_MIN : constraint.top();
    const float bottom = isNull ? FLT_MAX : constraint.top() + constraint.height();
    const float width  = isNull ? FLT_MAX : constraint.width();
    const float height = isNull ? FLT_MAX : constraint.height();
    
    _doublePropertyManager->setRange( _propertyToX[property], left, right );
    _doublePropertyManager->setRange( _propertyToY[property], top, bottom );
    _doublePropertyManager->setRange( _propertyToW[property], 0, width );
    _doublePropertyManager->setRange( _propertyToH[property], 0, height );
    
    _doublePropertyManager->setValue( _propertyToX[property], val.x() );
    _doublePropertyManager->setValue( _propertyToY[property], val.y() );
    _doublePropertyManager->setValue( _propertyToW[property], val.width() );
    _doublePropertyManager->setValue( _propertyToH[property], val.height() );
}

/*!
    \class QtRectFPropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtRectFPropertyManager provides and manages QRectF properties.

    A rectangle property has nested \e x, \e y, \e width and \e height
    subproperties. The top-level property's value can be retrieved
    using the value() function, and set using the setValue() slot.

    The subproperties are created by a QtDoublePropertyManager object. This
    manager can be retrieved using the subDoublePropertyManager() function. In
    order to provide editing widgets for the subproperties in a
    property browser widget, this manager must be associated with an
    editor factory.

    A rectangle property also has a constraint rectangle which can be
    retrieved using the constraint() function, and set using the
    setConstraint() slot.

    In addition, QtRectFPropertyManager provides the valueChanged() signal
    which is emitted whenever a property created by this manager
    changes, and the constraintChanged() signal which is emitted
    whenever such a property changes its constraint rectangle.

    \sa QtAbstractPropertyManager, QtDoublePropertyManager, QtRectPropertyManager
*/

/*!
    \fn void QtRectFPropertyManager::valueChanged(QtProperty *property, const QRectF &value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the new
    \a value as parameters.

    \sa setValue()
*/

/*!
    \fn void QtRectFPropertyManager::constraintChanged(QtProperty *property, const QRectF &constraint)

    This signal is emitted whenever property changes its constraint
    rectangle, passing a pointer to the \a property and the new \a
    constraint rectangle as parameters.

    \sa setConstraint()
*/

/*!
    \fn void QtRectFPropertyManager::decimalsChanged(QtProperty *property, int prec)

    This signal is emitted whenever a property created by this manager
    changes its precision of value, passing a pointer to the
    \a property and the new \a prec value

    \sa setDecimals()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtRectFPropertyManager::QtRectFPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtRectFPropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
    
    d_ptr->_doublePropertyManager = new QtDoublePropertyManager( this );
    connect( d_ptr->_doublePropertyManager, SIGNAL( valueChanged( QtProperty *, double ) ),
             this, SLOT( slotDoubleChanged( QtProperty *, double ) ) );
    connect( d_ptr->_doublePropertyManager, SIGNAL( propertyDestroyed( QtProperty * ) ),
             this, SLOT( slotPropertyDestroyed( QtProperty * ) ) );
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtRectFPropertyManager::~QtRectFPropertyManager()
{
    clear();
}

/*!
    Returns the manager that creates the nested \e x, \e y, \e width
    and \e height subproperties.

    In order to provide editing widgets for the mentioned
    subproperties in a property browser widget, this manager must be
    associated with an editor factory.

    \sa QtAbstractPropertyBrowser::setFactoryForManager()
*/
QtDoublePropertyManager * QtRectFPropertyManager::subDoublePropertyManager() const
{
    return d_ptr->_doublePropertyManager;
}

/*!
    Returns the given \a property's value.

    If the given \a property is not managed by this manager, this
    function returns an invalid rectangle.

    \sa setValue(), constraint()
*/
QRectF QtRectFPropertyManager::value( const QtProperty * property ) const
{
    return getValue< QRectF >( d_ptr->_values, property );
}

/*!
    Returns the given \a property's precision, in decimals.

    \sa setDecimals()
*/
int QtRectFPropertyManager::decimals( const QtProperty * property ) const
{
    return getData< int >( d_ptr->_values, &QtRectFPropertyManagerPrivate::Data::decimals, property, 0 );
}

/*!
    Returns the given \a property's constraining rectangle. If returned value is null QRectF it means there is no constraint applied.

    \sa value(), setConstraint()
*/
QRectF QtRectFPropertyManager::constraint( const QtProperty * property ) const
{
    return getData< QRectF >( d_ptr->_values, &QtRectFPropertyManagerPrivate::Data::constraint, property, QRect() );
}

/*!
    \reimp
*/
QString QtRectFPropertyManager::valueText( const QtProperty * property ) const
{
    const QtRectFPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
    const QRectF v = it.value().val;
    const int dec = it.value().decimals;
    return QString( tr( "[(%1, %2), %3 x %4]" ).arg( QString::number( v.x(), 'f', dec ) )
                    .arg( QString::number( v.y(), 'f', dec ) )
                    .arg( QString::number( v.width(), 'f', dec ) )
                    .arg( QString::number( v.height(), 'f', dec ) ) );
}

/*!
    \fn void QtRectFPropertyManager::setValue(QtProperty *property, const QRectF &value)

    Sets the value of the given \a property to \a value. Nested
    properties are updated automatically.

    If the specified \a value is not inside the given \a property's
    constraining rectangle, the value is adjusted accordingly to fit
    within the constraint.

    \sa value(), setConstraint(), valueChanged()
*/
void QtRectFPropertyManager::setValue( QtProperty * property, const QRectF & val )
{
    const QtRectFPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    QtRectFPropertyManagerPrivate::Data data = it.value();
    
    QRectF newRect = val.normalized();
    if( !data.constraint.isNull() && !data.constraint.contains( newRect ) )
    {
        const QRectF r1 = data.constraint;
        const QRectF r2 = newRect;
        newRect.setLeft( qMax( r1.left(), r2.left() ) );
        newRect.setRight( qMin( r1.right(), r2.right() ) );
        newRect.setTop( qMax( r1.top(), r2.top() ) );
        newRect.setBottom( qMin( r1.bottom(), r2.bottom() ) );
        if( newRect.width() < 0 || newRect.height() < 0 )
            return;
    }
    
    if( data.val == newRect )
        return;
        
    data.val = newRect;
    
    it.value() = data;
    d_ptr->_doublePropertyManager->setValue( d_ptr->_propertyToX[property], newRect.x() );
    d_ptr->_doublePropertyManager->setValue( d_ptr->_propertyToY[property], newRect.y() );
    d_ptr->_doublePropertyManager->setValue( d_ptr->_propertyToW[property], newRect.width() );
    d_ptr->_doublePropertyManager->setValue( d_ptr->_propertyToH[property], newRect.height() );
    
    emit propertyChanged( property );
    emit valueChanged( property, data.val );
}

/*!
    Sets the given \a property's constraining rectangle to \a
    constraint.

    When setting the constraint, the current value is adjusted if
    necessary (ensuring that the current rectangle value is inside the
    constraint). In order to reset the constraint pass a null QRectF value.

    \sa setValue(), constraint(), constraintChanged()
*/
void QtRectFPropertyManager::setConstraint( QtProperty * property, const QRectF & constraint )
{
    const QtRectFPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    QtRectFPropertyManagerPrivate::Data data = it.value();
    
    QRectF newConstraint = constraint.normalized();
    if( data.constraint == newConstraint )
        return;
        
    const QRectF oldVal = data.val;
    
    data.constraint = newConstraint;
    
    if( !data.constraint.isNull() && !data.constraint.contains( oldVal ) )
    {
        QRectF r1 = data.constraint;
        QRectF r2 = data.val;
        
        if( r2.width() > r1.width() )
            r2.setWidth( r1.width() );
        if( r2.height() > r1.height() )
            r2.setHeight( r1.height() );
        if( r2.left() < r1.left() )
            r2.moveLeft( r1.left() );
        else if( r2.right() > r1.right() )
            r2.moveRight( r1.right() );
        if( r2.top() < r1.top() )
            r2.moveTop( r1.top() );
        else if( r2.bottom() > r1.bottom() )
            r2.moveBottom( r1.bottom() );
            
        data.val = r2;
    }
    
    it.value() = data;
    
    emit constraintChanged( property, data.constraint );
    
    d_ptr->setConstraint( property, data.constraint, data.val );
    
    if( data.val == oldVal )
        return;
        
    emit propertyChanged( property );
    emit valueChanged( property, data.val );
}

/*!
    \fn void QtRectFPropertyManager::setDecimals(QtProperty *property, int prec)

    Sets the precision of the given \a property to \a prec.

    The valid decimal range is 0-13. The default is 2.

    \sa decimals()
*/
void QtRectFPropertyManager::setDecimals( QtProperty * property, int prec )
{
    const QtRectFPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    QtRectFPropertyManagerPrivate::Data data = it.value();
    
    if( prec > 13 )
        prec = 13;
    else if( prec < 0 )
        prec = 0;
        
    if( data.decimals == prec )
        return;
        
    data.decimals = prec;
    d_ptr->_doublePropertyManager->setDecimals( d_ptr->_propertyToX[property], prec );
    d_ptr->_doublePropertyManager->setDecimals( d_ptr->_propertyToY[property], prec );
    d_ptr->_doublePropertyManager->setDecimals( d_ptr->_propertyToW[property], prec );
    d_ptr->_doublePropertyManager->setDecimals( d_ptr->_propertyToH[property], prec );
    
    it.value() = data;
    
    emit decimalsChanged( property, data.decimals );
}

/*!
    \reimp
*/
void QtRectFPropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = QtRectFPropertyManagerPrivate::Data();
    
    QtProperty * xProp = d_ptr->_doublePropertyManager->addProperty();
    xProp->setPropertyName( tr( "X" ) );
    d_ptr->_doublePropertyManager->setDecimals( xProp, decimals( property ) );
    d_ptr->_doublePropertyManager->setValue( xProp, 0 );
    d_ptr->_propertyToX[property] = xProp;
    d_ptr->_xToProperty[xProp] = property;
    property->addSubProperty( xProp );
    
    QtProperty * yProp = d_ptr->_doublePropertyManager->addProperty();
    yProp->setPropertyName( tr( "Y" ) );
    d_ptr->_doublePropertyManager->setDecimals( yProp, decimals( property ) );
    d_ptr->_doublePropertyManager->setValue( yProp, 0 );
    d_ptr->_propertyToY[property] = yProp;
    d_ptr->_yToProperty[yProp] = property;
    property->addSubProperty( yProp );
    
    QtProperty * wProp = d_ptr->_doublePropertyManager->addProperty();
    wProp->setPropertyName( tr( "Width" ) );
    d_ptr->_doublePropertyManager->setDecimals( wProp, decimals( property ) );
    d_ptr->_doublePropertyManager->setValue( wProp, 0 );
    d_ptr->_doublePropertyManager->setMinimum( wProp, 0 );
    d_ptr->_propertyToW[property] = wProp;
    d_ptr->_wToProperty[wProp] = property;
    property->addSubProperty( wProp );
    
    QtProperty * hProp = d_ptr->_doublePropertyManager->addProperty();
    hProp->setPropertyName( tr( "Height" ) );
    d_ptr->_doublePropertyManager->setDecimals( hProp, decimals( property ) );
    d_ptr->_doublePropertyManager->setValue( hProp, 0 );
    d_ptr->_doublePropertyManager->setMinimum( hProp, 0 );
    d_ptr->_propertyToH[property] = hProp;
    d_ptr->_hToProperty[hProp] = property;
    property->addSubProperty( hProp );
}

/*!
    \reimp
*/
void QtRectFPropertyManager::uninitializeProperty( QtProperty * property )
{
    QtProperty * xProp = d_ptr->_propertyToX[property];
    if( xProp )
    {
        d_ptr->_xToProperty.remove( xProp );
        delete xProp;
    }
    d_ptr->_propertyToX.remove( property );
    
    QtProperty * yProp = d_ptr->_propertyToY[property];
    if( yProp )
    {
        d_ptr->_yToProperty.remove( yProp );
        delete yProp;
    }
    d_ptr->_propertyToY.remove( property );
    
    QtProperty * wProp = d_ptr->_propertyToW[property];
    if( wProp )
    {
        d_ptr->_wToProperty.remove( wProp );
        delete wProp;
    }
    d_ptr->_propertyToW.remove( property );
    
    QtProperty * hProp = d_ptr->_propertyToH[property];
    if( hProp )
    {
        d_ptr->_hToProperty.remove( hProp );
        delete hProp;
    }
    d_ptr->_propertyToH.remove( property );
    
    d_ptr->_values.remove( property );
}

// QtEnumPropertyManager

class QtEnumPropertyManagerPrivate
{
    QtEnumPropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtEnumPropertyManager )
public:

    struct Data
    {
        Data() : val( -1 ) {}
        int val;
        QStringList enumNames;
        QMap< int, QIcon > enumIcons;
    };
    
    typedef QMap< const QtProperty *, Data > PropertyValueMap;
    PropertyValueMap _values;
};

/*!
    \class QtEnumPropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtEnumPropertyManager provides and manages enum properties.

    Each enum property has an associated list of enum names which can
    be retrieved using the enumNames() function, and set using the
    corresponding setEnumNames() function. An enum property's value is
    represented by an index in this list, and can be retrieved and set
    using the value() and setValue() slots respectively.

    Each enum value can also have an associated icon. The mapping from
    values to icons can be set using the setEnumIcons() function and
    queried with the enumIcons() function.

    In addition, QtEnumPropertyManager provides the valueChanged() signal
    which is emitted whenever a property created by this manager
    changes. The enumNamesChanged() or enumIconsChanged() signal is emitted
    whenever the list of enum names or icons is altered.

    \sa QtAbstractPropertyManager, QtEnumEditorFactory
*/

/*!
    \fn void QtEnumPropertyManager::valueChanged(QtProperty *property, int value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the new
    \a value as parameters.

    \sa setValue()
*/

/*!
    \fn void QtEnumPropertyManager::enumNamesChanged(QtProperty *property, const QStringList &names)

    This signal is emitted whenever a property created by this manager
    changes its enum names, passing a pointer to the \a property and
    the new \a names as parameters.

    \sa setEnumNames()
*/

/*!
    \fn void QtEnumPropertyManager::enumIconsChanged(QtProperty *property, const QMap<int, QIcon> &icons)

    This signal is emitted whenever a property created by this manager
    changes its enum icons, passing a pointer to the \a property and
    the new mapping of values to \a icons as parameters.

    \sa setEnumIcons()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtEnumPropertyManager::QtEnumPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtEnumPropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtEnumPropertyManager::~QtEnumPropertyManager()
{
    clear();
}

/*!
    Returns the given \a property's value which is an index in the
    list returned by enumNames()

    If the given property is not managed by this manager, this
    function returns -1.

    \sa enumNames(), setValue()
*/
int QtEnumPropertyManager::value( const QtProperty * property ) const
{
    return getValue< int >( d_ptr->_values, property, -1 );
}

/*!
    Returns the given \a property's list of enum names.

    \sa value(), setEnumNames()
*/
QStringList QtEnumPropertyManager::enumNames( const QtProperty * property ) const
{
    return getData< QStringList >( d_ptr->_values, &QtEnumPropertyManagerPrivate::Data::enumNames, property, QStringList() );
}

/*!
    Returns the given \a property's map of enum values to their icons.

    \sa value(), setEnumIcons()
*/
QMap< int, QIcon > QtEnumPropertyManager::enumIcons( const QtProperty * property ) const
{
    return getData< QMap< int, QIcon > >( d_ptr->_values, &QtEnumPropertyManagerPrivate::Data::enumIcons, property, QMap< int, QIcon >() );
}

/*!
    \reimp
*/
QString QtEnumPropertyManager::valueText( const QtProperty * property ) const
{
    const QtEnumPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
        
    const QtEnumPropertyManagerPrivate::Data & data = it.value();
    
    const int v = data.val;
    if( v >= 0 && v < data.enumNames.count() )
        return data.enumNames.at( v );
    return QString();
}

/*!
    \reimp
*/
QIcon QtEnumPropertyManager::valueIcon( const QtProperty * property ) const
{
    const QtEnumPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QIcon();
        
    const QtEnumPropertyManagerPrivate::Data & data = it.value();
    
    const int v = data.val;
    return data.enumIcons.value( v );
}

/*!
    \fn void QtEnumPropertyManager::setValue(QtProperty *property, int value)

    Sets the value of the given  \a property to \a value.

    The specified \a value must be less than the size of the given \a
    property's enumNames() list, and larger than (or equal to) 0.

    \sa value(), valueChanged()
*/
void QtEnumPropertyManager::setValue( QtProperty * property, int val )
{
    const QtEnumPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    QtEnumPropertyManagerPrivate::Data data = it.value();
    
    if( val >= data.enumNames.count() )
        return;
        
    if( val < 0 && data.enumNames.count() > 0 )
        return;
        
    if( val < 0 )
        val = -1;
        
    if( data.val == val )
        return;
        
    data.val = val;
    
    it.value() = data;
    
    emit propertyChanged( property );
    emit valueChanged( property, data.val );
}

/*!
    Sets the given \a property's list of enum names to \a
    enumNames. The \a property's current value is reset to 0
    indicating the first item of the list.

    If the specified \a enumNames list is empty, the \a property's
    current value is set to -1.

    \sa enumNames(), enumNamesChanged()
*/
void QtEnumPropertyManager::setEnumNames( QtProperty * property, const QStringList & enumNames )
{
    const QtEnumPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    QtEnumPropertyManagerPrivate::Data data = it.value();
    
    if( data.enumNames == enumNames )
        return;
        
    data.enumNames = enumNames;
    
    data.val = -1;
    
    if( enumNames.count() > 0 )
        data.val = 0;
        
    it.value() = data;
    
    emit enumNamesChanged( property, data.enumNames );
    
    emit propertyChanged( property );
    emit valueChanged( property, data.val );
}

/*!
    Sets the given \a property's map of enum values to their icons to \a
    enumIcons.

    Each enum value can have associated icon. This association is represented with passed \a enumIcons map.

    \sa enumNames(), enumNamesChanged()
*/
void QtEnumPropertyManager::setEnumIcons( QtProperty * property, const QMap< int, QIcon > & enumIcons )
{
    const QtEnumPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    it.value().enumIcons = enumIcons;
    
    emit enumIconsChanged( property, it.value().enumIcons );
    
    emit propertyChanged( property );
}

/*!
    \reimp
*/
void QtEnumPropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = QtEnumPropertyManagerPrivate::Data();
}

/*!
    \reimp
*/
void QtEnumPropertyManager::uninitializeProperty( QtProperty * property )
{
    d_ptr->_values.remove( property );
}

// QtFlagPropertyManager

class QtFlagPropertyManagerPrivate
{
    QtFlagPropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtFlagPropertyManager )
public:

    void slotBoolChanged( QtProperty * property, bool value );
    void slotPropertyDestroyed( QtProperty * property );
    
    struct Data
    {
        Data() : val( -1 ) {}
        int val;
        QStringList flagNames;
    };
    
    typedef QMap< const QtProperty *, Data > PropertyValueMap;
    PropertyValueMap _values;
    
    QtBoolPropertyManager * _boolPropertyManager;
    
    QMap< const QtProperty *, QList< QtProperty * > > _propertyToFlags;
    
    QMap< const QtProperty *, QtProperty * > _flagToProperty;
};

void QtFlagPropertyManagerPrivate::slotBoolChanged( QtProperty * property, bool value )
{
    QtProperty * prop = _flagToProperty.value( property, 0 );
    if( prop == 0 )
        return;
        
    QListIterator< QtProperty * > itProp( _propertyToFlags[prop] );
    int level = 0;
    while( itProp.hasNext() )
    {
        QtProperty * p = itProp.next();
        if( p == property )
        {
            int v = _values[prop].val;
            if( value )
                v |= ( 1 << level );
            else
                v &= ~( 1 << level );
            q_ptr->setValue( prop, v );
            return;
        }
        level++;
    }
}

void QtFlagPropertyManagerPrivate::slotPropertyDestroyed( QtProperty * property )
{
    QtProperty * flagProperty = _flagToProperty.value( property, 0 );
    if( flagProperty == 0 )
        return;
        
    _propertyToFlags[flagProperty].replace( _propertyToFlags[flagProperty].indexOf( property ), 0 );
    _flagToProperty.remove( property );
}

/*!
    \class QtFlagPropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtFlagPropertyManager provides and manages flag properties.

    Each flag property has an associated list of flag names which can
    be retrieved using the flagNames() function, and set using the
    corresponding setFlagNames() function.

    The flag manager provides properties with nested boolean
    subproperties representing each flag, i.e. a flag property's value
    is the binary combination of the subproperties' values. A
    property's value can be retrieved and set using the value() and
    setValue() slots respectively. The combination of flags is represented
    by single int value - that's why it's possible to store up to
    32 independent flags in one flag property.

    The subproperties are created by a QtBoolPropertyManager object. This
    manager can be retrieved using the subBoolPropertyManager() function. In
    order to provide editing widgets for the subproperties in a
    property browser widget, this manager must be associated with an
    editor factory.

    In addition, QtFlagPropertyManager provides the valueChanged() signal
    which is emitted whenever a property created by this manager
    changes, and the flagNamesChanged() signal which is emitted
    whenever the list of flag names is altered.

    \sa QtAbstractPropertyManager, QtBoolPropertyManager
*/

/*!
    \fn void QtFlagPropertyManager::valueChanged(QtProperty *property, int value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a  property and the new
    \a value as parameters.

    \sa setValue()
*/

/*!
    \fn void QtFlagPropertyManager::flagNamesChanged(QtProperty *property, const QStringList &names)

    This signal is emitted whenever a property created by this manager
    changes its flag names, passing a pointer to the \a property and the
    new \a names as parameters.

    \sa setFlagNames()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtFlagPropertyManager::QtFlagPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtFlagPropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
    
    d_ptr->_boolPropertyManager = new QtBoolPropertyManager( this );
    connect( d_ptr->_boolPropertyManager, SIGNAL( valueChanged( QtProperty *, bool ) ),
             this, SLOT( slotBoolChanged( QtProperty *, bool ) ) );
    connect( d_ptr->_boolPropertyManager, SIGNAL( propertyDestroyed( QtProperty * ) ),
             this, SLOT( slotPropertyDestroyed( QtProperty * ) ) );
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtFlagPropertyManager::~QtFlagPropertyManager()
{
    clear();
}

/*!
    Returns the manager that produces the nested boolean subproperties
    representing each flag.

    In order to provide editing widgets for the subproperties in a
    property browser widget, this manager must be associated with an
    editor factory.

    \sa QtAbstractPropertyBrowser::setFactoryForManager()
*/
QtBoolPropertyManager * QtFlagPropertyManager::subBoolPropertyManager() const
{
    return d_ptr->_boolPropertyManager;
}

/*!
    Returns the given \a property's value.

    If the given property is not managed by this manager, this
    function returns 0.

    \sa flagNames(), setValue()
*/
int QtFlagPropertyManager::value( const QtProperty * property ) const
{
    return getValue< int >( d_ptr->_values, property, 0 );
}

/*!
    Returns the given \a property's list of flag names.

    \sa value(), setFlagNames()
*/
QStringList QtFlagPropertyManager::flagNames( const QtProperty * property ) const
{
    return getData< QStringList >( d_ptr->_values, &QtFlagPropertyManagerPrivate::Data::flagNames, property, QStringList() );
}

/*!
    \reimp
*/
QString QtFlagPropertyManager::valueText( const QtProperty * property ) const
{
    const QtFlagPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
        
    const QtFlagPropertyManagerPrivate::Data & data = it.value();
    
    QString str;
    int level = 0;
    const QChar bar = QLatin1Char( '|' );
    const QStringList::const_iterator fncend = data.flagNames.constEnd();
    for( QStringList::const_iterator itFlag =  data.flagNames.constBegin(); itFlag != fncend; ++itFlag )
    {
        if( data.val & ( 1 << level ) )
        {
            if( !str.isEmpty() )
                str += bar;
            str += *itFlag;
        }
        
        level++;
    }
    return str;
}

/*!
    \fn void QtFlagPropertyManager::setValue(QtProperty *property, int value)

    Sets the value of the given \a property to \a value. Nested
    properties are updated automatically.

    The specified \a value must be less than the binary combination of
    the property's flagNames() list size (i.e. less than 2\sup n,
    where \c n is the size of the list) and larger than (or equal to)
    0.

    \sa value(), valueChanged()
*/
void QtFlagPropertyManager::setValue( QtProperty * property, int val )
{
    const QtFlagPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    QtFlagPropertyManagerPrivate::Data data = it.value();
    
    if( data.val == val )
        return;
        
    if( val > ( 1 << data.flagNames.count() ) - 1 )
        return;
        
    if( val < 0 )
        return;
        
    data.val = val;
    
    it.value() = data;
    
    QListIterator< QtProperty * > itProp( d_ptr->_propertyToFlags[property] );
    int level = 0;
    while( itProp.hasNext() )
    {
        QtProperty * prop = itProp.next();
        if( prop )
            d_ptr->_boolPropertyManager->setValue( prop, val & ( 1 << level ) );
        level++;
    }
    
    emit propertyChanged( property );
    emit valueChanged( property, data.val );
}

/*!
    Sets the given \a property's list of flag names to \a flagNames. The
    property's current value is reset to 0 indicating the first item
    of the list.

    \sa flagNames(), flagNamesChanged()
*/
void QtFlagPropertyManager::setFlagNames( QtProperty * property, const QStringList & flagNames )
{
    const QtFlagPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    QtFlagPropertyManagerPrivate::Data data = it.value();
    
    if( data.flagNames == flagNames )
        return;
        
    data.flagNames = flagNames;
    data.val = 0;
    
    it.value() = data;
    
    QListIterator< QtProperty * > itProp( d_ptr->_propertyToFlags[property] );
    while( itProp.hasNext() )
    {
        QtProperty * prop = itProp.next();
        if( prop )
        {
            delete prop;
            d_ptr->_flagToProperty.remove( prop );
        }
    }
    d_ptr->_propertyToFlags[property].clear();
    
    QStringListIterator itFlag( flagNames );
    while( itFlag.hasNext() )
    {
        const QString flagName = itFlag.next();
        QtProperty * prop = d_ptr->_boolPropertyManager->addProperty();
        prop->setPropertyName( flagName );
        property->addSubProperty( prop );
        d_ptr->_propertyToFlags[property].append( prop );
        d_ptr->_flagToProperty[prop] = property;
    }
    
    emit flagNamesChanged( property, data.flagNames );
    
    emit propertyChanged( property );
    emit valueChanged( property, data.val );
}

/*!
    \reimp
*/
void QtFlagPropertyManager::initializeProperty( QtProperty * property )
{
    d_ptr->_values[property] = QtFlagPropertyManagerPrivate::Data();
    
    d_ptr->_propertyToFlags[property] = QList< QtProperty * >();
}

/*!
    \reimp
*/
void QtFlagPropertyManager::uninitializeProperty( QtProperty * property )
{
    QListIterator< QtProperty * > itProp( d_ptr->_propertyToFlags[property] );
    while( itProp.hasNext() )
    {
        QtProperty * prop = itProp.next();
        if( prop )
        {
            delete prop;
            d_ptr->_flagToProperty.remove( prop );
        }
    }
    d_ptr->_propertyToFlags.remove( property );
    
    d_ptr->_values.remove( property );
}

// QtSizePolicyPropertyManager

class QtSizePolicyPropertyManagerPrivate
{
    QtSizePolicyPropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtSizePolicyPropertyManager )
public:

    QtSizePolicyPropertyManagerPrivate();
    
    void slotIntChanged( QtProperty * property, int value );
    void slotEnumChanged( QtProperty * property, int value );
    void slotPropertyDestroyed( QtProperty * property );
    
    typedef QMap< const QtProperty *, QSizePolicy > PropertyValueMap;
    PropertyValueMap _values;
    
    QtIntPropertyManager * _intPropertyManager;
    QtEnumPropertyManager * _enumPropertyManager;
    
    QMap< const QtProperty *, QtProperty * > _propertyToHPolicy;
    QMap< const QtProperty *, QtProperty * > _propertyToVPolicy;
    QMap< const QtProperty *, QtProperty * > _propertyToHStretch;
    QMap< const QtProperty *, QtProperty * > _propertyToVStretch;
    
    QMap< const QtProperty *, QtProperty * > _hPolicyToProperty;
    QMap< const QtProperty *, QtProperty * > _vPolicyToProperty;
    QMap< const QtProperty *, QtProperty * > _hStretchToProperty;
    QMap< const QtProperty *, QtProperty * > _vStretchToProperty;
};

QtSizePolicyPropertyManagerPrivate::QtSizePolicyPropertyManagerPrivate()
{
}

void QtSizePolicyPropertyManagerPrivate::slotIntChanged( QtProperty * property, int value )
{
    if( QtProperty * hProp = _hStretchToProperty.value( property, 0 ) )
    {
        QSizePolicy sp = _values[hProp];
        sp.setHorizontalStretch( value );
        q_ptr->setValue( hProp, sp );
    }
    else if( QtProperty * vProp = _vStretchToProperty.value( property, 0 ) )
    {
        QSizePolicy sp = _values[vProp];
        sp.setVerticalStretch( value );
        q_ptr->setValue( vProp, sp );
    }
}

void QtSizePolicyPropertyManagerPrivate::slotEnumChanged( QtProperty * property, int value )
{
    if( QtProperty * hProp = _hPolicyToProperty.value( property, 0 ) )
    {
        QSizePolicy sp = _values[hProp];
        sp.setHorizontalPolicy( metaEnumProvider()->indexToSizePolicy( value ) );
        q_ptr->setValue( hProp, sp );
    }
    else if( QtProperty * vProp = _vPolicyToProperty.value( property, 0 ) )
    {
        QSizePolicy sp = _values[vProp];
        sp.setVerticalPolicy( metaEnumProvider()->indexToSizePolicy( value ) );
        q_ptr->setValue( vProp, sp );
    }
}

void QtSizePolicyPropertyManagerPrivate::slotPropertyDestroyed( QtProperty * property )
{
    if( QtProperty * pointHProp = _hStretchToProperty.value( property, 0 ) )
    {
        _propertyToHStretch[pointHProp] = 0;
        _hStretchToProperty.remove( property );
    }
    else if( QtProperty * pointVProp = _vStretchToProperty.value( property, 0 ) )
    {
        _propertyToVStretch[pointVProp] = 0;
        _vStretchToProperty.remove( property );
    }
    else if( QtProperty * pointHPolicyProp = _hPolicyToProperty.value( property, 0 ) )
    {
        _propertyToHPolicy[pointHPolicyProp] = 0;
        _hPolicyToProperty.remove( property );
    }
    else if( QtProperty * pointVPolicyProp = _vPolicyToProperty.value( property, 0 ) )
    {
        _propertyToVPolicy[pointVPolicyProp] = 0;
        _vPolicyToProperty.remove( property );
    }
}

/*!
    \class QtSizePolicyPropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtSizePolicyPropertyManager provides and manages QSizePolicy properties.

    A size policy property has nested \e horizontalPolicy, \e
    verticalPolicy, \e horizontalStretch and \e verticalStretch
    subproperties. The top-level property's value can be retrieved
    using the value() function, and set using the setValue() slot.

    The subproperties are created by QtIntPropertyManager and QtEnumPropertyManager
    objects. These managers can be retrieved using the subIntPropertyManager()
    and subEnumPropertyManager() functions respectively. In order to provide
    editing widgets for the subproperties in a property browser widget,
    these managers must be associated with editor factories.

    In addition, QtSizePolicyPropertyManager provides the valueChanged()
    signal which is emitted whenever a property created by this
    manager changes.

    \sa QtAbstractPropertyManager, QtIntPropertyManager, QtEnumPropertyManager
*/

/*!
    \fn void QtSizePolicyPropertyManager::valueChanged(QtProperty *property, const QSizePolicy &value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the
    new \a value as parameters.

    \sa setValue()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtSizePolicyPropertyManager::QtSizePolicyPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtSizePolicyPropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
    
    d_ptr->_intPropertyManager = new QtIntPropertyManager( this );
    connect( d_ptr->_intPropertyManager, SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotIntChanged( QtProperty *, int ) ) );
    d_ptr->_enumPropertyManager = new QtEnumPropertyManager( this );
    connect( d_ptr->_enumPropertyManager, SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotEnumChanged( QtProperty *, int ) ) );
             
    connect( d_ptr->_intPropertyManager, SIGNAL( propertyDestroyed( QtProperty * ) ),
             this, SLOT( slotPropertyDestroyed( QtProperty * ) ) );
    connect( d_ptr->_enumPropertyManager, SIGNAL( propertyDestroyed( QtProperty * ) ),
             this, SLOT( slotPropertyDestroyed( QtProperty * ) ) );
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtSizePolicyPropertyManager::~QtSizePolicyPropertyManager()
{
    clear();
}

/*!
    Returns the manager that creates the nested \e horizontalStretch
    and \e verticalStretch subproperties.

    In order to provide editing widgets for the mentioned subproperties
    in a property browser widget, this manager must be associated with
    an editor factory.

    \sa QtAbstractPropertyBrowser::setFactoryForManager()
*/
QtIntPropertyManager * QtSizePolicyPropertyManager::subIntPropertyManager() const
{
    return d_ptr->_intPropertyManager;
}

/*!
    Returns the manager that creates the nested \e horizontalPolicy
    and \e verticalPolicy subproperties.

    In order to provide editing widgets for the mentioned subproperties
    in a property browser widget, this manager must be associated with
    an editor factory.

    \sa QtAbstractPropertyBrowser::setFactoryForManager()
*/
QtEnumPropertyManager * QtSizePolicyPropertyManager::subEnumPropertyManager() const
{
    return d_ptr->_enumPropertyManager;
}

/*!
    Returns the given \a property's value.

    If the given property is not managed by this manager, this
    function returns the default size policy.

    \sa setValue()
*/
QSizePolicy QtSizePolicyPropertyManager::value( const QtProperty * property ) const
{
    return d_ptr->_values.value( property, QSizePolicy() );
}

/*!
    \reimp
*/
QString QtSizePolicyPropertyManager::valueText( const QtProperty * property ) const
{
    const QtSizePolicyPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
        
    const QSizePolicy sp = it.value();
    const QtMetaEnumProvider * mep = metaEnumProvider();
    const int hIndex = mep->sizePolicyToIndex( sp.horizontalPolicy() );
    const int vIndex = mep->sizePolicyToIndex( sp.verticalPolicy() );
    //! Unknown size policy on reading invalid uic3 files
    const QString hPolicy = hIndex != -1 ? mep->policyEnumNames().at( hIndex ) : tr( "<Invalid>" );
    const QString vPolicy = vIndex != -1 ? mep->policyEnumNames().at( vIndex ) : tr( "<Invalid>" );
    const QString str = tr( "[%1, %2, %3, %4]" ).arg( hPolicy, vPolicy ).arg( sp.horizontalStretch() ).arg( sp.verticalStretch() );
    return str;
}

/*!
    \fn void QtSizePolicyPropertyManager::setValue(QtProperty *property, const QSizePolicy &value)

    Sets the value of the given \a property to \a value. Nested
    properties are updated automatically.

    \sa value(), valueChanged()
*/
void QtSizePolicyPropertyManager::setValue( QtProperty * property, const QSizePolicy & val )
{
    const QtSizePolicyPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    if( it.value() == val )
        return;
        
    it.value() = val;
    
    d_ptr->_enumPropertyManager->setValue( d_ptr->_propertyToHPolicy[property],
                                           metaEnumProvider()->sizePolicyToIndex( val.horizontalPolicy() ) );
    d_ptr->_enumPropertyManager->setValue( d_ptr->_propertyToVPolicy[property],
                                           metaEnumProvider()->sizePolicyToIndex( val.verticalPolicy() ) );
    d_ptr->_intPropertyManager->setValue( d_ptr->_propertyToHStretch[property],
                                          val.horizontalStretch() );
    d_ptr->_intPropertyManager->setValue( d_ptr->_propertyToVStretch[property],
                                          val.verticalStretch() );
                                          
    emit propertyChanged( property );
    emit valueChanged( property, val );
}

/*!
    \reimp
*/
void QtSizePolicyPropertyManager::initializeProperty( QtProperty * property )
{
    QSizePolicy val;
    d_ptr->_values[property] = val;
    
    QtProperty * hPolicyProp = d_ptr->_enumPropertyManager->addProperty();
    hPolicyProp->setPropertyName( tr( "Horizontal Policy" ) );
    d_ptr->_enumPropertyManager->setEnumNames( hPolicyProp, metaEnumProvider()->policyEnumNames() );
    d_ptr->_enumPropertyManager->setValue( hPolicyProp,
                                           metaEnumProvider()->sizePolicyToIndex( val.horizontalPolicy() ) );
    d_ptr->_propertyToHPolicy[property] = hPolicyProp;
    d_ptr->_hPolicyToProperty[hPolicyProp] = property;
    property->addSubProperty( hPolicyProp );
    
    QtProperty * vPolicyProp = d_ptr->_enumPropertyManager->addProperty();
    vPolicyProp->setPropertyName( tr( "Vertical Policy" ) );
    d_ptr->_enumPropertyManager->setEnumNames( vPolicyProp, metaEnumProvider()->policyEnumNames() );
    d_ptr->_enumPropertyManager->setValue( vPolicyProp,
                                           metaEnumProvider()->sizePolicyToIndex( val.verticalPolicy() ) );
    d_ptr->_propertyToVPolicy[property] = vPolicyProp;
    d_ptr->_vPolicyToProperty[vPolicyProp] = property;
    property->addSubProperty( vPolicyProp );
    
    QtProperty * hStretchProp = d_ptr->_intPropertyManager->addProperty();
    hStretchProp->setPropertyName( tr( "Horizontal Stretch" ) );
    d_ptr->_intPropertyManager->setValue( hStretchProp, val.horizontalStretch() );
    d_ptr->_intPropertyManager->setRange( hStretchProp, 0, 0xff );
    d_ptr->_propertyToHStretch[property] = hStretchProp;
    d_ptr->_hStretchToProperty[hStretchProp] = property;
    property->addSubProperty( hStretchProp );
    
    QtProperty * vStretchProp = d_ptr->_intPropertyManager->addProperty();
    vStretchProp->setPropertyName( tr( "Vertical Stretch" ) );
    d_ptr->_intPropertyManager->setValue( vStretchProp, val.verticalStretch() );
    d_ptr->_intPropertyManager->setRange( vStretchProp, 0, 0xff );
    d_ptr->_propertyToVStretch[property] = vStretchProp;
    d_ptr->_vStretchToProperty[vStretchProp] = property;
    property->addSubProperty( vStretchProp );
    
}

/*!
    \reimp
*/
void QtSizePolicyPropertyManager::uninitializeProperty( QtProperty * property )
{
    QtProperty * hPolicyProp = d_ptr->_propertyToHPolicy[property];
    if( hPolicyProp )
    {
        d_ptr->_hPolicyToProperty.remove( hPolicyProp );
        delete hPolicyProp;
    }
    d_ptr->_propertyToHPolicy.remove( property );
    
    QtProperty * vPolicyProp = d_ptr->_propertyToVPolicy[property];
    if( vPolicyProp )
    {
        d_ptr->_vPolicyToProperty.remove( vPolicyProp );
        delete vPolicyProp;
    }
    d_ptr->_propertyToVPolicy.remove( property );
    
    QtProperty * hStretchProp = d_ptr->_propertyToHStretch[property];
    if( hStretchProp )
    {
        d_ptr->_hStretchToProperty.remove( hStretchProp );
        delete hStretchProp;
    }
    d_ptr->_propertyToHStretch.remove( property );
    
    QtProperty * vStretchProp = d_ptr->_propertyToVStretch[property];
    if( vStretchProp )
    {
        d_ptr->_vStretchToProperty.remove( vStretchProp );
        delete vStretchProp;
    }
    d_ptr->_propertyToVStretch.remove( property );
    
    d_ptr->_values.remove( property );
}

// QtFontPropertyManager:
// QtFontPropertyManagerPrivate has a mechanism for reacting
// to QApplication::fontDatabaseChanged() [4.5], which is emitted
// when someone loads an application font. The signals are compressed
// using a timer with interval 0, which then causes the family
// enumeration manager to re-set its strings and index values
// for each property.

Q_GLOBAL_STATIC( QFontDatabase, fontDatabase )

class QtFontPropertyManagerPrivate
{
    QtFontPropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtFontPropertyManager )
public:

    QtFontPropertyManagerPrivate();
    
    void slotIntChanged( QtProperty * property, int value );
    void slotEnumChanged( QtProperty * property, int value );
    void slotBoolChanged( QtProperty * property, bool value );
    void slotPropertyDestroyed( QtProperty * property );
    void slotFontDatabaseChanged();
    void slotFontDatabaseDelayedChange();
    
    QStringList _familyNames;
    
    typedef QMap< const QtProperty *, QFont > PropertyValueMap;
    PropertyValueMap _values;
    
    QtIntPropertyManager * _intPropertyManager;
    QtEnumPropertyManager * _enumPropertyManager;
    QtBoolPropertyManager * _boolPropertyManager;
    
    QMap< const QtProperty *, QtProperty * > _propertyToFamily;
    QMap< const QtProperty *, QtProperty * > _propertyToPointSize;
    QMap< const QtProperty *, QtProperty * > _propertyToBold;
    QMap< const QtProperty *, QtProperty * > _propertyToItalic;
    QMap< const QtProperty *, QtProperty * > _propertyToUnderline;
    QMap< const QtProperty *, QtProperty * > _propertyToStrikeOut;
    QMap< const QtProperty *, QtProperty * > _propertyToKerning;
    
    QMap< const QtProperty *, QtProperty * > _familyToProperty;
    QMap< const QtProperty *, QtProperty * > _pointSizeToProperty;
    QMap< const QtProperty *, QtProperty * > _boldToProperty;
    QMap< const QtProperty *, QtProperty * > _italicToProperty;
    QMap< const QtProperty *, QtProperty * > _underlineToProperty;
    QMap< const QtProperty *, QtProperty * > _strikeOutToProperty;
    QMap< const QtProperty *, QtProperty * > _kerningToProperty;
    
    bool _settingValue;
    QTimer * _fontDatabaseChangeTimer;
};

QtFontPropertyManagerPrivate::QtFontPropertyManagerPrivate() :
    _settingValue( false ),
    _fontDatabaseChangeTimer( 0 )
{
}

void QtFontPropertyManagerPrivate::slotIntChanged( QtProperty * property, int value )
{
    if( _settingValue )
        return;
    if( QtProperty * prop = _pointSizeToProperty.value( property, 0 ) )
    {
        QFont f = _values[prop];
        f.setPointSize( value );
        q_ptr->setValue( prop, f );
    }
}

void QtFontPropertyManagerPrivate::slotEnumChanged( QtProperty * property, int value )
{
    if( _settingValue )
        return;
    if( QtProperty * prop = _familyToProperty.value( property, 0 ) )
    {
        QFont f = _values[prop];
        f.setFamily( _familyNames.at( value ) );
        q_ptr->setValue( prop, f );
    }
}

void QtFontPropertyManagerPrivate::slotBoolChanged( QtProperty * property, bool value )
{
    if( _settingValue )
        return;
    if( QtProperty * boldProp = _boldToProperty.value( property, 0 ) )
    {
        QFont f = _values[boldProp];
        f.setBold( value );
        q_ptr->setValue( boldProp, f );
    }
    else if( QtProperty * italicProp = _italicToProperty.value( property, 0 ) )
    {
        QFont f = _values[italicProp];
        f.setItalic( value );
        q_ptr->setValue( italicProp, f );
    }
    else if( QtProperty * underlineProp = _underlineToProperty.value( property, 0 ) )
    {
        QFont f = _values[underlineProp];
        f.setUnderline( value );
        q_ptr->setValue( underlineProp, f );
    }
    else if( QtProperty * strikeOutProp = _strikeOutToProperty.value( property, 0 ) )
    {
        QFont f = _values[strikeOutProp];
        f.setStrikeOut( value );
        q_ptr->setValue( strikeOutProp, f );
    }
    else if( QtProperty * kerningProp = _kerningToProperty.value( property, 0 ) )
    {
        QFont f = _values[kerningProp];
        f.setKerning( value );
        q_ptr->setValue( kerningProp, f );
    }
}

void QtFontPropertyManagerPrivate::slotPropertyDestroyed( QtProperty * property )
{
    if( QtProperty * pointSizeProp = _pointSizeToProperty.value( property, 0 ) )
    {
        _propertyToPointSize[pointSizeProp] = 0;
        _pointSizeToProperty.remove( property );
    }
    else if( QtProperty * pointFamilyProp = _familyToProperty.value( property, 0 ) )
    {
        _propertyToFamily[pointFamilyProp] = 0;
        _familyToProperty.remove( property );
    }
    else if( QtProperty * pointBoldProp = _boldToProperty.value( property, 0 ) )
    {
        _propertyToBold[pointBoldProp] = 0;
        _boldToProperty.remove( property );
    }
    else if( QtProperty * pointItalicProp = _italicToProperty.value( property, 0 ) )
    {
        _propertyToItalic[pointItalicProp] = 0;
        _italicToProperty.remove( property );
    }
    else if( QtProperty * pointUnderlineProp = _underlineToProperty.value( property, 0 ) )
    {
        _propertyToUnderline[pointUnderlineProp] = 0;
        _underlineToProperty.remove( property );
    }
    else if( QtProperty * pointStrikeOutProp = _strikeOutToProperty.value( property, 0 ) )
    {
        _propertyToStrikeOut[pointStrikeOutProp] = 0;
        _strikeOutToProperty.remove( property );
    }
    else if( QtProperty * pointKerningProp = _kerningToProperty.value( property, 0 ) )
    {
        _propertyToKerning[pointKerningProp] = 0;
        _kerningToProperty.remove( property );
    }
}

void  QtFontPropertyManagerPrivate::slotFontDatabaseChanged()
{
    if( !_fontDatabaseChangeTimer )
    {
        _fontDatabaseChangeTimer = new QTimer( q_ptr );
        _fontDatabaseChangeTimer->setInterval( 0 );
        _fontDatabaseChangeTimer->setSingleShot( true );
        QObject::connect( _fontDatabaseChangeTimer, SIGNAL( timeout() ), q_ptr, SLOT( slotFontDatabaseDelayedChange() ) );
    }
    if( !_fontDatabaseChangeTimer->isActive() )
        _fontDatabaseChangeTimer->start();
}

void QtFontPropertyManagerPrivate::slotFontDatabaseDelayedChange()
{
    typedef QMap< const QtProperty *, QtProperty * > PropertyPropertyMap;
    // rescan available font names
    const QStringList oldFamilies = _familyNames;
    _familyNames = fontDatabase()->families();
    
    // Adapt all existing properties
    if( !_propertyToFamily.empty() )
    {
        PropertyPropertyMap::const_iterator cend = _propertyToFamily.constEnd();
        for( PropertyPropertyMap::const_iterator it = _propertyToFamily.constBegin(); it != cend; ++it )
        {
            QtProperty * familyProp = it.value();
            const int oldIdx = _enumPropertyManager->value( familyProp );
            int newIdx = _familyNames.indexOf( oldFamilies.at( oldIdx ) );
            if( newIdx < 0 )
                newIdx = 0;
            _enumPropertyManager->setEnumNames( familyProp, _familyNames );
            _enumPropertyManager->setValue( familyProp, newIdx );
        }
    }
}

/*!
    \class QtFontPropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtFontPropertyManager provides and manages QFont properties.

    A font property has nested \e family, \e pointSize, \e bold, \e
    italic, \e underline, \e strikeOut and \e kerning subproperties. The top-level
    property's value can be retrieved using the value() function, and
    set using the setValue() slot.

    The subproperties are created by QtIntPropertyManager, QtEnumPropertyManager and
    QtBoolPropertyManager objects. These managers can be retrieved using the
    corresponding subIntPropertyManager(), subEnumPropertyManager() and
    subBoolPropertyManager() functions. In order to provide editing widgets
    for the subproperties in a property browser widget, these managers
    must be associated with editor factories.

    In addition, QtFontPropertyManager provides the valueChanged() signal
    which is emitted whenever a property created by this manager
    changes.

    \sa QtAbstractPropertyManager, QtEnumPropertyManager, QtIntPropertyManager, QtBoolPropertyManager
*/

/*!
    \fn void QtFontPropertyManager::valueChanged(QtProperty *property, const QFont &value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the
    new \a value as parameters.

    \sa setValue()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtFontPropertyManager::QtFontPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtFontPropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
    QObject::connect( qApp, SIGNAL( fontDatabaseChanged() ), this, SLOT( slotFontDatabaseChanged() ) );
    
    d_ptr->_intPropertyManager = new QtIntPropertyManager( this );
    connect( d_ptr->_intPropertyManager, SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotIntChanged( QtProperty *, int ) ) );
    d_ptr->_enumPropertyManager = new QtEnumPropertyManager( this );
    connect( d_ptr->_enumPropertyManager, SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotEnumChanged( QtProperty *, int ) ) );
    d_ptr->_boolPropertyManager = new QtBoolPropertyManager( this );
    connect( d_ptr->_boolPropertyManager, SIGNAL( valueChanged( QtProperty *, bool ) ),
             this, SLOT( slotBoolChanged( QtProperty *, bool ) ) );
             
    connect( d_ptr->_intPropertyManager, SIGNAL( propertyDestroyed( QtProperty * ) ),
             this, SLOT( slotPropertyDestroyed( QtProperty * ) ) );
    connect( d_ptr->_enumPropertyManager, SIGNAL( propertyDestroyed( QtProperty * ) ),
             this, SLOT( slotPropertyDestroyed( QtProperty * ) ) );
    connect( d_ptr->_boolPropertyManager, SIGNAL( propertyDestroyed( QtProperty * ) ),
             this, SLOT( slotPropertyDestroyed( QtProperty * ) ) );
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtFontPropertyManager::~QtFontPropertyManager()
{
    clear();
}

/*!
    Returns the manager that creates the \e pointSize subproperty.

    In order to provide editing widgets for the \e pointSize property
    in a property browser widget, this manager must be associated
    with an editor factory.

    \sa QtAbstractPropertyBrowser::setFactoryForManager()
*/
QtIntPropertyManager * QtFontPropertyManager::subIntPropertyManager() const
{
    return d_ptr->_intPropertyManager;
}

/*!
    Returns the manager that create the \e family subproperty.

    In order to provide editing widgets for the \e family property
    in a property browser widget, this manager must be associated
    with an editor factory.

    \sa QtAbstractPropertyBrowser::setFactoryForManager()
*/
QtEnumPropertyManager * QtFontPropertyManager::subEnumPropertyManager() const
{
    return d_ptr->_enumPropertyManager;
}

/*!
    Returns the manager that creates the  \e bold, \e italic, \e underline,
    \e strikeOut and \e kerning subproperties.

    In order to provide editing widgets for the mentioned properties
    in a property browser widget, this manager must be associated with
    an editor factory.

    \sa QtAbstractPropertyBrowser::setFactoryForManager()
*/
QtBoolPropertyManager * QtFontPropertyManager::subBoolPropertyManager() const
{
    return d_ptr->_boolPropertyManager;
}

/*!
    Returns the given \a property's value.

    If the given property is not managed by this manager, this
    function returns a font object that uses the application's default
    font.

    \sa setValue()
*/
QFont QtFontPropertyManager::value( const QtProperty * property ) const
{
    return d_ptr->_values.value( property, QFont() );
}

/*!
    \reimp
*/
QString QtFontPropertyManager::valueText( const QtProperty * property ) const
{
    const QtFontPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
        
    return QtPropertyBrowserUtils::fontValueText( it.value() );
}

/*!
    \reimp
*/
QIcon QtFontPropertyManager::valueIcon( const QtProperty * property ) const
{
    const QtFontPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QIcon();
        
    return QtPropertyBrowserUtils::fontValueIcon( it.value() );
}

/*!
    \fn void QtFontPropertyManager::setValue(QtProperty *property, const QFont &value)

    Sets the value of the given \a property to \a value. Nested
    properties are updated automatically.

    \sa value(), valueChanged()
*/
void QtFontPropertyManager::setValue( QtProperty * property, const QFont & val )
{
    const QtFontPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    const QFont oldVal = it.value();
    if( oldVal == val && oldVal.resolve() == val.resolve() )
        return;
        
    it.value() = val;
    
    int idx = d_ptr->_familyNames.indexOf( val.family() );
    if( idx == -1 )
        idx = 0;
    bool settingValue = d_ptr->_settingValue;
    d_ptr->_settingValue = true;
    d_ptr->_enumPropertyManager->setValue( d_ptr->_propertyToFamily[property], idx );
    d_ptr->_intPropertyManager->setValue( d_ptr->_propertyToPointSize[property], val.pointSize() );
    d_ptr->_boolPropertyManager->setValue( d_ptr->_propertyToBold[property], val.bold() );
    d_ptr->_boolPropertyManager->setValue( d_ptr->_propertyToItalic[property], val.italic() );
    d_ptr->_boolPropertyManager->setValue( d_ptr->_propertyToUnderline[property], val.underline() );
    d_ptr->_boolPropertyManager->setValue( d_ptr->_propertyToStrikeOut[property], val.strikeOut() );
    d_ptr->_boolPropertyManager->setValue( d_ptr->_propertyToKerning[property], val.kerning() );
    d_ptr->_settingValue = settingValue;
    
    emit propertyChanged( property );
    emit valueChanged( property, val );
}

/*!
    \reimp
*/
void QtFontPropertyManager::initializeProperty( QtProperty * property )
{
    QFont val;
    d_ptr->_values[property] = val;
    
    QtProperty * familyProp = d_ptr->_enumPropertyManager->addProperty();
    familyProp->setPropertyName( tr( "Family" ) );
    if( d_ptr->_familyNames.empty() )
        d_ptr->_familyNames = fontDatabase()->families();
    d_ptr->_enumPropertyManager->setEnumNames( familyProp, d_ptr->_familyNames );
    int idx = d_ptr->_familyNames.indexOf( val.family() );
    if( idx == -1 )
        idx = 0;
    d_ptr->_enumPropertyManager->setValue( familyProp, idx );
    d_ptr->_propertyToFamily[property] = familyProp;
    d_ptr->_familyToProperty[familyProp] = property;
    property->addSubProperty( familyProp );
    
    QtProperty * pointSizeProp = d_ptr->_intPropertyManager->addProperty();
    pointSizeProp->setPropertyName( tr( "Point Size" ) );
    d_ptr->_intPropertyManager->setValue( pointSizeProp, val.pointSize() );
    d_ptr->_intPropertyManager->setMinimum( pointSizeProp, 1 );
    d_ptr->_propertyToPointSize[property] = pointSizeProp;
    d_ptr->_pointSizeToProperty[pointSizeProp] = property;
    property->addSubProperty( pointSizeProp );
    
    QtProperty * boldProp = d_ptr->_boolPropertyManager->addProperty();
    boldProp->setPropertyName( tr( "Bold" ) );
    d_ptr->_boolPropertyManager->setValue( boldProp, val.bold() );
    d_ptr->_propertyToBold[property] = boldProp;
    d_ptr->_boldToProperty[boldProp] = property;
    property->addSubProperty( boldProp );
    
    QtProperty * italicProp = d_ptr->_boolPropertyManager->addProperty();
    italicProp->setPropertyName( tr( "Italic" ) );
    d_ptr->_boolPropertyManager->setValue( italicProp, val.italic() );
    d_ptr->_propertyToItalic[property] = italicProp;
    d_ptr->_italicToProperty[italicProp] = property;
    property->addSubProperty( italicProp );
    
    QtProperty * underlineProp = d_ptr->_boolPropertyManager->addProperty();
    underlineProp->setPropertyName( tr( "Underline" ) );
    d_ptr->_boolPropertyManager->setValue( underlineProp, val.underline() );
    d_ptr->_propertyToUnderline[property] = underlineProp;
    d_ptr->_underlineToProperty[underlineProp] = property;
    property->addSubProperty( underlineProp );
    
    QtProperty * strikeOutProp = d_ptr->_boolPropertyManager->addProperty();
    strikeOutProp->setPropertyName( tr( "Strikeout" ) );
    d_ptr->_boolPropertyManager->setValue( strikeOutProp, val.strikeOut() );
    d_ptr->_propertyToStrikeOut[property] = strikeOutProp;
    d_ptr->_strikeOutToProperty[strikeOutProp] = property;
    property->addSubProperty( strikeOutProp );
    
    QtProperty * kerningProp = d_ptr->_boolPropertyManager->addProperty();
    kerningProp->setPropertyName( tr( "Kerning" ) );
    d_ptr->_boolPropertyManager->setValue( kerningProp, val.kerning() );
    d_ptr->_propertyToKerning[property] = kerningProp;
    d_ptr->_kerningToProperty[kerningProp] = property;
    property->addSubProperty( kerningProp );
}

/*!
    \reimp
*/
void QtFontPropertyManager::uninitializeProperty( QtProperty * property )
{
    QtProperty * familyProp = d_ptr->_propertyToFamily[property];
    if( familyProp )
    {
        d_ptr->_familyToProperty.remove( familyProp );
        delete familyProp;
    }
    d_ptr->_propertyToFamily.remove( property );
    
    QtProperty * pointSizeProp = d_ptr->_propertyToPointSize[property];
    if( pointSizeProp )
    {
        d_ptr->_pointSizeToProperty.remove( pointSizeProp );
        delete pointSizeProp;
    }
    d_ptr->_propertyToPointSize.remove( property );
    
    QtProperty * boldProp = d_ptr->_propertyToBold[property];
    if( boldProp )
    {
        d_ptr->_boldToProperty.remove( boldProp );
        delete boldProp;
    }
    d_ptr->_propertyToBold.remove( property );
    
    QtProperty * italicProp = d_ptr->_propertyToItalic[property];
    if( italicProp )
    {
        d_ptr->_italicToProperty.remove( italicProp );
        delete italicProp;
    }
    d_ptr->_propertyToItalic.remove( property );
    
    QtProperty * underlineProp = d_ptr->_propertyToUnderline[property];
    if( underlineProp )
    {
        d_ptr->_underlineToProperty.remove( underlineProp );
        delete underlineProp;
    }
    d_ptr->_propertyToUnderline.remove( property );
    
    QtProperty * strikeOutProp = d_ptr->_propertyToStrikeOut[property];
    if( strikeOutProp )
    {
        d_ptr->_strikeOutToProperty.remove( strikeOutProp );
        delete strikeOutProp;
    }
    d_ptr->_propertyToStrikeOut.remove( property );
    
    QtProperty * kerningProp = d_ptr->_propertyToKerning[property];
    if( kerningProp )
    {
        d_ptr->_kerningToProperty.remove( kerningProp );
        delete kerningProp;
    }
    d_ptr->_propertyToKerning.remove( property );
    
    d_ptr->_values.remove( property );
}

// QtColorPropertyManager

class QtColorPropertyManagerPrivate
{
    QtColorPropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtColorPropertyManager )
public:

    void slotIntChanged( QtProperty * property, int value );
    void slotPropertyDestroyed( QtProperty * property );
    
    typedef QMap< const QtProperty *, QColor > PropertyValueMap;
    PropertyValueMap _values;
    
    QtIntPropertyManager * _intPropertyManager;
    
    QMap< const QtProperty *, QtProperty * > _propertyToR;
    QMap< const QtProperty *, QtProperty * > _propertyToG;
    QMap< const QtProperty *, QtProperty * > _propertyToB;
    QMap< const QtProperty *, QtProperty * > _propertyToA;
    
    QMap< const QtProperty *, QtProperty * > _rToProperty;
    QMap< const QtProperty *, QtProperty * > _gToProperty;
    QMap< const QtProperty *, QtProperty * > _bToProperty;
    QMap< const QtProperty *, QtProperty * > _aToProperty;
};

void QtColorPropertyManagerPrivate::slotIntChanged( QtProperty * property, int value )
{
    if( QtProperty * rProp = _rToProperty.value( property, 0 ) )
    {
        QColor c = _values[rProp];
        c.setRed( value );
        q_ptr->setValue( rProp, c );
    }
    else if( QtProperty * gProp = _gToProperty.value( property, 0 ) )
    {
        QColor c = _values[gProp];
        c.setGreen( value );
        q_ptr->setValue( gProp, c );
    }
    else if( QtProperty * bProp = _bToProperty.value( property, 0 ) )
    {
        QColor c = _values[bProp];
        c.setBlue( value );
        q_ptr->setValue( bProp, c );
    }
    else if( QtProperty * aProp = _aToProperty.value( property, 0 ) )
    {
        QColor c = _values[aProp];
        c.setAlpha( value );
        q_ptr->setValue( aProp, c );
    }
}

void QtColorPropertyManagerPrivate::slotPropertyDestroyed( QtProperty * property )
{
    if( QtProperty * pointRProp = _rToProperty.value( property, 0 ) )
    {
        _propertyToR[pointRProp] = 0;
        _rToProperty.remove( property );
    }
    else if( QtProperty * pointGProp = _gToProperty.value( property, 0 ) )
    {
        _propertyToG[pointGProp] = 0;
        _gToProperty.remove( property );
    }
    else if( QtProperty * pointBProp = _bToProperty.value( property, 0 ) )
    {
        _propertyToB[pointBProp] = 0;
        _bToProperty.remove( property );
    }
    else if( QtProperty * pointAProp = _aToProperty.value( property, 0 ) )
    {
        _propertyToA[pointAProp] = 0;
        _aToProperty.remove( property );
    }
}

/*!
    \class QtColorPropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtColorPropertyManager provides and manages QColor properties.

    A color property has nested \e red, \e green and \e blue
    subproperties. The top-level property's value can be retrieved
    using the value() function, and set using the setValue() slot.

    The subproperties are created by a QtIntPropertyManager object. This
    manager can be retrieved using the subIntPropertyManager() function.  In
    order to provide editing widgets for the subproperties in a
    property browser widget, this manager must be associated with an
    editor factory.

    In addition, QtColorPropertyManager provides the valueChanged() signal
    which is emitted whenever a property created by this manager
    changes.

    \sa QtAbstractPropertyManager, QtAbstractPropertyBrowser, QtIntPropertyManager
*/

/*!
    \fn void QtColorPropertyManager::valueChanged(QtProperty *property, const QColor &value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the new
    \a value as parameters.

    \sa setValue()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtColorPropertyManager::QtColorPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtColorPropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
    
    d_ptr->_intPropertyManager = new QtIntPropertyManager( this );
    connect( d_ptr->_intPropertyManager, SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotIntChanged( QtProperty *, int ) ) );
             
    connect( d_ptr->_intPropertyManager, SIGNAL( propertyDestroyed( QtProperty * ) ),
             this, SLOT( slotPropertyDestroyed( QtProperty * ) ) );
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtColorPropertyManager::~QtColorPropertyManager()
{
    clear();
}

/*!
    Returns the manager that produces the nested \e red, \e green and
    \e blue subproperties.

    In order to provide editing widgets for the subproperties in a
    property browser widget, this manager must be associated with an
    editor factory.

    \sa QtAbstractPropertyBrowser::setFactoryForManager()
*/
QtIntPropertyManager * QtColorPropertyManager::subIntPropertyManager() const
{
    return d_ptr->_intPropertyManager;
}

/*!
    Returns the given \a property's value.

    If the given \a property is not managed by \e this manager, this
    function returns an invalid color.

    \sa setValue()
*/
QColor QtColorPropertyManager::value( const QtProperty * property ) const
{
    return d_ptr->_values.value( property, QColor() );
}

/*!
    \reimp
*/

QString QtColorPropertyManager::valueText( const QtProperty * property ) const
{
    const QtColorPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
        
    return QtPropertyBrowserUtils::colorValueText( it.value() );
}

/*!
    \reimp
*/

QIcon QtColorPropertyManager::valueIcon( const QtProperty * property ) const
{
    const QtColorPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QIcon();
    return QtPropertyBrowserUtils::brushValueIcon( QBrush( it.value() ) );
}

/*!
    \fn void QtColorPropertyManager::setValue(QtProperty *property, const QColor &value)

    Sets the value of the given \a property to \a value.  Nested
    properties are updated automatically.

    \sa value(), valueChanged()
*/
void QtColorPropertyManager::setValue( QtProperty * property, const QColor & val )
{
    const QtColorPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    if( it.value() == val )
        return;
        
    it.value() = val;
    
    d_ptr->_intPropertyManager->setValue( d_ptr->_propertyToR[property], val.red() );
    d_ptr->_intPropertyManager->setValue( d_ptr->_propertyToG[property], val.green() );
    d_ptr->_intPropertyManager->setValue( d_ptr->_propertyToB[property], val.blue() );
    d_ptr->_intPropertyManager->setValue( d_ptr->_propertyToA[property], val.alpha() );
    
    emit propertyChanged( property );
    emit valueChanged( property, val );
}

/*!
    \reimp
*/
void QtColorPropertyManager::initializeProperty( QtProperty * property )
{
    QColor val;
    d_ptr->_values[property] = val;
    
    QtProperty * rProp = d_ptr->_intPropertyManager->addProperty();
    rProp->setPropertyName( tr( "Red" ) );
    d_ptr->_intPropertyManager->setValue( rProp, val.red() );
    d_ptr->_intPropertyManager->setRange( rProp, 0, 0xFF );
    d_ptr->_propertyToR[property] = rProp;
    d_ptr->_rToProperty[rProp] = property;
    property->addSubProperty( rProp );
    
    QtProperty * gProp = d_ptr->_intPropertyManager->addProperty();
    gProp->setPropertyName( tr( "Green" ) );
    d_ptr->_intPropertyManager->setValue( gProp, val.green() );
    d_ptr->_intPropertyManager->setRange( gProp, 0, 0xFF );
    d_ptr->_propertyToG[property] = gProp;
    d_ptr->_gToProperty[gProp] = property;
    property->addSubProperty( gProp );
    
    QtProperty * bProp = d_ptr->_intPropertyManager->addProperty();
    bProp->setPropertyName( tr( "Blue" ) );
    d_ptr->_intPropertyManager->setValue( bProp, val.blue() );
    d_ptr->_intPropertyManager->setRange( bProp, 0, 0xFF );
    d_ptr->_propertyToB[property] = bProp;
    d_ptr->_bToProperty[bProp] = property;
    property->addSubProperty( bProp );
    
    QtProperty * aProp = d_ptr->_intPropertyManager->addProperty();
    aProp->setPropertyName( tr( "Alpha" ) );
    d_ptr->_intPropertyManager->setValue( aProp, val.alpha() );
    d_ptr->_intPropertyManager->setRange( aProp, 0, 0xFF );
    d_ptr->_propertyToA[property] = aProp;
    d_ptr->_aToProperty[aProp] = property;
    property->addSubProperty( aProp );
}

/*!
    \reimp
*/
void QtColorPropertyManager::uninitializeProperty( QtProperty * property )
{
    QtProperty * rProp = d_ptr->_propertyToR[property];
    if( rProp )
    {
        d_ptr->_rToProperty.remove( rProp );
        delete rProp;
    }
    d_ptr->_propertyToR.remove( property );
    
    QtProperty * gProp = d_ptr->_propertyToG[property];
    if( gProp )
    {
        d_ptr->_gToProperty.remove( gProp );
        delete gProp;
    }
    d_ptr->_propertyToG.remove( property );
    
    QtProperty * bProp = d_ptr->_propertyToB[property];
    if( bProp )
    {
        d_ptr->_bToProperty.remove( bProp );
        delete bProp;
    }
    d_ptr->_propertyToB.remove( property );
    
    QtProperty * aProp = d_ptr->_propertyToA[property];
    if( aProp )
    {
        d_ptr->_aToProperty.remove( aProp );
        delete aProp;
    }
    d_ptr->_propertyToA.remove( property );
    
    d_ptr->_values.remove( property );
}

// QtCursorPropertyManager

// Make sure icons are removed as soon as QApplication is destroyed, otherwise,
// handles are leaked on X11.
static void clearCursorDatabase();
namespace {
    struct CursorDatabase : public QtCursorDatabase
    {
        CursorDatabase()
        {
            qAddPostRoutine( clearCursorDatabase );
        }
    };
}
Q_GLOBAL_STATIC( QtCursorDatabase, cursorDatabase )

static void clearCursorDatabase()
{
    cursorDatabase()->clear();
}

class QtCursorPropertyManagerPrivate
{
    QtCursorPropertyManager * q_ptr;
    Q_DECLARE_PUBLIC( QtCursorPropertyManager )
public:
    typedef QMap< const QtProperty *, QCursor > PropertyValueMap;
    PropertyValueMap _values;
};

/*!
    \class QtCursorPropertyManager
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtCursorPropertyManager provides and manages QCursor properties.

    A cursor property has a current value which can be
    retrieved using the value() function, and set using the setValue()
    slot. In addition, QtCursorPropertyManager provides the
    valueChanged() signal which is emitted whenever a property created
    by this manager changes.

    \sa QtAbstractPropertyManager
*/

/*!
    \fn void QtCursorPropertyManager::valueChanged(QtProperty *property, const QCursor &value)

    This signal is emitted whenever a property created by this manager
    changes its value, passing a pointer to the \a property and the new
    \a value as parameters.

    \sa setValue()
*/

/*!
    Creates a manager with the given \a parent.
*/
QtCursorPropertyManager::QtCursorPropertyManager( QObject * parent )
    : QtAbstractPropertyManager( parent ), d_ptr( new QtCursorPropertyManagerPrivate )
{
    d_ptr->q_ptr = this;
}

/*!
    Destroys this manager, and all the properties it has created.
*/
QtCursorPropertyManager::~QtCursorPropertyManager()
{
    clear();
}

/*!
    Returns the given \a property's value.

    If the given \a property is not managed by this manager, this
    function returns a default QCursor object.

    \sa setValue()
*/
#ifndef QT_NO_CURSOR
QCursor QtCursorPropertyManager::value( const QtProperty * property ) const
{
    return d_ptr->_values.value( property, QCursor() );
}
#endif

/*!
    \reimp
*/
QString QtCursorPropertyManager::valueText( const QtProperty * property ) const
{
    const QtCursorPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QString();
        
    return cursorDatabase()->cursorToShapeName( it.value() );
}

/*!
    \reimp
*/
QIcon QtCursorPropertyManager::valueIcon( const QtProperty * property ) const
{
    const QtCursorPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->_values.constFind( property );
    if( it == d_ptr->_values.constEnd() )
        return QIcon();
        
    return cursorDatabase()->cursorToShapeIcon( it.value() );
}

/*!
    \fn void QtCursorPropertyManager::setValue(QtProperty *property, const QCursor &value)

    Sets the value of the given \a property to \a value.

    \sa value(), valueChanged()
*/
void QtCursorPropertyManager::setValue( QtProperty * property, const QCursor & value )
{
    #ifndef QT_NO_CURSOR
    const QtCursorPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->_values.find( property );
    if( it == d_ptr->_values.end() )
        return;
        
    if( it.value().shape() == value.shape() && value.shape() != Qt::BitmapCursor )
        return;
        
    it.value() = value;
    
    emit propertyChanged( property );
    emit valueChanged( property, value );
    #endif
}

/*!
    \reimp
*/
void QtCursorPropertyManager::initializeProperty( QtProperty * property )
{
    #ifndef QT_NO_CURSOR
    d_ptr->_values[property] = QCursor();
    #endif
}

/*!
    \reimp
*/
void QtCursorPropertyManager::uninitializeProperty( QtProperty * property )
{
    d_ptr->_values.remove( property );
}

QT_END_NAMESPACE

#include "moc_qtpropertymanager.cpp"
#include "qtpropertymanager.moc"
