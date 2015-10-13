#include "_SwResponse.h"
#include "ErrorEnd.h"




StreamWork::SwCore::SwResponse::SwResponse(QString name, QString desc, alert::AlertManager * am, alert::ProcessId source) :Response(source), _desc(desc), _name(name), _am(am)
{

}

StreamWork::SwCore::SwResponse::~SwResponse()
{

}


void StreamWork::SwCore::SwResponse::setDescription(QString desc)
{
    _desc=desc;
}

void StreamWork::SwCore::SwResponse::setName(QString name)
{
    _name=name;
}

QString StreamWork::SwCore::SwResponse::getDescription()
{
    return _desc;
}

QString StreamWork::SwCore::SwResponse::getName()
{
    return _name;
}

void StreamWork::SwCore::SwResponse::execute()
{
    qDebug() << "Executing SwRespond " << _name;

    // envoi fin d'erreur
    alert::ProcessId processId(QUuid::createUuid(), 0);
    alert::PRefIErrorEnd errorEnd = new alert::ErrorEnd(processId, getProcessId());
    // leveé de l'alert
    if (_am)
    {
        _am->raiseErrorEnd(errorEnd);
    }
}
