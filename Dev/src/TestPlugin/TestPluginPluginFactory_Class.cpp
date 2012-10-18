/**
 * @file TestPluginPluginFactory_Class.cpp
 * @brief Plugin Factory
 * @version 1.0
 * @date Tue Apr 28 15:53:53 CEST 2009
 * @author F.Bighelli
 */


#include <QRegExp>

#include "TestPluginPluginFactory_Class.h"
#include "TestComponent.h"
#include "TestComponentEmitter.h"
#include "TestComponentReceiver.h"
#include "DataTest.h"
#include "DataTestCodecFactory.h"
#include "ISwServiceRecording.h"
#include "DataTestCodecITest.h"
#include "SwRecordDataCodecFactoryTemplate.h"
#include "TestAssistedComponent.h"
#include "TestComponentEmitterVassisted.h"
#include "TestComponentReceiverVassisted.h"
#include "SwRefProfilerUI.h"

using namespace StreamWork::SwExecution;
/** @brief Constructeur */
TestPluginPluginFactory_Class::TestPluginPluginFactory_Class():SwPluginFactory_Class() {

}
/** @brief Destructeur */
TestPluginPluginFactory_Class::~TestPluginPluginFactory_Class() {

}
/** @brief Initialisation */
void TestPluginPluginFactory_Class::Initialize() {
	//RegisterComponent("TestComponent","NoDescription");
	RegisterComponent("TestComponentEmitter","TestComponentEmitter");
	RegisterComponent("TestComponentEmitterVassisted","TestComponentEmitterVassisted");
	RegisterComponent("TestComponentReceiverVassisted","TestComponentReceiverVassisted");
	
	RegisterComponent("TestComponentReceiver","TestComponentReceiver");
	RegisterComponent("TestComponent","TestComponent");
	RegisterComponent("TestAssisted::Component","Test Assisted component");
    RegisterData(CG_DATA_TEST_ID,CG_DATA_TEST);

	RegisterComponent("SwRefProfiler","Ref profiler");
}
/** @brief Liberation */
void TestPluginPluginFactory_Class::Liberate() {

}
/** @brief instanciation d'un composant */
SwComponent_Class * TestPluginPluginFactory_Class::CreateInstanceOf(QString name) {
    if (name=="TestComponent") {
        return new TestComponent;
    }
    if (name=="TestComponentEmitter") {
        return new TestComponentEmitter;
    }
	if (name=="TestComponentEmitterVassisted") {
		return new TestComponentEmitterVassisted;
	}
	if (name=="TestComponentReceiverVassisted") {
		return new TestComponentReceiverVassisted;
	}
    if (name=="TestComponentReceiver") {
        return new TestComponentReceiver;
    }
    if (name=="TestComponent") {
        return new TestComponent;
    }
	if (name=="TestAssisted::Component") {
		return new TestAssistedComponent;
	}
	if (name=="SwRefProfiler") {
		return new SwRefProfilerUI;
	}
    return NULL;
}
/** @brief instanciation d'une data */
SwData_Class * TestPluginPluginFactory_Class::CreateInstanceOf(const SwUUID & type_id) {
    if (type_id==CG_DATA_TEST_ID) {
        return new DataTest();
    }
    return 0;
}
/** 
 * @brief acces a l'icone d'un composant
 * @param[in] name nom du composant
 * @return l'incone ou icone vide si non trouvé
 */
QIcon TestPluginPluginFactory_Class::CreateIconOf(QString name) const {
    QIcon ico;
    if (name=="TestComponent") {
        return ico; //change for personal icon
    }
    return ico;
}
/** 
 * @brief Acces a la version du plugin
 * @return version
 */
QString TestPluginPluginFactory_Class::GetPluginVersion(){
    QString build;

    build=QString("%1 %2").arg(__DATE__).arg(__TIME__);
    build=build.toUpper();
    build.replace(QRegExp("[ |:]+"), ".");
#ifndef QT_NO_DEBUG
    build+=" (Debug)";
#else 
    build+=" (Release)";
#endif
    return build;
}

//---------------------------------------------------------------------
// Interface ISwServicesManager_Listener
//---------------------------------------------------------------------
/*! \brief sur ajout d'un service */
void TestPluginPluginFactory_Class::OnRegisterService(ISwService * service) {
    ISwServiceRecording * rservice=dynamic_cast<ISwServiceRecording *>(service);
    if (rservice!=0) {
        rservice->registerCodecFactory(new DataTestCodecFactory());
        rservice->registerCodecFactory(new SwRecordDataCodecFactoryTemplate<DataTestCodecITest>(CG_DATA_TEST_ITEST));
    }

}
/*! \brief sur suppression d'une  interface */
void TestPluginPluginFactory_Class::OnUnregisterService(ISwService * service) {

}
