
#ifndef QS_WIDGETS_QSCOORDINATELINEINTERFACE_H
#define QS_WIDGETS_QSCOORDINATELINEINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsCoordinateLineInterface : public QsInterfaceAdapter
{

public:

    QsCoordinateLineInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
