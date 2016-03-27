#pragma once

#include <string>

#include "IServerCallback.h"
#include "ServiceName.h"
#include "ServiceOperation.h"
#include "OperationParam.h"
#include "BrainCloudTypes.h"

namespace BrainCloud {
	
	class BrainCloudOneWayMatch
	{
		public:
		/**
         * Starts a match
         *
		 * Service Name - OneWayMatch
	     * Service Operation - StartMatch
	   	 *
         * @param in_otherPlayerId The player to start a match with
         * @param in_rangeDelta The range delta used for the initial match search
         * @param in_callback The method to be invoked when the server response is received
         *
		 * @return The JSON returned in the callback is as follows:
		 * {
		 *    "status": 200,
		 *    "data": {
		 *        "playbackStreamId": "d18719db-9d02-2341-b62f-8e2f013369be",
		 *        "initiatingPlayerId": "d175f6ac-9221-4adc-aea4-f25f2426ff62",
		 *        "targetPlayerId": "07a0d23e-996b-4488-90ae-cb438342423a54",
		 *        "status": "STARTED",
		 *        "summary": {},
		 *        "initialSharedData": {
		 *            "entities": [],
		 *            "statistics": {}
		 *        },
		 *        "events": [],
		 *        "createdAt": 1437419496282,
		 *        "updatedAt": 1437419496282
		 *    }
		 * }
		 */
		void startMatch(const char * in_otherPlayerId, int32_t in_rangeDelta, IServerCallback * in_callback = NULL);
		
		/**
         * Cancels a match
         *
		 * Service Name - OneWayMatch
	     * Service Operation - CancelMatch
	   	 *
         * @param in_playbackStreamId The playback stream id returned in the start match
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data": null
         * }
         */
		void cancelMatch(const char * in_playbackStreamId, IServerCallback * in_callback = NULL);
		
		/**
         * Completes a match
         *
		 * Service Name - OneWayMatch
	     * Service Operation - CompleteMatch
	   	 *
         * @param in_playbackStreamId The playback stream id returned in the initial start match
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status": 200,
         *   "data": null
         * }
         */
		void completeMatch(const char * in_playbackStreamId, IServerCallback * in_callback = NULL);
	};
}