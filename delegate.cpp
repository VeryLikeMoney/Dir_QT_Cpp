#include "delegate.h"

bool TreeButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                    const QStyleOptionViewItem &option, const QModelIndex &index)
{
     if (event->type() == QEvent::MouseButtonRelease) {
        emit treeButtonClickedSignal(index);
        QString dirPath = fmodel->filePath(index);
        emit updateModelSignal();
        return 1;
     }
     return 0;
}

void TreeButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
     QStyledItemDelegate::paint(painter, option, index);
        if (fmodel->isDir(index)){
            QStyleOptionButton btnOption;
            btnOption.rect = option.rect.adjusted(10, 1, -10, -1);  // Положение кнопки в пределах ячейки
            QString dirPath = fmodel->filePath(index);
            if (clickedPath.count(dirPath)){
                btnOption.text = clickedPath.at(dirPath);
            }
            else{
                btnOption.text = "Обновить";
            }
            btnOption.state = QStyle::State_Enabled;
            option.widget->style()->drawControl(QStyle::CE_PushButton, &btnOption, painter);  
        }
}

void TreeButtonDelegate::addDirSize(QString dirPath, QString dirSize){
    clickedPath[dirPath] = dirSize;
}
