#ifndef _SWRESPONSE_H
#define _SWRESPONSE_H

//Local includes
#include "IResponse.h"



namespace StreamWork
{
    namespace SwCore
    {
        class SwResponse : public alert::IResponse
        {
        public:

            SwResponse(QString name, QString desc);
            ~SwResponse();


            virtual void execute();
            virtual QString getDescription();
            virtual QString getName();


            void setDescription(QString);
            void setName(QString);

        private:
            
            QString _desc;
            QString _name;


        };
    }
}


#endif