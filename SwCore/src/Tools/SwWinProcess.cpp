/*!
\file SwWinProcess.cpp
\date 12/04/2006
\brief Permet de lancer un process windows sans attaché les handle de connection réseau.
\author  AAY
\version 1.0
 */

#include "Tools/SwWinProcess.h"
#include <Windows.h>
#ifdef _DEBUG
#include <QDebug>
#endif // _DEBUG

using namespace StreamWork::SwCore;


//Extract from QT
static QString qt_create_commandline(const QString &program, const QStringList &arguments)
{
	QString args;
	if (!program.isEmpty()) {
		QString programName = program;
		if (!programName.startsWith(QLatin1Char('\"')) && !programName.endsWith(QLatin1Char('\"')) && programName.contains(QLatin1Char(' ')))
			programName = QLatin1Char('\"') + programName + QLatin1Char('\"');
		programName.replace(QLatin1Char('/'), QLatin1Char('\\'));

		// add the prgram as the first arg ... it works better
		args = programName + QLatin1Char(' ');
	}

	for (int i=0; i<arguments.size(); ++i) {
		QString tmp = arguments.at(i);
		// in the case of \" already being in the string the \ must also be escaped
		tmp.replace( QLatin1String("\\\""), QLatin1String("\\\\\"") );
		// escape a single " because the arguments will be parsed
		tmp.replace( QLatin1Char('\"'), QLatin1String("\\\"") );
		if (tmp.isEmpty() || tmp.contains(QLatin1Char(' ')) || tmp.contains(QLatin1Char('\t'))) {
			// The argument must not end with a \ since this would be interpreted
			// as escaping the quote -- rather put the \ behind the quote: e.g.
			// rather use "foo"\ than "foo\"
			QString endQuote(QLatin1Char('\"'));
			int length = tmp.length();
			while (length >0 && tmp.at(length -1) == QLatin1Char('\\')) {
				--length;
				endQuote += QLatin1Char('\\');
			}
			args += QLatin1String(" \"") + tmp.left(i) + endQuote;
		} else {
			args += QLatin1Char(' ') + tmp;
		}
	}
	return args;
}


//-----------------------------------------------------------------------
SwWinProcess::SwWinProcess(const QString &program,const QStringList &arguments )
{
	_program = program;
	_arguments = arguments;
	_pid = NULL;
	_isRunning = false;
}

//-----------------------------------------------------------------------
SwWinProcess::SwWinProcess()
{
	_pid = NULL;
	_isRunning = false;
}

//-----------------------------------------------------------------------
SwWinProcess::~SwWinProcess()
{
	if(_pid)
		TerminateProcess(_pid, 0xf291);
}

//-----------------------------------------------------------------------
bool StreamWork::SwCore::SwWinProcess::start(const QString &program,const QStringList &arguments )
{
	return internStart(program,arguments);
}

//-----------------------------------------------------------------------
bool StreamWork::SwCore::SwWinProcess::start()
{
	return internStart(_program,_arguments);
}

//-----------------------------------------------------------------------
bool SwWinProcess::start(const QString &program )
{
	return internStart(_program,QStringList());
}

//-----------------------------------------------------------------------
bool StreamWork::SwCore::SwWinProcess::internStart(const QString &program,const QStringList &arguments)
{
	Q_ASSERT(!program.isEmpty());

	if(_isRunning)
		return true;

	QString args = qt_create_commandline(program,arguments);

	bool success = false;

	if (_pid) {
		CloseHandle(_pid->hThread);
		CloseHandle(_pid->hProcess);
		delete _pid;
		_pid = 0;
	}
	_pid = new PROCESS_INFORMATION;
	memset(_pid, 0, sizeof(PROCESS_INFORMATION));


	DWORD dwCreationFlags = CREATE_NO_WINDOW;
	dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;

	STARTUPINFOW startupInfo = { sizeof( STARTUPINFO ), 0, 0, 0,(ulong)CW_USEDEFAULT, (ulong)CW_USEDEFAULT,(ulong)CW_USEDEFAULT, (ulong)CW_USEDEFAULT,0, 0, 0,STARTF_USESTDHANDLES,0, 0, 0,0, 0, 0};

	success = CreateProcess(0, (wchar_t*)args.utf16(),0, 0, FALSE, dwCreationFlags,0,0,&startupInfo, _pid);

	_isRunning = success;

	if (!success) 
	{
		// Capture the error string before we do CloseHandle below
#ifdef _DEBUG
		qDebug() << "Process failed to start : " << args ;
#endif // _DEBUG
		if (_pid) 
		{
			CloseHandle(_pid->hThread);
			CloseHandle(_pid->hProcess);
			delete _pid;
			_pid = 0;
		}
		return success;
	}

	// give the process a chance to start ...
	Sleep(20);

	return  success;
}

//-----------------------------------------------------------------------
void SwWinProcess::kill()
{
	if(_pid)
		TerminateProcess(_pid->hProcess, 0xf291);

	_isRunning = false;
}
