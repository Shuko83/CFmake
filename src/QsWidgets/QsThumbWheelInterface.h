
#ifndef QS_WIDGETS_QSTHUMBWHEELINTERFACE_H
#define QS_WIDGETS_QSTHUMBWHEELINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsThumbWheelInterface : public QsInterfaceAdapter
{

public:

	QsThumbWheelInterface(QObject *parent = 0);

	QString domXml() const;
	QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
