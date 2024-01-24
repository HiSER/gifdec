#ifndef __GIFDECODE_HPP
#define __GIFDECODE_HPP

#include "lib/gifdec.h"
#ifdef _MSC_VER
#include "Windows.h"
#endif


class GIFDecode
{
public:


	explicit GIFDecode(const char* filename);
	~GIFDecode();

	operator bool() { return gif != nullptr; }

	int getWidth() { return gif->width; }
	int getHeight() { return gif->height; }

	bool getFrame();
	bool isEOF() { return frame_status == FrameEOF; }

	void rewind() { gd_rewind(gif); }

	int getNextDelay();

#ifdef _MSC_VER
	void drawFrame(HDC hdc, int x = 0, int y = 0);
#else
	// TODO: other platforms
#endif

#ifdef _MSC_VER
#include <pshpack1.h>
	typedef struct
	{
		uint8_t blue;
		uint8_t green;
		uint8_t red;
	} tRGB;
	typedef struct
	{
		uint8_t blue;
		uint8_t green;
		uint8_t red;
		uint8_t alpha;
	} tRGBA;
#include <poppack.h>
#else
	typedef struct __attribute__((aligned(1)))
	{
		uint8_t blue;
		uint8_t green;
		uint8_t red;
	} tRGB;
	typedef struct __attribute__((aligned(1)))
	{
		uint8_t blue;
		uint8_t green;
		uint8_t red;
		uint8_t alpha;
	} tRGBA;
#endif

private:

	typedef enum
	{
		FrameError = -1,
		FrameEOF = 0,
		FrameAvailable = 1
	} eFrameStatus;

	gd_GIF* gif;
	tRGB* frame;
	tRGBA* frame32;
	eFrameStatus frame_status;

#ifdef _MSC_VER
	BITMAPINFO bmp_info;
	DWORD tick;
#endif

};

#endif /* __GIFDECODE_HPP */
