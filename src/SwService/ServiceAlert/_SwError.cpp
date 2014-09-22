#include "_SwError.h"

//QT includes

//Other External includes"

//Local includes

using namespace StreamWork::SwCore;
using namespace alert;

//---------------------------------------------------------------------------------
SwError::SwError(QString title, QString description, alert::EnumErrorLevel::ErrorLevel level)
:Error(ProcessId())
{
	setIndex(-1);
	setTitle(title);
	setDescription(description);
	setlevel(level);
	
}
//-------------------------------------------------------------------
SwError::~SwError()
{

}
