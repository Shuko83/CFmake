/**
 * @file TestComponentReceiverVassisted.cpp
 * @brief NoDescription
 * @version 1.0
 * @date Tue Apr 28 15:53:53 CEST 2009
 * @author F.Bighelli
 */


#include <SwApplication.h>
#include <SwMacros.h>
#include "TestComponentReceiverVassisted.h"
#include "DataTest.h"
#include "SwAssistedComponent.h"

using namespace StreamWork::SwFoundation;

//-------------------------------------------------------------------------
TestComponentReceiverVassisted::TestComponentReceiverVassisted():SwAssistedComponent() 
{
	//setExecutableServiceAvaibility(true);
	setPinServiceAvaibility(true);
}

//-------------------------------------------------------------------------
TestComponentReceiverVassisted::~TestComponentReceiverVassisted() 
{

}

//-------------------------------------------------------------------------
void TestComponentReceiverVassisted::eventReceiveData(SwPin * src,SwData_Class * data) 
{
    DataTest * d=(DataTest *)data;
    QString s=QString("R(%1) at:%2  on channel %3>>%4").arg(GetName()).arg(data->Time,20,'f',3).arg(d->Channel,5).arg(d->toString());  
    qDebug(s.toLatin1().data());
}

//-------------------------------------------------------------------------
void TestComponentReceiverVassisted::initializeComponent()
{
	_pin=registerPin(QString("In"),CG_DATA_TEST,true);
	_ltime = 0.0;
}
