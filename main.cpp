#include "qtreeimplmainwindow.h"
#include <QApplication>
#include <QDebug>
#include <quadtree.h>
#include <QTime>
#include <QPixmap>
#include <QPainter>
#include "osmreader.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTreeImplMainWindow w;
    w.show();

    QRectF boundingRect(QPointF(68.125776,26.475072), QPointF(55.597647,64.113379));
    qDebug() << boundingRect;
    qsrand(QDateTime::currentMSecsSinceEpoch());
    QuadTree tree(boundingRect);
    int falseCount = 0;
    for (int i = 0; i < 500000; i++) {
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
    qDebug() << falseCount << QuadTreeNode::splitCount << points.count() << tree.nodesCount() << tree.maxLevel();
    QPixmap pix(800,600);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    auto rec = pix.rect();
    rec.setSize(pix.size() - QSize(1, 1));
    QTime t;
    t.start();
    //tree.render(&p, rec, points);
    qDebug() << t.elapsed();


    OsmReader r("d:/develop/osmmap/cherep2.osm");
    auto tree2 = r.makeTree();
    //auto points_2 = tree2->query(tree2->boundingRect());
    QRectF queryRect(QPointF(59.132423,37.908972), QPointF(59.116719,37.967203));
    t.restart();

    QList<Point> points_2;

    points_2 = tree2->query(queryRect);

    qDebug() << "queryTime" << t.elapsed();
    //59.132423,37.908972
    //59.116719,37.967203
    tree2->render(&p, rec, points_2, queryRect);
    qDebug() << r.nodesCount() << points_2.count() << tree2->maxLevel();



    w.setPixmap(pix);
    points_2.clear();
    return a.exec();
}
