#pragma once

#include <string>

namespace BrainCloud {

    class OperationParam
    {
    public:
        static const OperationParam Authenticate;

         //Push Notification Service - Send Params
        static const OperationParam PushNotificationSendParamToPlayerId;
        static const OperationParam PushNotificationSendParamNotificationTemplateId;
        static const OperationParam PushNotificationSendParamMessage;
        static const OperationParam PushNotificationSendParamSubstitution;

        //Push Notification Service - Register Params
        static const OperationParam PushNotificationRegisterParamDeviceType;
        static const OperationParam PushNotificationRegisterParamDeviceToken;
        static const OperationParam PushNotificationRegisterParamPushId;
        static const OperationParam PushNotificationRegisterParamToPlayerId;
        static const OperationParam PushNotificationRegisterParamContent;
        static const OperationParam PushNotificationRegisterParamSendInterval;
        static const OperationParam PushNotificationRegisterParamScheduleIn;

        // Twitter Service - Verify Params
        static const OperationParam TwitterServiceVerifyToken;
        static const OperationParam TwitterServiceVerifyVerifier;

        // Twitter Service - Tweet Params
        static const OperationParam TwitterServiceTweetToken;
        static const OperationParam TwitterServiceTweetSecret;
        static const OperationParam TwitterServiceTweetTweet;
        static const OperationParam TwitterServiceTweetPic;

        // Authenticate Service - Authenticate Params
        static const OperationParam AuthenticateServiceAuthenticateAuthenticationType;
        static const OperationParam AuthenticateServiceAuthenticateAuthenticationToken;
        static const OperationParam AuthenticateServiceAuthenticateExternalId;
        static const OperationParam AuthenticateServiceAuthenticateGameId;
        static const OperationParam AuthenticateServiceAuthenticateDeviceId;
        static const OperationParam AuthenticateServiceAuthenticateForceMergeFlag;
        static const OperationParam AuthenticateServiceAuthenticateReleasePlatform;
        static const OperationParam AuthenticateServiceAuthenticateGameVersion;
        static const OperationParam AuthenticateServiceAuthenticateBrainCloudVersion;
        static const OperationParam AuthenticateServiceAuthenticateAuthFacebook;
        static const OperationParam AuthenticateServiceAuthenticateAuthEmail;
        static const OperationParam AuthenticateServiceAuthenticateAuthGameCenter;
        static const OperationParam AuthenticateServiceAuthenticateAuthUniversal;
        static const OperationParam AuthenticateServiceAuthenticateAuthSteam;
        static const OperationParam AuthenticateServiceAuthenticateAuthGoogle;
        static const OperationParam AuthenticateServiceAuthenticateAuthExternal;

        static const OperationParam AuthenticateServiceAuthenticateAuthUpgradeId;
        static const OperationParam AuthenticateServiceAuthenticateAuthAnonymous;
        static const OperationParam AuthenticateServiceAuthenticateAnonymousId;
        static const OperationParam AuthenticateServiceAuthenticateProfileId;
        static const OperationParam AuthenticateServiceAuthenticateForceCreate;
        static const OperationParam AuthenticateServiceAuthenticateExternalAuthName;
        static const OperationParam AuthenticateServiceAuthenticateRegion;
        static const OperationParam AuthenticateServiceAuthenticateCountryCode;
        static const OperationParam AuthenticateServiceAuthenticateLanguageCode;
        static const OperationParam AuthenticateServiceAuthenticateTimeZoneOffset;
        static const OperationParam AuthenticateServiceAuthenticateAuthTwitter;

        // Authenticate Service - Authenticate Params
        static const OperationParam IdentityServiceExternalId;
        static const OperationParam IdentityServiceAuthenticationType;
        static const OperationParam IdentityServiceConfirmAnonymous;

        // Async Match Service - Async Match Params
        static const OperationParam AsyncMatchServicePlayers;
        static const OperationParam AsyncMatchServiceMatchState;
        static const OperationParam AsyncMatchServiceMatchId;
        static const OperationParam AsyncMatchServiceCurrentPlayer;
        static const OperationParam AsyncMatchServiceMatchSummary;
        static const OperationParam AsyncMatchServicePushMessage;
        static const OperationParam AsyncMatchServiceOwnerId;
        static const OperationParam AsyncMatchServiceVersion;
        static const OperationParam AsyncMatchServiceStatistics;
        static const OperationParam AsyncMatchServiceMatchStatus;

        // Entity Service - Create Params
        static const OperationParam EntityServiceCreateEntityId;
        static const OperationParam EntityServiceCreateEntityType;
        static const OperationParam EntityServiceCreateEntitySubtype;
        static const OperationParam EntityServiceCreateData;
        static const OperationParam EntityServiceCreateAcl;
        static const OperationParam EntityServiceCreateFriendData;

        // Entity Service - Update Params
        static const OperationParam EntityServiceUpdateEntityType;
        static const OperationParam EntityServiceUpdateEntityId;
        static const OperationParam EntityServiceUpdateData;
        static const OperationParam EntityServiceUpdateAcl;
        static const OperationParam EntityServiceUpdateVersion;
        static const OperationParam EntityServiceUpdateTargetPlayerId;

        // Entity Service - Update Partial Params
        static const OperationParam EntityServiceUpdatePartialEntityType;
        static const OperationParam EntityServiceUpdatePartialEntityId;
        static const OperationParam EntityServiceUpdatePartialUpdateOps;

        // Entity Service - Get Entities By Type
        static const OperationParam EntityServiceGetEntitiesByTypeEntityType;

        // Entity Service - Delete Params
        static const OperationParam EntityServiceDeleteEntityType;
        static const OperationParam EntityServiceDeleteEntityId;

        // Entity Service - GetSharedEntitiesForPlayerId
        static const OperationParam EntityServiceGetSharedEntitiesTargetPlayerId;
        static const OperationParam EntityServiceGetSharedStatsTargetPlayerId;
        static const OperationParam EntityServiceGetSharedPlayerStateTargetPlayerId;

        // Global Entity Service - Params
        static const OperationParam GlobalEntityServiceEntityId;
        static const OperationParam GlobalEntityServiceEntityType;
        static const OperationParam GlobalEntityServiceIndexedId;
        static const OperationParam GlobalEntityServiceTimeToLive;
        static const OperationParam GlobalEntityServiceData;
        static const OperationParam GlobalEntityServiceAcl;
        static const OperationParam GlobalEntityServiceVersion;
        static const OperationParam GlobalEntityServiceMaxReturn;
        static const OperationParam GlobalEntityServiceWhere;
        static const OperationParam GlobalEntityServiceOrderBy;
        static const OperationParam GlobalEntityServiceContext;
        static const OperationParam GlobalEntityServicePageOffset;

        // Event Service - Send Params
        static const OperationParam EventServiceSendToId;
        static const OperationParam EventServiceSendEventType;
        static const OperationParam EventServiceSendEventId;
        static const OperationParam EventServiceSendEventData;
        static const OperationParam EventServiceSendRecordLocally;

        // Event Service - Update Event Data Params
        static const OperationParam EventServiceUpdateEventDataFromId;
        static const OperationParam EventServiceUpdateEventDataEventId;
        static const OperationParam EventServiceUpdateEventDataData;

        // Event Service - Delete Incoming Params
        static const OperationParam EventServiceDeleteIncomingEventId;
        static const OperationParam EventServiceDeleteIncomingFromId;

        // Event Service - Delete Sent Params
        static const OperationParam EventServiceDeleteSentEventId;
        static const OperationParam EventServiceDeleteSentToId;
        static const OperationParam EventServiceIncludeIncomingEvents;
        static const OperationParam EventServiceIncludeSentEvents;

        // Friend Service - Read Player State Params
        static const OperationParam FriendServiceAuthenticationType;
        static const OperationParam FriendServiceEntityId;
        static const OperationParam FriendServiceExternalId;
        static const OperationParam FriendServiceProfileId;
        static const OperationParam FriendServiceFriendId;
        static const OperationParam FriendServiceEntityType;
        static const OperationParam FriendServiceIncludeSummaryData;
        static const OperationParam FriendServiceEntitySubType;
        static const OperationParam FriendServiceReadPlayerStateFriendId;
        static const OperationParam FriendServiceSearchText;
        static const OperationParam FriendServiceMaxResults;

        //Achievements Event Data Params
        static const OperationParam GamificationServiceAchievementsName;
        static const OperationParam GamificationServiceAchievementsData;
        static const OperationParam GamificationServiceAchievementsGranted;
        static const OperationParam GamificationServiceCategory;
        static const OperationParam GamificationServiceIncludeMetaData;
        static const OperationParam GamificationServiceMilestonesName;

        // Player Statistics Event Params
        static const OperationParam PlayerStatisticEventServiceEventName;
        static const OperationParam PlayerStatisticEventServiceEventMultiplier;
        static const OperationParam PlayerStatisticEventServiceEvents;

        // Player State Service - Read Params
        static const OperationParam PlayerStateServiceReadEntitySubtype;

        // Player State Service - Update Summary Params
        static const OperationParam PlayerStateServiceUpdateSummaryFriendData;

        // Player State Service - Update Name Params
        static const OperationParam PlayerStateServiceUpdateNameData;

        // Player State Service - Atributes
        static const OperationParam PlayerStateServiceAttributes;
        static const OperationParam PlayerStateServiceWipeExisting;
        
        // Player State Service - Atributes
        static const OperationParam PlayerStateServicePictureUrl;
        static const OperationParam PlayerStateServiceContactEmail;

        // Player Statistics Service - Update Increment Params
        static const OperationParam PlayerStatisticsServiceStats;
        static const OperationParam PlayerStatisticsServiceStatNames;
        static const OperationParam PlayerStatisticsExperiencePoints;

        // Player Statistics Service - Read Params
        static const OperationParam PlayerStatisticsServiceReadEntitySubType;

        // Social Leaderboard Service - general parameters
        static const OperationParam SocialLeaderboardServiceLeaderboardId;
        static const OperationParam SocialLeaderboardServiceLeaderboardIds;
        static const OperationParam SocialLeaderboardServiceReplaceName;
        static const OperationParam SocialLeaderboardServiceScore;
        static const OperationParam SocialLeaderboardServiceData;
        static const OperationParam SocialLeaderboardServiceEventName;
        static const OperationParam SocialLeaderboardServiceEventMultiplier;
        static const OperationParam SocialLeaderboardServiceLeaderboardType;
        static const OperationParam SocialLeaderboardServiceRotationType;
        static const OperationParam SocialLeaderboardServiceRotationReset;
        static const OperationParam SocialLeaderboardServiceRetainedCount;
        static const OperationParam SocialLeaderboardServiceFetchType;
        static const OperationParam SocialLeaderboardServiceMaxResults;
        static const OperationParam SocialLeaderboardServiceSortOrder;
        static const OperationParam SocialLeaderboardServiceStartIndex;
        static const OperationParam SocialLeaderboardServiceEndIndex;
        static const OperationParam SocialLeaderboardServiceBeforeCount;
        static const OperationParam SocialLeaderboardServiceAfterCount;
        static const OperationParam SocialLeaderboardServiceIncludeLeaderboardSize;
        static const OperationParam SocialLeaderboardServiceVersionId;
        static const OperationParam SocialLeaderboardServiceLeaderboardResultCount;

        // Product service
        static const OperationParam ProductServiceVCId;
        static const OperationParam ProductServiceVCAmount;

        // Product Service - Get Inventory Params
        static const OperationParam ProductServiceGetInventoryPlatform;
        static const OperationParam ProductServiceGetInventoryUserCurrency;
        static const OperationParam ProductServiceGetInventoryCategory;

        // Product Service - Op Cash In Receipt Params
        static const OperationParam ProductServiceOpCashInReceiptReceipt;
        static const OperationParam ProductServiceOpCashInReceiptUrl;

        // Server Time Service - Read Params
        static const OperationParam ServerTimeServiceRead;

        // data creation params
        static const OperationParam ServiceMessageService;
        static const OperationParam ServiceMessageOperation;
        static const OperationParam ServiceMessageData;

        // data bundle creation Params
        static const OperationParam ServiceMessagePacketId;
        static const OperationParam ServiceMessageProfileId;
        static const OperationParam ServiceMessageSessionId;
        static const OperationParam ServiceMessageMessages;

        // Error Params
        static const OperationParam ServiceMessageReasonCode;
        static const OperationParam ServiceMessageStatusMessage;

        static const OperationParam ReleasePlatformIos;
        static const OperationParam ReleasePlatformFacebook;
        static const OperationParam ReleasePlatformBlackberry;
        static const OperationParam ReleasePlatformGoogleAndroid;
        static const OperationParam ReleasePlatformWindowsPhone;
        static const OperationParam ReleasePlatformWindows;
        static const OperationParam ReleasePlatformMac;
        static const OperationParam ReleasePlatformLinux;

        static const OperationParam ScriptServiceRunScriptName;
        static const OperationParam ScriptServiceRunScriptData;
        static const OperationParam ScriptServiceStartDateUTC;
        static const OperationParam ScriptServiceStartMinutesFromNow;

        static const OperationParam MatchMakingServicePlayerRating;
        static const OperationParam MatchMakingServiceMinutes;
        static const OperationParam MatchMakingServiceRangeDelta;
        static const OperationParam MatchMakingServiceNumMatches;
        static const OperationParam MatchMakingServiceExtraParams;
        static const OperationParam MatchMakingServicePlayerId;
        static const OperationParam MatchMakingServicePlaybackStreamId;

        static const OperationParam OneWayMatchServicePlayerId;
        static const OperationParam OneWayMatchServiceRangeDelta;
        static const OperationParam OneWayMatchServicePlaybackStreamId;

        static const OperationParam PlaybackStreamServicePlaybackStreamId;
        static const OperationParam PlaybackStreamServiceTargetPlayerId;
        static const OperationParam PlaybackStreamServiceInitiatingPlayerId;
        static const OperationParam PlaybackStreamServiceIncludeSharedData;
        static const OperationParam PlaybackStreamServiceEventData;
        static const OperationParam PlaybackStreamServiceSummary;

        static const OperationParam ProductServiceTransId;
        static const OperationParam ProductServiceLanguage;
        static const OperationParam ProductServiceItemId;
        static const OperationParam ProductServiceReceipt;
        static const OperationParam ProductServiceOrderId;
        static const OperationParam ProductServiceProductId;
        static const OperationParam ProductServiceToken;

        // S3 Handling
        static const OperationParam S3HandlingServiceCategory;
        static const OperationParam S3HandlingServiceFileDetails;

        // Shared Identity
        static const OperationParam AuthenticationServiceAuthenticationLevelName;
        static const OperationParam ScriptServiceParentLevelName;
        static const OperationParam PlayerStateServiceIncludeSummaryData;
        static const OperationParam IdentityServiceForceSingleton;
        
        // Redemption Code
        static const OperationParam RedemptionCodeServiceScanCode;
        static const OperationParam RedemptionCodeServiceCodeType;
        static const OperationParam RedemptionCodeServiceCustomRedemptionInfo;
        
        // DataStream
        static const OperationParam DataStreamEventName;
        static const OperationParam DataStreamEventProperties;
        
        // Profanity
        static const OperationParam ProfanityText;
        static const OperationParam ProfanityReplaceSymbol;
        static const OperationParam ProfanityFlagEmail;
        static const OperationParam ProfanityFlagPhone;
        static const OperationParam ProfanityFlagUrls;
        static const OperationParam ProfanityLanguages;
        
        // Upload
        static const OperationParam UploadLocalPath;
        static const OperationParam UploadCloudPath;
        static const OperationParam UploadCloudFilename;
        static const OperationParam UploadShareable;
        static const OperationParam UploadReplaceIfExists;
        static const OperationParam UploadFileSize;
        static const OperationParam UploadRecurse;
        
        /*
         OfflineMatchServicePlayerId                        = new OperationParam("playerId");
          static const OperationParam OfflineMatchServiceRangeDelta                      = new OperationParam("rangeDelta");
          static const OperationParam OfflineMatchServicePlaybackStreamId
         */

        std::string getValue() const { return _value; }

        bool operator== (const OperationParam& s) const;
        void operator= (const OperationParam& s);

    private:
        std::string _value;

        OperationParam(const char * value);
    };

}