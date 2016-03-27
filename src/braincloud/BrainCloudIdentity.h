#pragma once

#include <string>

#include "IServerCallback.h"
#include "ServiceName.h"
#include "ServiceOperation.h"
#include "OperationParam.h"

namespace BrainCloud {

	class BrainCloudIdentity
	{
	public:
		BrainCloudIdentity();

		/**
         * Attach the user's Facebook credentials to the current profile.
         *
		 * Service Name - Identity
	     * Service Operation - Attach
	   	 *
         * @param in_facebookId The facebook id of the user
		 * @param in_authenticationToken The validated token from the Facebook SDK
		 *   (that will be further validated when sent to the bC service)
		 * @param in_callback The method to be invoked when the server response is received
		 *
         * Errors to watch for:  SWITCHING_PROFILES - this means that the Facebook identity you provided
         * already points to a different profile.  You will likely want to offer the player the
         * choice to *SWITCH* to that profile, or *MERGE* the profiles.
         *
         * To switch profiles, call ClearSavedProfileID() and call AuthenticateFacebook().
         */
        void attachFacebookIdentity(const char * in_facebookId, const char * in_authenticationToken, IServerCallback * in_callback = NULL);

		/**
         * Merge the profile associated with the provided Facebook credentials with the
         * current profile.
         *
		 * Service Name - Identity
	     * Service Operation - Merge
	   	 *
         * @param in_facebookId The facebook id of the user
		 * @param in_authenticationToken The validated token from the Facebook SDK
		 *   (that will be further validated when sent to the bC service)
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 */
        void mergeFacebookIdentity(const char * in_facebookId, const char * in_authenticationToken, IServerCallback * in_callback = NULL);

		/**
		 * Detach the Facebook identity from this profile.
		 *
		 * Service Name - Identity
		 * Service Operation - Detach
		 *
		 * @param in_facebookId The facebook id of the user
		 * @param in_continueAnon Proceed even if the profile will revert to anonymous?
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set in_continueAnon to false, and
		 * disconnecting this identity would result in the profile being anonymous (which means that
		 * the profile wouldn't be retrievable if the user loses their device)
		 */
        void detachFacebookIdentity(const char * in_facebookId, bool in_continueAnon, IServerCallback * in_callback = NULL);

		/**
         * Attach a Game Center identity to the current profile.
         *
		 * Service Name - Identity
	     * Service Operation - Attach
	   	 *
         * @param in_gameCenterId The player's game center id  (use the playerID property from the local GKPlayer object)
         * @param in_callback The method to be invoked when the server response is received
		 *
         * Errors to watch for:  SWITCHING_PROFILES - this means that the Game Center identity you provided
         * already points to a different profile.  You will likely want to offer the player the
         * choice to *SWITCH* to that profile, or *MERGE* the profiles.
         *
         * To switch profiles, call ClearSavedProfileID() and call this method again.
         *
         */
		void attachGameCenterIdentity(const char * in_gameCenterId, IServerCallback * in_callback = NULL);

		/**
         * Merge the profile associated with the specified Game Center identity with the current profile.
         *
		 * Service Name - Identity
	     * Service Operation - Merge
	   	 *
         * @param in_gameCenterId The player's game center id  (use the playerID property from the local GKPlayer object)
         * @param in_callback The method to be invoked when the server response is received
         */
        void mergeGameCenterIdentity(const char * in_gameCenterId, IServerCallback * in_callback = NULL);

		/**
         * Detach the Game Center identity from the current profile.
         *
		 * Service Name - Identity
		 * Service Operation - Detach
		 *
         * @param in_gameCenterId The player's game center id  (use the playerID property from the local GKPlayer object)
         * @param in_continueAnon Proceed even if the profile will revert to anonymous?
         * @param in_callback The method to be invoked when the server response is received
		 *
		 * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set in_continueAnon to false, and
         * disconnecting this identity would result in the profile being anonymous (which means that
         * the profile wouldn't be retrievable if the user loses their device)
         */
        void detachGameCenterIdentity(const char * in_gameCenterId, bool in_continueAnon, IServerCallback * in_callback = NULL);

		/**
         * Attach a Email and Password identity to the current profile.
         *
		 * Service Name - Identity
	     * Service Operation - Attach
	   	 *
         * @param in_email The player's e-mail address
         * @param in_password The player's password
         * @param in_callback The method to be invoked when the server response is received
		 *
         * Errors to watch for:  SWITCHING_PROFILES - this means that the email address you provided
         * already points to a different profile.  You will likely want to offer the player the
         * choice to *SWITCH* to that profile, or *MERGE* the profiles.
         *
         * To switch profiles, call ClearSavedProfileID() and then call AuthenticateEmailPassword().
         */
        void attachEmailIdentity(const char * in_email, const char * in_password, IServerCallback * in_callback = NULL);

		/**
         * Merge the profile associated with the provided e=mail with the current profile.
         *
		 * Service Name - Identity
	     * Service Operation - Merge
	   	 *
         * @param in_email The player's e-mail address
         * @param in_password The player's password
         * @param in_callback The method to be invoked when the server response is received
		 *
         */
        void mergeEmailIdentity(const char * in_email, const char * in_password, IServerCallback * in_callback = NULL);

		/**
         * Detach the e-mail identity from the current profile
         *
		 * Service Name - Identity
		 * Service Operation - Detach
		 *
         * @param in_email The player's e-mail address
         * @param in_continueAnon Proceed even if the profile will revert to anonymous?
         * @param in_callback The method to be invoked when the server response is received
		 *
		 * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set in_continueAnon to false, and
         * disconnecting this identity would result in the profile being anonymous (which means that
         * the profile wouldn't be retrievable if the user loses their device)
         */
        void detachEmailIdentity(const char * in_email, bool in_continueAnon, IServerCallback * in_callback = NULL);

		/**
         * Attach a Universal (userid + password) identity to the current profile.
         *
		 * Service Name - Identity
	     * Service Operation - Attach
	   	 *
         * @param in_userId The player's userid
         * @param in_password The player's password
         * @param in_callback The method to be invoked when the server response is received
		 *
         * Errors to watch for:  SWITCHING_PROFILES - this means that the email address you provided
         * already points to a different profile.  You will likely want to offer the player the
         * choice to *SWITCH* to that profile, or *MERGE* the profiles.
         *
         * To switch profiles, call ClearSavedProfileID() and then call AuthenticateEmailPassword().
         */
        void attachUniversalIdentity(const char * in_userId, const char * in_password, IServerCallback * in_callback = NULL);

		/**
         * Merge the profile associated with the provided userId with the current profile.
         *
		 * Service Name - Identity
	     * Service Operation - Merge
	   	 *
         * @param in_userId The player's userid
         * @param in_password The player's password
         * @param in_callback The method to be invoked when the server response is received
         */
        void mergeUniversalIdentity(const char * in_userId, const char * in_password, IServerCallback * in_callback = NULL);

		/**
         * Detach the universal identity from the current profile
         *
		 * Service Name - Identity
		 * Service Operation - Detach
		 *
         * @param in_userId The player's userid
         * @param in_continueAnon Proceed even if the profile will revert to anonymous?
         * @param in_callback The method to be invoked when the server response is received
		 *
		 * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set in_continueAnon to false, and
         * disconnecting this identity would result in the profile being anonymous (which means that
         * the profile wouldn't be retrievable if the user loses their device)
         */
        void detachUniversalIdentity(const char * in_userId, bool in_continueAnon, IServerCallback * in_callback = NULL);

		/**
         * Attach a Steam (userid + steamsessionticket) identity to the current profile.
         *
		 * Service Name - Identity
	     * Service Operation - Attach
	   	 *
         * @param in_steamId String representation of 64 bit steam id
         * @param in_sessionTicket The player's session ticket (hex encoded)
         * @param in_callback The method to be invoked when the server response is received
         *
         * Errors to watch for:  SWITCHING_PROFILES - this means that the email address you provided
         * already points to a different profile.  You will likely want to offer the player the
         * choice to *SWITCH* to that profile, or *MERGE* the profiles.
         *
         * To switch profiles, call ClearSavedProfileID() and then call AuthenticateSteam().
         */
		void attachSteamIdentity(const char * in_steamId, const char * in_sessionTicket, IServerCallback * in_callback = NULL);

        /**
         * Merge the profile associated with the provided steam userid with the current profile.
         *
		 * Service Name - Identity
	     * Service Operation - Merge
	   	 *
         * @param in_steamId String representation of 64 bit steam id
         * @param in_sessionTicket The player's session ticket (hex encoded)
         * @param in_callback The method to be invoked when the server response is received
         *
         */
		void mergeSteamIdentity(const char * in_steamId, const char * in_sessionTicket, IServerCallback * in_callback = NULL);

        /**
         * Detach the steam identity from the current profile
         *
		 * Service Name - Identity
	     * Service Operation - Detach
	   	 *
         * @param in_steamId String representation of 64 bit steam id
         * @param in_continueAnon Proceed even if the profile will revert to anonymous?
         * @param in_callback The method to be invoked when the server response is received
         *
         * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set in_continueAnon to false, and
         * disconnecting this identity would result in the profile being anonymous (which means that
         * the profile wouldn't be retrievable if the user loses their device)
         */
		void detachSteamIdentity(const char * in_steamId, bool in_continueAnon, IServerCallback * in_callback = NULL);

        /**
         * Attach the user's Google credentials to the current profile.
         *
         * Service Name - Identity
         * Service Operation - Attach
         *
         * @param in_googleId The Google id of the user
         * @param in_authenticationToken The validated token from the Google SDK
         *   (that will be further validated when sent to the bC service)
         * @param in_callback The method to be invoked when the server response is received
         *
         * Errors to watch for:  SWITCHING_PROFILES - this means that the Google identity you provided
         * already points to a different profile.  You will likely want to offer the player the
         * choice to *SWITCH* to that profile, or *MERGE* the profiles.
         *
         * To switch profiles, call ClearSavedProfileID() and call AuthenticateGoogle().
         */
        void attachGoogleIdentity(const char * in_googleId, const char * in_authenticationToken, IServerCallback * in_callback = NULL);

        /**
         * Merge the profile associated with the provided Google credentials with the
         * current profile.
         *
         * Service Name - Identity
         * Service Operation - Merge
         *
         * @param in_googleId The Google id of the user
         * @param in_authenticationToken The validated token from the Google SDK
         *   (that will be further validated when sent to the bC service)
         * @param in_callback The method to be invoked when the server response is received
         *
         */
        void mergeGoogleIdentity(const char * in_googleId, const char * in_authenticationToken, IServerCallback * in_callback = NULL);

        /*
         * Detach the Google identity from this profile.
         *
         * Service Name - Identity
         * Service Operation - Detach
         *
         * @param in_googleId The Google id of the user
         * @param in_continueAnon Proceed even if the profile will revert to anonymous?
         * @param in_callback The method to be invoked when the server response is received
         *
         * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set in_continueAnon to false, and
         * disconnecting this identity would result in the profile being anonymous (which means that
         * the profile wouldn't be retrievable if the user loses their device)
         */
        void detachGoogleIdentity(const char * in_googleId, bool in_continueAnon, IServerCallback * in_callback = NULL);

        /**
         * Attach the user's Twitter credentials to the current profile.
         *
         * Service Name - Identity
         * Service Operation - Attach
         *
         * @param in_twitterId The Twitter id of the user
         * @param in_authenticationToken The authentication token derrived from the twitter APIs
         * @param in_secret The secret given when attempting to link with Twitter
         * @param in_callback The method to be invoked when the server response is received
         *
         * Errors to watch for:  SWITCHING_PROFILES - this means that the Twitter identity you provided
         * already points to a different profile.  You will likely want to offer the player the
         * choice to *SWITCH* to that profile, or *MERGE* the profiles.
         *
         * To switch profiles, call ClearSavedProfileID() and call AuthenticateTwitter().
         */
        void attachTwitterIdentity(const char * in_twitterId, const char * in_authenticationToken, const char * in_secret, IServerCallback * in_callback = NULL);

        /**
         * Merge the profile associated with the provided Twitter credentials with the
         * current profile.
         *
         * Service Name - Identity
         * Service Operation - Merge
         *
         * @param in_twitterId The Twitter id of the user
         * @param in_authenticationToken The authentication token derrived from the twitter APIs
         * @param in_secret The secret given when attempting to link with Twitter
         * @param in_callback The method to be invoked when the server response is received
         *
         */
        void mergeTwitterIdentity(const char * in_twitterId, const char * in_authenticationToken, const char * in_secret, IServerCallback * in_callback = NULL);

        /**
         * Detach the Twitter identity from this profile.
         *
         * Service Name - Identity
         * Service Operation - Detach
         *
         * @param in_twitterId The Twitter id of the user
         * @param in_continueAnon Proceed even if the profile will revert to anonymous?
         * @param in_callback The method to be invoked when the server response is received
         *
         * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set in_continueAnon to false, and
         * disconnecting this identity would result in the profile being anonymous (which means that
         * the profile wouldn't be retrievable if the user loses their device)
         */
        void detachTwitterIdentity(const char * in_twitterId, bool in_continueAnon, IServerCallback * in_callback = NULL);

        /**
         * Switch to a Child Profile
         *
         * Service Name - Identity
         * Service Operation - SWITCH_TO_CHILD_PROFILE
         *
         * @param in_childProfileId The profileId of the child profile to switch to
         * If null and forceCreate is true a new profile will be created
         * @param in_childGameId The appId of the child game to switch to
         * @param in_forceCreate Should a new profile be created if it does not exist?
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {
         *         "vcPurchased": 0,
         *         "experiencePoints": 0,
         *         "xpCapped": false,
         *         "playerName": "TestUser",
         *         "vcClaimed": 0,
         *         "rewards": {
         *             "rewardDetails": {},
         *             "rewards": {},
         *             "currency": {
         *                 "credits": {
         *                     "purchased": 0,
         *                     "balance": 0,
         *                     "consumed": 0,
         *                     "awarded": 0
         *                 },
         *                 "gold": {
         *                     "purchased": 0,
         *                     "balance": 0,
         *                     "consumed": 0,
         *                     "awarded": 0
         *                 }
         *             }
         *         },
         *         "loginCount": 1,
         *         "server_time": 1441901094386,
         *         "experienceLevel": 0,
         *         "currency": {},
         *         "statistics": {},
         *         "id": "a17b347b-695b-431f-b1e7-5f783a562310",
         *         "profileId": "a17t347b-692b-43ef-b1e7-5f783a566310",
         *         "newUser": false
         *     }
         * }
         */
        void switchToChildProfile(const char * in_childProfileId, const char * in_childGameId, bool in_forceCreate, IServerCallback * in_callback = NULL);

        /**
         * Switches to a child profile of an app when only one profile exists
         * If multiple profiles exist this returns an error
         *
         * Service Name - Identity
         * Service Operation - SWITCH_TO_CHILD_PROFILE
         *
         * @param in_childGameId The App ID of the child game to switch to
         * @param in_forceCreate Should a new profile be created if it does not exist?
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {
         *         "vcPurchased": 0,
         *         "experiencePoints": 0,
         *         "xpCapped": false,
         *         "playerName": "TestUser",
         *         "vcClaimed": 0,
         *         "rewards": {
         *             "rewardDetails": {},
         *             "rewards": {},
         *             "currency": {
         *                 "credits": {
         *                     "purchased": 0,
         *                     "balance": 0,
         *                     "consumed": 0,
         *                     "awarded": 0
         *                 },
         *                 "gold": {
         *                     "purchased": 0,
         *                     "balance": 0,
         *                     "consumed": 0,
         *                     "awarded": 0
         *                 }
         *             }
         *         },
         *         "loginCount": 1,
         *         "server_time": 1441901094386,
         *         "experienceLevel": 0,
         *         "currency": {},
         *         "statistics": {},
         *         "id": "a17b347b-695b-431f-b1e7-5f783a562310",
         *         "profileId": "a17t347b-692b-43ef-b1e7-5f783a566310",
         *         "newUser": false
         *     }
         * }
         */
        void switchToSingletonChildProfile(const char * in_childGameId, bool in_forceCreate, IServerCallback * in_callback = NULL);

        /**
         * Switch to a Parent Profile
         *
         * Service Name - Identity
         * Service Operation - SWITCH_TO_PARENT_PROFILE
         *
         * @param in_parentLevelName The level of the parent to switch to
         * If null and forceCreate is true a new profile will be created
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {
         *         "profileId": "1d1h32aa-4c41-404f-bc18-29b3fg5wab8a",
         *         "gameId": "123456"
         *     }
         * }
         */
        void switchToParentProfile(const char * in_parentLevelName, IServerCallback * in_callback = NULL);

        /**
         * Returns a list of all child profiles in child Apps
         *
         * Service Name - Identity
         * Service Operation - GET_CHILD_PROFILES
         *
         * @param in_includeSummaryData Whether to return the summary friend data along with this call
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {
         *         "children": [
         *             {
         *                 "appId": "123456",
         *                 "profileId": "b7h32751-befd-4a89-b6da-cd55hs3b2a86",
         *                 "profileName": "Child1",
         *                 "summaryFriendData": null
         *             },
         *             {
         *                 "appId": "123457",
         *                 "profileId": "a17b3432-195b-45hf-b1e7-5f78g3462310",
         *                 "profileName": "Child2",
         *                 "summaryFriendData": null
         *             }
         *         ]
         *     }
         * }
         */
        void getChildProfiles(bool in_includeSummaryData, IServerCallback * in_callback = NULL);

	private:
        void switchToChildProfile(const char * in_childProfileId, const char * in_childGameId, bool in_forceCreate, bool in_forceSingleton, IServerCallback * in_callback = NULL);

		void attachIdentity(const char * in_externalId, const char * in_authenticationToken, const char * in_authenticationType, IServerCallback * in_callback = NULL);
		void mergeIdentity(const char * in_externalId, const char * in_authenticationToken, const char * in_authenticationType, IServerCallback * in_callback = NULL);
		void detachIdentity(const char * in_externalId, const char * in_authenticationType, bool in_continueAnon, IServerCallback * in_callback = NULL);
	};
}
