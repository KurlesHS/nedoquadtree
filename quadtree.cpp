#include "quadtree.h"
#include <QPainter>
#include <QDebug>

const int maxPoint = 32;

QuadTree::QuadTree(const QRectF &boundingRect)

{
    rootNode.boundingRect = boundingRect;
    rootNode.parentNode = 0;
    rootNode.level = 0;
    mNodesCount = 1;
    mMaxLevel = 0;
}

QuadTree::~QuadTree()
{

}

bool QuadTree::addPoint(const Point &point)
{
    return addPoint(&rootNode, point);
}

QRectF QuadTree::boundingRect() const
{
    return rootNode.boundingRect;
}

QList<Point> QuadTree::query(const QRectF &boundingRect)
{
    QList<Point> p;
    query(p, &rootNode, boundingRect);
    return p;
}

int QuadTree::nodesCount() const
{
    return mNodesCount;
}

int QuadTree::maxLevel() const
{
    return mMaxLevel;
}

void QuadTree::render(QPainter *painter, const QRect &rect, const QList<Point> &points, QRectF boundRect)
{
    if (boundRect.isNull()) {
        boundRect = rootNode.boundingRect;
    }
    painter->save();
    QTransform t;
    qreal scaleX = rect.width() / boundRect.normalized().width();
    qreal scaleY = rect.height() / boundRect.normalized().height();
    t.scale(scaleX, scaleY);
    t.translate(-boundRect.normalized().topLeft().x(), -boundRect.normalized().topLeft().y());
    painter->setTransform(t);
    render(painter, rect, &rootNode, boundRect);
    for (const Point &p : points) {
        painter->drawPoint(p.point);
    }
    painter->restore();
}

void QuadTree::query(QList<Point> &points, QuadTreeNode *node, const QRectF &boundingRect)
{
    if (node->childNodes[0]) {
        for (int i = 0; i < 4; ++i) {
            query(points, node->childNodes[i], boundingRect);
        }
    } else {
        if (node->boundingRect.intersects(boundingRect)) {
            for (const Point &point : node->points){
                if (boundingRect.contains(point.point)){
                    points.append(point);
                }
            }
        }
    }
}

void QuadTree::render(QPainter *painter, const QRect &rect, QuadTreeNode *node, const QRectF &boundRect)
{

    auto normBoundRect = node->boundingRect.normalized();
    if (boundRect.intersects(normBoundRect)) {
        painter->drawRect(normBoundRect);
        if (node->childNodes[0]) {
            for (int i = 0; i < 4; ++ i) {
                render(painter, rect, node->childNodes[i], boundRect);
            }
        }
    }
}

bool QuadTree::addPoint(QuadTreeNode *startNode, const Point &point)
{
    if (!startNode->boundingRect.contains(point.point)) {
        return false;
    }
    if (!startNode->childNodes[0]) {
        if (startNode->points.count() < maxPoint) {
            startNode->points.append(point);
            return true;
        } else {
            splitNode(startNode);
            return addPoint(startNode, point);
        }
    } else {
        for (int i = 0; i < 4; ++ i) {
            if (startNode->childNodes[i]->boundingRect.contains(point.point)) {
                return addPoint(startNode->childNodes[i], point);
            }
        }
    }
    return false;
}

int QuadTreeNode::splitCount = 0;

void QuadTree::splitNode(QuadTreeNode *node)
{
    mMaxLevel = qMax(mMaxLevel, node->level + 1);
    ++(node->splitCount);
    mNodesCount += 4;
    qreal halfWidth = node->boundingRect.width() / 2.;
    qreal halfHeight = node->boundingRect.height() / 2.;
    QSizeF halfSize = node->boundingRect.size() / 2.;
    node->childNodes[0] = new QuadTreeNode(QRectF(node->boundingRect.topLeft(), halfSize), node, node->level + 1);
    node->childNodes[1] = new QuadTreeNode(QRectF(node->boundingRect.topLeft() + QPointF(halfWidth, 0), halfSize), node, node->level + 1);
    node->childNodes[2] = new QuadTreeNode(QRectF(node->boundingRect.topLeft() + QPointF(0, halfHeight), halfSize), node, node->level + 1);
    node->childNodes[3] = new QuadTreeNode(QRectF(node->boundingRect.topLeft() + QPointF(halfWidth, halfHeight), halfSize), node, node->level + 1);
    for (const Point &point : node->points) {
        for (int i = 0; i < 4; ++i) {
            auto childNode = node->childNodes[i];
            if (childNode->boundingRect.contains(point.point)) {
                childNode->points.append(point);
                break;
            }
        }
    }
    node->points.clear();
}

QuadTreeNode::QuadTreeNode(const QRectF &boundingRect, QuadTreeNode *parent, const int level) :
    boundingRect(boundingRect),
    parentNode(parent),
    level(level)
{
    memset(childNodes, 0, sizeof(QuadTreeNode*) * 4);
}
