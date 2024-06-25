/**
* @file _RecordManager.cpp
* @brief Record Manager
* @version 1.0
* @date Tue Apr 28 15:55:16 CEST 2009
* @author F.Bighelli
*/


#include "Main/SwApplication.h"
#include "Main/SwMacros.h"
#include "_RecordManager.h"
#include "SwRecordConstantes.h"
#include <QDirIterator>

using namespace StreamWork::SwCore;

//-------------------------------------------------------------------------
_RecordManager::_RecordManager(_SwServiceRecording * serviceRecord):SwAssistedComponent() 
{
	setExecutableServiceAvaibility(true);

	_serviceRecord		= serviceRecord;
	_writer				= NULL;
	_writerData			= NULL;
	_fileWriter			= NULL;
	_itime				= 0.0;
	_currentTime		= 0.0;
	_isRecording		= false;
	_maxRecordSize		= 100000; //100K
	_repository			= SwFileDescriptor(SwFileDescriptor::DirectorySelect,QString(),QString());


}

//-------------------------------------------------------------------------
_RecordManager::~_RecordManager() 
{
	//Stop record if we are recording
	if(_isRecording)
		stopRecording();

	unprovideInterface("RecordManager");
}

//-------------------------------------------------------------------------
void _RecordManager::initializeComponent() throw(SwException)
{
	provideInterface<ISwRecordManager>("RecordManager",(ISwRecordManager *)this);
	createPropertiesForThisObject(QString(),true);
}


//-------------------------------------------------------------------------
void _RecordManager::setRecordDirectory(QString directoryName) 
{
	_repository.setFileName(directoryName);
}

//-------------------------------------------------------------------------
void _RecordManager::startRecording()
{
	//Reset Total Size
	_totalSize = 0;

	QDir d=QDir(_repository.getFileName());

	if (!d.exists())
	{
		QDir::temp().mkpath(_repository.getFileName());
	}

	//Creation du fichier d'enregistrement
	_fileWriter = new QFile(d.absolutePath()+QDir::separator()+CG_RECORD_FILE);

	if (!_fileWriter->open(QIODevice::WriteOnly | QIODevice::Text)) 
	{
		delete _fileWriter;
		_fileWriter=0;
		return;
	}

	_dataCounter = 0;

	//Creation du writer
	_writer = new QXmlStreamWriter(_fileWriter);
	_writer->setAutoFormatting(true);
	_writer->writeStartDocument("1.0");
	_writer->writeStartElement(CG_RECORD);

	_itime = _currentTime;

	if( _writer != NULL ) 
	{
		foreach(ISwRecordManagerListener * listener,_listeners) 
		{
			listener->setStartRecTime(_itime);
			listener->setDataSize(0); // reset Size

		}
		_writer->writeStartElement(CG_RECORD_HEADER);
		_writer->writeAttribute(CG_RECORD_TIME,QString("%1").arg(_itime,0,'f',3));

		//Construction et enregistrement du mapping des points d'enregistrement
		buildRecordPointMapping();

		//Fin header
		_writer->writeEndElement();
		_writer->writeStartElement(CG_RECORD_BODY);
		//Enregistrement du record manager au pres des record points 
		QList<ISwRecordPoint *> points=_serviceRecord->getRecordPoints();
		foreach(ISwRecordPoint * rpoint,points) 
		{
			rpoint->setRecordManager(this);
		}

		_writer->writeStartElement(CG_RECORD_START);
		_writer->writeAttribute(CG_RECORD_TIME,QString("%1").arg(_currentTime-_itime,0,'f',3));
		_writer->writeEndElement();

	}

	//Creation du writer data
	createWriterData();

	setRecordingState(true);
}

//-------------------------------------------------------------------------
void _RecordManager::stopRecording()
{
	if (_writerData != NULL) 
	{
		closeWriterData();
	}

	if (_writer != NULL) 
	{
		_writer->writeStartElement(CG_RECORD_STOP);
		_writer->writeAttribute(CG_RECORD_TIME,QString("%1").arg(_currentTime-_itime,0,'f',3));
		_writer->writeAttribute(CG_RECORD_STOP_DCOUNT,QString("%1").arg(_dataCounter));
		_writer->writeEndElement();
		
		//Desenregistrement du record manager au pres des record points 
		QList<ISwRecordPoint *> points=_serviceRecord->getRecordPoints();
		foreach(ISwRecordPoint * rpoint,points) 
		{
			rpoint->setRecordManager(0);
		}

		_writer->writeEndElement(); //Body
		_writer->writeEndElement(); //Record

		//Fin du doc
		_writer->writeEndDocument();

		//Destruction du writer
		delete _writer;
		_writer = NULL;

		_fileWriter->close();
		delete _fileWriter;
		_fileWriter=0;
	}

	setRecordingState(false);
	setMainDir(_mainDir);
}

//-------------------------------------------------------------------------
QXmlStreamWriter *_RecordManager::queryRecordKey(ISwRecordPoint * recordPoint,double currentTime) 
{
	QMap<ISwRecordPoint *,int>::iterator it=_recordPointMapping.find(recordPoint);
	if (it!=_recordPointMapping.end()) 
	{
		_writerData->writeStartElement(CG_RECORD_KEY);
		//_writer->writeAttribute("t",QString("%1").arg(currentTime-_itime,0,'f',3));
		_writerData->writeAttribute(CG_RECORD_RP_ATT_NUMBER,QString("%1").arg(it.value()));
		return _writerData;
	}
	return 0;
}


//-------------------------------------------------------------------------
QXmlStreamWriter * _RecordManager::queryPropertyKey( ISwRecordPoint * recordPoint,double currentTime )
{
	QMap<ISwRecordPoint *,int>::iterator it=_recordPointMapping.find(recordPoint);
	if (it!=_recordPointMapping.end()) 
	{
		_writerData->writeStartElement(CG_RECORD_KEY_PROPERTY);
		_writerData->writeAttribute(CG_RECORD_RP_ATT_NUMBER,QString("%1").arg(it.value()));
		return _writerData;
	}
	return 0;
}

//-------------------------------------------------------------------------
void _RecordManager::finalizeRecordKey() 
{
	_writerData->writeEndElement();
}

//-------------------------------------------------------------------------
void _RecordManager::createWriterData() 
{
	QDir d=QDir(_repository.getFileName());
	QString name=QString(d.absolutePath()+QDir::separator()+CG_RECORD_FILE_DATA).arg(_dataCounter++);
	_fileWriterData=new QFile(name);
	if (!_fileWriterData->open(QIODevice::WriteOnly | QIODevice::Text)) 
	{
		delete _fileWriterData;
		_fileWriterData=0;
		return;
	}
	//Creation du writer
	_writerData=new QXmlStreamWriter(_fileWriterData);
	_writerData->setAutoFormatting(true);
	_writerData->writeStartDocument("1.0");
	_writerData->writeStartElement(CG_RECORD_DATA);


	_writerData->writeStartElement(CG_RECORD_EXE);
	_writerData->writeAttribute(CG_RECORD_TIME,QString("%1").arg(0.0,0,'f',3));
	_writerData->writeEndElement();

}

//-------------------------------------------------------------------------
void _RecordManager::closeWriterData() 
{
	_writerData->writeEndElement(); //Record Data
	//Fin du doc
	_writerData->writeEndDocument();
	_totalSize+=_fileWriterData->size();
	//Destruction du writer
	delete _writerData;
	_writerData=0;
	_fileWriterData->close();
	delete _fileWriterData;
	_fileWriterData=0;
}

//-------------------------------------------------------------------------
void _RecordManager::addRecordManagerListener(ISwRecordManagerListener * listener) 
{
	if (listener != NULL)
	{
		_listeners.push_back(listener);
		setMainDir(_mainDir);
	}

}

//-------------------------------------------------------------------------
void _RecordManager::removeRecordManagerListener(ISwRecordManagerListener * listener)
{
	if (listener != NULL)
		_listeners.removeOne(listener);
}

//-------------------------------------------------------------------------
void _RecordManager::Initialize(double start_time, StreamWork::SwExecution::ISwExecution_Service * executor) throw (SwException) 
{
	_currentTime=start_time;

}

//-------------------------------------------------------------------------
void _RecordManager::Start(double current_time) throw (SwException)
{
	_currentTime=current_time;

}      

//-------------------------------------------------------------------------
void _RecordManager::Execute(double current_time,bool is_first_call) throw (SwException)
{
	_currentTime=current_time;

	if(_writer != NULL) 
	{
		int size=_totalSize+(unsigned int)_fileWriterData->size();
		foreach(ISwRecordManagerListener * listener,_listeners) 
		{
			listener->setCurrentRecTime(current_time);
			listener->setDataSize(size);
		}

		if (_maxRecordSize<(int)_fileWriterData->size()) 
		{
			closeWriterData();
			createWriterData();
		}

		_writerData->writeStartElement(CG_RECORD_EXE);
		_writerData->writeAttribute(CG_RECORD_TIME,QString("%1").arg(current_time-_itime,0,'f',3));

		_writerData->writeEndElement();
	}
}            

//-------------------------------------------------------------------------
void _RecordManager::Stop(double current_time)
{

}            

//-------------------------------------------------------------------------
SwFileDescriptor _RecordManager::getRecordDirectory() const 
{
	return _repository;
}

//-------------------------------------------------------------------------
void _RecordManager::setRecordDirectory(const SwFileDescriptor & val)
{
	_repository=val;
}

//-------------------------------------------------------------------------
bool _RecordManager::getRecordingState() const
{
	return _isRecording;
}

//-------------------------------------------------------------------------
void _RecordManager::setRecordingState(bool val)
{
	if ( _isRecording == val )
		return;

	_isRecording = val;

	foreach(ISwRecordManagerListener * listener,_listeners) 
	{
		listener->setRecording(val);
	}
}

//-------------------------------------------------------------------------
int _RecordManager::getMaxRecordSize() const 
{
	return _maxRecordSize;
}

//-------------------------------------------------------------------------
void _RecordManager::setMaxRecordSize(int val) 
{
	_maxRecordSize = val;
}

//-------------------------------------------------------------------------
void _RecordManager::buildRecordPointMapping() 
{
	_recordPointMapping.clear();
	QList<ISwRecordPoint *> rpList=_serviceRecord->getRecordPoints();
	int i=0;
	QList<ISwRecordPoint *>::iterator it=rpList.begin();
	while(it!=rpList.end()) 
	{
		_recordPointMapping.insert((*it),i);
		_writer->writeStartElement(CG_RECORD_RP);
		_writer->writeAttribute(CG_RECORD_RP_ATT_NUMBER,QString("%1").arg(i));
		_writer->writeAttribute(CG_RECORD_RP_ATT_NAME,(*it)->getRecordName());
		_writer->writeEndElement();
		i++;
		it++;
	}
}

//-------------------------------------------------------------------------
bool _RecordManager::isRecording()
{
	return _isRecording;
}

//-------------------------------------------------------------------------
void _RecordManager::finalizePropertyKey()
{
	_writerData->writeEndElement();
}

//-------------------------------------------------------------------------
void _RecordManager::setMainDir( QString directoryName )
{
	_mainDir = directoryName;
	_recordsDir.clear();

	QDirIterator it(_mainDir,QStringList(),QDir::Dirs | QDir::NoDotAndDotDot);
	while (it.hasNext()) 
	{
		QFileInfo fileInfo (it.next());
		_recordsDir << fileInfo.fileName();
	}

	QString emitValue="" ;

	foreach(QString val, _recordsDir)
	{
		if(_recordsDir.last() == val)
			emitValue.append(val);
		else
			emitValue.append(val+SEP_REPLAY);
	}
	
	foreach(ISwRecordManagerListener * listener,_listeners) 
	{
		listener->setRecordList(emitValue);
	}

}
