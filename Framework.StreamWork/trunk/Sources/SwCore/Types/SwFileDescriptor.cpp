/**
@file SwFileDescriptor.cpp
@brief Descripteur de fichier
@author F.Bighelli
 */

#include "SwFileDescriptor.h"
 
using namespace StreamWork::SwCore;

/** @brief Constructor */
SwFileDescriptor::SwFileDescriptor() {
	_filename="";
    _filter="*";
    _fileType=FileRead;
}
/** @brief Constructor */
SwFileDescriptor::SwFileDescriptor(enum FileType fileType,const QString & filter,const QString &filename){
	_filename=filename;
    _filter=filter;
    _fileType=fileType;
}
/*! \brief Constructeur de copie */
SwFileDescriptor::SwFileDescriptor(const SwFileDescriptor & source){
	_filename=source._filename;
    _filter=source._filter;
    _fileType=source._fileType;
}
/*! \brief Destructeur */
SwFileDescriptor::~SwFileDescriptor(){
	//A Completer
}
/** @brief Acces au filtre */
QString SwFileDescriptor::getFilter() const{
	return _filter;
}
/** @brief Acces au type */
SwFileDescriptor::FileType SwFileDescriptor::getFileType() const{
	return _fileType;
}
/** @brief Acces au nom */
QString SwFileDescriptor::getFileName() const{
	return _filename;
}
void SwFileDescriptor::setFileName(const QString filename){
	_filename=filename;
}

/*! \brief Operateur d'affectation */
SwFileDescriptor & SwFileDescriptor::operator=(const SwFileDescriptor& source){
    if (this!=&source) {
	    _filename=source._filename;
        _filter=source._filter;
        _fileType=source._fileType;
    }
    return *this;
}
/*! \brief Operateur de comparaison*/
bool SwFileDescriptor::operator==(const SwFileDescriptor& source) const{
	return _filename==source._filename;
}
/*! \brief de recuperation de la valeur par une string*/
QString SwFileDescriptor::ToString() const{
	return getFileName();
}

QString SwFileDescriptor::getDoubleDottedPath(bool * relativeExists)
{
    QString pathAsWritten = getFileName();
    int indexOfSeparator = pathAsWritten.indexOf("::");
    QString relativePathString = "." + pathAsWritten.mid(pathAsWritten.indexOf("::") + 2);
    QString absolutePath = pathAsWritten;

    absolutePath.remove("::");
  

    if(indexOfSeparator != -1)
    {
    	//search in relative :
        if (QFile::exists(relativePathString))
    	{
            *relativeExists = true;
    		return relativePathString;
    	}
    	else
    	{
            *relativeExists = false;
    		return absolutePath;
    	}
    }
    else
   { 
       return pathAsWritten; 
   }

    *relativeExists = false;
    return "";
}

QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwFileDescriptor &myObj) {
    out<<myObj.getFileName();
    return out;
}
QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwFileDescriptor &myObj) {
    QString tmp;
    in>>tmp;
    myObj.setFileName(tmp);
    return in;
}
