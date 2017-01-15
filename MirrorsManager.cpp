#include "MirrorsManager.hpp"

MirrorsManager::MirrorsManager(IDBWrapper* wrapper) : wrapper_(wrapper)
{
    Logger::saveToFile("BDM/INF: MirrorsManager: Ctor");
    //mirrorsMo = wrapper_->createObject("CAR/BDM/MIRRORS", "Online", "Closed");
    //createMirrorMos();

}

MirrorsManager::~MirrorsManager()
{
    //dtor
}

void MirrorsManager::createMirrorMos()
{
    mirrorMos_.push_back(wrapper_->createObject("CAR/BDM/MIRRORS/MIRROR_1", "Online", "Closed"));
    wrapper_->expandObject(&mirrorMos_.back(), {"Label", "posX", "posY"}, {"LEFT", "0", "0"});
    mirrorMos_.push_back(wrapper_->createObject("CAR/BDM/MIRRORS/MIRROR_2", "Online", "Closed"));
    wrapper_->expandObject(&mirrorMos_.back(), {"Label", "posX", "posY"}, {"RIGHT", "0", "0"});
    mirrorMos_.push_back(wrapper_->createObject("CAR/BDM/MIRRORS/MIRROR_3", "Online", "Closed"));
    wrapper_->expandObject(&mirrorMos_.back(), {"Label", "posX", "posY"}, {"BACK", "0", "0"});
}

void MirrorsManager::getOperationCodeFromRCDM(string rcdmMsg)
{
    vector<string> splittedSignal = splitString(rcdmMsg, ';');
    if(atoi((splittedSignal[1]).c_str()) >= MIN_POWER_LEVEL)
    {
        if(splittedSignal[2] == "0x1" || splittedSignal[2] == "0x3")
        {
            setMirrors();
        }
        else if(splittedSignal[2] == "0x2")
        {
            resetMirrors();
        }
    }
}

void MirrorsManager::setMirrors()
{
    for(auto mirrorMo : mirrorMos_)
    {
        wrapper_->setProceduralState(&mirrorMo, "Opened");
    }
}

void MirrorsManager::resetMirrors()
{
    for(auto mirrorMo : mirrorMos_)
    {
        wrapper_->setProceduralState(&mirrorMo, "Closed");
    }
}

vector<string> MirrorsManager::splitString(string strPtr, char sign)
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
