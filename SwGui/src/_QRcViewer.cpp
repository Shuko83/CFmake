#include "_QRcViewer.h"
#include <QDir>
#include <QDirIterator>
#include <QListWidget>
#include <QDebug>
#include <QPainter>

/*****************************************************************************/
_QRcViewer::_QRcViewer(QDialog *parent): QDialog(parent)
{
	setupUi(this);
	QDir t_qrcDir(":/");

	QDirIterator itDir(t_qrcDir, QDirIterator::Subdirectories);

	//Parcours toutes les resources de l'appli et peuple la listWidget
	while (itDir.hasNext()) 
	{
		QString currentName = itDir.next();
		if(!QFileInfo(currentName).isDir() && QFileInfo(currentName).suffix() != "html")
		{
			QString filename = currentName.mid(currentName.lastIndexOf("/")+1);

			QIcon icon;
			if(_cached.contains(currentName))
				icon.addPixmap(_cached[currentName]);
			else
			{
				QPixmap pix = makeThumbnail(QPixmap(currentName));
				icon.addPixmap(pix);
				_cached.insert(currentName,pix);
			}

			QListWidgetItem * t_item = new QListWidgetItem(icon,filename,LW_icon);
			t_item->setData(Qt::UserRole,currentName);
		}
	}

	//Connect les slots
	connect(LW_icon,SIGNAL(itemDoubleClicked ( QListWidgetItem * )),this,SLOT(itemSelected(QListWidgetItem *)));
	connect(PB_Ok,SIGNAL(clicked()),this,SLOT(selectItem()));
}

/*****************************************************************************/
_QRcViewer::~_QRcViewer()
{

}


/*****************************************************************************/
QPixmap _QRcViewer::makeThumbnail(const QPixmap &pix) const
{
	//Generer une image de 48x48 pour que tous les icons soient 
 	//de la même taille
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

/*****************************************************************************/
void _QRcViewer::setIconName( QString name )
{
	//Parcours la liste des QListWidgetItem pour savoir lequel est sélectionné
	for (int i = 0; i < LW_icon->count() ; i++)
	{
		if(LW_icon->item(i) && LW_icon->item(i)->data(Qt::UserRole).toString() == name)
			LW_icon->setCurrentItem(LW_icon->item(i));
	}
}

/*****************************************************************************/
void _QRcViewer::itemSelected( QListWidgetItem * item)
{
	selectItem();
}

/*****************************************************************************/
void _QRcViewer::selectItem()
{
	// On récupère a selection qui est unique (seulement un item)
	QList<QListWidgetItem*> tmpList = LW_icon->selectedItems();

	if(tmpList.count() == 1 && tmpList.at(0))
	{
		emit iconSelected(tmpList.at(0)->data(Qt::UserRole).toString());
	}

	// On ferme la dialog ;)
	accept();
}
