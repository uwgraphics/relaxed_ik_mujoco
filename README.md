# RelaxedIK MuJoCo

This is a RelaxedIK Plugin for [MuJoCo](http://www.mujoco.org/) which is an advanced physics simulation engine that may have some extra features unavailable in CoppeliaSim. You can find an introduction and some useful citation information of RelaxedIK in [RelaxedIK Core](https://github.com/uwgraphics/relaxed_ik_core) which is a submodule of this repo. It is recommended to look at [RelaxedIK Core](https://github.com/uwgraphics/relaxed_ik_core) before working with this wrapper.

Note that this wrapper is developed in Mujoco 2.0 Linux and Ubuntu 18.04. 

## RelaxedIK Family

More information about RelaxedIK, CollisionIK, and all the wrappers could be found in this [documentation](https://uwgraphics.github.io/relaxed_ik_core/).

- [RelaxedIK (Deprecated)](https://github.com/uwgraphics/relaxed_ik/tree/dev)
- [RelaxedIK Core](https://github.com/uwgraphics/relaxed_ik_core)
- [RelaxedIK ROS1](https://github.com/uwgraphics/relaxed_ik_ros1)
- [RelaxedIK Unity](https://github.com/uwgraphics/relaxed_ik_unity)
- [RelaxedIK CoppeliaSim](https://github.com/uwgraphics/relaxed_ik_coppeliasim)
- [RelaxedIK Mujoco](https://github.com/uwgraphics/relaxed_ik_mujoco)

||**RelaxedIK (Deprecated)**|**RelaxedIK ROS1**|**RelaxedIK Unity**|**RelaxedIK Coppeliasim**|**RelaxedIK Mujoco**|  
|:------|:-----|:-----|:-----|:-----|:-----| 
|**RelaxedIK**|:o:|:o:|:o:|:o:|:o:|  
|**CollisionIK**|:x:|:o:|:x:|:x:|:x:|  

## Dependencies

To use this wrapper, you will first need to install Rust. Please go to https://www.rust-lang.org/learn/get-started for more infomation.

## Getting Started

1. Make sure that you have installed all the dependencies.
1. Clone this repo to the installation folder of MuJoCo as a sibling folder of *bin* and *include*.
1. Initialize relaxed_ik_core (the Rust library of RelaxedIK) as a submodule by running the following command from the project directory:
    ```bash
    git submodule update --init
    ```
1. Navigate to the *relaxed_ik_core* folder and go through the steps below to get relaxed_ik_core ready.
    1. If your robot is in this list: [baxter, hubo, iiwa7, jaco7, panda, sawyer, ur5, yumi], ignore this step. Else, you will need to clone [this repo](https://github.com/uwgraphics/relaxed_ik) and follow the step-by-step guide [there](https://github.com/uwgraphics/relaxed_ik/blob/dev/src/start_here.py) to get the required robot config files into corresponding folders in the *config* folder in the core. To specify, there should be (replace "sawyer" with your robot name or your urdf name in some cases):
        - 1 self-collision file <collision_sawyer.yaml> in the *collision_files* folder
        - 4 Rust neural network files <sawyer_nn, sawyer_nn.yaml, sawyer_nn_jointpoint, sawyer_nn_jointpoint.yaml> in the *collision_nn_rust* folder
        - 1 info file <sawyer_info.yaml> in the *info_files* folder
        - 1 joint state function file <sawyer_joint_state_define> in the *joint_state_define_functions* folder
        - 1 urdf file <sawyer.urdf> in the *urdfs* folder.
    2. Compile the core:
        ```bash
        cargo build
        ```
2. If you are working with a new robot, add the corresponding XML model into the root directory of this repo. Refer to the *franka_sim* and *sawyer_sim* folders for examples.
3. Complie RelaxedIK in Mujoco by running `make` from the root directory of this repo.
4. Select one of the robot models avaiable (sawyer and panda) and play with RelaxedIK by running the following commands from the root directory of this repo. When the program is running, the joint angle solutions will be printed to the console.
    ```bash
    cd ../bin
    # sawyer
    echo -n "sawyer_info.yaml" > ../relaxed_ik_mujoco/relaxed_ik_core/config/loaded_robot
    ./relaxedIK ../relaxed_ik_mujoco/sawyer_sim/sawyer.xml 
    # or panda
    echo -n "panda_info.yaml" > ../relaxed_ik_mujoco/relaxed_ik_core/config/loaded_robot
    ./relaxedIK ../relaxed_ik_mujoco/franka_sim/franka_panda.xml
    ```
5. If you want to fit RelaxedIK into your own project, please follow these steps:
    1. Go to `relaxed_ik_mujoco/relaxed_ik_core/config/loaded_robot` and type the name of the pre-computed robot arm you would like to run (available options are baxter, hubo, iiwa7, jaco7, panda, sawyer, ur5 and yumi).
    2. All the code required to call RelaxedIK is marked by "Added for RelaxedIK" in relaxed_ik_mujoco/relaxedIK.cpp. You can take a look at it for reference of how to call RelaxedIK in your script.
    3. Refer to relaxed_ik_mujoco/Makefile for the example of how to compile a Mujoco project with RelaxedIK
    4. Run the executable and enjoy RelaxedIK!
   
## For Testing Purposes

- Use the keyboard IK goal driver to control the robot. Please make sure that the Mujoco window has focus (i.e., make sure it's clicked), then use the following keystrokes:
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

