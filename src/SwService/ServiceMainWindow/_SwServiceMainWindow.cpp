/**
 * Include locaux
 */
#include "_SwServiceMainWindow.h"

/**
 * Include Interface
 */
#include "ISwServiceMainWindow.h"

/**
 * include QT
 */
#include <QDebug>
#include <QApplication>
#include "SwMacros.h"
#include <QTimer>
using namespace StreamWork::Service;

//-------------------------------------------------------------------------
_SwServiceMainWindow::_SwServiceMainWindow() : QObject()
{
	_window = nullptr;
}

//-----------------------------------------------------------------------
_SwServiceMainWindow::_SwServiceMainWindow(QMainWindow *window)
{
	_window = window;
	/*QTimer::singleShot(0, [&]()->void{ */SW_APP->RegisterService(this);/* });*/
}

//-------------------------------------------------------------------------
_SwServiceMainWindow::~_SwServiceMainWindow()
{
	SW_APP->UnregisterService(GetServiceName());
}

//-----------------------------------------------------------------------
QMainWindow* _SwServiceMainWindow::getMainWindow()
{
	return _window;
}

//-----------------------------------------------------------------------
void _SwServiceMainWindow::setMainWindow(QMainWindow *window)
{
	_window = window;
}

