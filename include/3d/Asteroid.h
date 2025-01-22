#ifndef ASTEROID_H
#define ASTEROID_H

#include <AquaEngine.h>

class Asteroid
{
public:
    void Init(AquaEngine::Command &command, bool is1);

    void Render(AquaEngine::Command &command);

    void SetMatrixSegments(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
        int index
    ) const;

    void SetTextureSegments(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
        int index
    ) const;

    void SetMaterialSegments(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment> &segment,
        int index
    ) const;

    void Move(float dx, float dy, float dz) const
    {
        m_model->Move(dx, dy, dz);
    }

    void Scale(float x, float y, float z) const
    {
        m_model->Scale(x, y, z);
    }

private:
    std::unique_ptr<AquaEngine::FBXModel> m_model;

    void ImportModel(AquaEngine::Command &command, bool is1);
};


#endif //ASTEROID_H
