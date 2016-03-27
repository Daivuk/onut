#pragma once

#include <string>
#include <vector>

#include "BrainCloudTypes.h"
#include "IServerCallback.h"

namespace BrainCloud
{
	class BrainCloudGlobalStatistics
	{
	public:
		/**
         * Method returns all of the global statistics.
         *   
		 * Service Name - GlobalStatistics
	     * Service Operation - Read
	   	 *
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return JSON describing the global statistics:
         * {
         *   "status":200,   
         *   "data":{      
         *     "statisticsExceptions":{      
         *     },      
         *     "statistics":{         
         *       "Level02_TimesBeaten":11,         
         *       "Level01_TimesBeaten":1,         
         *       "GameLogins":376,         
         *       "PlayersWhoLikePirateClothing":12      
         *     }   
         *   }
         * }
         */
		void readAllGlobalStats(IServerCallback * in_callback = NULL);
		
		/**
         * Reads a subset of global statistics as defined by the input collection.
         *    
		 * Service Name - GlobalStatistics
	     * Service Operation - ReadSubset
	   	 *
         * @param in_statistics A collection containing the statistics to read:
         * [ 
         *   "Level01_TimesBeaten",
         *   "Level02_TimesBeaten"
         * ]
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return JSON with the subset of global statistics:
         * {
         *   "status":200,   
         *   "data":{      
         *     "statisticsExceptions":{      
         *     },      
         *     "statistics":{         
         *       "Level02_TimesBeaten":11,         
         *       "Level01_TimesBeaten":1
         *     }   
         *   }
         * }
         */
        void readGlobalStatsSubset(const std::vector<std::string> & in_statistics, IServerCallback * in_callback = NULL);
        
        //Removal after March 04 2016
		DEPRECATED void readGlobalStatsSubset(const std::string& in_jsonData, IServerCallback * in_callback = NULL);
		
        /**
         * Method retrieves the global statistics for the given category.
         *
         * Service Name - GlobalStatistics
         * Service Operation - READ_FOR_CATEGORY
         *
         * @param in_category The global statistics category
         * @param in_callback Method to be invoked when the server response is received.
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data":{
         *     "gameStatistics": []
         *   }
         * }
         */
        void readGlobalStatsForCategory(const char * in_category, IServerCallback * in_callback = NULL);
        
		/**
         * Atomically increment (or decrement) global statistics.
         * Global statistics are defined through the brainCloud portal.
         *   
		 * Service Name - GlobalStatistics
	     * Service Operation - UpdateIncrement
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
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return JSON describing the new value of the statistics incremented (similar to ReadAll):
         * {
         *   "status":200,
         *   "data":{      
         *     "statisticsExceptions":{      
         *     },      
         *     "statistics":{         
         *       "Level02_TimesBeaten":11,
         *     }   
         *   }
         * }
         */
		void incrementGlobalGameStat(const std::string& in_jsonData, IServerCallback * in_callback = NULL);
	};
}