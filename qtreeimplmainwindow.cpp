#include "qtreeimplmainwindow.h"
#include "ui_qtreeimplmainwindow.h"

QTreeImplMainWindow::QTreeImplMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QTreeImplMainWindow)
{
    ui->setupUi(this);
}

QTreeImplMainWindow::~QTreeImplMainWindow()
{
    delete ui;
}

void QTreeImplMainWindow::setPixmap(const QPixmap &pixmap)
{
    ui->label->setPixmap(pixmap);
}
