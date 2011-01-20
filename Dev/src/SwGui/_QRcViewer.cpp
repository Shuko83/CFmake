#include "_QRcViewer.h"
#include <QDir>
#include <QDirIterator>
#include <QListWidget>
#include <QDebug>
#include <QPainter>

_QRcViewer::_QRcViewer(QDialog *parent): QDialog(parent)
{
	setupUi(this);
	QDir toto(":/");
	//listWidget->setDragDropMode(DragDropMode::no)
	QDirIterator itDir(toto, QDirIterator::Subdirectories);

	while (itDir.hasNext()) 
	{
		QString currentName = itDir.next();

		if(!QFileInfo(currentName).isDir())
		{
			QString filename = currentName.mid(currentName.lastIndexOf("/")+1);

			QListWidgetItem * toto = new QListWidgetItem(QIcon(makeThumbnail(QPixmap(currentName))),filename,listWidget);
		}
	}
}

_QRcViewer::~_QRcViewer()
{

}


QPixmap _QRcViewer::makeThumbnail(const QPixmap &pix) const
{
	int w = qMax(48, pix.width());
	int h = qMax(48, pix.height());
	QRect imgRect(0, 0, w, h);
	QImage img(w, h, QImage::Format_ARGB32_Premultiplied);
	img.fill(0);
	if (!pix.isNull()) {
		QRect r(0, 0, pix.width(), pix.height());
		r.moveCenter(imgRect.center());
		QPainter p(&img);
		p.drawPixmap(r.topLeft(), pix);
	}
	return QPixmap::fromImage(img);
}

void _QRcViewer::setIconName( QString name )
{
    //TO DO-
}