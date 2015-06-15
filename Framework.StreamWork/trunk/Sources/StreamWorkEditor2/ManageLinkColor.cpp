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
		foreach(QString val, allKeys)
			if (settings.value(val).toString().startsWith("#"))
				_mapColor.insert(val, settings.value(val).toString());
	}
	else
	{
		if (_mapColor.isEmpty())
		{
			_mapColor.insert("ISwWidget", "#9E6DFF");
			_mapColor.insert("ISxModel", "#40E1E7");
			_mapColor.insert("ISwAction", "#AAE740");
			_mapColor.insert("ISwToolBar", "#E7408B");
			_mapColor.insert("ISwQDockWidget", "#FDFF55");
			_mapColor.insert("ISwDockWidget", "#FDFF55");
			_mapColor.insert("ISwLayout", "#032F55");
			_mapColor.insert("IPlgGlobalContext", "#2d2dff");
			_mapColor.insert("ITdlDbRootFacade", "#6f0cae");
			_mapColor.insert("IDataLink", "#24cd1c");
			_mapColor.insert("IProtocolProvider", "#e52da6");
			_mapColor.insert("IExchangeObjectProvider", "#ff0000");
			//_mapColor.insert("IPlgContext","#e95a1a"); //rdyToUse

			QSettings settings(ColorIni, QSettings::IniFormat);
			for (QString val : _mapColor.keys())
				settings.setValue(val, _mapColor[val]);
		}
	}
}
