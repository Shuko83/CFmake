
#ifndef QS_WIDGETS_QSGROUPRADIOINTERFACE_H
#define QS_WIDGETS_QSGROUPRADIOINTERFACE_H

#include <QsInterfaceAdapter.h>

class QsGroupRadioInterface : public QsInterfaceAdapter
{

public:

	QsGroupRadioInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
