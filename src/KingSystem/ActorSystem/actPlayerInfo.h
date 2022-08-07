#pragma once

#include <heap/seadDisposer.h>
#include <math/seadMatrix.h>
#include "KingSystem/ActorSystem/actActorConstDataAccess.h"
#include "KingSystem/ActorSystem/actActorLinkConstDataAccess.h"

namespace ksys::act {

class PlayerBase;

// TODO
class PlayerInfo {
    SEAD_SINGLETON_DISPOSER(PlayerInfo)
    PlayerInfo();
    virtual ~PlayerInfo();

public:
    struct S0 {}
    PlayerBase* getPlayer() const;
    s32 getLife() const;
    u32 getMaxHearts() const;
    f32 getMaxStamina() const;
    sead::Matrix34f* getPlayerPosFromPlayerInfo();
private:
    

};

}  // namespace ksys::act
