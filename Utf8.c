#include <stddef.h>
#include "MinUnit.h"
#include "Utf8.h"

#define CHAR_AS_ULONG(c) ((unsigned long)(*(const unsigned char *)&(c)))

const char* ReadUtf8Character(const char* str, long *codepoint, unsigned long *codepointUtf8) {
    if (!str || !*str) {
        return NULL;
    }

    // Check the leading byte to determine the number of bytes in the UTF-8 character
    if ((str[0] & 0x80) == 0x00) {
        // 1-byte character (U+0000 to U+007F)
        *codepoint = (long)str[0];
        *codepointUtf8 = (unsigned long)str[0];
        return str + 1;
    } else if ((str[0] & 0xE0) == 0xC0) {
        // 2-byte character (U+0080 to U+07FF)
        if (!str[1]) {
            return NULL;
        }

        *codepoint = ((long)str[0] & 0x1F) << 6 | ((long)str[1] & 0x3F);
        *codepointUtf8 = (CHAR_AS_ULONG(str[0]) << 8) | CHAR_AS_ULONG(str[1]);

        return str + 2;
    } else if ((str[0] & 0xF0) == 0xE0) {
        // 3-byte character (U+0800 to U+FFFF)
        if (!str[1] || !str[2]) {
            return NULL;
        }

        *codepoint = ((long)str[0] & 0x0F) << 12 | ((long)str[1] & 0x3F) << 6 | ((long)str[2] & 0x3F);
        *codepointUtf8 = (CHAR_AS_ULONG(str[0]) << 16) | (CHAR_AS_ULONG(str[1]) << 8) | CHAR_AS_ULONG(str[2]);

        return str + 3;
    } else if ((str[0] & 0xF8) == 0xF0) {
        // 4-byte character (U+10000 to U+10FFFF)
        if (!str[1] || !str[2] || !str[3]) {
            return NULL;
        }

        *codepoint = ((long)str[0] & 0x07) << 18 | ((long)str[1] & 0x3F) << 12 | ((long)str[2] & 0x3F) << 6 | ((long)str[3] & 0x3F);
        *codepointUtf8 = (CHAR_AS_ULONG(str[0]) << 24) | (CHAR_AS_ULONG(str[1]) << 16) | (CHAR_AS_ULONG(str[2]) << 8) | CHAR_AS_ULONG(str[3]);

        return str + 4;
    } else {
        // Invalid UTF-8 character
        return str + 1;
    }
}

const char *Utf8_UnitTest(void) {
    const char *str = "X\xD0\xAF"; // X and cyrillic letter Ya
    long codepoint;
    unsigned long codepointUtf8;
    const char *next;

    next = ReadUtf8Character(str, &codepoint, &codepointUtf8);
    long expectedCodepoint1 = 0x58;
    unsigned long expectedCodepointUtf81 = 'X';
    mu_assert("ReadUtf8Character failed, next == str + 1", next == str + 1);
    mu_assert("ReadUtf8Character failed, codepoint == 0x58", codepoint == expectedCodepoint1);
    mu_assert("ReadUtf8Character failed, codepointUtf8 == 'X'", codepointUtf8 == expectedCodepointUtf81);

    next = ReadUtf8Character(next, &codepoint, &codepointUtf8);
    long expectedCodepoint2 = 0x42F;
    unsigned long expectedCodepointUtf82 = 0xD0AF;
    mu_assert("ReadUtf8Character failed, next == str + 3", next == str + 3);
    mu_assert("ReadUtf8Character failed, codepoint == 0x42F", codepoint == expectedCodepoint2);
    mu_assert("ReadUtf8Character failed, codepointUtf8 == 0xD0AF", codepointUtf8 == expectedCodepointUtf82);

    return NULL;
}
