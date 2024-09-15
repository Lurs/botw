#include "KingSystem/GameData/gdtSaveMgr.h"
#include "KingSystem/Utils/HeapUtil.h"
//#include "heap/seadExpHeap.h"
#include "heap/seadHeap.h"
#include "thread/seadDelegateThread.h"
#include "thread/seadMessageQueue.h"
#include "thread/seadThread.h"
namespace ksys {

    SEAD_SINGLETON_DISPOSER_IMPL(SaveMgr)

    void SaveMgr::init(const InitArg& arg) {
        mDoNotSave= arg.is_demo; 
        sead::Heap *heap;               
        mSaveAreaHeap = util::DualHeap::create(arg._8, sead::SafeString("SaveMgr"), arg.heap,nullptr,
         sizeof(void*), sead::Heap::cHeapDirection_Forward, false);
         
        sead::Heap* debug_heap2 = util::getDebugHeap2();
        bool use_debug_heap = arg._68 != 0 && debug_heap2 != nullptr;
        if (use_debug_heap) {
            mSaveAreaHeap->tryAlloc(arg.buf_size, static_cast<s32>(arg.buf_alignment));
            mSaveBufSize = arg.buf_size + arg._68;
            heap = debug_heap2;
        }
        else {
            mSaveBufSize = arg.buf_size;
            mSaveAreaHeap = debug_heap2;
            heap = mSaveAreaHeap;
        }
        
        mBuf3 = operator new[](mSaveBufSize, mSaveAreaHeap, (s32) arg.buf_alignment);
        std::memset(mBuf3, 0, mSaveBufSize);

        mFieldE30 = operator new[](arg.size2, mSaveAreaHeap, (s32) arg.buf_alignment);
        std::memset(mFieldE30, 0, arg.size2);
        mFieldE38 = arg.size2;

        sead::Delegate2<SaveMgr, sead::Thread*, sead::MessageQueue::Element> thread_params;
        mThread= new sead::DelegateThread("SaveMgrThread",&thread_params, heap,
                             (s32) arg.thread_priority,
                             sead::MessageQueue::BlockType::Blocking, 0x7fffffff, (s32) arg._1c, 0x20);
        mThread->setAffinity(arg.thread_affinity);
        mThread->start();
        mountSaveData(arg);
        this->mState = 0;

        if (mDoNotSave) {
            mIsRidDemo = *arg.save_rid_demo_dir.cstr() == sead::SafeStringBase<char>::cNullChar;
            if (arg.save_rid_demo_dir.cstr()[0] == sead::SafeStringBase<char>::cNullChar && use_debug_heap) {
                mSaveAreaHeap->tryAlloc(arg.buf_size, static_cast<s32>(arg.buf_alignment));
            }
            mBuf3 = operator new[](mSaveBufSize, mSaveAreaHeap, (s32) arg.buf_alignment);
            std::memset(mBuf3, 0, mSaveBufSize);
        }
        else {
            mIsRidDemo = false;
        }

        _8 = 1;        
    }

}
