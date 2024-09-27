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

    u64 Casting(u32 arg) const;

private:
    sead::Heap* mSaveAreaHeap = nullptr;
    sead::Thread* mThread = nullptr;
    u32 mState = 3;
    u32 _3c = 4;
    u32 _40 = 4;
    sead::FixedSafeString<0x20> _48;
    sead::FixedSafeString<0x20> mSaveActStr2;
    u16 _B8 = 0;
    sead::FixedSafeString<0x20> mSaveActStr3;
    //sead::FixedSafeString<0x20> _108;
    u16 mFieldF8 = 1;
    u16 _140 = 0;
    u32 _144 = 0x1E;
    u32 *mSaveSlotMaybe[0x84] = {0};
    sead::FixedSafeString<0x100> _1D0;
    u64 _318 = 0;
    u64 _310 = 0;
    u64 _308 = 0;
    u64 _300 = 0;
    u64 _2F8 = 0;
    u64 _2F0 = 0;
    u32 _320 = 0;
    sead::FileHandle mFileHandle;
    sead::ArchiveFileDevice mGameDataArc{nullptr};



    
    void* bgsvdata = nullptr;

    
    

    sead::FixedSafeString<0x100> _E60;
    u32 _F78 = 4;
    u16 _F8C[8] = {0};
    gdt::TriggerParam* mTriggerParam = nullptr;
    u64 _F88 = 0xFFFFFFFF;
    s32 _F90 = 0;
    //vtable _FA09 = &off_71024DFF28
    u32 _FB8 = 0;
    struct _FA8 {
        sead::ListNode* next;
        sead::ListNode* prev;
    };
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
    u32 mCaptionPictSizeCrc32 = 0;
    sead::DelegateFunc mInvoker3{}; //pInvoker3 = 0LL;
    sead::DelegateFunc mInvoker8{}; //pInvoker8 = 0LL;
    sead::DelegateFunc mInvokerAutoSave = {}; // invokerAutoSave = 0LL;
    u32 mField_103C = 0xFFFFFFFF;
    //vtable_1000 = &off_71024DFF88;
    //finishLoadCbs.next = &a1->finishLoadCbs;
    //finishLoadCbs.prev = &a1->finishLoadCbs;
    u8 mCheatBuf[0x541]; 
    
    void* mSaveBuf = nullptr;   
    u32 mSaveBufSize;
    u8 mCheatBufx;
    u32 _E18 = 0;    
    void* mFieldE20 = nullptr;
    u32 _E28 = 0;
    u32 _E2C = 0;    
    void* mFieldE30 = nullptr;
    u32 mFieldE38 = 0;
    u32 mCheatBufy;    
    s32 _E40 = 0;
    s64 _E44 = 0;
    u64 _E50 = 0;
    s32 _E58 = 0;
    //bool mCheatBuf3[2];
    
    u8 mCheatBuf4[0x122];
    void* mSaveBuf3 = nullptr;
    u8 mCheatBuf5[0x70];
    u32 mField_FF8 = 0;
    u8 mCheatBuf6[0x40];
    sead::SafeString mField_1040;
    u64 mField_1050 = 0;   
    bool mField_1058[1] = {0}; 
    bool mField_1059 = true;
    bool mIsRidDemo = true;

    bool mDoNotSave = false;
    //ksys::res::Handle::Handle(&this->res2b);
    sead::FixedSafeString<0x100> mField_10B0;
    u64 mField_11E0 = 0;
    u64 mField_11D8 = 0;
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
  sead::PtrArrayImpl::setBuffer(&a1->gap11C8, 0x80, a1->ptrArrayData); */
        
    
    


};


}  // namespace ksys
