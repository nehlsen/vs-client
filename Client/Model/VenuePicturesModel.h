#ifndef VSC_SLIDESHOWMODEL_H
#define VSC_SLIDESHOWMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QList>
#include "VenuePicture.h"

class VenuePictures;

class VenuePicturesModel : public QAbstractTableModel
{
Q_OBJECT

public:
    enum EventRoles
    {
        HashRole = Qt::UserRole + 1,
        LocalPathRole,
        CreatedByRole
    };

    explicit VenuePicturesModel(QObject *parent = nullptr);
    explicit VenuePicturesModel(VenuePictures *venuePictures, QObject *parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void onListCleared();
    void onPictureReady(const VenuePicture *picture);

protected:
    QList<const VenuePicture*> m_pictures;
};

#endif // VSC_SLIDESHOWMODEL_H
