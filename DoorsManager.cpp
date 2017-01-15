#include "DoorsManager.hpp"

DoorsManager::DoorsManager(IDBWrapper* wrapper) : wrapper_(wrapper)
{
    Logger::saveToFile("BDM/INF: DoorsManager: Ctor");
}

DoorsManager::~DoorsManager()
{
    //dtor
}

int DoorsManager::waitTillCARWillBeInInitializingState()
{
    Logger::saveToFile("BDM/INF: DoorsManager: waitTillCARWillBeInInitializingState");
    return 1;
}

void DoorsManager::createBDMObject()
{
    Logger::saveToFile("BDM/INF: DoorsManager: createBDMObject ");
    bdmMo_ = wrapper_->createObject("CAR/BDM", "Online", "Configuring");
}

void DoorsManager::createDOORSContainer()
{
    Logger::saveToFile("BDM/INF: DoorsManager: createDOORSContainer ");
    doorsMo_ = wrapper_->createObject("CAR/BDM/DOORS", "Online", "Configuring");
}

void DoorsManager::createAndSetDOORObjects()
{
    Logger::saveToFile("BDM/INF: DoorsManager: createDOORObjects ");
    for(int i = 1; i <=6; i++)
    {
        Mo doorMo = wrapper_->createObject("CAR/BDM/DOORS/DOOR_" + to_string(i), "Online", "Locked");
        wrapper_->expandObject(&doorMo, {"Label"}, {doorLabels_[i - 1]});
        createWindowObjects(doorMo.distname);
        if(doorLabels_[i - 1] == "MASK")
        {
            createWiperObjects(doorMo.distname + "/WINDOW/WIPER_1");
            createWiperObjects(doorMo.distname + "/WINDOW/WIPER_2");
        }
        else if(doorLabels_[i - 1] == "TRUNK")
        {
            createWiperObjects(doorMo.distname + "WINDOW/WIPER_1");
        }
        doorMos_.push_back(doorMo);

    }
}

void DoorsManager::createWindowObjects(string parent)
{
    Mo windowMo = wrapper_->createObject(parent + "/WINDOW", "Online", "Closed");
    windowMos_.push_back(windowMo);
}

void DoorsManager::createWiperObjects(string distname)
{
    Mo wiperMo = wrapper_->createObject(distname, "Online", "Off");
    wiperMos_.push_back(wiperMo);
}

void DoorsManager::completeConfiguring()
{
    wrapper_->setProceduralState(&doorsMo_, "Configured");
    Logger::saveToFile("BDM/INF: DoorsManager: Confiuration finished succesfully");
}

void DoorsManager::getOperationCodeFromRCDM(string signal)
{
    vector<string> splittedSignal = splitString(signal, ';');
    //cout << "BATTERY LEVEL: " << atoi((splittedSignal[1]).c_str()) << endl;
    if(atoi((splittedSignal[1]).c_str()) >= MIN_POWER_LEVEL)
    {
        if(splittedSignal[2] == "0x1")
        {
            Logger::saveToFile("UNLOCKING");
            unlockAllDoorsOperation();
        }
        else if(splittedSignal[2] == "0x2")
        {
            lockAllDoorsOperation();
        }
        else if(splittedSignal[2] == "0x3")
        {
            switchTrunkOperation();
        }
    }
    else
    {
        if(doorsMo_.proceduralState != doorProceduralState[2])
        {
            unlockAllDoorsOperation();
        }

    }
}

bool DoorsManager::waitForUserOpenDoorSignal()
{
    ifstream infile("D:/private/IDB/SYS/sigs/USER_DOORS_SIG.dat");
    if(infile.good())
    {
        return true;
    }
    return false;
}

void DoorsManager::someDoorsHaveBeenOpened()
{
    fstream sig( "D:/private/IDB/SYS/sigs/USER_DOORS_SIG.dat", std::ios::in );
    getline(sig, userDoorsSignal_);
    sig.close();
    Logger::saveToFile("BDM/INF: DoorsManager: someDoorsHaveBeenOpened: received signal: " + userDoorsSignal_ );
    remove("D:/private/IDB/SYS/sigs/USER_DOORS_SIG.dat");
    vector<string> splittedSignal = splitString(userDoorsSignal_, ';');
    int i = 0;
    for(const auto &label : doorLabels_)
    {
        if(label == splittedSignal[0])
        {
            Logger::saveToFile("BDM/INF: DoorsManager: someDoorsHaveBennOpened: doors: " + label + " has been " + splittedSignal[1]);
            wrapper_->setProceduralState(&doorMos_[i], splittedSignal[1]);
        }
        i++;
    }
    wrapper_->setProceduralState(&doorsMo_, splittedSignal[1]);
}

void DoorsManager::lockAllDoorsOperation()
{
    string response;
    for(int i = 1; i <=6; i++)
    {
        wrapper_->setProceduralState(&doorMos_[i - 1],  doorProceduralState[1]);
    }
    wrapper_->setProceduralState(&doorsMo_,  doorProceduralState[1]);
}

void DoorsManager::unlockAllDoorsOperation()
{
    string response;
    for(int i = 1; i <=6; i++)
    {
        wrapper_->setProceduralState(&doorMos_[i - 1],  doorProceduralState[2]);
    }
    wrapper_->setProceduralState(&doorsMo_,  doorProceduralState[2]);
}

void DoorsManager::switchTrunkOperation()
{
    string response;
    bool doorsOpened;
    if(doorMos_[5].proceduralState == doorProceduralState[1])
    {
        wrapper_->setProceduralState(&doorMos_[5],  doorProceduralState[2]);
        for(const auto &door : doorMos_)
        {
            if(door.proceduralState != doorProceduralState[1])
            {
                doorsOpened = true;
                Logger::saveToFile("Some doors are opened and we cannot change doors object status");
                break;
            }
            else
            {
               doorsOpened = false;
            }
        }
        if(!doorsOpened)
        {
            wrapper_->setProceduralState(&doorsMo_,  doorProceduralState[2]);
        }

    }
    else
    {
        wrapper_->setProceduralState(&doorMos_[5],  doorProceduralState[1]);
        for(const auto &door : doorMos_)
        {
            if(door.proceduralState != doorProceduralState[2])
            {
                doorsOpened = true;
                Logger::saveToFile("Some doors are opened and we cannot change doors object status");
                break;
            }
            else
            {
               doorsOpened = false;
            }
        }
        if(!doorsOpened)
        {
            wrapper_->setProceduralState(&doorsMo_,  doorProceduralState[1]);
        }
    }
}

bool DoorsManager::checkDoesAlarmExist(string distname)
{
    return wrapper_->checkObjectExists(distname);
}


vector<string> DoorsManager::splitString(string strPtr, char sign)
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
