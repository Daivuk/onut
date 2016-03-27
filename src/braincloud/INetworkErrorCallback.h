//
//  INetworkErrorCallback.h
//  GameClientLib
//
//  Created by Preston Jennings on 2015-10-29.
//
//

#ifndef _INETWORKERRORCALLBACK_H_
#define _INETWORKERRORCALLBACK_H_

namespace BrainCloud {
    
    class ServiceName;
    class ServiceOperation;
    class OperationParam;
    
    class INetworkErrorCallback
    {
    public:
        
        /**
         * The networkError method is invoked whenever a network error is encountered
         * communicating to the brainCloud server.
         *
         * Note this method is *not* invoked when FlushCachedMessages(true) is called.
         */
        virtual void networkError() = 0;
    };
    
};
#endif /* _INETWORKERRORCALLBACK_H_ */
