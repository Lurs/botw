#include "KingSystem/GameData/gdtSaveMgr.h"
#include "KingSystem/Utils/HeapUtil.h"
#include "heap/seadHeap.h"
#include "thread/seadDelegateThread.h"
#include "thread/seadMessageQueue.h"
#include "thread/seadThread.h"

namespace ksys {

    SEAD_SINGLETON_DISPOSER_IMPL(SaveMgr)

    void SaveMgr::init(const InitArg& arg) {
       
       mDoNotSave= arg.is_demo;
        auto heap_size = (int32_t) arg._8;
        mSaveAreaHeap = util::DualHeap::create(heap_size,sead::SafeString("SaveMgr"), arg.heap,arg._60,
          sizeof(void*), sead::Heap::cHeapDirection_Forward, false);
        u32 temp_68 = arg._68;
        u64 buf_size;
        u32 size;
        u64 alignment;
        void *buffer;
        sead::Heap* heap = nullptr;
        sead::Heap* debug_heap2 = util::getDebugHeap2();
        bool use_Debug_Heap = debug_heap2 != nullptr && temp_68 != 0;
        if (use_Debug_Heap) {
            mSaveAreaHeap->tryAlloc(Casting(arg.buf_size), (s32) arg.buf_alignment);
            //(sead::Buffer<sead::Heap*>) mSaveAreaHeap->tryAllocBuffer(arg.buf_size, mSaveAreaHeap, (s32) arg.buf_alignment);

            size = arg.buf_size + temp_68;
            mSaveBufSize = size;
            alignment = arg.buf_alignment;
            heap = debug_heap2;
        }
        else {
            mSaveBufSize = arg.buf_size;
            size = arg.buf_size;           
            heap = mSaveAreaHeap;
            alignment = arg.buf_alignment;
        }

        buffer = operator new[](size, heap, alignment);
        buf_size = mSaveBufSize;
        mSaveBuf = buffer;
        //sead::MemUtil::fillZero(mSaveAreaHeap, mSaveBufSize);
        std::memset(buffer, 0, buf_size);

        mFieldE20 = operator new[](0x100, mSaveAreaHeap, 8);
        //std::memset(mFieldE20, 0, 0x100);

        mFieldE30 = operator new[](static_cast<int64_t>(static_cast<int32_t>(arg.size2)), mSaveAreaHeap, (s32) arg.buf_alignment);
        
        std::memset(mFieldE30, 0, arg.size2);
        mFieldE38 = arg.size2;

        //heap = mSaveAreaHeap;
       
        mThread= new sead::DelegateThread(
            "SaveMgrThread", new (mSaveAreaHeap) sead::Delegate2 <SaveMgr, sead::Thread*,sead::MessageQueue::Element>(
            this, &SaveMgr::SaveMgrThread),
            mSaveAreaHeap,
            (s32) arg.thread_priority,sead::MessageQueue::BlockType::Blocking,
            0x7fffffff, (s32) arg._1c, 0x20);
        mThread->setAffinity(arg.thread_affinity);
        mThread->start();
        mountSaveData(arg);
        mState = 0;

        if (mDoNotSave) {
            mIsRidDemo = arg.save_rid_demo_dir.isEmpty();
            if (use_Debug_Heap) {
                mSaveAreaHeap->tryAlloc(static_cast<int64_t>(static_cast<int32_t>(arg.buf_size)), (s32)arg.buf_alignment);
                //buf_size = mSaveBufSize;
                heap = debug_heap2;
            }
            else {
                heap = mSaveAreaHeap;
                //buf_size = mSaveBufSize;
            }
            buffer = operator new[](mSaveBufSize, heap, arg.buf_alignment);
            mSaveBuf3 = buffer;
            std::memset(buffer, 0,mSaveBufSize);
        }
        else {
            mIsRidDemo = false;
        }
    
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

    u64 SaveMgr::Casting(u32 arg) const {
        return static_cast<int64_t>(static_cast<int32_t>(arg));
    }
    

}

