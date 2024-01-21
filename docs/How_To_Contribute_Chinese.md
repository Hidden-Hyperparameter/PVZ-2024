# Contribute to the project

首先，你应该检查以下要求。

## Test your GUI

我们使用 Vulkan 包作为图形界面。由于程序可能存在问题，请首先选择 `gui_test` 作为你的运行对象并编译。如果成功，你应当看到这个图片.两个方块同时对称的运动。如果界面有点卡，这是正常现象.

![succeed](/docs/pictures/gui_succeed.png)

成功运行此测试后，你可以再改回代码并开始开发。

## 开发

你可以基于现有框架自由添加元素。

### 如何添加新植物或僵尸

你必须执行以下步骤：

与`Game`集成：植物的图像必须在`Game`类（函数`LoadEverything`）中加载；价格应该写在`BackGround` 的 `InitPrice`中。
- 如果有新的子弹也应该写. 我们可以考虑建一个子弹的新的 `Class`, 比如叫做 `Bullet`。
- 在`units.h`中包含你的新头文件，这是一个包含所有单位的头文件。
- 在你的constructor中，**你必须设置单位名称**以适应**图片的文件夹名称**。否则，你的单位加载图片会失败。
- 最后别忘记修改 `Background` 中的 `MakePlant` 函数，没有这个函数（显然）程序不能通过名字判断应该创造什么植物.

你可以使用下面的Python代码将`.gif`文件转换为`.png`：
```python
from PIL import Image
from PIL.GifImagePlugin import ImageSequence
import os

def convert_gif_to_png_frames(gif_path, png_folder='frames'):
    gif = Image.open(gif_path)
    i = 0
    for frame in ImageSequence.Iterator(gif):
        frame.save(os.path.join(png_folder, f'{i}.png'), 'PNG')
        i += 1
convert_gif_to_png_frames('path_to_your_gif.gif')
```

名称标准必须是`number.png`才符合程序要求。

## 可能的错误和解决方案
如果在过程中遇到一些错误，请参考下面：

### Abort() 被调用

首先检查上面 “如何添加新植物或僵尸” 部分，你是否做了全部.

如果都做了，并且显示无法加载图片，那么这大概是由于加载或放置图像后的`assert`函数引起的：

1. 首先想办法定义 `VULKAN_DEBUG` 这个宏. 无论你是在 CMakeLists.txt 中启用 DEBUG_VULKAN 这个 option 还是在程序 [file_helper.h](/gui/lib/include/file_helper.h) 中直接 `#define`，只要成功定义，你就应当看到程序在某处输出你现在工作的地址.
2. 我们的所有图片资源储存在 [assets](/assets/) 中。在 `abort` 的信息之前，你应该能看到程序输出在哪里找不到文件. 调整 [file_helper.cpp](/gui/lib/src/file_helper.cpp) 的第2行，直到找到你所对应的文件。在调整该行时，请注意：
   - 不可以删去任何一个元素.因为这代表前面的人所尝试必须加入的元素.
   - 加入尽可能少的元素.因为你加入的元素越多，加载程序就越慢.

  
如果遇到了其他情况的 `abort`, 请发表 issue.

### 与 Class 相关的编译错误(如无法解析的外部符号)

首先参考这条建议.

![引用](/docs/pictures/quote.png)

如果还是不行，尝试：
- 删除 CMake Cache
- 多 declare 一些引用的 class
- 使用指针而非对象. 比如 class A 里最好存 B 的指针而非实例.

### Other problems

- 每一个 unit 在调用 `Remove` 之后，还会再显示一帧。确保此时不会 `vector index out of bound`.