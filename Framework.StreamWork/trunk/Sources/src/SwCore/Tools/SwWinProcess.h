/*!
\file SwWinProcess.h
\date 12/07/2013
\brief Permet de lancer un process windows sans attaché les handle de connection réseau.
\author  AAY
\version 1.0
*/

#ifndef _SwWinProcess_H
#define _SwWinProcess_H

#include <QStringList>
#include "SwCoreConstantes.h"

typedef struct _PROCESS_INFORMATION *SW_PID;

namespace StreamWork
{
	namespace SwCore
	{
		/*!
		\class SwWinProcess 
		\brief Permet de lancer un process windows sans attaché les handle de connection réseau.
		*/
		class BUILD_SWCORE SwWinProcess 
		{

		public:
			/*! Constructeur */
			SwWinProcess(const QString &program,const QStringList &arguments);

			/*! Constructeur */
			SwWinProcess();

			/*! Destructeur */
			~SwWinProcess();

			/**
			 * @brief	: Start with parameters
			 * @param	: QString & program - the program
			 * @param	: QStringList & arguments - the arguments in a list
			 */
			bool start(const QString &program,const QStringList &arguments);

			/**
			 * @brief	: Start without parameters
			 * @param	: QString & program - the program
			 */
			bool start(const QString &program);

			/**
			 * @brief	: Start with constructor parameter
			 */
			bool start();

			/**
			 * @brief	: kill the program
			 */
			void kill();

		private:
			bool internStart(const QString &program,const QStringList &arguments);
		private:
			QString _program;
			QStringList _arguments;
			SW_PID _pid;
			bool _isRunning;
		};
	}
}

#endif