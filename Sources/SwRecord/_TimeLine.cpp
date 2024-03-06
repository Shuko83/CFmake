/**
@file _TimeLine.cpp
@brief Ligne de temps de l'enregistrement
@author F.Bighelli
 */

#include "_TimeLine.h"
#include "SwRecordConstantes.h"
#include "Types/SwUUID.h"
#include "_RecordPoint.h"
 
using namespace StreamWork::SwCore;

typedef enum {StartDocElt,RecordElt,HeaderBeginElt,RecordPointBeginElt,RecordPointEndElt,HeaderEndElt,BodyElt} TElt;


//-------------------------------------------------------------------------
_TimeLine::_TimeLine() 
{
	_initTime	= 0;
	_startTime	= 0;
	_stopTime	= 0;
	_jumpTime   = 0;
	_pauseState = false;
	_state		= STATE_STOPPED;
}

//-------------------------------------------------------------------------
_TimeLine::~_TimeLine() 
{
    foreach( _ExecutionKey *ekey,_executionKeys) 
	{
        delete ekey;
    }
    _executionKeys.clear();
}

//-------------------------------------------------------------------------
void _TimeLine::reset() 
{
	_initTime				= 0;
	_startTime				= 0;
	_stopTime				= 0;
    _dataCounter			= 0;
    _executionKeyMaxSize	= 0;
	_pauseState				= false;
	setState(STATE_STOPPED);

    foreach( _ExecutionKey *ekey,_executionKeys) 
	{
        delete ekey;
    }

    _rpMap.clear();
    _executionKeys.clear();
}

//-------------------------------------------------------------------------
bool _TimeLine::loadRecordFile(QXmlStreamReader * reader,_SwServiceRecording * serviceRecord,QString repositoryName) 
{
    
	_repositoryName=repositoryName;
    reader->readNext();//Debug doc

    if(!reader->isStartDocument()) 
	{  
        return false;
    }
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); //Debug record
    if (!reader->isStartElement() || reader->name()!=CG_RECORD) 
	{ 
        return false;
    }
    //--------------------------------------------------------------------------
    // HEADER PART
    //--------------------------------------------------------------------------
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); //Debug header
    if (!reader->isStartElement() || reader->name()!=CG_RECORD_HEADER) 
        return false;
    if (reader->attributes().hasAttribute(CG_RECORD_TIME)) 
	{
        QString val=reader->attributes().value(QString(),CG_RECORD_TIME).toString();
        _initTime=val.toDouble();
    }
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); //Debug record point
    while(reader->isStartElement() && reader->name()==CG_RECORD_RP) 
	{
        if (reader->attributes().hasAttribute(CG_RECORD_RP_ATT_NUMBER) && 
            reader->attributes().hasAttribute(CG_RECORD_RP_ATT_NAME)) 
		{
            QString val=reader->attributes().value(QString(),CG_RECORD_RP_ATT_NUMBER).toString();
            int index=val.toInt();
            SwUUID uuid;
            val=reader->attributes().value(QString(),CG_RECORD_RP_ATT_NAME).toString();
            ISwRecordPoint *rp=serviceRecord->getRecordPoint(val);
            if (rp!=0) {
				rp->cleanKeys();
				rp->cleanProperty();
                _rpMap.insert(index,rp);
            }
        } 
        while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters);
        if(!reader->isEndElement()) //Fin record point
            return false;
        while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); //Debug record point || Fin header
    }
    if(!reader->isEndElement()) //Fin header
        return false;

    //--------------------------------------------------------------------------
    // BODY PART
    //--------------------------------------------------------------------------
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); //Debug body
    if (!reader->isStartElement() || reader->name()!=CG_RECORD_BODY) 
        return false;

    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); //Debug start
    qDebug(reader->tokenString().toLatin1().data());
    if (!reader->isStartElement() || reader->name()!=CG_RECORD_START) 
        return false;
    if (reader->attributes().hasAttribute(CG_RECORD_TIME)) 
	{
        QString val=reader->attributes().value(QString(),CG_RECORD_TIME).toString();
        _startTime=val.toDouble()+_initTime;
    }
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
    if(!reader->isEndElement()) //Fin start
        return false;

    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters);     

    if (reader->name()==CG_RECORD_STOP) 
	{
        QString val=reader->attributes().value(QString(),CG_RECORD_TIME).toString();
        _stopTime=val.toDouble()+_initTime;
        val=reader->attributes().value(QString(),CG_RECORD_STOP_DCOUNT).toString();
        _maxDataCounter=val.toInt();
    }
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
    if(!reader->isEndElement()) //Fin stop
        return false;
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
    if(!reader->isEndElement()) //Fin body
        return false;
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
    if(!reader->isEndElement()) //Fin record
        return false;
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
    if(!reader->isEndDocument ()) //Fin docuement
        return false;

    if (reader->hasError()) 
	{
        qDebug(reader->errorString().toLatin1().data());
        return false;
    }

    foreach(ISwReplayListener * listener,_listeners) 
	{
        listener->setStartTime(_initTime);   
        listener->setStopTime(_stopTime);   
        listener->setCurrentTime(_initTime);   
    }
    loadDataWriter();

    return true;
}



//-------------------------------------------------------------------------
double _TimeLine::queryInitTime() 
{
    setState(STATE_STARTED);

    return _initTime;
}

//-------------------------------------------------------------------------
double _TimeLine::queryStartTime() 
{
    return _startTime;
}

//-------------------------------------------------------------------------
double _TimeLine::queryExecuteTime(bool * isLast) 
{

	if(_jumpTime != 0)
	{

		//Todo Stocké dans 30sec avant et les relires rapidement
		QList<_ExecutionKey *> _tmpList;
		QList<_ExecutionKey *> _tmpList30;
		int diff = _executionKeys.front()->_currentTime - _initTime;

		while(diff < _jumpTime)
		{
			//Ajout dans la liste
			_ExecutionKey * ekey1=_executionKeys.front();
			_executionKeys.pop_front();
			_tmpList.append(ekey1);

			/*if(diff > 12 && diff < _jumpTime) 
				_tmpList30.append(ekey1);
*/
			if(_executionKeys.count() == 0)
			{
				//Clean ceux de la liste
				foreach(_ExecutionKey *tmp, _tmpList)
				{	
					//parcours la liste précédente
					/*double t=tmp->_currentTime;
					QList<ISwRecordPoint *>::iterator itrp=tmp->_recordPoints.begin();
					QList<int>::iterator itrpcall=tmp->_recordPointsCall.begin();
					while(itrp!=tmp->_recordPoints.end()) 
					{
						ISwRecordPoint * rp=(*itrp);
						_RecordPoint * toto = dynamic_cast<_RecordPoint*>(rp);
						if(toto)
						{
							ISwExecutable_Service *tst = dynamic_cast<ISwExecutable_Service*>(toto->QueryService(CG_SW_SERVICE_EXECUTABLE));
							
						
							int nbCall=(*itrpcall);
							for(int i=0;i<nbCall;i++) 
							{
								rp->submitKey();
								if (tst)
									tst->Execute(t,false);
							}
						}
						itrp++;
						itrpcall++;
						
					}*/
					

					for(ISwRecordPoint * tmpRP: tmp->_recordPoints)
					{
						tmpRP->cleanKeys();
					}
					
				}
				loadDataWriter();
			}

			diff = _executionKeys.front()->_currentTime - _initTime;

		}

		_jumpTime = 0;
		
		if(_executionKeys.count() == 0)
		{
			//Clean ceux de la liste
			for(_ExecutionKey *tmp : _tmpList)
			{	
				for(ISwRecordPoint * tmpRP : tmp->_recordPoints)
				{
					tmpRP->cleanKeys();
				}
			}
			loadDataWriter();
		}
		else
		{
			/*foreach(_ExecutionKey *tmp, _tmpList30)
			{
				//parcours la liste précédente
				double t=tmp->_currentTime;
				QList<ISwRecordPoint *>::iterator itrp=tmp->_recordPoints.begin();
				QList<int>::iterator itrpcall=tmp->_recordPointsCall.begin();
				while(itrp!=tmp->_recordPoints.end()) 
				{
					ISwRecordPoint * rp=(*itrp);
					int nbCall=(*itrpcall);
					for(int i=0;i<nbCall;i++) 
					{
						rp->submitKey();
					}
					itrp++;
					itrpcall++;
				}
			}*/
		}
	}

    if (_executionKeyMaxSize/2>_executionKeys.count() && _dataCounter<_maxDataCounter) 
	{
        loadDataWriter();
    }

    if (_executionKeys.isEmpty()) 
	{
        *isLast=true;
        return _stopTime;
    }
    _ExecutionKey * ekey=_executionKeys.front();
    _executionKeys.pop_front();
    if (_executionKeys.isEmpty()) 
	{
        *isLast=true;
        setState(STATE_STOPPED);
    }
    double t=ekey->_currentTime;
    QList<ISwRecordPoint *>::iterator itrp=ekey->_recordPoints.begin();
    QList<int>::iterator itrpcall=ekey->_recordPointsCall.begin();
    while(itrp!=ekey->_recordPoints.end()) 
	{
        ISwRecordPoint * rp=(*itrp);
        int nbCall=(*itrpcall);
        for(int i=0;i<nbCall;i++) 
		{
            rp->submitKey();
        }
        itrp++;
        itrpcall++;
    }

	QList<ISwRecordPoint *>::iterator itrp2=ekey->_recordPropertyPoints.begin();
	QList<int>::iterator itrpcall2=ekey->_recordPointsPropertyCall.begin();
	while(itrp2!=ekey->_recordPropertyPoints.end()) 
	{
		ISwRecordPoint * rp=(*itrp2);
		int nbCall=(*itrpcall2);
		for(int i=0;i<nbCall;i++) 
		{
			rp->submitProperty();
		}
		itrp2++;
		itrpcall2++;
	}

    delete ekey;
    foreach(ISwReplayListener * listener,_listeners) 
	{
        listener->setCurrentTime(t);   
    }
    return t;
}

//-------------------------------------------------------------------------
double _TimeLine::queryStopTime() 
{
   setState(STATE_STOPPED);

	return _stopTime;
}

//-------------------------------------------------------------------------
bool _TimeLine::queryPause() 
{
    if (_pauseState) 
	{
        setState(STATE_PAUSED);
		return _pauseState;
    }

    return false;
}

//-------------------------------------------------------------------------
bool _TimeLine::loadDataWriter() 
{
    QDir d=QDir(_repositoryName);
    QString name=QString(d.absolutePath()+QDir::separator()+CG_RECORD_FILE_DATA).arg(_dataCounter++);
    //Chargement du fichier d'enregistrement
    QFile *fileReader=new QFile(name);
    if (!fileReader->open(QIODevice::ReadOnly | QIODevice::Text)) 
	{        delete fileReader;
        fileReader=0;
        return false;
    }
    QXmlStreamReader * reader=new QXmlStreamReader(fileReader);

    reader->readNext();//Debug doc
    if(!reader->isStartDocument()) 
	{        
		return false;
    }
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); //Debug record
    if (!reader->isStartElement() || reader->name()!=CG_RECORD_DATA) 
	{
		        return false;
    }
    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 

    while(reader->isStartElement()) 
	{
		if (reader->name()==CG_RECORD_EXE) 
		{
            QString val=reader->attributes().value(QString(),CG_RECORD_TIME).toString();

            _ExecutionKey * ekey=new _ExecutionKey();
            ekey->_currentTime=val.toDouble()+_initTime;
            _executionKeys.push_back(ekey);

            while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 

        } 
		else if (reader->name()==CG_RECORD_KEY) 
		{
            QString val=reader->attributes().value(QString(),CG_RECORD_RP_ATT_NUMBER).toString();
            int index=val.toInt();

            while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 

            QMap<int,ISwRecordPoint *>::iterator it=_rpMap.find(index);
            int nbCall=0;
            if (it!=_rpMap.end()) 
			{
                while(reader->isStartElement()) 
				{
                    QString node_name=reader->name().toString();
                    it.value()->buildKey(reader);
                    nbCall++;

                    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters);

                    if (reader->isEndElement() && reader->name()==node_name)
                        while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
                }
            }
            _executionKeys.back()->_recordPoints.push_back(it.value());
            _executionKeys.back()->_recordPointsCall.push_back(nbCall);
		} else if (reader->name()==CG_RECORD_KEY_PROPERTY) 
		{
			QString val=reader->attributes().value(QString(),CG_RECORD_RP_ATT_NUMBER).toString();
			int index=val.toInt();

			while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 

			QMap<int,ISwRecordPoint *>::iterator it=_rpMap.find(index);
			int nbCall=0;
			if (it!=_rpMap.end()) 
			{
				while(reader->isStartElement()) 
				{
					QString node_name=reader->name().toString();
					it.value()->buildProperty(reader);
					nbCall++;

					while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters);

					if (reader->isEndElement() && reader->name()==node_name)
						while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
				}
			}
			_executionKeys.back()->_recordPropertyPoints.push_back(it.value());
			_executionKeys.back()->_recordPointsPropertyCall.push_back(nbCall);
		}else 
		{
            return false;
        }
        if(!reader->isEndElement()) 
		{
            //Fin key ou exe
            return false;
        }
        while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 
    }

    if(!reader->isEndElement()) //Fin record
        return false;

    while(!reader->atEnd() && reader->readNext()==QXmlStreamReader::Characters); 

    if(!reader->isEndDocument ()) //Fin docuement
        return false;

    if (reader->hasError()) 
	{
        qDebug(reader->errorString().toLatin1().data());
        return false;
    }

    delete reader;
    fileReader->close();
    delete fileReader;

    if (_executionKeyMaxSize==0)
        _executionKeyMaxSize=_executionKeys.count();

    foreach(ISwReplayListener * listener,_listeners) 
	{ 
        listener->setCacheTime(_executionKeys.back()->_currentTime);  
    }

    return true;
}

//-------------------------------------------------------------------------
void _TimeLine::addReplayManagerListener(ISwReplayListener * listener) 
{
	if (listener!=0)
        _listeners.push_back(listener);
}

//-------------------------------------------------------------------------
void _TimeLine::removeReplayManagerListener(ISwReplayListener * listener) 
{
	if (listener!=0)
        _listeners.removeOne(listener);
}

//-------------------------------------------------------------------------
void _TimeLine::setPauseState( bool val )
{
	_pauseState = val;
	if(_pauseState)
	{
		setState(STATE_PAUSED);
	}
	else
	{
		setState(STATE_STARTED);
	}
}

//-------------------------------------------------------------------------
void _TimeLine::setState( QString val )
{
	_state = val; 
	foreach(ISwReplayListener * listener,_listeners) 
	{
		listener->setState(_state);  
	}
}
