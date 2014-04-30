#include "SwDockWidget_MainDockMenuAction.h"

//-----------------------------------------------------------------------------
SwDockWidget_MainDockMenuAction::SwDockWidget_MainDockMenuAction(QObject * parent, QString enabledIcon, QString disabledIcon, QString checkedIcon)
:QAction(parent)
{
	_enabled = QIcon(enabledIcon);
	_disabled = QIcon(disabledIcon);
	_checked = QIcon(checkedIcon);

	//Creation de l'action
	setIcon(_disabled);
	setCheckable(true);
	setEnabled(false);

	//Mise a jour de l'icon
	connect(this, SIGNAL(changed()), this, SLOT(updateState()));
}

//-----------------------------------------------------------------------------
SwDockWidget_MainDockMenuAction::~SwDockWidget_MainDockMenuAction()
{
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainDockMenuAction::updateState()
{
	this->blockSignals(true);
	//Action desactivee
	if (!isEnabled())
	{
		if (!_disabled.isNull())
			setIcon(_disabled);
	}
	else
	{
		//Si actif
		if (isChecked())
		{
			if (!_checked.isNull())
				setIcon(_checked);
		}
		//Si inactif
		else
		{
			if (!_enabled.isNull())
				setIcon(_enabled);
		}
	}
	this->blockSignals(false);
}