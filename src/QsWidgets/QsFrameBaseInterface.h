
#ifndef QS_WIDGETS_QSFRAMEBASEINTERFACE_H
#define QS_WIDGETS_QSFRAMEBASEINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsFrameBaseInterface : public QsInterfaceAdapter
{

public:

    QsFrameBaseInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
