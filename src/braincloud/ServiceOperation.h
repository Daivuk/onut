#pragma once

#include <string>

namespace BrainCloud {

	class ServiceOperation
	{
	public:
        static const ServiceOperation None;
        
		static const ServiceOperation Authenticate;
		static const ServiceOperation Attach;
		static const ServiceOperation Merge;
		static const ServiceOperation Detach;
		static const ServiceOperation ResetEmailPassword;
        static const ServiceOperation SwitchToChildProfile;
        static const ServiceOperation SwitchToParentProfile;
        static const ServiceOperation GetChildProfiles;

		static const ServiceOperation Create;
		static const ServiceOperation CreateWithIndexedId;
		static const ServiceOperation Reset;
		static const ServiceOperation Read;
        static const ServiceOperation ReadSingleton;
		static const ServiceOperation ReadByType;
		static const ServiceOperation Verify;
		static const ServiceOperation ReadShared;
        
        // push notification
		static const ServiceOperation Deregister;
		static const ServiceOperation DeregisterAll;
		static const ServiceOperation Register;
        static const ServiceOperation SendSimple;
        static const ServiceOperation SendRich;
		
		static const ServiceOperation Trigger;
		static const ServiceOperation TriggerMultiple;
		static const ServiceOperation TriggerGrantExperience;
		
		static const ServiceOperation FullReset;
		static const ServiceOperation DataReset;
		
		static const ServiceOperation Update;
		static const ServiceOperation UpdatePartial;
        static const ServiceOperation UpdateShared;
		static const ServiceOperation UpdateSingleton;
		static const ServiceOperation UpdateAcl;
		static const ServiceOperation UpdateTimeToLive;
		static const ServiceOperation Delete;
		static const ServiceOperation DeleteSingleton;
		static const ServiceOperation UpdateSummary;
		static const ServiceOperation UpdateSetMinimum;
		static const ServiceOperation UpdateIncrementToMaximum;
		
		static const ServiceOperation GetFriendProfileInfoForExternalId;
        static const ServiceOperation GetExternalIdForProfileId;
		static const ServiceOperation ReadFriendEntity;
		static const ServiceOperation ReadFriendsEntities;
		static const ServiceOperation ReadFriendsWithApplication;
		static const ServiceOperation ReadFriends;
		static const ServiceOperation ReadFriendsPlayerState;
        static const ServiceOperation FindPlayerByName;
		static const ServiceOperation GetSocialLeaderboard;
        static const ServiceOperation GetMultiSocialLeaderboard;
		static const ServiceOperation GetGlobalLeaderboard;
		static const ServiceOperation GetGlobalLeaderboardPage;
		static const ServiceOperation GetGlobalLeaderboardView;
		static const ServiceOperation GetGlobalLeaderboardVersions;
		static const ServiceOperation GetCompletedTournament;
		static const ServiceOperation RewardTournament;
		static const ServiceOperation PostScore;
		static const ServiceOperation PostScoreDynamic;
		
		static const ServiceOperation InitThirdParty;
		static const ServiceOperation PostThirdPartyLeaderboardScore;
		static const ServiceOperation IncrementThirdPartyLeaderboardScore;
		static const ServiceOperation LaunchAchievementUI;
		static const ServiceOperation PostThirdPartyLeaderboardAchievement;
		static const ServiceOperation IsThirdPartyAchievementComplete;
		static const ServiceOperation ResetThirdPartyAchievements;
		static const ServiceOperation QueryThirdPartyAchievements;
		
		static const ServiceOperation GetInventory;
		static const ServiceOperation CashInReceipt;
		static const ServiceOperation AwardVC;
		static const ServiceOperation ConsumeVC;
		static const ServiceOperation GetPlayerVC;
		static const ServiceOperation ResetPlayerVC;
        
        static const ServiceOperation AwardParentVC;
        static const ServiceOperation ConsumeParentVC;
        static const ServiceOperation GetParentVC;
        static const ServiceOperation ResetParentVC;
		
		static const ServiceOperation Send;
		static const ServiceOperation UpdateEventData;
		static const ServiceOperation DeleteSent;
		static const ServiceOperation DeleteIncoming;
        static const ServiceOperation GetEvents;
		
		static const ServiceOperation UpdateIncrement;
		static const ServiceOperation ReadNextXplevel;
		static const ServiceOperation ReadXpLevels;
		static const ServiceOperation ReadSubset;
		static const ServiceOperation ReadForCategory;
		
		static const ServiceOperation SetXpPoints;
		
		static const ServiceOperation Run;
		static const ServiceOperation Tweet;
		
		static const ServiceOperation AwardAchievements;
		static const ServiceOperation ReadAchievements;
		static const ServiceOperation ReadAchievedAchievements;
		
		static const ServiceOperation SetPlayerRating;
		static const ServiceOperation ResetPlayerRating;
		static const ServiceOperation IncrementPlayerRating;
		static const ServiceOperation DecrementPlayerRating;
		static const ServiceOperation ShieldOn;
		static const ServiceOperation ShieldOnFor;
		static const ServiceOperation ShieldOff;
		static const ServiceOperation GetShieldExpiry;
		static const ServiceOperation FindPlayers;
		static const ServiceOperation FindPlayersUsingFilter;
		
		static const ServiceOperation SubmitTurn;
		static const ServiceOperation UpdateMatchSummary;
		static const ServiceOperation Abandon;
		static const ServiceOperation Complete;
		static const ServiceOperation ReadMatch;
		static const ServiceOperation ReadMatchHistory;
		static const ServiceOperation FindMatches;
		static const ServiceOperation FindMatchesCompleted;
		static const ServiceOperation DeleteMatch;
		
		static const ServiceOperation LastUploadStatus;
		
		static const ServiceOperation ReadQuests;
		static const ServiceOperation ReadCompletedQuests;
		static const ServiceOperation ReadInProgressQuests;
		static const ServiceOperation ReadNotStartedQuests;
		static const ServiceOperation ReadQuestsWithStatus;
		static const ServiceOperation ReadQuestsWithBasicPercentage;
		static const ServiceOperation ReadQuestsWithComplexPercentage;
		static const ServiceOperation ReadQuestsByCategory;
		
		static const ServiceOperation ReadGameStatisticsByCategory;
		static const ServiceOperation ReadPlayerStatisticsByCategory;
		
		static const ServiceOperation ReadMilestones;
		static const ServiceOperation ReadMilestonesByCategory;
		static const ServiceOperation ResetMilestones;
		
		static const ServiceOperation ReadCompletedMilestones;
		static const ServiceOperation ReadInProgressMilestones;
		
		static const ServiceOperation Logout;
		static const ServiceOperation UpdateName;
		
		static const ServiceOperation StartMatch;
		static const ServiceOperation CancelMatch;
		static const ServiceOperation CompleteMatch;
		static const ServiceOperation EnableMatchMaking;

		static const ServiceOperation StartStream;
		static const ServiceOperation ReadStream;
		static const ServiceOperation EndStream;
		static const ServiceOperation DeleteStream;
		static const ServiceOperation AddEvent;
		static const ServiceOperation GetStreamSummariesForInitiatingPlayer;
		static const ServiceOperation GetStreamSummariesForTargetPlayer;

		static const ServiceOperation GetUserInfo;
		
		static const ServiceOperation StartSteamTransaction;
		static const ServiceOperation FinalizeSteamTransaction;
		static const ServiceOperation VerifyMicrosoftReceipt;
		static const ServiceOperation ConfirmGooglePlayPurchase;
		static const ServiceOperation EligiblePromotions;

		static const ServiceOperation GetList;
		static const ServiceOperation GetListByIndexedId;
		static const ServiceOperation GetListCount;
        static const ServiceOperation GetPage;
        static const ServiceOperation GetPageOffset;
		
		static const ServiceOperation GetAttributes;
		static const ServiceOperation UpdateAttributes;
		static const ServiceOperation RemoveAttributes;
        static const ServiceOperation UpdatePlayerPicture;
        static const ServiceOperation UpdateContactEmail;

		static const ServiceOperation ReadProperties;
        
        static const ServiceOperation GetUpdatedFiles;
        static const ServiceOperation GetFileList;
        
        static const ServiceOperation ScheduleCloudScript;
        static const ServiceOperation RunParentScript;
        
        static const ServiceOperation RedeemCode;
        static const ServiceOperation GetRedeemedCodes;
        
        static const ServiceOperation CustomPageEvent;
        static const ServiceOperation CustomScreenEvent;
        static const ServiceOperation CustomTrackEvent;
        
        // profanity
        static const ServiceOperation ProfanityCheck;
        static const ServiceOperation ProfanityReplaceText;
        static const ServiceOperation ProfanityIdenitfyBadWords;
        
        // file
        static const ServiceOperation PrepareUserUpload;
        static const ServiceOperation ListUserFiles;
        static const ServiceOperation DeleteUserFiles;

		std::string getValue() const { return _value; }
		
		bool operator== (const ServiceOperation& s) const;
        bool operator!= (const ServiceOperation& s) const;
        void operator= (const ServiceOperation& s);
		
	private:
		std::string _value;
		
		ServiceOperation(const char * value);
	};
	
}