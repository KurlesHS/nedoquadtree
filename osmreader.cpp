#include "osmreader.h"
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>
OsmReader::OsmReader(const QString &pathToOsmFile) :
    mPathToOsmFile(pathToOsmFile),
    mNodesCount(0)
{

}

OsmReader::~OsmReader()
{

}

QuadTree *OsmReader::makeTree()
{
    QFile file(mPathToOsmFile);
    if (!file.open(QIODevice::ReadOnly)) {
        return nullptr;
    }
    QuadTree *tree = nullptr;
    QXmlStreamReader xmlReader(&file);
    qreal minLat = 44444444444444.;
    qreal maxLat = -4444444444444.;
    qreal minLon = 44444444444444.;
    qreal maxLon = -4444444444444.;
    QList<Point> points;
    while (!xmlReader.atEnd()) {
        xmlReader.readNext();
        if (xmlReader.isStartElement() && xmlReader.name() == "node") {
           Point point;
           qreal lat, lon;
           lat = lon = 0.;
           for (const QXmlStreamAttribute &attr : xmlReader.attributes()) {
               if (attr.name() == "id") {
                   point.descr = attr.value().toString();
               } else if (attr.name() == "lat") {
                   lat = attr.value().toString().toDouble();
               } else if (attr.name() == "lon") {
                   lon = attr.value().toString().toDouble();
               }
           }
           ++mNodesCount;
           point.point = QPointF(lon, lat);
           points.append(point);
           minLat = qMin(minLat, lat);
           maxLat = qMax(maxLat, lat);
           minLon = qMin(minLon, lon);
           maxLon = qMax(maxLon, lon);
        }
    }
    tree = new QuadTree(QRectF(QPointF(minLon, minLat), QPointF(maxLon, maxLat)).normalized());
    for (const Point &p : points) {
        tree->addPoint(p);
    }
    return tree;
}

int OsmReader::nodesCount() const
{
    return mNodesCount;
}

