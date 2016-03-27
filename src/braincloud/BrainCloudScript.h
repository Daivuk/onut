#pragma once

#include <string>

#include "IServerCallback.h"

namespace BrainCloud {
    
    class BrainCloudScript
    {
    public:
        /**
         * Executes a script on the server.
         *
         * Service Name - Script
         * Service Operation - Run
         *
         * @param in_scriptName The name of the script to be run
         * @param in_jsonScriptData Data to be sent to the script in json format
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *   "status":200,
         *   "data":null //// this value depends on what the script returns
         * }
         * @see The API documentation site for more details on cloud code
         */
        void runScript(const char * in_scriptName, const std::string & in_jsonScriptData, IServerCallback * in_callback = NULL);
        
        /**
         * Allows cloud script executions to be scheduled
         *
         * Service Name - Script
         * Service Operation - ScheduleCloudScript
         *
         * @param in_scriptName The name of the script to be run
         * @param in_jsonScriptData Data to be sent to the script in json format
         * @param in_startDateInUTC The start date as a time struct
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *  "status": 200,
         *  "data": {
         *      "result": {},
         *      "scriptName": "testScript",
         *      "jobId": "48266b95-d197-464d-bb6b-da70aa1e22a9",
         *      "runState": "Scheduled",
         *      "description": null,
         *      "gameId": "10170",
         *      "runEndTime": 0,
         *      "parameters": {
         *          "testParm1": 1
         *      },
         *      "runStartTime": 0,
         *      "scheduledStartTime": 1437576422378
         *      }
         * }
         * @see The API documentation site for more details on cloud code
         */
        void scheduleRunScriptUTC(const char * in_scriptName, const char * in_jsonScriptData, const struct tm* in_startDateInUTC, IServerCallback * in_callback = NULL);
        
        /**
         * Allows cloud script executions to be scheduled
         *
         * Service Name - Script
         * Service Operation - ScheduleCloudScript
         *
         * @param in_scriptName The name of the script to be run
         * @param in_jsonScriptData Data to be sent to the script in json format
         * @param in_minutesFromNow Number of minutes from now to run script
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *  "status": 200,
         *  "data": {
         *      "result": {},
         *      "scriptName": "testScript",
         *      "jobId": "48266b95-d197-464d-bb6b-da70aa1e22a9",
         *      "runState": "Scheduled",
         *      "description": null,
         *      "gameId": "10170",
         *      "runEndTime": 0,
         *      "parameters": {
         *          "testParm1": 1
         *      },
         *      "runStartTime": 0,
         *      "scheduledStartTime": 1437576422378
         *      }
         * }
         * @see The API documentation site for more details on cloud code
         */
        void scheduleRunScriptMinutes(const char * in_scriptName, const char * in_jsonScriptData, int in_minutesFromNow, IServerCallback * in_callback = NULL);
        
        /**
         * Run a cloud script in a parent app
         *
         * Service Name - Script
         * Service Operation - RUN_PARENT_SCRIPT
         *
         * @param in_scriptName The name of the script to be run
         * @param in_scriptData Data to be sent to the script in json format
         * @param in_parentLevel The level name of the parent to run the script from
         * @param in_callback The method to be invoked when the server response is received
         *
         * @return The JSON returned in the callback is as follows:
         * {
         *     "status": 200,
         *     "data": {
         *         "response": {
         *             "success": true
         *         },
         *         "success": true
         *     }
         * }
         * @see The API documentation site for more details on cloud code
         */    
        void runParentScript(const char * in_scriptName, const char * in_jsonScriptData, const char * in_parentLevel, IServerCallback * in_callback = NULL);
        
    };
}