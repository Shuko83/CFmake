/*!
\file SwSplash.h
\date 12/04/2006
\brief Ihm du wizard de generation de plugin
\author  Big
\version 1.0
 */

#ifndef _SwSplash_H
#define _SwSplash_H
/*
  * INCLUDES GLOBAUX
  */

#include <QSplashScreen>
#include <QPixmap>
#include "Tools/Log/ISwLogRecorder.h"
/*
  * INCLUDES LOCAUX
  */
using namespace StreamWork::SwCore;

/*!
	\class SwSplash 
	\brief Ihm du wizard de generation de plugin
*/
class SwSplash : public QSplashScreen, public ISwLogRecorder
{
    Q_OBJECT
public:
    /*! \brief Constructeur */
    SwSplash(const QPixmap & pixmap = QPixmap());
    /*! \brief Destructeur */
    ~SwSplash();
	/*! \brief Methode de log */
	void RecordLog(TSw_Log_Level level,QString message);
private slots:
    /*! \brief Close du splash */
    void CloseSplash();
};

#endif
