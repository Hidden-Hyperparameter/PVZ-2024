# PVZ-2024

 pvz-2024
 This repository is redirected from [pvz](https://github.com/liuhanzuo/pvz2024) to make a more OOP-styled approach for the Plant-VS-Zombie game.

## Requirements

Please use Visual Studio (instead of VS code) and the extension [easyx](https://easyx.cn/setup) for running this project. Follow the instruction in the website to setup easyx, and then open the `.sln` file using Visual Studio command "open project".

If the `main.cpp` can't run at all and give you some compilation errors, please first change the `main` function in `main.cpp` into notes (use `//`), and then change the notes in `test.cpp` into real `main` function. If this time the project can't run as well, then it means that your setup for EasyX isn't complete.

## Developments

You can add elements freely based on the existing framework. 

### How to add a new plant or zombie

You have to do the following steps:

- plants are inhereited from `Plant` class, zombies are inherited from `Zombie` class
- the images of plants have to be loaded in the `BackGround` class(function `LoadEverything` )
- the bullets (if new) should be load, too.
- 



If you encounter some errors during the process, please see below:

### Hints(Common questions in development)

- When you have to include something in `.h` file, then declare the class you will use. For example:

```cpp
#include "unit.h"
#include "zombie.h"
#include "game_parameters.h"
class Unit;
class Zombie;
class Parameter;
class Pea:public Unit{
	//your code here
}
```

This will probably solve most of the compilation error associated with **"undefined symbols"**.

- If you want to override the `Update` function which do the image-status changing work, you should now that `max_image_num_ - 1` is the maximum image status you can have, instead of `max_image_num_`.

This will solve the problem when you find **`abort()` has been called** when uploading images (in the function `IMAGE* GetImage(const std::string&, int status)`).

- strange problems (always ~80 compilation errors)
  Solution: don't include `units.h` in `background.h`. If you want to use specific items, try add its `.h` file in `background.cpp`.
