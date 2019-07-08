#include "GlobalConf.h"
#include <QSettings>
#include <QApplication>
#include <QDir>

#define GLOBAL_CONF_INI qApp->applicationDirPath() + QDir::separator() + "GlobalConf.ini"

//---------------------------------------------------------------------------------
GlobalConf::GlobalConf()
    : QObject()
    , _isSplineLink(true)
{
	load();
}

//---------------------------------------------------------------------------------
GlobalConf * GlobalConf::instance()
{
    static GlobalConf instance;
	return &instance;
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
