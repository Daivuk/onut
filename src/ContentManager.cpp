#include "ContentManager.h"

namespace onut {
    ContentManager::~ContentManager() {
        clear();
    }

    void ContentManager::clear() {
        for (auto& kv : m_resources) {
            delete kv.second;
        }
        m_resources.clear();
    }
}
