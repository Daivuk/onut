// Onut
#include <onut/Pool.h>

namespace onut
{
    OPoolRef Pool::create(size_t objSize, size_t objCount, FailAction failAction)
    {
        return std::make_shared<OPool>(objSize, objCount, failAction);
    }

    Pool::Pool(size_t objSize, size_t objCount, FailAction failAction)
        : m_objCount(objCount)
        , m_objSize(objSize)
        , m_failAction(failAction)
    {
        static const size_t alignment = sizeof(uintptr_t);
        static const size_t headerSize = 1;

        m_objTotalSize = ((m_objSize + headerSize) % alignment) ? (m_objSize + headerSize) + (alignment - ((m_objSize + headerSize) % alignment)) : (m_objSize + headerSize);
        m_memorySize = m_objTotalSize * m_objCount + alignment;

        // Allocate memory
        m_pMemory = new uint8_t[m_memorySize];
        memset(m_pMemory, 0, m_memorySize);

        // Align
        auto mod = reinterpret_cast<uintptr_t>(m_pMemory) % alignment;
        if (mod)
        {
            m_pFirstObj = m_pMemory + (alignment - mod);
        }
        else
        {
            m_pFirstObj = m_pMemory;
        }

        m_currentObjIndex = 0;
    }

    Pool::~Pool()
    {
        delete[] m_pMemory;
    }

    void Pool::clear()
    {
        memset(m_pMemory, 0, m_memorySize);
        m_currentObjIndex = 0;
        m_allocCount = 0;
    }

    size_t Pool::getAllocCount() const
    {
        return m_allocCount; 
    }

    void* Pool::getRawPointer() const 
    {
        return m_pFirstObj;
    }

    size_t Pool::size() const
    {
        return m_objCount;
    }

    bool Pool::isUsed(void* pObject) const
    {
        auto ptr = static_cast<uint8_t*>(pObject);
        auto used = ptr + m_objSize;
        return (*used) ? true : false;
    }

    void* Pool::operator[](size_t index) const
    {
        return m_pFirstObj + index * m_objTotalSize;
    }
}
