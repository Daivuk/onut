
#pragma once

#include "BrainCloudTypes.h"
#include <string>

namespace BrainCloud {

    class IServerCallback;
    class BrainCloudClient;
    
    class BrainCloudAsyncMatch {
    
    public:
        /**
         * Creates an instance of an asynchronous match.
         *
         * Service Name - AsyncMatch
         * Service Operation - Create
         *
         * @param in_jsonOpponentIds  JSON string identifying the opponent platform and id for this match.
         *
         * Platforms are identified as:
         * BC - a brainCloud profile id
         * FB - a Facebook id
         *
         * An exmaple of this string would be:
         * [
         *     {
         *         "platform": "BC",
         *         "id": "some-braincloud-profile"
         *     },
         *     {
         *         "platform": "FB",
         *         "id": "some-facebook-id"
         *     }
         * ]
         *
         * @param in_pushNotificationMessage Optional push notification message to send to the other party.
         *  Refer to the Push Notification functions for the syntax required.
         * @param in_callback Optional instance of IServerCallback to call when the server response is received.
         *
         * @returns The JSON returned in the callback is as follows:
         * {
         *     "gameId": "16245",
         *     "ownerId": "0df9f282-183b-4d67-866e-670fb35a2376",
         *     "matchId": "b55d12d6-f01f-45c5-827c-ded706374524",
         *     "version": 0,
         *     "players": [
         *         {
         *             "playerId": "0df9f282-183b-4d67-866e-670fb35a2376",
         *             "playerName": "UserB",
         *             "pictureUrl": null
         *         },
         *         {
         *             "playerId": "4693ec75-3a99-4577-aef7-0f41d299339c",
         *             "playerName": "Presto1",
         *             "pictureUrl": null
         *         }
         *     ],
         *     "status": {
         *         "status": "NOT_STARTED",
         *         "currentPlayer": "0df9f282-183b-4d67-866e-670fb35a2376"
         *     },
         *     "summary": null,
         *     "createdAt": 1415641372974,
         *     "updatedAt": 1415641372974
         * }
         */
        void createMatch(
            const char * in_jsonOpponentIds,
            const char * in_pushNotificationMessage = NULL,
            IServerCallback * in_callback = NULL );
        
        // Removal after May 10 2016 - Use method without in_matchId instead
        DEPRECATED void createMatch(
            const char * in_jsonOpponentIds,
            const char * in_pushNotificationMessage = NULL,
            const char * in_matchId = NULL,
            IServerCallback * in_callback = NULL );
    
        /**
         * Creates an instance of an asynchronous match with an initial turn.
         *
         * Service Name - AsyncMatch
         * Service Operation - Create
         *
         * @param in_jsonOpponentIds  JSON string identifying the opponent platform and id for this match.
         *
         * Platforms are identified as:
         * BC - a brainCloud profile id
         * FB - a Facebook id
         *
         * An exmaple of this string would be:
         * [
         *     {
         *         "platform": "BC",
         *         "id": "some-braincloud-profile"
         *     },
         *     {
         *         "platform": "FB",
         *         "id": "some-facebook-id"
         *     }
         * ]
         *
         * @param in_jsonMatchState    JSON string blob provided by the caller
         * @param in_pushNotificationMessage Optional push notification message to send to the other party.
         * Refer to the Push Notification functions for the syntax required.
         * @param in_nextPlayer Optionally, force the next player player to be a specific player
         * @param in_jsonSummary Optional JSON string defining what the other player will see as a summary of the game when listing their games
         * @param in_callback Optional instance of IServerCallback to call when the server response is received.
         *
         * @returns The JSON returned in the callback is as follows:
         * {
         *     "gameId": "16245",
         *     "ownerId": "0df9f282-183b-4d67-866e-670fb35a2376",
         *     "matchId": "b55d12d6-f01f-45c5-827c-ded706374524",
         *     "version": 0,
         *     "players": [
         *         {
         *             "playerId": "0df9f282-183b-4d67-866e-670fb35a2376",
         *             "playerName": "UserB",
         *             "pictureUrl": null
         *         },
         *         {
         *             "playerId": "4693ec75-3a99-4577-aef7-0f41d299339c",
         *             "playerName": "Presto1",
         *             "pictureUrl": null
         *         }
         *     ],
         *     "status": {
         *         "status": "NOT_STARTED",
         *         "currentPlayer": "0df9f282-183b-4d67-866e-670fb35a2376"
         *     },
         *     "summary": null,
         *     "createdAt": 1415641372974,
         *     "updatedAt": 1415641372974
         * }
         */
        void createMatchWithInitialTurn(
            const char * in_jsonOpponentIds,
            const char * in_jsonMatchState,
            const char * in_pushNotificationMessage = NULL,
            const char * in_nextPlayer = NULL,
            const char * in_jsonSummary = NULL,
            IServerCallback * in_callback = NULL);
        
        // Removal after May 10 2016 - Use method without in_matchId instead
        DEPRECATED void createMatchWithInitialTurn(
            const char * in_jsonOpponentIds,
            const char * in_jsonMatchState,
            const char * in_pushNotificationMessage = NULL,
            const char * in_matchId = NULL,
            const char * in_nextPlayer = NULL,
            const char * in_jsonSummary = NULL,
            IServerCallback * in_callback = NULL);
    
        /**
         * Submits a turn for the given match.
         *
         * Service Name - AsyncMatch
         * Service Operation - SubmitTurn
         *
         * @param in_ownerId Match owner identfier
         * @param in_matchId Match identifier
         * @param in_version Game state version to ensure turns are submitted once and in order
         * @param in_jsonMatchState JSON string provided by the caller
         * @param in_pushNotificationMessage Optional push notification message to send to the other party.
         *  Refer to the Push Notification functions for the syntax required.
         * @param in_nextPlayer Optionally, force the next player player to be a specific player
         * @param in_jsonSummary Optional JSON string that other players will see as a summary of the game when listing their games
         * @param in_jsonStatistics Optional JSON string blob provided by the caller
         * @param in_callback Optional instance of IServerCallback to call when the server response is received.
         *
         * @returns The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {
         *         "gameId": "145677",
         *         "ownerId": "2bd7abc6-c2ec-4946-a1a8-02bad38540ad",
         *         "matchId": "1aac24b2-7976-4fd7-b7c6-44dere6d26a4",
         *         "version": 1,
         *         "players": [
         *             {
         *                 "playerId": "2bd7abc6-c2ec-4946-a1a8-02bad38540ad",
         *                 "playerName": "UserB",
         *                 "pictureUrl": null
         *             },
         *             {
         *                 "playerId": "11c9dd4d-9ed1-416d-baw2-5228c1efafac",
         *                 "playerName": "UserA",
         *                 "pictureUrl": null
         *             }
         *         ],
         *         "status": {
         *             "status": "PENDING",
         *             "currentPlayer": "11c9dd4d-9ed1-416d-baw2-5228c1efafac"
         *         },
         *         "summary": {
         *             "resources": 1234
         *         },
         *         "createdAt": 1442507219609,
         *         "updatedAt": 1442507319700
         *     }
         * }
         */
        void submitTurn(
            const char * in_ownerId,
            const char * in_matchId,
            uint64_t in_version,
            const char * in_jsonMatchState,
            const char * in_pushNotificationMessage = NULL,
            const char * in_nextPlayer = NULL,
            const char * in_jsonSummary = NULL,
            const char * in_jsonStatistics = NULL,
            IServerCallback * in_callback = NULL );
    
        /**
         * Allows the current player (only) to update Summary data without having to submit a whole turn.
         *
         * Service Name - AsyncMatch
         * Service Operation - UpdateMatchSummary
         *
         * @param in_ownerId Match owner identfier
         * @param in_matchId Match identifier
         * @param in_version Game state version to ensure turns are submitted once and in order
         * @param in_jsonSummary JSON string that other players will see as a summary of the game when listing their games
         * @param in_callback Optional instance of IServerCallback to call when the server response is received.
         *
         * @returns The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {
         *         "gameId": "145677",
         *         "ownerId": "2bd723c6-c2ec-4946-a1a8-02b7a38540ad",
         *         "matchId": "1aac24b2-7976-4fd7-b7c6-44d7ae6d26a4",
         *         "version": 2,
         *         "players": [
         *             {
         *                 "playerId": "2bd723c6-c2ec-4946-a1a8-02b7a38540ad",
         *                 "playerName": "UserA",
         *                 "pictureUrl": null
         *             },
         *             {
         *                 "playerId": "11c2dd4d-9ed1-416d-bd04-5228c1efafac",
         *                 "playerName": "UserB",
         *                 "pictureUrl": null
         *             }
         *         ],
         *         "status": {
         *             "status": "PENDING",
         *             "currentPlayer": "11c2dd4d-9ed1-416d-bd04-5228c1efafac"
         *         },
         *         "summary": {
         *             "resources": 2564
         *         },
         *         "createdAt": 1442507219609,
         *         "updatedAt": 1442507550372
         *     }
         * }
         */
        void updateMatchSummaryData(
            const char * in_ownerId,
            const char * in_matchId,
            uint64_t in_version,
            const char * in_jsonSummary,
            IServerCallback * in_callback = NULL );
    
        /**
         * Marks the given match as complete.
         *
         * Service Name - AsyncMatch
         * Service Operation - Complete
         *
         * @param in_ownerId Match owner identifier
         * @param in_matchId Match identifier
         * @param in_callback Optional instance of IServerCallback to call when the server response is received.
         *
         * @returns The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {}
         * }
         */
        void completeMatch( const char * in_ownerId, const char * in_matchId, IServerCallback * in_callback = NULL );
    
        /**
         * Returns the current state of the given match.
         *
         * Service Name - AsyncMatch
         * Service Operation - ReadMatch
         *
         * @param in_ownerId   Match owner identifier
         * @param in_matchId   Match identifier
         * @param in_callback  Optional instance of IServerCallback to call when the server response is received.
         *
         * @returns The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {
         *         "gameId": "10299",
         *         "ownerId": "11c9dd4d-9ed1-416d-bd04-5228c1efafac",
         *         "matchId": "0d4c1803-887a-4f20-a2e4-73eeedba411e",
         *         "version": 1,
         *         "players": [
         *             {
         *                 "playerId": "11c9dd4d-9ed1-416d-bd04-5228c1efafac",
         *                 "playerName": "UserB",
         *                 "pictureUrl": null
         *             },
         *             {
         *                 "playerId": "2bd7abc6-c2ec-4946-a1a8-02b7a38540ad",
         *                 "playerName": "UserA",
         *                 "pictureUrl": null
         *             }
         *         ],
         *         "status": {
         *             "status": "PENDING",
         *             "currentPlayer": "2bd7abc6-c2ec-4946-a1a8-02b7a38540ad"
         *         },
         *         "summary": null,
         *         "statistics": {},
         *         "matchState": {},
         *         "createdAt": 1442508171624,
         *         "updatedAt": 1442508171632
         *     }
         * }
         */
        void readMatch( const char * in_ownerId, const char * in_matchId, IServerCallback * in_callback = NULL );
    
        /**
         * Returns the match history of the given match.
         *
         * Service Name - AsyncMatch
         * Service Operation - ReadMatchHistory
         *
         * @param in_ownerId   Match owner identifier
         * @param in_matchId   Match identifier
         * @param in_callback  Optional instance of IServerCallback to call when the server response is received.
         *
         * @returns The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {
         *         "gameId": "14577",
         *         "ownerId": "2bd32bc6-c2ec-4916-a1a8-02b7a38540ad",
         *         "matchId": "1aac24b2-7976-4fd7-b7c6-44d32e6d26a4",
         *         "turns": [
         *             {
         *                 "playerId": "2bd32bc6-c2ec-4916-a1a8-02b7a38540ad",
         *                 "matchState": {
         *                     "color": "red"
         *                 },
         *                 "createdAt": 1442507319697
         *             },
         *             {
         *                 "playerId": "11c9324d-9ed1-416d-b124-5228c1efafac",
         *                 "matchState": {},
         *                 "createdAt": 1442507703667
         *             }
         *         ]
         *     }
         * }
         */
        void readMatchHistory( const char * in_ownerId, const char * in_matchId, IServerCallback * in_callback = NULL );
    
        /**
         * Returns all matches that are NOT in a COMPLETE state for which the player is involved.
         *
         * Service Name - AsyncMatch
         * Service Operation - FindMatches
         *
         * @param in_callback  Optional instance of IServerCallback to call when the server response is received.
         *
         * @returns The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {
         *         "results": [
         *             {
         *                 "gameId": "123456",
         *                 "ownerId": "7630f98e-1236-4ead-88ee-27ce63b2db2c",
         *                 "matchId": "97c8d087-40d8-45c2-aa2b-6d0d83424ec5",
         *                 "version": 1,
         *                 "players": [
         *                     {
         *                         "playerId": "7630f98e-13b6-4ead-88ee-27ce63b2db2c",
         *                         "playerName": "UserA-122217922",
         *                         "pictureUrl": null
         *                     },
         *                     {
         *                         "playerId": "efab2d0b-90a1-48cf-8678-ae81d7aaed89",
         *                         "playerName": "UserB-122217922",
         *                         "pictureUrl": null
         *                     },
         *                     {
         *                         "playerId": "b28ff14a-364a-40b3-ac4e-d2b23983519c",
         *                         "playerName": "UserC-338593317",
         *                         "pictureUrl": null
         *                     }
         *                 ],
         *                 "status": {
         *                     "status": "PENDING",
         *                     "currentPlayer": "efab2d0b-90a1-48cf-8678-ae81d7aaed89"
         *                 },
         *                 "summary": null,
         *                 "createdAt": 1442586020180,
         *                 "updatedAt": 1442586020188
         *             }
         *         ]
         *     }
         * }
         */
        void findMatches( IServerCallback * in_callback );
    
        /**
         * Returns all matches that are in a COMPLETE state for which the player is involved.
         *
         * Service Name - AsyncMatch
         * Service Operation - FindMatchesCompleted
         *
         * @param in_callback  Optional instance of IServerCallback to call when the server response is received.
         *
         * @returns The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {
         *         "results": [
         *             {
         *                 "gameId": "10170",
         *                 "ownerId": "9ad4f990-5466-4d00-a334-de834e1ac4ec",
         *                 "matchId": "877dd25d-ea21-4857-ba2a-2134d0f5ace2",
         *                 "version": 2,
         *                 "players": [
         *                     {
         *                         "playerId": "9ad4f990-5466-4d00-a334-de834e1ac4ec",
         *                         "playerName": "",
         *                         "pictureUrl": null
         *                     },
         *                     {
         *                         "playerId": "963a2079-6e7a-48de-a4f2-8ab16c811975",
         *                         "playerName": "",
         *                         "pictureUrl": null
         *                     }
         *                 ],
         *                 "status": {
         *                     "status": "COMPLETE",
         *                     "currentPlayer": "963a2079-6e7a-48de-a4f2-8ab16c811975"
         *                 },
         *                 "summary": null,
         *                 "createdAt": 1442586358023,
         *                 "updatedAt": 1442586374787
         *             }
         *         ]
         *     }
         * }
         */
        void findCompleteMatches( IServerCallback * in_callback = NULL);
    
        /**
         * Marks the given match as abandoned.
         *
         * Service Name - AsyncMatch
         * Service Operation - Abandon
         *
         * @param in_ownerId   Match owner identifier
         * @param in_matchId   Match identifier
         * @param in_callback  Optional instance of IServerCallback to call when the server response is received.
         *
         * @returns The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {}
         * }
         */
        void abandonMatch( const char * in_ownerId, const char * in_matchId, IServerCallback * in_callback = NULL );
    
        /**
         * Removes the match and match history from the server. DEBUG ONLY, in production it is recommended
         *   the user leave it as completed.
         *
         * Service Name - AsyncMatch
         * Service Operation - Delete
         *
         * @param in_ownerId   Match owner identifier
         * @param in_matchId   Match identifier
         * @param in_callback  Optional instance of IServerCallback to call when the server response is received.
         *
         * @returns The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {}
         * }
         */
        void deleteMatch( const char * in_ownerId, const char * in_matchId, IServerCallback * in_callback = NULL );
    
    private:
        void createMatchInternal(
            const char * in_jsonOpponentIds,
            const char * in_jsonMatchState,
            const char * in_pushNotificationMessage,
            const char * in_matchId,
            const char * in_nextPlayer,
            const char * in_jsonSummary,
            IServerCallback * in_callback);
    };
}