
#ifndef QS_WIDGETS_QSDOUBLESPINBOXINTERFACE_H
#define QS_WIDGETS_QSDOUBLESPINBOXINTERFACE_H

#include <QsInterfaceAdapter.h>

class QsDoubleSpinBoxInterface : public QsInterfaceAdapter
{

public:

    QsDoubleSpinBoxInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
