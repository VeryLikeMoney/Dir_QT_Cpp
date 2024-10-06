#include <QtGui>
#include <QApplication>
#include <QTreeView>
#include <QMainWindow>
#include <QLineEdit>
#include <QWidget>
#include <QVBoxLayout>
#include <QSize>
#include <QDesktopWidget>
#include <QModelIndex>
#include <QStringList>
#include <QtGlobal>
#include <QThread>

#include "delegate.h"
#include "worker.h"


class MyWindow: public QMainWindow
{
    protected:
        QFileSystemModel* model = new QFileSystemModel(this);
        QTreeView* tree = new QTreeView(this);
        QLineEdit* lineEdit = new QLineEdit(this);
        QString path = QDir::homePath();
        TreeButtonDelegate *delegate = new TreeButtonDelegate(model,tree);

    public:
    void updateFilter(){
        // Добавление фильтра поиска
        QString filterText = lineEdit->text();
        if (!lineEdit->text().isEmpty()){
            model->setNameFilters(QStringList() << "*" + filterText + "*");
        }
        else{
            model->setNameFilters(QStringList() << "*");
        }
    }

    MyWindow(QWidget *parent = nullptr): QMainWindow(parent){
        this->setWindowTitle(QObject::tr("Dir View"));
        QWidget* central_widget = new QWidget;
        QVBoxLayout* layout = new QVBoxLayout(central_widget);
        tree->setModel(model);
        tree->setAnimated(false);
        tree->setIndentation(20);
        tree->setSortingEnabled(true);

        layout->addWidget(lineEdit);
        layout->addWidget(tree);

        // size setting
        this->setCentralWidget(central_widget);
        QSize size = QApplication::desktop()->availableGeometry(tree).size();
        this->resize(size / 2);
        tree->setColumnWidth(0, this->width() / 4);

        model->setNameFilterDisables(false);
        model->setFilter(QDir::Files | QDir::Dirs | QDir::Hidden);
        QModelIndex index = model->setRootPath(path);
        tree->setModel(model);
        tree->setRootIndex(index);
        tree->setItemDelegateForColumn(1, delegate);

        connect(lineEdit,&QLineEdit::textChanged,this, &MyWindow::updateFilter);
        TreeButtonDelegate::connect(delegate,  &TreeButtonDelegate::treeButtonClickedSignal,
                                    this, &MyWindow::treeButtonClicked);
        TreeButtonDelegate::connect(delegate,  &TreeButtonDelegate::updateModelSignal,
                                    this, &MyWindow::updateModel);
    }

    void updateModel(){
        QFileSystemModel* newModel = new QFileSystemModel(this);
        QModelIndex index = newModel->setRootPath(path);
        newModel->setNameFilterDisables(false);
        newModel->setFilter(QDir::Files | QDir::Dirs | QDir::Hidden);
        tree->setModel(newModel);
        tree->setRootIndex(index);
        delete model;
        this->model = newModel;
        this->delegate->fmodel = newModel;
        updateFilter();
    }

    void treeButtonClicked(QModelIndex index){
        // запуск отдельного потока на подчет размера
        QString dirPath = model->filePath(index);
        Worker* worker = new Worker(dirPath);
        QThread* thread = new QThread();
        worker->moveToThread(thread);
        connect(thread, &QThread::started, worker, &Worker::run);
        connect(worker, &Worker::addDirSizeSignals, delegate, &TreeButtonDelegate::addDirSize);
        //Завершает поток
        connect(worker, &Worker::addDirSizeSignals, thread, &QThread::quit);
        // Удаляем объекты после завершения работы
        connect(thread, &QThread::finished, worker, &QObject::deleteLater);
        connect(thread, &QThread::finished, thread, &QObject::deleteLater);
        thread->start();
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyWindow win = MyWindow();
    // Demonstrating look and feel features
    win.show();
    return app.exec();
}
