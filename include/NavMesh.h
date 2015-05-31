#pragma once

struct sMarchingLine
{
    float x1, y1;
    float x2, y2;
};

namespace onut
{
    class NavMesh
    {
    public:
        struct sNode;

        struct sNeighbor
        {
            float heuristic;
            sNode *pNode;
        };

        struct sNode
        {
            sNeighbor *pNeighbors;
            int neighborCount;
            float x, y, z;
        };

        NavMesh(bool *pCollisionTiles, int width, int height);
        virtual ~NavMesh();

        sNode *getNodes(int &count) const;

    private:
        sNode *m_nodes = nullptr;
        int m_nodeCount = 0;
    };
};
