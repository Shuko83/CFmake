#include "_SwResponse.h"




StreamWork::SwCore::SwResponse::SwResponse(QString name, QString desc) : _desc(desc), _name(name)
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
}
