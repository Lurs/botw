#include "KingSystem/GameData/gdtSaveMgr.h"
#include "KingSystem/Utils/HeapUtil.h"
#include "filedevice/seadFileDeviceMgr.h"
#include "heap/seadHeap.h"
#include "prim/seadRuntimeTypeInfo.h"
#include "thread/seadDelegateThread.h"
#include "thread/seadMessageQueue.h"
#include "thread/seadThread.h"
#include "KingSystem/GameData/gdtManager.h"

namespace ksys {

SEAD_SINGLETON_DISPOSER_IMPL(SaveMgr)

void SaveMgr::init(const InitArg& arg) {
    mDoNotSave_105B = arg.is_demo;
    auto heap_size = static_cast<s32>(arg._8);

    mSaveAreaHeap = util::DualHeap::create(heap_size,sead::SafeString("SaveMgr"), arg.heap,arg._60,
        sizeof(void*), sead::Heap::cHeapDirection_Forward, false);
        
    u32 temp_68 = arg._68;
    sead::Heap* debug_heap2 = ksys::util::getDebugHeap2();        
    bool use_Debug_Heap = debug_heap2 != nullptr && temp_68 != 0;

    char* buffer;
    if (use_Debug_Heap) {
        //mSaveAreaHeap->tryAlloc(static_cast<s32>(arg.buf_size), static_cast<s32>(arg.buf_alignment));
        mSaveAreaHeap->alloc(static_cast<s32>(arg.buf_size),static_cast<s32>(arg.buf_alignment));
        mSaveBufSize_E10 = arg.buf_size + temp_68;
        buffer = static_cast<char*>(operator new[](mSaveBufSize_E10, debug_heap2, static_cast<s32>(arg.buf_alignment)));
    } else {
        mSaveBufSize_E10 = arg.buf_size;
        buffer = static_cast<char*>(operator new[](mSaveBufSize_E10, mSaveAreaHeap, static_cast<s32>(arg.buf_alignment)));
    }
    mSaveBuf_E08 = buffer;
    sead::MemUtil::fillZero(mSaveBuf_E08,mSaveBufSize_E10);
    //memset(mSaveBuf, 0, mSaveBufSize);

    mField_E20 = operator new[](0x100, mSaveAreaHeap, 8);

    void* buffer2 = operator new[](static_cast<s64>(static_cast<s32>(arg.size2)), mSaveAreaHeap, static_cast<s32>(arg.buf_alignment));
    mField_E30 = buffer2;
    //memset(buffer2, 0, static_cast<s64>(static_cast<s32>(arg.size2)));
    sead::MemUtil::fillZero(buffer2,static_cast<s64>(static_cast<s32>(arg.size2)));

    mField_E38 = arg.size2;

    mThread = new (mSaveAreaHeap, 8) sead::DelegateThread(
        sead::SafeString ("SaveMgrThread"),
        new (mSaveAreaHeap) sead::Delegate2<SaveMgr, sead::Thread*, sead::MessageQueue::Element>(
            this, &SaveMgr::SaveMgrThread),
        mSaveAreaHeap,
        static_cast<s32>(arg.thread_priority),
        sead::MessageQueue::BlockType::Blocking,
        0x7FFFFFFF,
        static_cast<s32>(arg._1c),
        0x20
    );
    mThread->setAffinity(arg.thread_affinity);
    mThread->start();
    mountSaveData(arg);
    mState = 0;

    if (mDoNotSave_105B) {
        mIsRidDemo_105A = arg.save_rid_demo_dir.isEmpty();
        if (use_Debug_Heap) {
            //mSaveAreaHeap->tryAlloc(static_cast<s32>(static_cast<s32>(arg.buf_size)), static_cast<s32>(arg.buf_alignment));
            mSaveAreaHeap->alloc(static_cast<s32>(arg.buf_size),static_cast<s32>(arg.buf_alignment));
            mSaveBuf3_F80 = operator new[](mSaveBufSize_E10, debug_heap2, static_cast<s32>(arg.buf_alignment));
        } else {
            mSaveBuf3_F80 = operator new[](mSaveBufSize_E10, mSaveAreaHeap, static_cast<s32>(arg.buf_alignment));
        }
        //memset(mSaveBuf3, 0, mSaveBufSize);
        sead::MemUtil::fillZero(mSaveBuf3_F80,mSaveBufSize_E10);
    } else {
        mIsRidDemo_105A = 0;
    }

    this->mField_F8 = 1;
}


//extern 
extern void (*qword_7102606A98)();

void SaveMgr::SaveMgrThread(sead::Thread* thread, sead::MessageQueue::Element element) {
    u64 idx = static_cast<u64>(element) - 1;


    switch (idx) {
        case 0:
            SaveMgrThreadCmd1();
            break;
        case 1:
            SaveMgrThreadCmd2();
            break;
        case 2:
            SaveMgrThreadCmd3();
            break;
        case 3:
            if (qword_7102606A98)
                qword_7102606A98();
            break;
        default:
            return;
    }
}

void SaveMgr::SaveMgrThreadCmd1() {
    // Step 1: Check if gdt::Manager instance exists
    auto* gdt_manager = gdt::Manager::instance();
    if (gdt_manager) {
        // Retrieve TriggerParam using public methods
        mTriggerParam_F98 = gdt_manager->getParam().get().getBuffer();

        // Reset all flags to initial values using the TriggerParam instance
        if (mTriggerParam_F98) {
            mTriggerParam_F98->resetAllFlagsToInitialValues();
        }

        // Iterate over mField_FF0 linked list
        auto* current = static_cast<SaveCallback*>(mField_FF0);
        auto* end = static_cast<SaveCallback*>(mField_FE8);

        if (current != end) {
            do {
                auto& disposer = current->mDisposer;
                if (disposer.o.heap && disposer.o.heap->size) {
                    disposer.o.heap->size->invoke(this); // Pass 'this' as context
                }
                current = current->next;
            } while (current != end);
        }

        // Set mField_1050 to zero
        mField_1050 = 0;

        // Clear bit 3 (value 8) in mField_1058[0]
        mField_1058[0] &= ~8u;
    }

  // Step 2: Handle mDoNotSave_105B
    if (mDoNotSave_105B) {
        // Find the device using mSaveActStr2_80
        sead::FileDevice* device = sead::FileDeviceMgr::instance()->findDevice(mSaveActStr2_80);

        bool function_called = false;

        if (!device) {
            function_x_2();
            function_called = true;
        } else {
            // Use sead::IsDerivedFrom to check the type
            if (sead::IsDerivedFrom<sead::ArchiveFileDevice>(device)) {
                function_x_2();
                function_called = true;
            } else {
                // Check buffer contents
                bool non_zero_found = false;
                if (mSaveBuf3_F80 && mSaveBufSize_E10 > 0) {
                    u32 offset = 0;
                    while (offset < mSaveBufSize_E10) {
                        if (*(u32*)((char*)mSaveBuf3_F80 + offset) != 0) {
                            non_zero_found = true;
                            break;
                        }
                        offset += 4;
                    }
                }

                if (non_zero_found) {
                    function_x_2();
                    function_called = true;
                } else {
                    // Clear bit 3 (value 8) in mField_140
                    _140 &= ~8u;

                    // Call function_a
                    function_a();
                }
            }
        }

        // Proceed with the rest of the function
    }

    // Step 3: Continue with the rest of the function
    auto* trigger_param = mTriggerParam_F98;
    

    if (gdt_manager) {
        // Clear bit 2 (value 4) in flags using public methods
        gdt_manager->requestResetAllFlagsToInitial();

        // Copy parameters
        gdt_manager->copyParamToParam1();

        // Clear bit 0 (value 1) in flags using public methods
        gdt_manager->resetBitFlag(gdt::Manager::BitFlag::_1);

        // Reset changeOnlyOnce flags
        gdt_manager->setChangeOnlyOnce(false);
    }

    // Iterate over mFinishLoadCbs_1000 linked list
    auto* current_cb = static_cast<SaveCallback*>(mFinishLoadCbs_1000.next);
    auto* end_cb = static_cast<SaveCallback*>(&mFinishLoadCbs_1000);

    if (current_cb != end_cb) {
        do {
            auto& disposer = current_cb->mDisposer;
            if (disposer.o.heap && disposer.o.heap->size) {
                disposer.o.heap->size->invoke(this);
            }
            current_cb = current_cb->next;
        } while (current_cb != end_cb);
    }

    // Set mState to zero
    mState = 0;

    // Initialize mGap14C with 0xFF
    sead::MemUtil::fill(mGap14C, 0xFF, sizeof(mGap14C));

    // Clear bit 1 (value 2) in mField_1058[0]
    mField_1058[0] &= ~2u;

    // Update mIsRidDemo_105A and mField_10B0 if necessary
    if (mIsRidDemo_105A) {
        mIsRidDemo_105A = mDoNotSave_105B;
        mField_10B0.clear(); // Assuming mField_10B0 is a sead::FixedSafeString<0x100>
    }

    // Reset fields if _E4C is true
    if (_E4C) {
        _E4C = 0;
        _E58 = 0;
        _E50 = 0;
    }
}

}  // namespace ksys
