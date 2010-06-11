/**
@file SwIpV4Address.h
@brief Adresse IPV4
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWCORE_SWIPV4ADDRESS_H
#define _STREAMWORK_SWCORE_SWIPV4ADDRESS_H

#include <QString>
#include <QVariant>
#include <QDataStream>
#include "SwCoreConstantes.h"

namespace StreamWork {

    namespace SwCore {
    
        /**
        @class SwIpV4Address
        @brief Adresse IPV4
        */
        class BUILD_SWCORE SwIpV4Address {
        private:
            //Ip
            unsigned char _b0;
            unsigned char _b1;
            unsigned char _b2;
            unsigned char _b3;
        public:
	        /** @brief Constructor */
	        SwIpV4Address();
	        /** @brief Constructor */
	        SwIpV4Address(unsigned char b0,unsigned char b1,unsigned char b2,unsigned char b3);
            /*! \brief Constructeur de copie */
            SwIpV4Address(const SwIpV4Address & source);
            /*! \brief Destructeur */
            ~SwIpV4Address();
	        /** @brief Acces au nom */
            void getAddress(unsigned char &b0,unsigned char &b1,unsigned char &b2,unsigned char &b3) const;
            void setAddress(unsigned char b0,unsigned char b1,unsigned char b2,unsigned char b3);

            /*! \brief Operateur d'affectation */
            SwIpV4Address & operator=(const SwIpV4Address& source);
            /*! \brief Operateur de comparaison*/
            bool operator==(const SwIpV4Address& source) const;
            /*! \brief ecriture  de la valeur par une string*/
            void FromString(const QString & value);
            /*! \brief de recuperation de la valeur par une string*/
            QString ToString() const;

        };

    }
    
}

Q_DECLARE_METATYPE(StreamWork::SwCore::SwIpV4Address)

BUILD_SWCORE QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwIpV4Address &myObj);
BUILD_SWCORE QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwIpV4Address &myObj);

#endif
