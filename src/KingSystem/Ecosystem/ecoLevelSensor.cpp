#include "KingSystem/Ecosystem/ecoLevelSensor.h"
#include "KingSystem/Ecosystem/ecoSystem.h"
#include "KingSystem/GameData/gdtManager.h"
#include "KingSystem/Resource/resLoadRequest.h"
#include "KingSystem/Utils/Byaml/Byaml.h"

namespace ksys::eco {

LevelSensor::LevelSensor() = default;

LevelSensor::~LevelSensor() {
    mResHandle.requestUnload2();
    if (mRootIter)
        delete mRootIter;
}

void LevelSensor::init(sead::Heap* heap) {
    res::LoadRequest req;
    req.mRequester = "LevelSensor";
    mResHandle.load("Ecosystem/LevelSensor.byml", &req);
    auto* res = sead::DynamicCast<sead::DirectResource>(mResHandle.getResource());
    mRootIter = new (heap) al::ByamlIter(res->getRawData());
}

void LevelSensor::calculatePoints() {
    if (mDefaultPoints >= 0) {
        mPoints = mDefaultPoints;
    } else {
        al::ByamlIter flag;
        if (!mRootIter->tryGetIterByKey(&flag, "flag")) {
            return;
        }
        float point_sum = 0;
        for (int index = 0; index < flag.getSize(); index++) {
            al::ByamlIter iter_enemy;
            if (!flag.tryGetIterByIndex(&iter_enemy, index)) {
                return;
            }
            const char* name;
            if (!iter_enemy.tryGetStringByKey(&name, "name")) {
                return;
            }
            s32 kill_count = 0;
            if (!gdt::Manager::instance()->getParam().get().getS32(&kill_count, name)) {
                bool unique_kill = false;
                if (gdt::Manager::instance()->getParam().get().getBool(&unique_kill, name)) {
                    if (unique_kill) {
                        kill_count = 1;
                    }
                }
            }
            if (kill_count > 0) {
                f32 point;
                if (!iter_enemy.tryGetFloatByKey(&point, "point")) {
                    return;
                }
                point_sum += point * kill_count;
            }
        }
        mPoints = point_sum;
    }
    al::ByamlIter setting_iter;
    if (mRootIter->tryGetIterByKey(&setting_iter, "setting")) {
        f32 Level2WeaponPower;
        f32 Level2EnemyPower;
        if (setting_iter.tryGetFloatByKey(&Level2WeaponPower, "Level2WeaponPower") &&
            setting_iter.tryGetFloatByKey(&Level2EnemyPower, "Level2EnemyPower")) {
            mWeaponPoints = mPoints * Level2WeaponPower;
            mEnemyPoints = mPoints * Level2EnemyPower;
        }
    }
}

// TODO: this is a nightmare. needs to be fixed when mostly done
bool LevelSensor::scaleWeapon(const sead::SafeString& weapon, WeaponModifier min_modifier,
                              const char** scaled_weapon, WeaponModifier* scaled_modifier,
                              act::Actor* actor) const {
    // ksys::world::Manager* mgr;
    // ksys::eco::Ecosystem* ecosys = ksys::eco::Ecosystem::instance();
    ksys::eco::EcoMapInfo* mapinfo;
    al::ByamlIter weaponIter;
    al::ByamlIter actorIter;
    al::ByamlIter indexIter;
    al::ByamlIter paramIter;
    al::ByamlIter unknownIter;
    float floatValue;
    float floatValue2;
    int plus;
    u32 actorindex;
    u32 _24;
    u32 actorIndexPlusOne;
    u8 ueight;
    bool isNotRankUp = 0, _1, _23;
    const char* _;
    bool _15;

    if (actor == nullptr) {
        ksys::map::Object* placementobj = actor->getMapObject();
        if (placementobj) {
            sead::Vector3f translate = placementobj->getTranslate();
            if (ksys::world::Manager::instance()->isAocField() ||
                ksys::eco::Ecosystem::instance()->getMapArea(*mapinfo, translate.x, translate.z) ==
                    0x1C)
                return false;
        }
        // else {
        //     if (ksys::world::Manager::instance()->isAocField() ||
        //         ksys::eco::Ecosystem::instance()->getMapArea(*mapinfo, translate.x, translate.z)
        //         ==
        //             0x1C)
        //         return false;
        // }
    }

    if (!mRootIter->tryGetIterByKey(&weaponIter, "weapon")) {
        _23 = false;
    }

    if (weaponIter.getSize() < 1) {
        // somebool = false;
    } else {
        scaled_modifier = 0;
        u32 index = 0;
        if (!weaponIter.tryGetIterByIndex(&indexIter, index)) {
            return false;
        }
        if (indexIter.tryGetIterByKey(&actorIter, "actors")) {
            if (actorIter.getSize() < 2) {
                if (_15) {
                    _23 = true;
                    return _1 && _23;
                }
            }
            actorindex = 0;

            for (u32 index = 0; index < indexIter.getSize(); index++) {
                const char* name;
                if (!actorIter.tryGetIterByIndex(&paramIter, index) ||
                    !paramIter.tryGetStringByKey(&name, "name")) {
                    break;
                }
                if (!actorIter.tryGetFloatByKey(&floatValue, "value") ||
                    !paramIter.tryGetIntByKey(&plus, "plus")) {
                    _1 = false;
                    if (_15) {
                        _23 = true;
                        return _1 && _23;
                    }
                    // break;
                }

                if (min_modifier == WeaponModifier::None) {
                    if (min_modifier == WeaponModifier::Blue) {
                        if (plus) {
                            break;
                        }
                    } else {
                        if (min_modifier != WeaponModifier::Yellow || plus != 1) {
                        }
                    }
                } else if (plus != 0xFFFFFFFF)
                    break;
                if (floatValue >= this->mWeaponPoints)
                    break;
                // sead::SafeString _28 =
                if (weapon.cstr() != name) {
                }
                if (!paramIter.tryGetBoolByKey(&isNotRankUp, "not_rank_up")) {
                    break;
                }
                floatValue2 = 0.0;
                u32 actorIndexPlusOne = actorindex + 1;
                if ((actorindex + 1) >= actorIter.getSize()) {
                    _15 = 1;
                }
            }
            _24 = actorIndexPlusOne;
            if (this->mWeaponPoints > floatValue2) {
            }
            _24 = actorIndexPlusOne;
            char* value;
            char* actorType;
            int plusYellow;
            if ((_24) != 0 || !actorIter.tryGetIterByIndex(&unknownIter, _24) ||
                !unknownIter.tryGetStringByKey(&_, "name") ||
                !paramIter.tryGetStringByKey((const char**)&value, "series") ||
                !paramIter.tryGetStringByKey((const char**)&actorType, "actorType") ||
                !unknownIter.tryGetIntByKey(&plusYellow, "plus")) {
                bool _1 = false;
            }
            WeaponModifier isYellow, newModifier;
            if (plusYellow == 1)
                isYellow = WeaponModifier::Yellow;
            else
                isYellow = WeaponModifier::None;
            if (plusYellow)
                newModifier = isYellow;
            else
                newModifier = WeaponModifier::Blue;
            _1 = true;
            *scaled_modifier = newModifier;

            _15 = true;

            if (_15) {
                _23 = true;
                return _1 && _23;
            }
        }
    }
    // return _1;
}

}  // namespace ksys::eco
