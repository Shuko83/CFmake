/*
* INCLUDES LOCAUX
*/
#include "SwAliasManager.h"
#include "SwUnloadedATOAliasError.h"

/*
* INCLUDES GLOBAUX
*/
#include "LoadedAliasError.h"
#include "UnloadedXmlAliasError.h"
#include "ATOReader.h"
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QTextStream>

namespace SwServiceAlias
{
	//---------------------------------------------------------------------------------
	SwAliasManager::SwAliasManager()
		: _aliasManager(nullptr), _alertManager(nullptr)
	{
		_alertServiceHelper = new SwServiceManager_Helper<StreamWork::SwCore::ISwServiceAlert, SwAliasManager>();

		_roamingPath = QDir::homePath() + QDir::separator() + "AppData" + QDir::separator() + "Roaming" + QDir::separator() + "diginext" + QDir::separator() + "Starlinx" + QDir::separator() + "Alias" + QDir::separator();
		QDir dir;
		if (!dir.exists(_roamingPath))
		{
			dir.mkpath(_roamingPath);
		}
		// À mettre en dernier pour que toutes les variables soient initialisées
		_alertServiceHelper->setService(CG_SW_SERVICE_ALERT, this, &SwAliasManager::onServiceAlertManagerAvailable);
	}

	//---------------------------------------------------------------------------------
	SwAliasManager::~SwAliasManager()
	{
		saveAutoAlias();
		saveAliasFastAction();
		saveAliasOrder();

		delete _aliasManager;
		delete _alertServiceHelper;
	}

	//---------------------------------------------------------------------------------
	QString SwAliasManager::getPrimaryAlias(gusa::model::interactionInterface::ITrack *track) const
	{
		POINTER_UNAVAILABLE(_aliasManager, QString());

		// Recherche dans la liste des fast action
		QString numberFA = getNumber(track, Enums::SwSourceType::TN);
		QString numberFARange = numberFA;
		numberFARange.replace(numberFA.count() - 1, 1, CHAR_RANGE);

		if (_fastAction.contains(AliasManager::Source(numberFARange, static_cast<unsigned int>(Enums::SwSourceType::TN))))
		{
			return _aliasManager->tryGetRangeAlias(numberFA, static_cast<unsigned int>(Enums::SwSourceType::TN));
		}

		if (_fastAction.contains(AliasManager::Source(numberFA, static_cast<unsigned int>(Enums::SwSourceType::TN))))
		{
			return _aliasManager->getAlias(numberFA, static_cast<unsigned int>(Enums::SwSourceType::TN))->getNameAlias();
		}

		// Recherche par ordre de priorité
		for (QList<Enums::SwSourceType>::const_iterator ite = _primaryOrder.begin(); ite != _primaryOrder.end(); ++ite)
		{
			QString number = getNumber(track, *ite);
			QString aliasRange = _aliasManager->tryGetRangeAlias(number, static_cast<unsigned int>(*ite));

			// Un alias spécifique est prioritaire sur un alias de plage
			if (_aliasManager->isExist(number, static_cast<unsigned int>(*ite)))
			{
				return _aliasManager->getAlias(number, static_cast<unsigned int>(*ite))->getNameAlias();
			}

			if (!aliasRange.isNull())
			{
				return aliasRange;
			}
		}
		return QString();
	}

	//---------------------------------------------------------------------------------
	QString SwAliasManager::getSecondaryAlias(gusa::model::interactionInterface::ITrack *track) const
	{
		POINTER_UNAVAILABLE(_aliasManager, QString());

		// Recherche par ordre de priorité
		for (QList<Enums::SwSourceType>::const_iterator ite = _secondaryOrder.begin(); ite != _secondaryOrder.end(); ++ite)
		{
			QString number = getNumber(track, *ite);
			QString aliasRange = _aliasManager->tryGetRangeAlias(number, static_cast<unsigned int>(*ite));

			// Un alias spécifique est prioritaire sur un alias de plage
			if (_aliasManager->isExist(number, static_cast<unsigned int>(*ite)))
			{
				return _aliasManager->getAlias(number, static_cast<unsigned int>(*ite))->getNameAlias();
			}

			if (!aliasRange.isNull())
			{
				return aliasRange;
			}
		}
		return QString();
	}

	//---------------------------------------------------------------------------------
	bool SwAliasManager::isExist(gusa::model::interactionInterface::ITrack *track) const
	{
		POINTER_UNAVAILABLE(_aliasManager, false);

		bool result = false;
		QString number;
		unsigned int type;

		number = getNumber(track, Enums::SwSourceType::TN);
		type = static_cast<unsigned int>(Enums::SwSourceType::TN);
		result |= _aliasManager->isExist(number, type);

		number = getNumber(track, Enums::SwSourceType::VCS);
		type = static_cast<unsigned int>(Enums::SwSourceType::VCS);
		result |= _aliasManager->isExist(number, type);

		number = getNumber(track, Enums::SwSourceType::IFF1);
		type = static_cast<unsigned int>(Enums::SwSourceType::IFF1);
		result |= _aliasManager->isExist(number, type);

		number = getNumber(track, Enums::SwSourceType::IFF2);
		type = static_cast<unsigned int>(Enums::SwSourceType::IFF2);
		result |= _aliasManager->isExist(number, type);

		number = getNumber(track, Enums::SwSourceType::IFF3);
		type = static_cast<unsigned int>(Enums::SwSourceType::IFF3);
		result |= _aliasManager->isExist(number, type);

		return result;
	}

	//---------------------------------------------------------------------------------
	bool SwAliasManager::isExist(QString number, Enums::SwSourceType type) const
	{
		POINTER_UNAVAILABLE(_aliasManager, false);

		QString numberRange = number;
		numberRange.replace(numberRange.count() - 1, 1, CHAR_RANGE);

		return _aliasManager->isExist(number, static_cast<unsigned int>(type))
			|| _aliasManager->isExist(numberRange, static_cast<unsigned int>(type));
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::registerObserver(AliasManager::IAliasObserver *obs)
	{
		POINTER_UNAVAILABLE(_aliasManager, );

		_aliasManager->registerObserver(obs);
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::unregisterObserver(AliasManager::IAliasObserver *obs)
	{
		POINTER_UNAVAILABLE(_aliasManager, );

		_aliasManager->unregisterObserver(obs);
	}

	//---------------------------------------------------------------------------------
	QString SwAliasManager::getXmlSchemaPath() const
	{
		POINTER_UNAVAILABLE(_aliasManager, QString());

		return _aliasManager->getXmlSchemaPath();
	}

	//---------------------------------------------------------------------------------
	QString SwAliasManager::getRoamingPath() const
	{
		POINTER_UNAVAILABLE(_aliasManager, QString());

		return _roamingPath;
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::setXmlSchemaPath(QString path)
	{
		POINTER_UNAVAILABLE(_aliasManager, );

		_aliasManager->setXmlSchemaPath(path);
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::loadAlias(QString path)
	{
		POINTER_UNAVAILABLE(_aliasManager, );

		QFile *file = new QFile(path);
		_aliasManager->loadAlias(file);
		delete file;
		saveAutoAlias();
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::loadAliasFromATO(QString path)
	{
		POINTER_UNAVAILABLE(_aliasManager, );

		QMap<QString, QString> callSignsMap;
		if (ATOReader::getIFFMode3CallsignsMapFromATOFile(path, callSignsMap))
		{
			QList<AliasManager::Alias*> list;
			QMapIterator<QString, QString> ite(callSignsMap);
			bool doublon = false;
			while (ite.hasNext())
			{
				ite.next();
				list.append(new AliasManager::Alias(ite.key(), static_cast<unsigned int>(Enums::SwSourceType::IFF3), ite.value()));
				doublon = isExist(ite.key(), Enums::SwSourceType::IFF3);
			}

			if (doublon)
			{
				alert::Error *error = new AliasManager::Alerts::LoadedAliasError(alert::ProcessId(QUuid(ALIAS_MANAGER), 0), _aliasManager, list);
				_aliasManager->getAlertManager()->raiseError(error);
			}
			else
			{
				_aliasManager->addAlias(list);
			}
		}
		else
		{
			alert::Error *error = new SwUnloadedATOAliasError(alert::ProcessId(QUuid(ALIAS_MANAGER), 0), path);
			_aliasManager->getAlertManager()->raiseError(error);
		}
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::saveAlias(QString path) const
	{
		POINTER_UNAVAILABLE(_aliasManager, );

		QFile *file = new QFile(path);
		_aliasManager->saveAlias(file);
		delete file;
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::addAlias(QString number, Enums::SwSourceType type, QString name)
	{
		POINTER_UNAVAILABLE(_aliasManager, );

		_aliasManager->addAlias(number, static_cast<unsigned int>(type), name);
		saveAutoAlias();
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::editAlias(QString number, Enums::SwSourceType type, QString name)
	{
		POINTER_UNAVAILABLE(_aliasManager, );

		_aliasManager->replaceAlias(number, static_cast<unsigned int>(type), name);
		saveAutoAlias();
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::addAliasToTn(gusa::model::interactionInterface::ITrack *track, QString name)
	{
		POINTER_UNAVAILABLE(_aliasManager, );

		QString number = getNumber(track, Enums::SwSourceType::TN);
		unsigned int type = static_cast<unsigned int>(Enums::SwSourceType::TN);

		_fastAction.append(AliasManager::Source(number, type));
		if (_aliasManager->isExist(number, type))
		{
			_aliasManager->replaceAlias(number, type, name);
		}
		else
		{
			_aliasManager->addAlias(number, type, name);
		}
		saveAliasFastAction();
		saveAutoAlias();
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::clearAlias()
	{
		POINTER_UNAVAILABLE(_aliasManager, );

		_fastAction.clear();
		_aliasManager->clearAlias();
		saveAliasFastAction();
		saveAutoAlias();
	}

	//---------------------------------------------------------------------------------
	QList<AliasManager::IAlias*> SwAliasManager::getAllAlias() const
	{
		POINTER_UNAVAILABLE(_aliasManager, QList<AliasManager::IAlias*>());

		return _aliasManager->getAllAlias();
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::removeAlias(QString number, Enums::SwSourceType type)
	{
		POINTER_UNAVAILABLE(_aliasManager, );

		_fastAction.removeAll(AliasManager::Source(number, static_cast<unsigned int>(type)));
		_aliasManager->removeAlias(number, static_cast<unsigned int>(type));
		saveAliasFastAction();
		saveAutoAlias();
	}

	//---------------------------------------------------------------------------------
	QList<Enums::SwSourceType> SwAliasManager::getPrimaryOrder() const
	{
		POINTER_UNAVAILABLE(_aliasManager, QList<Enums::SwSourceType>());

		return _primaryOrder;
	}

	//---------------------------------------------------------------------------------
	QList<Enums::SwSourceType> SwAliasManager::getSecondaryOrder() const
	{
		POINTER_UNAVAILABLE(_aliasManager, QList<Enums::SwSourceType>());

		return _secondaryOrder;
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::setPrimaryOrder(QList<Enums::SwSourceType> order)
	{
		POINTER_UNAVAILABLE(_aliasManager, );

		_primaryOrder = order;
		saveAliasOrder();
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::setSecondaryOrder(QList<Enums::SwSourceType> order)
	{
		POINTER_UNAVAILABLE(_aliasManager, );

		_secondaryOrder = order;
		saveAliasOrder();
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::onServiceAlertManagerAvailable()
	{
		_alertManager = _alertServiceHelper->getService();
		_aliasManager = new AliasManager::AliasManager(*(_alertManager->getAlertManager()), QString("C:/tt4/Libraries/Components/Sources/AliasManager/Source/Alias.xsd"));

		loadAliasOrder();
		loadAliasFastAction();
		loadAutoAlias();

		SW_APP->RegisterService(this);
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::loadAliasOrder()
	{
#define NO_ATTRIBUT QString("NO_NAME")
		POINTER_UNAVAILABLE(_aliasManager, );

		if (QFile::exists(_roamingPath + FILE_ALIAS_ORDER + "." + EXTENSION_FILE_ALIAS_ORDER))
		{
			QFile *file = new QFile(_roamingPath + FILE_ALIAS_ORDER + "." + EXTENSION_FILE_ALIAS_ORDER);
			if (file->open(QIODevice::ReadOnly | QIODevice::Text))
			{
				QDomDocument doc;
				if (doc.setContent(file))
				{
					QDomElement root = doc.documentElement();
					QDomElement primary = root.firstChildElement("primary");
					if (!primary.isNull())
					{
						for (QDomElement src = primary.firstChildElement("source"); !src.isNull(); src = src.nextSiblingElement("source"))
						{
							QString type = src.attribute("type", NO_ATTRIBUT);
							if (type.compare(NO_ATTRIBUT) != 0)
							{
								_primaryOrder.append(Enums::SwSourceType(type.toUInt()));
							}
						}
					}
					QDomElement secondary = root.firstChildElement("secondary");
					if (!secondary.isNull())
					{
						for (QDomElement src = secondary.firstChildElement("source"); !src.isNull(); src = src.nextSiblingElement("source"))
						{
							QString type = src.attribute("type", NO_ATTRIBUT);
							if (type.compare(NO_ATTRIBUT) != 0)
							{
								_primaryOrder.append(Enums::SwSourceType(type.toUInt()));
							}
						}
					}
				}
				file->close();
			}
			delete file;
		}
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::loadAutoAlias()
	{
		POINTER_UNAVAILABLE(_aliasManager, );

		if (QFile::exists(_roamingPath + FILE_AUTO_ALIAS + "." + EXTENSION_FILE_ALIAS))
		{
			loadAlias(_roamingPath + FILE_AUTO_ALIAS + "." + EXTENSION_FILE_ALIAS);
		}
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::loadAliasFastAction()
	{
#define NO_ATTRIBUT QString("NO_NAME")
		POINTER_UNAVAILABLE(_aliasManager, );

		if (QFile::exists(_roamingPath + FILE_FAST_ACTION + "." + EXTENSION_FILE_FAST_ACTION))
		{
			QFile *file = new QFile(_roamingPath + FILE_FAST_ACTION + "." + EXTENSION_FILE_FAST_ACTION);
			if (file->open(QIODevice::ReadOnly | QIODevice::Text))
			{
				QDomDocument doc;
				if (doc.setContent(file))
				{
					QDomElement root = doc.documentElement();
					QDomElement fastAction = root.firstChildElement("fastAction");

					while (!fastAction.isNull())
					{
						QString number = fastAction.attribute("id", NO_ATTRIBUT);
						QString type = fastAction.attribute("type", NO_ATTRIBUT);

						// Si on a trouvé tous les attributs, on peut créer la source
						if (number.compare(NO_ATTRIBUT) != 0 && type.compare(NO_ATTRIBUT) != 0)
						{
							_fastAction.append(AliasManager::Source(number, type.toUInt()));
						}
						fastAction = fastAction.nextSiblingElement("fastAction");
					}
				}
				file->close();
			}
			delete file;
		}
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::saveAliasOrder() const
	{
		POINTER_UNAVAILABLE(_aliasManager, );

		QFile *file = new QFile(_roamingPath + FILE_ALIAS_ORDER + "." + EXTENSION_FILE_ALIAS_ORDER);
		if (file->open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QDomDocument doc;
			QDomElement root = doc.createElement("root");

			QDomElement primary = doc.createElement("primary");
			for (QList<Enums::SwSourceType>::const_iterator ite = _primaryOrder.begin(); ite != _primaryOrder.end(); ++ite)
			{
				QDomElement src = doc.createElement("source");
				src.setAttribute("type", static_cast<unsigned int>(*ite));
				primary.appendChild(src);
			}
			root.appendChild(primary);

			QDomElement secondary = doc.createElement("secondary");
			for (QList<Enums::SwSourceType>::const_iterator ite = _secondaryOrder.begin(); ite != _secondaryOrder.end(); ++ite)
			{
				QDomElement src = doc.createElement("source");
				src.setAttribute("type", static_cast<unsigned int>(*ite));
				secondary.appendChild(src);
			}
			root.appendChild(secondary);
			doc.appendChild(root);

			QTextStream ts(file);
			ts << doc.toString();
			file->close();
		}
		delete file;
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::saveAutoAlias() const
	{
		POINTER_UNAVAILABLE(_aliasManager, );

		saveAlias(_roamingPath + FILE_AUTO_ALIAS + "." + EXTENSION_FILE_ALIAS);
	}

	//---------------------------------------------------------------------------------
	void SwAliasManager::saveAliasFastAction() const
	{
		POINTER_UNAVAILABLE(_aliasManager, );

		QFile *file = new QFile(_roamingPath + FILE_FAST_ACTION + "." + EXTENSION_FILE_FAST_ACTION);
		if (file->open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QDomDocument doc;
			QDomElement root = doc.createElement("root");

			for (QList<AliasManager::Source>::const_iterator ite = _fastAction.begin(); ite != _fastAction.end(); ++ite)
			{
				QDomElement fastAction = doc.createElement("fastAction");
				fastAction.setAttribute("id", ite->getNumber());
				fastAction.setAttribute("type", ite->getType());
				root.appendChild(fastAction);
			}
			doc.appendChild(root);

			QTextStream ts(file);
			ts << doc.toString();
			file->close();
		}
		delete file;
	}

	//---------------------------------------------------------------------------------
	QString SwAliasManager::getNumber(gusa::model::interactionInterface::ITrack *track, Enums::SwSourceType type) const
	{
		QString result;
		switch (type)
		{
		case Enums::SwSourceType::TN:
			track->getId()->getL16TnValidator()->modelToHmi(track->getId()->getL16Tn(), result);
			break;
		case Enums::SwSourceType::VCS:
			track->getVoiceCallSignValidator()->modelToHmi(track->getVoiceCallSign(), result);
			break;
		case Enums::SwSourceType::IFF1:
			track->getIff()->getMode1Validator()->modelToHmi(track->getIff()->getMode1(), result);
			break;
		case Enums::SwSourceType::IFF2:
			track->getIff()->getMode2Validator()->modelToHmi(track->getIff()->getMode2(), result);
			break;
		case Enums::SwSourceType::IFF3:
			track->getIff()->getMode3Validator()->modelToHmi(track->getIff()->getMode3(), result);
			break;
		default:
			break;
		}

		return result;
	}
}  // namespace SwServiceAlias