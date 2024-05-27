#ifndef MINUI_UTF8_H
#define MINUI_UTF8_H

const char* ReadUtf8Character(const char* str, long *codepoint, unsigned long *codepointUtf8);

const char *Utf8_UnitTest(void);

#endif
