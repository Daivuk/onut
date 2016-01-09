#pragma once
#include "Asynchronous.h"
#include "Log.h"
#include "Utils.h"

#include <string>
#include <unordered_map>

namespace onut
{
    class ContentManager
    {
    public:
        ContentManager()
        {
            addDefaultSearchPaths();
        }

        virtual ~ContentManager()
        {
            clear();
        }

        /**
         * Add default paths.
         */
        void addDefaultSearchPaths()
        {
            addSearchPath("../../assets");
            addSearchPath("../../assets/fonts");
            addSearchPath("../../assets/pfx");
            addSearchPath("../../assets/shaders");
            addSearchPath("../../assets/sounds");
            addSearchPath("../../assets/textures");
            addSearchPath("../../assets/musics");
        }

        /**
         * Clear search paths
         */
        void clearSearchPaths()
        {
            m_searchPaths.clear();
        }

        // Find the file
        std::string find(const std::string& name)
        {
            std::string filename;
            for (auto& path : m_searchPaths)
            {
                filename = findFile<false>(name, path, true);
                if (!filename.empty())
                {
                    break;
                }
            }
            return filename;
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
                    auto pResource = load<Ttype>(name);
                    m_resources[name] = pResource;
                    if (pResource)
                    {
                        return pResource->getResource();
                    }
                    else
                    {
                        return nullptr;
                    }
                }
                auto pResourceHolder = dynamic_cast<ResourceHolder<Ttype>*>(it->second);
                if (pResourceHolder)
                {
                    return pResourceHolder->getResource();
                }
                else
                {
                    return nullptr;
                }
            }
            else
            {
                std::promise<Ttype*> p;
                std::future<Ttype*> f = p.get_future();

                OSync([&p, name, this]{
                    auto it = m_resources.find(name);
                    if (it == m_resources.end())
                    {
                        // Load it
                        OAsync([&p, name, this]{
                            auto pResource = load<Ttype>(name);
                            OSync([&p, name, pResource, this]{
                                if (pResource)
                                {
                                    m_resources[name] = pResource;
                                    p.set_value(pResource->getResource());
                                }
                                else
                                {
                                    p.set_value(nullptr);
                                }
                            });
                        });
                        return;
                    }
                    auto pResourceHolder = dynamic_cast<ResourceHolder<Ttype>*>(it->second);
                    if (pResourceHolder)
                    {
                        p.set_value(pResourceHolder->getResource());
                    }
                    else
                    {
                        p.set_value(nullptr);
                    }
                });

                return f.get();
            }
        }

        /**
        Add a resource by name
        */
        template <typename Ttype>
        void addResource(const std::string& name)
        {
            if (m_threadId == std::this_thread::get_id())
            {
                auto it = m_resources.find(name);
                if (it == m_resources.end())
                {
                    // Load it
                    auto pResource = load<Ttype>(name);
                    m_resources[name] = pResource;
                    return;
                }
            }
            else
            {
                auto pResource = load<Ttype>(name);
                OSync([name, this, pResource]
                {
                    auto it = m_resources.find(name);
                    if (it == m_resources.end())
                    {
                        // Load it
                        m_resources[name] = pResource;
                    }
                    else
                    {
                        delete pResource;
                    }
                });
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
        ResourceHolder<Ttype>* load(const std::string& name)
        {
            // Find the file
            std::string filename;
            for (auto& path : m_searchPaths)
            {
                filename = findFile<false>(name, path, true);
                if (!filename.empty())
                {
                    break;
                }
            }
            if (filename == "")
            {
                return nullptr;
            }

            // Create it
            Ttype* pResource = Ttype::createFromFile(filename, this);
            OLog("Loaded " + filename);

            // Put it in our map
            ResourceHolder<Ttype>* pResourceHolder = new ResourceHolder<Ttype>(pResource);

            return pResourceHolder;
        }

        decltype(std::this_thread::get_id())    m_threadId = std::this_thread::get_id();
        std::vector<std::string>                m_searchPaths;
    };
}
