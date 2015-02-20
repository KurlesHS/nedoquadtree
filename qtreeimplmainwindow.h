#ifndef QTREEIMPLMAINWINDOW_H
#define QTREEIMPLMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class QTreeImplMainWindow;
}

class QTreeImplMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QTreeImplMainWindow(QWidget *parent = 0);
    ~QTreeImplMainWindow();
    void setPixmap(const QPixmap &pixmap);

private:
    Ui::QTreeImplMainWindow *ui;
};

#endif // QTREEIMPLMAINWINDOW_H
