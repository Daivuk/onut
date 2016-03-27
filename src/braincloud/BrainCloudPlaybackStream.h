#pragma once

#include <string>

#include "IServerCallback.h"
#include "ServiceName.h"
#include "ServiceOperation.h"
#include "OperationParam.h"
#include "BrainCloudTypes.h"

namespace BrainCloud {

	class BrainCloudPlaybackStream
	{
	public:

		/**
		* Starts a stream
		*
		* Service Name - PlaybackStream
		* Service Operation - StartStream
		*
		* @param in_targetPlayerId The player to start a stream with
		* @param in_includeSharedData Whether to include shared data in the stream
		* @param in_callback The method to be invoked when the server response is received
		*
		* @return The JSON returned in the callback is as follows:
		* {
		*    "status": 200,
		*    "data": {
		*     "playbackStreamId": "b8da4619-2ddc-4184-b654-cd11d12a3275",
		*     "gameId": "10000",
		*     "initiatingPlayerId": "198bcafcd-6e84-4c30-9f6e-3f9f016440c6",
		*     "targetPlayerId": "a6943c74-6655-4245-8b2b-13bb908d3f88",
		*     "status": "STARTED",
		*     "summary": {},
		*     "initialSharedData": {
		*      "entities": [],
		*      "statistics": {}
		*     },
		*     "events": [],
		*     "createdAt": 1425481184200,
		*     "updatedAt": 1425481184200
		*    }
		*   }
		*
		*/
		void startStream(const char * in_targetPlayerId, bool in_includeSharedData, IServerCallback * in_callback = NULL);

		/**
		* Reads a stream
		*
		* Service Name - PlaybackStream
		* Service Operation - ReadStream
		*
		* @param in_playbackStreamId Identifies the stream to read
		* @param in_callback The method to be invoked when the server response is received
		*
		* @return The JSON returned in the callback is as follows:
		* {
		*    "status": 200,
		*    "data": {
		*     "playbackStreamId": "b8da4619-2ddc-4184-b654-cd11d12a3275",
		*     "gameId": "10000",
		*     "initiatingPlayerId": "198bcafcd-6e84-4c30-9f6e-3f9f016440c6",
		*     "targetPlayerId": "a6943c74-6655-4245-8b2b-13bb908d3f88",
		*     "status": "COMPLETE",
		*     "summary": { total : 5 },
		*     "initialSharedData": {
		*      "entities": [ {entry : 3}, {entry : 2 }],
		*      "statistics": {}
		*     },
		*     "events": [],
		*     "createdAt": 1425481184200,
		*     "updatedAt": 1425481184200
		*    }
		*   }
		*
		*/
		void readStream(const char * in_playbackStreamId, IServerCallback * in_callback = NULL);

		/**
		* Ends a stream
		*
		* Service Name - PlaybackStream
		* Service Operation - EndStream
		*
		* @param in_playbackStreamId Identifies the stream to read
		* @param in_callback The method to be invoked when the server response is received
		*
		* @return The JSON returned in the callback is as follows:
		* {
		*    "status": 200,
		*    "data": null
		* }
		*
		*/
		void endStream(const char * in_playbackStreamId, IServerCallback * in_callback = NULL);

		/**
		* Deletes a stream
		*
		* Service Name - PlaybackStream
		* Service Operation - DeleteStream
		*
		* @param in_playbackStreamId Identifies the stream to read
		* @param in_callback The method to be invoked when the server response is received
		*
		* @return The JSON returned in the callback is as follows:
		* {
		*    "status": 200,
		*    "data": null
		* }
		*
		*/
		void deleteStream(const char * in_playbackStreamId, IServerCallback * in_callback = NULL);

		/**
		* Adds a stream event
		*
		* Service Name - PlaybackStream
		* Service Operation - AddEvent
		*
		* @param in_playbackStreamId Identifies the stream to read
		* @param in_jsonEventData Describes the event
		* @param in_jsonSummary Current summary data as of this event
		* @param in_callback The method to be invoked when the server response is received
		*
		* @return The JSON returned in the callback is as follows:
		* {
		*    "status": 200,
		*    "data": null
		* }
		*
		*/
		void addEvent(const char * in_playbackStreamId, const char * in_jsonEventData, const char * in_jsonSummary, IServerCallback * in_callback = NULL);

		/**
		* Gets stream summaries for initiating player
		*
		* Service Name - PlaybackStream
		* Service Operation - GetStreamSummariesForInitiatingPlayer
		*
		* @param in_initiatingPlayerId The player that started the stream
		* @param in_callback The method to be invoked when the server response is received
		*
		* @return The JSON returned in the callback is as follows:
		* {
		*  "status": 200,
		*  "data": {
		*   "streams": [
		*    {
		*     "playbackStreamId": "b8da4619-2ddc-4184-b654-cd11d12a3275",
		*     "gameId": "10000",
		*     "initiatingPlayerId": "198bcadb-6e84-4c30-9f6e-3f9f016440c6",
		*     "targetPlayerId": "a6943c74-6636-4245-8b2b-13bb908d3f88",
		*     "status": "IN_PROGRESS",
		*     "summary": {
		*      "total": 5
		*     },
		*     "createdAt": 1425481184200,
		*     "updatedAt": 1425484485139
		*    }
		*   ]
		*  }
		* }         *
		*/
		void getStreamSummariesForInitiatingPlayer(const char * in_initiatingPlayerId, IServerCallback * in_callback = NULL);

		/**
		* Gets stream summaries for target player
		*
		* Service Name - PlaybackStream
		* Service Operation - GetStreamSummariesForTargetPlayer
		*
		* @param in_targetPlayerId The player that started the stream
		* @param in_callback The method to be invoked when the server response is received
		*
		* @return The JSON returned in the callback is as follows:
		* {
		*  "status": 200,
		*  "data": {
		*   "streams": [
		*    {
		*     "playbackStreamId": "b8da4619-2ddc-4184-b654-cd11d12a3275",
		*     "gameId": "10000",
		*     "initiatingPlayerId": "198bcadb-6e84-4c30-9f6e-3f9f016440c6",
		*     "targetPlayerId": "a6943c74-6636-4245-8b2b-13bb908d3f88",
		*     "status": "IN_PROGRESS",
		*     "summary": {
		*      "total": 5
		*     },
		*     "createdAt": 1425481184200,
		*     "updatedAt": 1425484485139
		*    }
		*   ]
		*  }
		* }         *
		*/
		void getStreamSummariesForTargetPlayer(const char * in_targetPlayerId, IServerCallback * in_callback = NULL);

	};
}