#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <AquaEngine.h>

#include "Guide.h"

class UIManager
{
public:
    void Init(AquaEngine::Command& command);
    void Render(AquaEngine::Command& command);

private:
    AquaEngine::RootSignature m_rootSignature;
    AquaEngine::PipelineState m_pipelineState;

    Guide m_guide;
};

#endif  // UIMANAGER_H
