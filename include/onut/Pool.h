#pragma once
// STL
#include <cassert>
#include <cstdint>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Pool);

namespace onut
{
    class Pool final
    {
    public:
        enum class FailAction
        {
            AllocateOnHeap,
            ReturnNull,
            Assert
        };

        static OPoolRef create(size_t objSize = 256, size_t objCount = 256, FailAction failAction = FailAction::ReturnNull);

        Pool(size_t objSize = 256, size_t objCount = 256, FailAction failAction = FailAction::ReturnNull);
        ~Pool();

        template<typename Ttype, typename ... Targs>
        Ttype* alloc(Targs... args)
        {
            // Are there still room?
            if (m_allocCount == m_objCount)
            {
                switch (m_failAction)
                {
                    case FailAction::AllocateOnHeap:
                        return new Ttype(args...);
                    case FailAction::ReturnNull:
                        return nullptr;
                    case FailAction::Assert:
                        assert(false); // No more memory available in the pool. Use bigger pool
                        return nullptr;
                }
            }

            // Make sure we are not trying to allocate an object too big
            auto objSize = sizeof(Ttype);
            if (objSize > m_objSize)
            {
                switch (m_failAction)
                {
                    case FailAction::AllocateOnHeap:
                        return new Ttype(args...);
                    case FailAction::ReturnNull:
                        return nullptr;
                    case FailAction::Assert:
                        assert(false); // No more memory available in the pool. Use bigger pool
                        return nullptr;
                }
            }

            // Loop the pool from the last time.
            auto startPoint = m_currentObjIndex;
            do
            {
                ++m_currentObjIndex;
                // Wrap
                if (m_currentObjIndex >= m_objCount)
                {
                    m_currentObjIndex = 0;
                }
                auto pObj = m_pFirstObj + m_currentObjIndex * m_objTotalSize;
                auto used = pObj + m_objSize;

                if (!*used)
                {
                    // Found one!
                    *used = 1;
                    Ttype* pRet = new(pObj)Ttype(args...);
                    ++m_allocCount;
                    return pRet;
                }
            } while (startPoint != m_currentObjIndex);

            switch (m_failAction)
            {
                case FailAction::AllocateOnHeap:
                    return new Ttype(args...);
                case FailAction::ReturnNull:
                    return nullptr;
                case FailAction::Assert:
                    assert(false); // No more memory available in the pool. Use bigger pool
                    return nullptr;
            }

            return nullptr;
        }

        template<typename Ttype>
        bool dealloc(Ttype* pObj)
        {
            if (pObj == nullptr)
            {
                return false;
            }
            auto ptr = reinterpret_cast<uint8_t*>(pObj);
            if (ptr >= m_pMemory &&
                ptr < m_pMemory + m_memorySize)
            {
                pObj->~Ttype();
                auto used = ptr + m_objSize;
                if (!*used)
                {
                    return false;
                }
                *used = 0;
                --m_allocCount;
                return true;
            }
            delete pObj;
            return true;
        }

        void clear();
        size_t getAllocCount() const;
        void* getRawPointer() const;
        bool isUsed(void* pObject) const;
        size_t size() const;
        void* operator[](size_t index) const;

        template<typename Ttype>
        Ttype* at(size_t index) const
        {
            return reinterpret_cast<Ttype*>(m_pFirstObj + index * m_objTotalSize);
        }

    protected:
        size_t m_allocCount = 0;
        size_t m_objCount = 0;
        size_t m_currentObjIndex = 0;
        uint8_t* m_pFirstObj = nullptr;
        size_t m_objTotalSize = 0;
        size_t m_objSize = 0;
        FailAction m_failAction = FailAction::ReturnNull;
        uint8_t* m_pMemory = nullptr;
        size_t m_memorySize = 0;
    };
}
