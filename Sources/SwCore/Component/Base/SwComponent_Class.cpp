/*!
 \file SwComponent_Class.cpp
 \brief Implementation of the Class SwComponent_Class
 \date 23-août-2006 18:59:26
 \author F.Bighelli
 */

#include <QtGlobal>
#include <QStringList>
#include <QRegExp>
#include "SwMacros.h"
#include "SwComponent_Class.h"
#include "_SwCleanLinksVisitor.h"

#include "QExceptionManager.h"

using namespace StreamWork::SwCore;

/*
 * CONSTANTES
 */
#define CL_INDEX_MAX 9999

//-----------------------------------------------------------------------
SwComponent_Class::SwComponent_Class() :QObject(), SwNamed_Class(), SwRef(), SwServicesManager_Class()
{
	_parent = nullptr;
	_child_components.clear();
	_current_child = _child_components.begin();
	_factory_component_name = QString();
	_active = true;
}

//-----------------------------------------------------------------------
SwComponent_Class::~SwComponent_Class()
{
	//Deconnexion des liens
	_SwCleanLinksVisitor cleanVisitor;
	AcceptVisitor(&cleanVisitor);

	_current_child = _child_components.end();
	QMap<QString, SwComponent_ClassPtr>::iterator it;
	for (it = _child_components.begin(); it != _child_components.end(); it++)
	{
		OnRemoveChild(this, it.value());
		it.value()->SetParent(nullptr); //ATTENTION !!!!
	}

	_child_components.clear();
	LiberateAllServices();
}

//-----------------------------------------------------------------------
void SwComponent_Class::SetFactoryComponentName(QString factory_component_name)
{
	_factory_component_name = factory_component_name;
}

//-----------------------------------------------------------------------
QString SwComponent_Class::GetFactoryComponentName()
{
	return _factory_component_name;
}

//-----------------------------------------------------------------------
void SwComponent_Class::InitializeResources() throw(SwException)
{

}

//-----------------------------------------------------------------------
void SwComponent_Class::SetParent(SwComponent_Class * parent)
{
	_parent = parent;
	OnParentChange();
}


// -----------------------------------------------------------------------
SwComponent_Class * SwComponent_Class::GetParent() const
{
	return _parent;
}

//-----------------------------------------------------------------------
void SwComponent_Class::AddChild(SwComponent_Class * child)  throw(SwException)
{
	QMap<QString, SwComponent_ClassPtr>::const_iterator it;
	//Recherche s'il existe un enfant ayant le même nom
	it = _child_components.find(child->GetName());
	if ( it != _child_components.end() )
	{
		//Oui - erreur
		QString msg = QString("Can't add new child because an existing child have the same name [%1]").arg(child->GetName());
		LAUNCH_SWEXCEPTION("SwCore", msg);
	}

	OnBeforeAddChild(this, child);
	//Non, insertion
	_child_components.insert(child->GetName(), SwComponent_ClassPtr(child));
	child->SetParent(this);
	OnAddChild(this, child);
}

//-----------------------------------------------------------------------
void SwComponent_Class::RemoveChild(SwComponent_Class * child)  throw(SwException)
{
	QMap<QString, SwComponent_ClassPtr>::iterator it;

	it = _child_components.find(child->GetName());
	if ( it == _child_components.end() )
	{
		QString msg = QString("Can't remove an unknown child [%1]").arg(child->GetName());
		LAUNCH_SWEXCEPTION("SwCore", msg);
	}

	//Signal de retrait
	OnRemoveChild(this, it.value());
	//Sinon
	//Mise a jour de l'iterateur courant si necessaire
	if ( _current_child == it ) _current_child++;
	//Celui-ci na plus de parent
	it.value()->SetParent(NULL);
	//Supression de l'enfant
	_child_components.erase(it);

	OnAfterRemoveChild(this);
}

//-----------------------------------------------------------------------
unsigned long SwComponent_Class::GetNbChildren() const
{
	return _child_components.size();
}

//-----------------------------------------------------------------------
SwComponent_Class * SwComponent_Class::GetChild(QString child_name) const
{
	QMap<QString, SwComponent_ClassPtr>::const_iterator it;

	it = _child_components.find(child_name);
	if ( it == _child_components.end() )
		return NULL;

	return it.value();
}

//-----------------------------------------------------------------------
SwComponent_Class * SwComponent_Class::GetChild(unsigned long index)  const
{
	QMap<QString, SwComponent_ClassPtr>::const_iterator it;

	if ( index >= (unsigned long) _child_components.size() )
		return NULL;

	it = _child_components.begin();
	it += index;
	if ( it != _child_components.end() )
		return it.value();

	return NULL;
}
//-----------------------------------------------------------------------
int SwComponent_Class::GetChildIndex(SwComponent_Class * comp_child)  const
{
	QMap<QString, SwComponent_ClassPtr>::const_iterator itc;
	int index;

	if ( comp_child->GetParent() != this )
		return -1;

	index = 0;
	itc = _child_components.begin();
	while ( (*itc) != comp_child && itc != _child_components.end() ) 
		itc++; index++; 

	if ( itc == _child_components.end() )
		return -1;

	return index;
}

//-----------------------------------------------------------------------
SwComponent_Class * SwComponent_Class::GetFirstChild()
{
	SwComponent_Class * child;
	_current_child = _child_components.begin();
	//Sommes nous la fin
	if ( _current_child == _child_components.end() )
		return NULL;//Oui

	//Non, recuperation de l'enfant
	child = _current_child.value();

	//Incrementation
	_current_child++;

	//Renvoie de l'enfant
	return child;
}

//-----------------------------------------------------------------------
SwComponent_Class * SwComponent_Class::GetNextChild()
{
	SwComponent_Class * child;

	//Sommes nous la fin
	if ( _current_child == _child_components.end() )
		return NULL;//Oui

	//Non, recuperation de l'enfant
	child = _current_child.value();
	//Incrementation
	_current_child++;
	//Renvoie de l'enfant
	return child;
}

//-----------------------------------------------------------------------
void SwComponent_Class::AcceptVisitor(ISwVisitor * visitor)
{
	QMap<QString, SwComponent_ClassPtr>::iterator it;

	//Visite de soi-meme
	visitor->Visit(this);
	//Visite des enfants
	for ( it = _child_components.begin(); it != _child_components.end(); it++ )
		it.value()->AcceptVisitor(visitor);
}

//-----------------------------------------------------------------------
QString SwComponent_Class::GetSuggestedNameForChild(QString initial_name)
{
	QMap<QString, SwComponent_ClassPtr>::iterator it;
	QString racine_nom;
	QString nouveau_nom;
	long index, new_index;
	bool result;
	QStringList list;
	QRegExp rx("(.*[^0-9])([0-9]+)$");

	//Check si le nom contient des caractères interdis et on remplace par "_"
	QString newName = initial_name.replace(QRegExp(":+"), "_");

	it = _child_components.find(newName);
	if ( it == _child_components.end() )
		return newName;

	//Si c'est un nom numerote -extraction de la racine
	rx.indexIn(newName);
	list = rx.capturedTexts();
	if ( list.count() == 3 )
	{
		//Matching, on recupere la racine
		racine_nom = list.at(1);
		index = list.at(2).toLong(&result);
		if ( !result )
		{
			racine_nom = newName;
			index = 0;
		}
	}
	else
	{
		//Not matching on prend le nom de l'element comme racine
		racine_nom = newName;
		index = 0;
	}
	new_index = index + 1;
	nouveau_nom = QString("%1%2").arg(racine_nom).arg(new_index);
	while ( _child_components.find(nouveau_nom) != _child_components.end() && new_index != index )
	{
		new_index++;
		if ( new_index > CL_INDEX_MAX )
			new_index = 0;
		nouveau_nom = QString("%1%2").arg(racine_nom).arg(new_index);
	}

	return nouveau_nom;
}

//-----------------------------------------------------------------------
bool SwComponent_Class::isActive()
{
	return _active;
}

//-----------------------------------------------------------------------
void SwComponent_Class::setActive(bool active)
{
	_active = active;
}

//-----------------------------------------------------------------------
void SwComponent_Class::SetName(const QString & new_name) throw(SwException)
{
	QMap<QString, SwComponent_ClassPtr>::iterator it;

	//On regarde si un enfant de notre parent a le même nom que le nouveau nom proposé
	if ( _parent != NULL && _parent->GetChild(new_name) != NULL )
	{
		//Oui, erreur
		QString msg = QString("Can't change name of %1 with new name %2 because an existing brother have already this name").arg(GetName()).arg(new_name);
		LAUNCH_SWEXCEPTION("SwCore", msg);
	}

	QString oldName = GetName();
	EXCEPTION_TRY();
	SwNamed_Class::SetName(new_name);
	//Si on a un parent
	if ( _parent != NULL )
	{
		_parent->_child_components.insert(GetName(), SwComponent_ClassPtr(this));
		it = _parent->_child_components.find(oldName);
		_parent->_child_components.erase(it);
	}
	EXCEPTION_CATCH();
	OnChangeComponentName(this);
}
