#pragma once

#include <string>

#include "IServerCallback.h"
#include "ServiceName.h"
#include "ServiceOperation.h"
#include "OperationParam.h"
#include "BrainCloudTypes.h"

namespace BrainCloud {

    class BrainCloudGlobalEntity
    {
    public:
        BrainCloudGlobalEntity();
        ~BrainCloudGlobalEntity();

        /**
        * Method creates a new entity on the server.
        *
        * Service Name - GlobalEntity
        * Service Operation - Create
        *
        * @param in_entityType The entity type as defined by the user
        * @param in_timeToLive Sets expiry time for entity in milliseconds if > 0
        * @param in_jsonEntityAcl The entity's access control list as json. A null acl implies default
        * @param in_jsonEntityData  The entity's data as a json string
        * @param in_callback The callback object
        *
        * @return The JSON returned in the callback is as follows:
        * {
        *   "status":200,
        *   "data":
        *   {
        *      gameId : "game",
        *      entityId : "hucfshugvgvfhug",
        *      ownerId : "ubfcbvfbsdvbb",
        *      entityType : "",
        *      entityIndexedId : "",
        *      version : 1,
        *      timeToLive: 0,
        *      expiresAt : 9947395735758975,
        *      data :
        *      {
        *          field : "value"
        *      },
        *      acl :
        *      {
        *          other, 1
        *      },
        *      createdAt : 74889479874,
        *      updatedAt : 73847474897487
        *
        *   }
        * }
        */
        void createEntity(std::string& in_entityType, int64_t in_timeToLive, std::string& in_jsonEntityAcl,
                        std::string& in_jsonEntityData, IServerCallback * in_callback = NULL);

        /**
        * Method creates a new entity on the server with an indexed id.
        *
        * Service Name - GlobalEntity
        * Service Operation - CreateWithIndexedId
        *
        * @param in_entityType The entity type as defined by the user
        * @param in_indexedId A secondary ID that will be indexed
        * @param in_timeToLive Sets expiry time for entity in milliseconds if > 0
        * @param in_jsonEntityAcl The entity's access control list as json. A null acl implies default
        * @param in_jsonEntityData  The entity's data as a json string
        * @param in_callback The callback object
        *
        * @return The JSON returned in the callback is as follows:
        * {
        *   "status":200,
        *   "data":
        *   {
        *      gameId : "game",
        *      entityId : "hucfshugvgvfhug",
        *      ownerId : "ubfcbvfbsdvbb",
        *      entityType : "",
        *      entityIndexedId : "hhjdyjghjd",
        *      version : 1,
        *      timeToLive: 0,
        *      expiresAt : 9947395735758975,
        *      data :
        *      {
        *          field : "value"
        *      },
        *      acl :
        *      {
        *          other, 1
        *      },
        *      createdAt : 74889479874,
        *      updatedAt : 73847474897487
        *
        *   }
        * }
        */
        void createEntityWithIndexedId(std::string& in_entityType, std::string& in_indexedId, int64_t in_timeToLive,
            std::string& in_jsonEntityAcl, std::string& in_jsonEntityData, IServerCallback * in_callback = NULL);

        /**
        * Method updates an existing entity on the server.
        *
        * Service Name - GlobalEntity
        * Service Operation - Update
        *
        * @param in_entityId The entity ID
        * @param in_version The version of the entity to update
        * @param in_jsonEntityData  The entity's data as a json string
        * @param in_callback The callback object
        *
        * @return The JSON returned in the callback is as follows:
        * {
        *   "status":200,
        *   "data":
        *   {
        *      gameId : "game",
        *      entityId : "hucfshugvgvfhug",
        *      ownerId : "ubfcbvfbsdvbb",
        *      entityType : "",
        *      entityIndexedId : "",
        *      version : 1,
        *      timeToLive: 0,
        *      expiresAt : 9947395735758975,
        *      data :
        *      {
        *          field : "value"
        *      },
        *      acl :
        *      {
        *          other, 1
        *      },
        *      createdAt : 74889479874,
        *      updatedAt : 73847474897487
        *
        *   }
        * }
        */
        void updateEntity(std::string& in_entityId, int64_t in_version, std::string& in_jsonEntityData, IServerCallback * in_callback = NULL);

        /**
        * Method updates an existing entity's Acl on the server.
        *
        * Service Name - GlobalEntity
        * Service Operation - UpdateAcl
        *
        * @param in_entityId The entity ID
        * @param in_version The version of the entity to update
        * @param in_jsonEntityAcl The entity's access control list as json.
        * @param in_callback The callback object
        *
        * @return The JSON returned in the callback is as follows:
        * {
        *   "status":200,
        *   "data":
        *   {
        *      gameId : "game",
        *      entityId : "hucfshugvgvfhug",
        *      ownerId : "ubfcbvfbsdvbb",
        *      entityType : "",
        *      entityIndexedId : "",
        *      version : 1,
        *      timeToLive: 0,
        *      expiresAt : 9947395735758975,
        *      data :
        *      {
        *          field : "value"
        *      },
        *      acl :
        *      {
        *          other, 1
        *      },
        *      createdAt : 74889479874,
        *      updatedAt : 73847474897487
        *
        *   }
        * }
        */
        void updateEntityAcl(std::string& in_entityId, int64_t in_version, std::string& in_jsonEntityAcl, IServerCallback * in_callback = NULL);

        /**
        * Method updates an existing entity's time to live on the server.
        *
        * Service Name - GlobalEntity
        * Service Operation - UpdateTimeToLive
        *
        * @param in_entityId The entity ID
        * @param in_version The version of the entity to update
        * @param in_timeToLive Sets expiry time for entity in milliseconds if > 0
        * @param in_callback The callback object
        *
        * @return The JSON returned in the callback is as follows:
        * {
        *   "status":200,
        *   "data":
        *   {
        *      gameId : "game",
        *      entityId : "hucfshugvgvfhug",
        *      ownerId : "ubfcbvfbsdvbb",
        *      entityType : "",
        *      entityIndexedId : "",
        *      version : 1,
        *      data :
        *      timeToLive: 0,
        *      expiresAt : 9947395735758975,
        *      {
        *          field : "value"
        *      },
        *      acl :
        *      {
        *          other, 1
        *      },
        *      createdAt : 74889479874,
        *      updatedAt : 73847474897487
        *
        *   }
        * }
        */
        void updateEntityTimeToLive(std::string& in_entityId, int64_t in_version, int64_t in_timeToLive, IServerCallback * in_callback = NULL);

        /**
        * Method deletes an existing entity on the server.
        *
        * Service Name - GlobalEntity
        * Service Operation - Delete
        *
        * @param in_entityId The entity ID
        * @param in_version The version of the entity to delete
        * @param in_callback The callback object
        *
        * @return The JSON returned in the callback is as follows:
        * {
        *   "status":200,
        *   "data":
        *   {
        *   }
        * }
        */
        void deleteEntity(std::string& in_entityId, int64_t in_version, IServerCallback * in_callback = NULL);

        /**
        * Method reads an existing entity from the server.
        *
        * Service Name - GlobalEntity
        * Service Operation - Read
        *
        * @param in_entityId The entity ID
        * @param in_callback The callback object
        *
        * @return The JSON returned in the callback is as follows:
        * {
        *   "status":200,
        *   "data":
        *   {
        *      gameId : "game",
        *      entityId : "hucfshugvgvfhug",
        *      ownerId : "ubfcbvfbsdvbb",
        *      entityType : "",
        *      entityIndexedId : "",
        *      version : 1,
        *      timeToLive: 0,
        *      expiresAt : 9947395735758975,
        *      data :
        *      {
        *          field : "value"
        *      },
        *      acl :
        *      {
        *          other, 1
        *      },
        *      createdAt : 74889479874,
        *      updatedAt : 73847474897487
        *
        *   }
        * }
        */
        void readEntity(std::string& in_entityId, IServerCallback * in_callback = NULL);

        /**
        * Method gets list of entities from the server base on type and/or where clause
        *
        * Service Name - GlobalEntity
        * Service Operation - GetList
        *
        * @param in_where Mongo style query string
        * @param in_orderBy Sort order
        * @param in_maxReturn The maximum number of entities to return
        * @param in_callback The callback object
        *
        * @return The JSON returned in the callback is as follows:
        * {
        *   "status":200,
        *   "data":
        *   {
        *      entities : [
        *          {
        *              gameId : "game",
        *              entityId : "hucfshugvgvfhug",
        *              ownerId : "ubfcbvfbsdvbb",
        *              entityType : "",
        *              entityIndexedId : "",
        *              version : 1,
        *              timeToLive: 0,
        *              expiresAt : 9947395735758975,
        *              data :
        *              {
        *                  field : "value"
        *              },
        *              acl :
        *              {
        *                  other, 1
        *              },
        *              createdAt : 74889479874,
        *              updatedAt : 73847474897487
        *          },
        *          {
        *              gameId : "game",
        *              entityId : "dgdgg",
        *              ownerId : "ubfcbvfbsdvbb",
        *              entityType : "",
        *              entityIndexedId : "",
        *              version : 1,
        *              timeToLive: 0,
        *              expiresAt : 9947395735758975,
        *              data :
        *              {
        *                  field : "value2"
        *              },
        *              acl :
        *              {
        *                  other, 1
        *              },
        *              createdAt : 74889479874,
        *              updatedAt : 73847474897487
        *          }
        *
        *   }
        * }
        */
        void getList(std::string& in_where, std::string& in_orderBy, int64_t in_maxReturn, IServerCallback * in_callback = NULL);

        /**
        * Method gets list of entities from the server base on indexed id
        *
        * Service Name - GlobalEntity
        * Service Operation - GetListByIndexedId
        *
        * @param in_entityIndexedId The entity indexed Id
        * @param in_maxReturn The maximum number of entities to return
        * @param in_callback The callback object
        *
        * @return The JSON returned in the callback is as follows:
        * {
        *   "status":200,
        *   "data":
        *   {
        *      entities : [
        *          {
        *              gameId : "game",
        *              entityId : "hucfshugvgvfhug",
        *              ownerId : "ubfcbvfbsdvbb",
        *              entityType : "",
        *              entityIndexedId : "",
        *              version : 1,
        *              timeToLive: 0,
        *              expiresAt : 9947395735758975,
        *              data :
        *              {
        *                  field : "value"
        *              },
        *              acl :
        *              {
        *                  other, 1
        *              },
        *              createdAt : 74889479874,
        *              updatedAt : 73847474897487
        *          },
        *          {
        *              gameId : "game",
        *              entityId : "dgdgg",
        *              ownerId : "ubfcbvfbsdvbb",
        *              entityType : "",
        *              entityIndexedId : "",
        *              version : 1,
        *              timeToLive: 0,
        *              expiresAt : 9947395735758975,
        *              data :
        *              {
        *                  field : "value2"
        *              },
        *              acl :
        *              {
        *                  other, 1
        *              },
        *              createdAt : 74889479874,
        *              updatedAt : 73847474897487
        *          }
        *
        *   }
        * }
        */
        void getListByIndexedId(std::string& in_entityIndexedId, int64_t in_maxReturn, IServerCallback * in_callback = NULL);

        /**
        * Method gets a count of entities based on the where clause
        *
        * Service Name - GlobalEntity
        * Service Operation - GetListCount
        *
        * @param in_where Mongo style query string
        * @param in_callback The callback object
        *
        * @return The JSON returned in the callback is as follows:
        * {
        *   "status":200,
        *   "data":
        *   {
        *      "entityListCount" : 5
        *   }
        * }
        */
        void getListCount(std::string& in_where, IServerCallback * in_callback = NULL);

        /**
         * Method uses a paging system to iterate through Global Entities
         * After retrieving a page of Global Entities with this method,
         * use GetPageOffset() to retrieve previous or next pages.
         *
         * Service Name - GlobalEntity
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
         *                     "ownerId": "153a5ac3-582e-42d7-9e7d-aa80ca7e6689",
         *                     "entityType": "testGlobalEntity",
         *                     "entityIndexedId": null,
         *                     "version": 1,
         *                     "data": {
         *                         "globalTestName": "Test Name 01"
         *                     },
         *                     "acl": {
         *                         "other": 2
         *                     },
         *                     "expiresAt": 9223372036854776000,
         *                     "timeToLive": 0,
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
         * Service Name - GlobalEntity
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
         *                     "entityId": "00edfd8e-5028-45d5-95d4-b1869cf2afaa",
         *                     "ownerId": "153a5ac3-582e-42d7-9e7d-aa80ca7e6689",
         *                     "entityType": "testGlobalEntity",
         *                     "entityIndexedId": null,
         *                     "version": 1,
         *                     "data": {
         *                         "globalTestName": "Test Name 01"
         *                     },
         *                     "acl": {
         *                         "other": 2
         *                     },
         *                     "expiresAt": 9223372036854776000,
         *                     "timeToLive": 0,
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

