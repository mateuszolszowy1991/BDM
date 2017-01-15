#ifndef REMOTECONTOLLERDEVICEMANAGER_H
#define REMOTECONTOLLERDEVICEMANAGER_H
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include "IDBWrapper.hpp"
#include "Logger.hpp"

using namespace std;

class RemoteContollerDeviceManager
{
    public:
        RemoteContollerDeviceManager(IDBWrapper* wrapper);
        virtual ~RemoteContollerDeviceManager();
        void prepareRequestForBdm();
        string startSignalProceeding();
        bool waitForSignal();
        void createRCObject();
    protected:

    private:
        void initialize();
        string prepareRequestForDoors();
        void removeSignal();
        void changeProceduralStateBack();
        string signal_;
        vector<string> splitString(string strPtr, char sign);
        IDBWrapper* wrapper_;
        Mo remoteContrMo_;
        const int MIN_POWER_LEVEL = 80;
};

#endif // REMOTECONTOLLERDEVICEMANAGER_H
