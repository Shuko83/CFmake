#include "SwDockWidget_MainDockConfiguration.h"

#include <QRadioButton>
#include <QWidgetAction>
#include <QMetaEnum>

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
	
	//Ajout des configurations principales
	ADDINLAYOUT(addConfiguration(":/DockWidget/rltb", RLTB));
	ADDINLAYOUT(addConfiguration(":/DockWidget/trlb", TRLB));
	ADDINLAYOUT(addConfiguration(":/DockWidget/brlt", BRLT));
	ADDINLAYOUT(addConfiguration(":/DockWidget/tbrl", TBRL));
	//Ajout des configurations secondaires, masquees par defaut
	_layout->addWidget(addSecondaryConfOption(":/Widget/expandDock"), 1, 3);

	ADDINLAYOUT(addConfiguration(":/DockWidget/bltr", BLTR, false));
	ADDINLAYOUT(addConfiguration(":/DockWidget/brtl", BRTL, false));
	ADDINLAYOUT(addConfiguration(":/DockWidget/tlbr", TLBR, false));
	ADDINLAYOUT(addConfiguration(":/DockWidget/trbl", TRBL, false));
	ADDINLAYOUT(addConfiguration(":/DockWidget/lbrt", LBRT, false));
	ADDINLAYOUT(addConfiguration(":/DockWidget/rblt", RBLT, false));
	ADDINLAYOUT(addConfiguration(":/DockWidget/ltrb", LTRB, false));
	ADDINLAYOUT(addConfiguration(":/DockWidget/rtlb", RTLB, false));
	ADDINLAYOUT(addConfiguration(":/DockWidget/rtbl", RTBL, false));
	ADDINLAYOUT(addConfiguration(":/DockWidget/ltbr", LTBR, false));
}

//-----------------------------------------------------------------------------
SwDockWidget_MainDockConfiguration::~SwDockWidget_MainDockConfiguration()
{
	_listBtn.clear();
}

//-----------------------------------------------------------------------------
QPushButton * SwDockWidget_MainDockConfiguration::addConfiguration(QString iconPath, ConfigurationIndex index, bool visible)
{
	//Creation du bouton
	QSize size = QImage(iconPath + "_on").size();
	QPushButton * button = new QPushButton(this);
	button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QIcon ico = QIcon();
	ico.addPixmap(QPixmap(iconPath + "_on"), QIcon::Selected, QIcon::On);
	ico.addPixmap(QPixmap(iconPath + "_off"), QIcon::Selected, QIcon::Off);
	button->setIcon(ico);
	button->setIconSize(size);
	button->resize(size);
	button->setFlat(true);
	button->setEnabled(true);
	button->setCheckable(true);
	button->setChecked(false);
	button->setAutoExclusive(true);
	button->setVisible(visible);
	connect(button, SIGNAL(toggled(bool)), this, SLOT(updateConf(bool)));
	_listBtn.insert(button, index);

	return button;
}

//-----------------------------------------------------------------------------
QPushButton * SwDockWidget_MainDockConfiguration::addSecondaryConfOption(QString iconPath)
{
	//Creation du bouton
	_btnMore = new QPushButton(this);
	_btnMore->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	_btnMore->setIcon(QIcon(iconPath));
	_btnMore->setIconSize(QSize(40, 8));
	_btnMore->setEnabled(true);
	connect(_btnMore, SIGNAL(clicked()), this, SLOT(showMore()));

	return _btnMore;
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainDockConfiguration::updateConf(bool state)
{
	QPushButton * button = qobject_cast<QPushButton*>(this->sender());

	if (button && state && _listBtn.contains(button))
	{
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

//-----------------------------------------------------------------------------
QString SwDockWidget_MainDockConfiguration::getActiveConfigurationInString()
{
	//Conversion de la configuration active en String
	const QMetaObject & mo = SwDockWidget_MainDockConfiguration::staticMetaObject;
	QMetaEnum me = mo.enumerator(mo.indexOfEnumerator("ConfigurationIndex"));
	return me.valueToKey(_activeConf);
}

//-----------------------------------------------------------------------------
QString SwDockWidget_MainDockConfiguration::getConfigurationInString(int index)
{
	//Conversion de la configuration active en String
	const QMetaObject & mo = SwDockWidget_MainDockConfiguration::staticMetaObject;
	QMetaEnum me = mo.enumerator(mo.indexOfEnumerator("ConfigurationIndex"));
	return me.valueToKey(index);
}

//----------------------------------------------------------------------------
void SwDockWidget_MainDockConfiguration::setActiveConfiguration(QString conf)
{
	if (!conf.isEmpty())
	{
		//Nombre de docks
		int nb = conf.length();
		SwDockWidget_MainDockConfiguration::ConfigurationIndex index = getExistingConf(conf, nb);
		if (index != SwDockWidget_MainDockConfiguration::NB_CONF)
			setActiveConfiguration(index);
	}
}

//----------------------------------------------------------------------------
SwDockWidget_MainDockConfiguration::ConfigurationIndex SwDockWidget_MainDockConfiguration::getExistingConf(QString conf, int nb)
{
	SwDockWidget_MainDockConfiguration::ConfigurationIndex index = SwDockWidget_MainDockConfiguration::NB_CONF;
	QString code("");
	const QMetaObject & mo = SwDockWidget_MainDockConfiguration::staticMetaObject;
	QMetaEnum me = mo.enumerator(mo.indexOfEnumerator("ConfigurationIndex"));

	//Recherche de compatibilite entre la configuration active et une configuration existante
	for (int i = 0; i < SwDockWidget_MainDockConfiguration::NB_CONF; i++)
	{
		code = QString(me.valueToKey(i));
		if (!code.left(nb).compare(conf))
		{
			//Si la configuration esty compatible, on l'active 
			return (SwDockWidget_MainDockConfiguration::ConfigurationIndex)i;
		}
	}

	//Gestion des cas non affiches car similaires a une configuration existante (symetrie)
	//TBRL est equivalent a TBLR, BTRL et BTLR
	if (!conf.compare(QString("TBLR").left(nb)) ||
		!conf.compare(QString("BTRL").left(nb)) ||
		!conf.compare(QString("BTLR").left(nb)))
	{
		index = SwDockWidget_MainDockConfiguration::TBRL;
	}

	//TRLB est equivalent a TLRB
	else if (!conf.compare(QString("TLRB").left(nb)))
	{
		index = SwDockWidget_MainDockConfiguration::TRLB;
	}

	//RTBL est equivalent a RBTL
	else if (!conf.compare(QString("RBTL").left(nb)))
	{
		index = SwDockWidget_MainDockConfiguration::RTBL;
	}

	//RLTB est equivalent a RLBT, LRBT et LRTB
	else if (!conf.compare(QString("RLBT").left(nb)) ||
			 !conf.compare(QString("LRBT").left(nb)) ||
			 !conf.compare(QString("LRTB").left(nb)))
	{
		index = SwDockWidget_MainDockConfiguration::RLTB;
	}

	//BRLT est equivalent a BLRT
	else if (!conf.compare(QString("BLRT").left(nb)))
	{
		index = SwDockWidget_MainDockConfiguration::BRLT;
	}

	//LTBR est equivalent a LBTR
	else if (!conf.compare(QString("LBTR").left(nb)))
	{
		index  =SwDockWidget_MainDockConfiguration::LTBR;
	}

	return index;
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainDockConfiguration::showMore()
{
	_btnMore->hide();

	//Affichage des configurations supplementaires
	QMapIterator<QPushButton *,ConfigurationIndex> it(_listBtn);
	while (it.hasNext()) 
	{
		it.next();
		it.key()->show();
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainDockConfiguration::hideEvent(QHideEvent *)
{
	//Masquage des configurations supplementaires
	QMapIterator<QPushButton *,ConfigurationIndex> it(_listBtn);
	while (it.hasNext()) 
	{
		it.next();
		if ((it.value() != RLTB) &&
			(it.value() != TRLB) &&
			(it.value() != BRLT) &&
			(it.value() != TBRL))
		{
			it.key()->hide();
		}
	}

	//Affichage du bouton "afficher plus"
	_btnMore->show();

	//Force la mise a jour des dimensions
	this->layout()->invalidate();
	this->layout()->activate();
}