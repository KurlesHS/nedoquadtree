#include "qtreeimplmainwindow.h"
#include "ui_qtreeimplmainwindow.h"

QTreeImplMainWindow::QTreeImplMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QTreeImplMainWindow)
{
    ui->setupUi(this);
    connect(ui->spinBox, SIGNAL(valueChanged(int)),
            ui->widget, SLOT(setPointsCount(int)));
    connect(ui->pushButton, SIGNAL(clicked()),
            ui->widget, SLOT(addPoint()));
    connect(ui->pushButtonQuery, SIGNAL(clicked()),
            ui->widget, SLOT(queryAll()));
    connect(ui->pushButtonClear, SIGNAL(clicked()),
            ui->widget, SLOT(clear()));

}

QTreeImplMainWindow::~QTreeImplMainWindow()
{
    delete ui;
}

void QTreeImplMainWindow::setPixmap(const QPixmap &pixmap)
{
    Q_UNUSED(pixmap)
    //ui->label->setPixmap(pixmap);
}
