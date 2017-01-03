
#ifndef QS_WIDGETS_QSTOOLBUTTONINTERFACE_H
#define QS_WIDGETS_QSTOOLBUTTONINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsToolButtonInterface : public QsInterfaceAdapter
{

public:

    QsToolButtonInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
