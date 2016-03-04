#include <bteifgl.h>

#ifdef _WIN32

#include <windows.h>

int	mouse_lx, mouse_ly, mouse_lb;
POINT	mouse_pos;

extern int window_center_x, window_center_y;
extern int window_mouse_buttons;

extern int window_width;
extern int window_height;


BTEIFGL_API int GfxDrv_MouseGetRelPos(int *dx, int *dy, int *mb)
{
	int x, y, b;

	GetCursorPos (&mouse_pos);
//	SetCursorPos (window_center_x, window_center_y);
	x=mouse_pos.x-window_center_x;
	y=mouse_pos.y-window_center_y;

	b=window_mouse_buttons;

//	if ((x<=-400) || (x>=400) || (y<=-300) || (y>=300))
	if ((x<=-(window_width/2)) || (x>=(window_width/2)) ||
		(y<=-(window_height/2)) || (y>=(window_height/2)))
	{
		x=mouse_lx;
		y=mouse_ly;
//		b=mouse_lb;
	}

	*dx=x-mouse_lx;
	*dy=y-mouse_ly;
	mouse_lx=x;
	mouse_ly=y;
	mouse_lb=b;

	*mb=b&7;

	return(0);
}

BTEIFGL_API int GfxDrv_MouseGetPos(int *mx, int *my, int *mb)
{
	int x, y, b;

	GetCursorPos (&mouse_pos);
//	SetCursorPos (window_center_x, window_center_y);
	x=mouse_pos.x-window_center_x;
	y=mouse_pos.y-window_center_y;

	b=window_mouse_buttons;

//	if ((x<=-400) || (x>=400) || (y<=-300) || (y>=300))
	if ((x<=-(window_width/2)) || (x>=(window_width/2)) ||
		(y<=-(window_height/2)) || (y>=(window_height/2)))
	{
		x=mouse_lx;
		y=mouse_ly;
//		b=mouse_lb;
	}

	*mx=x;
	*my=y;
	mouse_lx=x;
	mouse_ly=y;
	mouse_lb=b;

	*mb=b&7;

	return(0);
}

BTEIFGL_API int GfxDrv_MouseSetPos(int mx, int my)
{
	int x, y, b;

	x=window_center_x+mx;
	y=window_center_y+my;

	SetCursorPos (x, y);
	mouse_lx = mx;
	mouse_ly = my;

	return(0);
}

#endif

