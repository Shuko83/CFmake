/*!
\file SwBuffer_Toolbox.cpp
\date 18/04/2006
\brief boite a outil pour la gestion de buffer
\author  Big
\version 1.0
 */

/*
 * INCLUDES GLOBAUX
 */
/*
 * INCLUDES LOCAUX
 */
#include "SwBuffer_Toolbox.h"
using namespace StreamWork::SwCore;

/*! \brief methode permettant de convertir un byte array en chaine d'hexanumber
\param[in] ba le bytearray
\return la chaine;
*/
QString SwBuffer_Toolbox::ConvertByteArrayIntoString(const QByteArray & ba) {
    QString s;
    int val;

    for(int i=0;i<ba.length();i++) {
        val=(int)ba[i];
        s+=QString("%1").arg(val,2,16,QLatin1Char('0')).toUpper();
    }

    return s;
}
/*! \brief methode permettant de convertir un chaine d'hexanumber en byte array
\param[in] s la chaine
\return le bytearray;
*/
QByteArray SwBuffer_Toolbox::ConvertStringIntoByteArrayInto(const QString & s){
    QString stmp=s;
    QByteArray ba;
    bool ok;

    while (stmp.length()>0) {
        ba+=(char)stmp.left(2).toInt(&ok,16);
        if (ok==false) return QByteArray();
        stmp.remove(0,2);
    }
    return ba;
}

