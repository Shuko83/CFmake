/*
* INCLUDES LOCAUX
*/
#include "SwUnloadedATOAliasError.h"

/*
* INCLUDES GLOBAUX
*/
#include <QFileInfo>

namespace SwServiceAlias
{
	//---------------------------------------------------------------------------------
	SwUnloadedATOAliasError::SwUnloadedATOAliasError(alert::ProcessId processId, QString path)
		: alert::Error(processId)
	{
		setlevel(alert::EnumErrorLevel::Major);
		setIndex(-1);
		QString desc("The ATO file cannot be loaded : ");
		desc.append(QFileInfo(path).fileName());
		setDescription(desc);
	}

	//---------------------------------------------------------------------------------
	SwUnloadedATOAliasError::~SwUnloadedATOAliasError()
	{}
}  // namespace SwServiceAlias