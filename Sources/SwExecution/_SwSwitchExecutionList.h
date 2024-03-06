/**
* @file _SwSwitchExecutionList.h
* @brief permet de stocker :
*       la propriété concernant le nom de la liste d'exe
*       les noms des listes d'execution et leur index
*       une map avec le nom de la liste et sa liste de composants executables 
*/

#ifndef _SwSwitchExecutionList_H
#define _SwSwitchExecutionList_H

#include <QMap>

#include "Types/SwEnum.h"
#include "Properties/ISwProperty.h"
#include "_SwExecution_Service.h"

using namespace StreamWork::SwCore;

typedef QMap<int, QString> ExeListName;
typedef QMap<QString, QList<QString>> ExeListMap;
typedef QMap<QString, QList<_SwExecutionMode>> ModeListMap;

class _SwSwitchExecutionList
{
    public :
        _SwSwitchExecutionList();
        ~_SwSwitchExecutionList();

        void AddListName(int index,QString listName);
        void AddExeList(QString listName, QList<QString> exeList,QList<_SwExecutionMode> modeList);
        void DeleteList(int index);
        void DeleteExeList(QString listName);

        SwEnum _list_name;
        ISwProperty * _list_name_property;

        ExeListName _exeListName;
        ExeListMap _exeListMap;
        ModeListMap _modeListMap;

    private :
        

};
#endif
