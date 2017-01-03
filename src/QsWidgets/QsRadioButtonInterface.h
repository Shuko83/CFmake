
#ifndef QS_WIDGETS_QSRADIOBUTTONINTERFACE_H
#define QS_WIDGETS_QSRADIOBUTTONINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsRadioButtonInterface : public QsInterfaceAdapter
{

public:

    QsRadioButtonInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
