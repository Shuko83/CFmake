/**
 * Include locaux
 */
#include "_SwServiceShortcuts.h"

/**
 * Include Interface
 */
#include "ISwDevice.h"
#include "ISwShortcut.h"
#include "ISwShortcutObserver.h"

/**
 * include QT
 */
#include <QDebug>
#include <QShortcut>
#include <QApplication>

/**
 * Namespace
 */
using namespace StreamWork::SwGui;

//-------------------------------------------------------------------------
_SwServiceShortcuts::_SwServiceShortcuts() : _isConfigurationMode(false)
{
	// Rien de prévu ...
}

//-------------------------------------------------------------------------
_SwServiceShortcuts::~_SwServiceShortcuts()
{
	_mapDevicesAssoc.clear();
	_mapKeyboardAssoc.clear();

	_listDevices.clear();
	_mapShortcuts.clear();

	_listObs.clear();
	_listKeyShortcut.clear();

}


//-------------------------------------------------------------------------
void _SwServiceShortcuts::registerCommand( QString command, ISwShortcut *obj )
{
	if(obj && command != "")
	{

#ifdef _DEBUGAA
		qDebug() << "[Shortcut Services] - Register Command ("  <<obj->getName()<<","<<command<<")" ;
#endif

		QString cName = generateCombinedName(obj->getName(),command);
		_mapShortcuts.insert(cName,obj);

		//Notify observer
		foreach(ISwShortcutObserver *obs, _listObs)
		{
			obs->add(command,obj);
		}
	}
}

//-------------------------------------------------------------------------
void _SwServiceShortcuts::unregisterCommand( QString command, ISwShortcut *obj )
{
	if(obj && command != "")
	{
		QString cName = generateCombinedName(obj->getName(),command);

		if(_mapShortcuts.contains(cName))
		{

#ifdef _DEBUGAA
			qDebug() << "[Shortcut Services] - Unregister Command ("  <<obj->getName()<<","<<command<<")" ;
#endif
			//Notify observer
			foreach(ISwShortcutObserver *obs, _listObs)
			{
				obs->remove(command,obj);
			}

			_mapShortcuts.remove(cName);
		}
	}
}

//-------------------------------------------------------------------------
void _SwServiceShortcuts::registerDevice( ISwDevice * device )
{
	if(device)
	{
		_listDevices.append(device);

		device->registerObserver(this);

#ifdef _DEBUGAA
		qDebug() << "[Shortcut Services] - Register Devices ("  <<device->getName() << ")" ;
#endif

	}
}

//-------------------------------------------------------------------------
void _SwServiceShortcuts::unregisterDevice( ISwDevice * device )
{
	if(device)
	{
		_listDevices.removeOne(device);

		device->unregisterObserver(this);

#ifdef _DEBUGAA
		qDebug() << "[Shortcut Services] - Unregister Devices ("  <<device->getName() << ")" ;
#endif
	}
}

//-------------------------------------------------------------------------
void _SwServiceShortcuts::notify(QString entryName, ISwDevice *device)
{
	if(_isConfigurationMode)
		return ;

 	if(entryName != "" && device)
 	{
		QString cName = generateCombinedName(device->getName(),entryName);

 		QMap<QString,QList<QString>>::iterator it;
 
 		for(it = _mapDevicesAssoc.begin(); it != _mapDevicesAssoc.end(); it ++)
 		{
			if(it.value().contains(cName) && _mapShortcuts.contains(it.key()))
			{
				ISwShortcut * lShortcut = _mapShortcuts.value(it.key());

				if(lShortcut)
				{
#ifdef _DEBUGAA
					qDebug() << "[Shortcut Services] - ProcessCommand ("<< getParameter(it.key()) <<","<<lShortcut->getName() << ")";
#endif
					lShortcut->processCommand(getParameter(it.key()));
				}
			}
 		}
 	}
}

//-------------------------------------------------------------------------
void _SwServiceShortcuts::bind( QString swdevice, QString shortcut )
{

#ifdef _DEBUGAA
	qDebug() << "[Shortcut Services] - bind Devices ("  << getName(swdevice)<< ","<< getParameter(swdevice) <<") on ("<< getName(shortcut)<<","<< getParameter(shortcut)<<")" ;
#endif

	//On a deja un raccourci pour cette touche on ajoute a la liste
	if(_mapDevicesAssoc.contains(shortcut))
	{
		//on ajoute
		if(!_mapDevicesAssoc.value(shortcut).contains(swdevice))
			_mapDevicesAssoc[shortcut].append(swdevice);
	}
	else
	{
		QList<QString> lList;
		lList<< swdevice;
		_mapDevicesAssoc.insert(shortcut,lList);

	}
}

//-------------------------------------------------------------------------
void _SwServiceShortcuts::bindKeyboard( QString sequence, QString shortcut, QWidget * context )
{

#ifdef _DEBUGAA
	qDebug() << "[Shortcut Services] - bind keyboard ("  <<sequence <<") on ("<< getName(shortcut) <<","<< getParameter(shortcut)<<")" ;
#endif

	//On a deja un raccourci pour cette touche on ajoute a la liste
	if(_mapKeyboardAssoc.contains(shortcut))
	{
		//on ajoute
		if(!_mapKeyboardAssoc.value(shortcut).contains(sequence))
			_mapKeyboardAssoc[shortcut].push_front(sequence);
	}
	else
	{
		QList<QString> lList;
		lList<< sequence;
		_mapKeyboardAssoc.insert(shortcut,lList);
	}


	bool isAllReadyAShortcut = false;
	foreach(QShortcut* tshortcut, _listKeyShortcut )
	{
		if(tshortcut->key().toString() == sequence)
			isAllReadyAShortcut = true;

	}
	if(!isAllReadyAShortcut)
	{
		//Register QApp Shortcut
		QShortcut* lshortcut = new QShortcut(QKeySequence::fromString(sequence),context);
		lshortcut->setContext(Qt::ApplicationShortcut);
		connect(lshortcut,SIGNAL(activated()),this,SLOT(shortcutSlot()));

		_listKeyShortcut.append(lshortcut);
	}
	

}

//-------------------------------------------------------------------------
QList<ISwDevice*> _SwServiceShortcuts::getDevices()
{
	return _listDevices;
}

//-------------------------------------------------------------------------
void _SwServiceShortcuts::registerShortcutObserver( ISwShortcutObserver * obs )
{
	_listObs.append(obs);

	//Notify observer
	QMap<QString,ISwShortcut*>::iterator it;
	for(it = _mapShortcuts.begin(); it != _mapShortcuts.end(); it ++)
	{
		if(it.value())
		{
			obs->add(getParameter(it.key()),it.value());
		}
	}
}

//-------------------------------------------------------------------------
void _SwServiceShortcuts::unregisterShortcutObserver( ISwShortcutObserver * obs )
{
	if(_listObs.contains(obs))
	{
		_listObs.removeOne(obs);
	}
}

//-------------------------------------------------------------------------
void _SwServiceShortcuts::setConfigMode( bool value )
{
	_isConfigurationMode = value;
}

//-------------------------------------------------------------------------
QMap<QString,QList<QString>> _SwServiceShortcuts::getBindedDevices()
{
	return _mapDevicesAssoc;
}

//-------------------------------------------------------------------------
QMap<QString,QList<QString>> _SwServiceShortcuts::getBindedKeyboard()
{
	return _mapKeyboardAssoc;
}

//-------------------------------------------------------------------------
QString _SwServiceShortcuts::getName( QString val )
{
	return val.mid(0,val.indexOf(SC_SEP));
}

//-------------------------------------------------------------------------
QString _SwServiceShortcuts::getParameter( QString val )
{
	return val.mid(val.indexOf(SC_SEP)+QString(SC_SEP).length());
}

//-------------------------------------------------------------------------
QString _SwServiceShortcuts::generateCombinedName( QString prefix,QString suffix )
{
	return prefix+SC_SEP+suffix;
}

//-------------------------------------------------------------------------
void StreamWork::SwGui::_SwServiceShortcuts::unBindAll()
{
	_mapDevicesAssoc.clear();
	_mapKeyboardAssoc.clear();
 
 	foreach(QShortcut * ls , _listKeyShortcut)
 	{
		if(ls)
		{
 			disconnect(ls,SIGNAL(activated()),this,SLOT(shortcutSlot()));
			delete ls;
			ls= NULL;
		}
 	}

	_listKeyShortcut.clear();
}

//-------------------------------------------------------------------------
void _SwServiceShortcuts::shortcutSlot()
{
	if(_isConfigurationMode)
		return ;


	QShortcut * lShortcut= qobject_cast<QShortcut *> (sender());

	if(lShortcut)
	{
		//find all shortcut bind on the QShortcut sequence

		QString sequenceAsString = lShortcut->key().toString();
		QMap<QString,QList<QString>>::iterator it = _mapKeyboardAssoc.begin();
		QMap<QString,QList<QString>>::iterator end = _mapKeyboardAssoc.end();

		for(it ; it != end ; ++it)
		{
			if(it.value().contains(sequenceAsString) && _mapShortcuts.contains(it.key()))
			{
				ISwShortcut * lShortcut = _mapShortcuts.value(it.key());
				if(lShortcut)
				{
#ifdef _DEBUGAA
					qDebug() << "[Shortcut Services] Keyboard - ProcessCommand ("<< getParameter(it.key()) <<","<<lShortcut->getName() << ")";
#endif
					lShortcut->processCommand(getParameter(it.key()));
				}
			}
		}
	}
}
