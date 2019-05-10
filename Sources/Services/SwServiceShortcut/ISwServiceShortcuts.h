/*!
\file ISwServiceShortcuts.h
\date 12/04/2006
\brief service de mapping de shortcuts
\author  AAY
\version 1.0
 */

#ifndef _ISWSERVICESHORTCUTS_H
#define _ISWSERVICESHORTCUTS_H
/*
 * INCLUDES GLOBAUX
 */
#include <QList>
#include <QString>
#include <QMap>
#include <QObject>
#include <QKeySequence>
#include <QDebug>

/*
 * INCLUDES LOCAUX
 */
#include "ISwService.h"
#include "ISwShortcut.h"
#include "ISwDevice.h"

using namespace StreamWork::SwCore;

//Nom du service
#define CG_SW_SERVICE_SHORTCUTS "ServiceShortcuts"
#define SC_SEP "¤¤SEP¤¤"

namespace StreamWork
{
    namespace SwCore
    {
		class ISwShortcutObserver;

        /*!
        \interface ISwServiceShortcuts
        \brief service de mapping de shortcuts
        @ingroup SwGuiGrp
        @swinterface
        */
        class ISwServiceShortcuts: virtual public ISwService{
        public:

			/**
			 * @brief    : Permet d'enregistré une commande dans le service
			 * @param	 : QString command - Nom de la commande
			 * @param	 : ISwShortcut * obj - Pointeur sur l'objet
			 */
			virtual void registerCommand(QString category, QString command, ISwShortcut *obj) = 0;

			/**
			 * @brief    : Permet de desenregistré une commande dans le service
			 * @param	 : QString command - Nom de la commande
			 */
			virtual void unregisterCommand(QString category, QString command, ISwShortcut *obj) = 0;

			/**
			 * @brief    : Permet d'enregistrer un périphérique 
			 * @param	 : ISwDevice * device - pointeur sur le périphérique
			 */
			virtual void registerDevice(ISwDevice * device) = 0;

			/**
			 * @brief    : Permet de désenregistrer un périphérique 
			 * @param	 : ISwDevice * device - pointeur sur le périphérique
			 */
			virtual void unregisterDevice(ISwDevice * device) = 0;

			/**
			 * @brief    : Permet d'enregister un observer de nouvelle command
			 * @param	 : ISwShortcutObserver * obs - pointeur sur l'observeur
			 */
			virtual void registerShortcutObserver(ISwShortcutObserver * obs) = 0;

			/**
			 * @brief    : Permet de désenregistrer un observer de command
			 * @param	 : ISwShortcutObserver * obs - pointeur sur l'observeur
			 */
			virtual void unregisterShortcutObserver(ISwShortcutObserver * obs) = 0;

			/**
			 * @brief    : Bind un SwDevice sur un SwShortcut
			 * @param	 : QString device - DeviceName + SC_SEP + Device entry 
			 * @param	 : QString shortcut - ShortcutName + SC_SEP + CommandName 
			 */
			virtual void bind(QString swdevice, QString shortcut) = 0;

			/**
			 * @brief    : Unbind all shortcut
			 */
			virtual void unBindAll() = 0;

			/**
			 * @brief    : Unbind designed Devices shortcuts
			 */
			virtual void unBindDevices(QMap<QString,QList<QString>> devices) = 0;

			/**
			 * @brief    : Unbind designed Shortcuts shortcuts
			 */
			virtual void unBindShortcuts(QMap<QString,QList<QString>> shortcuts) = 0;

			/**
			 * @brief    : clear shortcuts service
			 */
			virtual void clearShortcutsService() = 0;

			/**
			 * @brief    : Bind une combinaison de touche sur un SwShortcut
			 * @param	 : QString sequence - Sequence de touches
			 * @param	 : QString shortcut - ShortcutName + SC_SEP + CommandName 
			 * @param	 : QWidget * - Pointeur sur le widget ou seront binder les raccourcis
			 */
			virtual void bindKeyboard(QString sequence, QString shortcut,QWidget * context) = 0;

			/**
			 * @brief    : Retourne la list des Devices disponibles
			 * @return   : QList<ISwDevice*> - List des Devices connectés
			 */
			virtual QList<ISwDevice*> getDevices() = 0;

			/**
			 * @brief    : Retourne la map des bind des devices
			 * @return   : QMap<QString,QList<QString>> - QMap<Shortcut.getname+SC_SEP+commandName,QList<Device.getName()+SC_SEP+entryName>>
			 */
			virtual QMap<QString,QList<QString>> getBindedDevices() = 0;


			/**
			 * @brief    : Retourne la map des bind des keyboards
			 * @return   : QMap<QString,QList<QString>> QMap<Shortcut.getname+SC_SEP+commandName,QList<Sequence.toString()>>
			 */
			virtual QMap<QString,QList<QString>> getBindedKeyboard() = 0;

			/**
			 * @brief    : Permet de configurer le service pour lui dire qu'on est en 
			 *			   mode de configuration pour désactiver les notifications 
			 *			   pendant le temps de la configuration
			 * @param	 : bool value - true si on est mode de configuration
			 */
			virtual void setConfigMode(bool value) = 0;

			/**
			 * @brief    : Retourne le texte avant le SC_SEP
			 * @return   : QString - Retourne Name
			 * @param	 : QString val - Une combinaison de Name + SC_SEP + commandName
			 */
			virtual QString getCategory(QString val) = 0;

			/**
			 * @brief    : Retourne le texte après le SC_SEP
			 * @return   : QString - Retourne commandName
			 * @param	 : QString val - Une combinaison de Name + SC_SEP + commandName
			 */
			virtual QString getShortcutName(QString val) = 0;

			/**
			 * @brief    : Genere un nom "prefix+SC_SEP+suffix)
			 * @return   : QString - le nom combiné
			 * @param	 : QString prefix - le prefix
			 * @param	 : QString suffix - le suffix
			 */
			virtual QString generateCombinedName(QString prefix,QString suffix) = 0; 


			//---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
			/**
			 * @brief    : Est appele uniquement par le service manager aupres duquel le service est enregistré
						   lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée
			 */
			virtual void Liberate(){}; 
            /**
             * @brief    : Renvoie le nom du service
             * @return   : le nom du service 
             */
            virtual QString GetServiceName(){return QString(CG_SW_SERVICE_SHORTCUTS); }

             /**
             * @brief    : Renvoie le nom du service reel (le nom de l'interface)
			 * @return   : le nom du service reel (le nom de l'interface)
			 * @note     : au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()
             */
            virtual QString GetServiceRealName() {return QString(typeid(*this).name());}
        };
    }
}

#endif
