#ifndef RESULT_H
#define RESULT_H

#include <AquaEngine.h>

class Result
{
public:
    void Init(AquaEngine::Command& command);
    void Render(AquaEngine::Command& command, bool win);

private:
    std::unique_ptr<AquaEngine::RectangleTexture> m_resultWinText;
    std::unique_ptr<AquaEngine::RectangleTexture> m_resultLoseText;
    std::unique_ptr<AquaEngine::Rectangle> m_background;

    AquaEngine::RootSignature m_textRootSignature;
    AquaEngine::PipelineState m_textPipelineState;

    AquaEngine::RootSignature m_backgroundRootSignature;
    AquaEngine::PipelineState m_backgroundPipelineState;
};

#endif  // RESULT_H
