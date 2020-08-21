# relaxed_ik_mujoco
Here is the project page that has more information about RelaxedIK that you may want to know about: https://uwgraphics.github.io/relaxed_ik_core/, and we are still working on it.

This is a Mujoco wrapper for RelaxedIK. You can find other wrappers of RelaxedIK on the project page.

## Dependencies
Note that this wrapper is developed in Mujoco 2.0 Linux and Ubuntu 18.04. 

### Rust Dependencies (Not optional)
To use this wrapper, you will first need to install Rust. More information is here: https://www.rust-lang.org/learn/get-started

## Run
1. Install all the dependencies.
1. Clone this repo to the installation folder of Mujoco (as a sibling of bin/ and include/).
    ```
    cd [The installation folder of Mujoco]
    git clone git@github.com:uwgraphics/relaxed_ik_mujoco.git
    ```
1. Get the submodule relaxed_ik_core (the core part of relaxed IK written in Rust) and compile it by running the following commands from the directory of this repo:
    ```
    git submodule update --init
    cd ./relaxed_ik_core
    cargo build
    ```
1. Complie RelaxedIK in Mujoco by running the following commands from the directory of this repo.
    ```
    make
    ```
1. Select one of the robot models we set up (available options now include sawyer and panda) and play with RelaxedIK by running the following commands from the directory of this repo. When the program is running, the joint angle solutions will be printed to the console.
    ```
    cd ../bin
    # sawyer
    echo -n "sawyer_info.yaml" > ../relaxed_ik_mujoco/relaxed_ik_core/config/loaded_robot
    ./relaxedIK ../relaxed_ik_mujoco/sawyer_sim/sawyer.xml 
    # panda
    echo -n "panda_info.yaml" > ../relaxed_ik_mujoco/relaxed_ik_core/config/loaded_robot
    ./relaxedIK ../relaxed_ik_mujoco/franka_sim/franka_panda.xml
    ```
1. Optional: Use the keyboard IK goal driver to control the robot. Please make sure that the Mujoco window has focus (i.e., make sure it's clicked), then use the following keystrokes:
    ```
    w - move chain 1 along +X
    x - move chain 1 along -X
    a - move chain 1 along +Y
    d - move chain 1 along -Y
    q - move chain 1 along +Z
    z - move chain 1 along -Z
    1 - rotate chain 1 around +X
    2 - rotate chain 1 around -X
    3 - rotate chain 1 around +Y
    4 - rotate chain 1 around -Y
    5 - rotate chain 1 around +Z
    6 rotate chain 1 around -Z

    i - move chain 2 along +X
    m - move chain 2 along -X
    j - move chain 2 along +Y
    l - move chain 2 along -Y
    u - move chain 2 along +Z
    n - move chain 2 along -Z
    = - rotate chain 2 around +X
    - - rotate chain 2 around -X
    0 - rotate chain 2 around +Y
    9 - rotate chain 2 around -Y
    8 - rotate chain 2 around +Z
    7 - rotate chain 2 around -Z
    ```
1. If you want to fit Relaxed IK into your own project, please follow these steps:
    1. Go to relaxed_ik_mujoco/relaxed_ik_core/config/loaded_robot and configure the name of the pre-computed robot arm you would like to run (available options are baxter, hubo, iiwa7, jaco7, panda, sawyer, ur5 and yumi).
    1. All the code required to call RelaxedIK is marked by "Added for Relaxed IK" in relaxed_ik_mujoco/relaxedIK.cpp. You can take a look at it for reference of how to call RelaxedIK in your script.
    1. Refer to relaxed_ik_mujoco/Makefile for the example of how to compile a Mujoco project with RelaxedIK
    1. Run the executable and enjoy RelaxedIK!
