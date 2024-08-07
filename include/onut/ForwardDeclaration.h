#ifndef FORWARDDECLARATION_H_INCLUDED
#define FORWARDDECLARATION_H_INCLUDED

// STL
#include <memory>

#define OMake std::make_shared
#define ODynamicCast std::dynamic_pointer_cast
#define OStaticCast std::static_pointer_cast
#define OThis shared_from_this()
#define OEnableOThis(__class__) public std::enable_shared_from_this<__class__>

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

#define ForwardDeclareStruct(__class__) \
struct __class__; \
using __class__ ## Ref = std::shared_ptr<__class__>; \
using __class__ ## Weak = std::weak_ptr<__class__>;

#endif
