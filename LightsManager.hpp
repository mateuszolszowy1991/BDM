#ifndef LIGHTSMANAGER_H
#define LIGHTSMANAGER_H
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Client.hpp"
#include "Logger.hpp"
#include "IDBWrapper.hpp"
#include <unistd.h>
#include <chrono>
#include <thread>

using namespace std;

class LightsManager
{
    public:

        LightsManager(IDBWrapper* wrapper);
        virtual ~LightsManager();
        void createLIGHTSContainer();
        void createAndSetLIGHTObjects();
        void completeConfiguring();
        void getOperationCodeFromRCDM(string rcdmMsg);
        void waitForUserLightAction();
        const int MIN_POWER_LEVEL = 80;
    protected:

    private:
        const int BLINK_INT = 200;
        Mo findLocalMo(string detail, string value) const;
        void blink(int times);
        Mo carMo_;
        Mo bdmMo_;
        Mo lightsMo_;
        string userLightsSignal_;
        void onUserSignalAction(string light, string action);
        vector<Mo> lightMos_;
        IDBWrapper* wrapper_;
        vector<string> lightLabels_ = {"DAILY_RIGHT","DAILY_LEFT","POSITION_FRONT_LEFT","POSITION_FRONT_RIGHT","POSITION_BACK_LEFT","POSITION_BACK_RIGHT",
                                        "BEAM_LEFT","BEAM_RIGHT","BLINKER_LEFT_FRONT","BLINKER_RIGHT_FRONT","BLINKER_LEFT_CENTER","BLINKER_RIGHT_CENTER",
                                        "BLINKER_LEFT_BACK","BLINKER_RIGHT_BACK","STOP_LEFT", "STOP_CENTER", "STOP_RIGHT", "FOG", "BACK"};
        vector<string> splitString(string strPtr, char sign);
};

#endif // LIGHTSMANAGER_H
