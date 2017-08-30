/**
@file SwFileDescriptor.cpp
@brief Descripteur de fichier
@author F.Bighelli
*/

#include "SwFileDescriptor.h"
#include "SwMacros.h"
#include <QApplication>
#include <QDir>

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
QString SwFileDescriptor::getDoubleDottedPath(bool * relativeExists, bool useOptionalPath)
{
	QString pathAsWritten = getFileName();

	// On commence par remplacer les éventuelles variables d'environnment :
	QRegularExpression environmentVariableRegExp("^([^%]*)%([^%]+)%(.*)$");
	QRegularExpressionMatch match;
	
	while (match = environmentVariableRegExp.match(pathAsWritten), match.hasMatch())
	{
		QString pre = match.captured(1);
		QString envVarName = match.captured(2);
		QString post = match.captured(3);
		
		QString envVarValue;
		if (qEnvironmentVariableIsSet(envVarName.toStdString().c_str()))
		{
			envVarValue = qgetenv(envVarName.toStdString().c_str());
		}
		
		pathAsWritten = pre + envVarValue + post;
	}

	// Rechrche du fichier suivant les :: et les ( )
	int indexOfSeparator = pathAsWritten.indexOf("::");
	QString pathWithoutDot = pathAsWritten;
	pathWithoutDot.replace("::", "");
	
	QString relativePathString;
	QString relativePathStringDoubleDot;
	QString absolutePath;
	QString qrcPathString;

	// Gestion d'un répertoire optionnel pour les cas ou l'arborescence n'est pas la même en 
	// dev local que dans un répertoire d'execution de l'appli
	QString relativePathStringWithOptionalFolderDoubleDot;
	QString relativePathStringWithOptionalFolder;
	QString absolutePathWithOptionalFolder;
	if (useOptionalPath && pathWithoutDot.contains("(") && pathWithoutDot.contains(")"))
	{
		int indexOfOpenBracet = pathWithoutDot.indexOf("(");
		int indexOfCloseBracet = pathWithoutDot.indexOf(")");
		QString optfolderName = pathWithoutDot.mid(indexOfOpenBracet, indexOfCloseBracet - indexOfOpenBracet + 1);

		QString optionnalPath = pathWithoutDot;
		optionnalPath.replace("(", "").replace(")", "");
		QString optionnalPathDoubleDot = pathAsWritten.mid(indexOfSeparator + 2);
		optionnalPathDoubleDot.replace("(", "").replace(")", "");

		relativePathStringWithOptionalFolderDoubleDot = QDir::cleanPath(qApp->applicationDirPath() + "/" + optionnalPathDoubleDot);
		relativePathStringWithOptionalFolder = QDir::cleanPath(SW_APP->GetApplicationDirPath() + "/" + optionnalPath);
		absolutePathWithOptionalFolder = pathWithoutDot;
		absolutePathWithOptionalFolder.replace("(", "").replace(")", "");
		
		pathWithoutDot.remove(optfolderName);
		pathAsWritten.remove(optfolderName);
	}
	else
	{
		qrcPathString = ":/" + pathAsWritten;
		if (indexOfSeparator != -1)
			qrcPathString = ":" + pathAsWritten.mid(indexOfSeparator + 2);
		qrcPathString = qrcPathString.replace("\\", "/");
	}

	relativePathString = QDir::cleanPath(SW_APP->GetApplicationDirPath() + "/" + pathWithoutDot);
	relativePathStringDoubleDot = QDir::cleanPath(qApp->applicationDirPath() + "/" + pathAsWritten.mid(indexOfSeparator + 2));

	absolutePath = pathWithoutDot;
	
	if (QFile::exists(absolutePathWithOptionalFolder))
	{
		if (relativeExists)
			*relativeExists = false;
		return  QDir::cleanPath(absolutePathWithOptionalFolder);
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
	else if (QFile::exists(relativePathStringWithOptionalFolderDoubleDot))
	{ 
		//search in relative with optionnal folder :
		if (relativeExists)
			*relativeExists = true;
		return  QDir::cleanPath(relativePathStringWithOptionalFolderDoubleDot);
	}
	else if (QFile::exists(relativePathStringWithOptionalFolder))
	{
		//search in relative to swAPP with optionnal folder :
		if (relativeExists)
			*relativeExists = true;
		return  QDir::cleanPath(relativePathStringWithOptionalFolder);
	}
	else if (QFile::exists(absolutePath))
	{
		if (relativeExists)
			*relativeExists = false;
		return  QDir::cleanPath(absolutePath);
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
