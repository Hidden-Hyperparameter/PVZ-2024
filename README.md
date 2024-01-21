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

## Others

### 无法解析的外部符号

删除build 文件夹重试,header gaurd

use pointers

don't use initializer list when constructing objects containing pointers. the sequence may matter

### May not work
add VULKAN_DEBUG manually