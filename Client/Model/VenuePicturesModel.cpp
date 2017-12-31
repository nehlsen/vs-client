#include "VenuePicturesModel.h"
#include "VenuePictures.h"

VenuePicturesModel::VenuePicturesModel(QObject *parent):
    QAbstractTableModel(parent)
{
}

VenuePicturesModel::VenuePicturesModel(VenuePictures *venuePictures, QObject *parent):
    QAbstractTableModel(parent)
{
    connect(venuePictures, &VenuePictures::pictureListCleared,
            this, &VenuePicturesModel::onListCleared);
    connect(venuePictures, &VenuePictures::pictureReady,
            this, &VenuePicturesModel::onPictureReady);
}

QVariant VenuePicturesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation != Qt::Horizontal) {
        return QVariant();
    }

    switch (section) {
        case 0: return tr("Hash");
        case 1: return tr("Local Path");
        case 2: return tr("Created By");
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

int VenuePicturesModel::rowCount(const QModelIndex &parent) const
{
    return m_pictures.count();
}

int VenuePicturesModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant VenuePicturesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= rowCount(QModelIndex())) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return m_pictures.at(index.row())->hash();
            case 1: return m_pictures.at(index.row())->localPath();
            case 2: return m_pictures.at(index.row())->createdBy();
        }
    } else {
        switch (role) {
            case HashRole: return m_pictures.at(index.row())->hash();
            case LocalPathRole: return m_pictures.at(index.row())->localPath();
            case CreatedByRole: return m_pictures.at(index.row())->createdBy();
        }
    }

    return QVariant();
}

QHash<int, QByteArray> VenuePicturesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[HashRole] = "hash";
    roles[LocalPathRole] = "localPath";
    roles[CreatedByRole] = "createdBy";
    return roles;
}

void VenuePicturesModel::onListCleared()
{
    beginResetModel();
    m_pictures.clear();
    endResetModel();
}

void VenuePicturesModel::onPictureReady(const VenuePicture *picture)
{
    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    m_pictures.append(picture);
    endInsertRows();
}
