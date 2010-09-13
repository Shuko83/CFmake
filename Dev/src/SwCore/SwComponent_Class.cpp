/*!
 \file SwComponent_Class.cpp
 \brief Implementation of the Class SwComponent_Class
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <QtGlobal>
#include <QStringList>
#include <QRegExp>
#include "SwComponent_Class.h"
#include "_SwCleanLinksVisitor.h"

using namespace StreamWork::SwCore;

/*
 * CONSTANTES
 */
#define CL_INDEX_MAX 9999

/*! \brief Constructeur */
SwComponent_Class::SwComponent_Class():QObject(),SwNamed_Class(),SwRef(),SwServicesManager_Class(){
    _parent=NULL;
    _child_components.clear();
    _current_child=_child_components.begin();
    _factory_component_name=QString();
    _active=true;
}

/*! \brief Destructeur */
SwComponent_Class::~SwComponent_Class(){
    //Deconnexion des liens
    _SwCleanLinksVisitor cleanVisitor;
    //AcceptVisitor(&cleanVisitor);

    QMap<QString,SwComponent_ClassPtr>::iterator it;

    //qDebug("Destroy component: %s",GetName().toLatin1().data());
    for(it=_child_components.begin();it!=_child_components.end();it++) {
        it.value()->SetParent(NULL); //ATTENTION !!!!
    }
    _child_components.clear();
    LiberateAllServices();
    //qDebug("End of destroy component: %s",GetName().toLatin1().data());
}

/*! \brief definition du nom usine du composant */
void SwComponent_Class::SetFactoryComponentName(QString factory_component_name) {
    _factory_component_name=factory_component_name;
}
/*! \brief acces nom usine du composant*/
QString SwComponent_Class::GetFactoryComponentName() {
    return _factory_component_name;
}

/*! \brief Initialisation des ressources
\exception SwException depend du composant*/
void SwComponent_Class::InitializeResources() throw(SwException){

}

/*! \brief definition du parent
\param[in] parent parent du composant*/
void SwComponent_Class::SetParent(SwComponent_Class * parent){
    _parent=parent;
    OnParentChange();
}

/*! \brief Acces au parent du composant
\return parent du composant*/
SwComponent_Class * SwComponent_Class::GetParent() const{
    return _parent;
}
/*! \brief Ajout d'un enfant
\param[in] child nouvel enfant a inserer
\exception SwException Can't add new child because an existing child have the same name*/
void SwComponent_Class::AddChild(SwComponent_Class * child)  throw(SwException){
    QMap<QString,SwComponent_ClassPtr>::const_iterator it;
    //Recherche s'il existe un enfant ayant le même nom
    it=_child_components.find(child->GetName());
    if (it!=_child_components.end()) {
        //Oui - erreur
        QString msg=QString("Can't add new child because an existing child have the same name [%1]").arg(child->GetName());
        LAUNCH_SWEXCEPTION("SwCore",msg);
    }
    OnBeforeAddChild(this,child);
    //Non, insertion
    _child_components.insert(child->GetName(),SwComponent_ClassPtr(child));
    child->SetParent(this);
    OnAddChild(this,child);
}
/*! \brief Retrait d'un enfant
\param[in] child nouvel enfant a inserer
\exception SwException Can't remove an unknown child*/
void SwComponent_Class::RemoveChild(SwComponent_Class * child)  throw(SwException){
    QMap<QString,SwComponent_ClassPtr>::iterator it;

    it=_child_components.find(child->GetName());
    if (it==_child_components.end()) {
       QString msg=QString("Can't remove an unknown child [%1]").arg(child->GetName());
       LAUNCH_SWEXCEPTION("SwCore",msg);
    }
    //Signal de retrait
    OnRemoveChild(this,it.value());
    //Sinon
    //Mise a jour de l'iterateur courant si necessaire
    if (_current_child==it) _current_child++;
    //Celui-ci na plus de parent
    it.value()->SetParent(NULL);
    //Supression de l'enfant
    _child_components.erase(it);
    OnAfterRemoveChild(this);
}

/*! \brief Acces au nombre d'enfants
\return le nombre d'enfant*/
unsigned long SwComponent_Class::GetNbChildren() const {
    return _child_components.size();
}
/*! \brief Acces a un enfant par son nom
\param[in] child_name nom de l'enfant
\return l'enfant correspondant ou NULL si non trouvé*/
SwComponent_Class * SwComponent_Class::GetChild(QString child_name) const{
    QMap<QString,SwComponent_ClassPtr>::const_iterator it;

    it=_child_components.find(child_name);
    if (it==_child_components.end()) {
        return NULL;
    }
    return it.value();
}
/*! \brief Acces a un enfant par son index */
SwComponent_Class * SwComponent_Class::GetChild(unsigned long index)  const{
    QMap<QString,SwComponent_ClassPtr>::const_iterator it;

    if (index>=(unsigned long)_child_components.size()) {
        return NULL;
    }
    //it=_child_components.begin();
    //while (l_index>0) {it++;l_index--;}
    //if (it!=_child_components.end())
    //    return it.value();
    //return NULL;
    it=_child_components.begin();
    it+=index;
    if (it!=_child_components.end())
        return it.value();
    return NULL;
}
/*! \brief Renvoie l'index d'un enfant defnit par son handle */
int SwComponent_Class::GetChildIndex(SwComponent_Class * comp_child)  const {
    QMap<QString,SwComponent_ClassPtr>::const_iterator itc;
    int index;

    if (comp_child->GetParent()!=this)
        return -1;

    index=0;
    itc=_child_components.begin();
    while ((*itc)!=comp_child && itc!=_child_components.end()) {itc++;index++;}
    if (itc==_child_components.end()) {
        return -1;
    }
    return index;
}
/*! \brief Acces au premier enfant
\return le premier enfant (par ordre alphabetique) ou NULL si non trouvé
\note la suppression pendant le parcours est autorisée*/
SwComponent_Class * SwComponent_Class::GetFirstChild(){
    SwComponent_Class * child;
    _current_child=_child_components.begin();
    //Sommes nous la fin
    if (_current_child==_child_components.end()) {
        return NULL;//Oui
    }
    //Non, recuperation de l'enfant
    child=_current_child.value();
    //Incrementation
    _current_child++;
    //Renvoie de l'enfant
    return child;
}
/*! \brief Acces a l'enfant suivant
\return l'enfant suivant(par ordre alphabetique) ou NULL si non trouvé
\note la suppression pendant le parcours est autorisée */
SwComponent_Class * SwComponent_Class::GetNextChild(){
    SwComponent_Class * child;

    //Sommes nous la fin
    if (_current_child==_child_components.end()) {
        return NULL;//Oui
    }
    //Non, recuperation de l'enfant
    child=_current_child.value();
    //Incrementation
    _current_child++;
    //Renvoie de l'enfant
    return child;
}
/*! \brief acces visiteur
\param[in] visiteur*/
void SwComponent_Class::AcceptVisitor(ISwVisitor * visitor) {
    QMap<QString,SwComponent_ClassPtr>::iterator it;

    //Visite de soi-meme
    visitor->Visit(this);
    //Visite des enfants
    for (it=_child_components.begin();it!=_child_components.end();it++) {
        it.value()->AcceptVisitor(visitor);
    }
}
/*! \brief Permet de suggerer un nom pour un futur enfant*/
QString SwComponent_Class::GetSuggestedNameForChild(QString initial_name) {
    QMap<QString,SwComponent_ClassPtr>::iterator it;
    QString racine_nom;
    QString nouveau_nom;
    long index,new_index;
    bool result;
    QStringList list;
    QRegExp rx("(.*[^0-9])([0-9]+)$");

    it=_child_components.find(initial_name);
    if (it==_child_components.end())
        return initial_name;

    //Si c'est un nom numerote -extraction de la racine
    rx.indexIn(initial_name);
    list = rx.capturedTexts();
    if (list.count()==3) {
        //Matching, on recupere la racine
        racine_nom=list.at(1);
        index=list.at(2).toLong(&result);
        if (!result) {
            racine_nom=initial_name;
            index=0;
        }
    } else {
        //Not matching on prend le nom de l'element comme racine
        racine_nom=initial_name;
        index=0;
    }
    new_index=index+1;
    nouveau_nom=QString("%1%2").arg(racine_nom).arg(new_index);
    while (_child_components.find(nouveau_nom)!=_child_components.end() && new_index!=index) {
        new_index++;
        if (new_index>CL_INDEX_MAX)
            new_index=0;
        nouveau_nom=QString("%1%2").arg(racine_nom).arg(new_index);
    }
    return nouveau_nom;
}
/*! \brief getter active */
bool SwComponent_Class::isActive() {
    return _active;
}
/*! \brief setter active */
void SwComponent_Class::setActive(bool active){
    _active=true;
}
//------------------------------------------------
//Surcharge de la classe mère SwNamed_Class
//------------------------------------------------
/*! \brief Permet de definir le nom de l'instance nommé (Surcharge)
\param[in] nouveau nom de l'enfant
\exception SwException Can't change name of because an existing brother have already this name
\exception SwException invalid name for instance
*/
void SwComponent_Class::SetName(const QString & new_name) throw(SwException) {
    QMap<QString,SwComponent_ClassPtr>::iterator it;

    //On regarde si un enfant de notre parent a le même nom que le nouveau nom proposé
    if (_parent!=NULL && _parent->GetChild(new_name)!=NULL) {
        //Oui, erreur
       QString msg=QString("Can't change name of %1 with new name %2 because an existing brother have already this name").arg(GetName()).arg(new_name);
       LAUNCH_SWEXCEPTION("SwCore",msg);
    }
    //Ancien nom
    QString oldName=GetName();
    //Changement de nom
    try {
        SwNamed_Class::SetName(new_name);
        //Si on a un parent
        if (_parent!=NULL) {
            _parent->_child_components.insert(GetName(),SwComponent_ClassPtr(this));
            it=_parent->_child_components.find(oldName);
            _parent->_child_components.erase(it);
        }
    } catch (SwException & ) {

    }
    OnChangeComponentName(this);
}
