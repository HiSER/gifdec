#include "GIFDecode.hpp"

GIFDecode::GIFDecode(const char* filename)
{
	gif = gd_open_gif(filename);
	if (!this)
		TRACE_ERROR("GIFDEC: open '%s' error", filename);
	else
	{
		frame = (tRGB*)malloc(sizeof(tRGB)* getWidth() * getHeight());
		if (frame == nullptr)
		{
			TRACE_ERROR("GIFDEC: malloc frame error");
			abort();
		}
		frame32 = (tRGBA*)malloc(sizeof(tRGBA)* getWidth() * getHeight());
		if (frame32 == nullptr)
		{
			TRACE_ERROR("GIFDEC: malloc frame32 error");
			abort();
		}
#ifdef _MSC_VER
		memset(&bmp_info, 0, sizeof(bmp_info));
		bmp_info.bmiHeader.biSize = sizeof(bmp_info.bmiHeader);
		bmp_info.bmiHeader.biWidth = getWidth();
		bmp_info.bmiHeader.biHeight = getHeight();
		bmp_info.bmiHeader.biPlanes = 1;
		bmp_info.bmiHeader.biBitCount = 32;
		bmp_info.bmiHeader.biCompression = BI_RGB;
		tick = 0;
#endif
	}
	frame_status = FrameError;
}

GIFDecode::~GIFDecode()
{
	if (this)
	{
		free(frame32);
		free(frame);
		gd_close_gif(gif);
	}
}

bool GIFDecode::getFrame()
{
#ifdef _MSC_VER
	tick = GetTickCount();
#endif
	frame_status = (eFrameStatus)gd_get_frame(gif);
	return frame_status == FrameAvailable;
}

#ifdef _MSC_VER
// TODO: supporting transparent
void GIFDecode::drawFrame(HDC hdc, int x, int y)
{
	tRGB* rgb;
	tRGBA* rgba;
	gd_render_frame(gif, (uint8_t*)frame);
	for (int y = 0; y < getHeight(); y++)
	{
		int iY = getWidth() * y;
		int nY = getWidth() * (getHeight() - y - 1);
		for (int x = 0; x < getWidth(); x++)
		{
			rgb = &frame[iY + x];
			rgba = &frame32[nY + x];
			rgba->blue = rgb->red;
			rgba->green = rgb->green;
			rgba->red = rgb->blue;
			rgba->alpha = 0xFF;
		}
	}
	SetDIBitsToDevice(hdc, x, y, getWidth(), getHeight(), 0, 0, 0, getHeight(), frame32, &bmp_info, DIB_RGB_COLORS);
}
#endif

#ifdef _MSC_VER
int GIFDecode::getNextDelay()
{
	DWORD delta = GetTickCount() - tick;
	DWORD delay = 10U * gif->gce.delay;
	if (delta >= delay)
		return 0;
	else
		return delay - delta;
}
#else
int GIFDECODE::getNextDelay()
{
	// TODO: other platforms
	return 0;
}
#endif
