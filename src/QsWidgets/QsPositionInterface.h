
#ifndef QS_WIDGETS_QSPOSITIONINTERFACE_H
#define QS_WIDGETS_QSPOSITIONINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsPositionInterface : public QsInterfaceAdapter
{

public:

    QsPositionInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
