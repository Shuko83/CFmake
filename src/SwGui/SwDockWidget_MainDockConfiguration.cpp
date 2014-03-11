#include "SwDockWidget_MainDockConfiguration.h"

#include <QRadioButton>
#include <QWidgetAction>
#include <QMetaEnum>
//#include <QDebug>

#define ADDINLAYOUT(a) {_layout->addWidget(a, num/4, num%4); num++;}

//-----------------------------------------------------------------------------
SwDockWidget_MainDockConfiguration::SwDockWidget_MainDockConfiguration(QString title, QWidget * parent)
: QMenu(title, parent), _activeConf(BLTR), _layout(NULL)
{
	//Layout contenant les boutons
	_layout = new QGridLayout(this);
	_layout->setMargin(0);
	QWidgetAction * action = new QWidgetAction(this);
	action->setDefaultWidget(_layout->widget());
	//Organisation du layout
	int num = 0;
	
	//Ajout des configurations
	ADDINLAYOUT(addConfiguration(":/DockWidget/images/DockWidget/rltb.png", RLTB));
	ADDINLAYOUT(addConfiguration(":/DockWidget/images/DockWidget/trlb.png", TRLB));
	ADDINLAYOUT(addConfiguration(":/DockWidget/images/DockWidget/brlt.png", BRLT));
	ADDINLAYOUT(addConfiguration(":/DockWidget/images/DockWidget/tbrl.png", TBRL));
	ADDINLAYOUT(addConfiguration(":/DockWidget/images/DockWidget/bltr.png", BLTR));
	ADDINLAYOUT(addConfiguration(":/DockWidget/images/DockWidget/brtl.png", BRTL));
	ADDINLAYOUT(addConfiguration(":/DockWidget/images/DockWidget/tlbr.png", TLBR));
	ADDINLAYOUT(addConfiguration(":/DockWidget/images/DockWidget/trbl.png", TRBL));
	ADDINLAYOUT(addConfiguration(":/DockWidget/images/DockWidget/lbrt.png", LBRT));
	ADDINLAYOUT(addConfiguration(":/DockWidget/images/DockWidget/rblt.png", RBLT));
	ADDINLAYOUT(addConfiguration(":/DockWidget/images/DockWidget/ltrb.png", LTRB));
	ADDINLAYOUT(addConfiguration(":/DockWidget/images/DockWidget/rtlb.png", RTLB));
	ADDINLAYOUT(addConfiguration(":/DockWidget/images/DockWidget/rtbl.png", RTBL));
	ADDINLAYOUT(addConfiguration(":/DockWidget/images/DockWidget/ltbr.png", LTBR));
}

//-----------------------------------------------------------------------------
SwDockWidget_MainDockConfiguration::~SwDockWidget_MainDockConfiguration()
{
	_listBtn.clear();
}

//-----------------------------------------------------------------------------
QRadioButton * SwDockWidget_MainDockConfiguration::addConfiguration(QString iconPath, ConfigurationIndex index)
{
	//Creation du bouton
	QRadioButton * button = new QRadioButton(this);
	button->setIcon(QIcon(iconPath));
	button->setIconSize(QSize(50,50));
	button->setEnabled(true);
	connect(button, SIGNAL(toggled(bool)), this, SLOT(updateConf(bool)));
	_listBtn.insert(button, index);

	return button;
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainDockConfiguration::updateConf(bool state)
{
	QRadioButton * button = qobject_cast<QRadioButton*>(this->sender());

	if (button && state && _listBtn.contains(button))
	{
		_activeConf = _listBtn.value(button);
		emit changeConf(_listBtn.value(button));
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainDockConfiguration::setActiveConfiguration(ConfigurationIndex index)
{
	if (index < NB_CONF)
	{
		_listBtn.key(index)->blockSignals(true);
		_listBtn.key(index)->setChecked(true);
		_activeConf = index;
		_listBtn.key(index)->blockSignals(false);
	}
}

//-----------------------------------------------------------------------------
SwDockWidget_MainDockConfiguration::ConfigurationIndex SwDockWidget_MainDockConfiguration::getActiveConfiguration()
{
	return _activeConf;
}

//----------------------------------------------------------------------------
void SwDockWidget_MainDockConfiguration::setActiveConfiguration(QString conf)
{
	if (!conf.isEmpty())
	{
		const QMetaObject & mo = SwDockWidget_MainDockConfiguration::staticMetaObject;
		QMetaEnum me = mo.enumerator(mo.indexOfEnumerator("ConfigurationIndex"));

		//Nombre de docks
		int nb = conf.length();
		QString code("");
		for (int i = 0; i < SwDockWidget_MainDockConfiguration::NB_CONF; i++)
		{
			code = QString(me.valueToKey(i));
			if (!code.left(nb).compare(conf))
			{
				setActiveConfiguration((SwDockWidget_MainDockConfiguration::ConfigurationIndex)i);
				return;
			}
		}

		//Gestion des cas non affiches car similaires a une configuration existante
		if (!conf.compare(QString("TBLR").left(nb)) ||
			!conf.compare(QString("BTRL").left(nb)) ||
			!conf.compare(QString("BTLR").left(nb)))
			setActiveConfiguration(SwDockWidget_MainDockConfiguration::TBRL);
		else if (!conf.compare(QString("TLRB").left(nb)))
			setActiveConfiguration(SwDockWidget_MainDockConfiguration::TRLB);
		else if (!conf.compare(QString("RBTL").left(nb)))
			setActiveConfiguration(SwDockWidget_MainDockConfiguration::RTBL);
		else if (!conf.compare(QString("RLBT").left(nb)) ||
				 !conf.compare(QString("LRBT").left(nb)) ||
				 !conf.compare(QString("LRTB").left(nb)))
			setActiveConfiguration(SwDockWidget_MainDockConfiguration::RLTB);
		else if (!conf.compare(QString("BLRT").left(nb)))
			setActiveConfiguration(SwDockWidget_MainDockConfiguration::BRLT);
		else if (!conf.compare(QString("LBTR").left(nb)))
			setActiveConfiguration(SwDockWidget_MainDockConfiguration::LTBR);
	}
}