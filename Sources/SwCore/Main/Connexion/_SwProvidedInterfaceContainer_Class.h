/*!
\file _SwProvidedInterfaceContainer_Class.h
\date 12/04/2006
\brief conteneur de stockage des informations relatives a une interface fournie
\author  Big
\version 1.0
 */

#ifndef __SwProvidedInterfaceContainer_Class_H
#define __SwProvidedInterfaceContainer_Class_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
#include <QList>
/*
  * INCLUDES LOCAUX
 */
#include "SwCoreConstantes.h"
#include "Tools/Exception/SwException.h"
#include "Main/SwMacros.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Provider_Class.h"

namespace StreamWork 
{
    namespace SwCore
    {
        /*!
        \class _SwProvidedInterfaceContainer_Class
        \brief conteneur de stockage des informations relatives a une interface fournie
        */
        class _SwProvidedInterfaceContainer_Class {
        private:
            /*!
            \class __KeyConsumption
            \brief entite de stockage d'une pair consommateur, interface
            */
            class __KeyConsumption {
            public:
                ISwInterfaces_Consumer * _consumer;
                QString _cinterface;
	            /*! \brief Construsteur */
                __KeyConsumption() {_consumer=NULL;_cinterface=QString();}
	            /*! \brief Construsteur parametre*/
	            __KeyConsumption(ISwInterfaces_Consumer * consumer,QString cinterface){_consumer=consumer;_cinterface=cinterface;}
	            /*! \brief Construsteur de copie*/
                __KeyConsumption(const __KeyConsumption & source) {
                    if (&source!=this) {
                        _consumer=source._consumer;    
                        _cinterface=source._cinterface;    
                    }
                }
	            /*! \brief Destrusteur */
                ~__KeyConsumption() {}
                /*! \brief Operateur d'affectation */
                __KeyConsumption & operator=(const __KeyConsumption& source) {
                    if (&source!=this) {
                        _consumer=source._consumer;    
                        _cinterface=source._cinterface;    
                    }
                    return *this;
                }
                /*! \brief Operateur d'egalite*/
                bool operator==(const __KeyConsumption& val) const {
                    if (&val==this) return true;
                    if (_consumer==val._consumer && _cinterface==val._cinterface) return true;
                    return false;
                }
                /*! \brief Operateur de comparaison */
                bool operator<(const __KeyConsumption& val) const {
                    if (&val==this) return false;
                    if (_consumer!=val._consumer) return _consumer<val._consumer;
                    return _cinterface<val._cinterface;
                }
            };
            /*! \brief Nom de l'interface */
            QString _name;
            /*! \brief Type de l'interface */
            QString _type;
            /*! \brief flag indicateur que l'interface est disponible */
            bool _isAvailable;
            /*! \brief Handle de l'interface */
            void * _handle;
            /*! \brief Liste des consommateurs*/
            QList<__KeyConsumption> _consumers;
            /*! \brief clef de consommations courante*/
            QList<__KeyConsumption>::iterator _current_consumersIt;
            /*! \brief fournisseur parent */
            SwInterfaces_Provider_Class * _parent;
        public:
	        /*! \brief Construsteur */
	        _SwProvidedInterfaceContainer_Class(SwInterfaces_Provider_Class * parent);
	        /*! \brief Destrusteur */
	        ~_SwProvidedInterfaceContainer_Class();
	        /*! \brief Definit les caracteristiques de l'interfaces */
	        void Define(QString name,QString type);
	        /*! \brief Acces au type de l'interface */
	        QString GetType();
	        /*! \brief Definition de l'interface */
	        void SetInterface(void * handle);
	        /*! \brief Changer la disponibilite */
	        void ChangeAvailability(bool is_available);
	        /*! \brief Revoie la disponibilite */
	        bool IsAvailable();
	        /*! \brief Enregistre une interface consommatrice */
            void RegisterConsumedInterface(ISwInterfaces_Consumer * consumer,QString cinterface) throw(SwException);
	        /*! \brief Desenregistre une interface consommatrice */
            void UnregisterConsumedInterface(ISwInterfaces_Consumer * consumer,QString cinterface);
            /*! \brief Acces a la premier consommateur et son nom */
            ISwInterfaces_Consumer * GetFirstConsumer(QString * pt_cinterfacename);
            /*! \brief Acces a la consommateur suivant et son nom */
            ISwInterfaces_Consumer * GetNextConsumer(QString * pt_cinterfacename);
        };
    }
}

#endif
