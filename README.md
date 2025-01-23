# h24w_23

## dependencies

- [FBXSDK](https://aps.autodesk.com/developer/overview/fbx-sdk)
- [Aqua-Engine](https://github.com/kavos113/aqua-engine)
- [Effekseer](https://github.com/effekseer/Effekseer)

```shell
vcpkg install
```

## run

```shell
cmake -S . -B build -DLOCAL_BUILD=ON
cmake --build build
```

## format, lint

requires LLVM

```shell
clang-format -i $(find -name "*.h" -not -path "./<BUILD_DIR>/*" -or -name "*.cpp" -not -path "./<BUILD_DIR>/*") --verbose -style=file
clang-tidy $(find -name "*.h" -not -path "./<BUILD_DIR>/*" -or -name "*.cpp" -not -path "./<BUILD_DIR>/*") --config-file=.clang-tidy -p ./<BUILD_DIR> --warnings-as-errors='*'
```

rename `<BUILD_DIR>` to your build directory