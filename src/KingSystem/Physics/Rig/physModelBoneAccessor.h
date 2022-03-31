#pragma once

#include <container/seadBuffer.h>
#include <gsys/gsysModelAccessKey.h>
#include <math/seadVector.h>
#include "KingSystem/Physics/Rig/physBoneAccessor.h"
#include "KingSystem/Utils/BitSet.h"

namespace gsys {
class Model;
class ModelUnit;
}  // namespace gsys

namespace ksys::phys {

namespace detail {
class ModelSkeleton;
}

class ModelBoneAccessor : public BoneAccessor {
public:
    class ModelBoneFilter {
    public:
        virtual void filter(util::BitSet<1024>* bones_to_keep,
                            const gsys::ModelUnit& model_unit) = 0;
    };

    ModelBoneAccessor();
    ~ModelBoneAccessor() override;

    bool init(const hkaSkeleton* skeleton, const gsys::Model* model, sead::Heap* heap);

    bool init(const gsys::Model* model, int model_unit_index, sead::Heap* heap,
              ModelBoneFilter* bone_filter);

    void finalize() override;

protected:
    struct BoneAccessKey {
        gsys::BoneAccessKeyEx key;
        bool _38;
        bool _39;
    };

    const gsys::Model* mModel{};
    sead::Buffer<BoneAccessKey> mBoneAccessKeys;
    sead::Vector3f _38 = sead::Vector3f::zero;
    detail::ModelSkeleton* mModelSkeleton{};
};

}  // namespace ksys::phys
