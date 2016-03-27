//
//  ServerCall.h
//  BrainCloudLib
//


#ifndef _SERVERCALL_H_
#define _SERVERCALL_H_

#include <map>
#include <string>
#include "json/json.h"
#include "IServerCallback.h"
#include "OperationParam.h"
#include "ServiceName.h"
#include "ServiceOperation.h"

namespace BrainCloud {

    /**
     * This is a data structure representing a single message to send to the
     * server.
     */
    class ServerCall {

    public:
        ServerCall();
		ServerCall( ServiceName serviceName, ServiceOperation serviceOperation, const Json::Value & data, IServerCallback * callback = NULL );

        virtual ~ServerCall( );

        IServerCallback *    getCallback( ) const              { return( _callback ); }
		const Json::Value *  getPayload( );
        ServiceOperation          getOperation( ) const             { return( _operation ); }
        ServiceName          getService( ) const               { return( _service ); }
		std::string          getPostPayload( ) const           { return( _postPayload ); }
		void                 setPostPayload( std::string val ) { _postPayload = val; }

    protected:

    private:

        IServerCallback *                   _callback;
		Json::Value                         _data;
		ServiceOperation					_operation;
		ServiceName							_service;
		Json::Value                         _payload;

		// Only used for non-bundled posts... 
		std::string                         _postPayload;

    };  // end class


}  // end namespace


#endif  // _SERVERCALL_H_
