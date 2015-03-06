#include "qtreeimplmainwindow.h"
#include <QApplication>
#include <QDebug>
#include <quadtree.h>
#include <QTime>
#include <QPixmap>
#include <QPainter>
#include <rtree.h>
#include "osmreader.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTreeImplMainWindow w;
    w.show();
#if 0
    static qreal minReal = std::numeric_limits<qreal>::min();
    QSizeF realSize (1.121, 1.121);
    QRectF realRect(QPointF(0, 0), realSize/* + QSize(minReal, minReal)*/);


    QRect testR(QPoint(1, 1), QSize(1,1));
    qDebug() << "test contains =" << realRect.contains(QPointF(realSize.width(), realSize.height()));

    testR.setTop(0);
    qDebug() << testR;

    QRectF boundingRect(QPointF(68.125776,26.475072), QPointF(55.597647,64.113379));
    qDebug() << boundingRect;
    qsrand(QDateTime::currentMSecsSinceEpoch());
    QuadTree tree(boundingRect);

    QList<QRectF> rects;
    for (int i = 0; i < 3; ++i) {
        for (int a = 0; a < 3; ++ a) {
            if (i != 1|| a != 1) {
                rects.append(QRectF(QPointF(a, i), QSizeF(1, 1)));
            }
        }
    }

    int count = 0;
    Point po;
    po.point = QPointF(1.7, 1.7);
    for (const QRectF &r : rects) {
        ++count;
        qDebug() << count << r << RTreeNode::distTo(po, r);
    }


    int falseCount = 0;
    for (int i = 0; i < 5000; i++) {
        qreal factorX = qrand() / (qreal)RAND_MAX;
        qreal factorY = qrand() / (qreal)RAND_MAX;
        qreal deltaX = boundingRect.width() * factorX;
        qreal deltaY = boundingRect.height() * factorY;
        Point p;
        p.descr = QString::number(i + 1);
        p.point = boundingRect.topLeft() + QPointF(deltaX, deltaY);
        if (!tree.addPoint(p)) {
            falseCount++;
        }
    }
    QList<Point> points = tree.query(boundingRect);
    qDebug() << falseCount << points.count() << tree.nodesCount() << tree.maxLevel();
    QPixmap pix(800,600);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    auto rec = pix.rect();
    rec.setSize(pix.size() - QSize(1, 1));
    QTime t;
    t.start();
    tree.render(&p, rec);
    qDebug() << t.elapsed();


    OsmReader r("d:/develop/osmmap/cherep2.osm");
    auto tree2 = r.makeQuadTree();
    auto rtree = r.makeRTree();
    qDebug() << "rtree bounding rect =" << rtree->boundingRect() << rtree->boundingRect().topLeft().x() << rtree->boundingRect().topLeft().y();
    //auto points_2 = tree2->query(tree2->boundingRect());
    // QRectF queryRect(QPointF(59.132423,37.908972), QPointF(59.116719,37.967203));
    QRectF queryRect2(QPointF(37.908972, 59.132423), QPointF(37.967203, 59.116719));
    queryRect2 = queryRect2.normalized();
    //queryRect2.setSize(queryRect2.size() / 2);
    //queryRect2.setTopLeft(queryRect2.topLeft() + QPointF(queryRect2.size().width(), queryRect2.size().height()));
    QPointF tl = queryRect2.topLeft();
    QSizeF s = queryRect2.size() / 8;

    //queryRect2 = QRectF(tl, s);

    t.restart();

    QList<Point> points_2;

    points_2 = tree2->query(queryRect2);

    qDebug() << "queryTime" << t.elapsed()  << points_2.size();
    //59.132423,37.908972
    //59.116719,37.967203
    pix.fill(Qt::transparent);
    //tree2->render(&p, rec, points_2, queryRect2);
    RTree rtree2;
    for (int i = 0; i < 300; i++) {
        qreal factorX = qrand() / (qreal)RAND_MAX;
        qreal factorY = qrand() / (qreal)RAND_MAX;
        qreal deltaX = boundingRect.width() * factorX;
        qreal deltaY = boundingRect.height() * factorY;
        Point p;
        p.descr = QString::number(i + 1);
        p.point = boundingRect.topLeft() + QPointF(deltaX, deltaY);
        rtree2.addPoint(p);
    }

    //rtree->render(&p, rec, queryRect2);
    rtree2.render(&p, rec, rtree2.boundingRect());
    qDebug() << r.nodesCount() << points_2.count() << tree2->maxLevel();
    t.restart();

    points_2 = rtree->query(queryRect2);
    qDebug() << "queryTime 2" << t.elapsed() << points_2.size();

    w.setPixmap(pix);
#endif
    return a.exec();
}
