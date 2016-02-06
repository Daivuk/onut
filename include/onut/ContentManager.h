#pragma once
#include <mutex>
#include <string>
#include <unordered_map>

#include "onut/forward_declaration.h"
OForwardDeclare(ContentManager);
OForwardDeclare(Resource);

namespace onut
{
    /*!
        Thread safe resource management class.
    */
    class ContentManager
    {
    public:
        static OContentManagerRef create();

        ContentManager();
        virtual ~ContentManager();

        // Resource
        void addResource(const std::string& name, const OResourceRef& pResource);
        void removeResource(const OResourceRef& pResource);
        size_t size();
        void clear();
        OResourceRef getResource(const std::string& name);

        // Search Paths
        void addDefaultSearchPaths();
        void addSearchPath(const std::string& path);
        void clearSearchPaths();
        std::string findResourceFile(const std::string& name);

    private:
        using ResourceMap = std::unordered_map<std::string, OResourceRef>;
        using SearchPaths = std::vector<std::string>;

        ResourceMap m_resources;
        SearchPaths m_searchPaths;
        std::mutex m_mutex;
    };
}

extern OContentManagerRef oContentManager;
