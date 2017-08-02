/*!
\file SwMacros.h
\date 28/04/2006
\brief Constantes, types de base et macro
\author  Big
\version 1.0
 */

#ifndef _SwMacros_H
#define _SwMacros_H


#include <iostream>
#include <vector>
#include <string>


#ifdef __cplusplus
#define SW_EXTERN_C    extern "C"
#else
#define SW_EXTERN_C    extern
#endif

/*!
  * MACRO POUR GENERATION FACILE D'UNE EXCEPTION
  */
#define LAUNCH_SWEXCEPTION(component,reason) { SwException _owner(QString(component),QString(__FILE__), __LINE__,QString(reason));throw(_owner);}

/*!
  * MACRO POUR ACCEDER A L'INSTANCE STREAMWORK
  */
#define SW_APP SwApplication::GetInstance()
/*!
  * MACRO POUR ACCEDER A L'INTERFACE DE LOG
  */
#define SW_LOG SwApplication::GetInstance()->Logger()
#define SW_ALERT SwApplication::GetInstance()->Alerter()
#define SW_DEBUG(text) SwApplication::GetInstance()->Logger().Log(LogLvl_Debug,text)
/*!
  * MACRO POUR ACCEDER AUX FABRIQUES DE COMPOSANTS ET DE DATA
  */
#define SW_FACTORIES SwApplication::GetInstance()->ComponentsBank()
/*!
  * MACRO POUR ACCEDER AUX FABRIQUES D'ADAPTEURS DE TYPE COMPLEXES
  */
#define SW_CTA_FACTORIES SwApplication::GetInstance()->CTFactoriesBank()
/*!
  * MACRO POUR CREE UNE DONNEE A PARTIR DE SON TYPE ID
  */
#define SW_CREATE_DATA(id_type) SwApplication::GetInstance()->ComponentsBank().CreateData(id_type)
/*!
  * MACRO POUR PUBLIER UN COMPOSANT VIA SON PLUGIN USINE a utiliser dans la methode surcharger CreateInstanceOf
  */
#define SW_PUBLISH_COMPONENT(name_of_component,class_name) { \
    if (name_of_component==name) { \
        class_name * new_comp=new class_name; \
        return new_comp; \
    } \
} \
/*!
  * MACRO DE GESTION DES FABRIQUES D'OBJET
  */
#define SW_REGISTER_OBJECT_FACTORY(id,factory) SwApplication::GetInstance()->ComponentsBank().RegisterObjectFactory(id,factory);
#define SW_UNREGISTER_OBJECT_FACTORY(id) SwApplication::GetInstance()->ComponentsBank().UnregisterObjectFactory(id);
#define SW_CREATE_OBJECT_FACTORY(id,param) SwApplication::GetInstance()->ComponentsBank().CreateObject(id,param);



/*!
 * UUID STREAMWORK
  Q_DECL_IMPORT const StreamWork::SwCore::SwUUID name; \
*/
#define SW_DEFINE_UUID(name,mostSigBits,leastSigBits) SW_EXTERN_C Q_DECL_EXPORT const StreamWork::SwCore::SwUUID name = SwUUID(Q_INT64_C(mostSigBits),Q_INT64_C(leastSigBits));
#define SW_DECLARE_UUID(name) SW_EXTERN_C Q_DECL_IMPORT const StreamWork::SwCore::SwUUID name;
#define SW_DECLARE_UUID_LOCAL(name) SW_EXTERN_C Q_DECL_EXPORT const StreamWork::SwCore::SwUUID name;


#endif

