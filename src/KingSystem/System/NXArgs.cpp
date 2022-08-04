#include "KingSystem/System/NXArgs.h"

namespace ksys {

SEAD_SINGLETON_DISPOSER_IMPL(nxargs)

// WIP
void nxargs::init(sead::Heap* heap) {
    sead::SafeString heapname = "nxargsHeap";
    sead::Heap* nxargsheap = nullptr;

    nxargsheap = sead::ExpHeap::create(0x13E8, heapname, heap, (u32)8,
                                       sead::ExpHeap::HeapDirection::cHeapDirection_Reverse, false);
    nxargs::ResLaunchParamData* reslaunchdata = new (nxargsheap) nxargs::ResLaunchParamData;

    if (nn::oe::TryPopLaunchParameter(reslaunchdata, 0x1000))
        return;
    for (int len; len++; len < 1000) {
        sead::FixedSafeString<5> inputmagic;
        sead::SafeString expectedmagic = "\00";

        inputmagic = expectedmagic;

        inputmagic.format("%s", reslaunchdata->header.magic);
        expectedmagic = "BotW";
        bool isMagicMatch = (inputmagic.cstr() == expectedmagic.cstr());
        if (isMagicMatch)
            break;
        // s64 index = 0;
        bool poplaunchparamresult = nn::oe::TryPopLaunchParameter(reslaunchdata, 0x1000);
        if (poplaunchparamresult)
            break;
    }
    this->mResField4 = reslaunchdata->header._4;
    this->mResField6 = reslaunchdata->header._6;
    argstype type = reslaunchdata->header.type;
    this->mType = type;
    if (type == argstype::TypeCreateActors) {
        this->mNumEntries = reslaunchdata->header.numentries;
        nxargs::allocEntries(heap, reslaunchdata);
    } else {
        this->mType = argstype::TypeNone;
    }
    heap->destroy();
}

void nxargs::allocEntries(sead::Heap* heap, nxargs::ResLaunchParamData* data) {
    u8 numEntries = this->mNumEntries;
    if (this->mNumEntries) {
        u64 entriesshifted = numEntries << 6;
        nxargs::LaunchParamEntry* launchparams = new (heap, std::nothrow) nxargs::LaunchParamEntry;
        if (launchparams) {
            if (((((entriesshifted - 0x40) >> 6) + 1))) {
                ;
            }
        }
    }
}

}  // namespace ksys