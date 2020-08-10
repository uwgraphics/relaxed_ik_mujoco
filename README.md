# relaxed_ik_mujoco
Here is the project page that has more information about RelaxedIK that you may want to know about: https://uwgraphics.github.io/relaxed_ik_core/

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
1. Complie and run the ur5 example of using RelaxedIK in Mujoco by running the following commands from the directory of this repo.
    ```
    make
    cd ../bin
    ./relaxedIK ../relaxed_ik_mujoco/ur5/ur5.xml
    ```
1. If you want to use Relaxed IK in your own project, follow these steps:
    1. Go to relaxed_ik_mujoco/relaxed_ik_core/config/loaded_robot and configure the name of the pre-computed robot arm you would like to run (available options are baxter, hubo, iiwa7, jaco7, panda, sawyer, ur5 and yumi).
    1. All the code required to call RelaxedIK is marked by "Added for Relaxed IK" in relaxed_ik_mujoco/relaxedIK.cpp. You can take a look at it for reference of how to call RelaxedIK in your script.
    1. Refer to relaxed_ik_mujoco/Makefile for the example of how to compile a Mujoco project with RelaxedIK
    1. Run the executable and enjoy RelaxedIK!
