#pragma once

#include <basis/seadTypes.h>
#include <heap/seadDisposer.h>
#include <mc/seadCoreInfo.h>
#include <prim/seadSafeString.h>
#include "KingSystem/GameData/gdtTriggerParam.h"
#include "container/seadListImpl.h"
#include "filedevice/seadArchiveFileDevice.h"
#include "filedevice/seadFileDevice.h"
#include "thread/seadThread.h"
#include "KingSystem/Resource/resHandle.h"


namespace ksys {


class SaveSystem {
    SEAD_SINGLETON_DISPOSER(SaveSystem)
    SaveSystem() = default;
    virtual ~SaveSystem();
public:   
    void init();

private:
    void invokedAutoSave();
    void invoked8();
    void invoked3();
    void invoked6();
    void invokedGetMapName();
    void newDayCallback();
    void gdmResetCallback();
    void gdmResetCallback2();
    void finishLoadCb();
    void callback();

    struct Invoker {
        void* otherFn;
        void* fn;
        void* vptr;
    };

    sead::Heap* albumInfoHeap{};
    void* albumInfo{};
    sead::BufferedSafeString overridingMapName;
    sead::BufferedSafeString field_1EA0;
    Invoker invokerAutoSave{};
    Invoker invoker8{};
    Invoker invoker3{};
    Invoker invoker5{};
    Invoker invoker6{};
    Invoker getMapNameInvoker{};
    //sead::ListNode newDayCallback;
    sead::ListNode resetCallback;
    sead::ListNode resetCallback2;
    sead::ListNode cb0;
    sead::ListNode cb1;
    //sead::ListNode finishLoadCb;
    sead::ListNode cb1_disposer;
    sead::ListNode finishLoadCb_disposer;
    sead::ListNode resetCallback2_disposer;
    sead::FixedSafeString<0x100> field_1D60;
    sead::SafeString field_1B70;
    sead::SafeString field_1C50;
    sead::SafeString field_1BE0;
};

}