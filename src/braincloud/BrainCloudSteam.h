#pragma once

#include <string>

#include "IServerCallback.h"
#include "ServiceName.h"
#include "ServiceOperation.h"
#include "OperationParam.h"
#include "BrainCloudTypes.h"

namespace BrainCloud {
	
	class BrainCloudSteam
	{
		public:
		void getUserInfo(IServerCallback * in_callback = NULL);
	};
}