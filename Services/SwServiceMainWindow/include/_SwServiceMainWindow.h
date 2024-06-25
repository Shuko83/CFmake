/*!
\file ISwServiceMainWindow.h
\date 24/011/2015
\brief service permettant d'accéder a la mainWindow de l'application
\author  AAY
\version 1.0
 */


#pragma once
/**
  * INCLUDES GLOBAUX
  */
#include <QObject>
#include <QWidget>
  
/**
 * INCLUDES LOCAUX
 */
#include "ISwServiceMainWindow.h"
#include "_SwServiceMainWindowConstantes.h"

namespace StreamWork
{
    namespace Service
    {
        /**
			\interface ISwServiceMainWindow
			\brief permettant d'accéder a la mainWindow de l'application
        */
		class BUILD_SERVICEMAINWINDOW _SwServiceMainWindow : public QObject, public ISwServiceMainWindow
		{
			Q_OBJECT;

        public:

			_SwServiceMainWindow();
			_SwServiceMainWindow(QMainWindow *window);

			virtual ~_SwServiceMainWindow();


			//---------------------------------------------------------
			//-				Interface ISwServiceMainWindow			  -
			//---------------------------------------------------------
			
			virtual QMainWindow* getMainWindow();
			virtual void setMainWindow(QMainWindow *window);


		private:
			QMainWindow *_window;

        };
    }
}

