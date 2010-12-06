/**
@file MainWindow.h
@brief Fenetre principale
@author F.Bighelli
 */

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include <QtGui>
#include "StreamView.h"
#include "StreamControler.h"
#include "QStreamTreeModel.h"
#include "QInterestAreaTreeModel.h"
#include "SwWizardFrontEnd.h"
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
	MainWindow();

public slots:
    /** @brief sur new stream */
    void onNewStream();
    /** @brief sur load stream */
    void onLoadStream();
    /** @brief sur save stream */
    void onSaveStream();
    /** @brief sur save as stream */
    void onSaveAsStream();
    /** @brief sur wizard */
    void onWizard();
    /** @brief sur quit */
    //void onQuit();
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
protected:
    /** @brief sur close event */
    virtual void MainWindow::closeEvent(QCloseEvent *event);
private:
    /** @brief Graphics view */
    StreamView * _streamView;
    /** @brief Stream Controler */
    StreamControler * _streamControler;
    /** @brief Wizard */
    SwWizardFrontEnd * _wizard;
    /** @brief Model */
    QStreamTreeModel * _streamTreeModel;
    /** @brief Model */
    QInterestAreaTreeModel * _iaTreeModel;
    /** @brief PropertyWidget */
    PropertiesWidget *_propertyWidget;
    /** @brief status label */
    QLineEdit *_statusWidget;
};

#endif
