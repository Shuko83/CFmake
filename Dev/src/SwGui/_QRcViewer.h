#ifndef _QRCVIEWER_H
#define _QRCVIEWER_H

#include <QDialog>
#include "SwGuiConstantes.h"
#include "ui__QRcViewerUi.h"

using namespace Ui;

class SWGUI_EXPORT _QRcViewer : public QDialog, public _QRcViewerUi
{
	Q_OBJECT

public:
	_QRcViewer(QDialog *parent = 0);
	~_QRcViewer();
	void setIconName(QString name);

signals:
    void iconSelected( const QString &);

private:
    QPixmap makeThumbnail(const QPixmap &pix) const;

};

#endif // QTRESOURCE_H
