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

// FIXME
class SaveMgr {
    SEAD_SINGLETON_DISPOSER(SaveMgr)
    SaveMgr();
    virtual ~SaveMgr();
public:
     struct InitArg {
        sead::Heap* heap;
        u32 _8;
        u32 buf_size;
        u32 buf_alignment;
        u32 size2;
        u32 thread_priority;
        u32 _1c;
        sead::CoreIdMask thread_affinity;
        sead::SafeString save_common_str = "save_common";
        sead::SafeString save_act_str = "save_act";
        bool is_demo = 0;
        sead::SafeString save_rid_demo_dir;
        sead::Heap* _60 = nullptr;
        u32 _68 = 0;
        //u32 _6c = 0;
    };
    static_assert(sizeof(InitArg) == 0x70);



    void init(const InitArg& arg);
    void mountSaveData(const InitArg& arg);
    void loadGameSaveData();
    void SaveMgrThread(sead::Thread*, sead::MessageQueue::Element);
    void SaveMgrThreadCmd1();
    void SaveMgrThreadCmd2();
    void SaveMgrThreadCmd3();
private:
    void function_x_2();
    void function_a();
    sead::Heap* mSaveAreaHeap = nullptr;
    sead::Thread* mThread = nullptr;
    u32 mState = 3;
    u32 _3c = 4;
    u32 _40 = 4;
    sead::FixedSafeString<0x20> _48;
    sead::FixedSafeString<0x20> mSaveActStr2_80;
    u16 _B8 = 0;
    sead::FixedSafeString<0x20> mSaveActStr3_C0;
    u8 mField_F8 = 1;
    u64 _100 = 0;
    sead::FixedSafeString<0x20> _108;    
    u16 _140 = 0;
    u32 _144 = 0x1E;
    u32 *mSaveSlotMaybe_148[0x11] = {0};
    sead::FixedSafeString<0x100> _1D0;
    sead::FileHandle* mFileHandle_2E8;
    u64 _2F0 = 0;
    u64 _2F8 = 0;
    u64 _300 = 0;
    u64 _308 = 0;
    u64 _310 = 0;
    u64 _318 = 0;    
    u32 _320 = 0;
    
    ksys::res::Handle res2_328;
    sead::SafeArray<ksys::res::Handle, 32> bgsvdataRes_378;
    sead::ArchiveFileDevice bgsvdataArchiveRes_D78{nullptr};
    void* bgsvdata_E00 = nullptr;
    void* mSaveBuf_E08 = nullptr;
    u32 mSaveBufSize_E10;
    u32 mSaveBufOffset_E14;
    u32 _E18 = 0;
    u32 mSaveBufSize_E1C;
    void* mField_E20 = nullptr;
    u32 _E28 = 0;
    u32 _E2C = 0;
    void* mField_E30 = nullptr;
    u32 mField_E38 = 0;
    u32 _E3C;
    s32 _E40 = 0;
    s32 _E44 = 0;
    s32 _E48 = 0;
    s32 _E4C = 0;
    u64 _E50 = 0;
    s32 _E58 = 0;
    sead::FixedSafeString<0x100> _E60;
    u32 _F78 = 4;
    void* mSaveBuf3_F80 = nullptr;
    u64 _F88 = 0xFFFFFFFF;
    s32 _F90 = 0;
    s32 _F94 = 0;
    gdt::TriggerParam* mTriggerParam_F98 = nullptr;

    //ksys::gdt::Manager::ResetSignal::Slot mGdtResetSlot;
    //ksys::gdt::Manager::ReinitSignal::Slot mGdtReinitSlot;
    struct _FA8 {
        sead::ListNode* next;
        sead::ListNode* prev;
        void * ptr;
    };
    _FA8 test_FA0;
    u64 _FB8 = 0;
    u32 _FC0[10] = {0};
    void* mField_FE8 = nullptr;
    void* mField_FF0 = nullptr;
    void* mField_FF8 = nullptr;
    

    struct Callbacklist {
      sead::ListNode* next;
      sead::ListNode* prev;
      void * ptr;
      void * ptr2;
    };

    Callbacklist mFinishLoadCbs_1000;
    
    sead::DelegateFunc* mInvokerAutoSave_1020 = nullptr; // invokerAutoSave = 0LL;
    sead::DelegateFunc* mInvoker8_1028= nullptr; //pInvoker8 = 0LL;
    sead::DelegateFunc* mInvoker3_1030= nullptr; //pInvoker3 = 0LL;
    u32 mCaptionPictSizeCrc32_1038 = 0;
    u32 mField_103C = 0xFFFFFFFF;

    sead::SafeString mField_1040;
    u64 mField_1050 = 0;
    bool mField_1058[1] = {0};
    bool mField_1059 = true;
    bool mIsRidDemo_105A = true;

    bool mDoNotSave_105B = false;
    ksys::res::Handle res2b_1060;
    //ksys::res::Handle::Handle(&this->res2b);
    sead::FixedSafeString<0x100> mField_10B0;
    sead::PtrArrayImpl test_11C8;
    u64 mField_11D8 = 0;
    u64 mField_11E0 = 0;
    u64 mField_11F0[4] = {};
    u64 mField_1208 = 0;
    
    
    //sead::PtrArrayImpl::setBuffer(&a1->gap11C8, 0x80, a1->ptrArrayData);
    
    //vtable _FA09 = &off_71024DFF28
    
    
    //someCbList.size = 0;
    //a1->someCbList.next = &a1->someCbList;
    //a1->someCbList.prev = &a1->someCbList;
    //vtable_FC0 = &off_71024DFF48;
    //someCbList.next = &a1->someCbList;
    //someCbList.prev = &a1->someCbList;
    //vtable_FE0 = &off_71024DFF68;
    //field_FF0 = &a1->field_FE8;
    //field_FE8 = this->field_FE8;

    //CallbackListfinishLoadCbs.size = 0;
   
    
    
    //vtable_1000 = &off_71024DFF88;
    //finishLoadCbs.next = &a1->finishLoadCbs;
    //finishLoadCbs.prev = &a1->finishLoadCbs;
    
    
    
    //bool mCheatBuf3[2];

    //u8 mCheatBuf4[0x122];
    
    //u8 mCheatBuf6[0x40];
    
    //gap11C8.ptrs = 0LL;
    //gap11C8.size = 0LL;
    /*v3 = &a1->field_1208;
    v4 = 0x7ELL;
    a1->field_11D8 = &a1->field_11E8;
    while ( 1 )
    {
    v3[0xFFFFFFFC] = v3 + 0xFFFFFFFE;
    if ( !v4 )
      break;
    v3[0xFFFFFFFE] = v3;
    v3 += 4;
    v4 -= 2LL;
  }
  a1->field_19D8 = 0LL;
  a1->field_11E0 = &a1->field_11E8;
   */
        
      


};


}  // namespace ksys
