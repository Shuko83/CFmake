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

#include "qteditorfactory.h"
#include "qtpropertybrowserutils_p.h"

#include <QSpinBox>
#include <QScrollBar>
#include <QComboBox>
#include <QAbstractItemView>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QHBoxLayout>
#include <QMenu>
#include <QKeyEvent>
#include <QApplication>
#include <QLabel>
#include <QToolButton>
#include <QColorDialog>
#include <QFontDialog>
#include <QSpacerItem>
#include <QKeySequenceEdit>
#include <QMap>

#if defined(Q_CC_MSVC)
    #pragma warning(disable: 4786) /* MS VS 6: truncating debug info after 255 characters */
#endif

using namespace sunset::widgets;

QT_BEGIN_NAMESPACE

// Set a hard coded left margin to account for the indentation
// of the tree view icon when switching to an editor

static inline void setupTreeViewEditorMargin( QLayout * lt )
{
    enum { DecorationMargin = 4 };
    if( QApplication::layoutDirection() == Qt::LeftToRight )
        lt->setContentsMargins( DecorationMargin, 0, 0, 0 );
    else
        lt->setContentsMargins( 0, 0, DecorationMargin, 0 );
}

// ---------- EditorFactoryPrivate :
// Base class for editor factory private classes. Manages mapping of properties to editors and vice versa.

template < class Editor >
class EditorFactoryPrivate
{
public:

    typedef QList< Editor * > EditorList;
    typedef QMap< QtProperty *, EditorList > PropertyToEditorListMap;
    typedef QMap< Editor *, QtProperty * > EditorToPropertyMap;
    
    Editor * createEditor( QtProperty * property, QWidget * parent );
    void initializeEditor( QtProperty * property, Editor * e );
    void slotEditorDestroyed( QObject * object );
    
    PropertyToEditorListMap  _createdEditors;
    EditorToPropertyMap _editorToProperty;
};

template < class Editor >
Editor * EditorFactoryPrivate< Editor >::createEditor( QtProperty * property, QWidget * parent )
{
    Editor * editor = new Editor( parent );
    initializeEditor( property, editor );
    return editor;
}

template < class Editor >
void EditorFactoryPrivate< Editor >::initializeEditor( QtProperty * property, Editor * editor )
{
    typename PropertyToEditorListMap::iterator it = _createdEditors.find( property );
    if( it == _createdEditors.end() )
        it = _createdEditors.insert( property, EditorList() );
    it.value().append( editor );
    _editorToProperty.insert( editor, property );
}

template < class Editor >
void EditorFactoryPrivate< Editor >::slotEditorDestroyed( QObject * object )
{
    const typename EditorToPropertyMap::iterator ecend = _editorToProperty.end();
    for( typename EditorToPropertyMap::iterator itEditor = _editorToProperty.begin(); itEditor !=  ecend; ++itEditor )
    {
        if( itEditor.key() == object )
        {
            Editor * editor = itEditor.key();
            QtProperty * property = itEditor.value();
            const typename PropertyToEditorListMap::iterator pit = _createdEditors.find( property );
            if( pit != _createdEditors.end() )
            {
                pit.value().removeAll( editor );
                if( pit.value().empty() )
                    _createdEditors.erase( pit );
            }
            _editorToProperty.erase( itEditor );
            return;
        }
    }
}

// ------------ QtSpinBoxFactory

class QtSpinBoxFactoryPrivate : public EditorFactoryPrivate< QSpinBox >
{
    QtSpinBoxFactory * q_ptr;
    Q_DECLARE_PUBLIC( QtSpinBoxFactory )
public:

    void slotPropertyChanged( QtProperty * property, int value );
    void slotRangeChanged( QtProperty * property, int min, int max );
    void slotSingleStepChanged( QtProperty * property, int step );
    void slotSetValue( int value );
};

void QtSpinBoxFactoryPrivate::slotPropertyChanged( QtProperty * property, int value )
{
    if( !_createdEditors.contains( property ) )
        return;
    QListIterator< QSpinBox * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QSpinBox * editor = itEditor.next();
        if( editor->value() != value )
        {
            editor->blockSignals( true );
            editor->setValue( value );
            editor->blockSignals( false );
        }
    }
}

void QtSpinBoxFactoryPrivate::slotRangeChanged( QtProperty * property, int min, int max )
{
    if( !_createdEditors.contains( property ) )
        return;
        
    QtIntPropertyManager * manager = q_ptr->propertyManager( property );
    if( !manager )
        return;
        
    QListIterator< QSpinBox * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QSpinBox * editor = itEditor.next();
        editor->blockSignals( true );
        editor->setRange( min, max );
        editor->setValue( manager->value( property ) );
        editor->blockSignals( false );
    }
}

void QtSpinBoxFactoryPrivate::slotSingleStepChanged( QtProperty * property, int step )
{
    if( !_createdEditors.contains( property ) )
        return;
    QListIterator< QSpinBox * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QSpinBox * editor = itEditor.next();
        editor->blockSignals( true );
        editor->setSingleStep( step );
        editor->blockSignals( false );
    }
}

void QtSpinBoxFactoryPrivate::slotSetValue( int value )
{
    QObject * object = q_ptr->sender();
    const QMap< QSpinBox *, QtProperty * >::ConstIterator  ecend = _editorToProperty.constEnd();
    for( QMap< QSpinBox *, QtProperty * >::ConstIterator itEditor = _editorToProperty.constBegin(); itEditor !=  ecend; ++itEditor )
    {
        if( itEditor.key() == object )
        {
            QtProperty * property = itEditor.value();
            QtIntPropertyManager * manager = q_ptr->propertyManager( property );
            if( !manager )
                return;
            manager->setValue( property, value );
            return;
        }
    }
}

/*!
    \class QtSpinBoxFactory
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtSpinBoxFactory class provides QSpinBox widgets for
    properties created by QtIntPropertyManager objects.

    \sa QtAbstractEditorFactory, QtIntPropertyManager
*/

/*!
    Creates a factory with the given \a parent.
*/
QtSpinBoxFactory::QtSpinBoxFactory( QObject * parent )
    : QtAbstractEditorFactory< QtIntPropertyManager >( parent ), d_ptr( new QtSpinBoxFactoryPrivate() )
{
    d_ptr->q_ptr = this;
    
}

/*!
    Destroys this factory, and all the widgets it has created.
*/
QtSpinBoxFactory::~QtSpinBoxFactory()
{
    qDeleteAll( d_ptr->_editorToProperty.keys() );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtSpinBoxFactory::connectPropertyManager( QtIntPropertyManager * manager )
{
    connect( manager, SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotPropertyChanged( QtProperty *, int ) ) );
    connect( manager, SIGNAL( rangeChanged( QtProperty *, int, int ) ),
             this, SLOT( slotRangeChanged( QtProperty *, int, int ) ) );
    connect( manager, SIGNAL( singleStepChanged( QtProperty *, int ) ),
             this, SLOT( slotSingleStepChanged( QtProperty *, int ) ) );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget * QtSpinBoxFactory::createEditor( QtIntPropertyManager * manager, QtProperty * property,
        QWidget * parent )
{
    QSpinBox * editor = d_ptr->createEditor( property, parent );
    editor->setSingleStep( manager->singleStep( property ) );
    editor->setRange( manager->minimum( property ), manager->maximum( property ) );
    editor->setValue( manager->value( property ) );
    editor->setKeyboardTracking( false );
    
    connect( editor, SIGNAL( valueChanged( int ) ), this, SLOT( slotSetValue( int ) ) );
    connect( editor, SIGNAL( destroyed( QObject * ) ),
             this, SLOT( slotEditorDestroyed( QObject * ) ) );
    return editor;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtSpinBoxFactory::disconnectPropertyManager( QtIntPropertyManager * manager )
{
    disconnect( manager, SIGNAL( valueChanged( QtProperty *, int ) ),
                this, SLOT( slotPropertyChanged( QtProperty *, int ) ) );
    disconnect( manager, SIGNAL( rangeChanged( QtProperty *, int, int ) ),
                this, SLOT( slotRangeChanged( QtProperty *, int, int ) ) );
    disconnect( manager, SIGNAL( singleStepChanged( QtProperty *, int ) ),
                this, SLOT( slotSingleStepChanged( QtProperty *, int ) ) );
}

// QtSliderFactory

class QtSliderFactoryPrivate : public EditorFactoryPrivate< QSlider >
{
    QtSliderFactory * q_ptr;
    Q_DECLARE_PUBLIC( QtSliderFactory )
public:
    void slotPropertyChanged( QtProperty * property, int value );
    void slotRangeChanged( QtProperty * property, int min, int max );
    void slotSingleStepChanged( QtProperty * property, int step );
    void slotSetValue( int value );
};

void QtSliderFactoryPrivate::slotPropertyChanged( QtProperty * property, int value )
{
    if( !_createdEditors.contains( property ) )
        return;
    QListIterator< QSlider * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QSlider * editor = itEditor.next();
        editor->blockSignals( true );
        editor->setValue( value );
        editor->blockSignals( false );
    }
}

void QtSliderFactoryPrivate::slotRangeChanged( QtProperty * property, int min, int max )
{
    if( !_createdEditors.contains( property ) )
        return;
        
    QtIntPropertyManager * manager = q_ptr->propertyManager( property );
    if( !manager )
        return;
        
    QListIterator< QSlider * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QSlider * editor = itEditor.next();
        editor->blockSignals( true );
        editor->setRange( min, max );
        editor->setValue( manager->value( property ) );
        editor->blockSignals( false );
    }
}

void QtSliderFactoryPrivate::slotSingleStepChanged( QtProperty * property, int step )
{
    if( !_createdEditors.contains( property ) )
        return;
    QListIterator< QSlider * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QSlider * editor = itEditor.next();
        editor->blockSignals( true );
        editor->setSingleStep( step );
        editor->blockSignals( false );
    }
}

void QtSliderFactoryPrivate::slotSetValue( int value )
{
    QObject * object = q_ptr->sender();
    const QMap< QSlider *, QtProperty * >::ConstIterator ecend = _editorToProperty.constEnd();
    for( QMap< QSlider *, QtProperty * >::ConstIterator itEditor = _editorToProperty.constBegin(); itEditor != ecend; ++itEditor )
    {
        if( itEditor.key() == object )
        {
            QtProperty * property = itEditor.value();
            QtIntPropertyManager * manager = q_ptr->propertyManager( property );
            if( !manager )
                return;
            manager->setValue( property, value );
            return;
        }
    }
}

/*!
    \class QtSliderFactory
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtSliderFactory class provides QSlider widgets for
    properties created by QtIntPropertyManager objects.

    \sa QtAbstractEditorFactory, QtIntPropertyManager
*/

/*!
    Creates a factory with the given \a parent.
*/
QtSliderFactory::QtSliderFactory( QObject * parent )
    : QtAbstractEditorFactory< QtIntPropertyManager >( parent ), d_ptr( new QtSliderFactoryPrivate() )
{
    d_ptr->q_ptr = this;
    
}

/*!
    Destroys this factory, and all the widgets it has created.
*/
QtSliderFactory::~QtSliderFactory()
{
    qDeleteAll( d_ptr->_editorToProperty.keys() );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtSliderFactory::connectPropertyManager( QtIntPropertyManager * manager )
{
    connect( manager, SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotPropertyChanged( QtProperty *, int ) ) );
    connect( manager, SIGNAL( rangeChanged( QtProperty *, int, int ) ),
             this, SLOT( slotRangeChanged( QtProperty *, int, int ) ) );
    connect( manager, SIGNAL( singleStepChanged( QtProperty *, int ) ),
             this, SLOT( slotSingleStepChanged( QtProperty *, int ) ) );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget * QtSliderFactory::createEditor( QtIntPropertyManager * manager, QtProperty * property,
        QWidget * parent )
{
    QSlider * editor = new QSlider( Qt::Horizontal, parent );
    d_ptr->initializeEditor( property, editor );
    editor->setSingleStep( manager->singleStep( property ) );
    editor->setRange( manager->minimum( property ), manager->maximum( property ) );
    editor->setValue( manager->value( property ) );
    
    connect( editor, SIGNAL( valueChanged( int ) ), this, SLOT( slotSetValue( int ) ) );
    connect( editor, SIGNAL( destroyed( QObject * ) ),
             this, SLOT( slotEditorDestroyed( QObject * ) ) );
    return editor;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtSliderFactory::disconnectPropertyManager( QtIntPropertyManager * manager )
{
    disconnect( manager, SIGNAL( valueChanged( QtProperty *, int ) ),
                this, SLOT( slotPropertyChanged( QtProperty *, int ) ) );
    disconnect( manager, SIGNAL( rangeChanged( QtProperty *, int, int ) ),
                this, SLOT( slotRangeChanged( QtProperty *, int, int ) ) );
    disconnect( manager, SIGNAL( singleStepChanged( QtProperty *, int ) ),
                this, SLOT( slotSingleStepChanged( QtProperty *, int ) ) );
}

// QtSliderFactory

class QtScrollBarFactoryPrivate : public  EditorFactoryPrivate< QScrollBar >
{
    QtScrollBarFactory * q_ptr;
    Q_DECLARE_PUBLIC( QtScrollBarFactory )
public:
    void slotPropertyChanged( QtProperty * property, int value );
    void slotRangeChanged( QtProperty * property, int min, int max );
    void slotSingleStepChanged( QtProperty * property, int step );
    void slotSetValue( int value );
};

void QtScrollBarFactoryPrivate::slotPropertyChanged( QtProperty * property, int value )
{
    if( !_createdEditors.contains( property ) )
        return;
        
    QListIterator< QScrollBar * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QScrollBar * editor = itEditor.next();
        editor->blockSignals( true );
        editor->setValue( value );
        editor->blockSignals( false );
    }
}

void QtScrollBarFactoryPrivate::slotRangeChanged( QtProperty * property, int min, int max )
{
    if( !_createdEditors.contains( property ) )
        return;
        
    QtIntPropertyManager * manager = q_ptr->propertyManager( property );
    if( !manager )
        return;
        
    QListIterator< QScrollBar * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QScrollBar * editor = itEditor.next();
        editor->blockSignals( true );
        editor->setRange( min, max );
        editor->setValue( manager->value( property ) );
        editor->blockSignals( false );
    }
}

void QtScrollBarFactoryPrivate::slotSingleStepChanged( QtProperty * property, int step )
{
    if( !_createdEditors.contains( property ) )
        return;
    QListIterator< QScrollBar * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QScrollBar * editor = itEditor.next();
        editor->blockSignals( true );
        editor->setSingleStep( step );
        editor->blockSignals( false );
    }
}

void QtScrollBarFactoryPrivate::slotSetValue( int value )
{
    QObject * object = q_ptr->sender();
    const QMap< QScrollBar *, QtProperty * >::ConstIterator ecend = _editorToProperty.constEnd();
    for( QMap< QScrollBar *, QtProperty * >::ConstIterator itEditor = _editorToProperty.constBegin(); itEditor != ecend; ++itEditor )
        if( itEditor.key() == object )
        {
            QtProperty * property = itEditor.value();
            QtIntPropertyManager * manager = q_ptr->propertyManager( property );
            if( !manager )
                return;
            manager->setValue( property, value );
            return;
        }
}

/*!
    \class QtScrollBarFactory
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtScrollBarFactory class provides QScrollBar widgets for
    properties created by QtIntPropertyManager objects.

    \sa QtAbstractEditorFactory, QtIntPropertyManager
*/

/*!
    Creates a factory with the given \a parent.
*/
QtScrollBarFactory::QtScrollBarFactory( QObject * parent )
    : QtAbstractEditorFactory< QtIntPropertyManager >( parent ), d_ptr( new QtScrollBarFactoryPrivate() )
{
    d_ptr->q_ptr = this;
    
}

/*!
    Destroys this factory, and all the widgets it has created.
*/
QtScrollBarFactory::~QtScrollBarFactory()
{
    qDeleteAll( d_ptr->_editorToProperty.keys() );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtScrollBarFactory::connectPropertyManager( QtIntPropertyManager * manager )
{
    connect( manager, SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotPropertyChanged( QtProperty *, int ) ) );
    connect( manager, SIGNAL( rangeChanged( QtProperty *, int, int ) ),
             this, SLOT( slotRangeChanged( QtProperty *, int, int ) ) );
    connect( manager, SIGNAL( singleStepChanged( QtProperty *, int ) ),
             this, SLOT( slotSingleStepChanged( QtProperty *, int ) ) );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget * QtScrollBarFactory::createEditor( QtIntPropertyManager * manager, QtProperty * property,
        QWidget * parent )
{
    QScrollBar * editor = new QScrollBar( Qt::Horizontal, parent );
    d_ptr->initializeEditor( property, editor );
    editor->setSingleStep( manager->singleStep( property ) );
    editor->setRange( manager->minimum( property ), manager->maximum( property ) );
    editor->setValue( manager->value( property ) );
    connect( editor, SIGNAL( valueChanged( int ) ), this, SLOT( slotSetValue( int ) ) );
    connect( editor, SIGNAL( destroyed( QObject * ) ),
             this, SLOT( slotEditorDestroyed( QObject * ) ) );
    return editor;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtScrollBarFactory::disconnectPropertyManager( QtIntPropertyManager * manager )
{
    disconnect( manager, SIGNAL( valueChanged( QtProperty *, int ) ),
                this, SLOT( slotPropertyChanged( QtProperty *, int ) ) );
    disconnect( manager, SIGNAL( rangeChanged( QtProperty *, int, int ) ),
                this, SLOT( slotRangeChanged( QtProperty *, int, int ) ) );
    disconnect( manager, SIGNAL( singleStepChanged( QtProperty *, int ) ),
                this, SLOT( slotSingleStepChanged( QtProperty *, int ) ) );
}

// QtCheckBoxFactory

class QtCheckBoxFactoryPrivate : public EditorFactoryPrivate< QtBoolEdit >
{
    QtCheckBoxFactory * q_ptr;
    Q_DECLARE_PUBLIC( QtCheckBoxFactory )
public:
    void slotPropertyChanged( QtProperty * property, bool value );
    void slotSetValue( bool value );
};

void QtCheckBoxFactoryPrivate::slotPropertyChanged( QtProperty * property, bool value )
{
    if( !_createdEditors.contains( property ) )
        return;
        
    QListIterator< QtBoolEdit * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QtBoolEdit * editor = itEditor.next();
        editor->blockCheckBoxSignals( true );
        editor->setChecked( value );
        editor->blockCheckBoxSignals( false );
    }
}

void QtCheckBoxFactoryPrivate::slotSetValue( bool value )
{
    QObject * object = q_ptr->sender();
    
    const QMap< QtBoolEdit *, QtProperty * >::ConstIterator ecend = _editorToProperty.constEnd();
    for( QMap< QtBoolEdit *, QtProperty * >::ConstIterator itEditor = _editorToProperty.constBegin(); itEditor != ecend;  ++itEditor )
        if( itEditor.key() == object )
        {
            QtProperty * property = itEditor.value();
            QtBoolPropertyManager * manager = q_ptr->propertyManager( property );
            if( !manager )
                return;
            manager->setValue( property, value );
            return;
        }
}

/*!
    \class QtCheckBoxFactory
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtCheckBoxFactory class provides QCheckBox widgets for
    properties created by QtBoolPropertyManager objects.

    \sa QtAbstractEditorFactory, QtBoolPropertyManager
*/

/*!
    Creates a factory with the given \a parent.
*/
QtCheckBoxFactory::QtCheckBoxFactory( QObject * parent )
    : QtAbstractEditorFactory< QtBoolPropertyManager >( parent ), d_ptr( new QtCheckBoxFactoryPrivate() )
{
    d_ptr->q_ptr = this;
    
}

/*!
    Destroys this factory, and all the widgets it has created.
*/
QtCheckBoxFactory::~QtCheckBoxFactory()
{
    qDeleteAll( d_ptr->_editorToProperty.keys() );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtCheckBoxFactory::connectPropertyManager( QtBoolPropertyManager * manager )
{
    connect( manager, SIGNAL( valueChanged( QtProperty *, bool ) ),
             this, SLOT( slotPropertyChanged( QtProperty *, bool ) ) );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget * QtCheckBoxFactory::createEditor( QtBoolPropertyManager * manager, QtProperty * property,
        QWidget * parent )
{
    QtBoolEdit * editor = d_ptr->createEditor( property, parent );
    editor->setChecked( manager->value( property ) );
    
    connect( editor, SIGNAL( toggled( bool ) ), this, SLOT( slotSetValue( bool ) ) );
    connect( editor, SIGNAL( destroyed( QObject * ) ),
             this, SLOT( slotEditorDestroyed( QObject * ) ) );
    return editor;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtCheckBoxFactory::disconnectPropertyManager( QtBoolPropertyManager * manager )
{
    disconnect( manager, SIGNAL( valueChanged( QtProperty *, bool ) ),
                this, SLOT( slotPropertyChanged( QtProperty *, bool ) ) );
}

// QtDoubleSpinBoxFactory

class QtDoubleSpinBoxFactoryPrivate : public EditorFactoryPrivate< QDoubleSpinBox >
{
    QtDoubleSpinBoxFactory * q_ptr;
    Q_DECLARE_PUBLIC( QtDoubleSpinBoxFactory )
public:

    void slotPropertyChanged( QtProperty * property, double value );
    void slotRangeChanged( QtProperty * property, double min, double max );
    void slotSingleStepChanged( QtProperty * property, double step );
    void slotDecimalsChanged( QtProperty * property, int prec );
    void slotSetValue( double value );
};

void QtDoubleSpinBoxFactoryPrivate::slotPropertyChanged( QtProperty * property, double value )
{
    QList< QDoubleSpinBox * > editors = _createdEditors[property];
    QListIterator< QDoubleSpinBox * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QDoubleSpinBox * editor = itEditor.next();
        if( editor->value() != value )
        {
            editor->blockSignals( true );
            editor->setValue( value );
            editor->blockSignals( false );
        }
    }
}

void QtDoubleSpinBoxFactoryPrivate::slotRangeChanged( QtProperty * property,
        double min, double max )
{
    if( !_createdEditors.contains( property ) )
        return;
        
    QtDoublePropertyManager * manager = q_ptr->propertyManager( property );
    if( !manager )
        return;
        
    QList< QDoubleSpinBox * > editors = _createdEditors[property];
    QListIterator< QDoubleSpinBox * > itEditor( editors );
    while( itEditor.hasNext() )
    {
        QDoubleSpinBox * editor = itEditor.next();
        editor->blockSignals( true );
        editor->setRange( min, max );
        editor->setValue( manager->value( property ) );
        editor->blockSignals( false );
    }
}

void QtDoubleSpinBoxFactoryPrivate::slotSingleStepChanged( QtProperty * property, double step )
{
    if( !_createdEditors.contains( property ) )
        return;
        
    QtDoublePropertyManager * manager = q_ptr->propertyManager( property );
    if( !manager )
        return;
        
    QList< QDoubleSpinBox * > editors = _createdEditors[property];
    QListIterator< QDoubleSpinBox * > itEditor( editors );
    while( itEditor.hasNext() )
    {
        QDoubleSpinBox * editor = itEditor.next();
        editor->blockSignals( true );
        editor->setSingleStep( step );
        editor->blockSignals( false );
    }
}

void QtDoubleSpinBoxFactoryPrivate::slotDecimalsChanged( QtProperty * property, int prec )
{
    if( !_createdEditors.contains( property ) )
        return;
        
    QtDoublePropertyManager * manager = q_ptr->propertyManager( property );
    if( !manager )
        return;
        
    QList< QDoubleSpinBox * > editors = _createdEditors[property];
    QListIterator< QDoubleSpinBox * > itEditor( editors );
    while( itEditor.hasNext() )
    {
        QDoubleSpinBox * editor = itEditor.next();
        editor->blockSignals( true );
        editor->setDecimals( prec );
        editor->setValue( manager->value( property ) );
        editor->blockSignals( false );
    }
}

void QtDoubleSpinBoxFactoryPrivate::slotSetValue( double value )
{
    QObject * object = q_ptr->sender();
    const QMap< QDoubleSpinBox *, QtProperty * >::ConstIterator itcend = _editorToProperty.constEnd();
    for( QMap< QDoubleSpinBox *, QtProperty * >::ConstIterator itEditor = _editorToProperty.constBegin(); itEditor != itcend; ++itEditor )
    {
        if( itEditor.key() == object )
        {
            QtProperty * property = itEditor.value();
            QtDoublePropertyManager * manager = q_ptr->propertyManager( property );
            if( !manager )
                return;
            manager->setValue( property, value );
            return;
        }
    }
}

/*! \class QtDoubleSpinBoxFactory
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtDoubleSpinBoxFactory class provides QDoubleSpinBox
    widgets for properties created by QtDoublePropertyManager objects.

    \sa QtAbstractEditorFactory, QtDoublePropertyManager
*/

/*!
    Creates a factory with the given \a parent.
*/
QtDoubleSpinBoxFactory::QtDoubleSpinBoxFactory( QObject * parent )
    : QtAbstractEditorFactory< QtDoublePropertyManager >( parent ), d_ptr( new QtDoubleSpinBoxFactoryPrivate() )
{
    d_ptr->q_ptr = this;
    
}

/*!
    Destroys this factory, and all the widgets it has created.
*/
QtDoubleSpinBoxFactory::~QtDoubleSpinBoxFactory()
{
    qDeleteAll( d_ptr->_editorToProperty.keys() );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtDoubleSpinBoxFactory::connectPropertyManager( QtDoublePropertyManager * manager )
{
    connect( manager, SIGNAL( valueChanged( QtProperty *, double ) ),
             this, SLOT( slotPropertyChanged( QtProperty *, double ) ) );
    connect( manager, SIGNAL( rangeChanged( QtProperty *, double, double ) ),
             this, SLOT( slotRangeChanged( QtProperty *, double, double ) ) );
    connect( manager, SIGNAL( singleStepChanged( QtProperty *, double ) ),
             this, SLOT( slotSingleStepChanged( QtProperty *, double ) ) );
    connect( manager, SIGNAL( decimalsChanged( QtProperty *, int ) ),
             this, SLOT( slotDecimalsChanged( QtProperty *, int ) ) );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget * QtDoubleSpinBoxFactory::createEditor( QtDoublePropertyManager * manager,
        QtProperty * property, QWidget * parent )
{
    QDoubleSpinBox * editor = d_ptr->createEditor( property, parent );
    editor->setSingleStep( manager->singleStep( property ) );
    editor->setDecimals( manager->decimals( property ) );
    editor->setRange( manager->minimum( property ), manager->maximum( property ) );
    editor->setValue( manager->value( property ) );
    editor->setKeyboardTracking( false );
    
    connect( editor, SIGNAL( valueChanged( double ) ), this, SLOT( slotSetValue( double ) ) );
    connect( editor, SIGNAL( destroyed( QObject * ) ),
             this, SLOT( slotEditorDestroyed( QObject * ) ) );
    return editor;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtDoubleSpinBoxFactory::disconnectPropertyManager( QtDoublePropertyManager * manager )
{
    disconnect( manager, SIGNAL( valueChanged( QtProperty *, double ) ),
                this, SLOT( slotPropertyChanged( QtProperty *, double ) ) );
    disconnect( manager, SIGNAL( rangeChanged( QtProperty *, double, double ) ),
                this, SLOT( slotRangeChanged( QtProperty *, double, double ) ) );
    disconnect( manager, SIGNAL( singleStepChanged( QtProperty *, double ) ),
                this, SLOT( slotSingleStepChanged( QtProperty *, double ) ) );
    disconnect( manager, SIGNAL( decimalsChanged( QtProperty *, int ) ),
                this, SLOT( slotDecimalsChanged( QtProperty *, int ) ) );
}

// QtLineEditFactory

class QtLineEditFactoryPrivate : public EditorFactoryPrivate< QLineEdit >
{
    QtLineEditFactory * q_ptr;
    Q_DECLARE_PUBLIC( QtLineEditFactory )
public:

    void slotPropertyChanged( QtProperty * property, const QString & value );
    void slotRegExpChanged( QtProperty * property, const QRegExp & regExp );
    void slotLengthChanged( QtProperty * property, int length );
    void slotSetValue( const QString & value );
};

void QtLineEditFactoryPrivate::slotPropertyChanged( QtProperty * property,
        const QString & value )
{
    if( !_createdEditors.contains( property ) )
        return;
        
    QListIterator< QLineEdit * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QLineEdit * editor = itEditor.next();
        if( editor->text() != value )
            editor->setText( value );
    }
}

void QtLineEditFactoryPrivate::slotRegExpChanged( QtProperty * property,
        const QRegExp & regExp )
{
    if( !_createdEditors.contains( property ) )
        return;
        
    QtStringPropertyManager * manager = q_ptr->propertyManager( property );
    if( !manager )
        return;
        
    QListIterator< QLineEdit * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QLineEdit * editor = itEditor.next();
        editor->blockSignals( true );
        const QValidator * oldValidator = editor->validator();
        QValidator * newValidator = 0;
        if( regExp.isValid() )
            newValidator = new QRegExpValidator( regExp, editor );
        editor->setValidator( newValidator );
        if( oldValidator )
            delete oldValidator;
        editor->blockSignals( false );
    }
}

void QtLineEditFactoryPrivate::slotLengthChanged( QtProperty * property,
        int length )
{
    if( !_createdEditors.contains( property ) )
        return;
        
    QtStringPropertyManager * manager = q_ptr->propertyManager( property );
    if( !manager )
        return;
        
    QListIterator< QLineEdit * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QLineEdit * editor = itEditor.next();
        editor->blockSignals( true );
        editor->setMaxLength( length );
        editor->blockSignals( false );
    }
}

void QtLineEditFactoryPrivate::slotSetValue( const QString & value )
{
    QObject * object = q_ptr->sender();
    const QMap< QLineEdit *, QtProperty * >::ConstIterator ecend = _editorToProperty.constEnd();
    for( QMap< QLineEdit *, QtProperty * >::ConstIterator itEditor = _editorToProperty.constBegin(); itEditor != ecend; ++itEditor )
        if( itEditor.key() == object )
        {
            QtProperty * property = itEditor.value();
            QtStringPropertyManager * manager = q_ptr->propertyManager( property );
            if( !manager )
                return;
            manager->setValue( property, value );
            return;
        }
}

/*!
    \class QtLineEditFactory
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtLineEditFactory class provides QLineEdit widgets for
    properties created by QtStringPropertyManager objects.

    \sa QtAbstractEditorFactory, QtStringPropertyManager
*/

/*!
    Creates a factory with the given \a parent.
*/
QtLineEditFactory::QtLineEditFactory( QObject * parent )
    : QtAbstractEditorFactory< QtStringPropertyManager >( parent ), d_ptr( new QtLineEditFactoryPrivate() )
{
    d_ptr->q_ptr = this;
    
}

/*!
    Destroys this factory, and all the widgets it has created.
*/
QtLineEditFactory::~QtLineEditFactory()
{
    qDeleteAll( d_ptr->_editorToProperty.keys() );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtLineEditFactory::connectPropertyManager( QtStringPropertyManager * manager )
{
    connect( manager, SIGNAL( valueChanged( QtProperty *, QString ) ),
             this, SLOT( slotPropertyChanged( QtProperty *, QString ) ) );
    connect( manager, SIGNAL( regExpChanged( QtProperty *, QRegExp ) ),
             this, SLOT( slotRegExpChanged( QtProperty *, QRegExp ) ) );
    connect( manager, SIGNAL( lengthChanged( QtProperty *, int ) ),
             this, SLOT( slotLengthChanged( QtProperty *, int ) ) );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget * QtLineEditFactory::createEditor( QtStringPropertyManager * manager,
        QtProperty * property, QWidget * parent )
{

    QLineEdit * editor = d_ptr->createEditor( property, parent );
    QRegExp regExp = manager->regExp( property );
    if( regExp.isValid() )
    {
        QValidator * validator = new QRegExpValidator( regExp, editor );
        editor->setValidator( validator );
    }
    editor->setMaxLength( manager->length( property ) );
    editor->setText( manager->value( property ) );
    
    connect( editor, SIGNAL( textEdited( QString ) ),
             this, SLOT( slotSetValue( QString ) ) );
    connect( editor, SIGNAL( destroyed( QObject * ) ),
             this, SLOT( slotEditorDestroyed( QObject * ) ) );
    return editor;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtLineEditFactory::disconnectPropertyManager( QtStringPropertyManager * manager )
{
    disconnect( manager, SIGNAL( valueChanged( QtProperty *, QString ) ),
                this, SLOT( slotPropertyChanged( QtProperty *, QString ) ) );
    disconnect( manager, SIGNAL( regExpChanged( QtProperty *, QRegExp ) ),
                this, SLOT( slotRegExpChanged( QtProperty *, QRegExp ) ) );
    disconnect( manager, SIGNAL( lengthChanged( QtProperty *, int ) ),
                this, SLOT( slotLengthChanged( QtProperty *, int ) ) );
}

// QtDateEditFactory

class QtDateEditFactoryPrivate : public EditorFactoryPrivate< QDateEdit >
{
    QtDateEditFactory * q_ptr;
    Q_DECLARE_PUBLIC( QtDateEditFactory )
public:

    void slotPropertyChanged( QtProperty * property, const QDate & value );
    void slotRangeChanged( QtProperty * property, const QDate & min, const QDate & max );
    void slotSetValue( const QDate & value );
};

void QtDateEditFactoryPrivate::slotPropertyChanged( QtProperty * property, const QDate & value )
{
    if( !_createdEditors.contains( property ) )
        return;
    QListIterator< QDateEdit * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QDateEdit * editor = itEditor.next();
        editor->blockSignals( true );
        editor->setDate( value );
        editor->blockSignals( false );
    }
}

void QtDateEditFactoryPrivate::slotRangeChanged( QtProperty * property,
        const QDate & min, const QDate & max )
{
    if( !_createdEditors.contains( property ) )
        return;
        
    QtDatePropertyManager * manager = q_ptr->propertyManager( property );
    if( !manager )
        return;
        
    QListIterator< QDateEdit * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QDateEdit * editor = itEditor.next();
        editor->blockSignals( true );
        editor->setDateRange( min, max );
        editor->setDate( manager->value( property ) );
        editor->blockSignals( false );
    }
}

void QtDateEditFactoryPrivate::slotSetValue( const QDate & value )
{
    QObject * object = q_ptr->sender();
    const QMap< QDateEdit *, QtProperty * >::ConstIterator  ecend = _editorToProperty.constEnd();
    for( QMap< QDateEdit *, QtProperty * >::ConstIterator itEditor = _editorToProperty.constBegin(); itEditor != ecend; ++itEditor )
        if( itEditor.key() == object )
        {
            QtProperty * property = itEditor.value();
            QtDatePropertyManager * manager = q_ptr->propertyManager( property );
            if( !manager )
                return;
            manager->setValue( property, value );
            return;
        }
}

/*!
    \class QtDateEditFactory
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtDateEditFactory class provides QDateEdit widgets for
    properties created by QtDatePropertyManager objects.

    \sa QtAbstractEditorFactory, QtDatePropertyManager
*/

/*!
    Creates a factory with the given \a parent.
*/
QtDateEditFactory::QtDateEditFactory( QObject * parent )
    : QtAbstractEditorFactory< QtDatePropertyManager >( parent ), d_ptr( new QtDateEditFactoryPrivate() )
{
    d_ptr->q_ptr = this;
    
}

/*!
    Destroys this factory, and all the widgets it has created.
*/
QtDateEditFactory::~QtDateEditFactory()
{
    qDeleteAll( d_ptr->_editorToProperty.keys() );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtDateEditFactory::connectPropertyManager( QtDatePropertyManager * manager )
{
    connect( manager, SIGNAL( valueChanged( QtProperty *, QDate ) ),
             this, SLOT( slotPropertyChanged( QtProperty *, QDate ) ) );
    connect( manager, SIGNAL( rangeChanged( QtProperty *, QDate, QDate ) ),
             this, SLOT( slotRangeChanged( QtProperty *, QDate, QDate ) ) );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget * QtDateEditFactory::createEditor( QtDatePropertyManager * manager, QtProperty * property,
        QWidget * parent )
{
    QDateEdit * editor = d_ptr->createEditor( property, parent );
    editor->setDisplayFormat( QtPropertyBrowserUtils::dateFormat() );
    editor->setCalendarPopup( true );
    editor->setDateRange( manager->minimum( property ), manager->maximum( property ) );
    editor->setDate( manager->value( property ) );
    
    connect( editor, SIGNAL( dateChanged( QDate ) ),
             this, SLOT( slotSetValue( QDate ) ) );
    connect( editor, SIGNAL( destroyed( QObject * ) ),
             this, SLOT( slotEditorDestroyed( QObject * ) ) );
    return editor;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtDateEditFactory::disconnectPropertyManager( QtDatePropertyManager * manager )
{
    disconnect( manager, SIGNAL( valueChanged( QtProperty *, QDate ) ),
                this, SLOT( slotPropertyChanged( QtProperty *, QDate ) ) );
    disconnect( manager, SIGNAL( rangeChanged( QtProperty *, QDate, QDate ) ),
                this, SLOT( slotRangeChanged( QtProperty *, QDate, QDate ) ) );
}

// QtTimeEditFactory

class QtTimeEditFactoryPrivate : public EditorFactoryPrivate< QTimeEdit >
{
    QtTimeEditFactory * q_ptr;
    Q_DECLARE_PUBLIC( QtTimeEditFactory )
public:

    void slotPropertyChanged( QtProperty * property, const QTime & value );
    void slotSetValue( const QTime & value );
};

void QtTimeEditFactoryPrivate::slotPropertyChanged( QtProperty * property, const QTime & value )
{
    if( !_createdEditors.contains( property ) )
        return;
    QListIterator< QTimeEdit * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QTimeEdit * editor = itEditor.next();
        editor->blockSignals( true );
        editor->setTime( value );
        editor->blockSignals( false );
    }
}

void QtTimeEditFactoryPrivate::slotSetValue( const QTime & value )
{
    QObject * object = q_ptr->sender();
    const  QMap< QTimeEdit *, QtProperty * >::ConstIterator ecend = _editorToProperty.constEnd();
    for( QMap< QTimeEdit *, QtProperty * >::ConstIterator itEditor = _editorToProperty.constBegin(); itEditor != ecend; ++itEditor )
        if( itEditor.key() == object )
        {
            QtProperty * property = itEditor.value();
            QtTimePropertyManager * manager = q_ptr->propertyManager( property );
            if( !manager )
                return;
            manager->setValue( property, value );
            return;
        }
}

/*!
    \class QtTimeEditFactory
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtTimeEditFactory class provides QTimeEdit widgets for
    properties created by QtTimePropertyManager objects.

    \sa QtAbstractEditorFactory, QtTimePropertyManager
*/

/*!
    Creates a factory with the given \a parent.
*/
QtTimeEditFactory::QtTimeEditFactory( QObject * parent )
    : QtAbstractEditorFactory< QtTimePropertyManager >( parent ), d_ptr( new QtTimeEditFactoryPrivate() )
{
    d_ptr->q_ptr = this;
    
}

/*!
    Destroys this factory, and all the widgets it has created.
*/
QtTimeEditFactory::~QtTimeEditFactory()
{
    qDeleteAll( d_ptr->_editorToProperty.keys() );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtTimeEditFactory::connectPropertyManager( QtTimePropertyManager * manager )
{
    connect( manager, SIGNAL( valueChanged( QtProperty *, QTime ) ),
             this, SLOT( slotPropertyChanged( QtProperty *, QTime ) ) );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget * QtTimeEditFactory::createEditor( QtTimePropertyManager * manager, QtProperty * property,
        QWidget * parent )
{
    QTimeEdit * editor = d_ptr->createEditor( property, parent );
    editor->setDisplayFormat( QtPropertyBrowserUtils::timeFormat() );
    editor->setTime( manager->value( property ) );
    
    connect( editor, SIGNAL( timeChanged( QTime ) ),
             this, SLOT( slotSetValue( QTime ) ) );
    connect( editor, SIGNAL( destroyed( QObject * ) ),
             this, SLOT( slotEditorDestroyed( QObject * ) ) );
    return editor;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtTimeEditFactory::disconnectPropertyManager( QtTimePropertyManager * manager )
{
    disconnect( manager, SIGNAL( valueChanged( QtProperty *, QTime ) ),
                this, SLOT( slotPropertyChanged( QtProperty *, QTime ) ) );
}

// QtDateTimeEditFactory

class QtDateTimeEditFactoryPrivate : public EditorFactoryPrivate< QDateTimeEdit >
{
    QtDateTimeEditFactory * q_ptr;
    Q_DECLARE_PUBLIC( QtDateTimeEditFactory )
public:

    void slotPropertyChanged( QtProperty * property, const QDateTime & value );
    void slotSetValue( const QDateTime & value );
    
};

void QtDateTimeEditFactoryPrivate::slotPropertyChanged( QtProperty * property,
        const QDateTime & value )
{
    if( !_createdEditors.contains( property ) )
        return;
        
    QListIterator< QDateTimeEdit * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QDateTimeEdit * editor = itEditor.next();
        editor->blockSignals( true );
        editor->setDateTime( value );
        editor->blockSignals( false );
    }
}

void QtDateTimeEditFactoryPrivate::slotSetValue( const QDateTime & value )
{
    QObject * object = q_ptr->sender();
    const  QMap< QDateTimeEdit *, QtProperty * >::ConstIterator ecend = _editorToProperty.constEnd();
    for( QMap< QDateTimeEdit *, QtProperty * >::ConstIterator itEditor = _editorToProperty.constBegin(); itEditor != ecend;  ++itEditor )
        if( itEditor.key() == object )
        {
            QtProperty * property = itEditor.value();
            QtDateTimePropertyManager * manager = q_ptr->propertyManager( property );
            if( !manager )
                return;
            manager->setValue( property, value );
            return;
        }
}

/*!
    \class QtDateTimeEditFactory
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtDateTimeEditFactory class provides QDateTimeEdit
    widgets for properties created by QtDateTimePropertyManager objects.

    \sa QtAbstractEditorFactory, QtDateTimePropertyManager
*/

/*!
    Creates a factory with the given \a parent.
*/
QtDateTimeEditFactory::QtDateTimeEditFactory( QObject * parent )
    : QtAbstractEditorFactory< QtDateTimePropertyManager >( parent ), d_ptr( new QtDateTimeEditFactoryPrivate() )
{
    d_ptr->q_ptr = this;
    
}

/*!
    Destroys this factory, and all the widgets it has created.
*/
QtDateTimeEditFactory::~QtDateTimeEditFactory()
{
    qDeleteAll( d_ptr->_editorToProperty.keys() );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtDateTimeEditFactory::connectPropertyManager( QtDateTimePropertyManager * manager )
{
    connect( manager, SIGNAL( valueChanged( QtProperty *, QDateTime ) ),
             this, SLOT( slotPropertyChanged( QtProperty *, QDateTime ) ) );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget * QtDateTimeEditFactory::createEditor( QtDateTimePropertyManager * manager,
        QtProperty * property, QWidget * parent )
{
    QDateTimeEdit * editor =  d_ptr->createEditor( property, parent );
    editor->setDisplayFormat( QtPropertyBrowserUtils::dateTimeFormat() );
    editor->setDateTime( manager->value( property ) );
    
    connect( editor, SIGNAL( dateTimeChanged( QDateTime ) ),
             this, SLOT( slotSetValue( QDateTime ) ) );
    connect( editor, SIGNAL( destroyed( QObject * ) ),
             this, SLOT( slotEditorDestroyed( QObject * ) ) );
    return editor;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtDateTimeEditFactory::disconnectPropertyManager( QtDateTimePropertyManager * manager )
{
    disconnect( manager, SIGNAL( valueChanged( QtProperty *, QDateTime ) ),
                this, SLOT( slotPropertyChanged( QtProperty *, QDateTime ) ) );
}

// QtKeySequenceEditorFactory

class QtKeySequenceEditorFactoryPrivate : public EditorFactoryPrivate< QKeySequenceEdit >
{
    QtKeySequenceEditorFactory * q_ptr;
    Q_DECLARE_PUBLIC( QtKeySequenceEditorFactory )
public:

    void slotPropertyChanged( QtProperty * property, const QKeySequence & value );
    void slotSetValue( const QKeySequence & value );
};

void QtKeySequenceEditorFactoryPrivate::slotPropertyChanged( QtProperty * property,
        const QKeySequence & value )
{
    if( !_createdEditors.contains( property ) )
        return;
        
    QListIterator< QKeySequenceEdit * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QKeySequenceEdit * editor = itEditor.next();
        editor->blockSignals( true );
        editor->setKeySequence( value );
        editor->blockSignals( false );
    }
}

void QtKeySequenceEditorFactoryPrivate::slotSetValue( const QKeySequence & value )
{
    QObject * object = q_ptr->sender();
    const  QMap< QKeySequenceEdit *, QtProperty * >::ConstIterator ecend = _editorToProperty.constEnd();
    for( QMap< QKeySequenceEdit *, QtProperty * >::ConstIterator itEditor =  _editorToProperty.constBegin(); itEditor != ecend; ++itEditor )
        if( itEditor.key() == object )
        {
            QtProperty * property = itEditor.value();
            QtKeySequencePropertyManager * manager = q_ptr->propertyManager( property );
            if( !manager )
                return;
            manager->setValue( property, value );
            return;
        }
}

/*!
    \class QtKeySequenceEditorFactory
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtKeySequenceEditorFactory class provides editor
    widgets for properties created by QtKeySequencePropertyManager objects.

    \sa QtAbstractEditorFactory
*/

/*!
    Creates a factory with the given \a parent.
*/
QtKeySequenceEditorFactory::QtKeySequenceEditorFactory( QObject * parent )
    : QtAbstractEditorFactory< QtKeySequencePropertyManager >( parent ), d_ptr( new QtKeySequenceEditorFactoryPrivate() )
{
    d_ptr->q_ptr = this;
    
}

/*!
    Destroys this factory, and all the widgets it has created.
*/
QtKeySequenceEditorFactory::~QtKeySequenceEditorFactory()
{
    qDeleteAll( d_ptr->_editorToProperty.keys() );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtKeySequenceEditorFactory::connectPropertyManager( QtKeySequencePropertyManager * manager )
{
    connect( manager, SIGNAL( valueChanged( QtProperty *, QKeySequence ) ),
             this, SLOT( slotPropertyChanged( QtProperty *, QKeySequence ) ) );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget * QtKeySequenceEditorFactory::createEditor( QtKeySequencePropertyManager * manager,
        QtProperty * property, QWidget * parent )
{
    QKeySequenceEdit * editor = d_ptr->createEditor( property, parent );
    editor->setKeySequence( manager->value( property ) );
    
    connect( editor, SIGNAL( keySequenceChanged( QKeySequence ) ),
             this, SLOT( slotSetValue( QKeySequence ) ) );
    connect( editor, SIGNAL( destroyed( QObject * ) ),
             this, SLOT( slotEditorDestroyed( QObject * ) ) );
    return editor;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtKeySequenceEditorFactory::disconnectPropertyManager( QtKeySequencePropertyManager * manager )
{
    disconnect( manager, SIGNAL( valueChanged( QtProperty *, QKeySequence ) ),
                this, SLOT( slotPropertyChanged( QtProperty *, QKeySequence ) ) );
}

// QtCharEdit

class QtCharEdit : public QWidget
{
    Q_OBJECT
public:
    QtCharEdit( QWidget * parent = 0 );
    
    QChar value() const;
    bool eventFilter( QObject * o, QEvent * e );
public Q_SLOTS:
    void setValue( const QChar & value );
Q_SIGNALS:
    void valueChanged( const QChar & value );
protected:
    void focusInEvent( QFocusEvent * e );
    void focusOutEvent( QFocusEvent * e );
    void keyPressEvent( QKeyEvent * e );
    void keyReleaseEvent( QKeyEvent * e );
    bool event( QEvent * e );
private slots:
    void slotClearChar();
private:
    void handleKeyEvent( QKeyEvent * e );
    
    QChar _value;
    QLineEdit * _lineEdit;
};

QtCharEdit::QtCharEdit( QWidget * parent )
    : QWidget( parent ),  _lineEdit( new QLineEdit( this ) )
{
    QHBoxLayout * layout = new QHBoxLayout( this );
    layout->addWidget( _lineEdit );
    layout->setMargin( 0 );
    _lineEdit->installEventFilter( this );
    _lineEdit->setReadOnly( true );
    _lineEdit->setFocusProxy( this );
    setFocusPolicy( _lineEdit->focusPolicy() );
    setAttribute( Qt::WA_InputMethodEnabled );
}

bool QtCharEdit::eventFilter( QObject * o, QEvent * e )
{
    if( o == _lineEdit && e->type() == QEvent::ContextMenu )
    {
        QContextMenuEvent * c = static_cast< QContextMenuEvent * >( e );
        QMenu * menu = _lineEdit->createStandardContextMenu();
        QList< QAction * > actions = menu->actions();
        QListIterator< QAction * > itAction( actions );
        while( itAction.hasNext() )
        {
            QAction * action = itAction.next();
            action->setShortcut( QKeySequence() );
            QString actionString = action->text();
            const int pos = actionString.lastIndexOf( QLatin1Char( '\t' ) );
            if( pos > 0 )
                actionString = actionString.remove( pos, actionString.length() - pos );
            action->setText( actionString );
        }
        QAction * actionBefore = 0;
        if( actions.count() > 0 )
            actionBefore = actions[0];
        QAction * clearAction = new QAction( tr( "Clear Char" ), menu );
        menu->insertAction( actionBefore, clearAction );
        menu->insertSeparator( actionBefore );
        clearAction->setEnabled( !_value.isNull() );
        connect( clearAction, SIGNAL( triggered() ), this, SLOT( slotClearChar() ) );
        menu->exec( c->globalPos() );
        delete menu;
        e->accept();
        return true;
    }
    
    return QWidget::eventFilter( o, e );
}

void QtCharEdit::slotClearChar()
{
    if( _value.isNull() )
        return;
    setValue( QChar() );
    emit valueChanged( _value );
}

void QtCharEdit::handleKeyEvent( QKeyEvent * e )
{
    const int key = e->key();
    switch( key )
    {
        case Qt::Key_Control:
        case Qt::Key_Shift:
        case Qt::Key_Meta:
        case Qt::Key_Alt:
        case Qt::Key_Super_L:
        case Qt::Key_Return:
            return;
        default:
            break;
    }
    
    const QString text = e->text();
    if( text.count() != 1 )
        return;
        
    const QChar c = text.at( 0 );
    if( !c.isPrint() )
        return;
        
    if( _value == c )
        return;
        
    _value = c;
    const QString str = _value.isNull() ? QString() : QString( _value );
    _lineEdit->setText( str );
    e->accept();
    emit valueChanged( _value );
}

void QtCharEdit::setValue( const QChar & value )
{
    if( value == _value )
        return;
        
    _value = value;
    QString str = value.isNull() ? QString() : QString( value );
    _lineEdit->setText( str );
}

QChar QtCharEdit::value() const
{
    return _value;
}

void QtCharEdit::focusInEvent( QFocusEvent * e )
{
    _lineEdit->event( e );
    _lineEdit->selectAll();
    QWidget::focusInEvent( e );
}

void QtCharEdit::focusOutEvent( QFocusEvent * e )
{
    _lineEdit->event( e );
    QWidget::focusOutEvent( e );
}

void QtCharEdit::keyPressEvent( QKeyEvent * e )
{
    handleKeyEvent( e );
    e->accept();
}

void QtCharEdit::keyReleaseEvent( QKeyEvent * e )
{
    _lineEdit->event( e );
}

bool QtCharEdit::event( QEvent * e )
{
    switch( e->type() )
    {
        case QEvent::Shortcut:
        case QEvent::ShortcutOverride:
        case QEvent::KeyRelease:
            e->accept();
            return true;
        default:
            break;
    }
    return QWidget::event( e );
}

// QtCharEditorFactory

class QtCharEditorFactoryPrivate : public EditorFactoryPrivate< QtCharEdit >
{
    QtCharEditorFactory * q_ptr;
    Q_DECLARE_PUBLIC( QtCharEditorFactory )
public:

    void slotPropertyChanged( QtProperty * property, const QChar & value );
    void slotSetValue( const QChar & value );
    
};

void QtCharEditorFactoryPrivate::slotPropertyChanged( QtProperty * property,
        const QChar & value )
{
    if( !_createdEditors.contains( property ) )
        return;
        
    QListIterator< QtCharEdit * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QtCharEdit * editor = itEditor.next();
        editor->blockSignals( true );
        editor->setValue( value );
        editor->blockSignals( false );
    }
}

void QtCharEditorFactoryPrivate::slotSetValue( const QChar & value )
{
    QObject * object = q_ptr->sender();
    const QMap< QtCharEdit *, QtProperty * >::ConstIterator ecend = _editorToProperty.constEnd();
    for( QMap< QtCharEdit *, QtProperty * >::ConstIterator itEditor = _editorToProperty.constBegin(); itEditor != ecend;  ++itEditor )
        if( itEditor.key() == object )
        {
            QtProperty * property = itEditor.value();
            QtCharPropertyManager * manager = q_ptr->propertyManager( property );
            if( !manager )
                return;
            manager->setValue( property, value );
            return;
        }
}

/*!
    \class QtCharEditorFactory
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtCharEditorFactory class provides editor
    widgets for properties created by QtCharPropertyManager objects.

    \sa QtAbstractEditorFactory
*/

/*!
    Creates a factory with the given \a parent.
*/
QtCharEditorFactory::QtCharEditorFactory( QObject * parent )
    : QtAbstractEditorFactory< QtCharPropertyManager >( parent ), d_ptr( new QtCharEditorFactoryPrivate() )
{
    d_ptr->q_ptr = this;
    
}

/*!
    Destroys this factory, and all the widgets it has created.
*/
QtCharEditorFactory::~QtCharEditorFactory()
{
    qDeleteAll( d_ptr->_editorToProperty.keys() );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtCharEditorFactory::connectPropertyManager( QtCharPropertyManager * manager )
{
    connect( manager, SIGNAL( valueChanged( QtProperty *, QChar ) ),
             this, SLOT( slotPropertyChanged( QtProperty *, QChar ) ) );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget * QtCharEditorFactory::createEditor( QtCharPropertyManager * manager,
        QtProperty * property, QWidget * parent )
{
    QtCharEdit * editor = d_ptr->createEditor( property, parent );
    editor->setValue( manager->value( property ) );
    
    connect( editor, SIGNAL( valueChanged( QChar ) ),
             this, SLOT( slotSetValue( QChar ) ) );
    connect( editor, SIGNAL( destroyed( QObject * ) ),
             this, SLOT( slotEditorDestroyed( QObject * ) ) );
    return editor;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtCharEditorFactory::disconnectPropertyManager( QtCharPropertyManager * manager )
{
    disconnect( manager, SIGNAL( valueChanged( QtProperty *, QChar ) ),
                this, SLOT( slotPropertyChanged( QtProperty *, QChar ) ) );
}

// QtEnumEditorFactory

class QtEnumEditorFactoryPrivate : public EditorFactoryPrivate< QComboBox >
{
    QtEnumEditorFactory * q_ptr;
    Q_DECLARE_PUBLIC( QtEnumEditorFactory )
public:

    void slotPropertyChanged( QtProperty * property, int value );
    void slotEnumNamesChanged( QtProperty * property, const QStringList & );
    void slotEnumIconsChanged( QtProperty * property, const QMap< int, QIcon > & );
    void slotSetValue( int value );
};

void QtEnumEditorFactoryPrivate::slotPropertyChanged( QtProperty * property, int value )
{
    if( !_createdEditors.contains( property ) )
        return;
        
    QListIterator< QComboBox * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QComboBox * editor = itEditor.next();
        editor->blockSignals( true );
        editor->setCurrentIndex( value );
        editor->blockSignals( false );
    }
}

void QtEnumEditorFactoryPrivate::slotEnumNamesChanged( QtProperty * property,
        const QStringList & enumNames )
{
    if( !_createdEditors.contains( property ) )
        return;
        
    QtEnumPropertyManager * manager = q_ptr->propertyManager( property );
    if( !manager )
        return;
        
    QMap< int, QIcon > enumIcons = manager->enumIcons( property );
    
    QListIterator< QComboBox * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QComboBox * editor = itEditor.next();
        editor->blockSignals( true );
        editor->clear();
        editor->addItems( enumNames );
        const int nameCount = enumNames.count();
        for( int i = 0; i < nameCount; i++ )
            editor->setItemIcon( i, enumIcons.value( i ) );
        editor->setCurrentIndex( manager->value( property ) );
        editor->blockSignals( false );
    }
}

void QtEnumEditorFactoryPrivate::slotEnumIconsChanged( QtProperty * property,
        const QMap< int, QIcon > & enumIcons )
{
    if( !_createdEditors.contains( property ) )
        return;
        
    QtEnumPropertyManager * manager = q_ptr->propertyManager( property );
    if( !manager )
        return;
        
    const QStringList enumNames = manager->enumNames( property );
    QListIterator< QComboBox * > itEditor( _createdEditors[property] );
    while( itEditor.hasNext() )
    {
        QComboBox * editor = itEditor.next();
        editor->blockSignals( true );
        const int nameCount = enumNames.count();
        for( int i = 0; i < nameCount; i++ )
            editor->setItemIcon( i, enumIcons.value( i ) );
        editor->setCurrentIndex( manager->value( property ) );
        editor->blockSignals( false );
    }
}

void QtEnumEditorFactoryPrivate::slotSetValue( int value )
{
    QObject * object = q_ptr->sender();
    const  QMap< QComboBox *, QtProperty * >::ConstIterator ecend = _editorToProperty.constEnd();
    for( QMap< QComboBox *, QtProperty * >::ConstIterator itEditor = _editorToProperty.constBegin(); itEditor != ecend; ++itEditor )
        if( itEditor.key() == object )
        {
            QtProperty * property = itEditor.value();
            QtEnumPropertyManager * manager = q_ptr->propertyManager( property );
            if( !manager )
                return;
            manager->setValue( property, value );
            return;
        }
}

/*!
    \class QtEnumEditorFactory
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtEnumEditorFactory class provides QComboBox widgets for
    properties created by QtEnumPropertyManager objects.

    \sa QtAbstractEditorFactory, QtEnumPropertyManager
*/

/*!
    Creates a factory with the given \a parent.
*/
QtEnumEditorFactory::QtEnumEditorFactory( QObject * parent )
    : QtAbstractEditorFactory< QtEnumPropertyManager >( parent ), d_ptr( new QtEnumEditorFactoryPrivate() )
{
    d_ptr->q_ptr = this;
    
}

/*!
    Destroys this factory, and all the widgets it has created.
*/
QtEnumEditorFactory::~QtEnumEditorFactory()
{
    qDeleteAll( d_ptr->_editorToProperty.keys() );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtEnumEditorFactory::connectPropertyManager( QtEnumPropertyManager * manager )
{
    connect( manager, SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotPropertyChanged( QtProperty *, int ) ) );
    connect( manager, SIGNAL( enumNamesChanged( QtProperty *, QStringList ) ),
             this, SLOT( slotEnumNamesChanged( QtProperty *, QStringList ) ) );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget * QtEnumEditorFactory::createEditor( QtEnumPropertyManager * manager, QtProperty * property,
        QWidget * parent )
{
    QComboBox * editor = d_ptr->createEditor( property, parent );
    editor->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Fixed );
    editor->view()->setTextElideMode( Qt::ElideRight );
    QStringList enumNames = manager->enumNames( property );
    editor->addItems( enumNames );
    QMap< int, QIcon > enumIcons = manager->enumIcons( property );
    const int enumNamesCount = enumNames.count();
    for( int i = 0; i < enumNamesCount; i++ )
        editor->setItemIcon( i, enumIcons.value( i ) );
    editor->setCurrentIndex( manager->value( property ) );
    
    connect( editor, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slotSetValue( int ) ) );
    connect( editor, SIGNAL( destroyed( QObject * ) ),
             this, SLOT( slotEditorDestroyed( QObject * ) ) );
    return editor;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtEnumEditorFactory::disconnectPropertyManager( QtEnumPropertyManager * manager )
{
    disconnect( manager, SIGNAL( valueChanged( QtProperty *, int ) ),
                this, SLOT( slotPropertyChanged( QtProperty *, int ) ) );
    disconnect( manager, SIGNAL( enumNamesChanged( QtProperty *, QStringList ) ),
                this, SLOT( slotEnumNamesChanged( QtProperty *, QStringList ) ) );
}

// QtCursorEditorFactory

Q_GLOBAL_STATIC( QtCursorDatabase, cursorDatabase )

class QtCursorEditorFactoryPrivate
{
    QtCursorEditorFactory * q_ptr;
    Q_DECLARE_PUBLIC( QtCursorEditorFactory )
public:
    QtCursorEditorFactoryPrivate();
    
    void slotPropertyChanged( QtProperty * property, const QCursor & cursor );
    void slotEnumChanged( QtProperty * property, int value );
    void slotEditorDestroyed( QObject * object );
    
    QtEnumEditorFactory * _enumEditorFactory;
    QtEnumPropertyManager * _enumPropertyManager;
    
    QMap< QtProperty *, QtProperty * > _propertyToEnum;
    QMap< QtProperty *, QtProperty * > _enumToProperty;
    QMap< QtProperty *, QList< QWidget * > > _enumToEditors;
    QMap< QWidget *, QtProperty * > _editorToEnum;
    bool _updatingEnum;
};

QtCursorEditorFactoryPrivate::QtCursorEditorFactoryPrivate()
    : _updatingEnum( false )
{

}

void QtCursorEditorFactoryPrivate::slotPropertyChanged( QtProperty * property, const QCursor & cursor )
{
    // update enum property
    QtProperty * enumProp = _propertyToEnum.value( property );
    if( !enumProp )
        return;
        
    _updatingEnum = true;
    _enumPropertyManager->setValue( enumProp, cursorDatabase()->cursorToValue( cursor ) );
    _updatingEnum = false;
}

void QtCursorEditorFactoryPrivate::slotEnumChanged( QtProperty * property, int value )
{
    if( _updatingEnum )
        return;
    // update cursor property
    QtProperty * prop = _enumToProperty.value( property );
    if( !prop )
        return;
    QtCursorPropertyManager * cursorManager = q_ptr->propertyManager( prop );
    if( !cursorManager )
        return;
    #ifndef QT_NO_CURSOR
    cursorManager->setValue( prop, QCursor( cursorDatabase()->valueToCursor( value ) ) );
    #endif
}

void QtCursorEditorFactoryPrivate::slotEditorDestroyed( QObject * object )
{
    // remove from _editorToEnum map;
    // remove from _enumToEditors map;
    // if _enumToEditors doesn't contains more editors delete enum property;
    const  QMap< QWidget *, QtProperty * >::ConstIterator ecend = _editorToEnum.constEnd();
    for( QMap< QWidget *, QtProperty * >::ConstIterator itEditor = _editorToEnum.constBegin(); itEditor != ecend; ++itEditor )
        if( itEditor.key() == object )
        {
            QWidget * editor = itEditor.key();
            QtProperty * enumProp = itEditor.value();
            _editorToEnum.remove( editor );
            _enumToEditors[enumProp].removeAll( editor );
            if( _enumToEditors[enumProp].isEmpty() )
            {
                _enumToEditors.remove( enumProp );
                QtProperty * property = _enumToProperty.value( enumProp );
                _enumToProperty.remove( enumProp );
                _propertyToEnum.remove( property );
                delete enumProp;
            }
            return;
        }
}

/*!
    \class QtCursorEditorFactory
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtCursorEditorFactory class provides QComboBox widgets for
    properties created by QtCursorPropertyManager objects.

    \sa QtAbstractEditorFactory, QtCursorPropertyManager
*/

/*!
    Creates a factory with the given \a parent.
*/
QtCursorEditorFactory::QtCursorEditorFactory( QObject * parent )
    : QtAbstractEditorFactory< QtCursorPropertyManager >( parent ), d_ptr( new QtCursorEditorFactoryPrivate() )
{
    d_ptr->q_ptr = this;
    
    d_ptr->_enumEditorFactory = new QtEnumEditorFactory( this );
    d_ptr->_enumPropertyManager = new QtEnumPropertyManager( this );
    connect( d_ptr->_enumPropertyManager, SIGNAL( valueChanged( QtProperty *, int ) ),
             this, SLOT( slotEnumChanged( QtProperty *, int ) ) );
    d_ptr->_enumEditorFactory->addPropertyManager( d_ptr->_enumPropertyManager );
}

/*!
    Destroys this factory, and all the widgets it has created.
*/
QtCursorEditorFactory::~QtCursorEditorFactory()
{
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtCursorEditorFactory::connectPropertyManager( QtCursorPropertyManager * manager )
{
    connect( manager, SIGNAL( valueChanged( QtProperty *, QCursor ) ),
             this, SLOT( slotPropertyChanged( QtProperty *, QCursor ) ) );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget * QtCursorEditorFactory::createEditor( QtCursorPropertyManager * manager, QtProperty * property,
        QWidget * parent )
{
    QtProperty * enumProp = 0;
    if( d_ptr->_propertyToEnum.contains( property ) )
        enumProp = d_ptr->_propertyToEnum[property];
    else
    {
        enumProp = d_ptr->_enumPropertyManager->addProperty( property->propertyName() );
        d_ptr->_enumPropertyManager->setEnumNames( enumProp, cursorDatabase()->cursorShapeNames() );
        d_ptr->_enumPropertyManager->setEnumIcons( enumProp, cursorDatabase()->cursorShapeIcons() );
        #ifndef QT_NO_CURSOR
        d_ptr->_enumPropertyManager->setValue( enumProp, cursorDatabase()->cursorToValue( manager->value( property ) ) );
        #endif
        d_ptr->_propertyToEnum[property] = enumProp;
        d_ptr->_enumToProperty[enumProp] = property;
    }
    QtAbstractEditorFactoryBase * af = d_ptr->_enumEditorFactory;
    QWidget * editor = af->createEditor( enumProp, parent );
    d_ptr->_enumToEditors[enumProp].append( editor );
    d_ptr->_editorToEnum[editor] = enumProp;
    connect( editor, SIGNAL( destroyed( QObject * ) ),
             this, SLOT( slotEditorDestroyed( QObject * ) ) );
    return editor;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtCursorEditorFactory::disconnectPropertyManager( QtCursorPropertyManager * manager )
{
    disconnect( manager, SIGNAL( valueChanged( QtProperty *, QCursor ) ),
                this, SLOT( slotPropertyChanged( QtProperty *, QCursor ) ) );
}

// QtColorEditWidget

class QtColorEditWidget : public QWidget
{
    Q_OBJECT
    
public:
    QtColorEditWidget( QWidget * parent );
    
    bool eventFilter( QObject * obj, QEvent * ev );
    
public Q_SLOTS:
    void setValue( const QColor & value );
    
private Q_SLOTS:
    void buttonClicked();
    
Q_SIGNALS:
    void valueChanged( const QColor & value );
    
private:
    QColor _color;
    QLabel * _pixmapLabel;
    QLabel * _label;
    QToolButton * _button;
};

QtColorEditWidget::QtColorEditWidget( QWidget * parent ) :
    QWidget( parent ),
    _pixmapLabel( new QLabel ),
    _label( new QLabel ),
    _button( new QToolButton )
{
    QHBoxLayout * lt = new QHBoxLayout( this );
    setupTreeViewEditorMargin( lt );
    lt->setSpacing( 0 );
    lt->addWidget( _pixmapLabel );
    lt->addWidget( _label );
    lt->addItem( new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Ignored ) );
    
    _button->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Ignored );
    _button->setFixedWidth( 20 );
    setFocusProxy( _button );
    setFocusPolicy( _button->focusPolicy() );
    _button->setText( tr( "..." ) );
    _button->installEventFilter( this );
    connect( _button, SIGNAL( clicked() ), this, SLOT( buttonClicked() ) );
    lt->addWidget( _button );
    _pixmapLabel->setPixmap( QtPropertyBrowserUtils::brushValuePixmap( QBrush( _color ) ) );
    _label->setText( QtPropertyBrowserUtils::colorValueText( _color ) );
}

void QtColorEditWidget::setValue( const QColor & c )
{
    if( _color != c )
    {
        _color = c;
        _pixmapLabel->setPixmap( QtPropertyBrowserUtils::brushValuePixmap( QBrush( c ) ) );
        _label->setText( QtPropertyBrowserUtils::colorValueText( c ) );
    }
}

void QtColorEditWidget::buttonClicked()
{
    const QColor newColor = QColorDialog::getColor( _color, this, QString(), QColorDialog::ShowAlphaChannel );
    if( newColor.isValid() && newColor != _color )
    {
        setValue( newColor );
        emit valueChanged( _color );
    }
}

bool QtColorEditWidget::eventFilter( QObject * obj, QEvent * ev )
{
    if( obj == _button )
    {
        switch( ev->type() )
        {
            case QEvent::KeyPress:
            case QEvent::KeyRelease:   // Prevent the QToolButton from handling Enter/Escape meant control the delegate
            {
                switch( static_cast< const QKeyEvent * >( ev )->key() )
                {
                    case Qt::Key_Escape:
                    case Qt::Key_Enter:
                    case Qt::Key_Return:
                        ev->ignore();
                        return true;
                    default:
                        break;
                }
            }
            break;
            default:
                break;
        }
    }
    return QWidget::eventFilter( obj, ev );
}

// QtColorEditorFactoryPrivate

class QtColorEditorFactoryPrivate : public EditorFactoryPrivate< QtColorEditWidget >
{
    QtColorEditorFactory * q_ptr;
    Q_DECLARE_PUBLIC( QtColorEditorFactory )
public:

    void slotPropertyChanged( QtProperty * property, const QColor & value );
    void slotSetValue( const QColor & value );
};

void QtColorEditorFactoryPrivate::slotPropertyChanged( QtProperty * property,
        const QColor & value )
{
    const PropertyToEditorListMap::iterator it = _createdEditors.find( property );
    if( it == _createdEditors.end() )
        return;
    QListIterator< QtColorEditWidget * > itEditor( it.value() );
    
    while( itEditor.hasNext() )
        itEditor.next()->setValue( value );
}

void QtColorEditorFactoryPrivate::slotSetValue( const QColor & value )
{
    QObject * object = q_ptr->sender();
    const EditorToPropertyMap::ConstIterator ecend = _editorToProperty.constEnd();
    for( EditorToPropertyMap::ConstIterator itEditor = _editorToProperty.constBegin(); itEditor != ecend; ++itEditor )
        if( itEditor.key() == object )
        {
            QtProperty * property = itEditor.value();
            QtColorPropertyManager * manager = q_ptr->propertyManager( property );
            if( !manager )
                return;
            manager->setValue( property, value );
            return;
        }
}

/*!
    \class QtColorEditorFactory
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtColorEditorFactory class provides color editing  for
    properties created by QtColorPropertyManager objects.

    \sa QtAbstractEditorFactory, QtColorPropertyManager
*/

/*!
    Creates a factory with the given \a parent.
*/
QtColorEditorFactory::QtColorEditorFactory( QObject * parent ) :
    QtAbstractEditorFactory< QtColorPropertyManager >( parent ),
    d_ptr( new QtColorEditorFactoryPrivate() )
{
    d_ptr->q_ptr = this;
}

/*!
    Destroys this factory, and all the widgets it has created.
*/
QtColorEditorFactory::~QtColorEditorFactory()
{
    qDeleteAll( d_ptr->_editorToProperty.keys() );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtColorEditorFactory::connectPropertyManager( QtColorPropertyManager * manager )
{
    connect( manager, SIGNAL( valueChanged( QtProperty *, QColor ) ),
             this, SLOT( slotPropertyChanged( QtProperty *, QColor ) ) );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget * QtColorEditorFactory::createEditor( QtColorPropertyManager * manager,
        QtProperty * property, QWidget * parent )
{
    QtColorEditWidget * editor = d_ptr->createEditor( property, parent );
    editor->setValue( manager->value( property ) );
    connect( editor, SIGNAL( valueChanged( QColor ) ), this, SLOT( slotSetValue( QColor ) ) );
    connect( editor, SIGNAL( destroyed( QObject * ) ), this, SLOT( slotEditorDestroyed( QObject * ) ) );
    return editor;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtColorEditorFactory::disconnectPropertyManager( QtColorPropertyManager * manager )
{
    disconnect( manager, SIGNAL( valueChanged( QtProperty *, QColor ) ), this, SLOT( slotPropertyChanged( QtProperty *, QColor ) ) );
}

// QtFontEditWidget

class QtFontEditWidget : public QWidget
{
    Q_OBJECT
    
public:
    QtFontEditWidget( QWidget * parent );
    
    bool eventFilter( QObject * obj, QEvent * ev );
    
public Q_SLOTS:
    void setValue( const QFont & value );
    
private Q_SLOTS:
    void buttonClicked();
    
Q_SIGNALS:
    void valueChanged( const QFont & value );
    
private:
    QFont _font;
    QLabel * _pixmapLabel;
    QLabel * _label;
    QToolButton * _button;
};

QtFontEditWidget::QtFontEditWidget( QWidget * parent ) :
    QWidget( parent ),
    _pixmapLabel( new QLabel ),
    _label( new QLabel ),
    _button( new QToolButton )
{
    QHBoxLayout * lt = new QHBoxLayout( this );
    setupTreeViewEditorMargin( lt );
    lt->setSpacing( 0 );
    lt->addWidget( _pixmapLabel );
    lt->addWidget( _label );
    lt->addItem( new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Ignored ) );
    
    _button->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Ignored );
    _button->setFixedWidth( 20 );
    setFocusProxy( _button );
    setFocusPolicy( _button->focusPolicy() );
    _button->setText( tr( "..." ) );
    _button->installEventFilter( this );
    connect( _button, SIGNAL( clicked() ), this, SLOT( buttonClicked() ) );
    lt->addWidget( _button );
    _pixmapLabel->setPixmap( QtPropertyBrowserUtils::fontValuePixmap( _font ) );
    _label->setText( QtPropertyBrowserUtils::fontValueText( _font ) );
}

void QtFontEditWidget::setValue( const QFont & f )
{
    if( _font != f )
    {
        _font = f;
        _pixmapLabel->setPixmap( QtPropertyBrowserUtils::fontValuePixmap( f ) );
        _label->setText( QtPropertyBrowserUtils::fontValueText( f ) );
    }
}

void QtFontEditWidget::buttonClicked()
{
    bool ok = false;
    QFont newFont = QFontDialog::getFont( &ok, _font, this, tr( "Select Font" ) );
    if( ok && newFont != _font )
    {
        QFont f = _font;
        // prevent mask for unchanged attributes, don't change other attributes (like kerning, etc...)
        if( _font.family() != newFont.family() )
            f.setFamily( newFont.family() );
        if( _font.pointSize() != newFont.pointSize() )
            f.setPointSize( newFont.pointSize() );
        if( _font.bold() != newFont.bold() )
            f.setBold( newFont.bold() );
        if( _font.italic() != newFont.italic() )
            f.setItalic( newFont.italic() );
        if( _font.underline() != newFont.underline() )
            f.setUnderline( newFont.underline() );
        if( _font.strikeOut() != newFont.strikeOut() )
            f.setStrikeOut( newFont.strikeOut() );
        setValue( f );
        emit valueChanged( _font );
    }
}

bool QtFontEditWidget::eventFilter( QObject * obj, QEvent * ev )
{
    if( obj == _button )
    {
        switch( ev->type() )
        {
            case QEvent::KeyPress:
            case QEvent::KeyRelease:   // Prevent the QToolButton from handling Enter/Escape meant control the delegate
            {
                switch( static_cast< const QKeyEvent * >( ev )->key() )
                {
                    case Qt::Key_Escape:
                    case Qt::Key_Enter:
                    case Qt::Key_Return:
                        ev->ignore();
                        return true;
                    default:
                        break;
                }
            }
            break;
            default:
                break;
        }
    }
    return QWidget::eventFilter( obj, ev );
}

// QtFontEditorFactoryPrivate

class QtFontEditorFactoryPrivate : public EditorFactoryPrivate< QtFontEditWidget >
{
    QtFontEditorFactory * q_ptr;
    Q_DECLARE_PUBLIC( QtFontEditorFactory )
public:

    void slotPropertyChanged( QtProperty * property, const QFont & value );
    void slotSetValue( const QFont & value );
};

void QtFontEditorFactoryPrivate::slotPropertyChanged( QtProperty * property,
        const QFont & value )
{
    const PropertyToEditorListMap::iterator it = _createdEditors.find( property );
    if( it == _createdEditors.end() )
        return;
    QListIterator< QtFontEditWidget * > itEditor( it.value() );
    
    while( itEditor.hasNext() )
        itEditor.next()->setValue( value );
}

void QtFontEditorFactoryPrivate::slotSetValue( const QFont & value )
{
    QObject * object = q_ptr->sender();
    const EditorToPropertyMap::ConstIterator ecend = _editorToProperty.constEnd();
    for( EditorToPropertyMap::ConstIterator itEditor = _editorToProperty.constBegin(); itEditor != ecend; ++itEditor )
        if( itEditor.key() == object )
        {
            QtProperty * property = itEditor.value();
            QtFontPropertyManager * manager = q_ptr->propertyManager( property );
            if( !manager )
                return;
            manager->setValue( property, value );
            return;
        }
}

/*!
    \class QtFontEditorFactory
    \internal
    \inmodule QtDesigner
    \since 4.4

    \brief The QtFontEditorFactory class provides font editing for
    properties created by QtFontPropertyManager objects.

    \sa QtAbstractEditorFactory, QtFontPropertyManager
*/

/*!
    Creates a factory with the given \a parent.
*/
QtFontEditorFactory::QtFontEditorFactory( QObject * parent ) :
    QtAbstractEditorFactory< QtFontPropertyManager >( parent ),
    d_ptr( new QtFontEditorFactoryPrivate() )
{
    d_ptr->q_ptr = this;
}

/*!
    Destroys this factory, and all the widgets it has created.
*/
QtFontEditorFactory::~QtFontEditorFactory()
{
    qDeleteAll( d_ptr->_editorToProperty.keys() );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtFontEditorFactory::connectPropertyManager( QtFontPropertyManager * manager )
{
    connect( manager, SIGNAL( valueChanged( QtProperty *, QFont ) ),
             this, SLOT( slotPropertyChanged( QtProperty *, QFont ) ) );
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget * QtFontEditorFactory::createEditor( QtFontPropertyManager * manager,
        QtProperty * property, QWidget * parent )
{
    QtFontEditWidget * editor = d_ptr->createEditor( property, parent );
    editor->setValue( manager->value( property ) );
    connect( editor, SIGNAL( valueChanged( QFont ) ), this, SLOT( slotSetValue( QFont ) ) );
    connect( editor, SIGNAL( destroyed( QObject * ) ), this, SLOT( slotEditorDestroyed( QObject * ) ) );
    return editor;
}

/*!
    \internal

    Reimplemented from the QtAbstractEditorFactory class.
*/
void QtFontEditorFactory::disconnectPropertyManager( QtFontPropertyManager * manager )
{
    disconnect( manager, SIGNAL( valueChanged( QtProperty *, QFont ) ), this, SLOT( slotPropertyChanged( QtProperty *, QFont ) ) );
}

QT_END_NAMESPACE

#include "moc_qteditorfactory.cpp"
#include "qteditorfactory.moc"
