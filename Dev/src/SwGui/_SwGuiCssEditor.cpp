/*!
\file _SwGuiCssEditor.cpp
\brief 
\version 1.0
\date 
\author 
*/

#include <QApplication>
#include <SwApplication.h>
#include <SwMacros.h>
#include <SwRefPtr.h>
#include <QList.h>
#include "_SwGuiCssEditor.h"
#include "_SwGuiCssDialog.h"
#include <QDialog>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*! \brief Constructeur */
_SwGuiCssEditor::_SwGuiCssEditor():Component() 
{

    _styleFile="StyleSheet/style.css";
}

/*! \brief Destructeur */
_SwGuiCssEditor::~_SwGuiCssEditor() 
{

	//Destruction properties automatiques
	//Desenregistrement automatiques des interfaces
	//Destruction Pins			         

	//DETRUIRE LES AUTRES ATTRIBUTS DE LA CLASSE

}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiCssEditor::initializeComponent() throw(SwException) 
{

	//--------------------------------------
	//Definition Interfaces fournis
	//--------------------------------------


	//--------------------------------------
	//Definition Interfaces consommés
	//--------------------------------------

	//S'enregistrer comme observer du consumer
	//getIConsumerService().AttachInterfacesServices_Listener(this);

	//--------------------------------------
	//Definition Pins
	//--------------------------------------

	//--------------------------------------
	//Definition Properties
	//--------------------------------------            
	getPropertiesService().CreatePropertiesForQObject(this, QString(),true);

	//--------------------------------------
	//Autres
	//--------------------------------------
}

/*! \brief terminaison du composante*/
void _SwGuiCssEditor::terminateComponent()
{
}

/*! \brief Callback sur les changements de propriétés*/
void _SwGuiCssEditor::eventPropertyChange(ISwProperty * property) 
{
}

//----------------------------------------------------
// Interface ISwPin_Listener
//----------------------------------------------------
/*! \brief Sur reception d'une donnée */
void _SwGuiCssEditor::eventReceiveData(SwPin * src,SwData_Class * data) 
{

}

//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwGuiCssEditor::eventBeforeInterfaceAvailability(
	QString interface_name,SwComponent_Class * provider_host) 
{
	// sur changement d'une interface, 
	// si l interface existe
	// on demande à la widget de retirer l'interface

}

/*! \brief Apres changement de la disponibilité de l'interface */
void _SwGuiCssEditor::eventAfterInterfaceAvailability(
	QString interface_name,SwComponent_Class * provider_host) 
{
	// sur changement d'une interface, 
	// si l action existe
	// on demande à la widget d'ajouter l'action
	// sur changement d'une interface, 
	// si l action existe
	// on demande à la widget de retirer l'action

}

/*! \brief Apres changement de la disponibilité de l'interface */
void _SwGuiCssEditor::eventActivationChanged() 
{
}

//---------------------------------------------------------------------
// ISwAdminSetup
//---------------------------------------------------------------------

/*! \brief Permet d'acceder aux preferences d'administration via StreamWorkEditor*/
void _SwGuiCssEditor::AdminSetup()
{
	_SwGuiCssDialog *tmpCssDialog = new _SwGuiCssDialog;
	tmpCssDialog->setMyStyleSheet(SW_APP->GetApplicationDirPath()+QDir::separator()+getStyle());
	tmpCssDialog->show();
	connect(tmpCssDialog,SIGNAL(saveCSS()),this,SLOT(applyStyle()));
}

QString _SwGuiCssEditor::getStyle() 
{
	//remove the app dir path
	return _styleFile.remove(QString(SW_APP->GetApplicationDirPath()+QDir::separator())); 
}

/*! \brief Applique le style du fichier par default*/
void _SwGuiCssEditor::applyStyle()
{
	setStyle(getStyle());
}

/*! \brief Set la propriété du composant et applique le style a la qApp*/
void _SwGuiCssEditor::setStyle( QString val )
{
	//Add app dir path 
	_styleFile = SW_APP->GetApplicationDirPath()+QDir::separator()+val; 
	
	QFile file(SW_APP->GetApplicationDirPath()+QDir::separator()+val);
	if(file.open(QFile::ReadOnly))
	{
		QString styleSheet = QLatin1String(file.readAll());
		qApp->setStyleSheet(styleSheet);
	}

}
