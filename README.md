# PVZ-2024

 pvz-2024

 This repository is redirected from [pvz](https://github.com/liuhanzuo/pvz2024) to make a more OOP-styled approach for the Plant-VS-Zombie game.

## Language

As #13 mentioned, since we are Chineses, 我们使用中文.

## Documents

请参考 [docs](/docs/README.md) 来获得更全面的信息.

## Notes before you fork this repository

1. Recursive clone: 本仓库含有 external links, 因此请确保使用 `recursive` 模式 clone 仓库。如果已经 clone 了，可以使用 `git submodule update --recursive`.
2. Build: 理论上，本仓库是 cross-platform 的。如果在 Mac 和 Linux 上失败，请发表 issue. 本仓库属于 CMake 项目，因此可以使用一切支持 CMake 的 IDE.
3. Vulkan: 本项目含有以 Vulkan 为基础的 graphics interface, 如果没有安装 Vulkan 包，请 [download at LunarG](https://www.lunarg.com/vulkan-sdk/). 在下载完成后，请将 Vulkan/bin 设置为系统 PATH. 在终端运行 `vkcube` 来检查是否下载成功。
**注意：`submodule` 中的Vulkan-Headers并不是上面所说的Vulkan 包，下载了那个`submodule` 并不代表安装了 Vulkan**.
4. Target: 本项目有两个 target. 请先测试 `test_gui` 可以运行，再运行主要项目.

## Code Edit

### Debug

在 PVZ-2024 这个项目的 [CMakeLists](/CMakeLists.txt) 里，有一个 option DEBUG_VULKAN. 如果将这个开关设为 ON, 则会显示来自 Vulkan 的调试信息。由于 CMake Cache 等原因，设置可能无效。这时候可能需要手动在 [file_helper.h](/gui/lib/include/file_helper.h) 内 `#define VULKAN_DEBUG`.

### Common problem: 无法解析的外部符号

1. 删除 CMake Cache 重试
2. 检查是否加入了 header guard, 即 `#pragma once`.
3. 检查头文件的循环引用, 使用指针来引用其他文件中的 `class` 对象.
4. Don't use initializer list when constructing objects containing pointers. the sequence may matter.
