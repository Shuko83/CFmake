#pragma once
/*
* INCLUDES GLOBAUX
*/
#include "IContextMenu.h"

/*
* INCLUDES QT
*/
#include <QObject>
#include <QDebug>
#include <QMainWindow>

/*
* INCLUDES LOCAUX
*/
#include "ISwService.h"

using namespace StreamWork::SwCore;

#define CG_SW_SERVICE_CONTEXT_MENU "ISwServiceContextMenu"

namespace StreamWork
{
	namespace Service
	{

		/*!
		\interface ISwServiceMainWindow
		\brief permettant d'accéder a la mainWindow de l'application
		*/
		class ISwServiceContextMenu : public IContextMenu, public ISwService
		{
		public:
			//---------------------------------------------------------------------
			// Interface ISwService
			//---------------------------------------------------------------------
			/**
			* @brief    : Est appele uniquement par le service manager aupres duquel le service est enregistré
			lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée
			*/
			void Liberate() override {};
			/**
			* @brief    : Renvoie le nom du service
			* @return   : le nom du service
			*/
			virtual QString GetServiceName() override { return QString(CG_SW_SERVICE_CONTEXT_MENU); }

			/**
			* @brief    : Renvoie le nom du service reel (le nom de l'interface)
			* @return   : le nom du service reel (le nom de l'interface)
			* @note     : au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(this).name()
			*/
			virtual QString GetServiceRealName() override{ return QString(typeid(this).name()); }

			//Methode specifique a Sx
			virtual void setMenuRequestCartoPosition(double, double, double) = 0;		
			virtual std::tuple<double, double, double> getMenuRequestCartoPosition() = 0;
		};
	}
}

