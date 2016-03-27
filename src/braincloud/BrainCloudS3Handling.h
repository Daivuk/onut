#pragma once

#include <string>

#include "IServerCallback.h"
#include "ServiceName.h"
#include "ServiceOperation.h"
#include "OperationParam.h"

namespace BrainCloud
{
    class BrainCloudS3Handling
    {
    public:
        /*
         * Sends an array of file details and returns
         * the details of any of those files that have changed
         *
         * Service Name - S3Handling
         * Service Operation - GetUpdatedFiles
         *
         * @param in_category  Category of files on server to compare against
         * @param in_fileDetailsJson  An array of file details
         * @param in_callback  Instance of IServerCallback to call when the server response is received
         *
         * @returns  The JSON returned in the callback is as follows.
         * {
         *     "status": 200,
         *     "data": {
         *         "fileDetails": [{
         *             "gameId": "12311331",
         *             "fileId": "3780516b-14f8-4055-8899-8eaab6ac7e82",
         *             "shortName": "Test Doc",
         *             "fileName": "testDoc.txt",
         *             "type": "g",
         *             "subType": "cust",
         *             "category": null,
         *             "fileSize": 4,
         *             "dateUploaded": 1437154770000,
         *             "relativeUrl": "/cust/testDoc.txt",
         *             "absoluteUrl": "http://internal.braincloudservers.com/s3/portal/g/12311331/cust/testDoc.txt",
         *             "md5Hash": "d41d8cd98f00b204e9800998ecf8427e"
         *         }]
         *     }
         * }
         */
        void getUpdatedFiles(const char * in_category, const char * in_fileDetails, IServerCallback * in_callback = NULL);
        
        /*
         * Retrieves the details of custom files stored on the server
         *
         * Service Name - S3Handling
         * Service Operation - GetFileList
         *
         * @param in_category  Category of files to retrieve
         * @param in_callback  Instance of IServerCallback to call when the server response is receieved
         *
         * @returns  The JSON returned in the callback is as follows.
         * {
         *     "status": 200,
         *     "data": {
         *         "fileDetails": [{
         *             "gameId": "12311331",
         *             "fileId": "3780516b-14f8-4055-8899-8eaab6ac7e82",
         *             "shortName": "Test Doc",
         *             "fileName": "testDoc.txt",
         *             "type": "g",
         *             "subType": "cust",
         *             "category": null,
         *             "fileSize": 4,
         *             "dateUploaded": 1437154770000,
         *             "relativeUrl": "/cust/testDoc.txt",
         *             "absoluteUrl": "http://internal.braincloudservers.com/s3/portal/g/12311331/cust/testDoc.txt",
         *             "md5Hash": "d41d8cd98f00b204e9800998ecf8427e"
         *         }]
         *     }
         * }
         */
        void getFileList(const char * in_category, IServerCallback * in_callback = NULL);
    };
}