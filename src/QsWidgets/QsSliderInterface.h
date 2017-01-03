
#ifndef QS_WIDGETS_QSSLIDERINTERFACE_H
#define QS_WIDGETS_QSSLIDERINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsSliderInterface : public QsInterfaceAdapter
{

public:

    QsSliderInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
