#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <AquaEngine.h>

#include "UIComponent.h"

class UIManager
{
public:
    void Init(AquaEngine::Command& command);
    void Render(AquaEngine::Command& command);

    void SetPointerPosition(float x, float y)
    {
        m_pointer->SetPosition(x, y);
    }

private:
    AquaEngine::RootSignature m_rootSignature;
    AquaEngine::PipelineState m_pipelineState;

    std::unique_ptr<UIComponent> m_guide;
    std::unique_ptr<UIComponent> m_pointer;
};

#endif  // UIMANAGER_H
