#pragma once

#include <string>

namespace BrainCloud {
	
	class ServiceName
	{
	public:
        static const ServiceName None;
        
		static const ServiceName Authenticate;
		static const ServiceName AuthenticateV2;
		static const ServiceName Identity;
		static const ServiceName Currency;
		static const ServiceName FriendData;
		static const ServiceName HeartBeat;
		static const ServiceName ServerTime;
		
		static const ServiceName PushNotification;
		static const ServiceName GlobalGameStatistics;
		static const ServiceName PlayerStatisticsEvent;
		static const ServiceName Twitter;
		static const ServiceName Steam;
		
		static const ServiceName PlayerState;
		static const ServiceName Entity;
		static const ServiceName GlobalEntity;
		static const ServiceName Friend;
		static const ServiceName Time;
		
		static const ServiceName SocialLeaderboard;
		static const ServiceName Event;
		static const ServiceName Product;
		static const ServiceName PlayerStatistics;
		static const ServiceName GlobalStatistics;
		static const ServiceName AsyncMatch;
		
		static const ServiceName Script;
		static const ServiceName MatchMaking;
		static const ServiceName OneWayMatch;
		static const ServiceName PlaybackStream;
		static const ServiceName Gamification;

		static const ServiceName GlobalApp;
        static const ServiceName S3Handling;
        static const ServiceName RedemptionCode;
        static const ServiceName DataStream;
        static const ServiceName Profanity;
        static const ServiceName File;
        
		
		std::string getValue() const { return _value; }
		
		bool operator== (const ServiceName& s) const;
        void operator= (const ServiceName& s);

	private:
		std::string _value;
		
		ServiceName(const char * value);
	};
	
}
