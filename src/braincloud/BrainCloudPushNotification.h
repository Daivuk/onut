#pragma once

#include <string>

#include "IServerCallback.h"
#include "ServiceName.h"
#include "ServiceOperation.h"
#include "OperationParam.h"
#include "BrainCloudTypes.h"
#include "Platform.h"

namespace BrainCloud {

    class BrainCloudPushNotification
    {
        public:

        /**
         * Deregisters all device tokens currently registered to the player.
         *
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return JSON as follows:
         * {
         *   "status":200,
         *   "data":null
         * }
         */
        void deregisterAllPushNotificationDeviceTokens(IServerCallback * in_callback = NULL);
        
        /**
         * Deregisters the given device token from the server to disable this device
         * from receiving push notifications.
         *
         * @param in_device The device platform being deregistered.
         * @param in_token The platform-dependant device token needed for push notifications.
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return JSON as follows:
         * {
         *   "status":200,
         *   "data":null
         * }
         */
        void deregisterPushNotificationDeviceToken(const Platform & in_platform, const char * in_token, IServerCallback * in_callback = NULL);
        
        /**
         * Registers the given device token with the server to enable this device
         * to receive push notifications.
         *
         * @param in_platform The device platform
         * @param in_deviceToken The platform-dependant device token needed for push notifications.
         *  On IOS, this is obtained using the application:didRegisterForRemoteNotificationsWithDeviceToken callback
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return JSON describing the new value of the statistics and any rewards that were triggered:
         * {
         *   "status":200,
         *   "data":null
         * }
         */
        void registerPushNotificationDeviceToken( const Platform & in_platform, const char * in_deviceToken, IServerCallback * in_callback = NULL );

        /**
         * Registers the given device token with the server to enable this device
         * to receive push notifications.
         *
         * @param deviceType The type of device (see DEVICE_TYPE_* constants)
         * @param deviceToken The platform-dependant device token needed for push notifications.
         *  On IOS, this is obtained using the application:didRegisterForRemoteNotificationsWithDeviceToken callback
         * @param callback The method to be invoked when the server response is received
         *
         * @return JSON describing the new value of the statistics and any rewards that were triggered:
         * {
         *   "status":200,
         *   "data":null
         * }
         */
        void registerPushNotificationDeviceToken( const char * in_deviceType, const char * in_deviceToken, IServerCallback * in_callback = NULL );

        /**
         * Sends a simple push notification based on the passed in message.
         * NOTE: It is possible to send a push notification to oneself.
         *
         * @param in_toPlayerId The braincloud playerId of the user to receive the notification
         * @param in_message Text of the push notification
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return JSON describing the result
         * {
         *   "status":200,
         *   "data":null
         * }
         */
        void sendSimplePushNotification(const char * in_toPlayerId, const char * in_message, IServerCallback * in_callback = NULL );

        /**
         * Sends a notification to a user based on a brainCloud portal configured notification template.
         * NOTE: It is possible to send a push notification to oneself.
         *
         * @param in_toPlayerId The braincloud playerId of the user to receive the notification
         * @param in_notificationTemplateId Id of the notification template
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return JSON describing the result
         * {
         *   "status":200,
         *   "data":null
         * }
         */
        void sendRichPushNotification(const char * in_toPlayerId, int32_t in_notificationTemplateId, IServerCallback * in_callback = NULL );

        /**
         * Sends a notification to a user based on a brainCloud portal configured notification template.
         * Includes JSON defining the substitution params to use with the template.
         * See the Portal documentation for more info.
         * NOTE: It is possible to send a push notification to oneself.
         *
         * @param in_toPlayerId The braincloud playerId of the user to receive the notification
         * @param in_notificationTemplateId Id of the notification template
         * @param in_substitutionJson JSON defining the substitution params to use with the template
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return JSON describing the result
         * {
         *   "status":200,
         *   "data":null
         * }
         */
        void sendRichPushNotificationWithParams(const char * in_toPlayerId, int32_t in_notificationTemplateId, const char * in_substitutionJson, IServerCallback * in_callback = NULL );

    private:
        void sendRichPushNotification(const char * in_toPlayerId, int32_t in_notificationTemplateId, const char * in_substitutionJson, IServerCallback * in_callback = NULL );
    };
}