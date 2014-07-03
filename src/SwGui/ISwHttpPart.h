/*!
 \file ISwHttpPart.h
 \brief interface partie serveur http
 \date 23-août-2006 16:04:34
 \author F.Bighelli
*/

#ifndef _ISwHttpPart_H
#define _ISwHttpPart_H

/*
  * INCLUDES GLOBAUX
  */
#include <QString>
#include <QByteArray>
//#include <QHttpRequestHeader>
#include <QIODevice>

namespace StreamWork
{
	namespace SwGui
	{
        /*!
		 \interface ISwHttpPart 
		 \brief interface definissant un groupe de page http
         \ingroup SwGuiGrp
         \swinterface
        
        Mise en oeuvre (page _about du server) 
        @see _SwGuiCompHttpServer
        @code 
        // Renvoie le path de la partie 
        QString _SwGuiCompHttpServer::getPath() {
            return "/_about";
        }

        // brief process de la requete 
        void _SwGuiCompHttpServer::processRequest(QHttpRequestHeader * request,
                                                    QMap<QString,QString> * parameters,
                                                    QByteArray * body,
                                                QIODevice * device) {
            if (request->path()=="/_about.jpg") {
                QFile image(":/SwGui/about.jpg");
                image.open(QIODevice::ReadOnly);
                QByteArray data=image.readAll();
                image.close();
                QHttpResponseHeader httpreponse(200,"Ok",1,1);
                httpreponse.setContentType("image/jpeg");
                httpreponse.setContentLength(data.size());
                device->write(httpreponse.toString().toUtf8());
                device->write(data);
                return;
            }
            //Si non trouve, renvoie erreur 404
            QHttpResponseHeader httpreponse(200,"Ok",1,1);
            httpreponse.setContentType("text/html");
            httpreponse.setValue("charset","utf-8");

            QTextStream os(device);
            os << httpreponse.toString() << "<link rel=\"icon\" type=\"image/jpg\" href=\"/_about.jpg\" /><center><img src=\"/_about.jpg\"><h1> Tiny Streamwork HttpServer version "<< CG_STREAMWORK_VERSION <<" </h1><img src=\"/_about.jpg\"></center>";
            os.flush();
        }
        @endcode
        Pour plus de details voir les sources du swlauncher.

         
         */
        class ISwHttpPart 
		{   
		public:
            /*! @brief Renvoie le path de la partie  */
			virtual QString  getPath()=0;
            /*! @brief process de la requete */
            virtual void processRequest(/*QHttpRequestHeader * request,
                                        QMap<QString,QString> * parameters,
                                        QByteArray * body,
                                        QIODevice * device
                                        */)=0;
		};
	}
}
#endif 
