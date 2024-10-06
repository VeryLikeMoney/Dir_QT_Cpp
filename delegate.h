#ifndef DELEGATE_H
#define DELEGATE_H

#include <QObject>
#include <QModelIndex>
#include <QStyledItemDelegate>
#include <QEvent>
#include <QFileSystemModel>
#include <map>
#include <QString>
#include <iostream>


class TreeButtonDelegate : public QStyledItemDelegate {
    Q_OBJECT
    std::map<QString, QString> clickedPath;

    public:
    QFileSystemModel* fmodel;
    void addDirSize(QString dirPath, QString dirSize);
    explicit TreeButtonDelegate(QFileSystemModel* model, QObject *parent = nullptr) : QStyledItemDelegate(parent) {
        this->fmodel = model;
    }
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                        const QStyleOptionViewItem &option, const QModelIndex &index) override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    signals:
        void treeButtonClickedSignal(QModelIndex index);
        void updateModelSignal() const;
};


#endif // DELEGATE_H
