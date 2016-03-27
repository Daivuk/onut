#pragma once

#include <string>

#include "IServerCallback.h"
#include "ServiceName.h"
#include "ServiceOperation.h"
#include "OperationParam.h"

namespace BrainCloud {
	
	class BrainCloudFriend
	{
	public:

        /**
         * Retrieves profile information for the specified user.
         *
         * Service Name - Friend
         * Service Operation - GetFriendProfileInfoForExternalId
         *
         * @param in_externalId The friend's external id e.g. Facebook id
         * @param in_authenticationType The authentication type of the friend id e.g. Facebook
         *
         * @return The JSON returned in the callback
         * {
         *   "status":200,
         *   "data": {
         *     "playerId" : "17c7ee96-1b73-43d0-8817-cba1953bbf57",
         *     "playerName" : "Donald Trump",
         *     "email" : "donald@trumpcastle.com",
         *     "playerSummaryData" : {},
         *   }
         * }
         */
        void getFriendProfileInfoForExternalId(const char *in_externalId, const char *in_authenticationType, IServerCallback * in_callback = NULL);

        /**
         * Retrieves the external ID for the specified user profile ID on the specified social platform.
         *
         * @param in_profileId Profile (player) ID.
         * @param in_authenticationType Associated authentication type.
         *
         * @return The JSON returned in the callback
         * {
         *    "status": 200,
         *    "data": {
         *        "externalId": "19e1c0cf-9a2d-4d5c-9a71-1b0f6asdfb4b"
         *    }
         * }
         */
        void getExternalIdForProfileId(const char * in_profileId, const char * in_authenticationType, IServerCallback * in_callback = NULL);
        
		/**
		* Returns a particular entity of a particular friend.
		*
		* Service Name - Friend
		* Service Operation - ReadFriendEntity
		*
		* @param in_entityId Id of entity to retrieve.
		* @param in_friendId Profile Id of friend who owns entity.
		* @param in_callback Method to be invoked when the server response is received.
		*
		* @return The JSON returned in the callback
		*/
		void readFriendEntity(const char* in_entityId, const char*  in_friendId, IServerCallback * in_callback = NULL);

		/**
		* Returns entities of all friends optionally based on type.
		*
		* Service Name - Friend
		* Service Operation - ReadFriendsEntities
		*
		* @param in_entityType Types of entities to retrieve.
		* @param in_callback Method to be invoked when the server response is received.
		*
		* @return The JSON returned in the callback
		*/
		void readFriendsEntities(const char* in_entityType, IServerCallback * in_callback = NULL);

		/**
		* Returns list of friends with optional summary data.
		*
		* Service Name - Friend
		* Service Operation - ReadFriendsWithApplication
		*
		* @param in_includeSummaryData Whether to include summary data
		* @param in_callback Method to be invoked when the server response is received.
		*
		* @return The JSON returned in the callback
		*/
		void readFriendsWithApplication(bool in_includeSummaryData = false, IServerCallback * in_callback = NULL);

		/**
         * Read a friend's player state.
         *
		 * Service Name - PlayerState
	     * Service Operation - ReadFriendsPlayerState
	   	 *
         * @param in_friendId Target friend
         * @param in_callback Method to be invoked when the server response is received.
         */
        void readFriendPlayerState( const char * in_friendId, IServerCallback * in_callback = NULL);
        
		/**
         * @deprecated - use BrainCloudPlayerState.updateSummaryFriendData
         *
         * Updates the "friend summary data" associated with the logged in player.
         * Some operations will return this summary data. For instance the social
         * leaderboards will return the player's score in the leaderboard along
         * with the friend summary data. Generally this data is used to provide
         * a quick overview of the player without requiring a separate API call
         * to read their public stats or entity data.
         *
		 * Service Name - PlayerState
	     * Service Operation - UpdateSummary
	   	 *
         * @param in_jsonSummaryData A JSON string defining the summary data.
         * For example:
         * {
         *   "xp":123,
         *   "level":12,
         *   "highScore":45123
         * }
         * @param in_callback Method to be invoked when the server response is received.
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data":null
         * }
         */
		void updateSummaryFriendData(const char * in_jsonSummaryData,  IServerCallback * in_callback = NULL );
        
        /**
         * Finds a list of players matching the search text by performing a substring
         * search of all player names.
         * If the number of results exceeds maxResults the message
         * "Too many results to return." is received and no players are returned
         *
         * Service Name - Friend
         * Service Operation - FindPlayerByName
         *
         * @param in_searchText The substring to search for. Minimum length of 3 characters.
         * @param in_maxResults  Maximum number of results to return. If there are more the message
         *                       "Too many results to return." is sent back instead of the players.
         * @param in_callback Method to be invoked when the server response is received.
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {
         *         "matches": [
         *         {
         *             "profileId": "63d1fdbd-2971-4791-a248-f8cda1a79bba",
         *             "playerSummaryData": null,
         *             "profileName": "ABC"
         *         }
         *         ],
         *         "matchedCount": 1
         *     }
         * }
         */
        void findPlayerByName( const char * in_searchText, int in_maxResults, IServerCallback * in_callback = NULL);
	};
}