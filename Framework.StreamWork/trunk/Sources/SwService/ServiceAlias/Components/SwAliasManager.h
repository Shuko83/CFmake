#pragma once

/*
* INCLUDES LOCAUX
*/
#include "ISwAliasManager.h"

/*
* INCLUDES GLOBAUX
*/
#include "ISwServiceAlert.h"
#include "SwServiceManager_Helper.h"
#include "AliasManager.h"
#include "SwFileDescriptor.h"

/**
 * @namespace	SwServiceAlias
 * @brief		Namespace de définition du service de gestion des alias
 **/
namespace SwServiceAlias
{
	/**
	 * @def		POINTER_UNAVAILABLE
	 * @brief	Macro qui vérifie que le pointeur @b ptr n'est pas null sinon il sort de la fonction appelante et retourne la valeur @b result
	 **/
#define POINTER_UNAVAILABLE(ptr, result) if (ptr == nullptr) return result

	/**
	 * @def		FILE_ALIAS_ORDER
	 * @brief	Nom du fichier utilisé pour la sauvegarde de l'ordre des alias
	 **/
#define FILE_ALIAS_ORDER	QString("orderAlias")

	/**
	 * @def		FILE_AUTO_ALIAS
	 * @brief	Nom du fichier utilisé pour la sauvegarde automatique des alias
	 **/
#define FILE_AUTO_ALIAS		QString("autosaveAlias")

	/**
	 * @def		FILE_FAST_ACTION
	 * @brief	Nom du fichier utilisé pour la sauvegarde de la liste des actions rapides sur les alias
	 **/
#define FILE_FAST_ACTION	QString("fastActionAlias")

	/**
	 * @def		EXTENSION_FILE_ALIAS_ORDER
	 * @brief	Définition de l'extension à utiliser pour la sauvegarde et le chargement de l'ordre des alias dans le service
	 **/
#define EXTENSION_FILE_ALIAS_ORDER	QString("oal")

	/**
	 * @def		EXTENSTION_FILE_FAST_ACTION
	 * @brief	Définition de l'extension à utiliser pour la sauvegarde et le chargement de la liste des actions rapides sur les alias
	 **/
#define EXTENSION_FILE_FAST_ACTION	QString("faal")

	/**
	 * @class	SwAliasManager	SwAliasManager.h	"C:\tt4\Framework\StreamWork\Sources\SwService\SwServiceAlias\Components\SwAliasManager.h"
	 * @brief	Classe de définition du service d'alias pour Starlinx
	 *
	 * @sa	SwServiceAlias::ISwAliasManager
	 **/
	class SwAliasManager : public QObject, public ISwAliasManager
	{
		Q_OBJECT;

	public:
		/**
		 * @fn		SwServiceAlias::SwAliasManager::SwAliasManager()
		 * @brief	Constructeur par défaut
		 **/
		SwAliasManager();

		/**
		 * @fn		virtual  SwServiceAlias::SwAliasManager::~SwAliasManager()
		 * @brief	Destructeur par défaut
		 **/
		virtual ~SwAliasManager();

		/**
		 * @fn		QT_NAMESPACE::QString SwServiceAlias::SwAliasManager::getPrimaryAlias(gusa::model::interactionInterface::ITrack *track)
		 * @brief	Récupérer l'alias principal lié à la piste indiquée en prenant en compte l'ordre de priorité des alias
		 *
		 * @param	track	Une piste
		 *
		 * @return	QString	L'alias lié à la piste
		 *
		 * @sa	SwServiceAlias::ISxAliasUser::getPrimaryAlias(gusa::model::interactionInterface::ITrack*)
		 **/
		QString getPrimaryAlias(gusa::model::interactionInterface::ITrack *track) const override;

		/**
		 * @fn		QString SwServiceAlias::SwAliasManager::getSecondaryAlias(gusa::model::interactionInterface::ITrack *track)
		 * @brief	Récupérer l'alias secondaire lié à la piste indiquée en prenant en compte l'ordre de priorité des alias
		 *
		 * @param	track	Une piste
		 *
		 * @return	QString	L'alias lié à la piste
		 *
		 * @sa	SwServiceAlias::ISxAliasUser::getSecondaryAlias(gusa::model::interactionInterface::ITrack*)
		 **/
		QString getSecondaryAlias(gusa::model::interactionInterface::ITrack *track) const override;

		/**
		 * @fn		bool SwServiceAlias::SwAliasManager::isExist(gusa::model::interactionInterface::ITrack *track)
		 * @brief	Indiquer si un alias existe pour la piste indiquée
		 *
		 * @param	track	Une piste
		 *
		 * @return	bool	@b True si un alias existe, sinon @b False
		 *
		 * @sa	SwServiceAlias::ISxAliasUser::isExist(gusa::model::interactionInterface::ITrack*)
		 **/
		bool isExist(gusa::model::interactionInterface::ITrack *track) const override;

		/**
		 * @fn		bool SwServiceAlias::SwAliasManager::isExist(QString number, Enums::SwSourceType type)
		 * @brief	Indiquer si un alias existe pour l'identifiant de piste indiqué
		 *
		 * @param	number	L'identifiant ou la plage d'identifiant de la piste
		 * @param	type	Le type d'identifiant
		 *
		 * @return	bool	@b True si un alias existe, sinon @b False
		 *
		 * @sa	SwServiceAlias::ISxAliasEditor::isExist(QString, Enums::SwSourceType)
		 **/
		bool isExist(QString number, Enums::SwSourceType type) const override;

		/**
		 * @fn		void SwServiceAlias::SwAliasManager::registerObserver(AliasManager::IAliasObserver *obs)
		 * @brief	Inscrire l'observateur indiqué
		 *
		 * @param	obs	L'observateur à inscrire
		 *
		 * @sa	SwServiceAlias::ISxAliasUser::registerObserver(IAliasObserver*)
		 **/
		void registerObserver(AliasManager::IAliasObserver *obs) override;

		/**
		 * @fn		void SwServiceAlias::SwAliasManager::unregisterObserver(AliasManager::IAliasObserver *obs)
		 * @brief	Désinscrire l'observateur indiqué
		 *
		 * @param	obs	L'observateur à désinscrire
		 *
		 * @sa	SwServiceAlias::ISxAliasUser::unregisterObserver(IAliasObserver*)
		 **/
		void unregisterObserver(AliasManager::IAliasObserver *obs) override;

		/**
		 * @fn		QString SwServiceAlias::SwAliasManager::getXmlSchemaPath()
		 * @brief	Récupérer le chemin absolu vers le fichier XSD
		 *
		 * @return	QString	QString	Chemin absolu vers le fichier XSD
		 *
		 * @sa	SwServiceAlias::ISxAliasSaver::getXmlSchemaPath()
		 **/
		QString getXmlSchemaPath() const override;

		/**
		* @fn		QString SwServiceAlias::SwAliasManager::getRoamingPath()
		* @brief	Retourner le chemin du dossier de sauvegarde utilisé par le service
		*
		* @return	QString	Le chemin du dossier de sauvegarde
		*
		* @sa	SwServiceAlias::ISwAliasSaver::getRoamingPath()
		**/
		QString getRoamingPath() const override;

		/**
		 * @fn		void SwServiceAlias::SwAliasManager::setXmlSchemaPath(QString path)
		 * @brief	Définir le chemin absolu vers le fichier XSD
		 *
		 * @param	path	Chemin absolu vers le fichier XSD
		 *
		 * @sa	SwServiceAlias::ISxAliasSaver::setXmlSchemaPath(QString)
		 **/
		void setXmlSchemaPath(QString path) override;

		/**
		 * @fn		void SwServiceAlias::SwAliasManager::loadAlias(QString path)
		 * @brief	Charger des alias dans le service depuis un fichier
		 *
		 * @param	path	Chemin absolu vers le fichier
		 *
		 * @sa	SwServiceAlias::ISxAliasSaver::loadAlias(QString)
		 **/
		void loadAlias(QString path) override;

		/**
		 * @fn		void SwServiceAlias::SwAliasManager::loadAliasFromATO(QString path)
		 * @brief	Charger des alias dans le service depuis un fichier ATO
		 *
		 * @param	path	Chemin absolu vers le fichier ATO
		 *
		 * @sa	SwServiceAlias::ISwAliasSaver::loadAliasFromATO(QString)
		 **/
		void loadAliasFromATO(QString path) override;

		/**
		 * @fn		void SwServiceAlias::SwAliasManager::saveAlias(QString path)
		 * @brief	Sauvegarder les alias du service dans un fichier
		 *
		 * @param	path	Chemin absolu vers le fichier
		 *
		 * @sa	SwServiceAlias::ISxAliasSaver::saveAlias(QString)
		 **/
		void saveAlias(QString path) const override;

		/**
		 * @fn		void SwServiceAlias::SwAliasManager::addAlias(QString number, Enums::SwSourceType type, QString name)
		 * @brief	Ajouter un alias dans le service en complétant l'identifiant si besoin
		 *
		 * @param	number	L'identifiant ou la plage d'identifiant de la piste
		 * @param	type	Le type d'identifiant
		 * @param	name	Le nom de l'alias
		 *
		 * @sa	SwServiceAlias::ISxAliasEditor::addAlias(QString, Enums::SwSourceType, QString)
		 **/
		void addAlias(QString number, Enums::SwSourceType type, QString name) override;

		/**
		 * @fn		void SwServiceAlias::SwAliasManager::editAlias(QString number, Enums::SwSourceType type, QString name)
		 * @brief	Modifier un alias dans le service en complétant l'identifiant si besoin
		 *
		 * @param	number	L'identifiant ou la plage d'identifiant de la piste
		 * @param	type	Le type d'identifiant
		 * @param	name	Le nom de l'alias
		 *
		 * @sa	SwServiceAlias::ISwAliasEditor::editAlias(QString, Enums::SwSourceType, QString)
		 **/
		void editAlias(QString number, Enums::SwSourceType type, QString name) override;

		/**
		 * @fn		void SwServiceAlias::SwAliasManager::addAliasToTn(const gusa::model::interactionInterface::ITrack *track, QString name)
		 * @brief	Ajouter un alias, liés au TN, dans le service (utilisé seulement en fastAction)
		 *
		 * @param	track	La piste de l'alias
		 * @param	name	Le nom de l'alias
		 *
		 * @sa	SwServiceAlias::ISxAliasEditor::addAliasToTn(gusa::model::interactionInterface::ITrack*, QString)
		 **/
		void addAliasToTn(gusa::model::interactionInterface::ITrack *track, QString name) override;

		/**
		 * @fn		void SwServiceAlias::SwAliasManager::clearAlias()
		 * @brief	Supprimer tous les alias présents dans le service
		 *
		 * @sa	SwServiceAlias::ISxAliasEditor::clearAlias()
		 **/
		void clearAlias() override;

		/**
		 * @fn		QList<AliasManager::IAlias*> SwServiceAlias::SwAliasManager::getAllAlias()
		 * @brief	Récupérer tous les alias présents dans le service
		 *
		 * @return	QList<AliasManager::IAlias*>	Liste de tous les alias du service
		 *
		 * @sa	SwServiceAlias::ISxAliasEditor::getAllAlias()
		 **/
		QList<AliasManager::IAlias*> getAllAlias() const override;

		/**
		 * @fn		void SwServiceAlias::SwAliasManager::removeAlias(QString number, Enums::SwSourceType type)
		 * @brief	Supprimer un alias du service
		 *
		 * @param	number	L'identifiant ou la plage d'identifiant de la piste
		 * @param	type	Le type d'identifiant
		 *
		 * @sa	SwServiceAlias::ISxAliasEditor::removeAlias(QString, Enums::SwSourceType)
		 **/
		void removeAlias(QString number, Enums::SwSourceType type) override;

		/**
		 * @fn		void SwServiceAlias::SwAliasManager::setPrimaryOrder(QList<Enums::SwSourceType> order)
		 * @brief	Définir l'ordre d'utilisation des alias pour l'alias principal
		 *
		 * @param	order	Liste des types de source à utiliser
		 *
		 * @sa	SwServiceAlias::ISxAliasEditor::setPrimaryOrder(QList<Enums::SwSourceType>)
		 **/
		void setPrimaryOrder(QList<Enums::SwSourceType> order) override;

		/**
		 * @fn		QList<Enums::SwSourceType> SwServiceAlias::SwAliasManager::getPrimaryOrder()
		 * @brief	Récupérer l'ordre d'utilisation des alias pour l'alias principal
		 *
		 * @return	QList<Enums::SwSourceType>	Liste des types de source pour l'alias principal
		 *
		 * @sa	SwServiceAlias::ISxAliasEditor::getPrimaryOrder()
		 **/
		QList<Enums::SwSourceType> getPrimaryOrder() const override;

		/**
		 * @fn		QList<Enums::SwSourceType> SwServiceAlias::SwAliasManager::setSecondaryOrder()
		 * @brief	Récupérer l'ordre d'utilisation des alias pour l'alias secondaire
		 *
		 * @return	QList<Enums::SwSourceType>	Liste des types de source pour l'alias secondaire
		 *
		 * @sa	SwServiceAlias::ISxAliasEditor::setSecondaryOrder()
		 **/
		QList<Enums::SwSourceType> getSecondaryOrder() const override;

		/**
		 * @fn		void SwServiceAlias::SwAliasManager::setSecondaryOrder(QList<Enums::SwSourceType> order)
		 * @brief	Définir l'ordre d'utilisation des alias pour l'alias secondaire
		 *
		 * @param	order	Liste des types de source à utiliser
		 *
		 * @sa	SwServiceAlias::ISxAliasEditor::setSecondaryOrder(QList<Enums::SwSourceType>)
		 **/
		void setSecondaryOrder(QList<Enums::SwSourceType> order) override;

		/**
		* @fn		void SwServiceAlias::SwAliasManager::onServiceAlertManagerAvailable()
		* @brief	Callback lors de la disponibilité du service d'alerte
		**/
		void onServiceAlertManagerAvailable(bool available);

		StreamWork::SwCore::SwFileDescriptor getXsdFile() const { return _xsdFile; }
		void setXsdFile(StreamWork::SwCore::SwFileDescriptor val) { _xsdFile = val; }

	private:
		/**
		 * @fn		void SwServiceAlias::SwAliasManager::loadAliasOrder()
		 * @brief	Charger l'ordre d'utilisation des alias
		 **/
		void loadAliasOrder();

		/**
		 * @fn		void SwServiceAlias::SwAliasManager::loadAutoAlias()
		 * @brief	Charger automatiquement les alias à partir d'un fichier défini dans la configuration
		 **/
		void loadAutoAlias();

		/**
		 * @fn		void SwServiceAlias::SwAliasManager::loadAliasFastAction()
		 * @brief	Charger la liste des actions rapides sur les alias
		 **/
		void loadAliasFastAction();

		/**
		 * @fn		void SwServiceAlias::SwAliasManager::saveAliasOrder()
		 * @brief	Sauvegarder l'ordre d'utilisation des alias
		 **/
		void saveAliasOrder() const;

		/**
		 * @fn		void SwServiceAlias::SwAliasManager::saveAutoAlias()
		 * @brief	Sauvegarder automatiquement les alias dans un fichier défini dans la configuration
		 **/
		void saveAutoAlias() const;

		/**
		 * @fn		void SwServiceAlias::SwAliasManager::saveAliasFastAction()
		 * @brief	Sauvegarder la liste des actions rapides sur les alias
		 **/
		void saveAliasFastAction() const;

		/**
		 * @fn		QT_NAMESPACE::QString SwServiceAlias::SwAliasManager::getNumber(gusa::model::interactionInterface::ITrack *track, Enums::SwSourceType type)
		 * @brief	Récupérer un identifiant, s'il existe, dans une piste
		 *
		 * @param	track	La piste
		 * @param	type	Le type d'identifiant recherché
		 * @return	QString	L'identification recherché
		 **/
		QString getNumber(gusa::model::interactionInterface::ITrack *track, Enums::SwSourceType type) const;

		/**
		 * @var		_aliasManager
		 * @brief	Le manageur générique d'alias
		 **/
		AliasManager::AliasManager *_aliasManager;

		/**
		 * @var		_fastAction
		 * @brief	Liste des sources sur lequel a été effectué un fast action
		 **/
		QList<AliasManager::Source> _fastAction;

		/**
		 * @var		_primaryOrder
		 * @brief	Liste de l'ordre d'utilisation des alias pour l'alias principal
		 **/
		QList<Enums::SwSourceType> _primaryOrder;

		/**
		 * @var		_secondaryOrder
		 * @brief	Liste de l'ordre d'utilisation des alias pour l'alias secondaire
		 **/
		QList<Enums::SwSourceType> _secondaryOrder;

		/**
		 * @var		_alertServiceHelper
		 * @brief	Pointeur de gestion de la disponibilité du service d'alerte
		 **/
		SwServiceManager_Helper<StreamWork::SwCore::ISwServiceAlert> *_alertServiceHelper;
		/**
		 * @var		_roamingPath
		 * @brief	Chemin de sauvegarde des informations du service (alias et ordre)
		 **/
		QString _roamingPath;

		StreamWork::SwCore::SwFileDescriptor _xsdFile;
	};
}  // namespace SwServiceAlias