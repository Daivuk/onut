#pragma once

#include <string>
#include <vector>

#include "IServerCallback.h"
#include "ServiceName.h"
#include "ServiceOperation.h"
#include "OperationParam.h"
#include "BrainCloudTypes.h"

namespace BrainCloud {
	
	typedef enum {
		HIGH_VALUE,
		CUMULATIVE,
        LAST_VALUE,
        LOW_VALUE
	} SocialLeaderboardType;
	
	typedef enum
	{
		NEVER,
		DAILY,
		WEEKLY,
		MONTHLY,
		YEARLY
	} RotationType;
    
    typedef enum
    {
        HIGHEST_RANKED
    } FetchType;
	
	typedef enum
	{
		HIGH_TO_LOW,
		LOW_TO_HIGH
	} SortOrder;

	class BrainCloudSocialLeaderboard
	{
	public:
		/**
         * Method returns the social leaderboard. A player's social leaderboard is
         * comprised of players who are recognized as being your friend.
         * For now, this applies solely to Facebook connected players who are
         * friends with the logged in player (who also must be Facebook connected).
         * In the future this will expand to other identification means (such as
         * Game Centre, Google circles etc).
         *
         * Leaderboards entries contain the player's score and optionally, some user-defined
         * data associated with the score. The currently logged in player will also
         * be returned in the social leaderboard.
         *
         * Note: If no friends have played the game, the bestScore, createdAt, updatedAt
         * will contain NULL.
         *
         * @param in_leaderboardId The id of the leaderboard to retrieve
         * @param in_replaceName If true, the currently logged in player's name will be replaced
         * by the string "You".
         * @param in_callback The method to be invoked when the server response is received
         *
         * @returns JSON string representing the entries in the leaderboard.
         * Note that the friend summary data is returned for each record
         * in the leaderboard.
         *
         * {
         *   "status": 200,
         *   "data": {
         *     "social_leaderboard": [
         *       {
         *         "updatedAt": 1395840936348,
         *         "pictureUrl": null,
         *         "playerId": "26f8bb07-3e94-458b-8485-f9031c3a6ef1",
         *         "createdAt": 1395840936348,
         *         "name": "You",
         *         "otherData": null,
         *         "authenticationType": null,
         *         "externalId": null,
         *         "summaryFriendData": null,
         *         "score": 20000
         *       },
         *       {
         *         "updatedAt": 1395840936351,
         *         "pictureUrl": null,
         *         "playerId": "3ad8bc09-4a34-e324-1231-3b2c1c3a6bc6",
         *         "createdAt": 1395840936351,
         *         "name": "Jenny Goldsmith",
         *         "otherData": null,
         *         "authenticationType": null,
         *         "externalId": null,
         *         "summaryFriendData": null,
         *         "score": 10000
         *       }
         *     ],
         *     "timeBeforeReset": 588182412,
         *     "server_time": 1395840957588
         *   }
         * }
         *
         */
        void getSocialLeaderboard( const char * in_leaderboardId, bool in_replaceName, IServerCallback * in_callback = NULL);
        
        /**
         * @deprecated 2.17 - use getSocialLeaderboard
         */
		void getLeaderboard( const char * in_leaderboardId, bool in_replaceName, IServerCallback * in_callback = NULL);
        
        /**
         * Reads multiple social leaderboards.
         *
         * @param in_leaderboardIds Collection of leaderboard IDs.
         * @param in_leaderboardResultCount Maximum count of entries to return for each leaderboard.
         * @param in_replaceName If true, the currently logged in player's name will be replaced
         * by the string "You".
         * @param in_callback The method to be invoked when the server response is received
         *
         * @returns JSON string representing the entries in each leaderboard.
         * Note that the friend summary data is returned for each record
         * in the leaderboard.
         *
         * {
         *   "server_time": 1445952203123,
         *   "leaderboards": [
         *    {
         *     "leaderboardId": "default",
         *     "leaderboard": [
         *      {
         *       "externalId": "pacer5",
         *       "name": "Rollo",
         *       "pictureUrl": "http://localhost:8080/gameserver/s3/portal/g/eggies/metadata/pacers/pacer5.png",
         *       "playerId": "pacer5",
         *       "authenticationType": null,
         *       "score": 100000,
         *       "data": {
         *        "pacerTag": null,
         *        "pacerLeaderboardTag": {}
         *       },
         *       "createdAt": null,
         *       "updatedAt": null
         *      },
         *      {
         *       "externalId": "pacer4",
         *       "name": "Chirp",
         *       "pictureUrl": "http://localhost:8080/gameserver/s3/portal/g/eggies/metadata/pacers/pacer4.png",
         *       "playerId": "pacer4",
         *       "authenticationType": null,
         *       "score": 80000,
         *       "data": {
         *        "pacerTag": null,
         *        "pacerLeaderboardTag": {}
         *       },
         *       "createdAt": null,
         *       "updatedAt": null
         *      }
         *     ],
         *     "self": {
         *      "externalId": null,
         *      "name": null,
         *      "pictureUrl": null,
         *      "playerId": "49390659-33bd-4812-b0c4-ab04e614ec98",
         *      "authenticationType": null,
         *      "score": 10,
         *      "data": {
         *       "nickname": "batman"
         *      },
         *      "createdAt": 1445952060607,
         *      "updatedAt": 1445952060607,
         *      "summaryFriendData": null
         *     },
         *     "selfIndex": 5
         *    }
         *   ]
         *  }
         */
        void getMultiSocialLeaderboard(const std::vector<std::string> & in_leaderboardIds, int in_leaderboardResultCount, bool in_replaceName, IServerCallback * in_callback = NULL);
        
		/**
         * Method returns the global leaderboard. 
         * 
         * Leaderboards entries contain the player's score and optionally, some user-defined 
         * data associated with the score. 
         * 
         * Note: If no leaderboard records exist then this method will return an empty list. 
         *               
		 * Service Name - SocialLeaderboard
	     * Service Operation - GetGlobalLeaderboard
	   	 *
         * @param in_leaderboardId The id of the leaderboard to retrieve
         * @param in_fetchType The type of scores to return.
         * @param in_maxResults The maximum number of scores returned. 		 
         * @param in_callback The method to be invoked when the server response is received
         * 
         * @returns JSON string representing the entries in the leaderboard.
         * Note that the friend summary data is returned for each record
         * in the leaderboard.
         *
         * {
         *  "status": 200,
         *  "data": {
         *   "leaderboardId": "abc",
         *   "social_leaderboard": [
         *    {
         *     "playerId": "8c86f306-73ea-4536-9c92-aba086064d2c",
         *     "score": 10,
         *     "data": {
         *      "nickname": "batman"
         *     },
         *     "createdAt": 1433863814394,
         *     "updatedAt": 1433863814394,
         *     "index": 0,
         *     "rank": 1,
         *     "name": "",
         *     "summaryFriendData": {
         *      "xp": 12,
         *      "favColour": "red"
         *     }
         *    },
         *    {
         *     "playerId": "ab21c0af-9d3e-4a81-b3c8-ddc1fb77d9a1",
         *     "score": 8,
         *     "data": {
         *      "nickname": "robin"
         *     },
         *     "createdAt": 1433864253221,
         *     "updatedAt": 1433864253221,
         *     "index": 1,
         *     "rank": 2,
         *     "name": "",
         *     "summaryFriendData": null
         *    }
         *   ],
         *   "timeBeforeReset": 48136284,
         *   "server_time": 1433864263716
         *  }
         * }
         */
		void getGlobalLeaderboard( const char * in_leaderboardId, FetchType in_fetchType, int in_maxResults, IServerCallback * in_callback = NULL);
		
        /**
         * Method returns a page of global leaderboard results.
         *
         * Leaderboards entries contain the player's score and optionally, some user-defined
         * data associated with the score.
         *
         * Note: This method allows the client to retrieve pages from within the global leaderboard list
         *
         * Service Name - SocialLeaderboard
         * Service Operation - GetGlobalLeaderboardPage
         *
         * @param in_leaderboardId The id of the leaderboard to retrieve.
         * @param in_sort Sort key Sort order of page.
         * @param in_startIndex The index at which to start the page.
         * @param in_endIndex The index at which to end the page.
         * @param in_includeLeaderboardSize Whether to return the leaderboard size
         * @param in_callback The method to be invoked when the server response is received
         *
         * @returns JSON string representing the entries in the leaderboard.
         * Note that the friend summary data is returned for each record
         * in the leaderboard.
         *
         * {
         *  "status": 200,
         *  "data": {
         *   "leaderboardId": "abc",
         *   "moreBefore": false,
         *   "timeBeforeReset": 48085996,
         *   "leaderboard": [
         *    {
         *     "playerId": "8c86f306-73ea-4536-9c92-aba086064d2c",
         *     "score": 10,
         *     "data": {
         *      "nickname": "batman"
         *     },
         *     "createdAt": 1433863814394,
         *     "updatedAt": 1433863814394,
         *     "index": 0,
         *     "rank": 1,
         *     "name": "",
         *     "summaryFriendData": {
         *      "xp": 12,
         *      "favColour": "red"
         *     }
         *    },
         *    {
         *     "playerId": "ab21c0af-9d3e-4a81-b3c8-ddc1fb77d9a1",
         *     "score": 8,
         *     "data": {
         *      "nickname": "robin"
         *     },
         *     "createdAt": 1433864253221,
         *     "updatedAt": 1433864253221,
         *     "index": 1,
         *     "rank": 2,
         *     "name": "",
         *     "summaryFriendData": null
         *    }
         *   ],
         *   "server_time": 1433864314004,
         *   "moreAfter": false
         *  }
         * }
		 */
		void getGlobalLeaderboardPage(const char * in_leaderboardId, SortOrder in_sortOrder, int in_startIndex, int in_endIndex, bool in_includeLeaderboardSize, IServerCallback * in_callback = NULL);

        /**
         * Method returns a page of global leaderboard results.
         * By using a non-current version id, the user can retrieve a historial leaderboard.
         * See GetGlobalLeaderboardVersions method to retrieve the version id.
         *
         * Service Name - SocialLeaderboard
         * Service Operation - GetGlobalLeaderboardPage
         *
         * @param in_leaderboardId The id of the leaderboard to retrieve.
         * @param in_sort Sort key Sort order of page.
         * @param in_startIndex The index at which to start the page.
         * @param in_endIndex The index at which to end the page.
         * @param in_includeLeaderboardSize Whether to return the leaderboard size
         * @param in_versionId The historical version to retrieve.
         * @param in_callback The method to be invoked when the server response is received
         *
         * @returns JSON string representing the entries in the leaderboard.
         * See GetGlobalLeaderboardPage documentation. Note that historial leaderboards do not
         * include the 'timeBeforeReset' parameter.
         */
        void getGlobalLeaderboardPageByVersion(const char * in_leaderboardId, SortOrder in_sortOrder, int in_startIndex, int in_endIndex, bool in_includeLeaderboardSize, int in_versionId, IServerCallback * in_callback = NULL);
        
        /**
         * Method returns a view of global leaderboard results that centers on the current player.
         *
         * Leaderboards entries contain the player's score and optionally, some user-defined
         * data associated with the score.
         *
         * Service Name - SocialLeaderboard
         * Service Operation - GetGlobalLeaderboardView
         *
         * @param in_leaderboardId The id of the leaderboard to retrieve.
         * @param in_sort Sort key Sort order of page.
         * @param in_beforeCount The count of number of players before the current player to include.
         * @param in_afterCount The count of number of players after the current player to include.
         * @param in_includeLeaderboardSize Whether to return the leaderboard size
         * @param in_callback The method to be invoked when the server response is received
         *
         * @returns JSON string representing the entries in the leaderboard.
         * Note that the friend summary data is returned for each record
         * in the leaderboard.
         *
         * {
         *  "status": 200,
         *  "data": {
         *   "leaderboardId": "abc",
         *   "moreBefore": false,
         *   "timeBeforeReset": 48085996,
         *   "leaderboard": [
         *    {
         *     "playerId": "8c86f306-73ea-4536-9c92-aba086064d2c",
         *     "score": 10,
         *     "data": {
         *      "nickname": "batman"
         *     },
         *     "createdAt": 1433863814394,
         *     "updatedAt": 1433863814394,
         *     "index": 0,
         *     "rank": 1,
         *     "name": "",
         *     "summaryFriendData": {
         *      "xp": 12,
         *      "favColour": "red"
         *     }
         *    },
         *    {
         *     "playerId": "ab21c0af-9d3e-4a81-b3c8-ddc1fb77d9a1",
         *     "score": 8,
         *     "data": {
         *      "nickname": "robin"
         *     },
         *     "createdAt": 1433864253221,
         *     "updatedAt": 1433864253221,
         *     "index": 1,
         *     "rank": 2,
         *     "name": "",
         *     "summaryFriendData": null
         *    }
         *   ],
         *   "server_time": 1433864314004,
         *   "moreAfter": false
         *  }
         * }
         */
		void getGlobalLeaderboardView(const char * in_leaderboardId, SortOrder in_sortOrder, int in_beforeCount, int in_afterCount, bool in_includeLeaderboardSize, IServerCallback * in_callback = NULL);

        /**
         * Method returns a view of global leaderboard results that centers on the current player.
         * By using a non-current version id, the user can retrieve a historial leaderboard.
         * See GetGlobalLeaderboardVersions method to retrieve the version id.
         *
         * Service Name - SocialLeaderboard
         * Service Operation - GetGlobalLeaderboardView
         *
         * @param in_leaderboardId The id of the leaderboard to retrieve.
         * @param in_sort Sort key Sort order of page.
         * @param in_beforeCount The count of number of players before the current player to include.
         * @param in_afterCount The count of number of players after the current player to include.
         * @param in_includeLeaderboardSize Whether to return the leaderboard size
         * @param in_versionId The historical version to retrieve.
         * @param in_callback The method to be invoked when the server response is received
         *
         * @returns JSON string representing the entries in the leaderboard.
         * See GetGlobalLeaderboardView documentation. Note that historial leaderboards do not
         * include the 'timeBeforeReset' parameter.
         */
        void getGlobalLeaderboardViewByVersion(const char * in_leaderboardId, SortOrder in_sortOrder, int in_beforeCount, int in_afterCount, bool in_includeLeaderboardSize, int in_versionId, IServerCallback * in_callback = NULL);
        
        /** Gets the global leaderboard versions.
         *
         * Service Name - SocialLeaderboard
         * Service Operation - GetGlobalLeaderboardVersions
         *
         * @param in_leaderboardId The leaderboard
         * @param in_callback The method to be invoked when the server response is received
         *
         * @returns JSON string representing the leaderboard versions.
         * {
         *   "status": 200,
         *   "data": {
         *     "leaderboardId":"L1",
         *     "leaderboardType":"HIGH_VALUE",
         *     "rotationType":"WEEKLY",
         *     "retainedCount":2,
         *     "versions":[
         *        {
         *         "versionId":27,
         *         "startingAt":1434499200000,
         *         "endingAt":1435104000000
         *        },
         *        {
         *         "versionId":26,
         *         "startingAt":1433894400000,
         *         "endingAt":1434499200000
         *        }
         *        ]
         *     }
         *   }
         * }
         */
        void getGlobalLeaderboardVersions(const char * in_leaderboardId, IServerCallback * in_callback = NULL);
        
		/**
         * Post the players score to the given social leaderboard.
         * You can optionally send a user-defined json string of data
         * with the posted score. This string could include information
         * relevant to the posted score.
         *
         * Note that the behaviour of posting a score can be modified in
         * the brainCloud portal. By default, the server will only keep
         * the player's best score.
         *
         * @param in_leaderboardId The leaderboard to post to
         * @param in_score The score to post
         * @param in_data Optional user-defined data to post with the score
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
		 * {
		 *   "status": 200,
		 *   "data": null
		 * }
		 */
		void postScoreToLeaderboard( const char * in_leaderboardId, int64_t in_score, const std::string& in_jsonOtherData, IServerCallback * in_callback = NULL);
		
		/**
         * Post the players score to the given social leaderboard.
         * Pass leaderboard config data to dynamically create if necessary.
         * You can optionally send a user-defined json string of data
         * with the posted score. This string could include information
         * relevant to the posted score.
         *
		 * Service Name - SocialLeaderboard
	     * Service Operation - PostScoreDynamic
	   	 *
         * @param in_leaderboardId The leaderboard to post to
         * @param in_score The score to post
         * @param in_data Optional user-defined data to post with the score
         * @param in_leaderboardType leaderboard type
         * @param in_rotationType Type of rotation
         * @param in_rotationReset Date to start rotation calculations
         * @param in_retainedCount How many rotations to keep
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data": null
         * }
         */
		void postScoreToDynamicLeaderboard ( const char * in_leaderboardId, int64_t in_score, const std::string& in_jsonData, SocialLeaderboardType in_leaderboardType, RotationType in_rotationType, const struct tm* in_rotationReset, int in_retainedCount, IServerCallback * in_callback = NULL);
		
		/**
         * Reset the player's score for the given social leaderboard id.
         *
         * @param in_leaderboardId The leaderboard to post to
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data": null
         * }
         */
        void resetLeaderboardScore( const char * in_leaderBoardName, IServerCallback * in_callback = NULL);
		
		/**
         * If a social leaderboard has been configured to reset periodically, each period
         * can be considered to be a tournament. When the leaderboard resets, the tournament
         * has ended and participants can be ranked based on their final scores.
         *
         * This API method will return the sorted leaderboard including:
         * the player
         * the game's pacers
         * all friends who participated in the tournament
         *
         * This API method will return the leaderboard results for a particular
         * tournament only once. If the method is called twice, the second call
         * will yield an empty result.
         *
         * Note that if the leaderboard has not been configured to reset, the concept of a
         * tournament does not apply.
         *
         * @param in_leaderboardId The id of the leaderboard
         * @param in_replaceName True if the player's name should be replaced with "You"
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data":{
         *   }
         * }
         */
		void getCompletedLeaderboardTournament(const char * in_leaderboardId, bool in_replaceName, IServerCallback * in_callback = NULL);
		
		/**
         * This method triggers a reward (via a player statistics event)
         * to the currently logged in player for ranking at the
         * completion of a tournament.
         *
         * @param in_leaderboardId The id of the leaderboard
         * @param in_eventName The player statistics event name to trigger
         * @param in_eventMultiplier The multiplier to associate with the event
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data":{
         *   }
         * }
         */
		void triggerSocialLeaderboardTournamentReward(const char * in_leaderboardId, const char * in_eventName, uint64_t in_eventMultiplier, IServerCallback * in_callback = NULL);
		
		/**
		 * Method to retrieve the player's completed tournaments for the game.
		 * Arguments:
		 * 	leaderboardId: Name of the leaderboard
		 *  replaceName: True to replace the player's name with "You"; false otherwise.
		 * 	callback: client-side callback for the results
		 */
		void playerTournamentReward( const char * in_eventName, uint64_t in_multiplier, IServerCallback * in_callback = NULL);
		
	private:
		std::string leaderboardTypeToString(SocialLeaderboardType type);
		std::string leaderboardRotationTypeToString(RotationType type);
        std::string sortOrderToString(SortOrder in_sortOrder);

	};
}