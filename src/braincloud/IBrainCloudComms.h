#pragma once

#include "BrainCloudTypes.h"

#include "IEventCallback.h"
#include "IFileUploadCallback.h"
#include "IGlobalErrorCallback.h"
#include "INetworkErrorCallback.h"
#include "IRewardCallback.h"
#include "IServerCallback.h"

#include "ServerCall.h"
#include "ServiceName.h"
#include "ServiceOperation.h"

#define MAX_BUNDLE_SIZE 50
#define NO_PACKET_EXPECTED -1
#define DEFAULT_AUTHENTICATION_TIMEOUT_MILLIS 15000


namespace BrainCloud {

    class IFileUploader;

	struct BrainCloudCallbackEvent
	{
		IServerCallback* callback;
		ServiceName m_service;
		ServiceOperation m_operation;
		bool m_error;
		unsigned short m_retryCount;
		int m_statusCode;
        int m_reasonCode;
		std::string m_data;

        BrainCloudCallbackEvent()
            : m_service(ServiceName::None)
		    , m_operation(ServiceOperation::None)
		    , m_error(false)
            , m_retryCount(0)
            , m_statusCode(0)
            , m_reasonCode(0)
        {
        }
	};
    
    enum UploadStatus
    {
        NONE = 0,
        UPLOADING = 1,
        CANCELLING = 2,
        FAILED = 3,
        COMPLETE = 4,
    };
    
    // although named as an interface, this is actually an abstract class
	class IBrainCloudComms
	{
	public:
        
        // pure virtual methods
        IBrainCloudComms();
        virtual ~IBrainCloudComms();
        

		virtual void initialize(const char * serverURL, const char * gameId, const char * secretKey) = 0;
		virtual void addToQueue(ServerCall *) = 0;

        virtual void sendHeartbeat() = 0;
		virtual void resetCommunication() = 0;
        virtual void shutdown() = 0;
		virtual void runCallbacks() = 0;

        virtual void registerEventCallback(IEventCallback *in_eventCallback) = 0;
        virtual void deregisterEventCallback() = 0;
        
        virtual void registerFileUploadCallback(IFileUploadCallback *in_fileUploadCallback) = 0;
        virtual void deregisterFileUploadCallback() = 0;
        
        virtual void registerRewardCallback(IRewardCallback *in_rewardCallback) = 0;
        virtual void deregisterRewardCallback() = 0;
        
        virtual void registerGlobalErrorCallback(IGlobalErrorCallback *in_globalErrorCallback) = 0;
        virtual void deregisterGlobalErrorCallback() = 0;
        
        virtual void registerNetworkErrorCallback(INetworkErrorCallback * in_networkErrorCallback) = 0;
        virtual void deregisterNetworkErrorCallback() = 0;
        
        virtual void cancelUpload(const char * in_fileUploadId) = 0;
        virtual double getUploadProgress(const char * in_fileUploadId) = 0;
        virtual int64_t getUploadTotalBytesToTransfer(const char * in_fileUploadId) = 0;
        virtual int64_t getUploadBytesTransferred(const char * in_fileUploadId) = 0;
       
        virtual void enableNetworkErrorMessageCaching(bool in_enabled) = 0;
        virtual void retryCachedMessages() = 0;
        virtual void flushCachedMessages(bool in_sendApiErrorCallbacks) = 0;
        
        // implemented methods
        void enableLogging(bool shouldEnable);
		bool isInitialized();
        bool isAuthenticated();
        
		void setHeartbeatInterval(int milliseconds);
        void setImmediateRetryOnError(bool value);
        const std::string& getGameId() const;
		void clearSessionId();
		const std::string& getSessionId() const;
		void setSessionId(const char *);
		void setServerUrl(const char *);
        
        const std::vector<int> & getPacketTimeouts();
        void setPacketTimeouts(const std::vector<int> & in_packetTimeouts);
        void setPacketTimeoutsToDefault();
        void setAuthenticationPacketTimeout(int in_timeoutSecs);
        int getAuthenticationPacketTimeout();
        void setOldStyleStatusMessageErrorCallback(bool in_enabled);
        void setErrorCallbackOn202Status(bool in_isError);

        int getUploadLowTransferRateTimeout();
        void setUploadLowTransferRateTimeout(int in_timeoutSecs);
        int getUploadLowTransferRateThreshold();
        void setUploadLowTransferRateThreshold(int in_bytesPerSec);
        
        static void createJsonErrorResponse(int in_statusCode,
                                            int in_reasonCode,
                                            const std::string & in_statusMessage,
                                            std::string & out_jsonErrorResponse);
        
    protected:

        int64_t _packetId;
        int64_t _expectedPacketId; // the next packetId we're expecting or -1 if nothing is in transit
        int _retryCount;
        int64_t _packetSendTimeMillis;
		bool _isInitialized;
        bool _isAuthenticated;
        int _heartbeatInterval;
        bool _immediateRetryOnError;
        bool _loggingEnabled;
        int _authenticationTimeoutMillis;
        bool _oldStyleStatusMessageErrorCallback;
        bool _cacheMessagesOnNetworkError;
        bool _errorCallbackOn202;
        
        /// This flag is set when _cacheMessagesOnNetworkError is true
        /// and a network error occurs. It is reset when a call is made
        /// to either retryCachedMessages or flushCachedMessages
        bool _blockingQueue;

        IEventCallback *_eventCallback;
        IFileUploadCallback *_fileUploadCallback;
        IGlobalErrorCallback * _globalErrorCallback;
        IRewardCallback *_rewardCallback;
        INetworkErrorCallback *_networkErrorCallback;
        
        std::string _gameId;
        std::string _sessionId;
        std::string _serverUrl;
        std::string _uploadUrl;
        std::string _secretKey;
        std::vector<int> _packetTimeouts;
        
        typedef std::map<std::string, IFileUploader *>::iterator tFileUploadsIterator;
        std::map<std::string, IFileUploader *> _fileUploads;
        int _uploadLowTransferRateTimeoutSecs;
        int _uploadLowTransferRateThresholdBytesPerSec;
        int _uploadOverallTimeoutSecs;
        
        int32_t _statusCodeCache;
        int32_t _reasonCodeCache;
        std::string _statusMessageCache;
        
        void setCredentials( const Json::Value& in_jsonAuthenticationResponse );
        void filterIncomingMessages(const ServerCall* servercall, const Json::Value& response);
        
        virtual void startFileUpload(const Json::Value & in_jsonPrepareUploadResponse) = 0;
        void runCallbacksFileUpload();
	};

};
