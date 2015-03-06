#ifndef RTREEWIDGET_H
#define RTREEWIDGET_H

#include <QWidget>
#include <rtree.h>

class RTreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RTreeWidget(QWidget *parent = 0);
    ~RTreeWidget();

    void paintEvent(QPaintEvent *);

private:
    qreal frnd() const;

signals:

public slots:
    void setPointsCount(const int count);
    void addPoint();
    void queryAll();
    void clear();

private:
    int mPointsCountToAdd;
    int mPointsCountTotal;
    RTree mTree;
    QPixmap p;
};

#endif // RTREEWIDGET_H
