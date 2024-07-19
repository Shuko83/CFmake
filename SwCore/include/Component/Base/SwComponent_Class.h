/*!
 \file SwComponent_Class.h
 \brief Implementation of the Class SwComponent_Class
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef _SwComponent_Class_H
#define _SwComponent_Class_H

/*
  * INCLUDES LOCAUX
  */
#include "Component/Base/SwNamed_Class.h"
#include "SwRef/SwRef.h"
#include "SwRef/SwRefPtr.h"
#include "Main/Services/Management/SwServicesManager_Class.h"
#include "Component/Interfaces/ISwVisitor.h"
#include "Component/Interfaces/ISwActivable.h"
/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
#include <QObject>

namespace StreamWork
{
	namespace SwCore
	{
        /*! \brief definition du pointeur securisé sur composant */
		class SwComponent_Class;
        class SwPluginFactory_Class;
        typedef SwRefPtr<SwComponent_Class> SwComponent_ClassPtr;
        /*!
		 \class SwComponent_Class 
		 \brief Classe de base de tous les composants
        @ingroup SwCoreGrp
		*/
		class BUILD_SWCORE SwComponent_Class : 
            public QObject,
            public SwNamed_Class, 
            public SwRef, 
            public SwServicesManager_Class,
            virtual public SwCore::ISwActivable 
		{
            Q_OBJECT
        private:
            friend class SwPluginFactory_Class;
            /*! \brief parent */
            SwComponent_Class * _parent;
            /*! \brief activation */
            bool _active;
            /*! \brief nom usine du composant (pour retrouver la façon de le fabriquer) */
			QString _factory_name;
            QString _factory_component_name;
            /*! \brief QMap des enfants */
            QMap<QString,SwComponent_ClassPtr> _child_components;
            /*! \brief iterateur de parcours sur la QMap des enfants */
            QMap<QString,SwComponent_ClassPtr>::iterator _current_child;
            /*! \brief definition du parent */
            void SetParent(SwComponent_Class * parent);
			/*! \brief definition du nom usine du composant */
			void SetFactoryName(QString factory_name);
            void SetFactoryComponentName(QString factory_component_name);
        protected:
			/*! \brief Log du temps*/
			bool _logTime;
            /*! \brief Destructeur */
            virtual ~SwComponent_Class();
		public:
            /*! \brief Constructeur */
            SwComponent_Class();


            /*! \brief acces nom usine du composant*/
			QString GetFactoryName();
			QString GetFactoryComponentName();

            /*! \brief Initialisation des ressources
            \note tous les services du composants doivent être déclarés dans cette methodes*/
            virtual void InitializeResources() throw(SwException);

            /*! \brief Acces au parent du composant */
			virtual SwComponent_Class * GetParent() const;
			/*! \brief Ajout d'un enfant */
			virtual void AddChild(SwComponent_Class * child)  throw(SwException);
			/*! \brief Retrait d'un enfant */
			virtual void RemoveChild(SwComponent_Class * child)  throw(SwException);

            /*! \brief Acces au nombre d'enfants */
			virtual unsigned long GetNbChildren() const;
            /*! \brief Acces a un enfant par son nom */
			virtual SwComponent_Class * GetChild(QString child_name)  const;
            /*! \brief Acces a un enfant par son index */
			virtual SwComponent_Class * GetChild(unsigned long index)  const;
            /*! \brief Renvoie l'index d'un enfant defnit par son handle */
			virtual int GetChildIndex(SwComponent_Class * comp_child)  const;
			/*! \brief Acces au premier enfant */
            virtual SwComponent_Class * GetFirstChild();
            /*! \brief Acces a l'enfant suivant */
			virtual SwComponent_Class * GetNextChild();

            /*! \brief acces visiteur */
            void AcceptVisitor(ISwVisitor * visitor);

            /*! \brief Permet de suggerer un nom pour un futur enfant*/
			QString GetSuggestedNameForChild(QString initial_name);

            /*! \brief getter active */
            virtual bool isActive();
            /*! \brief setter active */
            virtual void setActive(bool active);


        public:
            /*! \brief Permet de signaler un changement du parent*/
	        LibIndeSig::iSignal0 OnParentChange;
            /*! \brief Permet de signaler l'evenement avant l'ajout d'enfant (parent,enfant)*/
	        LibIndeSig::iSignal2<SwComponent_Class *,SwComponent_Class *> OnBeforeAddChild;
            /*! \brief Permet de signaler un ajout d'enfant (parent,enfant)*/
	        LibIndeSig::iSignal2<SwComponent_Class *,SwComponent_Class *> OnAddChild;
            /*! \brief Permet de signaler un retrait d'enfant (parent,enfant)*/
	        LibIndeSig::iSignal2<SwComponent_Class *,SwComponent_Class *> OnRemoveChild;
            /*! \brief Permet de signaler l'evenement apres retrait d'enfant (parent)*/
	        LibIndeSig::iSignal1<SwComponent_Class *> OnAfterRemoveChild;
			/*! \brief Permet de signaler un changement de nom de l'instance*/
			LibIndeSig::iSignal1<SwComponent_Class *> OnChangeComponentName;
        public:
            //------------------------------------------------
            //Surcharge de la classe mère SwNamed_Class
            //------------------------------------------------
            /*! \brief Permet de definir le nom de l'instance nommé (Surcharge)*/
			virtual void SetName(const QString & new_name) throw(SwException);
		};
	}
}
#endif 
