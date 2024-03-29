/*  Copyright © 2018, Roboti LLC

    This file is licensed under the MuJoCo Resource License (the "License").
    You may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        https://www.roboti.us/resourcelicense.txt
*/

#include "mujoco.h"
#include "glfw3.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "relaxedIK.h"

// MuJoCo data structures
mjModel* m = NULL;                  // MuJoCo model
mjData* d = NULL;                   // MuJoCo data
mjvCamera cam;                      // abstract camera
mjvOption opt;                      // visualization options
mjvScene scn;                       // abstract scene
mjrContext con;                     // custom GPU context

//////////////////// Added for Relaxed IK ////////////////////
// keyboard interactions to change the pose goal
std::vector<double> pos_l {0.0, 0.0, 0.0};
std::vector<double> rot_l {0.0, 0.0, 0.0};
std::vector<double> pos_r {0.0, 0.0, 0.0};
std::vector<double> rot_r {0.0, 0.0, 0.0};
double pos_stride = 0.015;
double rot_stride = 0.055;
//////////////////// Added for Relaxed IK ////////////////////

// mouse interaction
bool button_left = false;
bool button_middle = false;
bool button_right =  false;
double lastx = 0;
double lasty = 0;

std::vector<double> eulerToQuat(std::vector<double> euler) 
{
    double c1 = std::cos(euler[0]/2);
    double s1 = std::sin(euler[0]/2);
    double c2 = std::cos(euler[1]/2);
    double s2 = std::sin(euler[1]/2);
    double c3 = std::cos(euler[2]/2);
    double s3 = std::sin(euler[2]/2);
    double c1c2 = c1*c2;
    double s1s2 = s1*s2;
    std::vector<double> quat;
    quat.push_back(c1c2*s3 + s1s2*c3); // x
	quat.push_back(s1*c2*c3 + c1*s2*s3); // y
	quat.push_back(c1*s2*c3 - s1*c2*s3); // z
    quat.push_back(c1c2*c3 - s1s2*s3); // w
    return quat;
}

// keyboard callback
void keyboard(GLFWwindow* window, int key, int scancode, int act, int mods)
{
    // backspace: reset simulation
    if( act==GLFW_PRESS && key==GLFW_KEY_BACKSPACE )
    {
        mj_resetData(m, d);
        mj_forward(m, d);
    }
    //////////////////// Added for Relaxed IK ////////////////////
    if ( act==GLFW_REPEAT || act==GLFW_PRESS )
    {
        if ( key==GLFW_KEY_W ) {
            pos_l[0] += pos_stride;
        } else if ( key==GLFW_KEY_X ) {
            pos_l[0] -= pos_stride;
        } else if ( key==GLFW_KEY_A ) {   
            pos_l[1] += pos_stride;
        } else if ( key==GLFW_KEY_D ) {
            pos_l[1] -= pos_stride;
        } else if ( key==GLFW_KEY_Q ) {
            pos_l[2] += pos_stride;
        } else if ( key==GLFW_KEY_Z ) {   
            pos_l[2] -= pos_stride;         
        } else if ( key==GLFW_KEY_1 ) {
            rot_l[0] += rot_stride;
        } else if ( key==GLFW_KEY_2 ) {
            rot_l[0] -= rot_stride;
        } else if ( key==GLFW_KEY_3 ) {   
            rot_l[1] += rot_stride;
        } else if ( key==GLFW_KEY_4 ) {
            rot_l[1] -= rot_stride;
        } else if ( key==GLFW_KEY_5 ) {
            rot_l[2] += rot_stride;
        } else if ( key==GLFW_KEY_6 ) {
            rot_l[2] -= rot_stride;
        } else if ( key==GLFW_KEY_I ) {  
            pos_r[0] += pos_stride;
        } else if ( key==GLFW_KEY_M ) {
            pos_r[0] -= pos_stride;
        } else if ( key==GLFW_KEY_J ) {
            pos_r[1] += pos_stride;
        } else if ( key==GLFW_KEY_L ) {  
            pos_r[1] -= pos_stride;
        } else if ( key==GLFW_KEY_U ) {
            pos_r[2] += pos_stride;
        } else if ( key==GLFW_KEY_N ) {
            pos_r[2] -= pos_stride;
        } else if ( key==GLFW_KEY_EQUAL ) {
            rot_r[0] += rot_stride;
        } else if ( key==GLFW_KEY_MINUS ) {
            rot_r[0] -= rot_stride;
        } else if ( key==GLFW_KEY_0 ) {
            rot_r[1] += rot_stride;
        } else if ( key==GLFW_KEY_9 ) {
            rot_r[1] -= rot_stride;     
        } else if ( key==GLFW_KEY_8 ) {  
            rot_r[2] += rot_stride;          
        } else if ( key==GLFW_KEY_7 ) {
            rot_r[2] -= rot_stride;
        }
    }
    //////////////////// Added for Relaxed IK ////////////////////
}

// mouse button callback
void mouse_button(GLFWwindow* window, int button, int act, int mods)
{
    // update button state
    button_left =   (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS);
    button_middle = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)==GLFW_PRESS);
    button_right =  (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS);

    // update mouse position
    glfwGetCursorPos(window, &lastx, &lasty);
}

// mouse move callback
void mouse_move(GLFWwindow* window, double xpos, double ypos)
{
    // no buttons down: nothing to do
    if( !button_left && !button_middle && !button_right )
        return;

    // compute mouse displacement, save
    double dx = xpos - lastx;
    double dy = ypos - lasty;
    lastx = xpos;
    lasty = ypos;

    // get current window size
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // get shift key state
    bool mod_shift = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS ||
                      glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)==GLFW_PRESS);

    // determine action based on mouse button
    mjtMouse action;
    if( button_right )
        action = mod_shift ? mjMOUSE_MOVE_H : mjMOUSE_MOVE_V;
    else if( button_left )
        action = mod_shift ? mjMOUSE_ROTATE_H : mjMOUSE_ROTATE_V;
    else
        action = mjMOUSE_ZOOM;

    // move camera
    mjv_moveCamera(m, action, dx/height, dy/height, &scn, &cam);
}

// scroll callback
void scroll(GLFWwindow* window, double xoffset, double yoffset)
{
    // emulate vertical mouse motion = 5% of window height
    mjv_moveCamera(m, mjMOUSE_ZOOM, 0, -0.05*yoffset, &scn, &cam);
}

// main function
int main(int argc, const char** argv)
{
    // check command-line arguments
    if( argc!=2 )
    {
        printf(" USAGE:  basic modelfile\n");
        return 0;
    }

    // activate software
    mj_activate("mjkey.txt");

    // load and compile model
    char error[1000] = "Could not load binary model";
    if( strlen(argv[1])>4 && !strcmp(argv[1]+strlen(argv[1])-4, ".mjb") )
        m = mj_loadModel(argv[1], 0);
    else
        m = mj_loadXML(argv[1], 0, error, 1000);
    if( !m )
        mju_error_s("Load model error: %s", error);

    //////////////////// Added for Relaxed IK ////////////////////
    // It is usually necessary to disable gravity in order to run 
    // Relaxed IK, whether to disable other fligs depend on the specific
    // project. This example disables all the flags for simplicity.
    m->opt.disableflags = (1 << 12) - 1;
    //////////////////// Added for Relaxed IK ////////////////////

    // make data
    d = mj_makeData(m);

    // init GLFW
    if( !glfwInit() )
        mju_error("Could not initialize GLFW");

    // create window, make OpenGL context current, request v-sync
    GLFWwindow* window = glfwCreateWindow(1200, 900, "Demo", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // initialize visualization data structures
    mjv_defaultCamera(&cam);
    mjv_defaultOption(&opt);
    mjv_defaultScene(&scn);
    mjr_defaultContext(&con);

    // create scene and context
    mjv_makeScene(m, &scn, 2000);
    mjr_makeContext(m, &con, mjFONTSCALE_150);

    // install GLFW mouse and keyboard callbacks
    glfwSetKeyCallback(window, keyboard);
    glfwSetCursorPosCallback(window, mouse_move);
    glfwSetMouseButtonCallback(window, mouse_button);
    glfwSetScrollCallback(window, scroll);

    //////////////////// Added for Relaxed IK ////////////////////
    // It is necessary to cd to "relaxed_ik_core" to find the required 
    // config files for now 
    int MAXPATHLEN = 256;
    char buffer[MAXPATHLEN];
    if (chdir("../relaxed_ik_mujoco/relaxed_ik_core") != 0) {
        std::cout << "Error: failed to change to the directory relaxed_ik_core\n" <<
            "The current working directory is " << getcwd(buffer, MAXPATHLEN) << std::endl;
    };
    //////////////////// Added for Relaxed IK ////////////////////

    // run main loop, target real-time simulation and 60 fps rendering
    while( !glfwWindowShouldClose(window) )
    {
        //////////////////// Added for Relaxed IK ////////////////////
        // The position goal and the rotation goal
        std::vector<double> pos_goal = pos_l;
        pos_goal.insert(pos_goal.end(), pos_r.begin(), pos_r.end());
        std::vector<double> quat_l = eulerToQuat(rot_l);
        std::vector<double> quat_r = eulerToQuat(rot_r);
        std::vector<double> quat_goal = quat_l;
        quat_goal.insert(quat_goal.end(), quat_r.begin(), quat_r.end());

        // get the joint angle solution from relaxed IK
        // As the only callable function in this RelaxedIK library, "solve" takes in 
        // a position goal and a rotation goal and returns a joint angle solution. 
        // Usually you want to put it in the main loop to update the joint angles consistently.
        Opt x = solve(pos_goal.data(), (int) pos_goal.size(), quat_goal.data(), (int) quat_goal.size());

        // assign and print the joint angles
        std::cout << "RelaxedIK: [";
        for (int i = 0; i < x.length; i++) {
            d->qpos[i] = x.data[i];
            if (i < x.length - 1) {
                std::cout << x.data[i] << ", ";
            } else {
                std::cout << x.data[i] << "]\n";
            }
        }
        //////////////////// Added for Relaxed IK ////////////////////

        // advance interactive simulation for 1/60 sec
        //  Assuming MuJoCo can simulate faster than real-time, which it usually can,
        //  this loop will finish on time for the next frame to be rendered at 60 fps.
        //  Otherwise add a cpu timer and exit this loop when it is time to render.
        mjtNum simstart = d->time;
        while( d->time - simstart < 1.0/60.0 )
            mj_step(m, d);

        // get framebuffer viewport
        mjrRect viewport = {0, 0, 0, 0};
        glfwGetFramebufferSize(window, &viewport.width, &viewport.height);

        // update scene and render
        mjv_updateScene(m, d, &opt, NULL, &cam, mjCAT_ALL, &scn);
        mjr_render(viewport, &scn, &con);

        // swap OpenGL buffers (blocking call due to v-sync)
        glfwSwapBuffers(window);

        // process pending GUI events, call GLFW callbacks
        glfwPollEvents();
    }

    //free visualization storage
    mjv_freeScene(&scn);
    mjr_freeContext(&con);

    // free MuJoCo model and data, deactivate
    mj_deleteData(d);
    mj_deleteModel(m);
    mj_deactivate();

    // terminate GLFW (crashes with Linux NVidia drivers)
    #if defined(__APPLE__) || defined(_WIN32)
        glfwTerminate();
    #endif

    return 1;
}
