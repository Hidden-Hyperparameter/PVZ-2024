# Contribute To The Project
First, you should check the following requirements.
## Requirements

### Compiling
This project requires **Visual Studio(VS)** to compile. VS code **isn't** supported.

To open the project on VS, first open a new window. Then, select the `.sln` file and open it. If you succeed, then the interface should look like this.

![this](/docs/pictures/VS_studio.png).

**Notes: I haven't tried it on another computer yet, so if you encounter some problems at this step, feel free to submit an issue immediately!**

### Graphics Interface

We use the package `EasyX` for the graphics interface. 

**Be sure that you have installed the [EasyX extension](https://easyx.cn/) before running the project!**

To test whether you have downloaded it correctly, you can run the file [test.cpp](/test.cpp). The way that you run it should be: first change the `main` function in [main.cpp](/main.cpp) into comments(`//`), and then change the comments containing `main` function in `test.cpp` into real code. Then, use VS to run the project. If the test is run correctly, you should see the figure below, where the red dots appear whenever you left-click your mouse on the window.
![figure](/docs/pictures/test.png)

After succeed in running this test, you can then change back the code and start developing.

## Developments


You can add elements freely based on the existing framework. 

### How to add a new plant or zombie

You have to do the following steps:

Integrate with the `BackGround`: the images of plants have to be loaded in the `BackGround` class(function `LoadEverything`); the price should be written in `InitPrice`.
- the bullets (if new) should be written, too.
- Include your new header in `units.h`, a header file for all units.
- In your constructer, **YOU MUST CHANGE THE UNIT NAME** to fit the **FOLDER NAME**. Otherwise, your unit isn't going to load.


You can use the Python code below to change `.gif` file to `.png`:
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

The name standard must be `number.png` to meet the program.

## Possible Errors and Solutions
If you encounter some errors during the process, please see below:

### Abort() called

Usually, this is due to the `assert` functions after image loading or putting.

### Class-related Compilation errors

![quote](/docs/pictures/quote.png)

For example, don't include `units.h` in `background.h`. If you want to use specific units, try adding its `.h` file in `background.cpp`.
