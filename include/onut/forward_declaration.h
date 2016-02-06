#pragma once
#include <memory>

#define OForwardDeclare(__class__) \
namespace onut \
{ \
    class __class__; \
}; \
using O ## __class__ = onut::__class__; \
using O ## __class__ ## Ref = std::shared_ptr<onut::__class__>; \
using O ## __class__ ## Weak = std::weak_ptr<onut::__class__>;
