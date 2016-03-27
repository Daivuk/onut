#pragma once

#include <string>

#include "IServerCallback.h"
#include "ServiceName.h"
#include "ServiceOperation.h"
#include "OperationParam.h"

namespace BrainCloud
{
	class BrainCloudTime
	{
	public:
		/**
         * Method returns the server time in UTC. This is in UNIX millis time format.
         * For instance 1396378241893 represents 2014-04-01 2:50:41.893 in GMT-4.
         *
		 * Service Name - Time
	     * Service Operation - Read
	   	 *
         * Server API reference: ServiceName.Time, ServiceOperation.Read
         *
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return A JSON string such as:
         * {
         *   "status":200,
         *   "data":{
         *     "server_time":1396378241893
         *   }
         * }
         */
		void readServerTime( IServerCallback * in_callback = NULL);
	};
}