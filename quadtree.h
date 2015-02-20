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
    QuadTreeNode(const QRectF &boundingRect = QRectF(), QuadTreeNode *parent = 0, const int level=0);
    QRectF boundingRect;
    QuadTreeNode* childNodes[4];
    QuadTreeNode *parentNode;
    QList<Point> points;
    static int splitCount;
    int level;
};

class QuadTree
{
public:
    QuadTree(const QRectF &boundingRect);
    ~QuadTree();
    bool addPoint(const Point &point);
    QList<QPoint> points;
    QuadTreeNode rootNode;
    QRectF boundingRect() const;
    QList<Point> query(const QRectF &boundingRect);
    int nodesCount() const;
    int maxLevel() const;
    void render(QPainter *painter, const QRect &rect, const QList<Point> &points = QList<Point>(), QRectF boundRect = QRectF());

private:
    bool addPoint(QuadTreeNode *startNode, const Point &point);
    void splitNode(QuadTreeNode *node);
    void query(QList<Point> &points, QuadTreeNode *node, const QRectF &boundingRect);
    void render(QPainter *painter, const QRect &rect, QuadTreeNode *node, const QRectF &boundRect);

private:
    int mNodesCount;
    int mMaxLevel;
};

#endif // QUADTREE_H
