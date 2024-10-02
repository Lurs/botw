#include "KingSystem/GameData/gdtSaveMgr.h"
#include "KingSystem/Utils/HeapUtil.h"
#include "heap/seadHeap.h"
#include "thread/seadDelegateThread.h"
#include "thread/seadMessageQueue.h"
#include "thread/seadThread.h"

namespace ksys {

    SEAD_SINGLETON_DISPOSER_IMPL(SaveMgr)

    void SaveMgr::init(const InitArg& arg) {
       
        mDoNotSave = arg.is_demo;
        bool* p_doNotMountSave = &mDoNotSave;
        auto heap_size = static_cast<s32>(arg._8) ;

        mSaveAreaHeap = util::DualHeap::create(heap_size,sead::SafeString("SaveMgr"), arg.heap,arg._60,
          sizeof(void*), sead::Heap::cHeapDirection_Forward, false);
        //int v6 = arg._68;
        u32 temp_68 = arg._68;
        sead::Heap* debug_heap2 = ksys::util::getDebugHeap2();
        //bool v8 = (v6 != 0) && (DebugHeap2 != nullptr);
        bool use_Debug_Heap = debug_heap2 != nullptr && temp_68 != 0;

        char* buffer;
        if (use_Debug_Heap) {
            mSaveAreaHeap->tryAlloc(static_cast<s32>(arg.buf_size), static_cast<s32>(arg.buf_alignment));

            mSaveBufSize = arg.buf_size + temp_68;
            buffer = static_cast<char*>(operator new[](mSaveBufSize, debug_heap2, static_cast<s32>(arg.buf_alignment)));
        } else {
            mSaveBufSize = arg.buf_size;
            buffer = static_cast<char*>(operator new[](mSaveBufSize, mSaveAreaHeap, static_cast<s32>(arg.buf_alignment)));
        }
        mSaveBuf = buffer;
        memset(mSaveBuf, 0, mSaveBufSize);

        mFieldE20 = operator new[](0x100, mSaveAreaHeap, 8);

        void* buffer2 = operator new[](static_cast<s64>(static_cast<s32>(arg.size2)), mSaveAreaHeap, static_cast<s32>(arg.buf_alignment));
        mFieldE30 = buffer2;
        memset(buffer2, 0, static_cast<s64>(static_cast<s32>(arg.size2)));

        mFieldE38 = arg.size2;

        // Allocate memory for the thread using operator new (thread = operator new(0x108uLL, v16, 8);)
        void* thread = operator new(0x108uLL, mSaveAreaHeap, 8);

        sead::SafeString thread_name("SaveMgrThread");

        // Create Delegate2 with the correct member function pointer
        auto* params = new (mSaveAreaHeap) sead::Delegate2<SaveMgr, sead::Thread*, sead::MessageQueue::Element>(
            this, &SaveMgr::SaveMgrThread);

        // Create the DelegateThread using the allocated memory and initialized parameters
        mThread = new (thread) sead::DelegateThread(
            thread_name,
            params,
            mSaveAreaHeap,
            static_cast<s32>(arg.thread_priority),
            sead::MessageQueue::BlockType::Blocking,
            0x7fffffff,
            static_cast<s32>(arg._1c),
            0x20
        );
        mThread->setAffinity(arg.thread_affinity);
        mThread->start();
        mountSaveData(arg);
        mState = 0;

        if (*p_doNotMountSave) {
            mIsRidDemo = arg.save_rid_demo_dir.isEmpty();
            if (use_Debug_Heap) {
                mSaveAreaHeap->tryAlloc(static_cast<s32>(static_cast<s32>(arg.buf_size)), static_cast<s32>(arg.buf_alignment));
                mSaveBuf3 = operator new[](mSaveBufSize, debug_heap2, static_cast<s32>(arg.buf_alignment));
            } else {
                mSaveBuf3 = operator new[](mSaveBufSize, mSaveAreaHeap, static_cast<s32>(arg.buf_alignment));
            }
            memset(mSaveBuf3, 0, mSaveBufSize);
        } else {
            mIsRidDemo = 0;
        }

        this->mFieldF8 = 1;
    
    }

    void SaveMgr::SaveMgrThread(sead::Thread* thread, sead::MessageQueue::Element element) {
        switch(element) {
            case 1:
                 SaveMgrThreadCmd1();
                 break;
            case 2:
                SaveMgrThreadCmd2();
                break;
            case 3:
                SaveMgrThreadCmd3();
                break;
            case 4:
                if(this->mSaveAreaHeap)
                break;
            default:
                return;

        }
    }


    

}

