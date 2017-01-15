#ifndef ECHOSENSORSMANAGER_H
#define ECHOSENSORSMANAGER_H
#include "Logger.hpp"
#include "IDBWrapper.hpp"
#include <vector>
class EchoSensorsManager
{
    public:
        EchoSensorsManager(IDBWrapper* wrapper);
        virtual ~EchoSensorsManager();
        void initialize();
        bool checkIsRevertShiftEnabled();
    protected:

    private:
        IDBWrapper* wrapper_;
        vector<Mo> bumperMos_;
        vector<Mo> eSensorMos_;

        void createEchoSensorsObjects(string parent);
        void createBumperObjects();
        void startMeasurement();


};

#endif // ECHOSENSORSMANAGER_H
