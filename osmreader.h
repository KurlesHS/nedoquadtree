#ifndef OSMREADER_H
#define OSMREADER_H

#include <QString>
#include "quadtree.h"

class OsmReader
{
public:
    OsmReader(const QString &pathToOsmFile);
    ~OsmReader();
    QuadTree *makeTree();
    int nodesCount() const;

private:
    QString mPathToOsmFile;
    int mNodesCount;


};

#endif // OSMREADER_H
