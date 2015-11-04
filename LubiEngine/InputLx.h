#ifndef _INPUTLX_H
#define _INPUTLX_H

#include <Windows.h>

enum
{
	VKey_A = 0x41,
	VKey_B,
	VKey_C,
	VKey_D,
	VKey_E,
	VKey_F,
	VKey_G,
	VKey_H,
	VKey_I,
	VKey_J,
	VKey_K,
	VKey_L,
	VKey_M,
	VKey_N,
	VKey_O,
	VKey_P,
	VKey_Q,
	VKey_R,
	VKey_S,
	VKey_T,
	VKey_U,
	VKey_V,
	VKey_W,
	VKey_X,
	VKey_Y,
	VKey_Z
};

enum
{
	VKey_LeftArrow = 0x25,
	VKey_UpArrow,
	VKey_RightArrow,
	VKey_DownArrow
};

class InputLx
{
public:
	InputLx();
	~InputLx();

	// Only call this once when your Application begins.
	// I may be wrong, but I saw nothing from what I read that indicated
	// devices need to be unregistered.
	bool Initialize();

	// Everytime your MsgProc's uMsg is WM_INPUT, you will want to call the GetData function.
	// I believe this is smarter than calling it every frame in a game loop.
	void GetData(LPARAM lParam);

	// This should be self-explanatory. Depending on which function
	// you call, you will get true or false about the state of the mouse button.
	// The code in the source file may be a bit difficult to follow,
	// but I tried my best to make it match what was described as the best way
	// to be sure the 
	bool IsLeftMouseButtonDown(){ return m_bLMB; }
	bool IsRightMouseButtonDown(){ return m_bRMB; }

	// These functions return values that are relative to the change in position
	// of your mouse.
	int GetMouseChangeX() { return m_nMouseXChange; }
	int GetMouseChangeY() { return m_nMouseYChange; }

	// OPTIONALLY:
	// To obtain exact mouse coords check the uMsg in your Application's MsgProc
	// for WM_MOUSEMOVE, and use HIWORD() LOWORD() functions to extract the mouse X,Y
	// from lParam. Store them in the below variables.
	int GetMouseCoorx() { return m_nMouseX; }
	int GetMouseCoorY() { return m_nMouseY; }

	// Alphabetic as in any letter from the Alphabet. So IsAlphabeticKeyDown returns
	// a value of true or false from an array of 25 booleans. Each index is associated
	// with a position in the english alphabet. Use one of the enumerated values
	// such as VKey_A and subtract 0x41 or make a different enumeration list
	// for all 25 letters and set the first value to 0.
	bool IsAlphabeticKeyDown(int nLetter){ return m_baAlphabet[nLetter]; }

	// For ease of use, I seperated detection of Arrow Keys into their own function.
	// Just like the IsAlphabeticKeyDown function, you can use of the enumerated values
	// such as VKey_LeftArrow and but subtract 0x25 or make a different enumeration list
	// for all 4 arrow directions and set the first value to 0.
	bool IsArrowKeyDown(int nArrowDirection){ return m_baArrows[nArrowDirection]; }

	// If for some reason you need to reset all the detected input values to the state
	// of false use this. Ofcourse this would mean all keys and buttons are not being pressed.
	// I call this once in the constructor to assure the input states are consistent in the beginning.
	void ResetAllKeyData();

private:
	// The CheckKeyPress function is because of these issues:

	// 1. The Mouse
	// The flag is only set once when the button is pressed and so to determine if the mouse button is being held down you need to record it as down
	// until you get a RI_MOUSE_LEFT_BUTTON_UP flag.

	// 2. The Keyboard
	// When a key is held down the raw->data.heyboard.Flags has the RI_KEY_MAKE  bit set (actually none as the value is 0) and when the key is released 
	// the  RI_KEY_BREAK  bit is set (value is 1). 

	// 1 and 2 is what I got directly from ToyMaker's website. I believe for the most part CheckKeyPress takes what was said into account.
	bool CheckKeyPress(bool bLastKeyState, bool bThisKeyState);

private:
	// Two input devices are covered by this class. Mouse and Keyboard.
	// A good reference for them is:
	RAWINPUTDEVICE m_Rid[2];

	// As said on www.toymaker.info the mouse size is 40 and the keyboard 32.
	// So it appears as if the buffer will be filled with either or, not both.
	// Thus, just 40 bytes.
	BYTE m_buffer[40];

	// Should be obvious what functions return these two values.
	int m_nMouseXChange;
	int m_nMouseYChange;

	// Should be obvious what functions return these two values.
	int m_nMouseX;
	int m_nMouseY;

	// Again, should be obvious what functions use these. 
	// LMB = Left Mouse Button. RMB = Right Mouse Button
	bool m_bLMB;
	bool m_bRMB;

	// Notice how I use an array of bools rather than having a separate bool for each.
	// In the source file you will see how my enumerations come into play for knowing what index
	// to access.
	bool m_baAlphabet[25];
	bool m_baArrows[4];
};

#endif