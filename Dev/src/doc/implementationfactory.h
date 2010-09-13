/*! \page impl_fabrique Comment implementer une fabrique ? 

    \section impl_f_1 1.Surcharger StreamWork::SwCore::SwPluginFactory_Class

    Dans le header nous aurons donc:
     \code 
class MyPluginFactory_Class : public StreamWork::SwCore::SwPluginFactory_Class
{
    protected:
        //instanciation d'un composant
        StreamWork::SwCore::SwComponent_Class * CreateInstanceOf(QString name) ;
        // acces a l'icone d'un composant
        QIcon CreateIconOf(QString name) const;
        //instanciation d'une data
        StreamWork::SwCore::SwData_Class * CreateInstanceOf(const SwUUID & type_id);
    public:
        //Constructeur
        MyPluginFactory_Class();
        //Destructeur
        virtual ~MyPluginFactory_Class();
        //Initialisation
        void Initialize();
        //Finalisation de l'initialisation
        void FinalizeInitialisation();
        //Liberation
        void Liberate();
        //Acces a la version du plugin
        QString GetPluginVersion();
        //sur ajout d'un service global
        void OnRegisterService(StreamWork::SwCore::ISwService * service);            
        // sur suppression d'un service global
        void OnUnregisterService(StreamWork::SwCore::ISwService * service);            
};
    \endcode

    Mais aussi le point d'entree plugin:

     \code 
#ifndef QT_NO_DEBUG
extern "C" Q_DECL_EXPORT StreamWork::SwCore::SwPluginFactory_Class *  GetPluginInterfaceD() {
    return new MyPluginFactory_Class();    
}
#else
extern "C" Q_DECL_EXPORT StreamWork::SwCore::SwPluginFactory_Class *  GetPluginInterface() {
    return new MyPluginFactory_Class();    
}
#endif

     \endcode

    \section impl_f_2 2.Implemeter les diff&eacute;rentes m&eacute;thodes

    \subsection impl_f_compo_init La methode Initialize
    Dans cette methode vous devez declarer tous les composants et datas que votre fabrique est capable de construire.\n
    Exemple:
    \code
void MyPluginFactory_Class::Initialize() {
    RegisterComponent("MyFirstInternalComponantName","My First Componant Name");
    RegisterComponent("MySecondInternalComponantName","My Second Componant Name");
    //etc...
    RegisterData(MyDataUUID,"MyDataName");
    //etc...

}
    \endcode

    \subsection impl_f_compo_createinstanceof La methode FinalizeInitialisation
    Cette methode est appel&eacute;e lorsque tous les plugins ont &eacute;t&eacute; initialis&eacute;s.\n
    Si vous avez des services globaux &agrave; d&eacute;clacer, ils doivent &ecirc;tre enregistrer dans cette methode.\n
    Exemple:
    \code
void MyPluginFactory_Class::FinalizeInitialisation() {
    _myService =new MyService();
    SW_APP->RegisterService(_myService);
    //etc...
}
    \endcode

    \subsection impl_f_compo_liberate La methode Liberate
    Cette methode est appel&eacute;e a la fin de l'application permettant ainsi a votre fabrique de liberer les ressources.\n
    Exemple:
    \code
void MyPluginFactory_Class::Liberate() {
  //Liberation des ressources
}
    \endcode





    \subsection impl_f_compo_createinstanceofc La methode CreateInstanceOf  (composant)
    Cette methode permet de construire les composants de votre fabrique.\n
    Exemple:
    \code
StreamWork::SwCore::SwComponent_Class * MyPluginFactory_Class::CreateInstanceOf(QString name) {
    if (name=="MyFirstInternalComponantName") {
        return new MyFirstInternalComponantName();
    }
    if (name=="MyFirstInternalComponantName") {
        return new MyFirstInternalComponantName();
    }
    //etc...
    return NULL;
}
    \endcode
     
    \subsection impl_f_compo_createinstanceofd La methode CreateInstanceOf (data)
    Cette methode permet de construire les data de votre fabrique.\n
    Exemple:
    \code
StreamWork::SwCore::SwData_Class * MyPluginFactory_Class::CreateInstanceOf(const SwUUID & type_id) {

    if (type_id==MyDataUUID) {
        return new MyData();
    }
    //Etc...
    return NULL;
}
    \endcode
     
    \subsection impl_f_compo_createiconof La methode CreateIconOf
    Cette methode fournit un icon a StreamWorkEditor pour repr‚senter vos composants.\n
    Exemple:
    \code     
QIcon MyPluginFactory_Class::CreateIconOf(QString name) const {
    QIcon ico;
    if (name=="MyFirstInternalComponantName") {
        return QIcon(":/MyFirstComponantIcon.png"); 
    }
    return ico;
}
    \endcode     
   
   
    \subsection impl_f_compo_createiconof La methode CreateIconOf
    Cette methode fournit un icon a StreamWorkEditor pour repr‚senter vos composants.\n
    Exemple:
    \code     
QIcon MyPluginFactory_Class::CreateIconOf(QString name) const {
    QIcon ico;
    if (name=="MyFirstInternalComponantName") {
        return QIcon(":/MyFirstComponantIcon.png"); 
    }
    return ico;
}
    \endcode     

    \subsection impl_f_compo_onregisterservice La methode OnRegisterService
    A la fin de l'initialisation (FinalizeInitialisation) , les services globaux sont enregistr&eacute;s.\n
    A chaque enregistrement, cette callback est appel&eacute;e avec en parametres, le service enregistr&eacute;\n
    Vous pouvez alors traiter ce service
    \code     
void MyPluginFactory_Class::OnRegisterService(StreamWork::SwCore::ISwService * service) {
    IServiceExpecting * serviceExpecting=dynamic_cast<IServiceExpecting *>(service);
    if (serviceExpecting!=0) {
        //Process serviceExpecting
    }
}
    \endcode  
       
    \subsection impl_f_compo_onregisterservice La methode OnUnregisterService
    A la fin de l'application, les services globaux sont d&eacute;senregistr&eacute;s.\n
    A chaque liberation de service, cette callback est appel&eacute;e avec en parametres, le service liber&eacute;\n
    Vous pouvez alors traiter ce service
    \code     
void MyPluginFactory_Class::OnUnregisterService(StreamWork::SwCore::ISwService * service) {
     IServiceExpecting * serviceExpecting=dynamic_cast<IServiceExpecting *>(service);
    if (serviceExpecting!=0) {
        //Process serviceExpecting
    }

}
    \endcode     
   
    \subsection impl_f_compo_pluginversion La methode GetPluginVersion
    Cette methode permet de connaitre la version du plugin.\n
    L'exemple suivant est une proposition d'implementation 
    \code     
QString MyPluginFactory_Class::GetPluginVersion() {
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

    \endcode     
   
   
     
*/
