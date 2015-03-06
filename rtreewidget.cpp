#include "rtreewidget.h"
#include <QPainter>
#include <QDateTime>
#include <QDebug>

RTreeWidget::RTreeWidget(QWidget *parent) :
    QWidget(parent),
    mPointsCountToAdd(1),
    mPointsCountTotal(0)
{
    qsrand(QDateTime::currentMSecsSinceEpoch());
}

RTreeWidget::~RTreeWidget()
{

}

void RTreeWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    mTree.render(&p, rect(), mTree.boundingRect());
}

qreal RTreeWidget::frnd() const
{
    return ((qreal)qrand() / (RAND_MAX - 1));
}

void RTreeWidget::setPointsCount(const int count)
{
    mPointsCountToAdd = count;
}

void RTreeWidget::addPoint()
{
    for (int i = 0; i < mPointsCountToAdd; ++i) {
        Point p;
        p.point = QPointF(frnd(), frnd());
        mTree.addPoint(p);
    }
    mPointsCountTotal += mPointsCountToAdd;
    update();
}

void RTreeWidget::queryAll()
{
    //auto points = mTree.query(mTree.boundingRect());
    auto points = mTree.query(QRectF(QPointF(-1, -1), QSizeF(3, 3)));

    qDebug() << mPointsCountTotal << points.size() << mTree.size();
}

void RTreeWidget::clear()
{
    mTree.clearTree();
    mPointsCountTotal = 0;
    update();
}

