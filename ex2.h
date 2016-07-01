//
//  ex0.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ex0__ex0__
#define __ex0__ex0__

#include <iostream>
#define ORTHGONAL 0
#define PERSPECTIVE 1

GLenum polygonMode=GL_FILL;
int projection_mode = ORTHGONAL;
#endif /* defined(__ex0__ex0__) */
int left_button_down=0;
int right_button_down=0;

int last_mx = 0, last_my = 0, cur_mx = 0, cur_my = 0;
int arcball_on = false;

