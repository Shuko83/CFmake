/**
 @file : ISwConfCollector.h
 @brief: Implementation of the Interface ISwConfCollector
 @date : 14/06/2013 16:04:34
 @author : CGD
*/

#ifndef _ISwConfCollector_H
#define _ISwConfCollector_H


#include <QtGlobal>
#include "Properties/ISwProperty.h"

namespace StreamWork
{
	namespace SwCore
	{
        /**
		* @brief : interface ISwConfCollector 
		* Interface definissant le composant qui permet de collecter 
		* les ISwProperty des streams
        @ingroup SwCoreGrp
        @swinterface

        Cette interface doit être implementée par les composants ConfCollectors.
		L'interface doit être enregistrée auprès du service SwSaveConfigurationService 
		pour que l'IHM puisse accéder aux pointeurs des Properties de configuration. 	

        4 methodes doivent être définies:
		- GetConfName() qui permet de savoir à quelle configuration le collector sert.
		- GetPrefix() qui permet de savoir le sous groupe de paramètres concernés 
		  par les Properties du collector.
		- GetProperties() qui permet de retourner toutes les Property collectées dans une QHash.
		- GetProperty(QString) qui permet de retourner le pointeur vers une Property 
		  en connaissant son nom.
		*/

		class ISwConfCollector
		{     
		public:
            /** 
             * @brief Renvoie le nom du configurationCollector
             * @return QString : nom du configurationCollector 
			 */
			virtual QString getConfName() =0;

			/** 
			 * @brief Renvoie le nom du prefix du configurationCollector
			 * @return QString : nom du prefix 
			 */
			virtual QString getPrefix() =0;

            /** 
             * @brief Renvoie les Properties collectée par le configurationCollector
             * @return QHash<QString, ISwProperty> : QHash<nom décoré de la ISwProperty, pointeur sur la ISwProperty>
             */
			virtual QHash<QString, ISwProperty*> getProperties() =0;

			/** 
             * @brief Renvoie l'ordre des Properties collectée par le configurationCollector
             * @return QHash<QString, int> : QHash<nom décoré de la ISwProperty, order de la property>
             */
			virtual QHash<QString, int> getPropertiesOrder() =0;

			/** 
			 * @brief Renvoie le pointeur vers l'ISwProperty avant le nom passé en paramètre
			 * @return le pointeur vers l'ISwProperty si elle a été trouvée dans le ConfCollector
			 */
			virtual ISwProperty* getProperty(QString decoratedPropertyName) =0;

			/** 
			 * @brief Renvoie le nom de la property dont le pointeur est passé en paramètre
			 * @return le nom de la property avec l'arborescence
			 */
			virtual QString getConstructedPropertyName(ISwProperty* inProperty ) = 0;

			/** 
			 * @brief Ajoute une property dans la liste du confCollector
			 * @param QString : nom décodé de la property
			 * @param ISwProperty* pointeur sur la property
			 */
			virtual bool addExternalProperty(QString decoratedName, ISwProperty* property ) = 0;
		};
	}
}
#endif 
