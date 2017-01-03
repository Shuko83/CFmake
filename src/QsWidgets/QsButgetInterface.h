
#ifndef QS_WIDGETS_QSBUTGETINTERFACE_H
#define QS_WIDGETS_QSBUTGETINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsButgetInterface : public QsInterfaceAdapter
{

public:

    QsButgetInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};


#endif
