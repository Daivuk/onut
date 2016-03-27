#pragma once

#include <string>

#include "IServerCallback.h"
#include "ServiceName.h"
#include "ServiceOperation.h"
#include "OperationParam.h"
#include "BrainCloudTypes.h"

namespace BrainCloud {
	
	class BrainCloudEvent
	{
	public:
		/**
         * Sends an event to the designated player id with the attached json data.
         * Any events that have been sent to a player will show up in their
         * incoming event mailbox. If the in_recordLocally flag is set to true,
         * a copy of this event (with the exact same event id) will be stored
         * in the sending player's "sent" event mailbox.
         *
         * Note that the list of sent and incoming events for a player is returned
         * in the "ReadPlayerState" call (in the BrainCloudPlayer module).
         *
		 * Service Name - Event
	     * Service Operation - Send
	   	 *
         * @param in_toPlayerId The id of the player who is being sent the event
         * @param in_eventType The user-defined type of the event.
         * @param in_jsonEventData The user-defined data for this event encoded in JSON.
         * @param in_recordLocally If true, a copy of this event will be saved in the
         * user's sent events mailbox.
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback includes the server generated
         * event id and is as follows:
         * {
         *   "status":200,
         *   "data":{
         *     "eventId":3824
         *   }
         * }
         */
        void sendEvent( const char * in_toPlayerId, const char * in_eventType, const std::string& in_jsonEventData, bool in_recordLocally, IServerCallback * in_callback = NULL );
		
        /**
         * Updates an event in the player's incoming event mailbox.
         *
		 * Service Name - Event
	     * Service Operation - UpdateEventData
	   	 *
         * @param in_fromPlayerId The id of the player who sent the event
         * @param in_eventId The event id
         * @param in_jsonEventData The user-defined data for this event encoded in JSON.
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data":null
         * }
         */
        void updateIncomingEventData( const char * in_fromPlayerId, uint64_t in_eventId, const std::string& in_jsonEventData, IServerCallback * in_callback = NULL );
		
		/**
         * Delete an event out of the player's incoming mailbox.
         *
		 * Service Name - Event
	     * Service Operation - DeleteIncoming
	   	 *
         * @param in_fromPlayerId The id of the player who sent the event
         * @param in_eventId The event id
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data":null
         * }
         */
        void deleteIncomingEvent( const char * in_fromPlayerId, uint64_t in_eventId, IServerCallback * in_callback = NULL);
		
		/**
         * Delete an event from the player's sent mailbox.
         *
         * Note that only events sent with the "recordLocally" flag
         * set to true will be added to a player's sent mailbox.
         *
		 * Service Name - Event
	     * Service Operation - DeleteSent
	   	 *
         * @param in_toPlayerId The id of the player who is being sent the event
         * @param in_eventId The event id
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data":null
         * }
         */
        void deleteSentEvent( const char * in_toPlayerId, uint64_t in_eventId, IServerCallback * in_callback = NULL);
        
        /**
         * Get the events currently queued for the player.
         *
         * Service Name - Event
         * Service Operation - GetEvents
         *
         * @param in_includeIncomingEvents Get events sent to the player
         * @param in_includeSentEvents Get events sent from the player
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *     "sent_events": [
         *         {
         *             "gameId": "10045",
         *             "eventData": {
         *                 "someMapAttribute": "someValue"
         *             },
         *             "createdAt": 1440528872855,
         *             "eventId": 1847,
         *             "fromPlayerId": "f89233ba-aeeb-4be2-b267-89781c2f0a12",
         *             "toPlayerId": "78a2a76b-1158-4a5a-b1dc-aa49e37997e8",
         *             "eventType": "type1"
         *         }
         *     ],
         *     "incoming_events": [
         *         {
         *             "gameId": "10045",
         *             "eventData": {
         *                 "someMapAttribute": "XXX"
         *             },
         *             "createdAt": 1440528981281,
         *             "eventId": 11981,
         *             "fromPlayerId": "36373298-b8bb-4b5c-917a-1c889fdae094",
         *             "toPlayerId": "f89233ba-aeeb-4be2-b267-89781c2f0a12",
         *             "eventType": "type1"
         *         }
         *     ]
         * }
         */
        void getEvents( bool in_includeIncomingEvents, bool in_includeSentEvents, IServerCallback * in_callback = NULL);
		
	private:
		
	};
}