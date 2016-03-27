#pragma once

#include "BrainCloudTypes.h"
#include "ServiceName.h"
#include "ServiceOperation.h"

#include <string>
#include <vector>

namespace BrainCloud {

	class IServerCallback;
	class BrainCloudClient;

	class BrainCloudGamification {

	public:
		BrainCloudGamification();
		
		/**
         * Method retrieves all gamification data for the player.
         *  
		 * Service Name - Gamification
	     * Service Operation - Read
	   	 *
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return The JSON returned in the callback is as follows:
         * 
         * {
         *  "status": 200,
         *  "data": {
         *   "milestones": [
         *    {
         *     "id": "milestone02",
         *     "category": "general",
         *     "title": "Level 2 milestone",
         *     "status": "SATISFIED",
         *     "description": "Awarded when you get to level 2",
         *     "gameId": "10068",
         *     "rewards": {
         *      "currency": {
         *       "gold": 1000
         *      }
         *     },
         *     "extraData": null,
         *     "questId": null,
         *     "milestoneId": "milestone02"
         *    },
         *    {
         *     "id": "milestone01",
         *     "thresholds": {
         *      "playerStatistics": {
         *       "experiencePoints": 0
         *      }
         *     },
         *     "category": "general",
         *     "title": "Level 1 milestone",
         *     "status": "SATISFIED",
         *     "description": "Awarded when you get to player level 1",
         *     "gameId": "10068",
         *     "rewards": {
         *      "currency": {
         *       "gems": 10
         *      }
         *     },
         *     "extraData": null,
         *     "questId": null,
         *     "milestoneId": "milestone01"
         *    }
         *   ],
         *   "achievements": [
         *    {
         *     "fbEnabled": true,
         *     "imageUrl": null,
         *     "status": "NOT_AWARDED",
         *     "gameId": "10068",
         *     "steamEnabled": false,
         *     "extraData": null,
         *     "achievementId": "ach01",
         *     "invisibleUntilEarned": false,
         *     "steamAchievementId": "",
         *     "id": "ach01",
         *     "appleEnabled": false,
         *     "title": "Finish Tutorial",
         *     "fbGamePoints": 10,
         *     "description": "Achievement awarded when you finish the tutorial",
         *     "appleAchievementId": ""
         *    },
         *    {
         *     "fbEnabled": true,
         *     "imageUrl": null,
         *     "status": "NOT_AWARDED",
         *     "gameId": "10068",
         *     "steamEnabled": false,
         *     "extraData": null,
         *     "achievementId": "ach02",
         *     "invisibleUntilEarned": false,
         *     "steamAchievementId": "",
         *     "id": "ach02",
         *     "appleEnabled": false,
         *     "title": "Level up",
         *     "fbGamePoints": 10,
         *     "description": "Awarded when you level up for the first time!",
         *     "appleAchievementId": ""
         *    }
         *   ],
         *   "quests": [],
         *   "xp": {
         *    "xpCapped": false,
         *    "experiencePoints": 0,
         *    "xpLevel": {
         *     "gameId": "10068",
         *     "level": 0,
         *     "statusTitle": "Lesser",
         *     "experience": 0,
         *     "fbAction": ""
         *    },
         *    "experienceLevel": 0
         *   }
         *  }
         * }
         */
		void readAllGamification(bool in_includeMetaData = false, IServerCallback * in_callback = NULL );
		
		/**
         * Method retrieves all milestones defined for the game.
         *   
		 * Service Name - Gamification
	     * Service Operation - ReadMilestones
	   	 *
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data": {
         *     "milestones": [
         *       {
         *         "gameId": "com.bitheads.unityexample",
         *         "milestoneId": "milestone01",
         *         "playerStatistics": {
         *           "experiencePoints": null,
         *           "experienceLevel": null,
         *           "empty": true,
         *           "statistics": {}
         *         },
         *         "globalStatistics": {
         *           "statistics": {},
         *           "empty": true
         *         },
         *         "playerStatisticsUnlockThresholds": {
         *           "experiencePoints": null,
         *           "experienceLevel": null,
         *           "empty": true,
         *           "statistics": {}
         *         },
         *         "globalStatisticsUnlockThresholds": {
         *           "statistics": {},
         *           "empty": true
         *         },
         *         "reward": {
         *           "experiencePoints": null,
         *           "playerStatistics": null,
         *           "currencies": {
         *             "gems": 10
         *           },
         *           "globalGameStatistics": null,
         *           "achievement": null
         *         },
         *         "title": "Level 1 milestone",
         *         "extraData": null,
         *         "description": "Awarded when you get to player level 1",
         *         "category": "general",
         *         "key": {
         *           "gameId": "com.bitheads.unityexample",
         *           "milestoneId": "milestone01",
         *           "primaryKey": true
         *         }
         *       }
         *     ]
         *   }
         * }
         */
		void readMilestones(bool in_includeMetaData = false, IServerCallback * in_callback = NULL);
		
		/**
         * Read all of the achievements defined for the game.
         *   
		 * Service Name - Gamification
	     * Service Operation - ReadAchievements
	   	 *
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data": {
         *     "achievements": [
         *       {
         *         "gameId": "com.bitheads.unityexample",
         *         "achievementId": "ach01",
         *         "facebookUrl": "http://someurl.com",
         *         "title": "Finish Tutorial",
         *         "imageUrl": "http://someurl.com",
         *         "facebookGamePoints": 10,
         *         "extraData": null,
         *         "invisibleUntilEarned": null,
         *         "description": "Achievement awarded when you finish the tutorial",
         *         "key": {
         *           "gameId": "com.bitheads.unityexample",
         *           "achievementId": "ach01",
         *           "primaryKey": true
         *         }
         *       },
         *       {
         *         "gameId": "com.bitheads.unityexample",
         *         "achievementId": "ach02",
         *         "facebookUrl": "http://someurl.com",
         *         "title": "Level up",
         *         "imageUrl": "http://someurl.com",
         *         "facebookGamePoints": 10,
         *         "extraData": null,
         *         "invisibleUntilEarned": null,
         *         "description": "Awarded when you level up for the first time.",
         *         "key": {
         *           "gameId": "com.bitheads.unityexample",
         *           "achievementId": "ach02",
         *           "primaryKey": true
         *         }
         *       }
         *     ]
         *   }
         * }
         */
		void readAchievements(bool in_includeMetaData = false, IServerCallback * in_callback = NULL);
		
		/**
         * Method returns all defined xp levels and any rewards associated
         * with those xp levels.
         *   
		 * Service Name - Gamification
	     * Service Operation - ReadXpLevels
	   	 *
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return The JSON returned in the callback is as follows:
         * {
         *  "status": 200,
         *  "data": {
         *   "xp_levels": [
         *    {
         *     "level": 1,
         *     "statusTitle": "Peon",
         *     "experience": 0,
         *     "fbAction": ""
         *    },
         *    {
         *     "level": 2,
         *     "statusTitle": "Small Fry",
         *     "experience": 1000,
         *     "fbAction": "",
         *     "reward": {
         *      "currency": {
         *       "gold": 500
         *      }
         *     }
         *    }
         *   ]
         *  }
         * }
         */
		void readXpLevelsMetaData( IServerCallback * in_callback = NULL );
		
		 /**
         * Method retrives the list of achieved achievements.
         *   
		 * Service Name - Gamification
	     * Service Operation - ReadAchievedAchievements
	   	 *
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return The JSON returned in the callback is as follows:
         *  {
         *   "status": 200,
         *   "data": {
         *     "achievements": []
         *   }
         * }
         */
		void readAchievedAchievements(bool in_includeMetaData = false, IServerCallback * in_callback = NULL);
		
		/**
         * Method retrieves the list of completed milestones.
         *   
		 * Service Name - Gamification
	     * Service Operation - ReadCompleteMilestones
	   	 *
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data": {
         *     "milestones": []
         *   }
         * }
         */
		void readCompletedMilestones(bool in_includeMetaData = false, IServerCallback * in_callback = NULL);
		
		/**
         * Method retrieves the list of in progress milestones
         *   
		 * Service Name - Gamification
	     * Service Operation - ReadInProgressMilestones
	   	 *
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return The JSON returned in the callback is as follows:
         *  {
         *   "status": 200,
         *   "data": {
         *     "milestones": []
         *   }
         * }
         */
		void readInProgressMilestones(bool in_includeMetaData = false, IServerCallback * in_callback = NULL);
		
		 /**
         * Method retrieves milestones of the given category.
         *   
		 * Service Name - Gamification
	     * Service Operation - ReadMilestonesByCategory
	   	 *
         * @param in_category The milestone category
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data":{
         *     "milestones": []
         *   }
         * }
         */
		void readMilestonesByCategory(const char * in_category, bool in_includeMetaData = false, IServerCallback * in_callback = NULL);

		 /**
         * Method will award the achievements specified.
         *   
		 * Service Name - Gamification
	     * Service Operation - AwardAchievements
	   	 *
         * @param in_achievementIds Collection of achievement ids to award
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data":null
         * }
         */
        void awardAchievements(const std::vector<std::string> & in_achievements, IServerCallback * in_callback = NULL);
        
        //Removal after March 04 2016
        DEPRECATED void awardAchievements(const char * in_achievements, IServerCallback * in_callback = NULL);
		
		/**
		* Resets the specified milestones' statuses to LOCKED.
		*
		* Service Name - Gamification
		* Service Operation - ResetMilestones
		*
		* @param in_milestoneIds Collection of milestones to reset
		* @param in_callback Method to be invoked when the server response is received.
		*
		* @return The JSON returned in the callback is as follows.
		* {
		*   "status":200,
		*   "data":null
		* }
		*/
        void resetMilestones(const std::vector<std::string> & in_milestoneIds, IServerCallback * in_callback = NULL);
        
        //Removal after March 04 2016
        DEPRECATED void resetMilestones(const char * in_milestoneIds, IServerCallback * in_callback = NULL);
		
		/**
         * Method retrieves all of the quests defined for the game.
         * 
		 * Service Name - Gamification
	     * Service Operation - ReadQuests
	   	 *
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data":null
         * }
         * 
         * 
         *  {
         *   "status": 200,
         *   "data": {
         *     "quests": []
         *   }
         * }
         */
		void readQuests(bool in_includeMetaData = false, IServerCallback * callback = NULL);
		
		/**
         *  Method returns all completed quests.
         *   
		 * Service Name - Gamification
	     * Service Operation - ReadCompletedQuests
	   	 *
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return The JSON returned in the callback is as follows:
         *  {
         *   "status": 200,
         *   "data": {
         *     "quests": []
         *   }
         * }
         */
		void readCompletedQuests(bool in_includeMetaData = false, IServerCallback * callback = NULL);
		
		/**
         *  Method returns quests that are in progress.
         *   
		 * Service Name - Gamification
	     * Service Operation - ReadInProgressQuests
	   	 *
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return The JSON returned in the callback is as follows:
         *  {
         *   "status": 200,
         *   "data": {
         *     "quests": []
         *   }
         * }
         */
		void readInProgressQuests(bool in_includeMetaData = false, IServerCallback * callback = NULL);
		
		/**
         *  Method returns quests that have not been started.
         *   
		 * Service Name - Gamification
	     * Service Operation - ReadNotStartedQuests
	   	 *
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return The JSON returned in the callback is as follows:
         *  {
         *   "status": 200,
         *   "data": {
         *     "quests": []
         *   }
         * }
         */
		void readNotStartedQuests(bool in_includeMetaData = false, IServerCallback * callback = NULL);
		
		/**
         *  Method returns quests with a status.
         *   
		 * Service Name - Gamification
	     * Service Operation - ReadQuestsWithStatus
	   	 *
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return The JSON returned in the callback is as follows:
         *  {
         *   "status": 200,
         *   "data": {
         *     "quests": []
         *   }
         * }
         */
		void readQuestsWithStatus(bool in_includeMetaData = false, IServerCallback * callback = NULL);
		
		/**
         *  Method returns quests with a basic percentage.
         *   
		 * Service Name - Gamification
	     * Service Operation - ReadQuestsWithBasicPercentage
	   	 *
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return The JSON returned in the callback is as follows:
         *  {
         *   "status": 200,
         *   "data": {
         *     "quests": []
         *   }
         * }
         */
		void readQuestsWithBasicPercentage(bool in_includeMetaData = false, IServerCallback * callback = NULL);
		
		/**
         *  Method returns quests with a complex percentage.
         *   
		 * Service Name - Gamification
	     * Service Operation - ReadQuestsWithComplexPercentage
	   	 *
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return The JSON returned in the callback is as follows:
         *  {
         *   "status": 200,
         *   "data": {
         *     "quests": []
         *   }
         * }
         */
		void readQuestsWithComplexPercentage(bool in_includeMetaData = false, IServerCallback * callback = NULL);
		
		/**
         * Method returns quests for the given category.
         *   
		 * Service Name - Gamification
	     * Service Operation - ReadQuestsByCategory  
	   	 *
         * @param in_category The quest category
         * @param in_callback Method to be invoked when the server response is received.
         * 
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
		 *   "data": {
		 *     "quests": []
		 *   }
         * }
         */
		void readQuestsByCategory(const char * category, bool in_includeMetaData = false, IServerCallback * callback = NULL);

	};
}