
#ifndef QS_WIDGETS_QSSMARTCOMBOBOXINTERFACE_H
#define QS_WIDGETS_QSSMARTCOMBOBOXINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsSmartComboBoxInterface : public QsInterfaceAdapter
{

public:

    QsSmartComboBoxInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
