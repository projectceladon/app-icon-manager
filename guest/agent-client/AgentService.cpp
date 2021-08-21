/*
Copyright (C) <2021> Intel Corporation
 
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
 
http://www.apache.org/licenses/LICENSE-2.0
 
Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions
and limitations under the License.
 

SPDX-License-Identifier: Apache-2.0

Author: Mao Marc (marc.mao@intel.com)
Date: 2021.04.27

*/

#include <binder/IServiceManager.h>

#include "AgentService.h"

namespace android {

ANDROID_SINGLETON_STATIC_INSTANCE(AgentService);

AgentService::AgentService() : Singleton<AgentService>() {
  Mutex::Autolock _l(mLock);
  connectLocked();
}

void AgentService::connectLocked() {
  const String16 name("ServiceAgent");
  while (getService(name, &mAgentService) != NO_ERROR) {
    usleep(250000);
  }
  assert(mAgentService != NULL);

  // Create the death listener.
  class DeathObserver : public IBinder::DeathRecipient {
    AgentService& mAgentService;
    virtual void binderDied(const wp<IBinder>& who) {
      ALOGW("AgentService remote (ServiceAgent) died [%p]",
            who.unsafe_get());
      mAgentService.agentServiceDied();
    }

   public:
    explicit DeathObserver(AgentService& mgr) : mAgentService(mgr) {}
  };

  mDeathObserver = new DeathObserver(*const_cast<AgentService*>(this));
  IInterface::asBinder(mAgentService)->linkToDeath(mDeathObserver);
}

/*static*/ sp<IServiceAgent> AgentService::getAgentService() {
  AgentService& instance = AgentService::getInstance();
  Mutex::Autolock _l(instance.mLock);
  if (instance.mAgentService == NULL) {
    AgentService::getInstance().connectLocked();
    assert(instance.mAgentService != NULL);
    ALOGD("AgentService reconnected");
  }
  return instance.mAgentService;
}

void AgentService::agentServiceDied() {
  Mutex::Autolock _l(mLock);
  mAgentService = NULL;
  mDeathObserver = NULL;
}
}  // namespace android
