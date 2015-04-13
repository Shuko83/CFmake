#ifndef _SWSERVICESALERT_H
#define _SWSERVICESALERT_H
//QT includes
#include <QObject>

//Other External includes

//Local includes
#include "ISwServiceAlert.h"


namespace alert { 
	class AlertManager;
	class IAlertObserver;
}


namespace StreamWork
{
    namespace SwCore
    {
        /*!
        \interface _SWServiceAlert
        \brief service de  gestion des Alert
        @ingroup SwCore
        */
        class _SwServiceAlert: QObject, 
							   virtual public ISwServiceAlert
		{
			Q_OBJECT

        public:

			_SwServiceAlert();
			virtual ~_SwServiceAlert();

			//---------------------------------------------------------
			//-				Interface ISwServiceAlert			      -
			//---------------------------------------------------------
			virtual void addObserver(alert::IAlertObserver * alertObserver, QString errorCategory);
			virtual void removeCategoryObserver(alert::IAlertObserver * alertObserver);
			virtual void raiseAlert(QString title, QString description, alert::EnumErrorLevel::ErrorLevel level);
			virtual void raiseError(alert::Error * error);
			virtual void raiseError(alert::Error * error, double timeOut);
			virtual void raiseErrorEnd(alert::Error * error);
			virtual void raiseErrorEnd(alert::ErrorEnd * errorEnd);
			virtual alert::AlertManager * getAlertManager();
			
		private:
			alert::AlertManager * _alertManager;

        };
    }
}

#endif
