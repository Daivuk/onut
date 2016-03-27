#pragma once

#include <string>

#include "BrainCloudTypes.h"
#include "IServerCallback.h"

namespace BrainCloud {
    
    class BrainCloudMatchmaking
    {
    public:
        /**
         * Read match making record
         *
         * Service Name - MatchMaking
         * Service Operation - Read
         *
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {
         *         "lastMatch": 0,
         *         "matchAttackExpiry": 0,
         *         "matchEnabled": false,
         *         "sharedAPIToken": null,
         *         "shieldExpiry": 0,
         *         "playerRating": 100,
         *         "matchesPlayed": 0
         *     }
         * }
         */
        void read(IServerCallback * in_callback = NULL);
        
        /**
         * Sets player rating
         *
         * Service Name - MatchMaking
         * Service Operation - SetPlayerRating
         *
         * @param in_playerRating The new player rating.
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data": null
         * }
         */
        void setPlayerRating(int32_t in_playerRating, IServerCallback * in_callback = NULL);
        
        /**
         * Resets player rating
         *
         * Service Name - MatchMaking
         * Service Operation - ResetPlayerRating
         *
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data": null
         * }
         */
        void resetPlayerRating(IServerCallback * in_callback = NULL);
        
        /**
         * Increments player rating
         *
         * Service Name - MatchMaking
         * Service Operation - IncrementPlayerRating
         *
         * @param in_increment The increment amount
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data": null
         * }
         */
        void incrementPlayerRating(int32_t in_increment, IServerCallback * in_callback = NULL);
        
        /**
         * Decrements player rating
         *
         * Service Name - MatchMaking
         * Service Operation - DecrementPlayerRating
         *
         * @param in_decrement The decrement amount
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data": null
         * }
         */
        void decrementPlayerRating(int32_t in_decrement, IServerCallback * in_callback = NULL);
        
        /**
         * Turns shield on
         *
         * Service Name - MatchMaking
         * Service Operation - ShieldOn
         *
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data": null
         * }
         */
        void turnShieldOn(IServerCallback * in_callback = NULL);
        
        /**
         * Turns shield on for the specified number of minutes
         *
         * Service Name - MatchMaking
         * Service Operation - ShieldOnFor
         *
         * @param in_minutes Number of minutes to turn the shield on for
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data": null
         * }
         */
        void turnShieldOnFor(int32_t in_minutes, IServerCallback * in_callback = NULL);
        
        /**
         * Turns shield off
         *
         * Service Name - MatchMaking
         * Service Operation - ShieldOff
         *
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data": null
         * }
         */
        void turnShieldOff(IServerCallback * in_callback = NULL);
        
        /**
         * Gets the shield expiry for the given player id. Passing in a null player id
         * will return the shield expiry for the current player. The value returned is
         * the time in UTC millis when the shield will expire.
         *
         * Service Name - MatchMaking
         * Service Operation - GetShieldExpiry
         *
         * @param in_playerId The player id or use null to retrieve for the current player
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data": {
         *     "shieldExpiry": 1433259734956
         *   }
         * }
         */
        void getShieldExpiry(const char * in_playerId, IServerCallback * in_callback = NULL);
        
        /**
         * Finds matchmaking enabled players
         *
         * Service Name - MatchMaking
         * Service Operation - FindPlayers
         *
         * @param in_rangeDelta The range delta
         * @param in_numMatches The maximum number of matches to return
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data": {
         *     "matchesFound" : [
         *       {
         *         "playerId" : "9073dff7-0df6-437e-9be6-39cd704dcoj4",
         *         "playerName" : "Jane Smith",
         *         "playerRating" : 25,
         *         "pictureUrl" : "",
         *         "summaryFriendData" : null
         *       },
         *       {
         *         "playerId" : "9073dff7-0df6-437e-9be6-39cd704dcoj4",
         *         "playerName" : "John Smith",
         *         "playerRating" : 30,
         *         "pictureUrl" : "",
         *         "summaryFriendData" : null
         *       }
         *     ]
         *   }
         * }
         */
        void findPlayers(int32_t in_rangeDelta, int32_t in_numMatches, IServerCallback * in_callback = NULL);
        
        DEPRECATED void getOneWayPlayers(int32_t in_rangeDelta, int32_t in_numMatches, IServerCallback * in_callback = NULL);
        
        /**
         * Finds matchmaking enabled players
         *
         * Service Name - MatchMaking
         * Service Operation - FindPlayersUsingFilter
         *
         * @param in_rangeDelta The range delta
         * @param in_numMatches The maximum number of matches to return
         * @param in_jsonExtraParms Other parameters
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data": {
         *     "matchesFound" : [
         *       {
         *         "playerId" : "9073dff7-0df6-437e-9be6-39cd704dcoj4",
         *         "playerName" : "Jane Smith",
         *         "playerRating" : 25,
         *         "pictureUrl" : "",
         *         "summaryFriendData" : null
         *       },
         *       {
         *         "playerId" : "9073dff7-0df6-437e-9be6-39cd704dcoj4",
         *         "playerName" : "John Smith",
         *         "playerRating" : 30,
         *         "pictureUrl" : "",
         *         "summaryFriendData" : null
         *       }
         *     ]
         *   }
         * }
         */
        void findPlayersUsingFilter(int32_t in_rangeDelta, int32_t in_numMatches, std::string in_jsonExtraParms, IServerCallback * in_callback = NULL);
        
        DEPRECATED void getOneWayPlayersWithFilter(int32_t in_rangeDelta, int32_t in_numMatches, std::string in_jsonExtraParms, IServerCallback * in_callback = NULL);
        
        
        /**
         * Enables Match Making for the Player
         *
         * Service Name - MatchMaking
         * Service Operation - EnableMatchMaking
         *
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data": null
         * }
         */
        void enableMatchMaking(IServerCallback * in_callback = NULL);
    };
}