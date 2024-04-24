#pragma once

/**
* Include QT
*/
#include <QDialog>
#include <QFileSystemWatcher>

/**
* Include Local
*/
#include "ui_AboutWidget.h"

class AboutWidget : public QDialog
{
    Q_OBJECT;
    
public:
    AboutWidget( QWidget * parent = nullptr );
    
    ~AboutWidget();
    
    //------------------------------------------------------------------
    // Interface ISwWidget
    //------------------------------------------------------------------
    /**
    * @brief Renvoie le widget
    * @return Widget - pointer on this widget
    */
    virtual QWidget * GetWidget();
    
    /**
     * @brief   : Load the file and check if debug to be able to load the direct file on computer not the one in the resource files
     */
    void loadFile();
    
    void onService( bool available );
    
public slots:
    void reload();
    
protected:
    Ui::AboutWidgetClass ui;
    QFileSystemWatcher _releaseNoteWatcher;
};
