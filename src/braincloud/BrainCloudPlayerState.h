#pragma once

#include <string>
#include <vector>

#include "IServerCallback.h"
#include "ServiceName.h"
#include "ServiceOperation.h"
#include "OperationParam.h"
#include "BrainCloudTypes.h"

namespace BrainCloud {
	
	class BrainCloudPlayerState
	{
	public:
		/**
         * Read the state of the currently logged in player.
         * This method returns a JSON object describing most of the
         * player's data: entities, statistics, level, currency.
         * Apps will typically call this method after authenticating to get an
         * up-to-date view of the player's data.
         *
		 * Service Name - PlayerState
	     * Service Operation - Read
	   	 *
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data": {
         *     "vcPurchased": null,
         *     "id": "210ee817-d555-40c3-b109-c24a84c84dc7",
         *     "experiencePoints": 10,
         *     "sent_events": [],
         *     "vcClaimed": null,
         *     "server_time": 1395950294285,
         *     "experienceLevel": 1,
         *     "incoming_events": [],
         *     "currency": {
         *       "gems": {
         *         "purchased": 0,
         *         "balance": 0,
         *         "consumed": 0,
         *         "awarded": 0
         *       },
         *       "gold": {
         *         "purchased": 0,
         *         "balance": 0,
         *         "consumed": 0,
         *         "awarded": 0
         *       }
         *     },
         *     "statistics": {
         *       "minions": 0,
         *       "wood": 50,
         *       "pantelons": 3,
         *       "iron": 0
         *     },
         *     "abTestingId": 60
         *   }
         * }
         */
        void readPlayerState(IServerCallback * in_callback, const char* in_entityTypeFilter = NULL);
		
		/**
         * Completely deletes the player record and all data fully owned
         * by the player. After calling this method, the player will need
         * to re-authenticate and create a new profile.
         * This is mostly used for debugging/qa.
         *
		 * Service Name - PlayerState
	     * Service Operation - FullReset
	   	 *
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data":null
         * }
         */
		void deletePlayer (IServerCallback * in_callback = NULL);
		
        /**
         * This method will delete *most* data for the currently logged in player.
         * Data which is not deleted includes: currency, credentials, and
         * purchase transactions. ResetPlayer is different from DeletePlayer in that
         * the player record will continue to exist after the reset (so the user
         * does not need to re-authenticate).
         *
		 * Service Name - PlayerState
	     * Service Operation - DataReset
	   	 *
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data":null
         * }
         */
        void resetPlayerState(IServerCallback * in_callback = NULL);
		
		/**
         * Logs player out of server.
         *
		 * Service Name - PlayerState
	     * Service Operation - Logout
	   	 *
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data":null
         * }
         */
		void logout(IServerCallback * in_callback = NULL);
		
		/**
		* Sets the players name.
		*
		* Service Name - playerState
		* Service Operation - UPDATE_NAME
		*
		* @param in_playerName The name of the player
		* @param in_callback The method to be invoked when the server response is received
		*
		* @return The JSON returned in the callback is as follows:
		* {
		*   "status":200,
		*   "data":
		*   {
		*     "playerName": "someName"
		*   }
		* }
		*/
		void updatePlayerName(const char * in_playerName, IServerCallback * in_callback = NULL);

        /**
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
		* Retrieve the player attributes.
		*
		* Service Name - PlayerState
		* Service Operation - GetAttributes
		*
		* @param in_callback The method to be invoked when the server response is received
		*
		* @return The JSON returned in the callback is as follows:
		* {
		*   "status": 200,
		*   "data": {
		*     "attributes": {
		*          "key1": "value1",
		*          "key2": "value2"
		*     }
		*   }
		* }
		*/
		void getAttributes(IServerCallback * in_callback = NULL);

		/**
		* Update player attributes.
		*
		* Service Name - PlayerState
		* Service Operation - UpdateAttributes
		*
		* @param in_jsonAttributes Single layer json string that is a set of key-value pairs
		* @param in_wipeExisting Whether to wipe existing attributes prior to update.
		* @param in_callback The method to be invoked when the server response is received
		*
		* @return The JSON returned in the callback is as follows:
		* {
		*   "status": 200,
		* }
		*/
		void updateAttributes(const std::string& in_jsonAttributes, bool in_wipeExisting, IServerCallback * in_callback = NULL);

		/**
		* Remove player attributes.
		*
		* Service Name - PlayerState
		* Service Operation - RemoveAttributes
		*
		* @param in_attributeNames Collection of attribute names.
		* @param in_callback The method to be invoked when the server response is received
		*
		* @return The JSON returned in the callback is as follows:
		* {
		*   "status": 200,
		* }
		*/
        void removeAttributes(const std::vector<std::string> & in_attributeNames, IServerCallback * in_callback = NULL);
        
        /**
         * Update Player picture URL.
         *
         * Service Name - PlayerState
         * Service Operation - UPDATE_PICTURE_URL
         *
         * @param in_pictureUrl URL to apply
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {
         *         "playerPictureUrl": "https://some.domain.com/mypicture.jpg"
         *      }
         * }
         */
        void updatePlayerPictureUrl(const char * in_pictureUrl, IServerCallback * in_callback = NULL);
        
        /**
         * Update the player's contact email. 
         * Note this is unrelated to email authentication.
         *
         * Service Name - PlayerState
         * Service Operation - UPDATE_CONTACT_EMAIL
         *
         * @param in_contactEmail Updated email
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {
         *         "contactEmail": "someName@somedomain.com"
         *     }
         * }
         */
        void updateContactEmail(const char * in_contactEmail, IServerCallback * in_callback = NULL);
        
        //Removal after March 04 2016
        DEPRECATED void removeAttributes(const std::string& in_jsonAttributeNameList, IServerCallback * in_callback = NULL);

	private:

	};
}