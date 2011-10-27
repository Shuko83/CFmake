/*!
\file SwSplash.cpp
\date 12/04/2006
\brief Ihm du wizard de generation de plugin
\author  Big
\version 1.0
 */
#include <QTimer>
#include <QBitmap>
#include <ISwLogRecorder.h>
#include <QApplication>
#include "SwSplash.h"

/*! \brief Constructeur */
SwSplash::SwSplash(const QPixmap & pixmap)
    : QSplashScreen(pixmap,Qt::SplashScreen | Qt::WindowStaysOnTopHint)
{
    //setAttribute(Qt::WA_TranslucentBackground);
    setMask(pixmap.mask());
    /*QTimer * timer=new QTimer(this);*/
    QTimer::singleShot(2000,this, SLOT(CloseSplash()));
}
/*! \brief Destructeur */
SwSplash::~SwSplash()
{
}
/*! \brief Close du splash */
void SwSplash::CloseSplash() {
    hide();
}
/*! \brief Methode de log */
void SwSplash::RecordLog(TSw_Log_Level level,QString message) {
    if (this->isVisible() && level==LogLvl_Info) {
        showMessage(message, Qt::AlignLeft | Qt::AlignBottom,Qt::white);
        qApp->processEvents();
    }
}
