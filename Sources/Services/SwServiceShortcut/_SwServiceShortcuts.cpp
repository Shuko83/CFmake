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
using namespace StreamWork::SwCore;

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
void _SwServiceShortcuts::registerCommand( QString category, QString command, ISwShortcut *obj )
{
	if(obj && command != "")
	{

#ifdef _DEBUGAA
		qDebug() << "[Shortcut Services] - Register Command ("  <<obj->getCategory()<<","<<command<<")" ;
#endif
		if (_shortcutsName.indexOf(command) == -1)
			_shortcutsName.push_back(command);
		else
		{
			qCritical() << "A shortcut named " << command << " is already registered! the <"+category+"."+command+"> is ignored.";
			return;
		}

		QString cName = generateCombinedName(category, command);
		
		_mapShortcuts.insert(cName,obj);

		//Notify observer
		for (ISwShortcutObserver *obs : _listObs)
		{
			obs->add(category, command, obj);
		}
	}
}

//-------------------------------------------------------------------------
void _SwServiceShortcuts::unregisterCommand(QString category, QString command, ISwShortcut *obj)
{
	if(obj && command != "")
	{
		QString cName = generateCombinedName(category, command);

		if(_mapShortcuts.contains(cName))
		{

#ifdef _DEBUGAA
			qDebug() << "[Shortcut Services] - Unregister Command ("  <<obj->getCategory()<<","<<command<<")" ;
#endif
			//Notify observer
			for (ISwShortcutObserver *obs : _listObs)
			{
				obs->remove(category, command, obj);
			}

			_mapShortcuts.remove(cName);
			_shortcutsName.removeOne(command);
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
					qDebug() << "[Shortcut Services] - ProcessCommand ("<< getShortcutName(it.key()) <<","<<lShortcut->getCategory() << ")";
#endif
					lShortcut->processCommand(getShortcutName(it.key()));
				}
			}
 		}
 	}
}

//-------------------------------------------------------------------------
void _SwServiceShortcuts::bind( QString swdevice, QString shortcut )
{

#ifdef _DEBUGAA
	qDebug() << "[Shortcut Services] - bind Devices ("  << getCategory(swdevice)<< ","<< getShortcutName(swdevice) <<") on ("<< getCategory(shortcut)<<","<< getShortcutName(shortcut)<<")" ;
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
	qDebug() << "[Shortcut Services] - bind keyboard ("  <<sequence <<") on ("<< getCategory(shortcut) <<","<< getShortcutName(shortcut)<<")" ;
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
	for (QShortcut* tshortcut : _listKeyShortcut )
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

			QString category = getCategory(it.key());
			QString name = getShortcutName(it.key());

			obs->add(category, name, it.value());	//renommer getCategory et getShortcutName....
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
QString _SwServiceShortcuts::getCategory( QString val )
{
	return val.mid(0,val.indexOf(SC_SEP));
}

//-------------------------------------------------------------------------
QString _SwServiceShortcuts::getShortcutName( QString val )
{
	return val.mid(val.indexOf(SC_SEP)+QString(SC_SEP).length());
}

//-------------------------------------------------------------------------
QString _SwServiceShortcuts::generateCombinedName( QString prefix,QString suffix )
{
	return prefix+SC_SEP+suffix;
}

//-------------------------------------------------------------------------
void _SwServiceShortcuts::unBindAll()
{
	_mapDevicesAssoc.clear();
	_mapKeyboardAssoc.clear();
 
	for (QShortcut * ls : _listKeyShortcut)
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
void _SwServiceShortcuts::unBindDevices( QMap<QString,QList<QString>> devices )
{
	QMap<QString,QList<QString>>::iterator it;
	for(it = devices.begin(); it != devices.end(); it ++)
	{
		QString key = it.key();

		if(_mapDevicesAssoc.contains(key))
			_mapDevicesAssoc.remove(key);
	}
}

//-------------------------------------------------------------------------
void _SwServiceShortcuts::unBindShortcuts( QMap<QString,QList<QString>> shortcuts )
{
	// On disconnecte et supprime que les shortcuts de la liste passée en paramètre
	for (QShortcut* tshortcut : _listKeyShortcut )
	{
		QString StringSeq = tshortcut->key().toString();

		QMap<QString,QList<QString>>::iterator it;
		for(it = shortcuts.begin(); it != shortcuts.end(); it ++)
		{
			QList<QString> sequences = it.value();
			QString StringSeq2 = sequences.at(0);
			for(int i = 1; i<sequences.size(); i++)
			{
				StringSeq2 = StringSeq2+","+sequences.at(i);
			}
			// Pour les retrouver, on se base sur leur séquence associée
 			if (StringSeq == StringSeq2)
 			{
				if(_mapKeyboardAssoc.contains(it.key()))
					_mapKeyboardAssoc.remove(it.key());

				_listKeyShortcut.removeOne(tshortcut);

				shortcuts.remove(it.key());

 				disconnect(tshortcut, SIGNAL(activated()), this, SLOT(shortcutSlot()));
 				delete tshortcut;
 				tshortcut = NULL;
				break;
 			}
		}
	}
}


//-------------------------------------------------------------------------
void _SwServiceShortcuts::clearShortcutsService()
{
	// Le composant SxShortCutConfiguration lors du bindKeyBoard() créé les QShortCuts linké à son widget
	// Lorsque le clearShortcut() est appelé, tout ancien composant SxShortCutConfiguration a été deleté
	// et donc le widget et ses fils (des QShortcuts) aussi... pas besoin donc de les deleter à la mano...	
	_listKeyShortcut.clear();

	_mapDevicesAssoc.clear();
	_mapKeyboardAssoc.clear();

	_mapShortcuts.clear();
	_listDevices.clear();
	_listObs.clear();
}

//-------------------------------------------------------------------------
void _SwServiceShortcuts::shortcutSlot()
{
	if(_isConfigurationMode)
		return ;

	QShortcut * lShortcut= qobject_cast<QShortcut *> (sender());

	if(lShortcut)
	{
		//find all shortcut bind on the QShortcut sequenc
		QString sequenceAsString = lShortcut->key().toString();

		for(auto it = _mapKeyboardAssoc.begin(); it != _mapKeyboardAssoc.end(); ++it)
		{
			if(it.value().contains(sequenceAsString) && _mapShortcuts.contains(it.key()))
			{
				ISwShortcut * shortcut = _mapShortcuts.value(it.key());
				if(shortcut)
				{
#ifdef _DEBUGAA
					qDebug() << "[Shortcut Services] Keyboard - ProcessCommand ("<< getShortcutName(it.key()) <<","<<lShortcut->getCategory() << ")";
#endif
					shortcut->processCommand(getShortcutName(it.key()));
				}
			}
		}
	}
}
