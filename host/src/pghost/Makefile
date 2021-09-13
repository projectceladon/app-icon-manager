CC = g++ -fpermissive -I/usr/include/jsoncpp -DLG_SINGLE_MODE
#CC_FLAGS = -g -Wall -lpthread -lsqlite3
CC_FLAGS = -g -Wall -lpthread -fPIC  -ljsoncpp -I/usr/include/jsoncpp
#CC_FLAGS_CD = -g -Wall -lpthread -lX11 -ljpeg -I. -I/usr/include/libpng12 -ljpeg -lpng12
CC_FLAGS_CD = -g -Wall -lpthread -lX11 -ljpeg -I. -I/usr/include/libpng12 -ljpeg -lpng16
#CC_FLAGS_CD = -g -Wall -lpthread -lX11 -ljpeg -I. -I/usr/include/libpng12 -ljpeg

CC_FLAGS_MWC = -fPIC -fno-exceptions -Wno-deprecated  -DOS_UBUNTU=1 -ljsoncpp -I/usr/include/jsoncpp

CC_FLAGS_KYDROIDAPI = -fPIC -shared -std=c++14 -fpermissive -g -ldl -lstdc++ -ljsoncpp -I/usr/include/jsoncpp

CC_FLAGS_KYDROIDTEST = -L. -I./include -std=c++14 -fpermissive -g -lstdc++ -lkydroid -ljsoncpp -I/usr/include/jsoncpp

includes = $(wildcard *.h)

#all: vatsev vatlauncher vatclidaemon
all: mwc_launcher mwc_hostdaemon mwc libkydroid.so api_test msg_agent
#all: mwc
# ls *.cpp | grep -v "test_client.cpp" | sed '{ :ack; N; $! b ack; s/\n/ /g;}'


MWCHOSTDAEMONSRCS = vatclidaemon.cpp connsock.cpp connfactory.cpp connmgr.cpp \
               eventqueue.cpp utils.cpp vatclient.cpp

MWCLAUNCHERSRCS = vatlauncher.cpp lgclient.cpp connfactory.cpp connmgr.cpp eventqueue.cpp \
          connsock.cpp utils.cpp ShortcutMgrLg.cpp Log.cpp AdbProxy.cpp

MSG_AGENTSRCS = msg_agent.cpp lgclient.cpp connfactory.cpp connmgr.cpp eventqueue.cpp \
          connsock.cpp utils.cpp AdbProxy.cpp Log.cpp

MWCSRC = mwc_main.cpp ShortcutMgrLg.cpp Log.cpp AdbProxy.cpp

KYDROIDAPISRC = lib/kydroidapi.cpp ShortcutMgrLg.cpp Log.cpp AdbProxy.cpp

API_TESTSRC = api_test.cpp

MWCLAUNCHEROBJS = $(MWCLAUNCHERSRCS:.cpp=.o)
MWCHOSTDAEMONOBJS = $(MWCHOSTDAEMONSRCS:.cpp=.o)
MWCOBJS = $(MWCSRC:.cpp=.o)
KYDROIDAPIOBJS = $(KYDROIDAPISRC:.cpp=.o)
API_TESTOBJS = $(API_TESTSRC:.cpp=.o)
MSG_AGENTOBJS = $(MSG_AGENTSRCS:.cpp=.o)

mwc_launcher: $(MWCLAUNCHEROBJS)
	$(CC) $(MWCLAUNCHEROBJS) -o $@ $(CC_FLAGS)

mwc_hostdaemon: $(MWCHOSTDAEMONOBJS)
	$(CC) $(MWCHOSTDAEMONOBJS) -o $@ $(CC_FLAGS_CD)

mwc: $(MWCOBJS)
	$(CC) $(MWCOBJS) -o $@ $(CC_FLAGS_MWC)

libkydroid.so: $(KYDROIDAPIOBJS)
	$(CC) $(KYDROIDAPIOBJS) -o $@ $(CC_FLAGS_KYDROIDAPI)

api_test: $(API_TESTOBJS)
	$(CC) $(API_TESTOBJS) -o $@ $(CC_FLAGS_KYDROIDTEST)

msg_agent: $(MSG_AGENTOBJS)
	$(CC) $(MSG_AGENTOBJS) -o $@ $(CC_FLAGS)

%.o: %.cpp ${includes}
	$(CC) -c $(CC_FLAGS) $< -o $@

clean:
	-rm -f mwc_launcher mwc_hostdaemon mwc libkydroid.so api_test msg_agent $(MWCLAUNCHEROBJS) $(MWCHOSTDAEMONOBJS) $(MWCOBJS) $(KYDROIDAPIOBJS) $(API_TESTOBJS) $(MSG_AGENTOBJS)
