#pragma once

#include <string>

#include "ServiceName.h"
#include "ServiceOperation.h"
#include "OperationParam.h"
#include "IServerCallback.h"
#include "BrainCloudTypes.h"

namespace BrainCloud {
	
	class BrainCloudProduct
	{
	public:
		/**
         * Gets the player's currency for the given currency type
         * or all currency types if null passed in.
         *
		 * Service Name - Product
	     * Service Operation - GetPlayerVC
	   	 *
         * @param in_currencyType The currency type to retrieve or null
         * if all currency types are being requested.
		 *
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data": {
         *     "updatedAt": 1395693676208,
         *     "currencyMap": {
         *       "gold": {
         *         "purchased": 0,
         *         "balance": 0,
         *         "consumed": 0,
         *         "awarded": 0
         *       }
         *     },
         *     "playerId": "6ea79853-4025-4159-8014-60a6f17ac4e6",
         *     "createdAt": 1395693676208
         *   }
         * }
         */
		void getCurrency(const char* in_currencyType, IServerCallback * in_callback = NULL);
		
		/**
         * Award player the passed-in amount of currency. Returns
         * JSON representing the new currency values.
         *
		 * Service Name - Product
	     * Service Operation - AwardVC
	   	 *
		 * @param in_currencyType The currency type to award
		 * @param in_amount The amount of currency to award
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data": {
         *     "updatedAt": 1395693913234,
         *     "currencyMap": {
         *       "gems": {
         *         "purchased": 0,
         *         "balance": 0,
         *         "consumed": 0,
         *         "awarded": 0
         *       },
         *       "gold": {
         *         "purchased": 0,
         *         "balance": 123,
         *         "consumed": 0,
         *         "awarded": 123
         *       }
         *     },
         *     "playerId": "acf11847-055f-470d-abb7-b93052201491",
         *     "createdAt": 1395693907421
         *   }
         * }
         */
		void awardCurrency(const char* in_currencyType, int32_t in_amount, IServerCallback * in_callback = NULL);
		
		/**
         * Consume the passed-in amount of currency from the player.
         *
		 * Service Name - Product
	     * Service Operation - ConsumePlayerVC
	   	 *
         * @param in_currencyType The currency type to consume.
         * @param in_amount The amount of currency to consume.
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data": {
         *     "updatedAt": 1395693913234,
         *     "currencyMap": {
         *       "gems": {
         *         "purchased": 0,
         *         "balance": 0,
         *         "consumed": 0,
         *         "awarded": 0
         *       },
         *       "gold": {
         *         "purchased": 0,
         *         "balance": 0,
         *         "consumed": 123,
         *         "awarded": 123
         *       }
         *     },
         *     "playerId": "acf11847-055f-470d-abb7-b93052201491",
         *     "createdAt": 1395693907421
         *   }
         * }
         */
		void consumeCurrency(const char* in_currencyType, int32_t in_amount, IServerCallback * in_callback = NULL);
		
		/**
         * Resets the player's currency back to zero.
         *
		 * Service Name - Product
	     * Service Operation - ResetPlayerVC
	   	 *
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data":null
         * }
         */
		void resetCurrency(IServerCallback * in_callback = NULL);
		
		/**
         * Method gets the active sales inventory for the passed-in platform and
         * currency type.
         *
		 * Service Name - Product
	     * Service Operation - GetInventory
	   	 *
         * @param in_platform The store platform. Valid stores are:
         * - itunes
         * - facebook
         * - appworld
         * - steam
         * - windows
         * - windowsPhone
         * - googlePlay
         * @param in_userCurrency The currency to retrieve the sales
         * inventory for. This is only used for Steam and Facebook stores.
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
		 * {
		 *   "status":200,
		 *   "data":{
		 *      "product_inventory":[
		 *          {
		 *              "gameId":"com.roger.football",
		 *              "itemId":"0000001",
		 *              "title":"Item 0000001",
		 *              "description":"Buy 5 footballs",
		 *              "imageUrl":"http:",
		 *              "fbUrl":"http:",
		 *              "currency":{"footballs":5},
		 *              "priceData":{"currency":"USD","price":1000}
		 *           }
		 *       ],
		 *       "server_time":1398960658981
		 *    }
		 * }
		 */
		void getSalesInventory(const char * in_platform, const char * in_userCurrency, IServerCallback * in_callback = NULL);
        
        /**
         * Method gets the active sales inventory for the passed-in platform,
         * currency type and category.
         *
         * Service Name - Product
         * Service Operation - GetInventory
         *
         * @param in_platform The store platform. Valid stores are:
         * - itunes
         * - facebook
         * - appworld
         * - steam
         * - windows
         * - windowsPhone
         * - googlePlay
         * @param in_userCurrency The currency to retrieve the sales
         * inventory for. This is only used for Steam and Facebook stores.
		 * @param in_category Inventory category to retrieve
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data":{
         *      "product_inventory":[
         *          {
         *              "gameId":"com.roger.football",
         *              "itemId":"0000001",
         *              "title":"Item 0000001",
         *              "description":"Buy 5 footballs",
         *              "imageUrl":"http:",
         *              "fbUrl":"http:",
         *              "currency":{"footballs":5},
         *              "priceData":{"currency":"USD","price":1000}
         *           }
         *       ],
         *       "server_time":1398960658981
         *    }
         * }
         */
        void getSalesInventoryByCategory(const char * in_platform, const char * in_userCurrency, const char * in_category, IServerCallback * in_callback = NULL);


        // deprecated - use verifyItunesReceipt as method has been renamed
		void cashInItunesReceipt( const std::string& in_base64EncReceiptData, IServerCallback * in_callback, bool in_useSandbox );
        
		/**
         * Method verifies an iTunes receipt and awards the items related to this receipt.
         *
         * Service Name - Product
         * Server Operation - OP_CASH_IN_RECEIPT
         *
         * @param in_base64EncReceiptData The iTunes receipt
         * @param in_callback The method to be invoked when the server response is received
		 *
		 * @return The JSON returned in the callback is as follows:
		 * {
		 *   "status": 200,
		 *   "data":
		 *   {
		 *      "playerCurrency" : {
		 *         "playerId" : "sfhsjdfhfjhf",
		 *         "currencyMap" : {
		 *             "coin" : {
		 *                 "purchased" : NumberLong(0),
		 *                 "balance" : NumberLong(5000),
		 *                 "consumed" : NumberLong(0),
		 *                 "awarded" : NumberLong(5000)
		 *             },
		 *             "bar" : {
		 *                 "purchased" : NumberLong(0),
		 *                 "balance" : NumberLong(2),
		 *                 "consumed" : NumberLong(0),
		 *                 "awarded" : NumberLong(2)
		 *             }
		 *         },
		 *         "createdAt" : 763578645786,
		 *         "updatedAt" : 8692486255764,
		 *       },
		 *       "appleReceipt" : "gsgsfvgvg",
		 *       "status" : 0,
		 *       "server_time" : 987490827457
		 *   }
		 * }
		 */
        void verifyItunesReceipt( const std::string& in_base64EncReceiptData, IServerCallback * in_callback = NULL);


		/**
         * Initialize Steam Transaction
         *
         * Service Name - Product
         * Service Operation - StartSteamTransaction
         *
         * @param in_language ISO 639-1 language code
         * @param in_items Items to purchase
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data":
         *   {
         *      "steamStatus" : 200,
         *      "steamData" :
         *      {
         *      }
         *   }
         * }
         */
		void startSteamTransaction( const char* in_language, const char* in_itemId, IServerCallback * in_callback = NULL);
		
		/**
         * Finalize Steam Transaction. On success, the player will be awarded the
         * associated currencies.
         *
         * Service Name - Product
         * Service Operation - FinalizeSteamTransaction
         *
         * @param in_transId Steam transaction id
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data":
         *   {
         *      "steamStatus" : 200,
         *      "steamData" :
         *      {
         *      }
         *   }
         * }
         */
		void finalizeSteamTransaction(const char* in_transId, IServerCallback * in_callback = NULL);
		
		/**
         * Verify Microsoft Receipt. On success, the player will be awarded the
         * associated currencies.
         *
         * Service Name - Product
         * Service Operation - VerifyMicrosoftReceipt
         *
         * @param in_receipt Receipt XML
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data":
         *   {
         *      "result" : "OK"
         *   }
         * }
         */
		void verifyMicrosoftReceipt(const char* in_receipt, IServerCallback * in_callback = NULL);
        
        /**
         * Confirms a google play purchase. On success, the player will be awarded the
         * associated currencies.
         * 
         * Service Name - Product
         * Server Operation - CONFIRM_GOOGLEPLAY_PURCHASE
         * 
         * @param in_orderId The order id
         * @param in_productId The product id
         * @param in_token Google Play token string
         * @param in_callback The method to be invoked when the server response is received
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data":
         *   {
         *      "result" : "OK"
         *   }
         * }
         */
        void confirmGooglePlayPurchase(
                const char * in_orderId,
                const char * in_productId,
                const char * in_token,
                IServerCallback * in_callback = NULL);

		/**
         * Get Eligible Promotions
         *
         * Service Name - Product
         * Service Operation - GetEligiblePromotions
         *
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data":
         *   {
         *      {
		* "promotions": [
		*  {
		*   "gameId": "10019",
		*   "promotionId": 9,
		*   "type": "SCHEDULED",
		*   "name": "session >= 2",
		*   "message": "test1",
		*   "enabled": true,
		*   "targetAllUsers": false,
		*   "segments": [
		*    5
		*   ],
		*   "prices": [
		*    {
		*     "itemId": "regGems150",
		*     "priceId": 1
		*    }
		*   ],
		*   "notifications": [
		*    {
		*     "trigger": "ACTIVATED",
		*     "notificationTemplateId": 10
		*    }
		*   ],
		*   "startAt": 1415374185745,
		*   "endAt": 1415806185745,
		*   "createdAt": 0,
		*   "updatedAt": 1415729753294
		*  }
		* ]
		*}
         *   }
         * }
         */
		void getEligiblePromotions(IServerCallback * in_callback = NULL);
        
        /**
         * Awards currency in a parent app.
         *
         * Service Name - Product
         * Service Operation - AWARD_PARENT_VC
         *
         * @param in_currencyType The ID of the parent currency
         * @param in_amount The amount of currency to award
         * @param in_parentLevel The level of the parent containing the currency
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data": {
         *     "updatedAt": 1395693913234,
         *     "currencyMap": {
         *       "gems": {
         *         "purchased": 0,
         *         "balance": 0,
         *         "consumed": 0,
         *         "awarded": 0
         *       },
         *       "gold": {
         *         "purchased": 0,
         *         "balance": 123,
         *         "consumed": 0,
         *         "awarded": 123
         *       }
         *     },
         *     "playerId": "acf11847-055f-470d-abb7-b93052201491",
         *     "createdAt": 1395693907421
         *   }
         * }
         */
        void awardParentCurrency(const char* in_currencyType, int32_t in_amount, const char * in_parentLevel, IServerCallback * in_callback = NULL);
        
        /**
         * Consumes currency in a parent app.
         *
         * Service Name - Product
         * Service Operation - CONSUME_PARENT_VC
         *
         * @param in_currencyType The ID of the parent currency
         * @param in_amount The amount of currency to consume
         * @param in_parentLevel The level of the parent containing the currency
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data": {
         *     "updatedAt": 1395693913234,
         *     "currencyMap": {
         *       "gems": {
         *         "purchased": 0,
         *         "balance": 0,
         *         "consumed": 0,
         *         "awarded": 0
         *       },
         *       "gold": {
         *         "purchased": 0,
         *         "balance": 123,
         *         "consumed": 0,
         *         "awarded": 123
         *       }
         *     },
         *     "playerId": "acf11847-055f-470d-abb7-b93052201491",
         *     "createdAt": 1395693907421
         *   }
         * }
         */    
        void consumeParentCurrency(const char* in_currencyType, int32_t in_amount, const char * in_parentLevel, IServerCallback * in_callback = NULL);
        
        /**
         * Gets information on a single currency in a parent app
         * or all currency types if a null type is passed in.
         *
         * Service Name - Product
         * Service Operation - GET_PARENT_VC
         *
         * @param in_currencyType The ID of the parent currency or null to get all currencies
         * @param in_parentLevel The level of the parent containing the currency
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data": {
         *     "updatedAt": 1395693913234,
         *     "currencyMap": {
         *       "gems": {
         *         "purchased": 0,
         *         "balance": 0,
         *         "consumed": 0,
         *         "awarded": 0
         *       }
         *     },
         *     "playerId": "acf11847-055f-470d-abb7-b93052201491",
         *     "createdAt": 1395693907421
         *   }
         * }
         */
        void getParentCurrency(const char* in_currencyType, const char * in_parentLevel, IServerCallback * in_callback = NULL);
        
        /**
         * Resets all currencies in a parent app.
         *
         * Service Name - Product
         * Service Operation - RESET_PARENT_VC
         *
         * @param in_parentLevel The level of the parent containing the currencies
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": null
         * }
         */
        void resetParentCurrency(const char * in_parentLevel, IServerCallback * in_callback = NULL);
		
	};
}