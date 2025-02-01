# CMake SFML DODGE! Videogame
DODGE! is an arcade pixelart videogame, in which you must survive as long as possible while dodging the enemies that surround you, using PowerUps and collecting gems in order to achieve the highest score possible. It is built in C++ using SFML and Visual Studio.

## How to Use
1. Install [Git](https://git-scm.com/downloads) and [CMake](https://cmake.org/download/). Use your system's package manager if available.
2. Clone your new GitHub repo and open the repo in your text editor of choice.
3. If you use Linux, install SFML's dependencies using your system package manager. On Ubuntu and other Debian-based distributions you can use the following commands:
   ```
   sudo apt update
   sudo apt install \
       libxrandr-dev \
       libxcursor-dev \
       libudev-dev \
       libfreetype-dev \
       libopenal-dev \
       libflac-dev \
       libvorbis-dev \
       libgl1-mesa-dev \
       libegl1-mesa-dev
   ```
4. Configure and build your project. Most popular IDEs support CMake projects with very little effort on your part.

   - [VS Code](https://code.visualstudio.com) via the [CMake extension](https://code.visualstudio.com/docs/cpp/cmake-linux)
   - [Visual Studio](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170)
   - [CLion](https://www.jetbrains.com/clion/features/cmake-support.html)
   - [Qt Creator](https://doc.qt.io/qtcreator/creator-project-cmake.html)

   Using CMake from the command line is straightforward as well.
   Be sure to run these commands in the root directory of the project you just created.

   ```
   cmake -B build
   cmake --build build
   ```

5. Enjoy DODGE!

### How to play
1. Player movement: WASD / Arrow keys
2. Sprint: LShift (consumes energy)
3. P: Pause
4. Avoid the enemies to increase your score
5. Pick-up gems to increase your score even more!
6. Pick-up PowerUps to recover health and energy

#### License
This project is released under the MIT License.