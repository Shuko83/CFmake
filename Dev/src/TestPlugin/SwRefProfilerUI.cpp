#include "SwRefProfilerUI.h"

using namespace StreamWork::SwCore;
//================================  QActionMapper  ==========================================================

QAction & QActionMapper::GetAction()
{
	return *_qAction;
}

QActionMapper::QActionMapper( QAction * qAction )
{
	_qAction = qAction;
}
//================================  SwRefProfilerUI  ==========================================================

SwRefProfilerUI::SwRefProfilerUI(void)
{
	QAction * action = new QAction(this);
	action->setText(SwRefPtrTools::getInstance()->isWatching()?"stop watching":"start watching");
	connect(action, SIGNAL(triggered()), this, SLOT(toggleWatching()));
	_i_refWatcherSwitch = new QActionMapper(action);
}

SwRefProfilerUI::~SwRefProfilerUI(void)
{
	unprovideInterface("WatcherSwitch");
}

void SwRefProfilerUI::initializeComponent() throw(SwException)
{
	provideInterface("WatcherSwitch", _i_refWatcherSwitch); 		
}


void SwRefProfilerUI::toggleWatching()
{
	if(SwRefPtrTools::getInstance()->isWatching()){
		SwRefPtrTools::getInstance()->stopWatch();
		SwRefPtrTools::getInstance()->printResults();
		_i_refWatcherSwitch->GetAction().setText("start watching");
	}else{
		SwRefPtrTools::getInstance()->startWatch();
		_i_refWatcherSwitch->GetAction().setText("stop watching");
	}
}

