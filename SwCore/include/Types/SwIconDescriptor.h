/**
@file SwIconDescriptor.h
@brief IconDescriptor
@author Big
 */

#ifndef _STREAMWORK_SWCORE_SWICONDESCRIPTOR_H
#define _STREAMWORK_SWCORE_SWICONDESCRIPTOR_H


#include <QString>
#include <QIcon>
#include <QVariant>
#include <QDataStream>
#include "SwCoreConstantes.h"


namespace StreamWork {

    namespace SwCore {
    
        /**
        @class SwIconDescriptor
        @brief IconDescriptor
        */
        class BUILD_SWCORE SwIconDescriptor {
        public:
            /** @brief Constructor */
            SwIconDescriptor();
            /** @brief Destructor */
            virtual ~SwIconDescriptor();
            /*! \brief Operateur d'affectation */
            SwIconDescriptor & operator=(const SwIconDescriptor& source);
            /*! \brief Operateur de comparaison*/
            bool operator==(const SwIconDescriptor& source) const;
            /*! \brief de recuperation de la valeur par une string*/
            QString ToString() const;
            /*! \brief de recuperation de la valeur par un icon*/
            QIcon ToIcon() const;
            /*! \brief set du path de l'icon*/
            void setPath(QString path);
                        
        private:
            QString _path;
            QIcon _icon;
        };
    
    }

}

Q_DECLARE_METATYPE(StreamWork::SwCore::SwIconDescriptor)

BUILD_SWCORE QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwIconDescriptor &myObj);
BUILD_SWCORE QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwIconDescriptor &myObj);


#endif
