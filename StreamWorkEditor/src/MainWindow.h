/**
@file MainWindow.h
@brief Fenetre principale
@author F.Bighelli
 */

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include <QtCore>
#include <QtWidgets>
#include "StreamView.h"
#include "StreamControler.h"
#include "QStreamTreeModel.h"
#include "QInterestAreaTreeModel.h"
#include "PropertiesWidget.h"
#include "ISelectionObserver.h"

/**
@class MainWindow
@brief Fenetre principale
*/
class MainWindow : public QMainWindow , public ISelectionObserver{
    Q_OBJECT
public:
	/** @brief Constructor */
	MainWindow(QString streamFile = QString());
	virtual ~MainWindow();

    /** @brief sur load stream existant */
	void onLoadExistingStream(StreamWork::SwCore::SwComponent_Class * aStream,QString path,StreamWork::SwCore::SwComponent_Class * aStreamSource);
    /** @brief permet de voir l'editeur du stream a l'origine de l'ouverture de nous meme */
    void displayStreamSourceOpener();
    /** @brief getEditors */
	static QMap<StreamWork::SwCore::SwComponent_Class *,MainWindow *> * getEditors();

	void setIsMasterWindow(bool val);
public slots:
    /** @brief sur new stream */
    void onNewStream();
    /** @brief sur load stream */
    void onLoadStream();
	/** @brief sur load stream avec un fichier */
	void onLoadStreamf();
    /** @brief sur save stream */
    void onSaveStream();
    /** @brief sur save as stream */
    void onSaveAsStream();
    /** @brief sur wizard */
    void onWizard();
	/** @brief sur lancement SwGenerator */
	void onLaunchSwGenerator();
    /** @brief sur quit */
    void onQuit();
    /** @brief sur affichage de la liste des plugins */
    void onDisplayPluginList();
    /** @brief rearrange */
    void onRearrange();
    /** @brief sur nouvelle fenetre */
    void onNewWindow();
    /** @brief sur print */
    void onPrint();
    /** @brief sur selection */
    virtual void setSelection(QList<StreamWork::SwCore::SwComponent_Class *> & sel);
    /** @brief fermeture interne */
    virtual void internalClose();

	/** @brief clear des différents services */
	virtual void clearServices();

protected:
    /** @brief sur close event */
    virtual void closeEvent(QCloseEvent *event);
    /** @brief sur close event */
    virtual void focusInEvent ( QFocusEvent * event );

	void manageHistory(QFileInfo fi);

signals:
	void selection (const QString&, const QString&);
private:
    /** @brief Graphics view */
    StreamView * _streamView;
    /** @brief Stream Controler */
    StreamControler * _streamControler;
    /** @brief Model */
    QStreamTreeModel * _streamTreeModel;
    /** @brief Model */
    QInterestAreaTreeModel * _iaTreeModel;
    /** @brief PropertyWidget */
    PropertiesWidget *_propertyWidget;
    /** @brief status label */
    QLineEdit *_statusWidget;
    /** @brief stream source de l'ouverture de l'editeur */
	StreamWork::SwCore::SwComponent_Class * _streamSourceOpener;
	/** @brief History of open files */
	QStringList _history;

	QList<QAction *> _listOfActions;

	QMenu * _streamMenu;


	/** @brief To be able to clean other windows on close*/
	bool _isMasterWindow;

};

#endif
