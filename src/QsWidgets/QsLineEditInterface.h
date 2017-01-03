
#ifndef QS_WIDGETS_QSLINEEDITINTERFACE_H
#define QS_WIDGETS_QSLINEEDITINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsLineEditInterface : public QsInterfaceAdapter
{

public:

    QsLineEditInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
