#ifndef _QRCVIEWER_H
#define _QRCVIEWER_H

#include <QDialog>
#include "SwGuiConstantes.h"
#include "ui__QRcViewerUi.h"

class SWGUI_EXPORT _QRcViewer : public QDialog, public Ui::_QRcViewerUi
{
	Q_OBJECT

public:
	_QRcViewer(QDialog *parent = 0);
	~_QRcViewer();
	void setIconName(QString name);

public slots:
	void itemSelected(QListWidgetItem *);
	void selectItem();
signals:
    void iconSelected( const QString &);
	

private:
    QPixmap makeThumbnail(const QPixmap &pix) const;

	QHash<QString,QPixmap> _cached;

};

#endif // QTRESOURCE_H
