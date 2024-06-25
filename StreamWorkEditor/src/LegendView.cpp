#include "LegendView.h"
#include "ManageLinkColor.h"

LegendView::LegendView(QWidget *parent)
	: QWidget(parent),_pix(QSize(10,10))
{
	_formLayout = new QGridLayout (this);
	_formLayout->setSpacing(5);
	_formLayout->setMargin(2);

	connect(ManageLinkColor::getInstance(),SIGNAL(colorHasChange()),this,SLOT(updateLegend()));
	updateLegend();
}

LegendView::~LegendView()
{

}

//-------------------------------------------------------------------------
void LegendView::updateLegend()
{
	ManageLinkColor* linkC = ManageLinkColor::getInstance();
	if(linkC )
	{
		QMapIterator<QString,QString> it(linkC->getMapColor());

		int index=0;
		while(it.hasNext())
		{
			it.next();
			if(!_map.contains(it.key()))
			{
				QLabel * lab = new QLabel(it.key()+":",this);
				_formLayout->addWidget(lab,index,0,Qt::AlignRight);
				QPushButton * tmpBtn = new QPushButton(it.value());
				tmpBtn->setFlat(true);
				_pix.fill(linkC->getColorForInterface(it.key()));
				tmpBtn->setIcon(QIcon(_pix));
				_formLayout->addWidget(tmpBtn,index,1,Qt::AlignLeft);
				Labelpush blop;
				blop.L=lab;
				blop.P = tmpBtn;
				_map.insert(it.key(),blop);
			}
			else
			{
				//QPixmap tmpPix = _map[it.key()].P->icon().pixmap();
				_pix.fill(linkC->getColorForInterface(it.key()));
				_map[it.key()].P->setText(it.value());
				_map[it.key()].P->setIcon(QIcon(_pix));
			}
			index++;

		}
	}
}
