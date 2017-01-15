#include "RemoteContollerDeviceManager.hpp"

RemoteContollerDeviceManager::RemoteContollerDeviceManager(IDBWrapper* wrapper) : wrapper_(wrapper)
{
    Logger::saveToFile("BDM/INF: RemoteControllerDeviceManager ctor");

}

RemoteContollerDeviceManager::~RemoteContollerDeviceManager()
{
    //dtor
}

void RemoteContollerDeviceManager::createRCObject()
{
    remoteContrMo_ = wrapper_->createObject("CAR/BDM/REMOTE_CONTROLLER", "Offline", "Enabled");
}

bool RemoteContollerDeviceManager::waitForSignal()
{
    ifstream infile("/home/mato3/OSCAR/SYS/sigs/RC_SIG.dat");
    if(infile.good())
    {
        Logger::saveToFile("BDM/INF: RCDM: RC_SIG found" );
        fstream sig( "/home/mato3/OSCAR/SYS/sigs/RC_SIG.dat", std::ios::in );
        getline(sig, signal_);
        sig.close();
        Logger::saveToFile("BDM/INF: RemoteControllerDeviceManager: waitForSignal: received signal: " + signal_ );
        return infile.good();
    }
    //cout << "RC_SIG not found" <<endl;
    return false;
}

string RemoteContollerDeviceManager::startSignalProceeding()
{
   initialize();
   string request = prepareRequestForDoors();
   removeSignal();
   changeProceduralStateBack();
   return request;
}

string RemoteContollerDeviceManager::prepareRequestForDoors()
{
    return signal_;
}


void RemoteContollerDeviceManager::removeSignal()
{
    remove("/home/mato3/OSCAR/SYS/sigs/RC_SIG.dat");
}

void RemoteContollerDeviceManager::initialize()
{
    string power = splitString(signal_, ';')[1];
    Logger::saveToFile("BDM/DBG: RemoteControllerDeviceManager: signal power: " + power);
    if(atoi(power.c_str()) >= MIN_POWER_LEVEL)
    {
        Logger::saveToFile("BDM/DBG: RemoteControllerDeviceManager: initialize: power is more than MIN_POWER_LEVEL" );
        wrapper_->setOperationalState(&remoteContrMo_, "Online");
        wrapper_->setProceduralState(&remoteContrMo_, "Signalling");
    }
    else
    {
        Logger::saveToFile("BDM/DBG: RemoteControllerDeviceManager: initialize: power is less than MIN_POWER_LEVEL:" + power );
        wrapper_->setOperationalState(&remoteContrMo_, "Low_power");
        wrapper_->setProceduralState(&remoteContrMo_, "Signalling");
    }
}

void RemoteContollerDeviceManager::changeProceduralStateBack()
{
   wrapper_->setProceduralState(&remoteContrMo_, "Enabled");
}

vector<string> RemoteContollerDeviceManager::splitString(string strPtr, char sign)
{
    for (int i=0; i < strPtr.length(); i++)
    {
        if (strPtr[i] == sign)
            strPtr[i] = ' ';
    }
    vector<string> splitted;
    stringstream ss(strPtr);
    string temp;
    while (ss >> temp)
    {
        splitted.push_back(temp);
    }
    return splitted;
}
