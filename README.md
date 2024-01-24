### Original repository
```
https://github.com/lecram/gifdec
```

### MSVC Example
```c++
void drawGIF(const char* filename)
{
    auto gif = new GIFDecode(filename);
    if (!gif) return;

    HDC dc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, gif->getWidth(), gif->getHeight());
    HGDIOBJ old_bmp = SelectObject(dc, bmp);

    do
    {
        if (gif->isEOF()) gif->rewind();
        while (!stopped && gif->getFrame())
        {
            gif->drawFrame(dc);
            BitBlt(hdc, 0, 0, gif->getWidth(), gif->getHeight(), dc, 0, 0, SRCCOPY);
            Sleep(gif->getNextDelay());
        }
    } while (!stopped && gif->isEOF());

    SelectObject(dc, old_bmp);
    DeleteObject(bmp);
    DeleteObject(dc);
    delete gif;
}
```
