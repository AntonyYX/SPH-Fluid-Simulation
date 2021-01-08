## Platform
This project has only been tested on Windows x64.


## Packages Used
Eigen 3.3.9
GLEW 1.11.0
freeglut 3.0.0 MSVC

Here is link for downloading all three packages(if needed):
https://drive.google.com/file/d/1cxuKjSe_X8A9R-9-Qy8t38qDZNr-q79O/view?usp=sharing


## Example Program
Run /SPH_FluidSimulation/SPH_FluidSimulation.exe to display a precompiled solution.


## Setup
1. Install Eigen, GLEW, FreeGLUT
2. Use Visual Studio to open /SPH_FluidSimulation.sln
3. Change build mode to Release and x86
4. In "configuration Properties", add three packages to "Additional Include Directories" under "C/C++"
5. In "configuration Properties", add GLEW and FreeGLUT to "Additional Library Directories" under "Linker"
6. Build


## Parameters Tuning
In /SPH_FluidSimulation/src/main.cpp

1. Under "rendering hyperparameters", there are parameters related to the window size and particle sizes
2. Under "initialization hyperparameters", there are parameters that indicate the postion and number of particles generated
3. Under "physics hyperparameters", there are physics related parameters that affect the simulation

## Video Link
https://youtu.be/loLEcJlnGzI
