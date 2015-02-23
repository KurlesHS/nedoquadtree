#include "quadtree.h"
#include <QPainter>
#include <QDebug>

const int maxPoint = 32;

QuadTree::QuadTree(const QRectF &boundingRect)

{
    mRootNode = QuadTreeNode(boundingRect);
    mNodesCount = 1;
    mMaxLevel = 0;
}

QuadTree::~QuadTree()
{

}

bool QuadTree::addPoint(const Point &point)
{
    return mRootNode.addPoint(point);
}

void QuadTree::clearTree()
{
    mNodesCount = 1;
    mMaxLevel = 0;
    mRootNode.deleteChildNodes();
    mRootNode.removeAllPoints();
}

QRectF QuadTree::boundingRect() const
{
    return mRootNode.boundingRect();
}

QList<Point> QuadTree::query(const QRectF &boundingRect)
{
    QList<Point> p;
    mRootNode.query(p, boundingRect);
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
        boundRect = mRootNode.boundingRect();
    }
    painter->save();
    QTransform t;
    qreal scaleX = rect.width() / boundRect.normalized().width();
    qreal scaleY = rect.height() / boundRect.normalized().height();
    t.scale(scaleX, scaleY);
    t.translate(-boundRect.normalized().topLeft().x(), -boundRect.normalized().topLeft().y());
    painter->setTransform(t);
    render(painter, rect, &mRootNode, boundRect);
    for (const Point &p : points) {
        painter->drawPoint(p.point);
    }
    painter->restore();
}

void QuadTree::render(QPainter *painter, const QRect &rect, QuadTreeNode *node, const QRectF &boundRect)
{

    auto normBoundRect = node->boundingRect().normalized();
    if (boundRect.intersects(normBoundRect)) {
        painter->drawRect(normBoundRect);
        if (node->hasChildren()) {
            for (int i = 0; i < 4; ++ i) {
                render(painter, rect, node->childNode(i), boundRect);
            }
        }
        for (const Point &point : node->points()) {
            if (boundRect.contains(point.point)) {
                painter->drawPoint(point.point);
            }
        }
    }
}

QuadTreeNode::QuadTreeNode(const QRectF &boundingRect, QuadTreeNode *parent, const int level) :
    mBoundingRect(boundingRect),
    mParentNode(parent),
    mLevel(level)
{
    memset(mChildNodes, 0, sizeof(QuadTreeNode*) * 4);
}

QuadTreeNode::~QuadTreeNode()
{
    deleteChildNodes();
}

void QuadTreeNode::deleteChildNodes()
{
    for (int i = 0; i < 4; ++i) {
        if (mChildNodes[i]) {
            delete mChildNodes[i];
            mChildNodes[i] = nullptr;
        }
    }
}

void QuadTreeNode::removeAllPoints()
{
    mPoints.clear();
}
QRectF QuadTreeNode::boundingRect() const
{
    return mBoundingRect;
}

QuadTreeNode *QuadTreeNode::childNode(const int index)
{
    if (index >= 0 && index < 4) {
        return mChildNodes[index];
    }
    return nullptr;
}

void QuadTreeNode::splitNode()
{
    qreal halfWidth = boundingRect().width() / 2.;
    qreal halfHeight = boundingRect().height() / 2.;
    QSizeF halfSize = boundingRect().size() / 2.;
    mChildNodes[0] = new QuadTreeNode(QRectF(mBoundingRect.topLeft(), halfSize), this, level() + 1);
    mChildNodes[1] = new QuadTreeNode(QRectF(mBoundingRect.topLeft() + QPointF(halfWidth, 0), halfSize), this, level() + 1);
    mChildNodes[2] = new QuadTreeNode(QRectF(mBoundingRect.topLeft() + QPointF(0, halfHeight), halfSize), this, level() + 1);
    mChildNodes[3] = new QuadTreeNode(QRectF(mBoundingRect.topLeft() + QPointF(halfWidth, halfHeight), halfSize), this, level() + 1);

    for (const Point &point : mPoints) {
        for (int i = 0; i < 4; ++i) {
            auto childNode = mChildNodes[i];
            if (childNode->boundingRect().contains(point.point)) {
                childNode->addPoint(point);
                break;
            }
        }
    }
    mPoints.clear();
}

const QList<Point> &QuadTreeNode::points() const
{
    return mPoints;
}

bool QuadTreeNode::hasChildren() const
{
    return mChildNodes[0];
}

bool QuadTreeNode::addPoint(const Point &point)
{
    if (!boundingRect().contains(point.point)) {
        return false;
    }
    if (!hasChildren()) {
        if (mPoints.count() < maxPoint) {
            mPoints.append(point);
            return true;
        } else {
            splitNode();
            return addPoint(point);

        }
    } else {
        for (int i = 0; i < 4; ++ i) {
            if (mChildNodes[i]->boundingRect().contains(point.point)) {
                return mChildNodes[i]->addPoint(point);
            }
        }
    }
    return false;
}

void QuadTreeNode::query(QList<Point> &points, const QRectF &boundingRect)
{
    if (hasChildren()) {
        for (int i = 0; i < 4; ++i) {
            mChildNodes[i]->query(points, boundingRect);
        }
    } else {
        if (mBoundingRect.intersects(boundingRect)) {
            for (const Point &point : mPoints){
                if (boundingRect.contains(point.point)){
                    points.append(point);
                }
            }
        }
    }
}

int QuadTreeNode::level() const
{
    return mLevel;
}
