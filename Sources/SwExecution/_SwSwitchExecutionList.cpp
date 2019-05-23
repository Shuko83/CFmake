/**
* @file _SwSwitchExecutionList.cpp
* @brief permet de stocker :
*       la propriété concernant le nom de la liste d'exe
*       les noms des listes d'execution et leur index
*       une map avec le nom de la liste et sa liste de composants executables 
*/
#include "_SwSwitchExecutionList.h"

_SwSwitchExecutionList::_SwSwitchExecutionList() {
}
_SwSwitchExecutionList::~_SwSwitchExecutionList() {
}
void _SwSwitchExecutionList::AddListName(int index,QString listName) {
    //ajout dans la liste
    _exeListName.insert(index, listName);

    //ajout dans la propriété du composant
    _list_name.RemoveKey(index); // remove si cle deja prise
    _list_name.AddKey(index, listName);
    _list_name.FromInt(index);
    QString test2 = _list_name.ToString();
    QVariant _list_name_qvariant;
    _list_name_qvariant.setValue(_list_name);
    _list_name_property->SetValue(_list_name_qvariant);
}
void _SwSwitchExecutionList::AddExeList(QString listName, QList<QString> exeList,QList<_SwExecutionMode> modeList) {
    _exeListMap.insert(listName, exeList);
    _modeListMap.insert(listName, modeList);
}
void _SwSwitchExecutionList::DeleteList(int index) {

    //recherche du nom à retirer
    ExeListName::iterator it = _exeListName.find(index);

    if(it != _exeListName.end()) {

        //suppression de la liste d'executables associés
        DeleteExeList(it.value());
        //suppression dans la propriété
        _list_name.RemoveKey(it.key());
        //suppression du nom
        _exeListName.remove(it.key());

        //suppression dans la propriété et
        //reorganisation de la liste de nom afin de garder une cohérence entre 
        //les index de _exeListName et l'enum _list_name
        it = _exeListName.begin();
        while(it!= _exeListName.end()) {
            _list_name.RemoveKey(it.key());
            it++;
        }

        //copie des noms avec des index corrigés dans une liste temporaire
        //modification du SwEnum _list_name
        ExeListName tmpList;
        it = _exeListName.begin();
        int tmpIndex = 0;    
        while(it != _exeListName.end()) {
            tmpList.insert(tmpIndex, it.value());
            _list_name.AddKey(tmpIndex, it.value());
            tmpIndex++;
            it++;
        }

        //raz de la liste de nom
        _exeListName.clear();    

        //copie de la liste temporaire dans la liste definitive
        it = tmpList.begin();
        while(it != tmpList.end()) {
            _exeListName.insert(it.key(), it.value());    
            it++;
        }

        if(_exeListName.size()!=0) {
            _list_name.FromInt(0);
            QVariant _list_name_qvariant;
            _list_name_qvariant.setValue(_list_name);
            _list_name_property->SetValue(_list_name_qvariant);
        }
    }
}
void _SwSwitchExecutionList::DeleteExeList(QString listName) {
    ExeListMap::iterator it = _exeListMap.find(listName);
    if(it != _exeListMap.end())
        _exeListMap.erase(it);
    ModeListMap::iterator itm = _modeListMap.find(listName);
    if(itm != _modeListMap.end())
        _modeListMap.erase(itm);
}