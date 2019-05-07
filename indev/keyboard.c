/**
 * @file sdl_kb.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "keyboard.h"
#include "keyboard_sti_indev.h"
#if USE_KEYBOARD

#include "lvgl/lv_core/lv_group.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static uint32_t keycode_to_ascii(uint32_t sdl_key);

/**********************
 *  STATIC VARIABLES
 **********************/
static uint32_t last_key;
static lv_indev_state_t state;

static int gotta_key;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize the keyboard
 */
void keyboard_init(void)
{
    /*Nothing to init*/
}

static IndevMode currentMode;
void set_indev_mode (IndevMode newMode) {
	currentMode = newMode;
}

static sti_keypress_cb sti_keypress_f;
void set_indev_kp_function (sti_keypress_cb f) {
  sti_keypress_f = f;
}

#define LV_GROUP_KEY_UP             17      /*0x11*/
#define LV_GROUP_KEY_DOWN           18      /*0x12*/
#define LV_GROUP_KEY_RIGHT          19      /*0x13*/
#define LV_GROUP_KEY_LEFT           20      /*0x14*/
#define LV_GROUP_KEY_ESC            27      /*0x1B*/
#define LV_GROUP_KEY_DEL            127     /*0x7F*/
#define LV_GROUP_KEY_BACKSPACE      8       /*0x08*/
#define LV_GROUP_KEY_ENTER          10      /*0x0A, '\n'*/
#define LV_GROUP_KEY_NEXT           9       /*0x09, '\t'*/
#define LV_GROUP_KEY_PREV           11      /*0x0B, '*/


static void * keyname (uint32_t k) {
  switch (k) {
    case LV_GROUP_KEY_UP:
      return strdup("up");
    case LV_GROUP_KEY_DOWN:
      return strdup("down");
    case LV_GROUP_KEY_RIGHT:
      return strdup("right");
    case LV_GROUP_KEY_LEFT:
      return strdup("left");
    case LV_GROUP_KEY_ESC:
      return strdup("esc");
    case LV_GROUP_KEY_DEL:
      return strdup("del");
    case LV_GROUP_KEY_BACKSPACE:
      return strdup("backspace");
    case LV_GROUP_KEY_ENTER:
      return strdup("enter");
    case LV_GROUP_KEY_NEXT:
      return strdup("next");
    case LV_GROUP_KEY_PREV:
      return strdup("prev");

    default: {
      char buf[40];
      sprintf (buf, "0x%x", k);
      return strdup(buf);
    }
  }
}

/**
 * Get the last pressed or released character from the PC's keyboard
 * @param data store the read data here
 * @return false: because the points are not buffered, so no more data to be read
 */
bool keyboard_read(lv_indev_data_t * data)
{
  if (gotta_key) {
    data->state = state;
    data->key = keycode_to_ascii(last_key);
    gotta_key = false;

    #if 1
    if (state) {
      char * k1 = keyname(last_key);
      char * k2 = keyname(data->key);
      printf("keyboard.c state: %d, key: %s -> %s\n", data->state, k1, k2);
      free (k1);
      free (k2);
    }
    #endif
    if (sti_keypress_f) {
      (*sti_keypress_f) (data);
    }
  } else {
    data->state = LV_INDEV_STATE_REL;
    data->key = 0;
  }

    return false;       /*No more data to read so return false*/
}

/**
 * It is called periodically from the SDL thread to check a key is pressed/released
 * @param event describes the event
 */
void keyboard_handler(SDL_Event * event)
{
    /* We only care about SDL_KEYDOWN and SDL_KEYUP events */
    switch(event->type) {
        case SDL_KEYDOWN:                       /*Button press*/
            last_key = event->key.keysym.sym;   /*Save the pressed key*/
            state = LV_INDEV_STATE_PR;          /*Save the key is pressed now*/
            gotta_key = true;
            break;
        case SDL_KEYUP:                         /*Button release*/
            state = LV_INDEV_STATE_REL;         /*Save the key is released but keep the last key*/
            gotta_key = true;
            break;
        default:
            break;

    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Convert the key code LV_GROUP_KEY_... "codes" or leave them if they are not control characters
 * @param sdl_key the key code
 * @return
 */
static uint32_t keycode_to_ascii(uint32_t sdl_key)
{
    /*Remap some key to LV_GROUP_KEY_... to manage groups*/
    switch(sdl_key) {
        case SDLK_RIGHT:
        	if (currentMode == INDEV_MODE_NORMAL || currentMode == INDEV_MODE_BUTTON_LIST) {
        		return LV_GROUP_KEY_NEXT;
        	} else {
            	return LV_GROUP_KEY_RIGHT;
        	}

        case SDLK_KP_PLUS:
            return LV_GROUP_KEY_RIGHT;

        case SDLK_LEFT:
          if (currentMode == INDEV_MODE_NORMAL || currentMode == INDEV_MODE_BUTTON_LIST) {
        		return LV_GROUP_KEY_PREV;
        	} else {
            	return LV_GROUP_KEY_LEFT;
        	}

        case SDLK_KP_MINUS:
            return LV_GROUP_KEY_LEFT;

        case SDLK_DOWN:
          if (currentMode == INDEV_MODE_BUTTON_LIST) {
            return LV_GROUP_KEY_NEXT;
          } else {
            return LV_GROUP_KEY_DOWN;
          }

        case SDLK_UP:
          if (currentMode == INDEV_MODE_BUTTON_LIST) {
            return LV_GROUP_KEY_PREV;
          } else {
            return LV_GROUP_KEY_UP;
          }

        case SDLK_ESCAPE:
            return LV_GROUP_KEY_ESC;

#ifdef  LV_GROUP_KEY_BACKSPACE        /*For backward compatibility*/
        case SDLK_BACKSPACE:
            return LV_GROUP_KEY_BACKSPACE;
#endif

#ifdef  LV_GROUP_KEY_DEL        /*For backward compatibility*/
        case SDLK_DELETE:
            return LV_GROUP_KEY_DEL;
#endif
        case SDLK_KP_ENTER:
        case '\r':
            return LV_GROUP_KEY_ENTER;

        default:
            return sdl_key;
    }
}
#endif
