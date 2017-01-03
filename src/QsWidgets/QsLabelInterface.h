
#ifndef QS_WIDGETS_QSLABELINTERFACE_H
#define QS_WIDGETS_QSLABELINTERFACE_H

#include <QsInterfaceAdapter.h>



class QsLabelInterface : public QsInterfaceAdapter
{

public:

    QsLabelInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
