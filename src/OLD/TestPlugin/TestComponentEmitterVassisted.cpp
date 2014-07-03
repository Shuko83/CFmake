/**
 * @file TestComponentEmitterVassisted.cpp
 * @brief NoDescription
 * @version 1.0
 * @date Tue Apr 28 15:53:53 CEST 2009
 * @author F.Bighelli
 */


#include <QtGlobal>
#include <SwApplication.h>
#include <SwMacros.h>
#include <QString>
#include "TestComponentEmitterVassisted.h"
#include "DataTest.h"

using namespace StreamWork::SwFoundation;

//-------------------------------------------------------------------------
TestComponentEmitterVassisted::TestComponentEmitterVassisted():SwAssistedComponent() 
{
setExecutableServiceAvaibility(true);
setPinServiceAvaibility(true);
}

//-------------------------------------------------------------------------
TestComponentEmitterVassisted::~TestComponentEmitterVassisted() 
{

}


//-------------------------------------------------------------------------
void TestComponentEmitterVassisted::Initialize(double start_time,ISwExecution_Service * executor) throw (SwException) 
{
    qsrand((uint)start_time);
    _count=0;
}
//-------------------------------------------------------------------------
void TestComponentEmitterVassisted::Start(double current_time) throw (SwException)
{

}               

//-------------------------------------------------------------------------
void TestComponentEmitterVassisted::Execute(double current_time,bool is_first_call) throw (SwException)
{
    int channel=qrand()%100;
    int count=qrand()%4;
    for(int i=0;i<count;i++) 
	{
        if(channel%5==0) 
		{
            QString s=QString("E(%1) at:%2  on channel %3>").arg(GetName()).arg(current_time,20,'f',3).arg(channel);  
            //qDebug(s.toLatin1().data());
            DataTest *d=(DataTest *)SW_FACTORIES.CreateData(CG_DATA_TEST_ID);
            d->_addRef();
            d->Channel=channel;
            d->Priority=1;
            d->Time=current_time;
            d->SetContent(QString("%1").arg(_count));
            if(qrand()%4)
			{
                d->_itest=new TestImpl(_count);
            }
            _pin->SendData(d);
            d->_release();
            _count++;
        }
    }
}            

//-------------------------------------------------------------------------
void TestComponentEmitterVassisted::Stop(double current_time){

}           


//-------------------------------------------------------------------------
void TestComponentEmitterVassisted::initializeComponent() throw(SwException)
{
	_pin=registerPin(QString("out"),CG_DATA_TEST,true);
}

