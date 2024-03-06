/**
@file _SwFileEditorManager.cpp
@brief Implementation du manager des editeurs de fichiers
@author F.Bighelli
 */
#include "_SwFileEditorManager.h"
 
using namespace StreamWork::SwCore;

/** @brief Constructor */
_SwFileEditorManager::_SwFileEditorManager() {
	//A Completer
}
/** @brief destructor */
_SwFileEditorManager::~_SwFileEditorManager() {
    QMap<QString,QList<ISwFileEditor *> *>::iterator it=_db.begin();
    while (it!=_db.end()) {
        delete *it;
        it++;
    }
    _db.clear();
}
//---------------------------------------------------------------------
// Partie pour les editeurs
//---------------------------------------------------------------------
/** 
* @brief enregistrement d'un editeur de fichier
* @param editor acces interface editor 
*/
void _SwFileEditorManager::registerFileEditor(ISwFileEditor * editor){
    QMap<QString,QList<ISwFileEditor *> *>::iterator it;
    QStringList *liste=editor->getSuffixs();
    int count_suffixs=liste->count();
    for(int i=0;i<count_suffixs;i++) {
        it=_db.find(liste->at(i));
        QList<ISwFileEditor *> * flist;
        if (it==_db.end()) {
            flist=new QList<ISwFileEditor *>();
            _db.insert(liste->at(i),flist);
        } else {
            flist=*it;
        }
        flist->push_back(editor);
    }
}
/** 
* @brief deenregistrement d'un editeur de fichier
* @param editor acces interface editor 
*/
void _SwFileEditorManager::unregisterFileEditor(ISwFileEditor * editor){
    QMap<QString,QList<ISwFileEditor *> *>::iterator it=_db.begin();
    while (it!=_db.end()) {
        QList<ISwFileEditor *> *flist=*it;
        int index=flist->indexOf(editor);
        if (index!=-1) {
            flist->removeAt(index);
            if (flist->count()==0) {
                delete flist;
                it=_db.erase(it);
            } else
                it++;
        } else
            it++;
    }
}
//---------------------------------------------------------------------
// Partie pour les utlisateurs de l'editeur
//---------------------------------------------------------------------
/** 
* @brief edition d'un fichier (methode non bloquante)
* @param descriteur de fichier
* @return false si aucun editeur n'a été trouvé
*/            
bool _SwFileEditorManager::edit(SwFileDescriptor fd){
    QStringList liste=fd.getFileName().split(".");
    if(liste.count()==0)
        return false;
    QMap<QString,QList<ISwFileEditor *> *>::iterator it=_db.find(liste.at(liste.count()-1));
    if (it!=_db.end()) {
        return (*it)->at(0)->edit(fd);
    }
    return false;
}
