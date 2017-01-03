
#ifndef QS_WIDGETS_QSLISTWIDGETINTERFACE_H
#define QS_WIDGETS_QSLISTWIDGETINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsListWidgetInterface : public QsInterfaceAdapter
{

public:

    QsListWidgetInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
