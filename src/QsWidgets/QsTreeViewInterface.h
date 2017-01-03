#ifndef QS_WIDGETS_QSTREEVIEWINTERFACE_H
#define QS_WIDGETS_QSTREEVIEWINTERFACE_H

#include <QsInterfaceAdapter.h>

class QsTreeViewInterface : public QsInterfaceAdapter
{
public:
    QsTreeViewInterface(QObject *parent = 0);
    
    QString domXml() const;
    QWidget *createWidget(QWidget *parent);

	virtual QIcon icon() const;
};

#endif
