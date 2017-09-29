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
bool SwFileDescriptor::operator!=(const SwFileDescriptor& source) const
{
	return !operator==(source);
}

//-----------------------------------------------------------------------
QString SwFileDescriptor::ToString() const
{
	return getFileName();
}


//-----------------------------------------------------------------------
QString SwFileDescriptor::getDoubleDottedPath(bool useOptionalPath)
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

	QRegularExpression optionnalPathExp("\\(.*?\\)");
	if (pathAsWritten.count("::") > 1)
	{
		qWarning() << QString("SwFileDescriptor: Multiple Dev/Deploy separators (::) : %1").arg(pathAsWritten);
		return pathAsWritten;
	}

	if (!pathAsWritten.contains("::")) // cas simple
	{
		//if (pathAsWritten.contains(optionnalPathExp)) // chemin optionnel uniquement après ::
		//	qWarning() << QString("SwFileDescriptor bar formating : %1").arg(pathAsWritten);
		return pathAsWritten;
	}

	QString streamPath = pathAsWritten;
	streamPath.remove("::");
	
	QString executablePath = pathAsWritten.section("::", 1); // after ::

	// bad optionnal formating
	if (useOptionalPath
		&& (executablePath.count(optionnalPathExp) > 1
		|| pathAsWritten.count(optionnalPathExp) > executablePath.count(optionnalPathExp)))
	{
		qWarning() << QString("SwFileDescriptor bar formating : %1").arg(pathAsWritten);
		return pathAsWritten;
	}

	// pas de chemin optionnel
	if (!executablePath.contains(optionnalPathExp) || !useOptionalPath)
	{
		if (QFileInfo(streamPath).isAbsolute())
			streamPath = QDir::cleanPath(streamPath);
		else
			streamPath = QDir::cleanPath(SW_APP->GetApplicationDirPath() + "/" + streamPath);
		QString resourcePath = ":/" + executablePath;
		executablePath = QDir::cleanPath(SW_APP->GetApplicationDirPath() + "/" + executablePath);

		if (QFileInfo::exists(resourcePath))
			return resourcePath;
		if (QFileInfo::exists(executablePath))
			return executablePath;
		if (QFileInfo::exists(streamPath))
			return streamPath;

		qWarning() << QString("SwFileDescriptor file does not exist : %1").arg(pathAsWritten);
		return pathAsWritten;
	}

	QRegularExpression fullPathExp("^([^(]*)(?:\\((.*)\\))?([^)]*)$");

	QRegularExpressionMatch streamPathMatch = fullPathExp.match(streamPath);
	QString longStreamPath = streamPathMatch.captured(1) + streamPathMatch.captured(2) + streamPathMatch.captured(3);
	QString shortStreamPath = streamPathMatch.captured(1) + streamPathMatch.captured(3);

	QRegularExpressionMatch executablePathMatch = fullPathExp.match(executablePath);
	QString longExecutablePath = executablePathMatch.captured(1) + executablePathMatch.captured(2) + executablePathMatch.captured(3);
	QString shortExecutablePath = executablePathMatch.captured(1) + executablePathMatch.captured(3);

	if (QFileInfo(longStreamPath).isAbsolute())
		longStreamPath = QDir::cleanPath(longStreamPath);
	else
		longStreamPath = QDir::cleanPath(SW_APP->GetApplicationDirPath() + "/" + longStreamPath);

	if (QFileInfo(shortStreamPath).isAbsolute())
		shortStreamPath = QDir::cleanPath(shortStreamPath);
	else
		shortStreamPath = QDir::cleanPath(SW_APP->GetApplicationDirPath() + "/" + shortStreamPath);

	QString longResourcePath = ":/" + longExecutablePath;
	QString shortResourcePath = ":/" + shortExecutablePath;
	longExecutablePath = QDir::cleanPath(SW_APP->GetApplicationDirPath() + "/" + longExecutablePath);
	shortExecutablePath = QDir::cleanPath(SW_APP->GetApplicationDirPath() + "/" + shortExecutablePath);

	if (QFileInfo::exists(longResourcePath))
		return longResourcePath;
	if (QFileInfo::exists(shortResourcePath))
		return shortResourcePath;
	if (QFileInfo::exists(longExecutablePath))
		return longExecutablePath;
	if (QFileInfo::exists(shortExecutablePath))
		return shortExecutablePath;
	if (QFileInfo::exists(longStreamPath))
		return longStreamPath;
	if (QFileInfo::exists(shortStreamPath))
		return shortStreamPath;

	qWarning() << QString("SwFileDescriptor file does not exist : %1").arg(pathAsWritten);
	return pathAsWritten;
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
