#ifndef SWDOCKWIDGET_DOCKAREA_H
#define SWDOCKWIDGET_DOCKAREA_H

#include <QtWidgets>
#include <QtXml/QDomDocument>
#include <QCloseEvent>
#include <QMenuBar>
#include <QPropertyAnimation>

#include "SwDockWidget_Splitter.h"
#include "ui_SwDockWidget_MainArea.h"
#include "SwDockWidget_Overlay.h"
#include "SwDockWidget_ToolBarWindow.h"
#include "SwDockWidget_MainDockWidget.h"
#include "SwDockWidget_MainDockConfiguration.h"

class SwDockWidget_DockWidget;

//-----------------------------------------------------------------------------
//Classe principale
//-----------------------------------------------------------------------------
class SwDockWidget_MainArea :public SwDockWidget_ToolBarWindow
{
	Q_OBJECT

public:
	SwDockWidget_MainArea(QWidget *parent = 0, QMenuBar * menuBar = 0);
	~SwDockWidget_MainArea();
	
	//Ajout d'un dock
	void addDockWidget(SwDockWidget_DockWidget * dock, QString menuName = "");

	//Suppression d'un dock
	void removeDockWidget(SwDockWidget_DockWidget * dock);
	//Suppression d'un menu
	void removeMenu(QString menu);

	//Enregistrement du widget principal
	void setMainWidget(QWidget * widget, bool quitOnClose);
	QWidget * getMainWidget();

	//Ancrage d'un dock
	QWidget * pinDockTo(QObject * obj, QWidget * mainWidget, Qt::DockWidgetArea area, bool absolute = false);	

	//Chargement des tailles et positions
	void loadDockPosition(QString filePath);

	//Verrouillage de la disposition des docks
	void lock();
	void releaseLock();
	
	//Fermeture
	bool close();

	//Parametre de la sauvegarde de la configuration
	void setConfigurationFileName(QString name, bool apply = false);
	QString getConfigurationFileName();
	void setSaveAutoPeriod(unsigned int value);

protected:
	//Evenements
	bool eventFilter(QObject *obj , QEvent * event);
	virtual void closeEvent(QCloseEvent * event );
	virtual bool event(QEvent * event );

protected slots:
	//Liberation d'un dock
	void releaseDock(QObject *obj = NULL);
	void switchWidgetStateFromMenu(bool visible);

	//Sauvegarde des tailles et positions
	void saveAllPositions();

	void changeDockConfiguration(SwDockWidget_MainDockConfiguration::ConfigurationIndex index);
	void lockConf(bool state);

	//void freeMainTab(int index, QPoint pos);
	//void moveMainTab(QPoint pos);
	//void stopMovingMainTab();
	void moveMainTab(int index, Qt::DockWidgetArea area);
	
	void setLock(bool state);


private:
	//Initialisation
	void init();

	//Gestion de la barre de titre
	void setMenu();
	void addWidgetInMenu(SwDockWidget_DockWidget * widget, QString menuName = "");
	void createMainWidgetAction(SwDockWidget_DockWidget * dock, QString iconPath, QString disabledIcon, QString checkedIcon);
	void updateConfiguration();
	QString checkConfiguration(QSplitter * splitter);
	QString getStringFromMainDock(SwDockWidget_MainDockWidget * dock);

	//Minimisation / Maximisation de l'application
	void hideAll();

	//Chargement des tailles et positions
	void savePosition(QDomDocument doc, QDomElement dom, QWidget * widget);
	QDomElement writeWidgetParameters(QDomDocument doc, QDomElement dom, QWidget * widget);
	QWidget * readWidgetParameters(QDomNode node);
	
	//Fleches de positionnement
	void createArrowsButton();
	void addButton(QPushButton ** button, int size, QIcon icon);
	void manageArrows(QObject * obj);
	void showArrows(QWidget * widget);
	void showArrowsOnEmptyMainDock(QWidget * widget);
	void showArrowsOnSecondScreen(QRect rect);
	void hideArrows(bool withOverlay = true);
	void highlightArrows(QPoint pos, bool toMain);
	
	//Ancrage d'un dock
	QWidget * managePinDock(QObject * dock, QWidget * parent);
	void updateMainDock();
	
	//Gestion du survol des widgets
	QWidget * getDockableWidget();
	QPoint getPosition(QWidget * widget);

	//Zone de pre-positionnement
	void showOverlay(Qt::DockWidgetArea area, bool toMain = false);
	void hideOverlay();
	void setupOverlay(QObject * obj, QWidget * widget);
	void setupEmptyMainDockOverlay(QWidget * widget);

	//Gestion des docks principaux
	SwDockWidget_MainDockWidget * setupMainDockWidget(QString name, Qt::DockWidgetArea area, bool withAddButton = true);

private:
	Ui::DockArea ui;

	//bool _lock; //Lock configuration
	QAction * _lockAction;
	QString _configurationFileName;
	
	//Liste des DockWidgets
	QList<QObject*> _list;	
	//Liste des docks principaux
	QList<QObject*> _listMainDock;
	
	//Dock principaux
	SwDockWidget_MainDockWidget * _leftMainDock;
	SwDockWidget_MainDockWidget * _rightMainDock;
	SwDockWidget_MainDockWidget * _topMainDock;
	SwDockWidget_MainDockWidget * _bottomMainDock;
	SwDockWidget_MainDockWidget * _secondScreenMainDock;
	SwDockWidget_MainDockWidget * _tempDock; //Utilise pour le deplacement d'un onglet
	//Effet
	QPropertyAnimation * _posEffect;

	//Boutons de redimensionnement
	QPushButton * _centerWidgetBtn;
	QPushButton * _leftWidgetBtn;
	QPushButton * _topWidgetBtn;
	QPushButton * _rightWidgetBtn;
	QPushButton * _bottomWidgetBtn;
	QPushButton * _tabBtn;
	QPushButton *  _secondScreenBtn;
	QPushButton * _leftCentralBtn;
	QPushButton * _topCentralBtn;
	QPushButton * _rightCentralBtn;
	QPushButton * _bottomCentralBtn;
	QPushButton * _centerDockBtn;

	//Menu
	QMenuBar * _menuBar;
	QMenu * _widgetMenu;
	QMap<QString, QMenu*> _listSousMenu;
	SwDockWidget_MainDockConfiguration * _mainDockConf;
	QWidget * _emptyWidget;

	bool _isMovingDock; //Si un DockWidget est en train d'etre deplace
	QString _movingDock; //Nom du DockWidget en train d'etre deplace
	bool _isReleasing; //Si un DockWidget est en train d'etre libere

	//Zone de prepositionnement
	SwDockWidget_Overlay *_overlay;

	//Periode de sauvegarde automatique des parametres
	QTimer * _saveTimer;
	unsigned int _saveAutoPeriod;

	bool _quitOnClose;
};

#endif
