//
//  IEventCallback.h
//  GameClientLib
//
//  Created by Preston Jennings on 2015-10-29.
//
//

#ifndef _IEVENTCALLBACK_H_
#define _IEVENTCALLBACK_H_

#include <string>

namespace BrainCloud {
    
    class IEventCallback
    {
    public:
        virtual ~IEventCallback( )  { }
        
        /**
         * Method called when an event is returned from brainCloud
         *
         * @param jsonData - returned data from the server
         */
        virtual void eventCallback(std::string const & jsonData) = 0;
    };
    
};
#endif /* _IEVENTCALLBACK_H_ */
