#ifndef GUIDE_H
#define GUIDE_H

#include <AquaEngine.h>

// UI 1
class Guide
{
public:
    void Init(
        AquaEngine::Command &command,
        AquaEngine::DescriptorHeapSegmentManager &manager
    );
    void Render(AquaEngine::Command &command);

private:
    std::unique_ptr<AquaEngine::RectangleTexture> m_rectangle;
};

#endif  // GUIDE_H
