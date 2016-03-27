#pragma once

#include "ServiceName.h"
#include "ServiceOperation.h"
#include "OperationParam.h"
#include "IServerCallback.h"
#include "BrainCloudTypes.h"

namespace BrainCloud {
	
	class BrainCloudGlobalApp
	{
	public:
		/**
         * Read game's global properties
         *
         * Service Name - GlobalApp
         * Service Operation - ReadProperties
         *
         * @param in_callback The method to be invoked when the server response is received
         *
		 * @return JSON describing the global properties:
		 * {
		 *   "status":200,   
		 *   "data":{    
		 *     {
		 *		 "pName": {
		 *		  "name": "pName",
		 *		  "description": "pValue",
		 *		  "value": "pDescription"
		 *		 }
		 *		}
		 *   }
		 * }
         */
		void readProperties(IServerCallback * in_callback = NULL);
		
	protected:
		
	};
}