/*!
\file _SwGuiConsole.cpp
\brief 
\version 1.0
\date 
\author 
*/

#include <QApplication>
#include <SwApplication.h>
#include <SwMacros.h>
#include <SwRefPtr.h>
#include "windows.h"
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include "_SwGuiConsole.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*! \brief Constructeur */
_SwGuiConsole::_SwGuiConsole():Component() 
{

	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;

	//Release console if already got one
	FreeConsole();

	// allocate a console for this app
	AllocConsole();

	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&coninfo);
	coninfo.dwSize.Y = 999;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),coninfo.dwSize);

	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );

	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "r" );
	*stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );

	// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );

	std::cout << "Attention les erreurs de signaux Qt ne sont pas affiche dans cette console."<<std::endl;
}

/*! \brief Destructeur */
_SwGuiConsole::~_SwGuiConsole() 
{


}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiConsole::initializeComponent() throw(SwException) 
{


	//--------------------------------------
	//Autres
	//--------------------------------------
}

/*! \brief terminaison du composante*/
void _SwGuiConsole::terminateComponent()
{
}

/*! \brief Callback sur les changements de propriétés*/
void _SwGuiConsole::eventPropertyChange(ISwProperty * property) 
{
}

//----------------------------------------------------
// Interface ISwPin_Listener
//----------------------------------------------------
/*! \brief Sur reception d'une donnée */
void _SwGuiConsole::eventReceiveData(SwPin * src,SwData_Class * data) 
{

}

//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwGuiConsole::eventBeforeInterfaceAvailability(
	QString interface_name,SwComponent_Class * provider_host) 
{
	// sur changement d'une interface, 
	// si l interface existe
	// on demande à la widget de retirer l'interface

}

/*! \brief Apres changement de la disponibilité de l'interface */
void _SwGuiConsole::eventAfterInterfaceAvailability(
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
void _SwGuiConsole::eventActivationChanged() 
{
}
