
#ifndef QS_WIDGETS_QSLABELTITLEINTERFACE_H
#define QS_WIDGETS_QSLABELTITLEINTERFACE_H

#include <QsInterfaceAdapter.h>

class QsLabelTitleInterface : public QsInterfaceAdapter
{

public:

    QsLabelTitleInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
