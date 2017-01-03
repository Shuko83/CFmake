
#ifndef QS_WIDGETS_QSPUSHBUTTONINTERFACE_H
#define QS_WIDGETS_QSPUSHBUTTONINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsPushButtonInterface : public QsInterfaceAdapter
{

public:

    QsPushButtonInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
