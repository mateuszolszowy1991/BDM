#include "LightsManager.hpp"

LightsManager::LightsManager(IDBWrapper* wrapper) : wrapper_(wrapper)
{
    Logger::saveToFile("BDM/INF: LightsManager: ctor");
}

LightsManager::~LightsManager()
{
    //dtor
}

void LightsManager::createLIGHTSContainer()
{
    lightsMo_ = wrapper_->createObject("CAR/BDM/LIGHTS", "Online", "Configuring");
}

void LightsManager::createAndSetLIGHTObjects()
{
    for(int i = 1; i <=lightLabels_.size(); i++)
    {
        Mo lightMo = wrapper_->createObject("CAR/BDM/LIGHTS/LIGHT_" + to_string(i), "Online", "Off");
        vector<string> values = {lightLabels_[i - 1]};
        vector<string> properties = {"Label"};
        wrapper_->expandObject(&lightMo, properties, values);
        lightMos_.push_back(lightMo);
        //lightMos_.back().showDetails();
    }
}

void LightsManager::getOperationCodeFromRCDM(string rcdmMsg)
{
    vector<string> splittedSignal = splitString(rcdmMsg, ';');
    if(atoi((splittedSignal[1]).c_str()) >= MIN_POWER_LEVEL)
    {
        if(splittedSignal[2] == "0x1" || splittedSignal[2] == "0x3")
        {
            blink(1);
        }
        else if(splittedSignal[2] == "0x2")
        {
            blink(2);
        }
    }
    else
    {
        blink(4);
    }
}

void LightsManager::blink(int times)
{
    string response;
    vector<Mo> mos;
    for(const auto &label : lightLabels_)
    {
        if(label.find("BLINKER"))
        {
            mos.push_back(findLocalMo("Label", label));
        }
    }
    for(int i = 0; i < times; i++)
    {
       for(auto &mo: mos)
        {
            wrapper_->setProceduralState(&mo, "On");
            //response = client_->sendRequest(("0x3-BDM-"+mo.distname + ";" + mo.proceduralStatePtr + ";On").c_str());
        }
        sleep(BLINK_INT);
        for(auto &mo: mos)
        {
            wrapper_->setProceduralState(&mo, "Off");
            //response = client_->sendRequest(("0x3-BDM-"+mo.distname + ";" + mo.proceduralStatePtr + ";Off").c_str());
        }
        sleep(BLINK_INT);
    }

}

void LightsManager::waitForUserLightAction()
{
    ifstream infile("D:/private/IDB/SYS/sigs/USER_LIGHT_SIG.dat");
    if(infile.good())
    {
        fstream sig( "D:/private/IDB/SYS/sigs/USER_LIGHT_SIG.dat", std::ios::in );
        getline(sig, userLightsSignal_);
        sig.close();
        Logger::saveToFile("BDM/INF: DoorsManager: someDoorsHaveBeenOpened: received signal: " + userLightsSignal_ );
        remove("D:/private/IDB/SYS/sigs/USER_LIGHT_SIG.dat");
        vector<string> splittedSignal = splitString(userLightsSignal_, ';');
        onUserSignalAction(splittedSignal[0], splittedSignal[1]);
    }
}

void LightsManager::onUserSignalAction(string lightKind, string action)
{
    for(auto &light: lightMos_)
    {
        if(light.details[0].second.find(lightKind) != string::npos && light.proceduralState != action)
        {
            wrapper_->setProceduralState(&light, action);
        }
    }
}

Mo LightsManager::findLocalMo(string detail, string value) const
{
    for(const auto &mo : lightMos_)
    {
        if(mo.details[0].first == detail && mo.details[0].second == value)
        {
            return mo;
        }
    }
    return Mo();
}

vector<string> LightsManager::splitString(string strPtr, char sign)
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
