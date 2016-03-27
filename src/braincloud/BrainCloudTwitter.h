#pragma once

#include <string>

#include "IServerCallback.h"
#include "ServiceName.h"
#include "ServiceOperation.h"
#include "OperationParam.h"

namespace BrainCloud {
	
	class BrainCloudTwitter
	{
		public:
		// TODO: document these
		void authorizeTwitter( IServerCallback * in_callback = NULL);
		
        void verifyTwitter( const char * in_token, const char * in_verifier, IServerCallback * in_callback = NULL);
		
        void tweet( const char * in_token, const char * in_secret, const char * in_tweet, const char * in_picture, IServerCallback * in_callback = NULL);
	};
}