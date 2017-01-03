/*!
  \file SwMacros.h
  \date 28/04/2006
  \brief Constantes, types de base et macro
  \author  Big
  \version 1.0
 */

#ifndef _SwMacros_H
#define _SwMacros_H

#include <QtCore/QtGlobal>
#include <SwApplication.h>

#ifdef __cplusplus
#  define SW_EXTERN_C    extern "C"
#else
#  define SW_EXTERN_C    extern
#endif

/*!
 * MACRO POUR GENERATION FACILE D'UNE EXCEPTION
 */
#define LAUNCH_SWEXCEPTION(component,reason) { StreamWork::SwCore::SwException _owner(QString(component),QString(__FILE__), __LINE__,QString(reason)); throw(_owner); }

/*!
 * MACRO POUR ACCEDER A L'INSTANCE STREAMWORK
 */
#define SW_APP StreamWork::SwCore::SwApplication::GetInstance()

/*!
 * MACROS POUR ACCEDER A L'INTERFACE DE LOG
 */
#define SW_LOG StreamWork::SwCore::SwApplication::GetInstance()->Logger()
#define SW_ALERT StreamWork::SwCore::SwApplication::GetInstance()->Alerter()
#define SW_DEBUG(text) StreamWork::SwCore::SwApplication::GetInstance()->Logger().Log(StreamWork::SwCore::LogLvl_Debug,text)

/*!
 * MACRO POUR ACCEDER AUX FABRIQUES DE COMPOSANTS ET DE DATA
 */
#define SW_FACTORIES StreamWork::SwCore::SwApplication::GetInstance()->ComponentsBank()

/*!
 * MACRO POUR ACCEDER AUX FABRIQUES D'ADAPTEURS DE TYPE COMPLEXES
 */
#define SW_CTA_FACTORIES StreamWork::SwCore::SwApplication::GetInstance()->CTFactoriesBank()

/*!
 * MACRO POUR CREE UNE DONNEE A PARTIR DE SON TYPE ID
 */
#define SW_CREATE_DATA(id_type) StreamWork::SwCore::SwApplication::GetInstance()->ComponentsBank().CreateData(id_type)

/*!
 * MACRO POUR PUBLIER UN COMPOSANT VIA SON PLUGIN USINE a utiliser dans la methode surcharger CreateInstanceOf
 */
#define SW_PUBLISH_COMPONENT(name_of_component,class_name) if (name_of_component == name) { return new class_name; } 
#define SW_PUBLISH_ICON(name_of_component,qrcRessource) if (name_of_component == name) { return QIcon(qrcRessource); } 

/*!
 * MACRO DE GESTION DES FABRIQUES D'OBJET
 */
#define SW_REGISTER_OBJECT_FACTORY(id,factory) StreamWork::SwCore::SwApplication::GetInstance()->ComponentsBank().RegisterObjectFactory(id,factory);
#define SW_UNREGISTER_OBJECT_FACTORY(id) StreamWork::SwCore::SwApplication::GetInstance()->ComponentsBank().UnregisterObjectFactory(id);
#define SW_CREATE_OBJECT_FACTORY(id,param) StreamWork::SwCore::SwApplication::GetInstance()->ComponentsBank().CreateObject(id,param);

/*!
 * UUID STREAMWORK
 */
#define SW_DEFINE_UUID(name,mostSigBits,leastSigBits) SW_EXTERN_C Q_DECL_EXPORT const StreamWork::SwCore::SwUUID name = SwUUID(Q_INT64_C(mostSigBits),Q_INT64_C(leastSigBits));
#define SW_DECLARE_UUID(name) SW_EXTERN_C Q_DECL_IMPORT const StreamWork::SwCore::SwUUID name;
#define SW_DECLARE_UUID_LOCAL(name) SW_EXTERN_C Q_DECL_EXPORT const StreamWork::SwCore::SwUUID name;

#endif
