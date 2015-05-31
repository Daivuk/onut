#include "NavMesh.h"
#include <vector>
#include <unordered_map>

struct sMarchingPoint
{
    float x, y;
    bool operator==(const sMarchingPoint &other) const
    {
        return
            x >= other.x - .1f &&
            x <= other.x + .1f &&
            y >= other.y - .1f &&
            y <= other.y + .1f;
    }
    bool operator!=(const sMarchingPoint &other) const
    {
        return !(
            x >= other.x - .1f &&
            x <= other.x + .1f &&
            y >= other.y - .1f &&
            y <= other.y + .1f);
    }
};

struct sMarchingLoop
{
    std::vector<sMarchingPoint> points;
};

std::vector<sMarchingLine> edges;
std::vector<sMarchingLoop> loops;

sMarchingLine *marching[16] = {
    nullptr, // 0000
    new sMarchingLine{.5f, 1.5f, 1.5f, .5f}, // 0001
    new sMarchingLine{.5f, .5f, 1.5f, 1.5f}, // 0010
    new sMarchingLine{.5f, 1.5f, 1.5f, 1.5f}, // 0011
    new sMarchingLine{1.5f, 1.5f, .5f, .5f}, // 0100
    new sMarchingLine{1.5f, 1.5f, 1.5f, .5f}, // 0101
    nullptr, // 0110
    nullptr, // 0111
    new sMarchingLine{1.5f, .5f, .5f, 1.5f}, // 1000
    nullptr, // 1001
    new sMarchingLine{.5f, .5f, .5f, 1.5f}, // 1010
    nullptr, // 1011
    new sMarchingLine{1.5f, .5f, .5f, .5f}, // 1100
    nullptr, // 1101
    nullptr, // 1110
    nullptr // 1111
};

namespace onut
{
    NavMesh::NavMesh(bool *pCollisionTiles, int width, int height)
    {
        // Create marching edges
        //std::vector<sMarchingLine> edges;
        for (int j = 0; j < height - 1; ++j)
        {
            for (int i = 0; i < width - 1; ++i)
            {
                int marchId = 
                    (pCollisionTiles[(j) * width + (i)] ? 0 : 1) |
                    (pCollisionTiles[(j) * width + (i + 1)] ? 0 : 1) << 1 | 
                    (pCollisionTiles[(j + 1) * width + (i)] ? 0 : 1) << 2 | 
                    (pCollisionTiles[(j + 1) * width + (i + 1)] ? 0 : 1) << 3;
                auto pM = marching[marchId];
                if (!pM) continue;
                auto m = *pM;
                m.x1 += (float)i;
                m.x2 += (float)i;
                m.y1 += (float)j;
                m.y2 += (float)j;
                edges.push_back(m);
            }
        }

        // Create loop objects from those edges
        for (auto &edge : edges)
        {
            bool found = false;
            for (auto &loop : loops)
            {
                for (auto &point : loop.points)
                {

                }
            }
        }
    }

    NavMesh::~NavMesh()
    {
        if (m_nodes)
        {
            for (auto i = 0; i < m_nodeCount; ++i)
            {
                auto &pNode = m_nodes[i];
                delete[] pNode.pNeighbors;
            }
            delete[] m_nodes;
        }
    }

    NavMesh::sNode *NavMesh::getNodes(int &count) const
    {
        count = m_nodeCount;
        return m_nodes;
    }
}
