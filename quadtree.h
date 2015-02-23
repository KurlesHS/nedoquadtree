#ifndef QUADTREE_H
#define QUADTREE_H

#include <QRectF>
#include <QList>
#include <QString>

struct Point {
    QPointF point;
    QString descr;
};

class QPainter;

class QuadTreeNode {
public:
    QuadTreeNode(const QRectF &mBoundingRect = QRectF(), QuadTreeNode *parent = 0, const int mLevel=0);
    ~QuadTreeNode();
    void deleteChildNodes();
    void removeAllPoints();
    QRectF boundingRect() const;

    QuadTreeNode *childNode(const int index);
    void splitNode();
    const QList<Point> &points() const;
    bool hasChildren() const;
    bool addPoint(const Point &point);
    void query(QList<Point> &points, const QRectF &boundingRect);

    int level() const;

private:
    QRectF mBoundingRect;
    QuadTreeNode* mChildNodes[4];
    QuadTreeNode *mParentNode;
    QList<Point> mPoints;
    int mLevel;
};

class QuadTree
{
public:
    QuadTree(const QRectF &boundingRect);
    ~QuadTree();
    bool addPoint(const Point &point);
    void clearTree();
    QList<QPoint> points;
    QRectF boundingRect() const;
    QList<Point> query(const QRectF &boundingRect);
    int nodesCount() const;
    int maxLevel() const;
    void render(QPainter *painter, const QRect &rect, const QList<Point> &points = QList<Point>(), QRectF boundRect = QRectF());

private:
    void render(QPainter *painter, const QRect &rect, QuadTreeNode *node, const QRectF &boundRect);

private:
    QuadTreeNode mRootNode;
    int mNodesCount;
    int mMaxLevel;
};

#endif // QUADTREE_H
