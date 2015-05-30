#pragma once
#include <cassert>
#include <cstdint>

namespace onut
{
    /**
    Pool of abitrary memory. Default allocating ~64k of memory
    template arguments:
    - TobjSize: Size of each objects. Default 256
    - TobjCount: Count of maximum allowed object. Default 256
    - Talignement: Alignement. This is required on some platforms, like iOS. Default sizeof(uintptr_t)
    - TuseAssert: If true, asserts will be used if out of memory or double deletion. Otherwise it will just return nullptr
    - TobjHeaderSize: Default is 1 (Used or unused)
    - TobjTotalSize: Default is TobjSize + TheaderSize size, aligned for Talignement.
    - TmemorySize: Total required memory. Default is TobjTotalSize * TobjCount + Talignment. It accounts for alignment.
    */
    template<uintptr_t TobjSize = 256,
             uintptr_t TobjCount = 256,
             uintptr_t Talignment = sizeof(uintptr_t),
             bool TuseAsserts = true,
             uintptr_t TheaderSize = 1,
             uintptr_t TobjTotalSize = ((TobjSize + TheaderSize) % Talignment) ? (TobjSize + TheaderSize) + (Talignment - ((TobjSize + TheaderSize) % Talignment)) : (TobjSize + TheaderSize),
             uintptr_t TmemorySize = TobjTotalSize * TobjCount + Talignment>
    class StaticPool
    {
    public:
        /**
        Constructor. Will allocate the memory
        */
        StaticPool()
        {
            // Allocate memory
            m_pMemory = new uint8_t[TmemorySize];
            memset(m_pMemory, 0, TmemorySize);

            // Align
            auto mod = reinterpret_cast<uintptr_t>(m_pMemory) % Talignment;
            if (mod)
            {
                m_pFirstObj = m_pMemory + (Talignment - mod);
            }
            else
            {
                m_pFirstObj = m_pMemory;
            }

            m_currentObjIndex = 0;
        }

        /**
        Destructor. Will free the memory
        */
        virtual ~StaticPool()
        {
            delete[] m_pMemory;
        }

        /**
        Allocate an object.
        @param args Parameter list for your constructor
        */
        template<typename Ttype,
            typename ... Targs>
            Ttype* alloc(Targs... args)
        {
            // Are there still room?
            if (m_allocCount == TobjCount)
            {
                if (TuseAsserts)
                {
                    assert(false); // No more memory available in the pool. Use bigger pool
                }
                return nullptr;
            }

            // Make sure we are not trying to allocate an object too big
            auto objSize = sizeof(Ttype);
            if (objSize > TobjSize)
            {
                if (TuseAsserts)
                {
                    assert(false); // Trying to allocate an object too big
                }
                return nullptr;
            }

            // Loop the pool from the last time.
            auto startPoint = m_currentObjIndex;
            do
            {
                ++m_currentObjIndex;
                // Wrap
                if (m_currentObjIndex >= TobjCount)
                {
                    m_currentObjIndex = 0;
                }
                auto pObj = m_pFirstObj + m_currentObjIndex * TobjTotalSize;
                auto used = pObj + TobjSize;

                if (!*used)
                {
                    // Found one!
                    *used = 1;
                    Ttype* pRet = new(pObj)Ttype(args...);
                    ++m_allocCount;
                    return pRet;
                }
            } while (startPoint != m_currentObjIndex);

            if (TuseAsserts)
            {
                assert(false); // No more memory available in the pool. Use bigger pool
            }
            return nullptr;
        }

        /**
        Dealloc an object
        @param pObj Pointer to the object to free
        @return True if dealloced
        */
        template<typename Ttype>
        bool dealloc(Ttype* pObj)
        {
            if (pObj == nullptr)
            {
                if (TuseAsserts)
                {
                    assert(false); // Can not dealloc nullptr
                }
                return false;
            }
            pObj->~Ttype();
            auto ptr = reinterpret_cast<uint8_t*>(pObj);
            auto used = ptr + TobjSize;
            if (!*used)
            {
                if (TuseAsserts)
                {
                    assert(false); // Memory was already deallocated. Double deletion!
                }
                return false;
            }
            *used = 0;
            --m_allocCount;
            return true;
        }

        /**
        Dealloc everything.
        Resets the pool, kills all objects initialized with it. Destructors won't be called!
        */
        void clear()
        {
            memset(m_pMemory, 0, TmemorySize);
            m_currentObjIndex = 0;
            m_allocCount = 0;
        }

        /**
        Get current allocation count
        @return Number of allocated objects
        */
        uintptr_t getAllocCount() const { return m_allocCount; }

        /**
        Get raw pointer to the first object
        @return Pointer to the first object in the array. Used or not
        */
        void* getRawPointer() const { return m_pFirstObj; }

        /**
        Check wether an object is used or not. By passing the object raw pointer
        @param pObject Raw pointer to the object
        @return Wether the object is used or not in the pool
        */
        bool isUsed(void* pObject) const
        {
            auto ptr = static_cast<uint8_t*>(pObject);
            auto used = ptr + TobjSize;
            return (*used) ? true : false;
        }

        /**
        Get the maximum number of allowed objects in the pool
        @return size of the pool.
        */
        uintptr_t size() const { return TobjCount; }

        /**
        Get raw pointer on the object at index
        @return raw pointer at index
        */
        void* operator[](uintptr_t index) const
        {
            return m_pFirstObj + index * TobjTotalSize;
        }

        /**
        Get object pointer at index
        @return pointer to object at index
        */
        template<typename Ttype>
        Ttype* at(uintptr_t index) const
        {
            return reinterpret_cast<Ttype*>(m_pFirstObj + index * TobjTotalSize);
        }

    protected:
        uint8_t*    m_pMemory = nullptr;
        uint8_t*    m_pFirstObj = nullptr;
        uintptr_t   m_currentObjIndex = 0;
        uintptr_t   m_allocCount = 0;
    };

    /**
    Pool of abitrary memory. Default allocating ~64k of memory
    template arguments:
    - TobjSize: Size of each objects. Default 256
    - TobjCount: Count of maximum allowed object. Default 256
    - Talignement: Alignement. This is required on some platforms, like iOS. Default sizeof(uintptr_t)
    - TuseAssert: If true, asserts will be used if out of memory or double deletion. Otherwise it will just return nullptr
    - TobjHeaderSize: Default is 1 (Used or unused)
    - TobjTotalSize: Default is TobjSize + TheaderSize size, aligned for Talignement.
    - TmemorySize: Total required memory. Default is TobjTotalSize * TobjCount + Talignment. It accounts for alignment.
    */
    template<bool TuseAsserts = true>
    class Pool
    {
    public:
        /**
        Constructor. Will allocate the memory
        */
        Pool(uintptr_t objSize = 256, uintptr_t objCount = 256, uintptr_t alignment = sizeof(uintptr_t), uintptr_t headerSize = 1)
            : TobjSize(objSize)
            , TobjCount(objCount)
            , Talignment(alignment)
            , TheaderSize(headerSize)
        {
            TobjTotalSize = ((objSize + headerSize) % alignment) ? (objSize + headerSize) + (alignment - ((objSize + headerSize) % alignment)) : (objSize + headerSize);
            TmemorySize = TobjTotalSize * objCount + alignment;

            // Allocate memory
            m_pMemory = new uint8_t[TmemorySize];
            memset(m_pMemory, 0, TmemorySize);

            // Align
            auto mod = reinterpret_cast<uintptr_t>(m_pMemory) % Talignment;
            if (mod)
            {
                m_pFirstObj = m_pMemory + (Talignment - mod);
            }
            else
            {
                m_pFirstObj = m_pMemory;
            }

            m_currentObjIndex = 0;
        }

        /**
        Destructor. Will free the memory
        */
        virtual ~Pool()
        {
            delete[] m_pMemory;
        }

        /**
        Allocate an object.
        @param args Parameter list for your constructor
        */
        template<typename Ttype,
            typename ... Targs>
            Ttype* alloc(Targs... args)
        {
            // Are there still room?
            if (m_allocCount == TobjCount)
            {
                if (TuseAsserts)
                {
                    assert(false); // No more memory available in the pool. Use bigger pool
                }
                return nullptr;
            }

            // Make sure we are not trying to allocate an object too big
            auto objSize = sizeof(Ttype);
            if (objSize > TobjSize)
            {
                if (TuseAsserts)
                {
                    assert(false); // Trying to allocate an object too big
                }
                return nullptr;
            }

            // Loop the pool from the last time.
            auto startPoint = m_currentObjIndex;
            do
            {
                ++m_currentObjIndex;
                // Wrap
                if (m_currentObjIndex >= TobjCount)
                {
                    m_currentObjIndex = 0;
                }
                auto pObj = m_pFirstObj + m_currentObjIndex * TobjTotalSize;
                auto used = pObj + TobjSize;

                if (!*used)
                {
                    // Found one!
                    *used = 1;
                    Ttype* pRet = new(pObj)Ttype(args...);
                    ++m_allocCount;
                    return pRet;
                }
            } while (startPoint != m_currentObjIndex);

            if (TuseAsserts)
            {
                assert(false); // No more memory available in the pool. Use bigger pool
            }
            return nullptr;
        }

        /**
        Dealloc an object
        @param pObj Pointer to the object to free
        @return True if dealloced
        */
        template<typename Ttype>
        bool dealloc(Ttype* pObj)
        {
            if (pObj == nullptr)
            {
                if (TuseAsserts)
                {
                    assert(false); // Can not dealloc nullptr
                }
                return false;
            }
            pObj->~Ttype();
            auto ptr = reinterpret_cast<uint8_t*>(pObj);
            auto used = ptr + TobjSize;
            if (!*used)
            {
                if (TuseAsserts)
                {
                    assert(false); // Memory was already deallocated. Double deletion!
                }
                return false;
            }
            *used = 0;
            --m_allocCount;
            return true;
        }

        /**
        Dealloc everything.
        Resets the pool, kills all objects initialized with it. Destructors won't be called!
        */
        void clear()
        {
            memset(m_pMemory, 0, TmemorySize);
            m_currentObjIndex = 0;
            m_allocCount = 0;
        }

        /**
        Get current allocation count
        @return Number of allocated objects
        */
        uintptr_t getAllocCount() const { return m_allocCount; }

        /**
        Get raw pointer to the first object
        @return Pointer to the first object in the array. Used or not
        */
        void* getRawPointer() const { return m_pFirstObj; }

        /**
        Check wether an object is used or not. By passing the object raw pointer
        @param pObject Raw pointer to the object
        @return Wether the object is used or not in the pool
        */
        bool isUsed(void* pObject) const
        {
            auto ptr = static_cast<uint8_t*>(pObject);
            auto used = ptr + TobjSize;
            return (*used) ? true : false;
        }

        /**
        Get the maximum number of allowed objects in the pool
        @return size of the pool.
        */
        uintptr_t size() const { return TobjCount; }

        /**
        Get raw pointer on the object at index
        @return raw pointer at index
        */
        void* operator[](uintptr_t index) const
        {
            return m_pFirstObj + index * TobjTotalSize;
        }

        /**
        Get object pointer at index
        @return pointer to object at index
        */
        template<typename Ttype>
        Ttype* at(uintptr_t index) const
        {
            return reinterpret_cast<Ttype*>(m_pFirstObj + index * TobjTotalSize);
        }

    protected:
        uint8_t*    m_pMemory = nullptr;
        uint8_t*    m_pFirstObj = nullptr;
        uintptr_t   m_currentObjIndex = 0;
        uintptr_t   m_allocCount = 0;

        uintptr_t   TobjSize;
        uintptr_t   TobjCount;
        uintptr_t   Talignment;
        uintptr_t   TheaderSize;
        uintptr_t   TobjTotalSize;
        uintptr_t   TmemorySize;
    };
}

typedef onut::Pool<false> OPool;
