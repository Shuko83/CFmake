/*!
\file ISwServiceMainWindow.h
\date 24/011/2015
\brief service permettant d'accéder a la mainWindow de l'application
\author  AAY
\version 1.0
 */

#pragma once
/*
 * INCLUDES GLOBAUX
 */
#include <QObject>
#include <QDebug>
#include <QMainWindow>

/*
 * INCLUDES LOCAUX
 */
#include "ISwService.h"

//Nom du service
#define CG_SW_SERVICE_MAINWINDOW "ISwServiceMainWindow"

namespace StreamWork
{
    namespace Service
    {

        /*!
        \interface ISwServiceMainWindow
        \brief permettant d'accéder a la mainWindow de l'application
        */
        class ISwServiceMainWindow: virtual public StreamWork::SwCore::ISwService
		{
        public:

			virtual QMainWindow* getMainWindow() = 0;

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
			virtual QString GetServiceName() { return QString(CG_SW_SERVICE_MAINWINDOW); }

             /**
             * @brief    : Renvoie le nom du service reel (le nom de l'interface)
			 * @return   : le nom du service reel (le nom de l'interface)
			 * @note     : au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()
             */
            virtual QString GetServiceRealName() {return QString(typeid(*this).name());}
        };
    }
}

