#ifndef __SWCONFIGURATIONMANAGERGUI_H
#define __SWCONFIGURATIONMANAGERGUI_H

#include <QWidget>
#include <ISwConfigurationEditor.h>
#include <_ISwConfigurationLaunchEditor.h>
#include <_SwManagedConfiguration.h>
#include <_ISwManagedConfigurationsList.h>
#include <_ISwConfigurationLaunchManager.h>
#include <ISwConfigurationController.h>
#include <ISwConfigurationControllerListener.h>
#include <QModelIndex>
#include "ui__SwConfigurationManagerGui.h"

using namespace StreamWork::SwConfiguration;

class _SwConfigurationManagerGui : 
    public QWidget,
    public ISwConfigurationEditor,
    public ISwConfigurationLaunchEditor,
    public ISwConfigurationControllerListener
{
    Q_OBJECT

public:
    _SwConfigurationManagerGui(QWidget *parent = 0);
    ~_SwConfigurationManagerGui();
    
    void setConfigurationController (ISwConfigurationController * controller);
    void setEdition(bool enabled);
    // configuration properties editor
    virtual void setManagedConfigurationsModel(_ISwManagedConfigurationsList * model);

    virtual void onConfigurationListChanged();

    virtual void onCurrentConfigurationChanged();

    virtual void onCurrentConfigurationLoaded();

    virtual void onCurrentConfigurationSaved();
    
    

    // launch properties editor
    virtual void setLaunchConfigurationModel(_ISwConfigurationLaunchManager * model);
    
    virtual void startModeUpdated();

    virtual void autoStartUpdated();

    virtual void configurationFileNameUpdated();

    virtual void recordActivatedUpdated();

    virtual void recordDirectoryUpdated();

    virtual void replayDirectoryUpdated();



    void displayMessageBox(QString title, QString text);

    void setStarted();

    void setStopped();


    /** @brief sur changement d etat*/
    virtual void OnStartedChanged(ISwConfigurationController *);

    /** @brief sur changement d etat*/
    virtual void OnConfigurationEditedChanged(ISwConfigurationController *);

    virtual void setMonoConfiguration(bool value) ;


protected:
bool askForStop();    
virtual void setConfigurationsList(QList<_SwManagedConfiguration *> & configList);    
void setConfigurationEdited(bool value);
private:
    Ui::_SwConfigurationManagerGuiClass ui;
    QList<_SwManagedConfiguration *>                _configList;
    _ISwManagedConfigurationsList *                 _configurations;
    _ISwConfigurationLaunchManager *                _launchConfiguration;
    ISwConfigurationController *                   _controller;
    bool                                           _treeviewClicked;
    QString                                        _configurationFileName;
    
    QString                                        _lastOpenedRecordPath;
    QString                                        _lastOpenedConfigPath;
    bool                                           _monoConfiguration;

private slots:
    void on_checkBoxRecord_clicked();
    void on_checkBoxAutoStart_clicked();
    void on_radioButtonReplay_clicked();
    void on_radioButtonNormal_clicked();
    void on_checkBoxRecord_stateChanged(int);
    void on_checkBoxAutoStart_stateChanged(int);
    void on_pushButtonStop_clicked();
    void on_pushButtonStart_clicked();
    void on_toolButtonLoadReplayFile_clicked();
    void on_toolButtonLoadRecordFile_clicked();
    void on_radioButtonReplay_toggled(bool);
    void on_radioButtonNormal_toggled(bool);
    void on_toolButtonSaveAsConfig_clicked();
    void on_toolButtonLoadConfig_clicked();
    void on_comboBox_activated(int);

    void onCurrentConfigModelChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
    void onCurrentConfigTreeViewClicked(const QModelIndex & );
};

Q_DECLARE_METATYPE(_SwManagedConfiguration *);

#endif // __SWCONFIGURATIONMANAGERGUI_H
