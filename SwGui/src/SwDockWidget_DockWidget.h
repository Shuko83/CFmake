#ifndef SWDOCKWIDGET_DOCKWIDGET_H
#define SWDOCKWIDGET_DOCKWIDGET_H

#include <QtGlobal>

#include <QtWidgets>

#include <QTimer>

namespace Ui {
    class DockWidget;
}

class SwDockWidget_DockWidget : public QWidget
{
	Q_OBJECT

	Q_PROPERTY (bool _canBeClose READ canBeClose WRITE setCanBeClose);
	Q_PROPERTY (bool _canBeMoved READ canMove WRITE setCanBeMoved);
	Q_PROPERTY (bool _canBePin READ canBePin WRITE setCanBePin);
	Q_PROPERTY (bool _canBeResize READ canBeResize WRITE setCanBeResize);
	Q_PROPERTY (bool _canBeOutside READ canBeOutside WRITE setCanBeOutside);
	Q_PROPERTY (bool _alwaysOnTop READ isAlwaysOnTop WRITE setAlwaysOnTop);

public:
	SwDockWidget_DockWidget(QWidget *parent = 0);
	~SwDockWidget_DockWidget();

	virtual void setWidget(QWidget * widget);
	QWidget * getWidget();

	void setTitle(QString title);
	QString getTitle();

	void setMainArea(QWidget * area);
	QWidget * getMainArea();

	//Options
	bool canBeResize();
	bool canBeClose();
	bool canBePin();
	bool canMove();
	bool canBeOutside();
	bool isAlwaysOnTop();

	void setCanBeResize(bool state);
	void setCanBeClose(bool state);
	void setCanBePin(bool state);
	void setCanBeMoved(bool state);
	void setCanBeOutside(bool state);
	void setAlwaysOnTop(bool state);

	//Evenements
	void parentResized();

	//Passage en mode "deplacement"
	void setInitialMoving(QPoint pos);
	void setMoving(bool state);
	bool isMoving();
	void move(int x, int y);
	void move(const QPoint & pos);

	//Parametrage de la taille apres liberation du SwDockWidget_DockWidget
	void undockedSize(const QSize & size);
	void setRawSize(const QSize & size);
	void setFreeSize(QSize size);
	QSize getRawSize();
	QSize getFreeSize();
	void restoreSize();

	//Accesseurs
	bool isInTab();
	bool isInToolBar();
	Qt::DockWidgetArea getArea();
	QWidget * getTabWidget();
	QWidget * getToolBarItem();
	QAction * getMenuAction();
	QString getToolbarItemName();

	//Mutateurs
	void returnToMainArea();
	void setInTab(bool state);
	void setInToolBar(bool state, QWidget * toolBarItem = NULL);
	void setArea(Qt::DockWidgetArea area);
	void setTabWidget(QWidget * widget);
	virtual void lock();
	virtual void releaseLock();
	void setToolBarItem(QWidget * toolbar);
	void setMenuAction(QAction * action);
	void setToolbarItemName(QString name);

	//Autre options
	void hideFrame();
	void showFrame();
	void removeSpacer();
	void showShadow(int area);
	void hideShadow();
	int getShadowSize();

	//Interface ISwDockWidget
	SwDockWidget_DockWidget & GetDockWidget();

protected:
	virtual void mouseReleaseEvent( QMouseEvent * event );
	virtual void mousePressEvent( QMouseEvent * event );
	virtual void mouseMoveEvent( QMouseEvent * event );
	virtual bool event( QEvent * event );
	void showShadow();
	
signals:
	void releaseDockAsked();
	void reduceInToolBarAsked();
	void releaseFromToolBarAsked();

public slots:
	void buttonsSlots();
	void blink();

private:
	bool manageResize(QMouseEvent * event);
	void setupShadow(QColor color = QColor(109, 183, 255, 120));
	void setCursorAspect();
	void setTextToLabel( QLabel *label, QString text );
	void updateBtn();
	QPoint getAdjustedPosition(int x, int y);


protected:
	QWidget * _widget; //Widget contenu dans le SwDockWidget_DockWidget
	Ui::DockWidget* ui;

private:
	QString _title; //Titre du Dock
	QWidget * _emptyWidget;
	QString _toolBarItemName; //Nom affiche dans la toolbar

	//Options du dock
	bool _canBeClose; //Fermeture autorisee
	bool _canBeMoved; //Deplacement autorise
	bool _canBePin; //Ancrage autorise
	bool _canBeResize; //Redimensionnement autorise
	bool _canBeOutside; //Deplacement en dehors de l'ecran autorise
	bool _alwaysOnTop ; //Garder au premier plan

	bool _canMove; //Deplacement initie
	bool _couldBePin; //Etat precedent de l'autorisation d'ancrage

	bool _lock; //Verrouillage de la disposition et configuration
	bool _isResizing;
	bool _inTabWidget; //SwDockWidget_DockWidget dans un TabWidget
	bool _inToolBar; //Dans une toolbar

	QPoint _clickPos; //Position du curseur lors du clic
	QWidget * _mainArea; //MainArea contenant le SwDockWidget_DockWidget
	
	Qt::DockWidgetArea _area; //Zone d'attachement du SwDockWidget_DockWidget
	Qt::DockWidgetArea _isMinResizing; //Cote en cours de redimensionnement
	int _shownShadow; //Ombres affichees

	QWidget * _tabWidget; //TabWidget auquel est attache le SwDockWidget_DockWidget
	QWidget * _toolBarItem; //Bouton du dock dans la toolBar
	QAction * _action; //Action permettant d'ouvrir ou fermer le dock depuis un menu

	QSize _freeSize; //Taille du dock lorsqu'il est libre

	int _blinkStatus;
	QTimer * _timer;

	QColor _shadowColor;
};

#endif
