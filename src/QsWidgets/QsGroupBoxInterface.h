
#ifndef QS_WIDGETS_QSGROUPBOXINTERFACE_H
#define QS_WIDGETS_QSGROUPBOXINTERFACE_H

#include <QsInterfaceAdapter.h>

class QsGroupBoxInterface : public QsInterfaceAdapter
{

public:

    QsGroupBoxInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
