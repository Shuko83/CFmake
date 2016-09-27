#pragma once

/**
* INCLUDES LOCAUX
*/
#include "SwServiceContextMenuConstantes.h"
#include "ISwServiceContextMenu.h"
#include "SwActionService.h"
#include "SwServiceManager_Helper.h"

/**
* INCLUDES GLOBAUX
*/
#include <tuple>
#include <QObject>
#include <QWidget>

/**
* Interfaces
*/
#include "ContextMenu.h"

namespace StreamWork
{
	namespace Service
	{
		class BUILD_SWSERVICECONTEXTMENU SwServiceContextMenu : public QObject,
																public ContextMenu,
																public ISwServiceContextMenu
		{
			Q_OBJECT;

		public:

			SwServiceContextMenu();			
			virtual ~SwServiceContextMenu();

			void registerContextAction(Action::IAction * action) override { ContextMenu::registerContextAction(action); };
			void registerContextAction(QAction * action) override { ContextMenu::registerContextAction(action); };
			
			QMenu * getMenu() override;	

			void enableContextMenu(bool enable) override{ ContextMenu::enableContextMenu(enable); };

			void setMenuRequestCartoPosition(double latitude, double longitude, double altitude) override;
			std::tuple<double, double, double> getMenuRequestCartoPosition() override;

		private:

			std::tuple<double, double, double> _requestCartoPosition; //latitude, longitude et altitude (deg decimal, deg decimal et mètre) 

			bool _hasBeenInitialized;

			SwServiceManager_Helper<SwActionService> * _actionServiceHelper;
		};
	}
}

