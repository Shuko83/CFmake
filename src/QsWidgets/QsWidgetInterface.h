
#ifndef QS_WIDGETS_QSWIDGETINTERFACE_H
#define QS_WIDGETS_QSWIDGETINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsWidgetInterface : public QsInterfaceAdapter
{

public:

    QsWidgetInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
