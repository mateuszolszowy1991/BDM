#include "Client.hpp"
#include "DoorsManager.hpp"
#include "LightsManager.hpp"
#include "RemoteContollerDeviceManager.hpp"
#include "IDBWrapper.hpp"
#include "EchoSensorsManager.hpp"
#include "MirrorsManager.hpp"
#include <iostream>
using namespace std;

int main()
{
    Logger::clearLogs();
    Logger::saveToFile("BodyDeviceManager application started ");
    Client* client = new Client();
    IDBWrapper* wrapper = new IDBWrapper(client, "BDM");
    LightsManager* lightsManager_ = new LightsManager(wrapper);
    RemoteContollerDeviceManager* rcdm = new RemoteContollerDeviceManager(wrapper);
    DoorsManager* doorsManager_ = new DoorsManager(wrapper);
    EchoSensorsManager* eSManager_ = new EchoSensorsManager(wrapper);
    MirrorsManager* mirrorManager_ = new MirrorsManager(wrapper);
    if(client->clientInitialize())
    {
        client->connectToIDB();
        client->start();
    }
    client->sendRequest("0x0-BDM-Welcome");
    if(doorsManager_->waitTillCARWillBeInInitializingState())
    {
        doorsManager_->createBDMObject();
        doorsManager_->createDOORSContainer();
        doorsManager_->createAndSetDOORObjects();
        doorsManager_->completeConfiguring();
    }

    lightsManager_->createLIGHTSContainer();
    lightsManager_->createAndSetLIGHTObjects();

    rcdm->createRCObject();
    eSManager_->initialize();
    while(true)
    {
        if(rcdm->waitForSignal())
        {
            string rcdmMsg = rcdm->startSignalProceeding();

            doorsManager_->getOperationCodeFromRCDM(rcdmMsg);
            //lightsManager_->getOperationCodeFromRCDM(rcdmMsg);
            mirrorManager_->getOperationCodeFromRCDM(rcdmMsg);
        }
        else if(doorsManager_->waitForUserOpenDoorSignal())
        {
            doorsManager_->someDoorsHaveBeenOpened();
        }
        lightsManager_->waitForUserLightAction();

    }

    getchar();
}
