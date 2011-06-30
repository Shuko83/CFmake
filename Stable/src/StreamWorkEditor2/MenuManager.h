/**
@file MenuManager.h
@brief manager de menu
@author F.Bighelli
 */

#ifndef _MENUMANAGER_H
#define _MENUMANAGER_H

#include <QtGui>
#include <QtCore>

#include "StreamControler.h"
#include "LinkGraphicItem.h"
#include "InterestArea.h"
#include "ISwExecution_Service.h"
#include "ISwAdminSetup.h"
#include "ISwWidget.h"
/**
@class MenuManager
@brief manager de menu
*/
class MenuManager : public QObject{
    Q_OBJECT
public:
    /** @brief acces singleton */
    static MenuManager * getInstance();
    /** @brief Definition du controleur */
    void setControler(StreamControler * controler);
    /** @brief acces au controleur */
    StreamControler * getControler();
    /** @brief selection changed */
    void selectionChanged();
    /** @brief construction du menu en fonction du contexte */
    QMenu * buildContextMenu(const QPointF & pos);
    /** @brief Acces au menu de la fenetre principale */
    QMenu * getMenu();

public slots:
	/** @brief sur remove */
    void onRemove();
    /** @brief sur Connection */
    void onConnect();
    /** @brief sur Connection */
    void onCreateAndConnect();
    /** @brief sur deconnection */
    void onDisconnect();
	/** @brief sur start execution*/
    void onStartExecution();
	/** @brief sur stop execution*/
    void onStopExecution();
	/** @brief sur setup */
    void onSetup();
	/** @brief sur show */
    void onShow();
	/** @brief sur ajout de la zone d'interet */
    void onAddInterestArea();
	/** @brief activated */
    void onActivated();
	/** @brief activated */
    void onDeActivated();
	/** @brief on copy style */
    void onCopyStyle();
	/** @brief on paste style */
    void onPasteStyle();
    /** @brief on change background color */
    void onChangeBackGroundColor();
    /** @brief on jump */
    void onJumpToConnected();
private:
	/** @brief Constructor */
	MenuManager();
	/** @brief Destructor */
	~MenuManager();
	/** @brief rebuildMenu() */
	void rebuildMenu();
	/** @brief buildMenuForContext() */
    void buildMenuForContext(QMenu * menu);
    /** @brief construit le menu pour le connector */
    void buildMenuForConnector(QMenu * menu,ConnectorGraphicItem * connector);
    /** @brief build action name for citem */
    QString buildActionNameForConnector(ConnectorGraphicItem *citem);
private:
    /** @brief Menu */
    QMenu * _menu;
    /** @brief Menu contextuel*/
    QMenu * _contextualMenu;
    /** @brief Menu contextuel swap*/
    QMenu * _contextualMenuSwap;
    /** @brief Menu */
    StreamControler * _streamControler;
    /** @brief Liste des composants selectionnéés */
    QList<ComponentGraphicItem *> _gwList;
    /** @brief Liste des interest area selectionnéés */
    QList<InterestArea *> _iaList;
    /** @brief Liste des link selectionnes */
    QList<LinkGraphicItem *> _lkList;
    /** @brief Liste des executeurs */
    QList<StreamWork::SwExecution::ISwExecution_Service *> _exeList;
    /** @brief Liste des adminSetups */
    QList<StreamWork::SwCore::ISwAdminSetup *> _adminList;
	/** @brief Liste des composants ayant un ISwWidget*/
	QList<StreamWork::SwCore::ISwProperty *> _propertyList;
    /** @brief pour un connector */
    QList<ConnectorGraphicItem *> _connectorList;
    ConnectorGraphicItem *_selectedConnector;

    QList<ConnectorGraphicItem *> _connectableItems;
    /** @brief Descativation du changement de selection */
    bool _disableSelectionChanged;
    /** @brief menu position*/
    QPointF _menuPosition;
    /** @brief Le menu a besoin d'etre reconstruit*/
    bool menuNeedBeRebuild;
    /** @brief Color copied*/
    QColor _copyTextColor;
    QColor _copyBgColor;
};

#endif
