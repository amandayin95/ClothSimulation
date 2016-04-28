//
//  Control.hpp
//  ClothSimulation
//
//  Created by Amanda Yin on 4/27/16.
//  Copyright © 2016 Amanda Yin. All rights reserved.
//

#ifndef Control_hpp
#define Control_hpp

// init calls
int make_menu();

// glut callbacks
void menu_func (int value);
void keyboard_func (unsigned char key, int x, int y);
void special_keyboard_func (int key, int x, int y);
void mouse_click_func (int button, int state, int x, int y);
void mouse_move_func (int x, int y);


// menu calls
void menu_help ();
void reset_globals();

// mouse calls
void mouse_rotate_camera (int dx, int dy);
void mouse_translate_camera (int dx, int dy);
void zoom(bool zoomIn);


#endif /* Control_hpp */
