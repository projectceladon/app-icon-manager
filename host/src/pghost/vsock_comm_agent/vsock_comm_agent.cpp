#include "vsock_msg_dispatcher.h"

using namespace vsock;
class AppstatusComponent : public Component {
    public:
    virtual ~AppstatusComponent() {}
    virtual void ProcessMsg(Message& msg, uint64_t hndl) {
        LOGIT("Process msg - %s\n", msg.payload);
	char cmd[512];
	snprintf(cmd, sizeof(cmd), "/opt/cfc/mwc/bin/msg_agent localhost 3000 CRASHAPP %s", msg.payload);
	system(cmd);
    }

    virtual void init() {
        LOGIT("Init");
    }

};

int main() {
    MsgDispatcher msg_dispatcher;
    std::vector<MSG_TYPE> msg_list_clipboard, msg_list_appstatus ;
    msg_list_clipboard.emplace_back(MSG_TYPE_CLIPBOARD);
    msg_list_appstatus.emplace_back(MSG_TYPE_APPSTATUS);
    LOGIT("CREATED\n");
    //msg_dispatcher.RegisterComponent(msg_list_clipboard, new ClipboardComponent());
    msg_dispatcher.RegisterComponent(msg_list_appstatus, new AppstatusComponent());
    msg_dispatcher.Start();
    LOGIT("STARTED\n");
    while (1) {
        sleep(2000);
    }
}
