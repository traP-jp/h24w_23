#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H

#include <AquaEngine.h>
#include <Effekseer.h>
#include <EffekseerRendererDX12.h>

class EffectManager
{
public:
    void Init(AquaEngine::Command &command);
    void SetCamera(const std::shared_ptr<AquaEngine::Camera> &camera);
    void Render(
        AquaEngine::Command &command,
        const std::shared_ptr<AquaEngine::Camera> &camera
    );

    Effekseer::ManagerRef GetManager() const
    {
        return m_manager;
    }

    void UpdateCamera(const std::shared_ptr<AquaEngine::Camera> &camera);

private:
    EffekseerRenderer::RendererRef m_renderer = nullptr;
    Effekseer::ManagerRef m_manager = nullptr;
    Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> m_memoryPool
        = nullptr;
    Effekseer::RefPtr<EffekseerRenderer::CommandList> m_commandList = nullptr;
};

#endif  // EFFECTMANAGER_H
