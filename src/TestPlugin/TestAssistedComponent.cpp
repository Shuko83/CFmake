/**
 * @file TestAssistedComponent.cpp
 * @brief NoDescription
 * @version 1.0
 * @date Tue Apr 28 15:53:53 CEST 2009
 * @author F.Bighelli
 */


#include <SwApplication.h>
#include <SwMacros.h>
#include "TestAssistedComponent.h"
#include <QDebug>
#include "ISwWidget.h"
#include "SwPin.h"
using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;
using namespace StreamWork::SwFoundation;

/** @brief Constructeur */
TestAssistedComponent::TestAssistedComponent():SwAssistedComponent() 
{
  
	setPinServiceAvaibility(true);

	_test.AddKey(1,QString("Toto"));
	_test.AddKey(2,QString("Titi"));
	_test.FromInt(1);

}
/** @brief Destructeur */
TestAssistedComponent::~TestAssistedComponent()
{
	unconsummeInterface("montruc2");
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void TestAssistedComponent::initializeComponent() throw(SwException)
{
	setComponentNameForShortcut("toto");

	addShortcut("function 1");
	provideInterface("montruc",this);
 
	consummeInterface<ISwWidget>("montruc2");

	registerPin(QString("out"),"DataTest",true);

	createPropertiesForQObject(this);

}

void TestAssistedComponent::interfaceAvailable(QString interfaceName)
{
	if (interfaceName =="montruc2")
	{
		ISwWidget * _i_bottomWidget;
		_i_bottomWidget = getInterface<ISwWidget>("montruc2");
		
		qDebug() << "Montruc2 << "  << _i_bottomWidget->GetWidget().objectName();
	}
}


void TestAssistedComponent::interfaceUnavailable(QString interfaceName)
{
	if (interfaceName =="montruc2")
	{
	}
}
// /** @brief record */
 SwEnum TestAssistedComponent::getTest() const {
     return _test;
 }
 void TestAssistedComponent::setTest(SwEnum val) {
     _test=val;
 }