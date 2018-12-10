#ifndef CONTENTMANAGER_H_INCLUDED
#define CONTENTMANAGER_H_INCLUDED

// STL
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(ContentManager);
OForwardDeclare(Resource);

namespace onut
{
    /*!
        Thread safe resource management class.
    */
    class ContentManager : public std::enable_shared_from_this<ContentManager>
    {
    public:
        using SearchPaths = std::vector<std::string>;

        static OContentManagerRef create();
        virtual ~ContentManager();

        // Resource
        void addResource(const std::string& name, const OResourceRef& pResource);
        void removeResource(const OResourceRef& pResource);
        bool isResourceLoaded(const std::string& name);
        void reload(const std::string& name);
        size_t size();
        void clear();
        OResourceRef getResource(const std::string& name);
        template<typename Tresource> std::shared_ptr<Tresource> getResourceAs(const std::string& name);

        // Search Paths
        void addDefaultSearchPaths();
        void addSearchPath(const std::string& path);
        void clearSearchPaths();
        std::string findResourceFile(const std::string& name);
        const SearchPaths& getSearchPaths() const;

    private:
        ContentManager();

        using ResourceMap = std::unordered_map<std::string, OResourceRef>;

        ResourceMap m_resources;
        SearchPaths m_searchPaths;
        std::mutex m_mutex;
    };

    template<typename Tresource>
    inline std::shared_ptr<Tresource> ContentManager::getResourceAs(const std::string& name)
    {
        auto pRet = std::dynamic_pointer_cast<Tresource>(getResource(name));
        if (!pRet)
        {
            auto searchName = name;
            auto pos = name.find_last_of("\\/");
            if (pos != std::string::npos)
            {
                searchName = name.substr(pos + 1);
            }
            auto filename = findResourceFile(searchName);
            if (!filename.empty())
            {
                pRet = Tresource::createFromFile(filename, shared_from_this());
                if (pRet)
                {
                    pRet->setName(name);
                    pRet->setFilename(filename);
                }
                addResource(name, pRet);
            }
        }
        return pRet;
    }
}

extern OContentManagerRef oContentManager;

#endif
