#pragma once
// STL
#include <memory>

#define OMake std::make_shared
#define ODynamicCast std::dynamic_pointer_cast
#define OStaticCast std::static_pointer_cast
#define OThis shared_from_this()

#define OForwardDeclare(__class__) \
namespace onut \
{ \
    class __class__; \
}; \
using O ## __class__ = onut::__class__; \
using O ## __class__ ## Ref = std::shared_ptr<onut::__class__>; \
using O ## __class__ ## Weak = std::weak_ptr<onut::__class__>;

#define ForwardDeclare(__class__) \
class __class__; \
using __class__ ## Ref = std::shared_ptr<__class__>; \
using __class__ ## Weak = std::weak_ptr<__class__>;
