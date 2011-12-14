#ifndef THEMESMODEL_H
#define THEMESMODEL_H

#include <QAbstractListModel>

class ThemeItem;
class QDir;

class ThemesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {IdRole = Qt::UserRole,
               AuthorRole,
               DescriptionRole,
               VersionRole,
               PreviewRole,
               PathRole};

    explicit ThemesModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

signals:

public slots:

private:
    void load();
    void loadTheme(const QDir &themePath);
    QList<ThemeItem*> m_themes;

};

#endif // THEMESMODEL_H
