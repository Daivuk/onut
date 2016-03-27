//
//  IServerCallback.h
//  BrainCloudLib
//


#ifndef _ISERVERCALLBACK_H_
#define _ISERVERCALLBACK_H_

#include <string>

namespace BrainCloud {

	class ServiceName;
	class ServiceOperation;
	class OperationParam;

    /**
     * Pure virtual class providing a means for waking up the client when a
     * response is received from the server.
     *
     * @author keving
     */
    class IServerCallback {

    public:
        // Destructor - make sure it is virtual to avoid problems with
        // destroying a sub-classed object using an IServerCallback pointer.
        virtual ~IServerCallback( )  { }

        // All sub-classes must implement the following methods.

        /**
         * The serverCallback() method returns server data back to the layer
         * interfacing with the BrainCloud library.
         *
         * @param serviceName - name of the requested service
         * @param serviceOperation - requested operation
         * @param jsonData - returned data from the server
         */
        virtual void serverCallback( ServiceName serviceName, ServiceOperation serviceOperation, std::string const & jsonData) = 0;
		
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
        virtual void serverError( ServiceName serviceName, ServiceOperation serviceOperation, int statusCode, int reasonCode, const std::string & jsonError) = 0;
		
        /**
         * Warnings are returned back to the layer which is interfacing with the
         * BrainCloud library through the serverWarning() callback.
         *
         * Examples:
         *   - communication failure with the server, but the BrainCloud library
         *     is attempting a retry
         *   - an error occurred on the server and no callback was set for the
         *     message (?)
         *   - the server responded but no callback was set for the message
         *
         * This could provide an opportunity for the user interface to display
         * there are communication issues.  At this point, the interface could
         * slow down or pause gameplay because the game state might be lost if
         * the server cannot be reached, or some other action.
         *
		 * @param serviceName The service name being called
		 * @param serviceOperation The service operation being called
         * @param statusCode The error status return code (400, 403, 500, etc)
         * @param reasonCode The brainCloud reason code (see reason codes on apidocs site)
         * @param numRetries The number of retries of this message
         * @param statusMessage Any error messages related to the issue
         *
         * @deprecated This callback will be removed in the future
         */
        virtual void serverWarning( ServiceName serviceName, ServiceOperation serviceOperation, int statusCode, int reasonCode, int numRetries, const std::string & statusMessage) = 0;
		
    protected:

    private:

    };  // end class

}  // end namespace


#endif  // _ISERVERCALLBACK_H_
