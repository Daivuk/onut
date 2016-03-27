#pragma once

#include <string>
#include <vector>

#include "IServerCallback.h"
#include "BrainCloudTypes.h"

namespace BrainCloud
{
	/**
	 * This class is used to directly update the player's statistics registered on the server.
	 *	However the preferred mechanism is to use "PlayerStatisticsEvents" via the
	 *  BrainCloud.triggerPlayerStatisticsEvent() method. Using that method allows achivements,
	 *	experience, and other award to be triggered in a manner set up on the server.
	 */
	class BrainCloudPlayerStatistics {
		
	public:
		/**
         * Read all available player statistics.
         *
		 * Service Name - PlayerStatistics
	     * Service Operation - Read
	   	 *
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data":{
         *     "statistics":{
         *       "minions":0,
         *       "wood":50,
         *       "pantelons":3,
         *       "iron":0
         *     }
         *   }
         * }
         */
		void readAllPlayerStats(IServerCallback * in_callback = NULL);
		
		/**
         * Reads a subset of player statistics as defined by the input collection.
         *          
		 * Service Name - PlayerStatistics
	     * Service Operation - ReadSubset
	   	 *
         * @param in_statistics A collection containing the subset of statistics to read:
         * ex. [ "pantaloons", "minions" ]
         * @param in_callback The method to be invoked when the server response is received
         * 
         * @return JSON with the subset of global statistics:
         * {
         *   "status":200,   
         *   "data":{      
         *     "statistics":{         
         *       "wood":11,         
         *       "minions":1
         *     }   
         *   }
         * }
         */
        void readPlayerStatsSubset(const std::vector<std::string> & in_statistics, IServerCallback * in_callback = NULL);
        
        //Removal after March 04 2016
        DEPRECATED void readPlayerStatsSubset(const std::string& in_jsonData, IServerCallback * in_callback = NULL);
        
        /**
         * Method retrieves the player statistics for the given category.
         *
         * Service Name - PlayerStatistics
         * Service Operation - READ_FOR_CATEGORY
         *
         * @param in_category The player statistics category
         * @param in_callback Method to be invoked when the server response is received.
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data":{
         *     "playerStatistics": []
         *   }
         * }
         */
        void readPlayerStatsForCategory(const char * in_category, IServerCallback * in_callback = NULL);

		/**
         * Reset all of the statistics for this player back to their initial value.
         *
		 * Service Name - PlayerStatistics
	     * Service Operation - Reset
	   	 *
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data":null
         * }
         */
		void resetAllPlayerStats(IServerCallback * in_callback = NULL);
		
		/**
         * Atomically increment (or decrement) player statistics.
         * Any rewards that are triggered from player statistic increments
         * will be considered. Player statistics are defined through the brainCloud portal.
         * Note also that the "xpCapped" property is returned (true/false depending on whether
         * the xp cap is turned on and whether the player has hit it).
         *
		 * Service Name - PlayerStatistics
	     * Service Operation - Update
	   	 *
         * @param in_jsonData The JSON encoded data to be sent to the server as follows:
         * {
         *   stat1: 10,
         *   stat2: -5.5,
         * }
         * would increment stat1 by 10 and decrement stat2 by 5.5.
         * For the full statistics grammer see the api.braincloudservers.com site.
         * There are many more complex operations supported such as:
         * {
         *   stat1:INC_TO_LIMIT#9#30
         * }
         * which increments stat1 by 9 up to a limit of 30.
         *
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return JSON describing the new value of the statistics and any rewards that were triggered:
         *  {
         *    "status":200,
         *    "data":{
         *      "experiencePoints":10,
         *      "xpCapped":false,
         *      "rewardDetails":{
         *        "xp":{
         *          "experienceLevels":[
         *            {
         *              "level":1,
         *              "reward":{
         *                "currency":{
         *                  "gold":1000
         *                }
         *              }
         *            }
         *          ]
         *        }
         *      },
         *      "rewards":{
         *        "experienceLevels":[
         *          1
         *        ],
         *        "currency":{
         *          "gold":1000
         *        }
         *      },
         *      "experienceLevel":1,
         *      "statistics":{
         *        "LIVES":1
         *      },
         *      "currency":{
         *        "gems":{
         *          "purchased":0,
         *          "balance":10,
         *          "consumed":0,
         *          "awarded":10
         *        },
         *        "gold":{
         *          "purchased":0,
         *          "balance":2000,
         *          "consumed":0,
         *          "awarded":2000
         *        }
         *      }
         *    }
         *  }
         */
		void incrementPlayerStats(const std::string& in_jsonData, IServerCallback * in_callback = NULL);
		
		/**
         * Returns JSON representing the next experience level for the player.
         *
		 * Service Name - PlayerStatistics
	     * Service Operation - ReadNextXpLevel
	   	 *
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return JSON describing the next experience level for the player.
         * {
         *   "status":200,
         *   "data":{
         *     "xp_level":{
         *       "gameId":"com.bitheads.unityexample",
         *       "numericLevel":2,
         *       "experience":20,
         *       "reward":{
         *         "globalGameStatistics":null,
         *         "experiencePoints":null,
         *         "playerStatistics":null,
         *         "achievement":null,
         *         "currencies":{
         *           "gems":10,
         *           "gold":2000
         *         }
         *       },
         *       "facebookAction":"",
         *       "statusTitle":"Jester",
         *       "key":{
         *         "gameId":"com.bitheads.unityexample",
         *         "numericLevel":2,
         *         "primaryKey":true
         *       }
         *     }
         *   }
         * }
         */
		void getNextExperienceLevel(IServerCallback* in_callback);
		
		/**
         * Increments the player's experience. If the player goes up a level,
         * the new level details will be returned along with a list of rewards.
         *
		 * Service Name - PlayerStatistics
	     * Service Operation - UpdateIncrement
	   	 *
		 * @param in_xpValue The amount to increase the player's experience by
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return JSON describing the player's .
         * {
         *   "status":200,
         *   "data":{
         *     "statisticsExceptions":{
         *     },
         *     "milestones":{
         *     },
         *     "experiencePoints":110,
         *     "quests":{
         *     },
         *     "experienceLevel":1,
         *     "statistics":{
         *       "minions":0,
         *       "wood":50,
         *       "pantelons":3,
         *       "iron":0
         *     }
         *   }
         * }
         */
		void incrementExperiencePoints(int in_xpValue, IServerCallback* in_callback);
		
		/**
         * Sets the player's experience to an absolute value. Note that this
         * is simply a set and will not reward the player if their level changes
         * as a result.
		 *
		 * Service Name - PlayerStatistics
	     * Service Operation - SetXpPoints
	   	 *
		 * @param in_xpValue The amount to set the the player's experience to
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows.
         * {
         *   "status":200,
         *   "data":null
         * }
         */
		void setExperiencePoints(int xpValue, IServerCallback* in_callback);
        
	protected:
	};
}
