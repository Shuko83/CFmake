
#ifndef QS_WIDGETS_QsSearchComboBoxInterface_H
#define QS_WIDGETS_QsSearchComboBoxInterface_H

#include <QsInterfaceAdapter.h>


class QsSearchComboBoxInterface : public QsInterfaceAdapter
{

public:

    QsSearchComboBoxInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;

};

#endif
