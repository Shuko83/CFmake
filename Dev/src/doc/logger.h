/*! \page impl_log Comment utiliser le gestion de log ?

    Le gestionnaire de log est accessible ainsi
    \code
    SW_APP->Logger()
    \endcode
    Le type renvoye est StreamWork::SwCore::ISwLogger.

    \section impl_log_rec Comment ecouter les logs de l'application ?
    Pour ecouter les logs de l'application, if faut implementer l'interface StreamWork::SwCore::ISwLogRecorder
    et s'enregistrer au niveau du logger:
    \code
    SW_APP->Logger().AttachLogRecorder(myLogRecorder);
    \endcode
    Cette operation peut - par exemple - etre effectuer dans l'initialisation d'une fabrique.\n
    On peut se desenregistrer ainsi:
    \code
    SW_APP->Logger().DetachLogRecorder(myLogRecorder);
    \endcode
    
    
    \section impl_log_send Comment generer des logs dans l'application ?
    Il suffit d'appeler la methode Log avec le niveau d'importance et le message associe.\n
    Exemple:
    \code
    SW_APP->Logger().Log(LogLvl_Info,QString("Hello Foo"));
    \endcode


*/