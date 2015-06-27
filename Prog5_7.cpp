// A bouncing square/ball with trail

// INCLUDES ///////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN  

#include <windows.h>  
#include <windowsx.h>  
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// DEFINES ////////////////////////////////////////////////

// defines for windows 
#define WINDOW_CLASS_NAME "WINCLASS1"
#define WINDOW_WIDTH  400
#define WINDOW_HEIGHT 400

// general defines
#define BALL_RADIUS 20

// GLOBALS ////////////////////////////////////////////////
HWND main_window_handle = NULL; // save the window handle
HPEN blue_pen, black_pen;       // pens to draw with
int dx = 2; // initial velocity of ball               
int dy = 4;
bool trail= false;

// FUNCTIONS //////////////////////////////////////////////
LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
// this is the main message handler of the system
PAINTSTRUCT		ps;		// used in WM_PAINT
HDC				hdc;	// handle to a device context

// what is the message 
switch(msg)
	{	
	case WM_CREATE: 
        {
		// do initialization stuff here
        // create the pens here
        blue_pen  = CreatePen(PS_SOLID,0, RGB(0,0,255));
        black_pen = CreatePen(PS_SOLID,0, RGB(0,0,0));
		return(0);
		} break;

	case WM_PAINT: 
		{
		// simply validate the window
		hdc = BeginPaint(hwnd,&ps);	 
   		EndPaint(hwnd,&ps);
		return(0);
   		} break;

	case WM_DESTROY: 
		{
		// kill the application			

        // delete the pens
        DeleteObject(blue_pen);
        DeleteObject(black_pen);

		PostQuitMessage(0);
		return(0);
		} break;
	case WM_KEYDOWN:
		{
			int virtual_code = (int)wparam;
			int key_bits= (int)lparam;

			switch(virtual_code)
				{
					case VK_LEFT :
						{
							dx--; 
							break; //left arrow
						}
					case VK_RIGHT : 
						{
							dy--;
							break; //right arrow
						}
					case VK_UP : 
						{
							dx++;
							break; //up arrow
						}
					case VK_DOWN : 
						{
							dy++; 
							break; //down arrow
						}
					case 0x54 : //VK_T wasn't working so just used actaull value (check index for others)
						{
							trail=true;
							break;
						}
					default: break;
				}//end switch
		return(0);
		}break;

	default:break;

    } // end switch

// process any messages that we didn't take care of 
return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

// WINMAIN ////////////////////////////////////////////////
int WINAPI WinMain(	HINSTANCE hinstance,
					HINSTANCE hprevinstance,
					LPSTR lpcmdline,
					int ncmdshow)
{

WNDCLASS winclass;	// this will hold the class we create
HWND	 hwnd;		// generic window handle
MSG		 msg;		// generic message
HDC      hdc;       // generic dc
PAINTSTRUCT ps;     // generic paintstruct
RECT     rect;      // generic rectangle
HBRUSH   hbrush;    // generic brush handle

int ball_x = WINDOW_WIDTH/2, // initial position of ball
    ball_y = WINDOW_WIDTH/2;

/*
int dx = 4, // initial velocity of ball               
    dy = 2;
*/
// first fill in the window class stucture
winclass.style			= CS_DBLCLKS | CS_OWNDC | 
                          CS_HREDRAW | CS_VREDRAW;
winclass.lpfnWndProc	= WindowProc;
winclass.cbClsExtra		= 0;
winclass.cbWndExtra		= 0;
winclass.hInstance		= hinstance;
winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
winclass.hbrBackground	= (HBRUSH) GetStockObject(BLACK_BRUSH);
winclass.lpszMenuName	= NULL;
winclass.lpszClassName	= WINDOW_CLASS_NAME;

// register the window class
if (!RegisterClass(&winclass))
	return(0);

// create the window
if (!(hwnd = CreateWindow(WINDOW_CLASS_NAME, // class
						  "Bouncing Ball Screen Saver",	 // title
						  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
					 	  0,0,	   // x,y
						  WINDOW_WIDTH,  // width
                          WINDOW_HEIGHT, // height
						  NULL,	   // handle to parent 
						  NULL,	   // handle to menu
						  hinstance,// instance
						  NULL)))	// creation parms
return(0);

// save the window handle in a global
main_window_handle = hwnd;

// enter main event loop
while(1)
	{
	if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
		// test if this is a quit
        if (msg.message == WM_QUIT)
           break;
	
		// translate any accelerator keys
		TranslateMessage(&msg);

		// send the message to the window proc
		DispatchMessage(&msg);
		} // end if

    // main game processing goes here /////////////////////
    
    // get the graphics device context 
    hdc = GetDC(hwnd);                 //ball_y= 400/20 = 200
                                       //ball_x= 400/20 = 200
    // fill in rect structure          Ball_RADIUS=20
    rect.left  = ball_x - BALL_RADIUS; //180
    rect.right = ball_x + BALL_RADIUS; //220
    rect.top   = ball_y - BALL_RADIUS; //180
    rect.bottom= ball_y + BALL_RADIUS; //220
    
    // erase the ball /////////////////////////////////////
    SelectObject(hdc, GetStockObject(BLACK_BRUSH));
    SelectObject(hdc, black_pen);
    Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
	//i added (start)
	if(trail)
	{
		SelectObject(hdc, blue_pen);
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	}
	//i added (end)

    // move the ball //////////////////////////////////////
    ball_x+=dx; //dx=4 ball_x=204
    ball_y+=dy;	//dy=2 ball_y=202

    // test for collision with wall, if so reverse direction
    if (ball_x >= WINDOW_WIDTH || ball_x <=0)
       {
       // reverse x velocity
       dx=-dx;

       // move ball back
       ball_x+=dx;
       } // end if
    
    if (ball_y >= WINDOW_HEIGHT || ball_y <=0)
       {
       // reverse y velocity
       dy=-dy;

       // move ball back
       ball_y+=dy;
       } // end if

    // fill in rect structure with new position
    rect.left  = ball_x - BALL_RADIUS;
    rect.right = ball_x + BALL_RADIUS;
    rect.top   = ball_y - BALL_RADIUS;
    rect.bottom= ball_y + BALL_RADIUS;
    
    // draw the ball
    SelectObject(hdc, blue_pen);
    Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
    
    // release the device context
    ReleaseDC(hwnd,hdc);

    // slow things down a bit
    Sleep(10);

    } // end while

// return to Windows like this
return(msg.wParam);

} // end WinMain

///////////////////////////////////////////////////////////

