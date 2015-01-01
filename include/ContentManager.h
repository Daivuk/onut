#pragma once
#include <unordered_map>
#include <mutex>
#include <string>
#include "StringUtils.h"

namespace onut
{
    class Texture;
    class BMFont;

    template<bool TuseAssert = true,
        typename TmutexType = std::mutex>
    class ContentManager
    {
    public:
        ContentManager()
        {
            addSearchPath("../../assets");
            addSearchPath("../../assets/fonts");
            addSearchPath("../../assets/pfx");
            addSearchPath("../../assets/shaders");
            addSearchPath("../../assets/sounds");
            addSearchPath("../../assets/textures");
            addSearchPath("../../assets/musics");
        }

        virtual ~ContentManager()
        {
            clear();
        }

        /**
        Get a resource by name
        */
        template <typename Ttype>
        Ttype* getResource(const std::string& name)
        {
            if (m_threadId == std::this_thread::get_id())
            {
                auto it = m_resources.find(name);
                if (it == m_resources.end())
                {
                    // Load it
                    return load<Ttype>(name);
                }
                auto pResourceHolder = dynamic_cast<ResourceHolder<Ttype>*>(it->second);
                if (!pResourceHolder)
                {
                    if (TuseAssert)
                    {
                        assert(false); // Resource not found
                    }
                    return nullptr;
                }
                return pResourceHolder->getResource();
            }
            else
            {
                m_mutex.lock();
                auto it = m_resources.find(name);
                if (it == m_resources.end())
                {
                    m_mutex.unlock();
                    // Load it
                    return load<Ttype>(name);
                }
                auto pResourceHolder = dynamic_cast<ResourceHolder<Ttype>*>(it->second);
                m_mutex.unlock();
                if (!pResourceHolder)
                {
                    if (TuseAssert)
                    {
                        assert(false); // Resource not found
                    }
                    return nullptr;
                }
                return pResourceHolder->getResource();
            }
        }

        /**
        Delete all content loaded by this ContentManager
        */
        void clear()
        {
            for (auto& kv : m_resources)
            {
                delete kv.second;
            }
            m_resources.clear();
        }

        /**
        * Add a search path to this content manager. Considering the executable is placed under bin/platform/, those are the default added:
        * - ../../assets
        * - ../../assets/fonts
        * - ../../assets/pfx
        * - ../../assets/shaders
        * - ../../assets/sounds
        * - ../../assets/textures
        * - ../../assets/musics
        * You have to add subfolders, they won't be searched.
        */
        void addSearchPath(const std::string& path)
        {
            m_searchPaths.push_back(path);
        }

    private:
        class IResourceHolder
        {
        public:
            virtual ~IResourceHolder() {}
        };

        std::unordered_map<std::string, IResourceHolder*> m_resources;

    public:
        /**
        Get the count of all loaded assets
        */
        auto size() -> decltype(m_resources.size()) const
        {
            return m_resources.size();
        }

    private:
        template<typename Ttype>
        class ResourceHolder : public IResourceHolder
        {
        public:
            ResourceHolder(Ttype* pResource) : m_pResource(pResource) {}
            virtual ~ResourceHolder()
            {
                if (m_pResource)
                {
                    delete m_pResource;
                    m_pResource = nullptr;
                }
            }

            Ttype* getResource() const { return m_pResource; }

        private:
            Ttype* m_pResource = nullptr;
        };

        template<typename Ttype>
        Ttype* load(const std::string& name)
        {
            // Find the file
            std::string filename;
            for (auto& path : m_searchPaths)
            {
                filename = findFile<TuseAssert>(name, path, false);
                if (!filename.empty())
                {
                    break;
                }
            }
            if (filename == "") return nullptr;

            // Create it
            Ttype* pResource = Ttype::createFromFile(filename);

            // Put it in our map
            ResourceHolder<Ttype>* pResourceHolder = new ResourceHolder<Ttype>(pResource);
            m_mutex.lock();
            m_resources[name] = pResourceHolder;
            m_mutex.unlock();

            return pResource;
        }

        TmutexType                              m_mutex;
        decltype(std::this_thread::get_id())    m_threadId = std::this_thread::get_id();
        std::vector<std::string>                m_searchPaths;
    };
}
