#pragma once

#include <stdint.h>

#define DERP_KEY_UNKNOWN   -1
#define DERP_KEY_SPACE   32
#define DERP_KEY_APOSTROPHE   39 /* ' */
#define DERP_KEY_COMMA   44 /* , */
#define DERP_KEY_MINUS   45 /* - */
#define DERP_KEY_PERIOD   46 /* . */
#define DERP_KEY_SLASH   47 /* / */
#define DERP_KEY_0   48
#define DERP_KEY_1   49
#define DERP_KEY_2   50
#define DERP_KEY_3   51
#define DERP_KEY_4   52
#define DERP_KEY_5   53
#define DERP_KEY_6   54
#define DERP_KEY_7   55
#define DERP_KEY_8   56
#define DERP_KEY_9   57
#define DERP_KEY_SEMICOLON   59 /* ; */
#define DERP_KEY_EQUAL   61 /* = */
#define DERP_KEY_A   65
#define DERP_KEY_B   66
#define DERP_KEY_C   67
#define DERP_KEY_D   68
#define DERP_KEY_E   69
#define DERP_KEY_F   70
#define DERP_KEY_G   71
#define DERP_KEY_H   72
#define DERP_KEY_I   73
#define DERP_KEY_J   74
#define DERP_KEY_K   75
#define DERP_KEY_L   76
#define DERP_KEY_M   77
#define DERP_KEY_N   78
#define DERP_KEY_O   79
#define DERP_KEY_P   80
#define DERP_KEY_Q   81
#define DERP_KEY_R   82
#define DERP_KEY_S   83
#define DERP_KEY_T   84
#define DERP_KEY_U   85
#define DERP_KEY_V   86
#define DERP_KEY_W   87
#define DERP_KEY_X   88
#define DERP_KEY_Y   89
#define DERP_KEY_Z   90
#define DERP_KEY_LEFT_BRACKET   91 /* [ */
#define DERP_KEY_BACKSLASH   92 /* \ */
#define DERP_KEY_RIGHT_BRACKET   93 /* ] */
#define DERP_KEY_GRAVE_ACCENT   96 /* ` */
#define DERP_KEY_WORLD_1   161 /* non-US #1 */
#define DERP_KEY_WORLD_2   162 /* non-US #2 */
#define DERP_KEY_ESCAPE   256
#define DERP_KEY_ENTER   257
#define DERP_KEY_TAB   258
#define DERP_KEY_BACKSPACE   259
#define DERP_KEY_INSERT   260
#define DERP_KEY_DELETE   261
#define DERP_KEY_RIGHT   262
#define DERP_KEY_LEFT   263
#define DERP_KEY_DOWN   264
#define DERP_KEY_UP   265
#define DERP_KEY_PAGE_UP   266
#define DERP_KEY_PAGE_DOWN   267
#define DERP_KEY_HOME   268
#define DERP_KEY_END   269
#define DERP_KEY_CAPS_LOCK   280
#define DERP_KEY_SCROLL_LOCK   281
#define DERP_KEY_NUM_LOCK   282
#define DERP_KEY_PRINT_SCREEN   283
#define DERP_KEY_PAUSE   284
#define DERP_KEY_F1   290
#define DERP_KEY_F2   291
#define DERP_KEY_F3   292
#define DERP_KEY_F4   293
#define DERP_KEY_F5   294
#define DERP_KEY_F6   295
#define DERP_KEY_F7   296
#define DERP_KEY_F8   297
#define DERP_KEY_F9   298
#define DERP_KEY_F10   299
#define DERP_KEY_F11   300
#define DERP_KEY_F12   301
#define DERP_KEY_F13   302
#define DERP_KEY_F14   303
#define DERP_KEY_F15   304
#define DERP_KEY_F16   305
#define DERP_KEY_F17   306
#define DERP_KEY_F18   307
#define DERP_KEY_F19   308
#define DERP_KEY_F20   309
#define DERP_KEY_F21   310
#define DERP_KEY_F22   311
#define DERP_KEY_F23   312
#define DERP_KEY_F24   313
#define DERP_KEY_F25   314
#define DERP_KEY_KP_0   320
#define DERP_KEY_KP_1   321
#define DERP_KEY_KP_2   322
#define DERP_KEY_KP_3   323
#define DERP_KEY_KP_4   324
#define DERP_KEY_KP_5   325
#define DERP_KEY_KP_6   326
#define DERP_KEY_KP_7   327
#define DERP_KEY_KP_8   328
#define DERP_KEY_KP_9   329
#define DERP_KEY_KP_DECIMAL   330
#define DERP_KEY_KP_DIVIDE   331
#define DERP_KEY_KP_MULTIPLY   332
#define DERP_KEY_KP_SUBTRACT   333
#define DERP_KEY_KP_ADD   334
#define DERP_KEY_KP_ENTER   335
#define DERP_KEY_KP_EQUAL   336
#define DERP_KEY_LEFT_SHIFT   340
#define DERP_KEY_LEFT_CONTROL   341
#define DERP_KEY_LEFT_ALT   342
#define DERP_KEY_LEFT_SUPER   343
#define DERP_KEY_RIGHT_SHIFT   344
#define DERP_KEY_RIGHT_CONTROL   345
#define DERP_KEY_RIGHT_ALT   346
#define DERP_KEY_RIGHT_SUPER   347
#define DERP_KEY_MENU   348

namespace DERP 
{
	//Entirely static class used for hadling input with basic interface
	class Input 
	{
		friend class Application;
		
	private:
		

		static uint16_t keyHeld[348];
		static uint16_t keyUp[348];
		static uint16_t keyDown[348];
	public:

		static bool getKeyUp(uint8_t key);
		static bool getKeyDown(uint8_t key);
		static bool getKeyHeld(uint8_t key);

		static bool getMouseUp(uint8_t key);
		static bool getMouseDown(uint8_t key);
		static bool getMouseHeld(uint8_t key);
	};
}