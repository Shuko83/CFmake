
#ifndef _SwRefProfilerUI_H
#define _SwRefProfilerUI_H

/*
  * INCLUDES LOCAUX
  */

#include <SwAssistedComponent.h>
#include <ISwAction.h>

using namespace StreamWork::SwCore;

class QActionMapper : public ISwAction{
public :
	QActionMapper(QAction * qAction);

	virtual QAction & GetAction();

private :
	QAction * _qAction;

};

class SwRefProfilerUI : public StreamWork::SwFoundation::SwAssistedComponent
{
    Q_OBJECT
public:
    SwRefProfilerUI();
    virtual ~SwRefProfilerUI();

    virtual void initializeComponent() throw(SwException);

public slots :
		void toggleWatching();

private:
	ISwAction * _i_refWatcherSwitch;

};



#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
