#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "StringUtils.h"

namespace onut {
    class Texture;
    class BMFont;

    class ContentManager {
    public:
        virtual ~ContentManager();

        /**
            Get a resource by name
        */
        template <typename Ttype>
        Ttype* getResource(const std::string& name) {
            auto it = m_resources.find(name);
            if (it == m_resources.end()) {
                // Load it
                return load<Ttype>(name);
            }
            auto pResourceHolder = dynamic_cast<ResourceHolder<Ttype>*>(it->second);
            if (!pResourceHolder) {
                return nullptr;
            }
            return pResourceHolder->getResource();
        }

        /**
            Delete all content loaded by this ContentManager
        */
        void clear();

    private:
        class IResourceHolder {
        public:
            virtual ~IResourceHolder() {}
        };

        template <typename Ttype>
        class ResourceHolder : public IResourceHolder {
        public:
            ResourceHolder(Ttype* pResource) : m_pResource(pResource) {}
            virtual ~ResourceHolder() {
                if (m_pResource) {
                    delete m_pResource;
                    m_pResource = nullptr;
                }
            }

            Ttype* getResource() const { return m_pResource; }

        private:
            Ttype* m_pResource = nullptr;
        };

        template <typename Ttype>
        Ttype* load(const std::string& name) {
            auto filename = findFile(name);
            Ttype* pResource = Ttype::createFromFile(filename);
            ResourceHolder<Ttype>* pResourceHolder = new ResourceHolder<Ttype>(pResource);
            m_resources[name] = pResourceHolder;
            return pResource;
        }

        std::unordered_map<std::string, IResourceHolder*> m_resources;
    };
};
