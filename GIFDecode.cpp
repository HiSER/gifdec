#include "GIFDecode.hpp"

GIFDecode::GIFDecode(const char* filename)
{
#ifdef _MSC_VER
	main_dc = nullptr;
#endif
	gif = gd_open_gif(filename);
    if (!isLoaded())
	{
		TRACE_ERROR("GIFDecode: open '%s' error", filename);
		return;
	}
	frame = (tRGB*)malloc(sizeof(tRGB)* getWidth() * getHeight());
	if (frame == nullptr)
	{
		TRACE_ERROR("GIFDecode: malloc frame error");
		abort();
	}
	frame32 = (tRGBA*)malloc(sizeof(tRGBA)* getWidth() * getHeight());
	if (frame32 == nullptr)
	{
		TRACE_ERROR("GIFDecode: malloc frame32 error");
		abort();
	}
	frame_status = FrameError;
#ifdef _MSC_VER
	tick = 0;
#endif
}

GIFDecode::~GIFDecode()
{
	if (isLoaded())
	{
#ifdef _MSC_VER
		release();
#endif
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
void GIFDecode::start(HDC hdc, int x, int y)
{
	main_dc = hdc;
	main_x = x;
	main_y = y;

	BITMAPINFO bmp_info;
	memset(&bmp_info, 0, sizeof(bmp_info));
	bmp_info.bmiHeader.biSize = sizeof(bmp_info.bmiHeader);
	bmp_info.bmiHeader.biWidth = getWidth();
	bmp_info.bmiHeader.biHeight = getHeight();
	bmp_info.bmiHeader.biPlanes = 1;
	bmp_info.bmiHeader.biBitCount = 32;
	bmp_info.bmiHeader.biCompression = BI_RGB;

	bg_dc = CreateCompatibleDC(main_dc);
	bg_bmp = CreateCompatibleBitmap(main_dc, getWidth(), getHeight());
	old_bg_bmp = SelectObject(bg_dc, bg_bmp);

	anim_dc = CreateCompatibleDC(main_dc);
	anim_bmp = CreateDIBSection(anim_dc, &bmp_info, DIB_RGB_COLORS, (void**)&bmp_bits, NULL, 0);
	old_anim_bmp = SelectObject(anim_dc, anim_bmp);

	BitBlt(bg_dc, 0, 0, getWidth(), getHeight(), main_dc, main_x, main_y, SRCCOPY);
}

void GIFDecode::release()
{
	if (isStarted())
	{
		BitBlt(main_dc, main_x, main_y, getWidth(), getHeight(), bg_dc, 0, 0, SRCCOPY);
		SelectObject(anim_dc, old_anim_bmp);
		DeleteObject(anim_bmp);
		DeleteObject(anim_dc);
		SelectObject(bg_dc, old_bg_bmp);
		DeleteObject(bg_bmp);
		DeleteObject(bg_dc);
		main_dc = nullptr;
	}
}

void GIFDecode::draw()
{
	tRGB* pIN;
	tRGBA* pOUT;
	int iY, oY, bX, bY;
	gd_render_frame(gif, (uint8_t*)frame);
	BitBlt(anim_dc, 0, 0, getWidth(), getHeight(), bg_dc, 0, 0, SRCCOPY);
	for (bY = 0; bY < getHeight(); bY++)
	{
		iY = getWidth() * bY;
		oY = getWidth() * (getHeight() - bY - 1);
		for (bX = 0; bX < getWidth(); bX++)
		{
			pIN = &frame[iY + bX];
			pOUT = &bmp_bits[oY + bX];
			if (!gd_is_bgcolor(gif, pIN->raw))
			{
				pOUT->blue = pIN->rgb.red;
				pOUT->green = pIN->rgb.green;
				pOUT->red = pIN->rgb.blue;
			}
			pOUT->alpha = 0xFF;
		}
	}
	BitBlt(main_dc, main_x, main_y, getWidth(), getHeight(), anim_dc, 0, 0, SRCCOPY);
}
#endif

#ifdef _MSC_VER
int GIFDecode::getNextDelay()
{
	DWORD delta = GetTickCount() - tick;
	DWORD delay = 10U * gif->gce.delay;
	if (delta >= delay)
		return 1;
	else
		return delay - delta;
}
#else
int GIFDECODE::getNextDelay()
{
	// TODO: other platforms
	return 10;
}
#endif
