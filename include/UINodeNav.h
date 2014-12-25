#pragma once
#include <chrono>
#include <vector>

namespace onut
{
    template<typename TmathType>
    class UINodeNavAccessor
    {
    public:
        static inline auto getX(const TmathType& node) -> decltype(node.x)
        {
            return node.x;
        }

        static inline auto getY(const TmathType& node) -> decltype(node.y)
        {
            return node.y;
        }
    };

    /**
    Helper class to navigate nodes using the controller.
    Image nodes layed out in 2D. This will pick the best choice.
    It will also repeat the current direction, but will be careful to ignore
    the repeat if the direction is oposite
    */
    template<typename TnodeType,
        typename TnodeAccessor = UINodeNavAccessor < TnodeType >>
    class UINodeNav
    {
    private:
        typedef decltype(TnodeAccessor::getX(TnodeType()))  Tprecision;
        typedef decltype(std::vector<TnodeType>().size())   TindexType;
        typedef decltype(std::chrono::milliseconds(1))      TdurationType;
        typedef decltype(std::chrono::steady_clock::now())  TtimePoint;

        std::vector<TnodeType>      m_nodes;
        Tprecision                  m_deadZone;
        Tprecision                  m_travelAngle;
        TdurationType               m_repeatTime;
        TtimePoint                  m_repeatStart;
        Tprecision                  m_lastDirX = static_cast<Tprecision>(0);
        Tprecision                  m_lastDirY = static_cast<Tprecision>(0);
        TindexType                  m_selectedIndex;

    public:
        /**
        Constructor
        @param nodes Vector of nodes
        @param deadZone Specify the dead zone for the movement. Default is 0.3
        @param travelAngle Specify the acceptable radius angle to go to another node. Default is 45 deg
        @param repeatTime Time to wait before repeating if the dir is held down.
        */
        UINodeNav(const std::vector<TnodeType>& nodes = {},
                  Tprecision deadZone = static_cast<Tprecision>(0.3),
                  TdurationType repeatTime = std::chrono::milliseconds(200),
                  Tprecision travelAngle = static_cast<Tprecision>(0.70710678118654752440084436210485)) :
                  m_nodes(nodes),
                  m_deadZone(deadZone),
                  m_travelAngle(travelAngle),
                  m_repeatTime(repeatTime),
                  m_selectedIndex(0)
        {}

        /**
        Set the nodes.
        Previous nodes will be overriden.
        @param nodes Vector of nodes
        */
        void setNodes(const std::vector<TnodeType>& nodes)
        {
            clear();
            m_nodes = nodes;
        }

        /**
        Add a new node
        @param node New node to be added
        */
        void addNode(const TnodeType& node)
        {
            m_nodes.push_back(node);
        }

        /**
        Get the selected node's index
        @return Index of the selected node
        */
        TindexType getSelectedIndex() const
        {
            return m_selectedIndex;
        }

        /**
        Get the selected node
        @return the selected node
        */
        const TnodeType& getSelected() const
        {
            return m_nodes[m_selectedIndex];
        }

        /**
        Set the selected node
        @param index Index of the node to selec
        */
        void setSelected(TindexType index)
        {
            m_selectedIndex = index;
        }

        /**
        Set the selected node
        @param node Value of the node to select
        @note If the node is not found, selection will not be changed
        */
        void setSelected(const TnodeType& node)
        {
            for (TindexType i = 0; i < m_nodes.size(); ++i)
            {
                if (node == m_nodes[i])
                {
                    m_selectedIndex = i;
                    return;
                }
            }
        }

        /**
        Remove a node at specified index
        @param index Index of the node to remove.
        */
        void removeNode(TindexType index)
        {
            if (index == m_selectedIndex)
            {
                // Pick the next closest one to this node
                Tprecision posX = TnodeAccessor::getX(m_nodes[m_selectedIndex]);
                Tprecision posY = TnodeAccessor::getY(m_nodes[m_selectedIndex]);
                Tprecision closestDis = static_cast<Tprecision>(1000000.0);
                TindexType i = 0;
                for (auto& node : m_nodes)
                {
                    if (m_selectedIndex != i)
                    {
                        Tprecision otherPosX = TnodeAccessor::getX(m_nodes[i]);
                        Tprecision otherPosY = TnodeAccessor::getY(m_nodes[i]);
                        auto disSq = (otherPosX - posX) * (otherPosX - posX) + (otherPosY - posY) * (otherPosY - posY);
                        if (disSq < closestDis)
                        {
                            m_selectedIndex = i;
                            closestDis = disSq;
                        }
                    }
                    ++i;
                }
            }
            if (m_selectedIndex > index)
            {
                --m_selectedIndex;
            }
            m_nodes.erase(m_nodes.begin() + index);
        }

        /**
        Remove a node by value
        @param node Node to be compared against. Only first found one will be removed.
        */
        void removeNode(const TnodeType& node)
        {
            auto begin = m_nodes.begin();
            auto end = m_nodes.end();
            TindexType i = 0;
            for (auto it = begin; it != end; ++it, ++i)
            {
                if (*it == node)
                {
                    removeNode(i);
                    return;
                }
            }
        }

        /**
        Clear all the nodes
        */
        void clear()
        {
            m_nodes.clear();
            m_selectedIndex = 0;
        }

        /**
        Get how many nodes are in the system
        @return Node count
        */
        TindexType size()
        {
            return m_nodes.size();
        }

        /**
        Navigate. Call this frequently to know which nodes to travel to next.
        @param dir Direction of the control. This can be your controller thumbstick axis. Ranged from -1 to 1 in both X and Y axis
        @param currentIndex Index of the currently selected node.
        @return The index of the newly selected node. Or the same as previous.
        */
        template<typename TaxisType,
            typename TaxisAccessor = UINodeNavAccessor < TaxisType >>
            bool navigate(const TaxisType& dir)
        {
            auto previousSelection = m_selectedIndex;
            auto lenSq = (TaxisAccessor::getX(dir) * TaxisAccessor::getX(dir) + TaxisAccessor::getY(dir) * TaxisAccessor::getY(dir));
            if (lenSq >= m_deadZone * m_deadZone)
            {
                auto len = sqrt(lenSq);
                auto dirNormX = TaxisAccessor::getX(dir) / len;
                auto dirNormY = TaxisAccessor::getY(dir) / len;
                auto now = std::chrono::steady_clock::now();
                auto timeSinceRepeat = now - m_repeatStart;
                auto dotWithLastDir = m_lastDirX * static_cast<Tprecision>(dirNormX)+
                    m_lastDirY * static_cast<Tprecision>(dirNormY);
                if (timeSinceRepeat >= m_repeatTime || dotWithLastDir <= m_travelAngle)
                {
                    m_repeatStart = now;
                    auto bestChoice = m_selectedIndex;
                    auto startPosX = TnodeAccessor::getX(m_nodes[m_selectedIndex]);
                    auto startPosY = TnodeAccessor::getY(m_nodes[m_selectedIndex]);
                    auto bestDot = static_cast<Tprecision>(1000000.0);
                    for (TindexType i = 0; i < m_nodes.size(); ++i)
                    {
                        if (i != m_selectedIndex)
                        {
                            auto iPosX = TnodeAccessor::getX(m_nodes[i]);
                            auto iPosY = TnodeAccessor::getY(m_nodes[i]);
                            auto dirToNodeX = iPosX - startPosX;
                            auto dirToNodeY = iPosY - startPosY;
                            auto dirToNodeLenSq = dirToNodeX * dirToNodeX + dirToNodeY * dirToNodeY;
                            auto dirToNodeLen = sqrt(dirToNodeLenSq);
                            auto dirToNodeNormX = dirToNodeX / dirToNodeLen;
                            auto dirToNodeNormY = dirToNodeY / dirToNodeLen;
                            auto dot = dirNormX * dirToNodeNormX + dirNormY * dirToNodeNormY;
                            if (dot > m_travelAngle)
                            {
                                auto fullDot = dirNormX * dirToNodeX + dirNormY * dirToNodeY;
                                if (fullDot < bestDot)
                                {
                                    bestDot = fullDot;
                                    bestChoice = i;
                                }
                            }
                        }
                    }
                    if (bestChoice != m_selectedIndex)
                    {
                        m_selectedIndex = bestChoice;
                        m_lastDirX = dirNormX;
                        m_lastDirY = dirNormY;
                    }
                }
            }
            return m_selectedIndex != previousSelection;
        }
    };
}
