/**
@file SwFileDescriptor.h
@brief Descripteur de fichier
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWCORE_SWFILEDESCRIPTOR_H
#define _STREAMWORK_SWCORE_SWFILEDESCRIPTOR_H

#include <QString>
#include <QVariant>
#include <QDataStream>
#include "SwCoreConstantes.h"


namespace StreamWork {

    namespace SwCore {
    
        /**
        @class SwFileDescriptor
        @brief Descripteur de fichier
        @ingroup SwCoreGrp
        */
        class BUILD_SWCORE SwFileDescriptor {
        public:
            /** @brief Enumerateur de type */
            enum FileType {FileRead,FileWrite,DirectorySelect};
        private:    
            /** @brief Filename */
            QString _filename;
            /** @brief Filter */
            QString _filter;
            /** @brief Filter */
            enum FileType _fileType;
        public:
	        /** @brief Constructor */
	        SwFileDescriptor();
	        /** @brief Constructor */
	        SwFileDescriptor(enum FileType fileType,const QString & filter,const QString &filename=QString());
            /*! \brief Constructeur de copie */
            SwFileDescriptor(const SwFileDescriptor & source);
            /*! \brief Destructeur */
            ~SwFileDescriptor();
	        /** @brief Acces au filtre */
            QString getFilter() const;
	        /** @brief Acces au type */
            enum FileType getFileType() const;
	        /** @brief Acces au nom */
            QString getFileName() const;
            void setFileName(const QString filename);

            /** @brief
            /* Pour un path de la forme C:\path\to\file \::relativeFolder
            /* renvoie relativeFolder si relativeFolder existe a coté de l'exe
            /* renvoie C:\path\to\file \relativeFolder sinon (le cas general quand on est ds le stream)
            */
            QString getDoubleDottedPath(bool * relativeExists);

            /*! \brief Operateur d'affectation */
            SwFileDescriptor & operator=(const SwFileDescriptor& source);
            /*! \brief Operateur de comparaison*/
            bool operator==(const SwFileDescriptor& source) const;
            /*! \brief de recuperation de la valeur par une string*/
            QString ToString() const;

        };

    }
    
}

Q_DECLARE_METATYPE(StreamWork::SwCore::SwFileDescriptor)

BUILD_SWCORE QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwFileDescriptor &myObj);
BUILD_SWCORE QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwFileDescriptor &myObj);

#endif
