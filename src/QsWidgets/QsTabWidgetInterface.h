
#ifndef QS_WIDGETS_QSTABWIDGETINTERFACE_H
#define QS_WIDGETS_QSTABWIDGETINTERFACE_H

#include <QsInterfaceAdapter.h>



class QsTabWidgetInterface : public QsInterfaceAdapter
{

public:

    QsTabWidgetInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
