#include "ManageLinkColor.h"

ManageLinkColor * ManageLinkColor::_instance = NULL;

#define ColorIni qApp->applicationDirPath() + QDir::separator() + "LinkColor.ini"

ManageLinkColor::ManageLinkColor(void)
{
}

ManageLinkColor::~ManageLinkColor(void)
{
}

//-------------------------------------------------------------------------
ManageLinkColor * ManageLinkColor::getInstance()
{
	if(_instance == NULL)
	{
		_instance = new ManageLinkColor();
		_instance->loadStaticColor();
	}
	return _instance;
}

//-------------------------------------------------------------------------
QColor ManageLinkColor::getColorForInterface( QString interfaceName )
{
	QString hexColor = "#FF9900";
	
	QMapIterator<QString,QString> it(_mapColor);
	while(it.hasNext())
	{
		it.next();
		if(interfaceName.contains(it.key()))
		{
			return QColor(it.value());
		}
	}
	return QColor(hexColor);
}

//-----------------------------------------------------------------------
void ManageLinkColor::loadStaticColor()
{
	if (QFile::exists(ColorIni))
	{
		QSettings settings(ColorIni, QSettings::IniFormat);
		QStringList allKeys = settings.allKeys();
		for (QString val : allKeys)
			if (settings.value(val).toString().startsWith("#"))
				_mapColor.insert(val, settings.value(val).toString());
	}

	if(_mapColor.count() == 0) // moche mais pour erase les anciennes valeurs locales
	{
		_mapColor.clear();
		_mapColor.insert("ISwWidget", "#9E6DFF");
		_mapColor.insert("ISxModel", "#40E1E7");
		_mapColor.insert("ISwAction", "#AAE740");
		_mapColor.insert("ISwToolBar", "#E7408B");
		_mapColor.insert("ISwLayout", "#074f8e");
		_mapColor.insert("IGuDatabase", "#ff0000");
		_mapColor.insert("IGuReplay", "#FDFF55");
		_mapColor.insert("ISxDatabase", "#2d2dff");
		_mapColor.insert("ISwEvent", "#24cd1c");
		_mapColor.insert("ICartoSDK", "#2d2dff");
		_mapColor.insert("ICartoProperties", "#e52da6");
		_mapColor.insert("IVgeoObjectsManager", "#50e616");
		_mapColor.insert("ISxDBAccess", "#0391da");

		QSettings settings(ColorIni, QSettings::IniFormat);
		for (QString val : _mapColor.keys())
			settings.setValue(val, _mapColor[val]);
	}
}
