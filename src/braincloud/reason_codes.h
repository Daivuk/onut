#ifndef _REASONCODES_H_
#define _REASONCODES_H_

// Error codes
static const int NO_REASON_CODE = 0;

static const int INVALID_NOTIFICATION = 20200;

static const int INVALID_REQUEST = 40001;

// 402
static const int SWITCHING_FACEBOOK_MEMORY = 40201;
static const int MERGING_MEMORY = 40202;
static const int RESETING_MEMORY = 40203;
static const int MOVING_ANONYMOUS_MEMORY = 40204;
static const int LOGIN_SECURITY_ERROR = 40205;
static const int MISSING_IDENTITY_ERROR = 40206;
static const int SWITCHING_PROFILES = 40207;
static const int MISSING_PROFILE_ERROR = 40208;

static const int SECURITY_ERROR = 40209;

static const int DOWNGRADING_TO_ANONYMOUS_ERROR = 40210;
static const int DUPLICATE_IDENTITY_TYPE = 40211;
static const int MERGE_PROFILES = 40212;
static const int INVALID_PROPERTY_NAME = 40213;
static const int EMAIL_NOT_VALIDATED = 40214;


// Forbidden (403)... 
static const int UNABLE_TO_GET_FRIENDS_FROM_FACEBOOK = 40300;
static const int BAD_SIGNATURE = 40301;
static const int UNABLE_TO_VALIDATE_PLAYER = 40302;
static const int PLAYER_SESSION_EXPIRED = 40303;
static const int NO_SESSION = 40304;
static const int PLAYER_SESSION_MISMATCH = 40305;
static const int OPERATION_REQUIRES_A_SESSION = 40306;
static const int TOKEN_DOES_NOT_MATCH_USER = 40307;
static const int EVENT_CAN_ONLY_SEND_TO_FRIEND_OR_SELF = 40309;
static const int NOT_FRIENDS = 40310;
static const int VC_BALANCE_CANNOT_BE_SPECIFIED = 40311;
static const int VC_LIMIT_EXCEEDED = 40312;
static const int UNABLE_TO_GET_MY_DATA_FROM_FACEBOOK = 40313;
static const int INVALID_AUTHENTICATION_TYPE = 40315;
static const int INVALID_GAME_ID = 40316;


// This product and receipt have already been claimed
static const int APPLE_TRANS_ID_ALREADY_CLAIMED = 40317;
// @deprecated - use APPLE_TRANS_ID_ALREADY_CLAIMED
static const int ITUNES_PURCHASE_ALREADY_CLAIMED = APPLE_TRANS_ID_ALREADY_CLAIMED;

// 400 Bad version
static const int CLIENT_VERSION_NOT_SUPPORTED = 40318;
static const int BRAINCLOUD_VERSION_NOT_SUPPORTED = 40319;
static const int PLATFORM_NOT_SUPPORTED = 40320;

static const int INVALID_PLAYER_STATISTICS_EVENT_NAME = 40321;

// Game Version No Longer Supported
static const int GAME_VERSION_NOT_SUPPORTED = 40322;
// @deprecated - use GAME_VERSION_NOT_SUPPORTED
static const int APP_VERSION_NO_LONGER_SUPPORTED = GAME_VERSION_NOT_SUPPORTED;

static const int BAD_REFERENCE_DATA = 40324;
static const int MISSING_OAUTH_TOKEN = 40325;
static const int MISSING_OAUTH_VERIFIER = 40326;
static const int MISSING_OAUTH_TOKEN_SECRET = 40327;
static const int MISSING_TWEET = 40328;
static const int FACEBOOK_PAYMENT_ID_ALREADY_PROCESSED = 40329;
static const int DISABLED_GAME = 40330;
static const int MATCH_MAKING_DISABLED = 40331;
static const int UPDATE_FAILED = 40332;
static const int INVALID_OPERATION = 40333;	// invalid operation for API call
static const int MATCH_RANGE_ERROR = 40334;
static const int PLAYER_IN_MATCH = 40335;
static const int MATCH_PLAYER_SHIELDED = 40336;
static const int MATCH_PLAYER_MISSING = 40337;
static const int MATCH_PLAYER_LOGGED_IN = 40338;
static const int INVALID_ITEM_ID = 40339;
static const int MISSING_PRICE = 40340;
static const int MISSING_USER_INFO = 40341;
static const int MISSING_STEAM_RESPONSE = 40342;
static const int MISSING_STEAM_TRANSACTION = 40343;
static const int ENTITY_VERSION_MISMATCH = 40344;
static const int MISSING_RECORD = 40345;
static const int INSUFFICIENT_PERMISSIONS = 40346;
static const int MISSING_IN_QUERY = 40347;
static const int RECORD_EXPIRED = 40348;
static const int INVALID_WHERE = 40349;
static const int S3_ERROR = 40350;
static const int INVALID_ATTRIBUTES = 40351;
static const int IMPORT_MISSING_GAME_DATA = 40352;
static const int IMPORT_SCHEMA_VERSION_TOO_OLD = 40353;
static const int IMPORT_SCHEMA_VERSION_INVALID = 40355;
static const int PLAYER_SESSION_LOGGED_OUT = 40356;
static const int API_HOOK_SCRIPT_ERROR = 40357;
static const int MISSING_REQUIRED_PARAMETER = 40358;
static const int INVALID_PARAMETER_TYPE = 40359;
static const int INVALID_IDENTITY_TYPE = 40360;
static const int EMAIL_SEND_ERROR = 40361;
static const int CHILD_ENTITY_PARTIAL_UPDATE_INVALID_DATA = 40362;
static const int MISSING_SCRIPT = 40363;
static const int SCRIPT_SECURITY_ERROR = 40364;
static const int SERVER_SESSION_EXPIRED = 40365;
static const int STREAM_DOES_NOT_EXIT = 40366;
static const int STREAM_ACCESS_ERROR = 40367;
static const int STREAM_COMPLETE = 40368;
static const int INVALID_STATISTIC_NAME = 40369;
static const int INVALID_HTTP_REQUEST = 40370;
static const int GAME_LIMIT_REACHED = 40371;
static const int GAME_RUNSTATE_DISABLED = 40372;
static const int INVALID_COMPANY_ID = 40373;
static const int INVALID_PLAYER_ID = 40374;
static const int INVALID_TEMPLATE_ID = 40375;
static const int MINIMUM_SEARCH_INPUT = 40376;
static const int MISSING_GAME_PARENT = 40377;
static const int GAME_PARENT_MISMATCH = 40378;
static const int CHILD_PLAYER_MISSING = 40379;
static const int MISSING_PLAYER_PARENT = 40380;
static const int PLAYER_PARENT_MISMATCH = 40381;
static const int MISSING_PLAYER_ID = 40382;
static const int DECODE_CONTEXT = 40383;
static const int INVALID_QUERY_CONTEXT = 40384;
static const int GROUP_MEMBER_NOT_FOUND = 40385;
static const int INVALID_SORT = 40386;
static const int GAME_NOT_FOUND = 40387;
static const int GAMES_NOT_IN_SAME_COMPANY = 40388;
static const int IMPORT_NO_PARENT_ASSIGNED = 40389;
static const int IMPORT_PARENT_CURRENCIES_MISMATCH = 40390;
static const int INVALID_SUBSTITUION_ENTRY = 40391;
static const int INVALID_TEMPLATE_STRING = 40392;
static const int TEMPLATE_SUBSTITUTION_ERROR = 40393;
static const int INVALID_OPPONENTS = 40394;
static const int REDEMPTION_CODE_NOT_FOUND = 40395;
static const int REDEMPTION_CODE_VERSION_MISMATCH = 40396;
static const int REDEMPTION_CODE_ACTIVE = 40397;
static const int REDEMPTION_CODE_NOT_ACTIVE = 40398;
static const int REDEMPTION_CODE_TYPE_NOT_FOUND = 40399;
static const int REDEMPTION_CODE_INVALID = 40400;
static const int REDEMPTION_CODE_REDEEMED = 40401;
static const int REDEMPTION_CODE_REDEEMED_BY_SELF = 40402;
static const int REDEMPTION_CODE_REDEEMED_BY_OTHER = 40403;
static const int SCRIPT_EMPTY = 40404;
static const int ITUNES_COMMUNICATION_ERROR = 40405;
static const int ITUNES_NO_RESPONSE = 40406;
static const int ITUNES_RESPONSE_NOT_OK = 40407;
static const int JSON_PARSING_ERROR = 40408;
static const int ITUNES_NULL_RESPONSE = 40409;
static const int ITUNES_RESPONSE_WITH_NULL_STATUS = 40410;
static const int ITUNES_STATUS_BAD_JSON_RECEIPT = 40411;
static const int ITUNES_STATUS_BAD_RECEIPT = 40412;
static const int ITUNES_STATUS_RECEIPT_NOT_AUTHENTICATED = 40413;
static const int ITUNES_STATUS_BAD_SHARED_SECRET = 40414;
static const int ITUNES_STATUS_RECEIPT_SERVER_UNAVAILABLE = 40415;
static const int ITUNES_RECEIPT_MISSING_ITUNES_PRODUCT_ID = 40416;
static const int PRODUCT_NOT_FOUND_FOR_ITUNES_PRODUCT_ID = 40417;
static const int DATA_STREAM_EVENTS_NOT_ENABLED = 40418;
static const int INVALID_DEVICE_TOKEN = 40419;
static const int ERROR_DELETING_DEVICE_TOKEN = 40420;
static const int WEBPURIFY_NOT_CONFIGURED = 40421;
static const int WEBPURIFY_EXCEPTION = 40422;
static const int WEBPURIFY_FAILURE = 40423;
static const int WEBPURIFY_NOT_ENABLED = 40424;
static const int NAME_CONTAINS_PROFANITY = 40425;
static const int NULL_SESSION = 40426;
static const int PURCHASE_ALREADY_VERIFIED = 40427;
static const int GOOGLE_IAP_NOT_CONFIGURED = 40428;
static const int UPLOAD_FILE_TOO_LARGE = 40429;
static const int FILE_ALREADY_EXISTS = 40430;
static const int CLOUD_STORAGE_SERVICE_ERROR = 40431;
static const int FILE_DOES_NOT_EXIST = 40432;
static const int UPLOAD_ID_MISSING = 40433;
static const int UPLOAD_JOB_MISSING = 40434;
static const int UPLOAD_JOB_EXPIRED = 40435;
static const int UPLOADER_EXCEPTION = 40436;
static const int UPLOADER_FILESIZE_MISMATCH = 40437;

static const int NO_TWITTER_CONSUMER_KEY = 500001;
static const int NO_TWITTER_CONSUMER_SECRET = 500002;
static const int INVALID_CONFIGURATION = 500003;
static const int ERROR_GETTING_REQUEST_TOKEN = 500004;
static const int ERROR_GETTING_ACCESS_TOKEN = 500005;

static const int FACEBOOK_ERROR = 500010;
static const int FACEBOOK_SECRET_MISMATCH = 500011;
static const int FACEBOOK_AUTHENTICATION_ERROR = 500012;
static const int FACEBOOK_APPLICATION_TOKEN_REQUEST_ERROR = 500013;
static const int FACEBOOK_BAD_APPLICATION_TOKEN_SIGNATURE = 500014;


// Client defined value for a timeout detected client-side.
static const int CLIENT_NETWORK_ERROR_TIMEOUT = 90001;
static const int CLIENT_UPLOAD_FILE_CANCELLED = 90100;
static const int CLIENT_UPLOAD_FILE_TIMED_OUT = 90101;
static const int CLIENT_UPLOAD_FILE_UNKNOWN = 90102;

#endif
