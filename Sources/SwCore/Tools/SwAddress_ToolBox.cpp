/*!
 \file SwAddress_ToolBox.cpp
 \brief Implementation of the Class SwAddress_Class
 \date 24-août-2006 12:21:05
 \author F.Bighelli
*/
/*
 * INCLUDES GLOBAUX
 */
#include <QStringList>
/*
 * INCLUDES LOCAUX
 */
#include "SwAddress_ToolBox.h"
#include "SwApplication.h"
#include "SwMacros.h"

using namespace StreamWork::SwCore;


/*! \brief Construit le chemin absolu jusqu'a la cible
\param[in] target la cible
\return le chemin absolu */
QString SwAddress_ToolBox::BuildAbsolutePath(SwComponent_Class *target){
    SwComponent_Class * c_target;
    QList<SwComponent_Class *> fifo_target;
    QString path;

    //path absolu
    //Construction d'une fifo de composant depuis target jusqu'a la racine
    c_target=target;
	while(c_target->GetParent()!=NULL) {
        fifo_target.push_front(c_target);
		c_target=c_target->GetParent();
    }
    //Construction du path
    path="";
    while (!fifo_target.isEmpty()) {
        path+=fifo_target.front()->GetName();
        fifo_target.pop_front();
        if (!fifo_target.isEmpty()) path+=CG_PATH_SEPARATOR;
    }
    return path;
}
/*! \brief Construit le chemin relatif de la source jusqu'a la cible
\param[in] source la source
\param[in] target la cible
\return le chemin relatif
\exception SwException Can't find relative path from source to target because they dont have the same parent root*/
QString SwAddress_ToolBox::BuildRelativePath(SwComponent_Class *source,SwComponent_Class *target) throw(SwException){
    SwComponent_Class * c_target;
    SwComponent_Class * c_source;
    QList<SwComponent_Class *> fifo_target;
    QList<SwComponent_Class *> fifo_source;
    QString path;

    //path relatif
    //Construction d'une fifo de composant depuis target jusqu'a la racine
    c_target=target;

    do {
        fifo_target.push_front(c_target);
		c_target=c_target->GetParent();
    } while(c_target!=NULL);
    //Construction d'une fifo de composant depuis source jusqu'a la racine
    c_source=source;
    do {
        fifo_source.push_front(c_source);
		c_source=c_source->GetParent();
    } while(c_source!=NULL);
    //on verifie que le front est commun (le parent_racine est identique)
    if (fifo_target.front()!=fifo_source.front()) {
        QString msg=QString("Can't find relative path from %1 to %2 because they dont have the same parent root").arg(source->GetName()).arg(target->GetName());
        LAUNCH_SWEXCEPTION("SwCore",msg);
    }
    //Suppression des debut de fifo qui sont a commun
    while (!fifo_source.isEmpty() && !fifo_target.isEmpty() && fifo_target.front()==fifo_source.front()) {
        fifo_target.pop_front();
        fifo_source.pop_front();
    }
    //Path montant
    path=CG_PATH_SEPARATOR;
    for(int i=0;i<fifo_source.size();i++)
        path+=CG_PATH_SEPARATOR;
    //Path descendant
    while (!fifo_target.isEmpty()) {
        path+=fifo_target.front()->GetName();
        fifo_target.pop_front();
        if (!fifo_target.isEmpty()) path+=CG_PATH_SEPARATOR;
    }
    return path;
}
/*! \brief Construit un chemin universel jusqu'a la cible
(utile si source et cible ne sont pas dans le même stream)
\param[in] target la cible
\return le chemin universal */
QString SwAddress_ToolBox::BuildUniversalPath(SwComponent_Class *target) {
    SwComponent_Class * c_target;
    QList<SwComponent_Class *> fifo_target;
    QString path;

    //path absolu
    //Construction d'une fifo de composant depuis target jusqu'a la racine
    c_target=target;
	while(c_target->GetParent()!=NULL) {
        fifo_target.push_front(c_target);
		c_target=c_target->GetParent();
    }
    //Construction du path
    //Ajout du nom du stream
    path=CG_PATH_STREAM_HEADER;
    path+=c_target->GetName();
    path+=CG_PATH_SEPARATOR;
    while (!fifo_target.isEmpty()) {
        path+=fifo_target.front()->GetName();
        fifo_target.pop_front();
        if (!fifo_target.isEmpty()) path+=CG_PATH_SEPARATOR;
    }
    return path;

}

/*! \brief parcours du path
\param[in] start_c composant de depart
\param[in] start_i index de depart
\param[in] fragment_path path fragmente
\return le composant trouve ou NULL sinon
*/
SwComponent_Class * FindComponent(SwComponent_Class * start_c,
                                                       int start_i,
                                                       QStringList & fragment_path){
    SwComponent_Class * next_c;

    next_c=NULL;
    //Si fin de chaine alors le composant est trouve
    if (start_i>=fragment_path.size() || start_c==NULL)
        return start_c;
    //Si c'est acces au parent
    if (fragment_path[start_i].isEmpty())
        next_c=start_c->GetParent();
    else {
        //Pas de support de renommage
		next_c=start_c->GetChild(fragment_path[start_i]);
    }
    return FindComponent(next_c,start_i+1,fragment_path);
}


/*! \brief Trouve la cible a partir de la source et du path
\param[in] path le chemin jusqu'a la cible
\param[in] source la source
\return la cible
\exception SwException Unable to find target for an invalid path
\exception SwException Unable to find target from a NULL source
*/
SwComponent_Class * SwAddress_ToolBox::FindTarget(QString path,SwComponent_Class *source) throw(SwException){
    QStringList f_path;
    QString streamname;
    SwComponent_Class * c_target;
    int index;

    //Si le path est null ou source indefini alors on ne fait rien
    if (path.isNull()) {
        QString msg=QString("Unable to find target for an invalid path");
        LAUNCH_SWEXCEPTION("SwCore",msg);
    }
    if (source==NULL && path[0]!=CG_PATH_STREAM_HEADER) {
        QString msg=QString("Unable to find target from a NULL source");
        LAUNCH_SWEXCEPTION("SwCore",msg);
    }
    //Construction du path fragmenté
    f_path=path.split(CG_PATH_SEPARATOR);
    //Suppression de la derniere chaine si elle est vide
    if (f_path.size()>1 && f_path[f_path.size()-1].isEmpty()) {
        f_path.removeAt(f_path.size()-1);
    }
    if (f_path[0].isEmpty()) {
        //Si le premier element est vide, on part de la source locale
        // et attaque a la chaine suivante
        c_target=source;
        index=1;
    } else {
        //Si ce n'est pas une adresse universelle
        if (path[0]!=CG_PATH_STREAM_HEADER && source) {
            //on par de la racine et on part de la premier chaine
            c_target=source;
		    while (c_target->GetParent()!=NULL) c_target=c_target->GetParent();
            index=0;
        } else {
            //Extraction du nom
            streamname=f_path[0];
            streamname.remove(QChar(CG_PATH_STREAM_HEADER), Qt::CaseInsensitive);
            //On part de la racine du bon stream et on par de la deuxieme chaine
            c_target=SW_APP->GetFirstStream();
            while (c_target!=NULL && c_target->GetName()!=streamname) {
                c_target=SW_APP->GetNextStream();
            }
            if (c_target==NULL) {
                QString msg=QString("Stream source %1 undefined").arg(path);
                LAUNCH_SWEXCEPTION("SwCore",msg);
            }
            index=1;
        }
    }
    //recherche du composant
    return FindComponent(c_target,index,f_path);
}
