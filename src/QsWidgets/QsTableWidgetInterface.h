
#ifndef QS_WIDGETS_QSTABLEWIDGETINTERFACE_H
#define QS_WIDGETS_QSTABLEWIDGETINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsTableWidgetInterface : public QsInterfaceAdapter
{

public:

    QsTableWidgetInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
