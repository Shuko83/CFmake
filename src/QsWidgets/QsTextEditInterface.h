
#ifndef QS_WIDGETS_QSTEXTEDITINTERFACE_H
#define QS_WIDGETS_QSTEXTEDITINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsTextEditInterface : public QsInterfaceAdapter
{

public:

    QsTextEditInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
