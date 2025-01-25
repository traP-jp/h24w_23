# h24w_23

## dependencies

- [FBXSDK](https://aps.autodesk.com/developer/overview/fbx-sdk)
- [Aqua-Engine](https://github.com/kavos113/aqua-engine)
- [Effekseer](https://github.com/effekseer/Effekseer)
- Windows SDK(DirectX12, DirectX11, Direct2D, DirectWrite, WinSock2, XAudio2)

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

## Play Guide

このゲームは2人用です。

- LANケーブルとスイッチングハブを用いて、2台のPCを接続します。
- ファイアウォールなどの設定を確認し、通信が可能な状態にします。
- 最初のダイアログで、どちらかがPlayer1、もう片方がPlayer2を選択します。
- 次のダイアログで、相手のIPアドレスを入力します。このとき、OKは押さずに待機します。
- 両方のマシンのコンソールに「Engine Initialized」と表示されたら、2人同時にOKを押します。
- Have fun!!!!!!