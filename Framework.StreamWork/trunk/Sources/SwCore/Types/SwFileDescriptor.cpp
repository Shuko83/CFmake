/**
@file SwFileDescriptor.cpp
@brief Descripteur de fichier
@author F.Bighelli
*/

#include "SwFileDescriptor.h"
#include "SwMacros.h"

using namespace StreamWork::SwCore;


//-----------------------------------------------------------------------
SwFileDescriptor::SwFileDescriptor()
{
	_filename = "";
	_filter = "*";
	_fileType = FileRead;
}

//-----------------------------------------------------------------------
SwFileDescriptor::SwFileDescriptor(enum FileType fileType, const QString & filter, const QString &filename)
{
	_filename = filename;
	_filter = filter;
	_fileType = fileType;
}

//-----------------------------------------------------------------------
SwFileDescriptor::SwFileDescriptor(const SwFileDescriptor & source)
{
	_filename = source._filename;
	_filter = source._filter;
	_fileType = source._fileType;
}

//-----------------------------------------------------------------------
SwFileDescriptor::~SwFileDescriptor()
{
	//A Completer
}

//-----------------------------------------------------------------------
QString SwFileDescriptor::getFilter() const
{
	return _filter;
}

//-----------------------------------------------------------------------
SwFileDescriptor::FileType SwFileDescriptor::getFileType() const
{
	return _fileType;
}

//-----------------------------------------------------------------------
QString SwFileDescriptor::getFileName() const
{
	return _filename;
}

//-----------------------------------------------------------------------
void SwFileDescriptor::setFileName(const QString filename)
{
	_filename = filename;
}


//-----------------------------------------------------------------------
SwFileDescriptor & SwFileDescriptor::operator=(const SwFileDescriptor& source)
{
	if ( this != &source )
	{
		_filename = source._filename;
		_filter = source._filter;
		_fileType = source._fileType;
	}
	return *this;
}

//-----------------------------------------------------------------------
bool SwFileDescriptor::operator==(const SwFileDescriptor& source) const
{
	return _filename == source._filename;
}

//-----------------------------------------------------------------------
QString SwFileDescriptor::ToString() const
{
	return getFileName();
}


//-----------------------------------------------------------------------
QString SwFileDescriptor::getDoubleDottedPath(bool * relativeExists)
{
	QString pathAsWritten = getFileName();
	int indexOfSeparator = pathAsWritten.indexOf("::");
	QString tmpS = pathAsWritten;
	tmpS.replace("::", "");
	QString relativePathString = QDir::cleanPath(SW_APP->GetApplicationDirPath() + "/" + tmpS);
	QString relativePathStringDoubleDot = QDir::cleanPath(qApp->applicationDirPath() +"/"+ pathAsWritten.mid(indexOfSeparator+2));
	QString qrcPathString = ":" + pathAsWritten.mid(indexOfSeparator+2 );
	QString absolutePath = pathAsWritten;
	absolutePath.remove("::");
	qrcPathString = qrcPathString.replace("\\", "/");

	if ( QFile::exists(absolutePath) )
	{
		if ( relativeExists )
			*relativeExists = false;
		return  QDir::cleanPath(absolutePath);
	}
	else if ( QFile::exists(relativePathString) )
	{
		QFileInfo info(relativePathString);
		//search in relative to swAPP:
		if ( relativeExists )
			*relativeExists = true;
		return info.absoluteFilePath();
	}
	else if ( QFile::exists(relativePathStringDoubleDot) )
	{
		//search in relative :
		if ( relativeExists )
			*relativeExists = true;
		return  QDir::cleanPath(relativePathStringDoubleDot);
	}
	else if ( QFile::exists(qrcPathString) )
	{
		//search in QtRessource :
		if ( relativeExists )
			*relativeExists = true;
		return qrcPathString;
	}
	
	//On dit que c'est absolute quand meme
	if ( relativeExists )
		*relativeExists = false;
	return QDir::cleanPath(relativePathString);
}


//-----------------------------------------------------------------------
QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwFileDescriptor &myObj)
{
	out << myObj.getFileName();
	return out;
}

//-----------------------------------------------------------------------
QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwFileDescriptor &myObj)
{
	QString tmp;
	in >> tmp;
	myObj.setFileName(tmp);
	return in;
}
