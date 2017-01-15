#ifndef MIRRORSMANAGER_H
#define MIRRORSMANAGER_H
#include "IDBWrapper.hpp"
#include <vector>


class MirrorsManager
{
    public:
        MirrorsManager(IDBWrapper* wrapper);
        virtual ~MirrorsManager();
        void getOperationCodeFromRCDM(string rcdmMsg);

    protected:

    private:
        vector<string> splitString(string strPtr, char sign);
        const int MIN_POWER_LEVEL = 80;
        void createMirrorMos();
        void setMirrors();
        void resetMirrors();
        IDBWrapper* wrapper_;
        Mo mirrorsMo;
        vector<Mo> mirrorMos_;
};

#endif // MIRRORSMANAGER_H
