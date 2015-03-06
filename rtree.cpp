#include "rtree.h"
#include <QPainter>
#include <QDebug>

static const int maxItems = 32;


struct Centroid {
    Point point;
    QList<Point> points;
};

RTree::RTree()
{

}

RTree::~RTree()
{

}

int RTree::size() const
{
    int s = 0;
    mRootNode.size(s);
    return s;
}

void RTree::addPoint(const Point &point)
{
    mRootNode.doInsert(point);
}

void RTree::clearTree()
{
    mRootNode.clear();
}

void RTreeNode::render(QPainter *painter, const QRect &rect, const QRectF &boundRect)
{

    auto normBoundRect = boundingRect().normalized();
    if (boundRect.intersects(normBoundRect)) {
        if (!hasChildren()){
            painter->drawRect(normBoundRect);
        } else {
            int count = 0;
            for (RTreeNode *childNode : mChildNodes) {
                painter->save();
                if (count == 0) {
                    painter->setPen(Qt::blue);
                } else {
                    painter->setPen(Qt::green);
                }
                childNode->render(painter, rect, boundRect);
                painter->restore();
                ++count;
            }
        }
        painter->save();
        for (const Point &point : mPoints) {
            if (boundRect.contains(point.point)) {
                painter->drawPoint(point.point);
            }
        }
        painter->restore();
    }
}

QRectF RTree::boundingRect() const
{
    return mRootNode.boundingRect();
}

QList<Point> RTree::query(const QRectF &boundingRect)
{
    QList<Point> retVal;
    mRootNode.query(retVal, boundingRect.normalized());
    return retVal;
}

void RTree::render(QPainter *painter, const QRect &rect, const QRectF &boundRect)
{
    painter->save();
    QTransform t;
    qreal scaleX = rect.width() / boundRect.normalized().width();
    qreal scaleY = rect.height() / boundRect.normalized().height();
    t.scale(scaleX, scaleY);
    t.translate(-boundRect.normalized().topLeft().x(), -boundRect.normalized().topLeft().y());
    painter->setTransform(t);
    mRootNode.render(painter, rect, boundRect);
    painter->restore();
}

RTreeNode::RTreeNode(const QRectF &boundingRect) :
    mBoundingRect(boundingRect)
{

}

RTreeNode::~RTreeNode()
{
    clear();
}

void RTreeNode::clear()
{
    qDeleteAll(mChildNodes);
    mChildNodes.clear();
    mPoints.clear();
}

void RTreeNode::doInsert(const Point &point)
{
    adjustBounds(point);
    if (hasChildren()) {
        RTreeNode *choisedLeaf = choiceLeaf(point);
        if (choisedLeaf){
            choisedLeaf->doInsert(point);
        }
    } else {
        if (mPoints.size() < maxItems) {
            mPoints.append(point);
        } else {
            linearSplit();
            doInsert(point);
        }
    }
}

QRectF RTreeNode::boundHelper(const QList<Point> &points)
{
    if (points.count() == 0) {
        return QRectF();
    }
    qreal minX = std::numeric_limits<qreal>::max();
    qreal maxX = std::numeric_limits<qreal>::min();
    qreal minY = minX;
    qreal maxY = maxX;
    for (const Point &point: points) {
        if (point.point.x() > maxX) maxX = point.point.x();
        if (point.point.x() < minX) minX = point.point.x();
        if (point.point.y() > maxY) maxY = point.point.y();
        if (point.point.y() < minY) minY = point.point.y();
    }
    return QRectF(QPointF(minX, minY ), QPointF(maxX, maxY));
}

QRectF RTreeNode::boundingRect() const
{
    return mBoundingRect;
}

void RTreeNode::query(QList<Point> &points, const QRectF &boundingRect)
{
    if (mBoundingRect.intersects(boundingRect)) {
        if (hasChildren()) {
            for (RTreeNode *node : mChildNodes) {
                node->query(points, boundingRect);
            }
        } else {
            for (const Point &point : mPoints) {
                if (boundingRect.contains(point.point)) {
                    points.append(point);
                }
            }
        }
    }
}

void RTreeNode::linearSplit()
{
    /*
    Corner-based splitting: An improved
    node splitting algorithm for R-tree
    */
    QRectF bound = boundHelper(mPoints);
    //QRectF bound = mBoundingRect;

    qreal halfX = bound.width() / 2.;
    qreal halfY = bound.height() / 2.;
    QSizeF halfSize = bound.size() / 2.;
    int count[4];
    QList<Point> points[4];
    QRectF rects[4];
    memset(count, 0, sizeof(int) * 4);
    rects[0] = QRectF(bound.topLeft() + QPointF(0, halfY), halfSize);
    rects[1] = QRectF(bound.topLeft(), halfSize);
    rects[2] = QRectF(bound.topLeft() + QPointF(halfX, 0), halfSize);
    rects[3] = QRectF(bound.topLeft() + QPointF(halfX, halfY), halfSize);
    for (const Point &point : mPoints) {
        bool match = false;
        for (int i = 0; i < 4; ++i) {
            if (rects[i].contains(point.point)) {
                ++count[i];
                points[i].append(point);
                match = true;
                break;
            }
        }
        if (!match) {
            // обработка ситуации, когда из-за ошибки точности qreal точка
            // не попадает ни в один из углов прямоугльника
            QList<QRectF> r = {rects[0], rects[1], rects[2], rects[3]};
            int idxToAdd = nearest(r, point.point);
            if (idxToAdd >= 0) {
                points[idxToAdd].append(point);
            } else {
                qDebug() << "error" << idxToAdd;
            }
        }
    }
    RTreeNode *n1 = new RTreeNode();
    RTreeNode *n2 = new RTreeNode();

    if (count[0] > count[2]) {
        n1->mPoints.append(points[0]);
        n2->mPoints.append(points[2]);
    } else {
        n1->mPoints.append(points[2]);
        n2->mPoints.append(points[0]);
    }

    if (count[1] > count[3]) {
        n1->mPoints.append(points[3]);
        n2->mPoints.append(points[1]);
    } else {
        n1->mPoints.append(points[1]);
        n2->mPoints.append(points[3]);

    }
    n1->adjustBounds();
    n2->adjustBounds();
    mChildNodes.append(n1);
    mChildNodes.append(n2);

    mPoints.clear();
}

RTreeNode *RTreeNode::choiceLeaf(const Point &point)
{
    for (RTreeNode *node : mChildNodes) {
        if (node->mBoundingRect.contains(point.point)) {
            return node;
        }
    }
    // не нашли подходящего узла
    // будем искать ближайший

    RTreeNode *nearest = nullptr;
    qreal minDist = std::numeric_limits<qreal>::max();
    for (RTreeNode *node : mChildNodes) {
        qreal dist = squareDistTo(point, node->mBoundingRect);
        if (dist < minDist) {
            nearest = node;
            minDist = dist;
        }
    }
    return nearest;
}

void RTreeNode::adjustBounds(const Point &point)
{
    if (mBoundingRect.contains(point.point)) {
        return;
    }
    if (mBoundingRect.isNull()) {
        qreal min = std::numeric_limits<qreal>::min() * 100;
        mBoundingRect = QRectF(point.point, QSizeF(min, min));
        return;
    }
    if (point.point.y() > mBoundingRect.bottom()) {
        mBoundingRect.setBottom(point.point.y());
    } else if (point.point.y() < mBoundingRect.top()) {
        mBoundingRect.setTop(point.point.y());
    }
    if (point.point.x() > mBoundingRect.right()) {
        mBoundingRect.setRight(point.point.x());
    } else if (point.point.x() < mBoundingRect.left()) {
        mBoundingRect.setLeft(point.point.x());
    }
}

void RTreeNode::adjustBounds()
{
   if (hasChildren()) {
        QList<Point> listP;
        for (RTreeNode *node : mChildNodes) {
            Point p;
            p.point = node->mBoundingRect.topLeft();
            listP.append(p);
            p.point = node->mBoundingRect.bottomRight();
            listP.append(p);
        }
        mBoundingRect = boundHelper(listP);
   } else {
       mBoundingRect = boundHelper(mPoints);
   }
}

void RTreeNode::size(int &count) const
{
    for (RTreeNode *node : mChildNodes) {
        node->size(count);
    }
    count += mPoints.count();
}

bool RTreeNode::hasChildren() const
{
    return !mChildNodes.isEmpty();
}

qreal RTreeNode::squareDistTo(const Point &point, const QRectF &rect)
{
    if (point.point.x() >= rect.left() && point.point.x() <= rect.right()) {
        // прямоугольник находиться либо над либо под точкой
        if (rect.bottom() <= point.point.y()) {
            // над
            return pow(point.point.y() - rect.bottom(), 2);
        } else {
            return pow(rect.top() - point.point.y(), 2);
        }
    }

    if (point.point.y() >= rect.top() && point.point.y() <= rect.bottom()) {
        // прямоугольник либо слева либо справа
        if (rect.right() <= point.point.x()) {
          // слева
            return pow(point.point.x() - rect.right(), 2);
        } else {
            return pow(rect.left() - point.point.x(), 2);
        }
    }

    if (point.point.x() > rect.right()) {
        // где-то слева
        qreal x = point.point.x() - rect.right();
        x *= x;
        if (point.point.y()  > rect.bottom()) {
            // слева-вверху
            qreal y = point.point.y() - rect.bottom();
            y *= y;
            return x + y;
        } else {
            qreal y = rect.top() - point.point.y();
            y *= y;
            return x + y;
        }
    }

    qreal x = rect.left() - point.point.x();
    x *= x;
    if (point.point.y()  > rect.bottom()) {
        // справа-вверху
        qreal y = point.point.y() - rect.bottom();
        y *= y;
        return x + y;
    } else {
        qreal y = rect.top() - point.point.y();
        y *= y;
        return x + y;
    }

    return 0;
}

int RTreeNode::nearest(const QList<QRectF> &rects, const QPointF &point)
{
    qreal minDst = std::numeric_limits<qreal>::max();
    int idxToReturn = -1;
    for (int idx = 0; idx < rects.count(); ++idx) {
        Point p;
        p.point = point;
        qreal tmp = squareDistTo(p, rects.at(idx));
        if (tmp < minDst) {
            idxToReturn = idx;
            minDst = tmp;
        }
    }
    return idxToReturn;
}
