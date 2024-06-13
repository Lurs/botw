#include "KingSystem/GameData/gdtSaveMgr.h"
//#include "KingSystem/Utils/HeapUtil.h"
namespace ksys {

    SEAD_SINGLETON_DISPOSER_IMPL(SaveMgr)

    void SaveMgr::init(const InitArg& arg) {
        this->mDoNotSave= arg.is_demo;
        //this->heap = util::DualHeap::create(arg._8, sead::SafeString("SaveMgr"), arg->gameDataHeap, arg._60, sizeof(void*), sead::Heap::cHeapDirection_Forward, false);
    }

}
