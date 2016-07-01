//  ex0.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "ex2.h"
#include "Model.h"
#include "ShaderIO.h"
#include "OpenMeshObject.h"
#include <limits>

/** Internal Definitions */

#define	WINDOW_SIZE         (600) // initial size of the window               //
#define	WINDOW_POS_X        (100) // initial X position of the window         //
#define	WINDOW_POS_Y        (100) // initial Y position of the window         //

#define RC_OK                 (0) // Everything went ok                       //
#define RC_INVALID_ARGUMENTS  (1) // Invalid arguments given to the program   //
#define RC_INPUT_ERROR        (2) // Invalid input to the program             //

#define	ARGUMENTS_PROGRAM     (0) // program name position on argv            //
#define	ARGUMENTS_INPUTFILE   (1) // given input file position on argv        //
#define	ARGUMENTS_REQUIRED    (2) // number of required arguments             //

/** Key definitions */

#define KEY_ANIMATE         ('a') // Key used to start animation              //
#define KEY_ESC            ('\e') // Key used to terminate the program - ESC  //
#define KEY_QUIT            ('q') // Key used to terminate the program        //
#define KEY_RESET           ('r') // Key used to reset the applied TX's	      //
#define KEY_RELOAD          ('l') // Key used to reload the shaders 	      //
#define KEY_WIREFRAME       ('w') // Key used to toggle wireframe rendering   //
#define KEY_PROJECTION       ('p') // Key used to toggle projection mode prespective or orthographoc   //
#define KEY_TEX_SCALE_I       ('d') // Key used to increase texture scale   //
#define KEY_TEX_SCALE_D       ('s') // Key used to decrease textue scale   //
#define KEY_TURB_COEFF_I       ('g') // Key used to increase texture scale   //
#define KEY_TURB_COEFF_D       ('f') // Key used to decrease textue scale   //
#define KEY_NORMAL       ('n') // Key used to toggle between uniform face normal and avarage   //
#define KEY_PHONG      ('1') // Key used to toggle to phong full model   //
#define KEY_GOURAUD    ('2') // Key used to toggle to grouraud model  //
#define KEY_RGB        ('3') // Key used to toggle to rgb intepolation   //
#define KEY_PLUS_SHINE    ('=') // Key used to increase shine coef   //
#define KEY_MINUS_SHINE        ('-') // Key used to decrease shine coef   //
#define KEY_TEX_SWITCH      ('t') // Key used to toggle between textures   //






/** display callback */
void display(void);

/** window reshape callback  */
void windowResize(int width, int height);

/** keyboard callback  */
void keyboard(unsigned char key, int x, int y);

/** mouse click callback */
void mouse(int button, int state, int x, int y) ;

/** mouse dragging callback  */
void motion(int x, int y) ;

/** timer callback */
void timer(int value) ;

/** Global variables */

int     g_nFPS = 0, g_nFrames = 0;              // FPS and FPS Counter
int     g_dwLastFPS = 0;                        // Last FPS Check Time
bool    g_startAnimation = false;
bool    g_duringAnimation = false;

// A global variable for our model (a better practice would be to use a singletone that holds all model):
Model _model;

/** main function */
int main(int argc, char* argv[])
{
    std::cout << "Starting ex2..." << std::endl;

    // Initialize GLUT
    glutInit(&argc, argv) ;
#ifdef __APPLE__
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE) ;
#else
    glutInitContextVersion(3, 3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glEnable(GL_DEPTH_TEST);
#endif
    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
    glutCreateWindow("CG Ex2 Jadams + Shirelga");

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    int glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
        exit(1);
    }

#ifdef __APPLE__
    GLint sync = 1;
    CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &sync);
#endif

    // Set callback functions:
    glutDisplayFunc(display) ;
    glutReshapeFunc(windowResize) ;
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(100, timer, 0);   // uint millis int value

    // Init anything that can be done once and for all:
    _model.init(argv[1]);

    // Set clear color to black:
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // Start events/drawing loop
    glutMainLoop();

    return 0;
}

void display(void)
{

    // Clear the screen buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Let the model to draw itself...
    _model.draw(polygonMode);

    // Swap those buffers so someone will actually see the results... //
    glutSwapBuffers();
}

// This method is called when the window is resized
void windowResize(int w, int h)
{
    // Update model to fit the new resolution
    _model.resize(w, h);
    int max_axis = std::max(w, h);
    // set the new viewport //
    glViewport((w - max_axis)/2, (h - max_axis)/2, max_axis, max_axis);

    // Refresh the display //
    //glutPostRedisplay();
}

/********************************************************************
 * Function  :	keyboard
 * Arguments :	key : the key that was pressed
 *              x   : x value of the current mouse location
 *              y   : y value of the current mouse location
 * Returns   : n/a
 * Throws    : n/a
 *
 * Purpose   : This function handles all the keyboard input from the user.
 *             It supports terminating the application when the KEY_QUIT is pressed.
 *
 \******************************************************************/
void keyboard(unsigned char key, int x, int y)
{
    unsigned int lower_key = tolower(key);

    switch(lower_key)
    {
        case KEY_RESET:
            _model._open.reset();
            // reset to initial perspective of the object
            // For use in a future exercise
            break;
        case KEY_RELOAD:
            // Reload the shading programs of the object
            // For use in a future exercise
            break;
        case KEY_WIREFRAME:
            // Toggle wireframe mode

            if (polygonMode==GL_FILL) {
                polygonMode=GL_LINE;
            }
            else if(polygonMode==GL_LINE) {
                polygonMode=GL_FILL;
            }
            break;
        case KEY_PROJECTION:
            if(_model._open.get_perspective_mode() == ORTHGONAL)
            {
//                std::cout<<"change to perspective"<<std::endl;
                _model._open.set_perspective_mode(PERSPECTIVE);
            }
            else
            {
//                std::cout<<"change to ortho"<<std::endl;
                _model._open.set_perspective_mode(ORTHGONAL);
            }
            break;
        case KEY_NORMAL:
            if(_model._open.get_normal_mode() == 1)
            {
//                std::cout<<"change to perspective"<<std::endl;
                _model._open.set_normal_mode(0);
            }
            else
            {
//                std::cout<<"change to ortho"<<std::endl;
                _model._open.set_normal_mode(1);
            }
            break;
        case KEY_ANIMATE:
            if (!g_duringAnimation) {
                g_startAnimation = true;
            }
            break;
        case KEY_PHONG:
            _model._open.set_shading_mode(1);
            break;
        case KEY_GOURAUD:
            _model._open.set_shading_mode(2);
            break;
        case KEY_RGB:
            _model._open.set_shading_mode(3);
            break;
        case KEY_PLUS_SHINE:
            _model._open.set_shine(1);
            break;
        case KEY_MINUS_SHINE:
            _model._open.set_shine(0);
            break;
        case KEY_TEX_SCALE_I:
            _model._open.set_texture_scale(1);
            break;
        case KEY_TEX_SCALE_D:
            _model._open.set_texture_scale(0);
            break;
        case KEY_TURB_COEFF_I:
            _model._open.set_trub_coeff(1);
            break;
        case KEY_TURB_COEFF_D:
            _model._open.set_trub_coeff(0);
            break;
        case KEY_TEX_SWITCH:
            _model._open.set_texture();
            break;
        case KEY_QUIT:
        case KEY_ESC:
            // Terminate the program:
            exit(RC_OK);
            break;
        default:
            std::cerr << "Key " << lower_key << " undefined\n";
            break;
    }
    return;
}

/********************************************************************
 * Function  :   mouse
 * Arguments :   button  : the button that was engaged in some action
 *               state   : the new state of that button
 *               x       : x value of the current mouse location
 *               y       : y value of the current mouse location
 * Returns   :   n/a
 * Throws    :   n/a
 *
 * Purpose   :   This function handles mouse actions.
 *
 \******************************************************************/
void mouse(int button, int state, int x, int y)
{

    if(state==GLUT_DOWN) {

        _model._open.update_current_pos(x, y);
        _model._open.update_last_pos(x, y);

        if (button == GLUT_LEFT_BUTTON) {
            _model._open.update_left_down(true);
            //_model._open.update_right_down(false);

        }

        if (button == GLUT_RIGHT_BUTTON) {
            _model._open.update_right_down(true);
            //_model._open.update_left_down(false);
        }

        if (button==GLUT_MIDDLE_BUTTON) {
            //std::cout<<"You pressed the middle button."<<std::endl;
            _model._open.update_middle_down(true);
        }
    }
    else {
        _model._open.update_right_down(false);
        _model._open.update_left_down(false);
        _model._open.update_middle_down(false);
    }
    return;
}


/********************************************************************
 * Function  :   motion
 * Arguments :   x   : x valustd::cout<<"We are currently at ("<<x<<","<<y<<")"<<std::endl;e of the current mouse location
 *               y   : y value of the current mouse location
 * Returns   :   n/a
 * Throws    :   n/a
 *
 * Purpose   :   This function handles mouse dragging events.
 *
 \******************************************************************/
void motion(int x, int y)
{
    if (_model._open.get_right_down()) {
        //std::cout<<"We're inside the moving right button."<<std::endl;
        _model._open.update_current_pos(x,y);
        _model._open.translate();


    }
    if (_model._open.get_left_down()) {
        //std::cout<<"We're inside the moving left button."<<std::endl;
        _model._open.update_current_pos(x, y);
        _model._open.rotate();
    }

    if (_model._open.get_middle_down()) {
        //std::cout<<"We're inside the moving middle button."<<std::endl;
        _model._open.update_current_pos(x,y);
        _model._open.scale();
    }
    //do something

    //this works, do something here.

    return;
}


void timer(int value) {

    glutTimerFunc(25, timer, ++value);   // uint millis int value
    glutPostRedisplay();
}