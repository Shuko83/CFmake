/*!
\file _SWServiceShortcuts.h
\date 12/04/2006
\brief service de mapping de shortcuts
\author  AAY
\version 1.0
 */

#ifndef _SWSERVICESHORTCUTS_H
#define _SWSERVICESHORTCUTS_H
/*
  * INCLUDES GLOBAUX
 */
/*
  * INCLUDES LOCAUX
 */
#include "ISwServiceShortcuts.h"
#include "ISwDeviceObserver.h"
#include <QObject>
#include <QWidget>



#define _MAX_SHORTCUT_BY_COMMAND 2

namespace StreamWork
{
    namespace SwGui
    {
        /*!
        \interface _SWServiceShortcuts
        \brief service de mapping de shortcuts
        @ingroup SwGuiGrp
        */
        class _SwServiceShortcuts: QObject, virtual public ISwServiceShortcuts, virtual public ISwDeviceObserver{

			Q_OBJECT

        public:

			_SwServiceShortcuts();

			virtual ~_SwServiceShortcuts();


			//---------------------------------------------------------
			//-				Interface ISwServiceShortcuts			  -
			//---------------------------------------------------------

			/**
			 * @brief    : Permet d'enregistré une commande dans le service
			 * @param	 : QString command - Nom de la commande
			 * @param	 : ISwShortcut * obj - Pointeur sur l'objet
			 */
			void registerCommand(QString command, ISwShortcut *obj);

			/**
			 * @brief    : Permet de desenregistré une commande dans le service
			 * @param	 : QString command - Nom de la commande
			 */
			void unregisterCommand(QString command, ISwShortcut *obj);

			/**
			 * @brief    : Permet d'enregistrer un périphérique 
			 * @param	 : ISwDevice * device - pointeur sur le périphérique
			 */
			void registerDevice(ISwDevice * device);

			/**
			 * @brief    : Permet de désenregistrer un périphérique 
			 * @param	 : ISwDevice * device - pointeur sur le périphérique
			 */
			void unregisterDevice(ISwDevice * device);

				/**
			 * @brief    : Permet d'enregister un observer de nouvelle command
			 * @param	 : ISwShortcutObserver * obs - pointeur sur l'observeur
			 */
			virtual void registerShortcutObserver(ISwShortcutObserver * obs);

			/**
			 * @brief    : Permet de désenregistrer un observer de command
			 * @param	 : ISwShortcutObserver * obs - pointeur sur l'observeur
			 */
			virtual void unregisterShortcutObserver(ISwShortcutObserver * obs);

			/**
			 * @brief    : Bind un SwDevice sur un SwShortcut
			 * @param	 : QString device - DeviceName + SC_SEP + Device entry 
			 * @param	 : QString shortcut - ShortcutName + SC_SEP + CommandName 
			 */
			virtual void bind(QString swdevice, QString shortcut);

			/**
			 * @brief    : Unbind all shortcuts
			 */
			virtual void unBindAll();

			/**
			 * @brief    : Bind une combinaison de touche sur un SwShortcut
			 * @param	 : QString sequence - Sequence de touches
			 * @param	 : QString shortcut - ShortcutName + SC_SEP + CommandName 
			 * @param	 : QWidget * - Pointeur sur le widget ou seront binder les raccourcis
			 */
			virtual void bindKeyboard(QString sequence, QString shortcut,QWidget * context);

			/**
			 * @brief    : Retourne la list des Devices disponibles
			 * @return   : QList<ISwDevice*> - List des Devices connectés
			 */
			virtual QList<ISwDevice*> getDevices() ;

			/**
			 * @brief    : Retourne la map des bind des devices
			 * @return   : QMap<SwShortcut,QList<SwDevice>>
			 */
			virtual QMap<QString,QList<QString>> getBindedDevices();


			/**
			 * @brief    : Retourne la map des bind des keyboards
			 * @return   : QMap<QString,QList<QString>>
			 */
			virtual QMap<QString,QList<QString>> getBindedKeyboard();

			/**
			 * @brief    : Permet de configurer le service pour lui dire qu'on est en 
			 *			   mode de configuration pour désactiver les notifications 
			 *			   pendant le temps de la configuration
			 * @param	 : bool value - true si on est mode de configuration
			 */
			virtual void setConfigMode(bool value);

			/**
			 * @brief    : Retourne le texte avant le SC_SEP
			 * @return   : QString - Retourne Name
			 * @param	 : QString val - Une combinaison de Name + SC_SEP + commandName
			 */
			virtual QString getName(QString val);

			/**
			 * @brief    : Retourne le texte après le SC_SEP
			 * @return   : QString - Retourne commandName
			 * @param	 : QString val - Une combinaison de Name + SC_SEP + commandName
			 */
			virtual QString getParameter(QString val);

			/**
			 * @brief    : Genere un nom "prefix+SC_SEP+suffix)
			 * @return   : QString - le nom combiné
			 * @param	 : QString prefix - le prefix
			 * @param	 : QString suffix - le suffix
			 */
			virtual QString generateCombinedName(QString prefix,QString suffix); 


			//---------------------------------------------------------
			//-				Interface ISwDeviceObserver				  -
			//---------------------------------------------------------

		   /**
            * @brief    : Méthode appelée lors d'un appuye sur un ISwDeviceEntry
            * @param	 : QString entryName - le nom de l'entrée appuyée
            * @param	 : ISwDevice * device - sur le device
            */
           void notify(QString entryName, ISwDevice *device);

	    private slots:
			void shortcutSlot();
		private:

			QList<ISwShortcutObserver*> _listObs;

			QMap<QString,ISwShortcut*> _mapShortcuts; // QMap<Shorcut.getname+SC_SEP+commandName,ISwShortcut>
			QList<ISwDevice*> _listDevices;

			QMap<QString,QList<QString>> _mapDevicesAssoc;	// QMap<Shorcut.getname+SC_SEP+commandName,QList<Device.getName()+SC_SEP+entryName>>
			QMap<QString,QList<QString>> _mapKeyboardAssoc; //QMap<Shortcut.getname+SC_SEP+commandName,QList<Sequence.toString()>>

			QList<QShortcut*> _listKeyShortcut;
			bool _isConfigurationMode;

        };
    }
}

#endif
