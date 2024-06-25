/*!
\file _SwSwitchExecutionSetup.h
\date dec 2008
\brief Ihm de configuration de l'execution
\author  HLG
\version 1.0
 */

#ifndef __SwSwitchExecutionSetup_H
#define __SwSwitchExecutionSetup_H
/*
  * INCLUDES GLOBAUX
  */

#include <QDialog>
#include <QList>
#include <QLineEdit>
#include <QComboBox>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QTreeView>
#include <QTableView>
#include <QVBoxLayout>
/*
  * INCLUDES LOCAUX
  */

#include "Component/Base/SwComponent_Class.h"
#include "Properties/ISwProperty.h"
#include "ISwExecutable_Service.h"
#include "_SwExecutorStreamTreeModel.h"
#include "_SwExecutorListEditor.h"
#include "_SwSwitchExecutionList.h"
#include "_SwExecution_Service.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;

/*!
	\class _SwSwitchExecutionSetup 
	\brief Ihm de configuration de l'execution
*/

typedef QMap<int, QString> ExeListName;

class _SwSwitchExecutionSetup : public QDialog
{
    Q_OBJECT
protected:
    /*! \brief composant racine */
    SwComponent_Class * _root_component;
    /* List des interfaces d'execution */
    QList<QString> _exe_paths;
    /* List des modes d'executions*/
    QList<_SwExecutionMode> _exe_modes;

public:
    /*! \brief Constructeur */
    _SwSwitchExecutionSetup(QWidget *parent,SwComponent_Class * root_component,QList<QString> * current_list,
                            _SwSwitchExecutionList * switchExecutionList);
    /*! \brief Destructeur */
    ~_SwSwitchExecutionSetup();

    void SaveExePathInCurrentList(void);

    /*! \brief Permet d'acceder a la nouvelle liste */
    const QList<QString> * GetEditedList();    
    const QList<_SwExecutionMode> * GetEditedListModes();
    _SwExecutorListEditor * list_view;
    _SwExecutorStreamTreeModel * model; 

    QVBoxLayout * main_layout;
    QHBoxLayout * list_layout1; 
    QHBoxLayout * list_layout2; 
    QHBoxLayout * data_layout;  
    QHBoxLayout * button_layout;

    QScrollArea * sa;
    QTreeView * view;
    QSplitter * splitter;
    QPushButton * but;
    QPushButton * button_deleteSelectedList;
    QPushButton * button_addNewListName;


    QLineEdit * edit_newListName;
    QComboBox * combo_exeList;

private:
    _SwSwitchExecutionList * _switchExecutionList;
    void UpdateExecutableView(void);
    bool deleteInProgress;
    bool addInProgress;

public slots:
    void Clicked_addNewListName(void);
    void Clicked_deleteSelectedList(void);
    void CurrentIndexChanged(int index);
};

#endif
