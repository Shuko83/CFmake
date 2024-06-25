/**
@file _ExecutionKey.h
@brief Clef d'enregistrement de l'execution
@author F.Bighelli
 */

#ifndef __EXECUTIONKEY_H
#define __EXECUTIONKEY_H

#include <QtCore>
#include "ISwRecordPoint.h"

using namespace StreamWork::SwRecord;

/**
@class _ExecutionKey
@brief Clef d'enregistrement de l'execution
*/
class _ExecutionKey {
public:
	/** @brief Points d'enregistrements a stimuler */
    QList<ISwRecordPoint *> _recordPoints;
	/** @brief nombre de stimulation a faire sur un record point */
    QList<int> _recordPointsCall;

	/** @brief Points d'enregistrements a stimuler pour les property*/
	QList<ISwRecordPoint *> _recordPropertyPoints;
	/** @brief nombre de stimulation a faire sur un record point property*/
	QList<int> _recordPointsPropertyCall;

	/** @brief Temps courant */
    double _currentTime;
	/** @brief Constructor */
	_ExecutionKey();

};

#endif
