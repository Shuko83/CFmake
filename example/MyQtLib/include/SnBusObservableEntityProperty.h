#ifndef SNBUSOBSERVABLEENTITYPROPERTY_H
#define SNBUSOBSERVABLEENTITYPROPERTY_H

#include "qtvariantproperty.h"

namespace sunset {

    namespace widgets {
    
        class SN_WIDGETS_EXPORT SnBusObservableEntityPropertyManager final : public QtVariantPropertyManager
        {
            Q_OBJECT
        public:
            SnBusObservableEntityPropertyManager( QObject * parent = 0 );
            ~SnBusObservableEntityPropertyManager();
            
            QString prefix( const QtProperty * property ) const;
            QString suffix( const QtProperty * property ) const;
            
            virtual int valueType( int propertyType ) const;
            virtual bool isPropertyTypeSupported( int propertyType ) const;
            
            static int entityTypeId();
            static int arrayTypeId();
            static int exchangeTypeId();
        signals:
            void prefixChanged( QtProperty * property, const QString & prefix );
            void suffixChanged( QtProperty * property, const QString & suffix );
        public slots:
            void setPrefix( QtProperty * property, const QString & prefix );
            void setSuffix( QtProperty * property, const QString & suffix );
            virtual void setValue( QtProperty * property, const QVariant & val );
            virtual void setValueText( const QtProperty * property, QString valueText );
            
        protected:
            QString valueText( const QtProperty * property ) const;
            virtual bool hasValue( const QtProperty * property ) const;
            virtual void initializeProperty( QtProperty * property );
            virtual void uninitializeProperty( QtProperty * property );
        private:
            QMap< const QtProperty *, QString > _propertyToValueText;
            struct PrefixSuffixData
            {
                QString prefix;
                QString suffix;
            };
            QMap< const QtProperty *, PrefixSuffixData > _propertyToData;
        };
        
        class SN_WIDGETS_EXPORT SnBusObservableEntityPropertyFactory : public QtAbstractEditorFactory< SnBusObservableEntityPropertyManager >
        {
            Q_OBJECT
        public:
            SnBusObservableEntityPropertyFactory( QObject * parent = 0 );
            ~SnBusObservableEntityPropertyFactory();
        protected:
            void connectPropertyManager( SnBusObservableEntityPropertyManager * manager );
            QWidget * createEditor( SnBusObservableEntityPropertyManager * manager, QtProperty * property,
                                    QWidget * parent );
            void disconnectPropertyManager( SnBusObservableEntityPropertyManager * manager );
        private slots:
        
            void slotPrefixChanged( QtProperty * property, const QString & prefix );
            void slotSuffixChanged( QtProperty * property, const QString & suffix );
            void slotEditorDestroyed( QObject * object );
        private:
            QtVariantEditorFactory * _originalFactory;
            QMap< QtProperty *, QList< QWidget * > > _createdEditors;
            QMap< QWidget *, QtProperty * > _editorToProperty;
        };
        
    }
    
}
#endif
