#include "GlobalConf.h"
#include <QSettings>
#include <QApplication>
#include <QDir>

static GlobalConf * _instance = 0;

#define GLOBAL_CONF_INI qApp->applicationDirPath() + QDir::separator() + "GlobalConf.ini"

//---------------------------------------------------------------------------------
GlobalConf::GlobalConf() :QObject()
{
	_isSplineLink = true;
}


//---------------------------------------------------------------------------------
GlobalConf * GlobalConf::instance()
{
	if (_instance == 0)
	{
		_instance = new GlobalConf();
		_instance->load();
	}
	return _instance;
}

//-----------------------------------------------------------------------
void GlobalConf::load()
{
	QSettings settings(GLOBAL_CONF_INI, QSettings::IniFormat);
	_isSplineLink = settings.value("_isSplineLink", true).toBool();

}

//-----------------------------------------------------------------------
void GlobalConf::save()
{
	QSettings settings(GLOBAL_CONF_INI, QSettings::IniFormat);
	settings.setValue("_isSplineLink", _isSplineLink);
}

//---------------------------------------------------------------------------------
GlobalConf::~GlobalConf()
{
}

//-----------------------------------------------------------------------
bool GlobalConf::isSplineLink() const
{
	return _isSplineLink;
}

//-----------------------------------------------------------------------
void GlobalConf::setSplineLink(bool val)
{
	_isSplineLink = val;
	save();
}
