#ifndef KEYBOARD_STI_INDEV_H
#define KEYBOARD_STI_INDEV_H
/**********************
 *      TYPEDEFS
 **********************/
typedef enum indevMode { INDEV_MODE_NORMAL, INDEV_MODE_KB } IndevMode;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void set_indev_mode (IndevMode newMode);
#endif
