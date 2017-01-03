
#ifndef QS_WIDGETS_QSCHECKBOXINTERFACE_H
#define QS_WIDGETS_QSCHECKBOXINTERFACE_H

#include <QsInterfaceAdapter.h>



class QsCheckBoxInterface : public QsInterfaceAdapter
{

public:

    QsCheckBoxInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
