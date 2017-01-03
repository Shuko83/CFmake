
#ifndef QS_WIDGETS_QSINTERFACEADAPTER_H
#define QS_WIDGETS_QSINTERFACEADAPTER_H

#include <QDesignerCustomWidgetInterface>



class QsInterfaceAdapter : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    QsInterfaceAdapter(QObject *parent , QString name , QString includeFile, bool isContainer = false);

    virtual bool isContainer() const;
    virtual bool isInitialized() const;
    virtual QIcon icon() const;
    virtual QString domXml() const = 0;
    virtual QString group() const;
    virtual QString includeFile() const;
    virtual QString name() const;
    virtual QString toolTip() const;
    virtual QString whatsThis() const ;
    virtual QWidget *createWidget(QWidget *parent) = 0;
    virtual void initialize(QDesignerFormEditorInterface *core);

	void setStyleSheet(QString styleSheet);
protected:
	QString _styleSheet;

private:
    bool _initialized;
    QString _name;
    QString _includeFile;
    bool _isContainer;
};

#endif
