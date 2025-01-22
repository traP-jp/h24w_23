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
            if (src MATCHES ".*\\.(fbx|png)$")
                file(COPY ${src} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/models)
            endif ()
            if (src MATCHES ".*\\.hdr$")
                file(COPY ${src} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures)
            endif ()
            if (src MATCHES ".*\\.efkefc$")
                file(COPY ${src} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/effects)
            endif ()
        endif ()
    endforeach ()

    find_package(directxtex CONFIG REQUIRED)

    add_executable(h24w_23 WIN32 ${src_list})
    target_include_directories(h24w_23 PRIVATE include)
    target_include_directories(h24w_23 PRIVATE $ENV{aquaengine_dir}/include)
    target_include_directories(h24w_23 PRIVATE $ENV{FBXSDK_DIR}/include)
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
    target_link_libraries(h24w_23 PUBLIC $ENV{FBXSDK_DIR}/lib/x64/debug/libfbxsdk-md.lib)
    target_link_libraries(h24w_23 PUBLIC $ENV{FBXSDK_DIR}/lib/x64/debug/libxml2-md.lib)
    target_link_libraries(h24w_23 PUBLIC $ENV{FBXSDK_DIR}/lib/x64/debug/zlib-md.lib)

    target_compile_definitions(h24w_23 PRIVATE DEBUG)
endmacro()

set(src_files
        src/main.cpp
        src/view/MainWindow.cpp
        src/view/IPDialog.cpp
        src/3d/Engine.cpp
        src/3d/GameView.cpp
        src/3d/Player.cpp
        src/2d/D2DEngine.cpp
        src/2d/Title.cpp
        src/network/Network.cpp
)

set(include_files
        include/view/BaseWindow.h
        include/view/MainWindow.h
        include/view/IPDialog.h
        include/3d/Engine.h
        include/3d/GameView.h
        include/3d/Player.h
        include/2d/D2DEngine.h
        include/2d/Title.h
        include/network/Network.h
)

# TODO: delete isu, ninja
set(resources
        include/view/resource.h
        include/view/IPDialog.rc
        resources/models/isu.fbx
        resources/models/isu.png
        resources/models/ninja.png
        resources/models/ninja.fbx
        resources/models/player_model/arm.fbx
        resources/models/player_model/body.fbx
        resources/models/player_model/buster.fbx
        resources/models/player_model/eye.fbx
        resources/models/player_model/gun.fbx
        resources/models/player_model/head.fbx
        resources/models/player_model/ring.fbx
        resources/models/player_model/thuraster.fbx
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
        resources/textures/space.hdr
        shaders/main.hlsl
        shaders/renderTarget.hlsl
        shaders/skybox.hlsl
        shaders/cubemap.hlsl
)

create_executable(${src_files} ${include_files} ${resources})
