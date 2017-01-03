
#ifndef QS_WIDGETS_QSSPINBOXINTERFACE_H
#define QS_WIDGETS_QSSPINBOXINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsSpinBoxInterface : public QsInterfaceAdapter
{

public:

    QsSpinBoxInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
