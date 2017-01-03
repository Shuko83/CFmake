
#ifndef QS_WIDGETS_QSCOMBOBOXINTERFACE_H
#define QS_WIDGETS_QSCOMBOBOXINTERFACE_H

#include <QsInterfaceAdapter.h>


class QsComboBoxInterface : public QsInterfaceAdapter
{

public:

    QsComboBoxInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
