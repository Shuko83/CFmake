
#ifndef QS_WIDGETS_QSDOCKWIDGETINTERFACE_H
#define QS_WIDGETS_QSDOCKWIDGETINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsDockWidgetInterface : public QsInterfaceAdapter
{

public:

    QsDockWidgetInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
