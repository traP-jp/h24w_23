cmake_minimum_required(VERSION 3.29)
project(h24w_23)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

macro(create_executable files)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

    set(srcs ${ARGV})
    foreach (src ${srcs})
        if (src MATCHES ".*\\.(cpp|c|cc|cxx|h|hpp|rc)$")
            list(APPEND src_list ${src})
        endif ()
        if (src MATCHES ".*\\.(hlsl|hlsli)$")
            file(COPY ${src} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)
        endif ()
        if (LOCAL_BUILD)
            if (src MATCHES ".*effects/Textures.*\\.png$")
                file(COPY ${src} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/effects/Textures)
            elseif (src MATCHES ".*effects/Meshes.*$")
                file(COPY ${src} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/effects/Meshes)
            elseif (src MATCHES ".*effects/Materials.*$")
                file(COPY ${src} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/effects/Materials)
            elseif (src MATCHES ".*effects.*$")
                file(COPY ${src} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/effects)
            elseif (src MATCHES ".*\\.(fbx|png)$")
                file(COPY ${src} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/models)
            endif ()
            if (src MATCHES ".*\\.hdr$")
                file(COPY ${src} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures)
            endif ()
            if (src MATCHES ".*\\.efkefc$")
                file(COPY ${src} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/effects)
            endif ()
            if (src MATCHES ".*\\.wav$")
                file(COPY ${src} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/music)
            endif ()
        endif ()
    endforeach ()

    find_package(directxtex CONFIG REQUIRED)

    add_executable(h24w_23 WIN32 ${src_list})
    target_include_directories(h24w_23 PRIVATE include)
    target_include_directories(h24w_23 PRIVATE $ENV{aquaengine_dir}/include)
    target_include_directories(h24w_23 PRIVATE $ENV{FBXSDK_DIR}/include)
    target_include_directories(h24w_23 PRIVATE $ENV{effekseer_dir}/include/Effekseer)
    target_include_directories(h24w_23 PRIVATE $ENV{effekseer_dir}/include/EffekseerRendererDX12)
    target_link_libraries(h24w_23 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/aqua-engine-0.1.2/lib/aqua-engine.lib)
    target_link_libraries(h24w_23 PUBLIC d2d1.lib)
    target_link_libraries(h24w_23 PUBLIC dwrite.lib)
    target_link_libraries(h24w_23 PUBLIC d3d12.lib)
    target_link_libraries(h24w_23 PUBLIC dxgi.lib)
    target_link_libraries(h24w_23 PUBLIC d3dcompiler.lib)
    target_link_libraries(h24w_23 PUBLIC d3d11.lib)
    target_link_libraries(h24w_23 PUBLIC Microsoft::DirectXTex)
    target_link_libraries(h24w_23 PUBLIC ws2_32.lib)
    target_link_libraries(h24w_23 PUBLIC MsWSock.lib)
    target_link_libraries(h24w_23 PUBLIC AdvAPI32.lib)
    target_link_libraries(h24w_23 PUBLIC winmm.lib)
    target_link_libraries(h24w_23 PUBLIC xaudio2.lib)
    target_link_libraries(h24w_23 PUBLIC windowscodecs.lib)
    target_link_libraries(h24w_23 PUBLIC $ENV{FBXSDK_DIR}/lib/x64/debug/libfbxsdk-md.lib)
    target_link_libraries(h24w_23 PUBLIC $ENV{FBXSDK_DIR}/lib/x64/debug/libxml2-md.lib)
    target_link_libraries(h24w_23 PUBLIC $ENV{FBXSDK_DIR}/lib/x64/debug/zlib-md.lib)
    target_link_libraries(h24w_23 PUBLIC $ENV{effekseer_dir}/lib/Effekseerd.lib)
    target_link_libraries(h24w_23 PUBLIC $ENV{effekseer_dir}/lib/EffekseerRendererDX12d.lib)
    target_link_libraries(h24w_23 PUBLIC $ENV{effekseer_dir}/lib/LLGId.lib)

    set_property(TARGET h24w_23 PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")

    #target_compile_definitions(h24w_23 PRIVATE DEBUG)  # for nsight, d3d11on12 is not supported
    #target_compile_definitions(h24w_23 PRIVATE NSIGHT)
endmacro()

set(src_files
        src/main.cpp
        src/view/MainWindow.cpp
        src/view/IPDialog.cpp
        src/3d/Engine.cpp
        src/3d/GameView.cpp
        src/3d/Player.cpp
        src/3d/Camera.cpp
        src/3d/Asteroid.cpp
        src/3d/Bullet.cpp
        src/3d/EffectManager.cpp
        src/3d/SideUI.cpp
        src/2d/D2DEngine.cpp
        src/2d/Title.cpp
        src/2d/UIManager.cpp
        src/2d/UIComponent.cpp
        src/2d/GameInfo.cpp
        src/2d/TitleImage.cpp
        src/2d/Result.cpp
        src/network/Network.cppj
        src/music/BGM.cpp
)

set(include_files
        include/view/BaseWindow.h
        include/view/MainWindow.h
        include/view/IPDialog.h
        include/3d/Engine.h
        include/3d/GameView.h
        include/3d/Player.h
        include/3d/Camera.h
        include/3d/Asteroid.h
        include/3d/Bullet.h
        include/3d/EffectManager.h
        include/3d/SideUI.h
        include/2d/D2DEngine.h
        include/2d/Title.h
        include/2d/UIManager.h
        include/2d/UIComponent.h
        include/2d/GameInfo.h
        include/2d/TitleImage.h
        include/2d/Result.h
        include/network/Network.h
        include/music/BGM.h
)

set(resources
        include/view/resource.h
        include/view/IPDialog.rc
        resources/models/player_model/arm.fbx
        resources/models/player_model/body.fbx
        resources/models/player_model/buster.fbx
        resources/models/player_model/eye.fbx
        resources/models/player_model/gun.fbx
        resources/models/player_model/head.fbx
        resources/models/player_model/ring.fbx
        resources/models/player_model/thuraster.fbx
        #resources/models/player_model/thuraster_noanim.fbx
        resources/models/player_texture/arm_diff_tex.png
        resources/models/player_texture/body_diff_tex.png
        resources/models/player_texture/buster_diff_tex.png
        resources/models/player_texture/gun_diff_tex.png
        resources/models/player_texture/head_diff_tex.png
        resources/models/player_texture/ring_diff_tex.png
        resources/models/player_texture/thuraster_diff_tex.png
        resources/models/player_texture/player_tex.png
        resources/models/player_texture/hanten/head_diff_tex_lr.png
        resources/models/player_texture/hanten/head_diff_tex_ud.png
        resources/models/player_texture/hanten/head_diff_tex_lrud.png
        resources/models/player_texture/hanten/buster_diff_tex_lr.png
        resources/models/player_texture/hanten/buster_diff_tex_ud.png
        resources/models/player_texture/hanten/buster_diff_tex_lrud.png
        resources/models/player_texture/hanten/head_diff_tex2.png
        resources/models/asteroid_model/asteroid_patern1.fbx
        resources/models/asteroid_model/asteroid_patern2.fbx
        resources/models/asteroid_tex/asteroid_patern1_diff_tex.png
        resources/models/cube.fbx
        resources/models/bullet_model/bullet.fbx
        resources/models/bullet_model/bullet_tex.png
        resources/models/bullet_model/bullet_alpha.png
        resources/textures/space.hdr
        resources/textures/sample1.hdr
        resources/effects/tyakudan.efkefc
        resources/effects/ef_common_flashlight01_t.png
        resources/effects/Star.png
        resources/effects/tex_eff_light02.png
        resources/effects/booster.efkefc
        resources/effects/alpha.png
        resources/effects/blackhall.efkmat
        resources/effects/blackhall.efkmodel
        resources/effects/blackhall.fbx
        resources/effects/blackhall1.png
        resources/effects/blackhall2.png
        resources/effects/blackhall3.png
        resources/effects/blackhall4.png
        resources/effects/blackhall5.png
        resources/effects/booster.efkefc
        resources/effects/disk.efkmat
        resources/effects/ef_common_flashlight01_t.png
        resources/effects/hall.efkmat
        resources/effects/hall.efkmodel
        resources/effects/hall.fbx
        resources/effects/hassya.efkefc
        resources/effects/ms_lightning01.efkmodel
        resources/effects/ms_lightning01.fbx
        resources/effects/ef_lightning01.efkefc
        resources/effects/Materials/mt_dissolve01.efkmat
        resources/effects/Materials/mt_dissolve02.efkmat
        resources/effects/Materials/mt_ice01.efkmat
        resources/effects/Materials/mt_lightning01.efkmat
        resources/effects/Materials/mt_lightning02.efkmat
        resources/effects/Materials/mt_lightning03.efkmat
        resources/effects/Materials/mt_slash01.efkmat
        resources/effects/Materials/mt_toon_shockwave01.efkmat
        resources/effects/Materials/mt_wind02.efkmat
        resources/effects/Materials/mt_wind03_groundAura.efkmat
        resources/effects/Materials/mt_wind03_groundCrack.efkmat
        resources/effects/Materials/mt_wind03_slash01.efkmat
        resources/effects/Materials/mt_wind03_tornado01.efkmat
        resources/effects/Materials/mt_wind03_tornado01_test.efkmat
        resources/effects/Meshes/ms_groundAura01.efkmodel
        resources/effects/Meshes/ms_groundAura01.fbx
        resources/effects/Meshes/ms_groundAura02.efkmodel
        resources/effects/Meshes/ms_groundAura02.fbx
        resources/effects/Meshes/ms_ice01.efkmodel
        resources/effects/Meshes/ms_ice01.fbx
        resources/effects/Meshes/ms_ice02.efkmodel
        resources/effects/Meshes/ms_ice02.fbx
        resources/effects/Meshes/ms_ice03.efkmodel
        resources/effects/Meshes/ms_ice03.fbx
        resources/effects/Meshes/ms_lightning01.efkmodel
        resources/effects/Meshes/ms_lightning01.fbx
        resources/effects/Meshes/ms_lightning02.efkmodel
        resources/effects/Meshes/ms_lightning02.fbx
        resources/effects/Meshes/ms_lightning03_01.efkmodel
        resources/effects/Meshes/ms_lightning03_01.fbx
        resources/effects/Meshes/ms_lightning03_02.efkmodel
        resources/effects/Meshes/ms_lightning03_02.fbx
        resources/effects/Meshes/ms_lightning03_03.efkmodel
        resources/effects/Meshes/ms_lightning03_03.fbx
        resources/effects/Meshes/ms_plane01.efkmodel
        resources/effects/Meshes/ms_plane01.fbx
        resources/effects/Meshes/ms_rock01.efkmodel
        resources/effects/Meshes/ms_rock01.fbx
        resources/effects/Meshes/ms_slash01.efkmodel
        resources/effects/Meshes/ms_slash01.fbx
        resources/effects/Meshes/ms_slash02.efkmodel
        resources/effects/Meshes/ms_slash02.fbx
        resources/effects/Meshes/ms_tornado01.efkmodel
        resources/effects/Meshes/ms_tornado01.fbx
        resources/effects/Meshes/ms_tornado02.efkmodel
        resources/effects/Meshes/ms_tornado02.fbx
        resources/effects/Meshes/ms_tornado03.efkmodel
        resources/effects/Meshes/ms_tornado03.fbx
        resources/effects/Meshes/ms_tornado04.efkmodel
        resources/effects/Meshes/ms_tornado04.fbx
        resources/effects/Meshes/ms_wind01.efkmodel
        resources/effects/Meshes/ms_wind01.fbx
        resources/effects/Meshes/ms_wind02.efkmodel
        resources/effects/Meshes/ms_wind02.fbx
        resources/effects/sphia.efkmodel
        resources/effects/sphia.fbx
        resources/effects/Textures/tx_aura01_256.png
        resources/effects/Textures/tx_aura02_256.png
        resources/effects/Textures/tx_aura03_256.png
        resources/effects/Textures/tx_aura04_256.png
        resources/effects/Textures/tx_aura05_256.png
        resources/effects/Textures/tx_aura06_256.png
        resources/effects/Textures/tx_aura07_256.png
        resources/effects/Textures/tx_aura08_256.png
        resources/effects/Textures/tx_aura09_256.png
        resources/effects/Textures/tx_aura10_256.png
        resources/effects/Textures/tx_aura11_256.png
        resources/effects/Textures/tx_aura12_256.png
        resources/effects/Textures/tx_aura13_256.png
        resources/effects/Textures/tx_aura14_256.png
        resources/effects/Textures/tx_aura15_256.png
        resources/effects/Textures/tx_aura16_256.png
        resources/effects/Textures/tx_crack01_256.png
        resources/effects/Textures/tx_crack02_256.png
        resources/effects/Textures/tx_crack03_256.png
        resources/effects/Textures/tx_debris01_128.png
        resources/effects/Textures/tx_directional_aura01_256.png
        resources/effects/Textures/tx_directional_aura02_256.png
        resources/effects/Textures/tx_fire_flipbook01_1024.png
        resources/effects/Textures/tx_foam01.png
        resources/effects/Textures/tx_glow01_128.png
        resources/effects/Textures/tx_glow02_128.png
        resources/effects/Textures/tx_glow03_128.png
        resources/effects/Textures/tx_gradient01_256.png
        resources/effects/Textures/tx_hit01_256.png
        resources/effects/Textures/tx_ice01_256.png
        resources/effects/Textures/tx_lightning01_256.png
        resources/effects/Textures/tx_lightning04_128x256.png
        resources/effects/Textures/tx_magiccircle01_512.png
        resources/effects/Textures/tx_noise01_256.png
        resources/effects/Textures/tx_noise03_256.png
        resources/effects/Textures/tx_noise04_256.png
        resources/effects/Textures/tx_noise05_256.png
        resources/effects/Textures/tx_noise06_256.png
        resources/effects/Textures/tx_noise07_256.png
        resources/effects/Textures/tx_noise08_256.png
        resources/effects/Textures/tx_normal01_256.png
        resources/effects/Textures/tx_normal02_256.png
        resources/effects/Textures/tx_normal03_256.png
        resources/effects/Textures/tx_normal04_256.png
        resources/effects/Textures/tx_normal05_256.png
        resources/effects/Textures/tx_normal06_256.png
        resources/effects/Textures/tx_prism_aura01.png
        resources/effects/Textures/tx_ring01_256.png
        resources/effects/Textures/tx_shape01_128.png
        resources/effects/Textures/tx_shockring01_256.png
        resources/effects/Textures/tx_shockring02_256.png
        resources/effects/Textures/tx_shockring03_256.png
        resources/effects/Textures/tx_slash01_256.png
        resources/effects/Textures/tx_smoke01_256.png
        resources/effects/Textures/tx_smoke02_256.png
        resources/effects/Textures/tx_snow01_256.png
        resources/effects/Textures/tx_star01_256.png
        resources/effects/Textures/tx_star02_256.png
        resources/effects/Textures/tx_trail01_256.png
        resources/effects/Textures/tx_trail01_256_90.png
        resources/effects/Textures/tx_trail02_256.png
        resources/effects/Textures/tx_trail02_256_90.png
        resources/effects/Textures/tx_trail03_256.png
        resources/effects/Textures/tx_trail03_256_90.png
        resources/effects/Textures/tx_white01_8.png
        resources/UI/UI1.png
        resources/UI/UI3.png
        resources/UI/UI4.png
        resources/UI/UI2/UI2_alpha1.png
        resources/UI/UI2/UI2_alpha2.png
        resources/UI/UI2/UI2_fbx.fbx
        resources/UI/title3.png
        resources/UI/titlelogo.png
        resources/UI/result_lose.png
        resources/UI/result_win.png
        resources/music/hackathon3_r1.wav
        resources/music/hackathon3_r2.wav
        shaders/main.hlsl
        shaders/renderTarget.hlsl
        shaders/skybox.hlsl
        shaders/cubemap.hlsl
        shaders/bullet.hlsl
        shaders/ui/ui.hlsl
        shaders/ui/side_ui.hlsl
        shaders/ui/resultback.hlsl
        shaders/ui/resulttext.hlsl
        shaders/eye.hlsl
)

create_executable(${src_files} ${include_files} ${resources})
