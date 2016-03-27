#pragma once

#include <string>

#include "IServerCallback.h"
#include "ServiceName.h"
#include "ServiceOperation.h"
#include "OperationParam.h"
#include "BrainCloudTypes.h"

namespace BrainCloud {
    
    class BrainCloudDataStream
    {
    public:
        
        /**
         * Creates custom data stream page event
         *
         * @param eventName Name of event
         * @param eventProperties Properties of event
         * @return json as follows
         *
         * {
         * "status": 200,
         * "data": {
         *   null
         *  }
         * }
         * 
         */
        void customPageEvent(const char * in_eventName, const char * in_jsonEventProperties, IServerCallback * in_callback = NULL);
        
        /**
         * Creates custom data stream screen event
         *
         * @param eventName Name of event
         * @param eventProperties Properties of event
         * @return json as follows
         *
         * {
         * "status": 200,
         * "data": {
         *   null
         *  }
         * }
         * 
         */
        void customScreenEvent(const char * in_eventName, const char * in_jsonEventProperties, IServerCallback * in_callback = NULL);
        
        /**
         * Creates custom data stream track event
         *
         * @param eventName Name of event
         * @param eventProperties Properties of event
         * @return json as follows
         *
         * {
         * "status": 200,
         * "data": {
         *   null
         *  }
         * }
         * 
         */
        void customTrackEvent(const char * in_eventName, const char * in_jsonEventProperties, IServerCallback * in_callback = NULL);
        
    };
}
