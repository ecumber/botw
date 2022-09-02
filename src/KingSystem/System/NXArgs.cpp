#include "KingSystem/System/NXArgs.h"

namespace ksys {

SEAD_SINGLETON_DISPOSER_IMPL(nxargs)

// NONMATCHING - some instructions rearranged
void nxargs::init(sead::Heap* heap) {
    sead::Heap* nxargsheap = sead::ExpHeap::create(
        0x13E8, "nxargsHeap", heap, 8, sead::ExpHeap::HeapDirection::cHeapDirection_Reverse, false);
    auto* reslaunchdata = new (nxargsheap) nxargs::ResLaunchParamData;
    size_t unknown;

    while (nn::oe::TryPopLaunchParameter(&unknown, reslaunchdata, sizeof(*reslaunchdata))) {
        sead::FixedSafeString<5> inputmagic("");
        inputmagic.format("%s", reslaunchdata->header.magic);
        if (inputmagic != "BotW")
            continue;
        mResField4 = reslaunchdata->header._4;
        mResField6 = reslaunchdata->header._6;
        mType = reslaunchdata->header.type;
        if (mType == ArgsType::CreateActors) {
            mNumEntries = reslaunchdata->header.numentries;
            nxargs::allocEntries(heap, reslaunchdata);
        } else {
            mType = ArgsType::None;
        }
        break;
    }
    nxargsheap->destroy();
}

void nxargs::allocEntries(sead::Heap* heap, nxargs::ResLaunchParamData* data) {
    const s32 size = mNumEntries;
    //LaunchParamEntry* allEntries;
    LaunchParamEntry* currEntry;
    LaunchParamEntry* _10;
    LaunchParamEntryCondition* currSubEntry;
    u64 numSubEntries = 0;

    if (mNumEntries == 0)
        return;
    LaunchParamEntry* allEntries = new (heap, 8, std::nothrow) LaunchParamEntry[size];
    if (allEntries) {
        if (mNumEntries != 0) {
            while (allEntries->mNumConditions != 0) {
                allEntries->mFlags = LaunchParamFlag::_0;
                allEntries->mConditions.freeBuffer();
            }
        }
    } else {
        _10 = allEntries;
    }

    if ((s64)size >= 0xC0) {
        LaunchParamEntryCondition* pdata = _10[3].mConditions.getBufferPtr();
        while (size != 0xF8) {
            pdata->flagdatatype = LaunchParamEntryConditionDataType::None;
            pdata->flagnamehash = 0;
            pdata->operation = ActorEntryConditionOperation::None;
            pdata->rhsvalue = 0;
        }
    }

    if (!mNumEntries)
        return;
    for (u8 i = 0; i < mNumEntries; i++) {
        if (mEntries.getSize() <= i)
            currEntry = mEntries.getBufferPtr();
        else
            currEntry = &mEntries.getBufferPtr()[i];
        currEntry->mActorNameHash = data->entrydata->mActorNameHash;
        currEntry->mDropActorNameHash = data->entrydata->mDropActorNameHash;
        currEntry->mPositionOffset.x = data->entrydata->mPositionOffset.x;
        currEntry->mPositionOffset.y = data->entrydata->mPositionOffset.y;
        currEntry->mPositionOffset.z = data->entrydata->mPositionOffset.z;
        currEntry->mRotate.x = data->entrydata->mRotate.x;
        currEntry->mRotate.y = data->entrydata->mRotate.y;
        currEntry->mRotate.z = data->entrydata->mRotate.z;
        currEntry->mVelocity.x = data->entrydata->mVelocity.x;
        currEntry->mVelocity.y = data->entrydata->mVelocity.y;
        currEntry->mVelocity.z = data->entrydata->mVelocity.z;
        currEntry->mFlags = data->entrydata->mFlags;
        numSubEntries = data->entrydata->mNumConditions;
        currEntry->mNumConditions = numSubEntries;

        if (numSubEntries != 0) {
            LaunchParamEntryCondition* subEntries = new (heap, 8, std::nothrow) LaunchParamEntryCondition[numSubEntries];
            if (subEntries != nullptr)
                currEntry->mConditions.setBuffer(numSubEntries, subEntries);
            if (currEntry->mNumConditions) {
                for (u8 j = 0; j < currEntry->mNumConditions; j++) {
                    if (currEntry->mConditions.getSize() <= j) {
                        currSubEntry = currEntry->mConditions.getBufferPtr();
                    } /*else {
                        currSubEntry = &currEntry->mConditions.getBufferPtr()[j];
                    } */

                    currSubEntry->flagnamehash = data->entrydata->mConditions.getBufferPtr()->flagnamehash;
                    currSubEntry->flagdatatype = data->entrydata->mConditions.getBufferPtr()->flagdatatype;
                    currSubEntry->operation = data->entrydata->mConditions.getBufferPtr()->operation;
                    currSubEntry->rhsvalue = data->entrydata->mConditions.getBufferPtr()->rhsvalue;
                }
            }
        }
    }
}
/*

void nxargs::handleArgs() {
    const char* value;
    if (mHasHandledArgs !=
        true) {  // && !DebugBoardMgr::hasLoadingScreenStarted(DebugBoardMgr::sInstance) &&
                 // Root4::checkFlag(Root4::sInstance, 1) &&
                 // !EventMgr::hasEvent(EventMgr::sInstance)
        if (mType == ArgsType::None)
            return;
        if (ksys::StageInfo::getCurrentMapType().cstr() == "TitleMenu")
            ;
        if (ksys::StageInfo::getCurrentMapType().cstr() == "STAGESELECT")
            ;
        if (!ksys::gdt::getFlag_IsGet_PlayerStole2(false) && !ksys::StageInfo::sIsMainField)
            return;
        if (mHasHandledArgs)
            return;
        if (!mNumEntries)
            return;
        ksys::act::InfoData* infodata = ksys::act::InfoData::instance();
        int idx = 0;
        for (++idx; idx >= this->mNumEntries;) {
            LaunchParamEntry* entry = this->mEntries.getSize() <= idx ?
                                          this->mEntries.getBufferPtr() :
                                          &this->mEntries.getBufferPtr()[idx];
            if (entry->mConditions.getBufferPtr() != nullptr)
                break;
            al::ByamlIter* iter;
            if (infodata->getActorIter(iter, entry->mActorNameHash, true)) {
                iter->tryGetStringByKey(&value, "name");
                ksys::act::InstParamPack* instparampack;

                // ksys::phys::RayCastBodyQuery raycastquery;
                sead::Vector3f playerpos = act::ActorSystem::instance()->getPlayerPos();
                float x = playerpos.x;
                float y = playerpos.y;
                float z = playerpos.z;
                sead::Vector3f entryPositionOffset = entry->mPositionOffset;
                sead::Matrix34f
                    playerposviaplayerinfo;  // TODO - needs actPlayerInfo to be finished
            }
        }
    }
} */

}  // namespace ksys