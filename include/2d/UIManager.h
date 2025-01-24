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

    void SetTargetPosition(float x, float y)
    {
        m_target->SetPosition(x, y);
    }

    void SetTargetRotation(float angle)
    {
        m_target->Rotate(angle);
    }

    void SetTargetRotateAndSinScale(float angle)
    {
        m_target->RotateAndSinScale(angle);
    }

    void SetTargetColor(float r, float g, float b)
    {
        m_target->SetColor(r, g, b);
    }

private:
    AquaEngine::RootSignature m_rootSignature;
    AquaEngine::PipelineState m_pipelineState;

    std::unique_ptr<UIComponent> m_guide;
    std::unique_ptr<UIComponent> m_pointer;
    std::unique_ptr<UIComponent> m_target;
};

#endif  // UIMANAGER_H
