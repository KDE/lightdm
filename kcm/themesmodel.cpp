#include "themesmodel.h"

#include <QString>
#include <QPixmap>
#include <QList>
#include <QDebug>
#include <QDir>
#include <QSettings>

class ThemeItem {
public:
    QString id;
    QString name;
    QString description;
    QPixmap preview;
};

ThemesModel::ThemesModel(QObject *parent) :
    QAbstractListModel(parent)
{
    this->load();
}

int ThemesModel::rowCount(const QModelIndex &parent) const
{
    return m_themes.size();
}

QVariant ThemesModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    switch(role) {
    case Qt::DisplayRole:
        return m_themes[row]->name;
    case Qt::DecorationRole:
        if (m_themes[row]->preview.isNull()) {
            return QVariant();
        }
        return m_themes[row]->preview.scaled(QSize(80,80), Qt::KeepAspectRatio);
    case ThemesModel::PreviewRole:
        return m_themes[row]->preview;
    case Qt::ToolTipRole:
        return m_themes[row]->description;
    }

    return QVariant();
}

void ThemesModel::load()
{
    qDebug() << "loading themes";
    QDir lightDmDir("/usr/local/share/lightdm/themes");
    foreach(const QString dir, lightDmDir.entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
        if (QFile::exists(lightDmDir.filePath(dir+"/index.theme"))) {
            loadTheme(QDir(lightDmDir.filePath(dir)));
        }
    }
}

void ThemesModel::loadTheme(const QDir &themePath) {
    QSettings themeInfo(themePath.filePath("index.theme"), QSettings::IniFormat);

    ThemeItem *theme = new ThemeItem;
    theme->id = themePath.dirName();
    theme->name = themeInfo.value("theme/name").toString();
    theme->description = themeInfo.value("theme/description").toString();
    theme->preview = QPixmap(themePath.filePath(themeInfo.value("theme/preview").toString()));

    qDebug() << QString("adding theme") << theme->name;

    beginInsertRows(QModelIndex(), m_themes.size(), m_themes.size()+1);
    m_themes.append(theme);
    endInsertRows();
}
