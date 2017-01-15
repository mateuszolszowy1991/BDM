#ifndef DOORSMANAGER_H
#define DOORSMANAGER_H
#include "IDBWrapper.hpp"
#include <vector>
#include <sstream>
#include <fstream>

#include "Logger.hpp"



using namespace std;
class DoorsManager
{
    public:
        DoorsManager(IDBWrapper* wrapper);
        virtual ~DoorsManager();
        int waitTillCARWillBeInInitializingState();
        void createBDMObject();
        void createDOORSContainer();
        void createAndSetDOORObjects();
        void completeConfiguring();
        void getOperationCodeFromRCDM(string signal);
        const int MIN_POWER_LEVEL = 80;
        bool waitForUserOpenDoorSignal();
        void someDoorsHaveBeenOpened();

    protected:
        void createWindowObjects(string parent);
        void createWiperObjects(string distname);
    private:
        void lockAllDoorsOperation();
        void unlockAllDoorsOperation();
        void switchTrunkOperation();
        bool checkDoesAlarmExist(string distname);


        Mo carMo_;
        Mo bdmMo_;
        Mo doorsMo_;
        vector<Mo> doorMos_;
        vector<Mo> windowMos_;
        vector<Mo> wiperMos_;
        vector<string> doorLabels_ = {"MASK", "FRONT_LEFT", "FRONT_RIGHT", "BACK_LEFT", "BACK_RIGHT", "TRUNK"};
        vector<string> doorProceduralState = {"unset", "Locked", "Unlocked"};
        IDBWrapper* wrapper_;
        string userDoorsSignal_;

        vector<string> splitString(string strPtr, char sign);
};

#endif // DOORSMANAGER_H
