#ifndef __AGENT_SERVICE_H__
#define __AGENT_SERVICE_H__

#include <stdint.h>
#include <sys/types.h>
#include <binder/IBinder.h>

#include <utils/Singleton.h>
#include <utils/StrongPointer.h>

#include <com/intel/serviceagent/IServiceAgent.h>
#include <com/intel/serviceagent/BnPackageInstallerListener.h>
#include <com/intel/serviceagent/BnClipboardListener.h>
#include <com/intel/serviceagent/BnHostSettingsListener.h>
#include <com/intel/serviceagent/BnInputMethodManagerAgent.h>

using IServiceAgent = com::intel::serviceagent::IServiceAgent;
using IPackageManagerAgent = com::intel::serviceagent::IPackageManagerAgent;
using IPackageInstallerListener = com::intel::serviceagent::IPackageInstallerListener;
using BnPackageInstallerListener = com::intel::serviceagent::BnPackageInstallerListener;
using IClipboardAgent = com::intel::serviceagent::IClipboardAgent;
using BnClipboardListener = com::intel::serviceagent::BnClipboardListener;
using BnHostSettingsListener = com::intel::serviceagent::BnHostSettingsListener;
using IInputMethodManagerAgent = com::intel::serviceagent::IInputMethodManagerAgent;

namespace android {

class AgentService : public Singleton<AgentService> {
  sp<IServiceAgent> mAgentService;
  sp<IBinder::DeathRecipient> mDeathObserver;
  Mutex mLock;

  AgentService();
  void connectLocked();
  void agentServiceDied();
  friend class Singleton<AgentService>;

 public:
  // Get a connection to the Agent Service.  This will block until
  // a connection is established.
  static sp<IServiceAgent> getAgentService();
};
}  // namespace android

#endif //__AGENT_SERVICE_H__
