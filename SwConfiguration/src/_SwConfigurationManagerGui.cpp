#include "_SwConfigurationManagerGui.h"
#include "SwGuiDefaultItemDelegate.h"
#include <QFileDialog>
#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QDomElement>
#include "Types/SwEnum.h"
#include <QMessageBox>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

_SwConfigurationManagerGui::_SwConfigurationManagerGui(QWidget *parent)
    : QWidget(parent)
{
    
    ui.setupUi(this);
    ui.treeView->setItemDelegate(new SwGuiDefaultItemDelegate(this));
    ui.treeView->setAlternatingRowColors(true); 
    _controller = 0;
    _launchConfiguration = 0;
    _configurations = 0;
    //_currentConfigEdited = false;
    _treeviewClicked = false;
    _configurationFileName = "";
    setStopped();
    _lastOpenedRecordPath = "";
    _lastOpenedConfigPath = "";
    setMonoConfiguration(false);
}


void   _SwConfigurationManagerGui::setMonoConfiguration(bool value)
{
    _monoConfiguration = value;
    ui.GroupBoxMode->setVisible(!_monoConfiguration);
    ui.pushButtonStart->setVisible(!_monoConfiguration);
    ui.pushButtonStop->setVisible(!_monoConfiguration);
    //ui.GroupBoxMode->setMinimumWidth(value?0:450);
    ui.widget_2->setMinimumWidth(value?0:450);
    ui.widget_2->setVisible(!_monoConfiguration);
    this->layout();
   
}

_SwConfigurationManagerGui::~_SwConfigurationManagerGui()
{
    if (ui.treeView != 0 && ui.treeView->model() != 0)
    {
        disconnect(
            ui.treeView->model(),
            SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this,
            SLOT(onCurrentConfigModelChanged(const QModelIndex &, const QModelIndex &))
            );
        disconnect(
            ui.treeView,
            SIGNAL(clicked(const QModelIndex & )),
            this,
            SLOT(onCurrentConfigTreeViewClicked(const QModelIndex & ))
            );
    }
    if (_controller!= 0)
        _controller->removeControllerListener(this);
}


void _SwConfigurationManagerGui::setConfigurationEdited(bool value)
{
    if (_controller != 0)
    {
        _controller->setConfigurationEdited(value);
    }
}

void _SwConfigurationManagerGui::setEdition(bool enabled)
{
    if (enabled)
    {

        // quand l edition est remise a true les fenetre d edition / selection enregistrement/replay 
        // doivent etre mise a jour suivant les propriétés du composant
       startModeUpdated();
       recordActivatedUpdated();

    } else
    {
        ui.widgetRecord->setEnabled(enabled);
        ui.widgetReplay->setEnabled(enabled);

    }
    
    bool recordEnabled = enabled;
    if (_configurations != 0 && _configurations->getSelectedConfiguration() != 0)
    {
        recordEnabled = recordEnabled && _configurations->getSelectedConfiguration()->isRecordAvailable();
    } else
    {
        recordEnabled = false;
    }
    ui.checkBoxRecord->setEnabled(recordEnabled);
    ui.radioButtonNormal->setEnabled(enabled);
    ui.radioButtonReplay->setEnabled(enabled);
    ui.treeView->setEnabled(enabled);
}

void _SwConfigurationManagerGui::setConfigurationController (
    ISwConfigurationController * controller)
{
    if (_controller!= 0)
        _controller->removeControllerListener(this);

    _controller = controller;

    if (_controller!= 0)
        _controller->addControllerListener(this);
}



    
void _SwConfigurationManagerGui::setConfigurationsList(const QList<_SwManagedConfiguration *> & configList)
{
    _configList = configList;

    ui.comboBox->clear();
    ui.comboBox->setCurrentIndex(-1);
    QList<_SwManagedConfiguration * >::iterator it;
    it = _configList.begin();
    while (it != _configList.end())
    {
        
        if (*it!=0)
        {
            _SwManagedConfiguration * config = *it;
            QVariant variant = QVariant::fromValue<_SwManagedConfiguration*>(config);
            ui.comboBox->addItem((*it)->getName(),variant);
        }
        it++;
    }
}


void _SwConfigurationManagerGui::setManagedConfigurationsModel(_ISwManagedConfigurationsList * model)
{
    _configurations = model;
}


void _SwConfigurationManagerGui::onConfigurationListChanged()
{
    if (_configurations != 0)
        setConfigurationsList(_configurations->getConfigurationsList());
    else
    {
        _configList.clear();
        ui.comboBox->clear();
    }
}

void _SwConfigurationManagerGui::onCurrentConfigurationChanged()
{

    _SwManagedConfiguration * config = 0;
    config = _configurations->getSelectedConfiguration();
    _treeviewClicked = false;

    

    if (config != 0)
    {
        // l enregistrement activé ou non dépend de la conf selectionnée 
        recordActivatedUpdated();
        
        if (ui.treeView->model() != 0)
        {
            disconnect(
                ui.treeView->model(),
                SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
                this,
                SLOT(onCurrentConfigModelChanged(const QModelIndex &, const QModelIndex &))
                );
            disconnect(
                ui.treeView,
                SIGNAL(clicked(const QModelIndex & )),
                this,
                SLOT(onCurrentConfigTreeViewClicked(const QModelIndex & ))
                );
        }

        //QItemSelectionModel * m=ui.treeView->selectionModel();
        ui.treeView->setModel(config->getModel());
        //delete m;

        if (ui.treeView->model() != 0)
        {

            connect(
                ui.treeView,
                SIGNAL(clicked(const QModelIndex & )),
                this,
                SLOT(onCurrentConfigTreeViewClicked(const QModelIndex & ))
                );
            connect(
                ui.treeView->model(),
                SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
                this,
                SLOT(onCurrentConfigModelChanged(const QModelIndex &, const QModelIndex &))
                );

        }
        ui.treeView->setEnabled(config->isEnabled());

        // selection de la combobox
        bool found = false;
        int cpt = 0;
        while (!found && cpt < ui.comboBox->count ())
        {
            QVariant data  = ui.comboBox->itemData(cpt);
            _SwManagedConfiguration *  conf = data.value<_SwManagedConfiguration *>();
            
            if (conf != 0 && conf->getName().compare(config->getName()) == 0)
            {
                ui.comboBox->setCurrentIndex(cpt);
                
                found = true;
            }
            cpt++;
        }
    }
    else
    {
        ui.comboBox->setCurrentIndex(-1);
        QItemSelectionModel * m=ui.treeView->selectionModel();
        ui.treeView->setModel(0);
        delete m;
        ui.treeView->setEnabled(false);
    }
    
    ui.treeView->expandAll();
    ui.treeView->resizeColumnToContents(0);
}

void _SwConfigurationManagerGui::onCurrentConfigurationLoaded()
{

}

void _SwConfigurationManagerGui::onCurrentConfigurationSaved()
{

}

////// launch configuration
void _SwConfigurationManagerGui::setLaunchConfigurationModel(_ISwConfigurationLaunchManager * model)
{
    _launchConfiguration = model;
    configurationFileNameUpdated();
    recordActivatedUpdated();
    recordDirectoryUpdated();
    replayDirectoryUpdated();
    autoStartUpdated();
    startModeUpdated();
}

void _SwConfigurationManagerGui::autoStartUpdated()
{
    if (_launchConfiguration != 0)
    {
        ui.checkBoxAutoStart->setChecked(_launchConfiguration->getAutoStart());
    }
}

void _SwConfigurationManagerGui::startModeUpdated()
{
    if (_launchConfiguration != 0)
    {
        if (_configurations != 0 && _configurations->getSelectedConfiguration() != 0 && _launchConfiguration->getStartMode().ToInt() == 0)
        {
            ui.checkBoxRecord->setEnabled(_configurations->getSelectedConfiguration()->isRecordAvailable());
        }
        else
        {
            ui.checkBoxRecord->setEnabled(false);
        }
        ui.radioButtonNormal->setChecked(_launchConfiguration->getStartMode().ToInt() == 0);
        ui.radioButtonReplay->setChecked(_launchConfiguration->getStartMode().ToInt() == 1);
        ui.widgetNormal->setEnabled(_launchConfiguration->getStartMode().ToInt() == 0);
        ui.widgetReplay->setEnabled(_launchConfiguration->getStartMode().ToInt() == 1);
        ui.treeView->setEnabled(_launchConfiguration->getStartMode().ToInt() == 0);
        if (_controller != 0)
            _controller->setConfigurationEdited(_controller->getConfigurationEdited());

    }
}

void _SwConfigurationManagerGui::configurationFileNameUpdated()
{
    if (_launchConfiguration != 0)
    {
        _configurationFileName = _launchConfiguration->getConfigurationFile().getFileName();
        //ui.lineEditConfigurationFile->setText(_launchConfiguration->getConfigurationFile().getFileName());
        //ui.lineEditConfigurationFile->setToolTip(_launchConfiguration->getConfigurationFile().getFileName());
    }
}

void _SwConfigurationManagerGui::recordActivatedUpdated()
{
    if (_launchConfiguration != 0)
    {
        bool configAcceptsRecord = false;
        bool recordValue =false;
        recordValue =_launchConfiguration->getRecordActivated();
        if (_configurations->getSelectedConfiguration() != 0)
        {
            configAcceptsRecord = _configurations->getSelectedConfiguration()->isRecordAvailable();
            if (recordValue && !configAcceptsRecord)
            {
                // on force la valeur dans le cas ou une config n accepte pas l enregistrement.
                _launchConfiguration->setRecordActivated(false);
                // return ici car la callback va etre rappelé
                return;
            }
        } 
        
        ui.checkBoxRecord->setChecked(_launchConfiguration->getRecordActivated());
        ui.checkBoxRecord->setEnabled(configAcceptsRecord);
        ui.widgetRecord->setEnabled(_launchConfiguration->getRecordActivated());
    }
}

void _SwConfigurationManagerGui::recordDirectoryUpdated()
{
    if (_launchConfiguration != 0)
    {
        ui.lineEditRecordFile->setText(_launchConfiguration->getRecordDirectory().getFileName());
        ui.lineEditRecordFile->setToolTip(_launchConfiguration->getRecordDirectory().getFileName());
    }
}

void _SwConfigurationManagerGui::replayDirectoryUpdated()
{
    if (_launchConfiguration != 0)
    {
        ui.lineEditReplayFile->setText(_launchConfiguration->getReplayDirectory().getFileName());
        ui.lineEditReplayFile->setToolTip(_launchConfiguration->getReplayDirectory().getFileName());
    }
}


bool _SwConfigurationManagerGui::askForStop()
{

    QMessageBox msgBox;
    msgBox.setWindowTitle("Application running...");
    msgBox.setText("Do you want to stop application and change configuration ?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if (_controller != 0 && ret == QMessageBox::Ok)
    {
        _controller->Stop();
        return true;
    }
    
    return false;
}

void _SwConfigurationManagerGui::on_comboBox_activated(int index)
{

    if (_controller != 0 && _controller->isStarted())
    {    
        if (!askForStop())
            return;
    }
    
    int listSize = _configList.size();
    if (_configurations != 0 && listSize > index)
    {
        setConfigurationEdited(true);
        QVariant data  = ui.comboBox->itemData(index);
        _SwManagedConfiguration *  config = data.value<_SwManagedConfiguration*>();

        _configurations->setSelectedConfiguration(config); 
    }
    if (_configurations != 0 && _configurations->getSelectedConfiguration() != 0 && _launchConfiguration->getStartMode().ToInt() == 0)
    {
        ui.radioButtonNormal->setEnabled(_configurations->getSelectedConfiguration()->isRecordAvailable());
        ui.radioButtonReplay->setEnabled(_configurations->getSelectedConfiguration()->isRecordAvailable());
        ui.checkBoxRecord->setEnabled(_configurations->getSelectedConfiguration()->isRecordAvailable());
    }
    else
    {
        ui.radioButtonNormal->setEnabled(false);
        ui.radioButtonReplay->setEnabled(false);
        ui.checkBoxRecord->setEnabled(false);
    }
}

void _SwConfigurationManagerGui::on_toolButtonLoadConfig_clicked()
{

    if (_controller != 0 && _controller->isStarted())
    {    
        if (!askForStop())
            return;
    }

    QString fileName = QFileDialog::getOpenFileName(
        this, // Parent Widget
        "Select Configuration File", // Caption
        _lastOpenedConfigPath, // Directory
        "XML Files (*.conf)" // Filter
        );

    if(!fileName.isNull()) 
    {
        QFileInfo fileInfo(fileName);
        if (fileInfo.exists() && fileInfo.isFile())
        {
            _lastOpenedConfigPath = fileInfo.dir().absolutePath();
        }
        SwFileDescriptor desc(SwFileDescriptor::FileRead, "XML Files (*.conf)");
        desc.setFileName(fileName);
        /*if (_configurations->LoadConfiguration(fileName))
            _launchConfiguration->setConfigurationFile(desc);  */
        // on ne change pas le fichier courant de configuration 
        // on dispose d un fichier courant sauvé dans tout les cas au On quit de l app
        // les fichiers chargés écrasent le fichier courant
        _configurations->LoadConfiguration(fileName);
        setConfigurationEdited(true);
    }
    
}

void _SwConfigurationManagerGui::on_toolButtonSaveAsConfig_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, // Parent Widget
        "Select Configuration File", // Caption
        _lastOpenedConfigPath, // Directory
        "XML Files (*.conf)" // Filter
        );
    if(!fileName.isNull()) 
    {
        QFileInfo fileInfo(fileName);
        if (fileInfo.exists() && fileInfo.isFile())
        {
            _lastOpenedConfigPath = fileInfo.dir().absolutePath();
        }
        SwFileDescriptor desc(SwFileDescriptor::FileWrite, "XML Files (*.conf)");
        desc.setFileName(fileName);
        //_launchConfiguration->setConfigurationFile(desc);
        _configurations->SaveConfiguration(fileName);
        
    }
    
}

void _SwConfigurationManagerGui::on_radioButtonNormal_toggled(bool value)
{

}

void _SwConfigurationManagerGui::on_radioButtonReplay_toggled(bool value)
{

}

void _SwConfigurationManagerGui::on_toolButtonLoadRecordFile_clicked()
{
    QString filters = "";
    QFileDialog * dialog =  new QFileDialog(this,"Select Record Directory",_lastOpenedRecordPath,filters);
    dialog->setFileMode(QFileDialog::DirectoryOnly );
    dialog->setAcceptMode(QFileDialog::AcceptSave  );
    dialog->setOption(QFileDialog::ShowDirsOnly,true);
    
    QStringList reslist;
    //dialog.setViewMode(QFileDialog::Detail);
    //dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    if (dialog->exec())
    {
          reslist = dialog->selectedFiles();
    }                                            
    delete dialog;
    dialog = 0;
    QString fileName ="";
    if (reslist.size() != 0)
    {
        fileName =  reslist.first();
    }
   /*  = QFileDialog::getSaveFileName(
        this, // Parent Widget
        "Select Record Directory", // Caption
        _lastOpenedRecordPath,// Directory 
        //SW_APP->getApplicationDirPath();
        // directory base,
        filters,// filter
        0,                        // selectedFilter
        QFileDialog::ShowDirsOnly
        );      */
    
    if(!fileName.isNull()) 
    {
        QFileInfo fileInfo(fileName);
        if (fileInfo.exists() && !fileInfo.isFile())
        {
            _lastOpenedRecordPath = fileInfo.dir().absolutePath();
        }
        SwFileDescriptor desc(SwFileDescriptor::DirectorySelect, "Directory");
        desc.setFileName(fileName);
        _launchConfiguration->setRecordDirectory(desc);
        setConfigurationEdited(true);
        //ui.lineEditRecordFile->setText(fileName);
        //setConfigurationEdited(true);
    }
  
}

void _SwConfigurationManagerGui::on_toolButtonLoadReplayFile_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(
        this, // Parent Widget
        "Select Record Directory", // Caption
        _lastOpenedRecordPath // Directory
        );
    
    if(!fileName.isNull()) 
    {
        QFileInfo fileInfo(fileName);
        if (fileInfo.exists() && !fileInfo.isFile())
        {
            _lastOpenedRecordPath = fileInfo.dir().absolutePath();
        }
        SwFileDescriptor desc(SwFileDescriptor::DirectorySelect, "Directory");
        desc.setFileName(fileName);
        _launchConfiguration->setReplayDirectory(desc);
        setConfigurationEdited(true);
    }
}

void _SwConfigurationManagerGui::setStarted()
{
        ui.pushButtonStart->setEnabled(true);
        ui.pushButtonStop->setEnabled(true);
        setEdition(false);
}

void _SwConfigurationManagerGui::setStopped()
{
        if (_controller != 0)
        {
            ui.pushButtonStart->setEnabled(true);
        }
        else
            ui.pushButtonStart->setEnabled(false);

        ui.pushButtonStop->setEnabled(false);

        setEdition(true);
}

void _SwConfigurationManagerGui::on_pushButtonStart_clicked()
{
    _controller->Save();
    if (!_controller->isStarted())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Configuration Saved...");
        msgBox.setText("Do you want to start application now ?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
         
        if (_controller != 0 && ret == QMessageBox::Ok)
        {
            _controller->Start();
        }
    }
}



void _SwConfigurationManagerGui::on_pushButtonStop_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Application Running...");
    msgBox.setText("Do you want to stop application now ?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();    
    
    if (_controller != 0 && ret == QMessageBox::Ok)
    {
        _controller->Stop();
    }
}



void _SwConfigurationManagerGui::on_checkBoxAutoStart_stateChanged(int value)
{

}

void _SwConfigurationManagerGui::on_checkBoxRecord_stateChanged(int value)
{
    
}

void _SwConfigurationManagerGui::displayMessageBox(QString title, QString text)
{
    QMessageBox::information(this,title,text);
}

/** @brief sur changement d etat du controller*/
void _SwConfigurationManagerGui::OnStartedChanged(ISwConfigurationController * controller)
{
    if (controller == _controller && controller != 0)
    {
        if (_controller->isStarted()) 
            setStarted();
        else
            setStopped();
    }
}


/** @brief sur changement d etat*/
void _SwConfigurationManagerGui::OnConfigurationEditedChanged(ISwConfigurationController * controller)
{
    bool canSave = false;
    //OnStartedChanged(controller);
    if (_controller != 0)
    {
       
        //ui.pushButtonStart->setEnabled(true);
        if (_controller->getConfigurationEdited())
            ui.pushButtonStart->setText("Save *");
        else
            ui.pushButtonStart->setText("Save");

        canSave = _launchConfiguration != 0 && (_launchConfiguration->getStartMode().ToInt() == 0);
    }
    else 
    {
        ui.pushButtonStart->setText("Save *");
         
    }
    ui.pushButtonStart->setEnabled(canSave);
}


void _SwConfigurationManagerGui::onCurrentConfigModelChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight)
{
     
     if (_treeviewClicked)
     {
        setConfigurationEdited(true);
     }
     _treeviewClicked = false;
}

void _SwConfigurationManagerGui::onCurrentConfigTreeViewClicked(const QModelIndex & )
{
    _treeviewClicked = true;
}

void _SwConfigurationManagerGui::on_radioButtonNormal_clicked()
{
    if (_launchConfiguration != 0)
    {
       
       SwEnum startMode;
       
       startMode.AddKey(0,"Normal Mode");
       startMode.AddKey(1,"Replay Mode");   
     

        if (_launchConfiguration->getStartMode().ToInt() != 0)
        {
            if (ui.radioButtonNormal->isChecked())
            {
                _controller->SetNormalConfiguration();
                startMode.FromInt(0);
                _launchConfiguration->setStartMode(startMode);
                setConfigurationEdited(false);
               
            }
        }        
 
    }
}

void _SwConfigurationManagerGui::on_radioButtonReplay_clicked()
{
    if (_launchConfiguration != 0)
    {
       
       SwEnum startMode;
       
       startMode.AddKey(0,"Normal Mode");
       startMode.AddKey(1,"Replay Mode");   
     

        if (_launchConfiguration->getStartMode().ToInt() != 1)
        {
            if (ui.radioButtonReplay->isChecked())
            {
                _controller->SetReplayConfiguration();
                startMode.FromInt(1);
                _launchConfiguration->setStartMode(startMode);
                //setConfigurationEdited(true);
            }
        }        
 
    }
}

void _SwConfigurationManagerGui::on_checkBoxAutoStart_clicked()
{
    if (_launchConfiguration != 0)
    {
        if (_launchConfiguration->getAutoStart() != ui.checkBoxAutoStart->isChecked())
        {
            _launchConfiguration->setAutoStart(ui.checkBoxAutoStart->isChecked());    
            setConfigurationEdited(true);
        }
           
    
    }
}

void _SwConfigurationManagerGui::on_checkBoxRecord_clicked()
{
    if (_launchConfiguration != 0)
    {
        if (_launchConfiguration->getRecordActivated() != ui.checkBoxRecord->isChecked())
        {
            _launchConfiguration->setRecordActivated(ui.checkBoxRecord->isChecked());    
            setConfigurationEdited(true);
        }
           
    
    }
    
}
