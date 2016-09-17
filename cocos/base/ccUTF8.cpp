/****************************************************************************
 Copyright (c) 2014 cocos2d-x.org
 Copyright (c) 2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "base/ccUTF8.h"
#include "platform/CCCommon.h"
#include "base/CCConsole.h"

NS_CC_BEGIN

namespace StringUtils {

/*
 * @str:    the string to search through.
 * @c:        the character to not look for.
 *
 * Return value: the index of the last character that is not c.
 * */
unsigned int getIndexOfLastNotChar16(const std::vector<char16_t>& str, char16_t c)
{
    int len = static_cast<int>(str.size());

    int i = len - 1;
    for (; i >= 0; --i)
        if (str[i] != c) return i;

    return i;
}

/*
 * @str:    the string to trim
 * @index:    the index to start trimming from.
 *
 * Trims str st str=[0, index) after the operation.
 *
 * Return value: the trimmed string.
 * */
static void trimUTF16VectorFromIndex(std::vector<char16_t>& str, int index)
{
    int size = static_cast<int>(str.size());
    if (index >= size || index < 0)
        return;

    str.erase(str.begin() + index, str.begin() + size);
}

/*
 * @ch is the unicode character whitespace?
 *
 * Reference: http://en.wikipedia.org/wiki/Whitespace_character#Unicode
 *
 * Return value: weather the character is a whitespace character.
 * */
bool isUnicodeSpace(char16_t ch)
{
    return  (ch >= 0x0009 && ch <= 0x000D) || ch == 0x0020 || ch == 0x0085 || ch == 0x00A0 || ch == 0x1680
    || (ch >= 0x2000 && ch <= 0x200A) || ch == 0x2028 || ch == 0x2029 || ch == 0x202F
    ||  ch == 0x205F || ch == 0x3000;
}

bool isCJKUnicode(char16_t ch)
{
    return (ch >= 0x4E00 && ch <= 0x9FBF)   // CJK Unified Ideographs
        || (ch >= 0x2E80 && ch <= 0x2FDF)   // CJK Radicals Supplement & Kangxi Radicals
        || (ch >= 0x2FF0 && ch <= 0x30FF)   // Ideographic Description Characters, CJK Symbols and Punctuation & Japanese
        || (ch >= 0x3100 && ch <= 0x31BF)   // Korean
        || (ch >= 0xAC00 && ch <= 0xD7AF)   // Hangul Syllables
        || (ch >= 0xF900 && ch <= 0xFAFF)   // CJK Compatibility Ideographs
        || (ch >= 0xFE30 && ch <= 0xFE4F)   // CJK Compatibility Forms
        || (ch >= 0x31C0 && ch <= 0x4DFF);  // Other exiensions
}

void trimUTF16Vector(std::vector<char16_t>& str)
{
    int len = static_cast<int>(str.size());

    if ( len <= 0 )
        return;

    int last_index = len - 1;

    // Only start trimming if the last character is whitespace..
    if (isUnicodeSpace(str[last_index]))
    {
        for (int i = last_index - 1; i >= 0; --i)
        {
            if (isUnicodeSpace(str[i]))
                last_index = i;
            else
                break;
        }

        trimUTF16VectorFromIndex(str, last_index);
    }
}

std::vector<char16_t> getChar16VectorFromUTF16String(const std::u16string& utf16)
{
    std::vector<char16_t> ret;
    size_t len = utf16.length();
    ret.reserve(len);
    for (size_t i = 0; i < len; ++i)
    {
        ret.push_back(utf16[i]);
    }
    return ret;
}

std::string format(const char* format, ...)
{
#define CC_MAX_STRING_LENGTH (1024*100)

	std::string ret;

	va_list ap;
	va_start(ap, format);

	char* buf = (char*)malloc(CC_MAX_STRING_LENGTH);
	if (buf != nullptr)
	{
		vsnprintf(buf, CC_MAX_STRING_LENGTH, format, ap);
		ret = buf;
		free(buf);
	}
	va_end(ap);

	return ret;
}

} //namespace StringUtils {


int cc_wcslen(const unsigned short* str)
{
    if (str == nullptr)
        return -1;
    int i=0;
    while(*str++) i++;
    return i;
}

void cc_utf8_trim_ws(std::vector<unsigned short>* str)
{
    if (str == nullptr)
        return;
    // unsigned short and char16_t are both 2 bytes
    std::vector<char16_t>* ret = reinterpret_cast<std::vector<char16_t>*>(str);
    StringUtils::trimUTF16Vector(*ret);
}

bool isspace_unicode(unsigned short ch)
{
    return StringUtils::isUnicodeSpace(ch);
}


bool iscjk_unicode(unsigned short ch)
{
    return StringUtils::isCJKUnicode(ch);
}


unsigned int cc_utf8_find_last_not_char(const std::vector<unsigned short>& str, unsigned short c)
{
    std::vector<char16_t> char16Vector;
    for (const auto& e : str)
    {
        char16Vector.push_back(e);
    }

    return StringUtils::getIndexOfLastNotChar16(char16Vector, c);
}

std::vector<unsigned short> cc_utf16_vec_from_utf16_str(const unsigned short* str)
{
    std::vector<unsigned short> str_new;

    if (str == nullptr)
        return str_new;

    int len = cc_wcslen(str);

    for (int i = 0; i < len; ++i)
    {
        str_new.push_back(str[i]);
    }
    return str_new;
}

NS_CC_END
