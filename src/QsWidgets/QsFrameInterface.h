
#ifndef QS_WIDGETS_QSFRAMEINTERFACE_H
#define QS_WIDGETS_QSFRAMEINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsFrameInterface : public QsInterfaceAdapter
{

public:

    QsFrameInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
