#pragma once

#include <string>

#include "IServerCallback.h"
#include "ServiceName.h"
#include "ServiceOperation.h"
#include "OperationParam.h"

namespace BrainCloud
{
	class BrainCloudRedemptionCode
	{
	public:
        
        /**
         * Redeem a code.
         *  
		 * Service Name - RedemptionCode
	     * Service Operation - REDEEM_CODE
	   	 *
         * @param in_scanCode The code to redeem
         * @param in_codeType The type of code
         * @param in_jsonCustomRedemptionInfo Optional - A JSON string containing custom redemption data
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {
         *         "gameId": "123456",
         *         "scanCode": "1200347",
         *         "codeType": "default",
         *         "version": 2,
         *         "codeState": "Redeemed",
         *         "customCodeInfo": {},
         *         "customRedemptionInfo": {},
         *         "redeemedByProfileId": "28adw61e-5634-49ae-9b09-f61930ce6e43",
         *         "redeemedByProfileName": "",
         *         "invalidationReason": null,
         *         "createdAt": 0,
         *         "activatedAt": null,
         *         "redeemedAt": 1445875694706,
         *         "invalidatedAt": null
         *     }
         * }
         */
		void redeemCode(const char* in_scanCode, const char* in_codeType, const char* in_jsonCustomRedemptionInfo, IServerCallback * in_callback = NULL);
        
        /**
         * Retrieve the codes already redeemed by player.
         *  
		 * Service Name - RedemptionCode
	     * Service Operation - GET_REDEEMED_CODES
	   	 *
         * @param in_codeType Optional - The type of codes to retrieve. Returns all codes if left unspecified.
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {
         *         "codes": [
         *             {
         *                 "gameId": "123456",
         *                 "scanCode": "999999",
         *                 "codeType": "default",
         *                 "version": 2,
         *                 "codeState": "Redeemed",
         *                 "customCodeInfo": {},
         *                 "customRedemptionInfo": {},
         *                 "redeemedByProfileId": "28d0745e-5634-49ae-9b09-f61930ce6e43",
         *                 "redeemedByProfileName": "",
         *                 "invalidationReason": null,
         *                 "createdAt": 0,
         *                 "activatedAt": null,
         *                 "redeemedAt": 1445456503428,
         *                 "invalidatedAt": null
         *             }
         *         ]
         *     }
         * }
         */
        void getRedeemedCodes(const char* in_codeType, IServerCallback * in_callback = NULL);
	};
}