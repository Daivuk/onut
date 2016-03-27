//
//  IGlobalErrorCallback.h
//  BrainCloudLib
//

#ifndef _IGLOBALERRORCALLBACK_H_
#define _IGLOBALERRORCALLBACK_H_

#include <string>

namespace BrainCloud
{
	class ServiceName;
	class ServiceOperation;
	class OperationParam;

    class IGlobalErrorCallback
    {
    public:
        /**
         * Errors are returned back to the layer which is interfacing with the
         * BrainCloud library through the serverError() callback.
         *
         * A server error might indicate a failure of the client to communicate
         * with the server after N retries.
         *
		 * @param serviceName The service name being called
		 * @param serviceOperation The service operation being called
         * @param statusCode The error status return code (400, 403, 500, etc)
         * @param reasonCode The brainCloud reason code (see reason codes on apidocs site)
         * @param jsonError The error json string
         */
        virtual void globalError( ServiceName serviceName, ServiceOperation serviceOperation, int statusCode, int reasonCode, const std::string & jsonError) = 0;
    };
}

#endif  // _IGLOBALERRORCALLBACK_H_
