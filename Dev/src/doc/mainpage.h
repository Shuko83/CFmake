/*! \mainpage StreamWork Documentation
    
    Derniere mise a jour 25/11/2010
    
    \section sec_install Installation


    \section sec_getting_started Qu'est ce qu'une application bas&eacute; sur StreamWork?

    Une application StreamWork correspond &agrave; une assemblage de composant de traitement qui interagissent entre eux.
    Les composants de traitements sont regroup&eacute; en arbre de composants.
    Toute application Streamwork dispose d'un singleton de la classe StreamWork::SwCore::SwApplication (accessible via la macro SW_APP) qui
    r&eacute;f&eacute;rence tous les arbres de composants.

    \section sec_services Notion de services
    
    Un service permet la mise en oeuvre de certains m&eacute;canismes.

    Tout composant ainsi que le singleton StreamWork::SwCore::SwApplication dispose d'un gestionnaire de service.
    Les services globaux a toute l'application sont enregistr&eacute; au niveau du singleton SwApplication
    Les services locaux sont enregistr&eacute; au niveau du composants.

    Les services globaux sont:
    \li StreamWork::SwCore::ISwServiceFileEditorManager service de gestion d'editeur de fichier

    Les services locaux:
    \li StreamWork::SwCore::ISwInterfaces_Provider service de gestion de fournisseur d'interface
    \li StreamWork::SwCore::ISwInterfaces_Consumer service de gestion de consommateur d'interface
    \li StreamWork::SwCore::ISwPins_Manager service de gestion de connecteur
    \li StreamWork::SwCore::ISwProperties service de gestion de propri&eacutet&eacutes
    \li StreamWork::SwCore::ISwPins_Manager service de gestion de connecteur
    \li StreamWork::SwExecution::ISwExecutable_Service service de gestion de composant executable
    
    \section sec_plugin_factory Notion de plugins

    Au demarrage d'une application StreamWork, celle-ci charge tous les plugins qu'elle trouve dans les chemins qui lui sont fournis.
    Les plugins sont des libraries dynamiques fournissants une fabrique de plusieurs composants.
    Les fabriques sont des classes d&eacute;riv&eacute;es de StreamWork::SwCore::SwPluginFactory_Class.

    \ref impl_fabrique
    
    \section sec_availables Liste des entit&eacute;s disponibles
    \ref SwEntry
    
    
    \section sec_man_log Gestionnaire de log et d'alerte
    
    Une application StreamWork dispose d'un gestionnaire de log integr&eacute;.\n
    Un gestionnaire d'alerte au mecanisme similaire existe aussi.\n
    \ref impl_log
   
   
    \todo 
   

    \swinterfaces
    
*/
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
