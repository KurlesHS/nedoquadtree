#ifndef RTREE_H
#define RTREE_H

#include <QRectF>
#include <QVector>
#include <QList>

#include "point.h"

class QPainter;

class RTreeNode
{
public:
    RTreeNode(const QRectF &boundingRect = QRectF());
    ~RTreeNode();
    void doInsert(const Point &point);
    void query(QList<Point> &points, const QRectF &boundingRect);
    void clear();

    QRectF boundingRect() const;
    void size(int &count) const;

    void render(QPainter *painter, const QRect &rect, const QRectF &boundRect);

private:
    void adjustBounds(const Point &point);
    void adjustBounds();

    void linearSplit();
    RTreeNode *choiceLeaf(const Point &point);
    bool hasChildren() const;

    static int nearest(const QList<QRectF> &rects, const QPointF &point);
    static QRectF boundHelper(const QList<Point> &points);
    static qreal squareDistTo(const Point &point, const QRectF &rect);


private:
    QRectF mBoundingRect;
    QList<RTreeNode*> mChildNodes;
    QList<Point> mPoints;
};

class RTree
{
public:
    RTree();
    ~RTree();
    int size() const;
    void addPoint(const Point &point);
    void clearTree();
    QRectF boundingRect() const;
    QList<Point> query(const QRectF &boundingRect);
    void render(QPainter *painter, const QRect &rect, const QRectF &boundRect);

private:
    RTreeNode mRootNode;
};

#endif // RTREE_H
