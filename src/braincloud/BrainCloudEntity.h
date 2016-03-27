#pragma once

#include <string>

#include "IServerCallback.h"
#include "ServiceName.h"
#include "ServiceOperation.h"
#include "OperationParam.h"
#include "BrainCloudTypes.h"

namespace BrainCloud {

	class BrainCloudEntity
	{
	public:
		/**
		 * Method creates a new entity on the server.
		 *
		 * Service Name - Entity
		 * Service Operation - Create
		 *
		 * @param in_entityType The entity type as defined by the user
		 * @param in_jsonEntityData	The entity's data as a json string
		 * @param in_jsonEntityAcl The entity's access control list as json. A null acl implies default
		 * permissions which make the entity readable/writeable by only the player.
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 * @return The JSON returned in the callback is as follows:
		 * {
		 *   "status":200,
		 *   "data":null
		 * }
		 */
		void createEntity( const char * in_entityType, const std::string& in_jsonEntityData, const std::string& in_jsonEntityAcl, IServerCallback * in_callback = NULL);
		
		/**
		 * Method updates a new entity on the server. This operation results in the entity
		 * data being completely replaced by the passed in JSON string.
		 *
		 * Service Name - Entity
		 * Service Operation - Update
		 *
		 * @param in_entityId The id of the entity to update
		 * @param in_entityType The entity type as defined by the user
		 * @param in_jsonEntityData	The entity's data as a json string.
		 * @param in_jsonEntityAcl The entity's access control list as json. A null acl implies default
		 * permissions which make the entity readable/writeable by only the player.
         * @param in_version Current version of the entity. If the version of the
         * entity on the server does not match the version passed in, the
         * server operation will fail. Use -1 to skip version checking.
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 * @return The JSON returned in the callback is as follows:
		 * {
		 *   "status":200,
		 *   "data":null
		 * }
		 */
		void updateEntity( const char * in_entityId, const char * in_entityType, const std::string& in_jsonEntityData, const std::string& in_jsonEntityAcl, int64_t in_version, IServerCallback * in_callback = NULL);
		
		/**
		* Method updates a new singleton entity on the server. This operation results in the entity
		* data being completely replaced by the passed in JSON string. If the entity doesn't exists it is created
		*
		* Service Name - Entity
		* Service Operation - UpdateSingleton
		*
		* @param in_entityType The entity type as defined by the user
		* @param in_jsonEntityData	The entity's data as a json string.
		* permissions which make the entity readable/writeable by only the player.
        * @param in_jsonEntityAcl The entity's access control list as json. A null acl implies default
        * permissions which make the entity readable/writeable by only the player.
        * @param in_version Current version of the entity. If the version of the
        * entity on the server does not match the version passed in, the
        * server operation will fail. Use -1 to skip version checking.
		* @param in_callback The method to be invoked when the server response is received
		*
		* @return The JSON returned in the callback is as follows:
		* {
		*   "status":200,
		*    "data" :   {
		*         "entityId": "113db68a-48ad-4fc9-9f44-5fd36fc6445f",
		*         "entityType": "settings",
		*         "version": 1,
		*         "data": {
		*           "name": "john",
		*           "age": 30
		*         },
		*         "createdAt": 1395943044322,
		*         "updatedAt": 1395943044322
		*       },
		* }
		*/
		void updateSingleton(const char * in_entityType, const std::string& in_jsonEntityData, const std::string& in_jsonEntityAcl, int64_t in_version, IServerCallback * in_callback = NULL);

		/**
         * Update partially, an entity's data.  As opposed to the updateEntity
		 * method this method can only include data to be changed. The data is
		 * formatted similar to the way Mongo works for selective update.
		 * Fields can be added, modified or deleted.
		 * e.g. {"$set" : { "field1" : "value1", "field2" : "value2" },
		 *  "$unset" : { "field3" : "" } }
		 * This would add/update fields "field1" and "field2" and delete "field3".
		 * Fields that exist are updated and fields that don't exist are added by the "$set"
         *
         * @param entityType See ENTITY description at top of class
         * @param entityId Id of the target entity
         * @param partialData The new data to be sent to the server
         * @param callback The callback for success or failure of the update
         *                 (collisions are possible for child entities)
         */
		//Unavailable for now...
		//void updatePartialEntity( const char * entityType, int64_t entityId, const std::string& jsonUpdateOps, IServerCallback * callback );
		
		/**
		 * Method deletes the given entity on the server.
		 *
		 * Service Name - Entity
		 * Service Operation - Delete
		 *
		 * @param in_entityId The id of the entity to update
         * @param in_version Current version of the entity. If the version of the
         * entity on the server does not match the version passed in, the
         * server operation will fail. Use -1 to skip version checking.
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 * @return The JSON returned in the callback is as follows. Note that status 200 is returned
		 * whether or not the given entity was found on the server.
		 * {
		 *   "status":200,
		 *   "data":null
		 * }
		 */
		void deleteEntity(const char * in_entityId, int64_t in_version, IServerCallback * in_callback = NULL);
		
		/**
		* Method deletes the given singleton entity on the server.
		*
		* Service Name - Entity
		* Service Operation - DeleteSingleton
		*
		* @param in_entityType The type of the entity to delete
        * @param in_version Current version of the entity. If the version of the
        * entity on the server does not match the version passed in, the
        * server operation will fail. Use -1 to skip version checking.
		* @param in_callback The method to be invoked when the server response is received
		*
		* @return The JSON returned in the callback is as follows. Note that status 200 is returned
		* whether or not the given entity was found on the server.
		* {
		*   "status":200,
		*   "data":null
		* }
		*/
		void deleteSingleton(const char * in_entityType, int64_t in_version, IServerCallback * in_callback = NULL);

        /** Method to get a specific entity.
         *
         * Service Name - Entity
         * Service Operation - Read
         *
         * @param in_entityId The entity id
         * @param in_callback The method to be invoked when the server response is received
         */
        void getEntity(const char * in_entityId, IServerCallback * in_callback = NULL);
        
        /** Method retreives a singleton entity on the server. If the entity doesn't exist, null is returned.
         *
         * Service Name - Entity
         * Service Operation - ReadSingleton
         *
         * @param in_entityType The entity type as defined by the user
         * @param in_callback The method to be invoked when the server response is received
         */
        void getSingleton(const char * in_entityType, IServerCallback * in_callback = NULL);
        
		/** Method returns all player entities that match the given type.
		 * Service Name - Entity
		 * Service Operation - ReadByType
		 * 
		 * @param in_entityType The entity type to search for
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 * @return JSON including the entities matching the given type
		 * {
		 *   "status": 200,
		 *   "data": {
		 *     "entities": [
		 *       {
         *         "entityId": "113db68a-48ad-4fc9-9f44-5fd36fc6445f",
         *         "entityType": "person",
         *         "version": 1,
         *         "data": {
         *           "name": "john",
         *           "age": 30
         *         },
         *         "acl": {
         *           "other": 0
         *         },
         *         "createdAt": 1395943044322,
         *         "updatedAt": 1395943044322
         *       },
		 *       {
         *         "entityId": "255db68a-48ad-4fc9-9f44-5fd36fc6445f",
         *         "entityType": "person",
         *         "version": 1,
         *         "data": {
         *           "name": "mary",
         *           "age": 25
         *         },
         *         "acl": {
         *           "other": 0
         *         },
         *         "createdAt": 1395943044322,
         *         "updatedAt": 1395943044322
         *       }
		 *     ]
		 *   }
		 */
		void getEntitiesByType( const char * in_entityType, IServerCallback * in_callback = NULL );
		
		
		/**
         * Method returns all shared entities for the given player id.
         * An entity is shared if its ACL allows for the currently logged
         * in player to read the data.
         *             
		 * Service Name - Entity
	     * Service Operation - ReadShared
	   	 *
         * @param in_playerId The player id to retrieve shared entities for
         * @param in_callback The method to be invoked when the server response is received
         * 
         * @return JSON including the shared entities for the given player id
         * {
         *   "status": 200,
         *   "data": {
         *     "entities": [
         *       {
         *         "entityId": "544db68a-48ad-4fc9-9f44-5fd36fc6445f",
         *         "entityType": "publicInfo",
         *         "version": 1,
         *         "data": {
         *           "name": "john",
         *           "age": 30
         *         },
         *         "acl": {
         *           "other": 1
         *         },
         *         "createdAt": 1395943044322,
         *         "updatedAt": 1395943044322
         *       }
         *     ]
         *   }
         * }
         */
		void getSharedEntitiesForPlayerId( const char * in_playerId, IServerCallback * in_callback = NULL  );
		
		/**
		 * Method updates a shared entity owned by another player. This operation results in the entity
		 * data being completely replaced by the passed in JSON string.
		 *
		 * Service Name - Entity
		 * Service Operation - UpdateShared
		 *
		 * @param in_entityId The id of the entity to update
		 * @param in_targetPlayerId The id of the player who owns the shared entity
		 * @param in_entityType The entity type as defined by the user
		 * @param in_jsonEntityData	The entity's data as a json string.
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 * @return The JSON returned in the callback is as follows:
		 * {
		 *   "status":200,
		 *   "data":null
		 * }
		 */
		void updateSharedEntity(const char * in_entityId, const char * in_targetPlayerId, const char * in_entityType, const std::string& in_jsonEntityData, int64_t in_version, IServerCallback * in_callback = NULL);
        
        /**
         * Method uses a paging system to iterate through user entities
         * After retrieving a page of entities with this method,
         * use GetPageOffset() to retrieve previous or next pages.
         *
         * Service Name - Entity
         * Service Operation - GetPage
         *
         * @param in_context The json context for the page request.
         *                   See the portal appendix documentation for format.
         * @param in_callback The callback object
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {
         *         "results": {
         *             "moreBefore": false,
         *             "count": 200,
         *             "items": [
         *                 {
         *                     "entityId": "00edfd8e-5028-45d5-95d4-b1869cf2afaa",
         *                     "entityType": "testEntity",
         *                     "version": 1,
         *                     "data": {
         *                         "testName": "Test Name 01"
         *                     },
         *                     "acl": {
         *                         "other": 2
         *                     },
         *                     "createdAt": 1437505537168,
         *                     "updatedAt": 1437505537168
         *              }],
         *              "page": 1,
         *              "moreAfter": true
         *         },
         *         "context": "eyJzZWFyY2hDcml0ZXJpYSI6eyJlbnRpdHlUeXBlIjoiYnVpbGRpbmciLCJnYW
         *              1lSWQiOiIxMDI4NyIsIiRvciI6W3sib3duZXJJZCI6Ijk5MjM4ZmFiLTkxYTItNDdiYy1
         *              iMDExLWJjMThhN2IyOWY3NiJ9LHsiYWNsLm90aGVyIjp7IiRuZSI6MH19XX0sInNvcnRD
         *              cml0ZXJpYSI6eyJjcmVhdGVkQXQiOjEsInVwZGF0ZWRBdCI6LTF9LCJwYWdpbmF0aW9uI
         *              jp7InJvd3NQZXJQYWdlIjo1MCwicGFnZU51bWJlciI6NH0sIm9wdGlvbnMiOm51bGx9"
         *     }
         * }
         */
        void getPage(const char * in_context, IServerCallback * in_callback = NULL);
        
        /**
         * Method to retrieve previous or next pages after having called the GetPage method.
         *
         * Service Name - Entity
         * Service Operation - GetPageOffset
         *
         * @param in_context The context string returned from the server from a
         *      previous call to GetPage or GetPageOffset
         * @param in_pageOffset The positive or negative page offset to fetch. Uses the last page
         *      retrieved using the context string to determine a starting point.
         * @param in_callback The callback object
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {
         *         "results": {
         *             "moreBefore": true,
         *             "count": 200,
         *             "items": [
         *                 {
         *                     "entityId": "01234d8e-5028-45d5-95d4-b1869cf2afaa",
         *                     "entityType": "testEntity",
         *                     "version": 1,
         *                     "data": {
         *                         "testName": "Test Name 02"
         *                     },
         *                     "acl": {
         *                         "other": 1
         *                     },
         *                     "createdAt": 1437505537168,
         *                     "updatedAt": 1437505537168
         *              }],
         *              "page": 2,
         *              "moreAfter": true
         *         },
         *         "context": "eyJzZWFyY2hDcml0ZXJpYSI6eyJlbnRpdHlUeXBlIjoiYnVpbGRpbmciLCJnYW
         *              1lSWQiOiIxMDI4NyIsIiRvciI6W3sib3duZXJJZCI6Ijk5MjM4ZmFiLTkxYTItNDdiYy1
         *              iMDExLWJjMThhN2IyOWY3NiJ9LHsiYWNsLm90aGVyIjp7IiRuZSI6MH19XX0sInNvcnRD
         *              cml0ZXJpYSI6eyJjcmVhdGVkQXQiOjEsInVwZGF0ZWRBdCI6LTF9LCJwYWdpbmF0aW9uI
         *              jp7InJvd3NQZXJQYWdlIjo1MCwicGFnZU51bWJlciI6NH0sIm9wdGlvbnMiOm51bGx9"
         *     }
         * }
         */
        void getPageOffset(const char * in_context, int in_pageOffset, IServerCallback * in_callback = NULL);
		
	};
}