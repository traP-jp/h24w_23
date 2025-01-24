#ifndef POINTER_H
#define POINTER_H

#include <AquaEngine.h>

// UI3
class Pointer
{
public:
    void Init(AquaEngine::Command &command);
    void Render(AquaEngine::Command &command);
    void SetPosition(float x, float y);

private:
    struct Matrix
    {
        DirectX::XMMATRIX matrix;
    };

    std::unique_ptr<AquaEngine::RectangleTexture> m_rectangle;

    AquaEngine::RootSignature m_rootSignature;
    AquaEngine::PipelineState m_pipelineState;

    AquaEngine::GPUBuffer<Matrix> m_matrixBuffer;
    AquaEngine::ConstantBufferView m_matrixCBV;
};

#endif  // POINTER_H
