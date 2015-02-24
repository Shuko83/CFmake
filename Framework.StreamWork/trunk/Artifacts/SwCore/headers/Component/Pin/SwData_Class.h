/*!
 \file SwData_Class.h
 \brief classe representant les données echangées
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef _SwData_Class_H
#define _SwData_Class_H

/*
  * INCLUDES GLOBAUX
  */
#include <QDataStream>
#include "SwException.h"
#include "SwMacros.h"
#include "SwRef.h"
#include "SwRefPtr.h"
#include "SwUUID.h"
/*
  * CONSTANTES GLOBALES
  */
#define CG_SW_DEFAULT_TYPE "None"
#define CG_SW_DEFAULT_TYPE_ID 0

namespace StreamWork
{
	namespace SwCore
	{
        /*! \brief definition du pointeur securisé sur data */
        class SwData_Class;
        class SwPluginFactory_Class;
        typedef SwRefPtr<SwData_Class> SwData_ClassPtr;
        /*!
		 \class SwData_Class 
		 \brief classe de base de toutes les données echangées
        @ingroup SwCoreGrp
		*/
        class BUILD_SWCORE SwData_Class: public SwRef
		{
        private:
            friend class SwPluginFactory_Class;
        public:
            /*! \brief Numero de canal */
            unsigned int Channel;
            /*! \brief priorite */
            int Priority;
            /*! \brief temps de creation */
            double Time;
        protected:
            /*! \brief Destructeur */
            virtual ~SwData_Class();
            //---------------------------------------------------------------------
            // Serialisation interne
            //---------------------------------------------------------------------
            /*! \brief Lecture */
            virtual void InternalRead(QDataStream * data) throw(SwException);
            /*! \brief Ecriture */
            virtual void InternalWrite(QDataStream * data);
            //---------------------------------------------------------------------
            // Creation
            //---------------------------------------------------------------------
            /*! \brief Creation */
            virtual SwData_Class * CreateInstance();
		public:
            /*! \brief Constructeur */
            SwData_Class();
            //---------------------------------------------------------------------
            // Identification
            //---------------------------------------------------------------------
            /*! \brief Identification de la donnée par chaine de caractere
            A surcharger*/
            virtual QString GetType();
            /*! \brief Identification de la donnée par valeur
            A surcharger*/
            virtual SwUUID GetTypeId();
            //---------------------------------------------------------------------
            // Clonage
            //---------------------------------------------------------------------
            /*! \brief Clonage de la donnée */
            SwData_Class * Clone() throw(SwException);
            //---------------------------------------------------------------------
            // Serialisation
            //---------------------------------------------------------------------
            /*! \brief Lecture 
            \warning si les données ne sont pas complete -> exception*/
            void Read(QDataStream * data) throw(SwException);
            /*! \brief Lecture avec resolution 
            \warning si les données ne sont pas complete -> exception*/
            static SwData_Class * ReadResolve(QDataStream * data) throw(SwException);
            /*! \brief Ecriture */
            void Write(QDataStream * data);
		};
	}
}
#endif 
