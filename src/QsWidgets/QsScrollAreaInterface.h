
#ifndef QS_WIDGETS_QSSCROLLAREAINTERFACE_H
#define QS_WIDGETS_QSSCROLLAREAINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsScrollAreaInterface : public QsInterfaceAdapter
{

public:

    QsScrollAreaInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
