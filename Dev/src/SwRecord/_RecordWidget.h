/**
@file _RecordWidget.h
@brief widget d'enregistremenr
@author F.Bighelli
 */

#ifndef __RECORDWIDGET_H
#define __RECORDWIDGET_H

#include <QtGui>
#include <QtCore>
#include "ISwRecordManagerListener.h"


/**
@class _RecordWidget
@brief widget d'enregistremenr
*/
class _RecordWidget : public QWidget, public virtual StreamWork::SwRecord::ISwRecordManagerListener {
    Q_OBJECT
public:
	/** @brief Constructor */
	_RecordWidget();
	/** @brief activation */
    void setEnableRecordInformation(bool enable);
	/** @brief temps de demarrage */
    void setStartTime(double stime);
	/** @brief temps courant */
    void setCurrentTime(double ctime);
	/** @brief write data size */
    void setDataSize(int size);
public slots:
	/** @brief write data size */
    void onTimeout();
private:
    /* @brief etat */
    bool _rEnable;
    /* @brief pixmap */
    QPixmap _rPixmap;
    QPixmap _rPixmapOff;
    QPixmap _rPixmapOut;
    /* @brief labels */
    QLabel * _lIcon;
    QLabel * _lDateStart;
    QLabel * _lDateCurrent;
    QLabel * _lDataSize;
    /* @brief timer */
    QTimer * _timer;
    bool _isSymbolOn;
    double _opacity;
};

#endif
