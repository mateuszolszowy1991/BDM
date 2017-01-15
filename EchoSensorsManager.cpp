#include "EchoSensorsManager.hpp"

EchoSensorsManager::EchoSensorsManager(IDBWrapper* wrapper) : wrapper_(wrapper)
{
    Logger::saveToFile("BDM/INF: EchoSensorsManager: Ctor");
}

EchoSensorsManager::~EchoSensorsManager()
{
    //dtor
}

void EchoSensorsManager::initialize()
{
    createBumperObjects();
    /*if(checkIsRevertShiftEnabled())
    {
        for(auto &sensor : eSensorMos_)
        {
            wrapper_->setProceduralState(&sensor, "Enabled");

        }
        startMeasurement();
    }*/
}

void EchoSensorsManager::createBumperObjects()
{
    Mo bumperMo = wrapper_->createObject("CAR/BDM/BUMPER_1", "NA", "NA");
    wrapper_->expandObject(&bumperMo, {"Label"}, {"FRONT"});
    createEchoSensorsObjects(bumperMo.distname);
    bumperMos_.push_back(bumperMo);
    bumperMo = wrapper_->createObject("CAR/BDM/BUMPER_2", "NA", "NA");
    wrapper_->expandObject(&bumperMo, {"Label"}, {"BACK"});
    createEchoSensorsObjects(bumperMo.distname);
    bumperMos_.push_back(bumperMo);
}

void EchoSensorsManager::createEchoSensorsObjects(string parent)
{
    for(int i = 1; i <= 4; i++)
    {
        Mo eSensor = wrapper_->createObject(parent + "/ECHO_SENSOR_" + to_string(i), "Online", "Disabled");
        eSensorMos_.push_back(eSensor);
    }

}

bool EchoSensorsManager::checkIsRevertShiftEnabled()
{
    Mo requestMo = wrapper_->setObserverForObjectCreate("CAR/MDM/GEARBOX/ECHO_SENSORS_ENABLE_REQ");
    if(requestMo.distname == "CAR/MDM/GEARBOX/ECHO_SENSORS_ENABLE_REQ")
    {
        return true;
    }
    return false;
}

void EchoSensorsManager::startMeasurement()
{
    Logger::saveToFile("BDM/INF: EchoSensorsManager: measurement started");
}
