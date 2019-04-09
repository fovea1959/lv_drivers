#ifndef KEYBOARD_STI_INDEV_H
#define KEYBOARD_STI_INDEV_H
/**********************
 *      TYPEDEFS
 **********************/
typedef enum indevMode { INDEV_MODE_NORMAL, INDEV_MODE_KB, INDEV_MODE_BUTTON_LIST } IndevMode;
typedef void (*sti_keypress_cb)(uint32_t k);

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void set_indev_mode (IndevMode newMode);
void set_indev_kp_function (sti_keypress_cb f);
#endif
