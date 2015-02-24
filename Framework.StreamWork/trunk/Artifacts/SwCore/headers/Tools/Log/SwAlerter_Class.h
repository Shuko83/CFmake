/*!
\file SwLogger_Class.h
\date 25/03/09
\brief implementation d'un alert basique
\author  HLG
\version 1.0
 */

#ifndef _SwAlerter_Class_H
#define _SwAlerter_Class_H
/*
  * INCLUDES GLOBAUX
 */
#include <QSet>
/*
  * INCLUDES LOCAUX
 */
//#include "SwCoreConstantes.h"
#include "ISwAlerter.h"

namespace StreamWork
{
    namespace SwCore
    {
        /*!
        \class SwAlert_Class
        \brief alert basique
        */
        class SwAlerter_Class : public ISwAlerter {
        private:
            /*! \brief Liste des alert recorders*/
            QSet<ISwAlertRecorder *> _alert_recorders;
        public:
            /*! \brief Constructeur*/
            SwAlerter_Class();
            /*! \brief Destructeur*/
            virtual ~SwAlerter_Class();
	        /*! \brief Methode d'alerte */
	        void Alert(TSw_Alert_Level level,const char *format,...);
	        /*! \brief Methode d'alerte */
	        void Alert(TSw_Alert_Level level,QString alert_msg);
	        /*! \brief Permet d'attacher un alert recorder*/
	        void AttachAlertRecorder(ISwAlertRecorder * alert_recorder);
	        /*! \brief Permet de detacher un alert recorder*/
	        void DetachAlertRecorder(ISwAlertRecorder * alert_recorder);

        };
    }
}

#endif
