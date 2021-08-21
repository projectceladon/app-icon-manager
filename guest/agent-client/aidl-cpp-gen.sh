set -x
mkdir -p include
aidl-cpp -I. com/intel/serviceagent/IServiceAgent.aidl include/ IServiceAgent.cpp
aidl-cpp -I. com/intel/serviceagent/IPackageManagerAgent.aidl include/ IPackageManagerAgent.cpp
aidl-cpp -I. com/intel/serviceagent/IPackageInstallerListener.aidl include/ IPackageInstallerListener.cpp
aidl-cpp -I. com/intel/serviceagent/IClipboardAgent.aidl include/ IClipboardAgent.cpp
