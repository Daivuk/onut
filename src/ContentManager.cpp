// Onut
#include <onut/ContentManager.h>
#include <onut/Files.h>
#include <onut/Resource.h>

// STL
#include <cassert>

OContentManagerRef oContentManager;

namespace onut
{
    OContentManagerRef ContentManager::create()
    {
        return std::shared_ptr<OContentManager>(new OContentManager());
    }

    ContentManager::ContentManager()
    {
        addDefaultSearchPaths();
    }

    ContentManager::~ContentManager()
    {
        std::unique_lock<std::mutex> locker(m_mutex);
    }

    void ContentManager::addDefaultSearchPaths()
    {
        addSearchPath("../../assets");
    }

    void ContentManager::clearSearchPaths()
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_searchPaths.clear();
    }

    void ContentManager::addSearchPath(const std::string& path)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_searchPaths.push_back(path);
    }

    std::string ContentManager::findResourceFile(const std::string& name)
    {
        std::string filename;
        std::unique_lock<std::mutex> locker(m_mutex);
        for (auto& path : m_searchPaths)
        {
            filename = findFile(name, path, true);
            if (!filename.empty())
            {
                break;
            }
        }
        return filename;
    }

    void ContentManager::addResource(const std::string& name, const OResourceRef& pResource)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_resources[name] = pResource;
    }

    void ContentManager::removeResource(const OResourceRef& pResource)
    {
        auto& resourceName = pResource->getName();
        std::unique_lock<std::mutex> locker(m_mutex);
        for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
        {
            if (it->second == pResource)
            {
                m_resources.erase(it);
                return;
            }
        }
    }

    size_t ContentManager::size()
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        return m_resources.size();
    }

    void ContentManager::clear()
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_resources.clear();
    }

    OResourceRef ContentManager::getResource(const std::string& name)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        auto it = m_resources.find(name);
        if (it != m_resources.end())
        {
            return it->second;
        }
        return nullptr;
    }
}
