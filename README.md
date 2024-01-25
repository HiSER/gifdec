### Original repository
```
https://github.com/lecram/gifdec
```

### MSVC WinCE Example
```c++
void drawGIF(const char* filename)
{
    auto gif = new GIFDecode(filename);
    if (!gif) return;
    gif->start(hdc, 100, 100);
    do
    {
        if (gif->isEOF()) gif->rewind();
        while (!stopped && gif->getFrame())
        {
            gif->draw();
            Sleep(gif->getNextDelay());
        }
    } while (!stopped && gif->isEOF());
    gif->release();
    delete gif;
}
```
