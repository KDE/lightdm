#ifndef SELECTIMAGEBUTTONDESIGNERFACTORY_H
#define SELECTIMAGEBUTTONDESIGNERFACTORY_H

#include <QDesignerCustomWidgetInterface>
#include <QObject>

class SelectImageButtonDesignerFactory : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    SelectImageButtonDesignerFactory(QObject *parent = 0);

    bool isContainer() const;
    QIcon icon() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget *createWidget(QWidget *parent);
};



#endif // SELECTIMAGEBUTTONDESIGNERFACTORY_H
