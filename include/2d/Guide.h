#ifndef GUIDE_H
#define GUIDE_H

#include <AquaEngine.h>

// UI 1
class Guide
{
public:
    void Init(AquaEngine::Command &command);
    void Render(AquaEngine::Command &command);

private:
    std::unique_ptr<AquaEngine::RectangleTexture> m_rectangle;

    AquaEngine::RootSignature m_rootSignature;
    AquaEngine::PipelineState m_pipelineState;
};

#endif  // GUIDE_H
