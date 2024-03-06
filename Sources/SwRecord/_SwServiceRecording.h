/**
@file _SwServiceRecording.h
@brief Implementation du service de record
@author F.Bighelli
 */

#ifndef __SWSERVICERECORDING_H
#define __SWSERVICERECORDING_H


#include "ISwServiceRecording.h"
#include <QtCore>

using namespace StreamWork::SwRecord;
using namespace StreamWork::SwCore;

/**
@class _SwServiceRecording
@brief Implementation du service de record
*/
class _SwServiceRecording : public ISwServiceRecording{
private:
    /* @brief map des fabriques*/
    QMultiMap<QString,ISwRecordDataCodecFactory *> _factories;
    /* @brief map des point d'enregistrement*/
    QList<ISwRecordPoint *> _points;
public:
	/** @brief Constructor */
	_SwServiceRecording();
	/** @brief Destructor */
	~_SwServiceRecording();
    //---------------------------------------------------------------------
    // Partie pour l'enregistrement des factories
    //---------------------------------------------------------------------
    /** 
    * @brief enregistrement d'une fabrique de codecs
    * @param codecFactory fabrique de codec
    */
    virtual void registerCodecFactory(ISwRecordDataCodecFactory * codecFactory);
    /** 
    * @brief enregistrement d'une fabrique de codecs
    * @param codecFactory fabrique de codec
    */
    virtual void unregisterCodecFactory(ISwRecordDataCodecFactory * codecFactory);
    //---------------------------------------------------------------------
    // Partie pour l'enregistrement des point d'enregistrements
    //---------------------------------------------------------------------
    /** 
    * @brief enregistrement d'un point d'enregistrement
    * @param recordPoint point d'enregistrement
    */
    virtual void registerRecordPoint(ISwRecordPoint * recordPoint);
    /** 
    * @brief desenregistrement d'un point d'enregistrement
    * @param recordPoint recordPoint
    */
    virtual void unregisterRecordPoint(ISwRecordPoint * recordPoint);
    //---------------------------------------------------------------------
    // Acces aux codecs disponibles et creation d'un codec
    //---------------------------------------------------------------------
    /** @brief acces au codec disponible pour les SwData */
    virtual QStringList getSwDataCodecs();
    /** @brief Construction d'un codec */
    virtual ISwRecordDataCodec * buildCodec(QString swDataName);
    /** @brief Construction d'une liste de codec */
    virtual QList<ISwRecordDataCodec *> buildCodecs(QString swDataName);
    //---------------------------------------------------------------------
    // Interface ISwService
    //---------------------------------------------------------------------
    /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
    lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
    virtual void Liberate();
    //---------------------------------------------------------------------
    // Acces aux points d'enregitrement
    //---------------------------------------------------------------------
    /** @brief Acces au points d'enregistrements */
    QList<ISwRecordPoint *> getRecordPoints();
    /** @brief Acces au points d'enregistrements */
    ISwRecordPoint * getRecordPoint(QString name);
};

#endif
