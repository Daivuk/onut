#pragma once

#include <string>

#include "IServerCallback.h"
#include "ServiceName.h"
#include "ServiceOperation.h"
#include "OperationParam.h"

namespace BrainCloud {
	
	class BrainCloudPlayerStatisticsEvent
	{
		public:
		/**
         * Trigger an event server side that will increase the players statistics.
         * This may cause one or more awards to be sent back to the player - 
         * could be achievements, experience, etc. Achievements will be sent by this
         * client library to the appropriate awards service (Apple Game Center, etc).
         * 
         * This mechanism supercedes the PlayerStatisticsService API methods, since
         * PlayerStatisticsService API method only update the raw statistics without
         * triggering the rewards.
         *          
		 * Service Name - PlayerStatisticsEvent
	     * Service Operation - Trigger
	   	 *
         * @see BrainCloudPlayerStatistics
         * 
         * @param in_callback The method to be invoked when the server response is received
         * 
         * @return The JSON returned in the callback is as follows:
		 * {
		 *   "status": 200,
		 *   "data": {
		 *   "experiencePoints": 25,
		 *   "achievementsGranted": [
		 *     "ach01"
		 *    ],
		 *    "currencyAwarded": {
		 *      "gems": {
		 *        "purchased": 0,
		 *        "balance": 10,
		 *        "consumed": 0,
		 *        "awarded": 10
		 *      },
		 *      "gold": {
		 *        "purchased": 0,
		 *        "balance": 2000,
		 *        "consumed": 0,
		 *        "awarded": 2000
		 *      }
		 *    },
		 *    "totalPlayerStatistics": {
		 *      "minions": 100,
		 *      "wood": 55,
		 *      "iron": 5
		 *    },
		 *    "experienceLevel": 2,
		 *    "experienceLevelsAwarded": [
		 *      2
		 *    ],
		 *    "playerStatisticsAwarded": {
		 *      "minions": 100,
		 *      "wood": 5,
		 *      "iron": 5
		 *    },
		 *    "experiencePointsAwarded": 15,
		 *    "currency": {
		 *      "gems": {
		 *        "purchased": 0,
		 *        "balance": 20,
		 *        "consumed": 0,
		 *        "awarded": 20
		 *      },
		 *      "gold": {
		 *        "purchased": 0,
		 *        "balance": 3000,
		 *        "consumed": 0,
		 *        "awarded": 3000
		 *      }
		 *    }
		 *  }
		 * }
         */
        void triggerPlayerStatisticsEvent(const char * in_eventName, int in_eventMultiplier, IServerCallback * in_callback = NULL);
        
		/**
         * See documentation for TriggerPlayerStatisticsEvent for more
         * documentation.
         *
		 * Service Name - PlayerStatisticsEvent
	     * Service Operation - TriggerMultiple
	   	 *
         * @param in_jsonData
         *   [
         *     {
         *       "eventName": "event1",
         *       "eventMultiplier": 1
         *     },
         *     {
         *       "eventName": "event2",
         *       "eventMultiplier": 1
         *     }
         *   ]
         */
		void triggerPlayerStatisticsEvents(const std::string& in_jsonData, IServerCallback * in_callback = NULL);
	};
}