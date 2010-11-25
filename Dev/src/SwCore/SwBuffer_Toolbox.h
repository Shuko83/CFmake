/*!
\file SwBuffer_Toolbox.h
\date 18/04/2006
\brief boite a outil pour la gestion de buffer
\author  Big
\version 1.0
 */

#ifndef _SwBuffer_Toolbox_H
#define _SwBuffer_Toolbox_H
/*
  * INCLUDES GLOBAUX
  */
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
/*
 * INCLUDES LOCAUX
 */

namespace StreamWork 
{
	namespace SwCore
	{
        /*!
        \class SwBuffer_Toolbox
        \brief boite a outil pour la gestion de buffer
        @ingroup SwCoreGrp
        \note
        */

        class SwBuffer_Toolbox  {
        public:
	        /*! \brief methode permettant de convertir un byte array en chaine d'hexanumber
            \param[in] ba le bytearray
            \return la chaine;
            */
	        static QString ConvertByteArrayIntoString(const QByteArray & ba);
	        /*! \brief methode permettant de convertir un chaine d'hexanumber en byte array 
            \param[in] s la chaine 
            \return le bytearray;
            */
	        static QByteArray ConvertStringIntoByteArrayInto(const QString & s);

	        /*! \brief methode permettant de convertir une donnée d'une type supportant << et >> avec un QDataStream en String 
            \param[in] val la valeur
            \return la chaine;
            */
            template<typename T> static inline QString ConvertIntoString(const T & val) {
                QByteArray working_array;
                QDataStream * datastream=new QDataStream(&working_array,QIODevice::WriteOnly);
                (*datastream)<<val;
                return ConvertByteArrayIntoString(working_array);
            }
	        /*! \brief methode permettant de faire l'operation inverse de la precedente 
            \param[in] s la chaine 
            \param[in] val la valeur a charger 
            \return true si ok
            */
            template<typename T> static inline bool ConvertFromString(QString s,T & val) {
                QByteArray working_array=ConvertStringIntoByteArrayInto(s);
                if (working_array.length()<=0) return false;
                QDataStream * datastream=new QDataStream(&working_array,QIODevice::ReadOnly);
                (*datastream)>>val;
                return true;
            }

       };
    }
}

#endif
