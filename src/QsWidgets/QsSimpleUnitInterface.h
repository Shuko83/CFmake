
#ifndef QS_WIDGETS_QSSIMPLEUNITINTERFACE_H
#define QS_WIDGETS_QSSIMPLEUNITINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsSimpleUnitInterface : public QsInterfaceAdapter
{

public:

    QsSimpleUnitInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
