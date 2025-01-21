#ifndef PLAYER_H
#define PLAYER_H

#include <AquaEngine.h>

#include <memory>

class Player
{
public:
    void Init(AquaEngine::Command& command);
    void Render(AquaEngine::Command& command);
    void SetMatrixSegments(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment>& segment,
        int index
    ) const;  // this is player index
    void SetTextureSegments(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment>& segment,
        int index
    ) const;  // this is player index
    void SetMaterialSegments(
        const std::shared_ptr<AquaEngine::DescriptorHeapSegment>& segment,
        int index
    ) const;  // this is player index

private:
    std::unique_ptr<AquaEngine::FBXModel> m_arm;
    std::unique_ptr<AquaEngine::FBXModel> m_body;
    std::unique_ptr<AquaEngine::FBXModel> m_buster;
    std::unique_ptr<AquaEngine::FBXModel> m_eye;
    std::unique_ptr<AquaEngine::FBXModel> m_gun;
    std::unique_ptr<AquaEngine::FBXModel> m_head;
    std::unique_ptr<AquaEngine::FBXModel> m_ring;
    std::unique_ptr<AquaEngine::FBXModel> m_thuraster;

    void ImportModel(AquaEngine::Command& command);
};

#endif  // PLAYER_H
