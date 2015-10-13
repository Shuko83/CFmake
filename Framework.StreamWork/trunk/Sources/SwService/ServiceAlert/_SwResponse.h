#ifndef _SWRESPONSE_H
#define _SWRESPONSE_H

//Local includes
#include "Response.h"
#include "AlertManager.h"



namespace StreamWork
{
    namespace SwCore
    {
        class SwResponse : public alert::Response
        {
        public:

            SwResponse(QString name, QString desc, alert::AlertManager * am, alert::ProcessId source);
            ~SwResponse();


            virtual void execute();
            virtual QString getDescription();
            virtual QString getName();


            void setDescription(QString);
            void setName(QString);

        private:
            
            QString _desc;
            QString _name;

            alert::AlertManager * _am;
        };
    }
}


#endif