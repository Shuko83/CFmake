/**
@file _ReplayWidget.h
@brief widget de rejeu
@author F.Bighelli
 */

#ifndef __REPLAYWIDGET_H
#define __REPLAYWIDGET_H

#include <QtGui>
#include <QtCore>
#include "ISwReplayManagerListener.h"

using namespace StreamWork::SwRecord;
/**
@class _ReplayWidget
@brief widget de rejeu
*/
class _ReplayWidget : public QWidget, public virtual ISwReplayManagerListener {
    Q_OBJECT
public:
	/** @brief Constructor */
	_ReplayWidget();
	/** @brief activation */
    void setEnableReplayInformation(bool enable);
	/** @brief start time */
    void setStartTime(double vtime);
	/** @brief stop time */
    void setStopTime(double vtime);
	/** @brief current time */
    void setCurrentTime(double vtime);
	/** @brief cache time */
    void setCacheTime(double vtime);
    /** @brief renvoie l'etat de la pause */
    bool getPauseState();
    /** @brief force l'etat de la pause */
    void ForcePauseState();
protected:
	/** @brief paint event */
    virtual void paintEvent ( QPaintEvent * event );
	/** @brief enter event */
    virtual void enterEvent ( QEvent * event );
	/** @brief leave event */
    virtual void leaveEvent ( QEvent * event );
	/** @brief press event */
    virtual void mousePressEvent ( QMouseEvent * event );
private:
    /* @brief time to text time */
    QString timeToTextTime(double vtime);
    /* @brief etat */
    bool _rEnable;
    /* @brief pixmap et etat pause */
    QPixmap _pPause;
    bool _pause;
    bool _pauseProposal;
    /* @brief time */
    double _startTime;
    double _stopTime;
    double _cacheTime;
    double _currentTime;

};

#endif
