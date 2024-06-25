#ifndef LEGENDVIEW_H
#define LEGENDVIEW_H

#include <QWidget>
#include <QEvent>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QMap>
typedef struct {
	QLabel *L;
	QPushButton *P;
}Labelpush;


class LegendView : public QWidget
{
	Q_OBJECT

public:
	LegendView(QWidget *parent = 0);
	~LegendView();

public slots:
	void updateLegend();

private:
	QGridLayout  *_formLayout;
	QMap<QString,Labelpush> _map;
	QPixmap _pix;

};

#endif // LEGENDVIEW_H
