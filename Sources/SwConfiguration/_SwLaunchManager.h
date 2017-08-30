/*!
\file _SwLaunchManager.h
\date
\brief
\author
\version
*/

#ifndef _SwLaunchManager_H
#define _SwLaunchManager_H
/*
  * INCLUDES GLOBAUX
  */
#include <SwProperties_Class.h>
#include <SwFileDescriptor.h>
#include <SwEnum.h>
#include <_ISwConfigurationLaunchManager.h>
#include <_ISwConfigurationLaunchEditor.h>
#include <QObject>
/*
  * INCLUDES LOCAUX
  */




namespace StreamWork {
	namespace SwConfiguration {

		using namespace StreamWork::SwCore;
		/*!
			\class _SwLaunchManager
			\brief
			*/
		class _SwLaunchManager : public _ISwConfigurationLaunchManager
		{

			Q_OBJECT;
			Q_PROPERTY(StreamWork::SwCore::SwEnum startMode READ getStartMode WRITE setStartMode);
			Q_PROPERTY(bool autoStart READ getAutoStart WRITE setAutoStart);
			Q_PROPERTY(bool recordActivated READ getRecordActivated WRITE setRecordActivated);

			Q_PROPERTY(StreamWork::SwCore::SwFileDescriptor recordDirectory READ getRecordDirectory WRITE setRecordDirectory);
			Q_PROPERTY(StreamWork::SwCore::SwFileDescriptor replayDirectory READ getReplayDirectory WRITE setReplayDirectory);
			Q_PROPERTY(StreamWork::SwCore::SwFileDescriptor configurationFile READ getConfigurationFile WRITE setConfigurationFile);

		protected:


		public:
			/*! \brief Constructeur */
			_SwLaunchManager();

			/*! \brief Destructeur */
			virtual ~_SwLaunchManager();

			////////// interface ISwConfigurationlaunchManager
			virtual void setEditor(ISwConfigurationLaunchEditor * editor);

			virtual SwEnum getStartMode();
			virtual void setStartMode(SwEnum value);

			virtual bool getAutoStart();
			virtual void setAutoStart(bool value);

			virtual bool getRecordActivated();
			virtual void setRecordActivated(bool value);

			virtual SwFileDescriptor getRecordDirectory();
			virtual void setRecordDirectory(SwFileDescriptor value);

			virtual SwFileDescriptor getReplayDirectory();
			virtual void setReplayDirectory(SwFileDescriptor value);

			virtual SwFileDescriptor getConfigurationFile();
			virtual void setConfigurationFile(SwFileDescriptor value);

			////////// interface ISwConfigurationlaunchManager   

			virtual bool LoadValues(QString filename);
			virtual bool SaveValues(QString filename);


			void LoadLaunchProperties(QDomElement & elt);
			void SaveLaunchProperties(QDomElement & elt, QDomDocument &doc);
		protected:


			bool                 _autoStart;
			SwEnum               _startMode;
			SwFileDescriptor     _configurationFile;

			SwFileDescriptor     _recordDirectory;
			bool                 _recordActivated;

			SwFileDescriptor     _replayDirectory;

			bool                 _isRunning;
			bool                 _isLoaded;

			ISwConfigurationLaunchEditor * _editor;
		};
	}
}

#endif
