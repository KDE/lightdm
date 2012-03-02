#include "selectimagebuttondesignerfactory.h"

#include <QtCore/QtPlugin>

#include "selectimagebutton.h"

SelectImageButtonDesignerFactory::SelectImageButtonDesignerFactory(QObject *parent)
: QObject(parent)
{
}

bool SelectImageButtonDesignerFactory::isContainer() const
{
    return false;
}

QIcon SelectImageButtonDesignerFactory::icon() const
{
    return QIcon();
}

QString SelectImageButtonDesignerFactory::group() const
{
    return QString();
}

QString SelectImageButtonDesignerFactory::includeFile() const
{
    return "selectimagebutton.h";
}

QString SelectImageButtonDesignerFactory::name() const
{
    return "SelectImageButton";
}

QString SelectImageButtonDesignerFactory::toolTip() const
{
    return QString();
}

QString SelectImageButtonDesignerFactory::whatsThis() const
{
    return QString();
}

QWidget * SelectImageButtonDesignerFactory::createWidget(QWidget *parent)
{
    return new SelectImageButton(parent);
}

Q_EXPORT_PLUGIN2(lightdm_config_widgets, SelectImageButtonDesignerFactory)





