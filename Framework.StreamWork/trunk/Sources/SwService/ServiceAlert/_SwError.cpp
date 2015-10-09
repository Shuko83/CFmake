#include "_SwError.h"
#include "_SwResponse.h"

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

    if (level == alert::EnumErrorLevel::Critical || level == alert::EnumErrorLevel::Major)
    {
        addResponse(new StreamWork::SwCore::SwResponse("SwResponse1", "SwResponse1 desc"));
        addResponse(new StreamWork::SwCore::SwResponse("SwResponse2", "SwResponse2 desc"));
        addResponse(new StreamWork::SwCore::SwResponse("SwResponse3", "SwResponse3 desc"));
    }
	
}

//-------------------------------------------------------------------
SwError::~SwError()
{

}

