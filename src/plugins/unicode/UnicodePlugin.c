/* Automatically generated from Squeak on (1 August 2008 6:20:31 pm) */

#if defined(WIN32) || defined(_WIN32) || defined(Win32)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif /* WIN32 */

#include "sqVirtualMachine.h"

/* memory access macros */
#define byteAt(i) (*((unsigned char *) (i)))
#define byteAtput(i, val) (*((unsigned char *) (i)) = val)
#define longAt(i) (*((int *) (i)))
#define longAtput(i, val) (*((int *) (i)) = val)

int unicodeClipboardGet(unsigned short *utf16, int utf16Length);
void unicodeClipboardPut(unsigned short *utf16, int utf16Length);
int unicodeClipboardSize(void);
void unicodeDrawString(char *utf8, int utf8Length, int *wPtr, int *hPtr, unsigned int *bitmapPtr);
int unicodeGetFontList(char *str, int strLength);
int unicodeGetXRanges(char *utf8, int utf8Length, int *resultPtr, int resultLength);
void unicodeMeasureString(char *utf8, int utf8Length, int *wPtr, int *hPtr);
void unicodeSetColors(int fgRed, int fgGreen, int fgBlue, int bgRed, int bgGreen, int bgBlue, int mapBGToTransparent);
void unicodeSetFont(char *fontName, int fontSize, int boldFlag, int italicFlag, int antiAliasFlag);

/*** Variables ***/
struct VirtualMachine* interpreterProxy;
const char *moduleName = "UnicodePlugin 1 August 2008 (e)";

/*** Functions ***/
static char * asCString(int stringOop);
static void * cWordsPtrminSize(int oop, int minSize);
static int copyStringintomax(int stringOop, char *stringPtr, int maxChars);
EXPORT int primitiveClipboardGet(void);
EXPORT int primitiveClipboardPut(void);
EXPORT int primitiveClipboardSize(void);
EXPORT int primitiveDrawString(void);
EXPORT int primitiveGetFontList(void);
EXPORT int primitiveGetXRanges(void);
EXPORT int primitiveMeasureString(void);
EXPORT int primitiveSetColors(void);
EXPORT int primitiveSetFont(void);
EXPORT int setInterpreter(struct VirtualMachine* anInterpreter);

static char * asCString(int stringOop) {
	if (((stringOop & 1)) || (!(interpreterProxy->isBytes(stringOop)))) {
		interpreterProxy->success(0);
		return 0;
	}
	return ((char *) (interpreterProxy->firstIndexableField(stringOop)));
}

static void * cWordsPtrminSize(int oop, int minSize) {
	interpreterProxy->success((!((oop & 1))) && ((interpreterProxy->isWords(oop)) && ((interpreterProxy->stSizeOf(oop)) >= minSize)));
	if (interpreterProxy->failed()) {
		return 0;
	}
	return ((void *) (interpreterProxy->firstIndexableField(oop)));
}

static int copyStringintomax(int stringOop, char *stringPtr, int maxChars) {
	char *srcPtr;
	int count;
	int i;

	if (((stringOop & 1)) || (!(interpreterProxy->isBytes(stringOop)))) {
		interpreterProxy->success(0);
		return 0;
	}
	count = interpreterProxy->stSizeOf(stringOop);
	if (!(count < maxChars)) {
		interpreterProxy->success(0);
		return 0;
	}
	srcPtr = ((char *) (interpreterProxy->firstIndexableField(stringOop)));
	for (i = 1; i <= count; i += 1) {
		*stringPtr++ = *srcPtr++;
	}
	*stringPtr = 0;
	return 0;
}

EXPORT int primitiveClipboardGet(void) {
	unsigned short *utf16;
	int utf16Length;
	int count;
	int utf16Oop;

	utf16Oop = interpreterProxy->stackValue(0);
	if (((utf16Oop & 1)) || (!(interpreterProxy->isWords(utf16Oop)))) {
		interpreterProxy->success(0);
	}
	if (interpreterProxy->failed()) {
		return 0;
	}
	utf16 = ((unsigned short *) (interpreterProxy->firstIndexableField(utf16Oop)));
	utf16Length = 2 * (interpreterProxy->stSizeOf(utf16Oop));
	count = unicodeClipboardGet(utf16, utf16Length);
	interpreterProxy->popthenPush(2, ((count << 1) | 1));
	return 0;
}

EXPORT int primitiveClipboardPut(void) {
	unsigned short *utf16;
	int utf16Length;
	int strOop;
	int count;

	strOop = interpreterProxy->stackValue(1);
	count = interpreterProxy->stackIntegerValue(0);
	if (((strOop & 1)) || (!(interpreterProxy->isWords(strOop)))) {
		interpreterProxy->success(0);
	}
	if (interpreterProxy->failed()) {
		return 0;
	}
	utf16 = ((unsigned short *) (interpreterProxy->firstIndexableField(strOop)));
	utf16Length = 2 * (interpreterProxy->stSizeOf(strOop));
	if ((count >= 0) && (count < utf16Length)) {
		utf16Length = count;
	}
	unicodeClipboardPut(utf16, utf16Length);
	interpreterProxy->pop(2);
	return 0;
}

EXPORT int primitiveClipboardSize(void) {
	int count;

	count = unicodeClipboardSize();
	interpreterProxy->popthenPush(1, ((count << 1) | 1));
	return 0;
}

EXPORT int primitiveDrawString(void) {
	int bitmapOop;
	void *bitmapPtr;
	int result;
	int h;
	int utf8Oop;
	int w;
	int utf8Length;
	char *utf8;

	utf8Oop = interpreterProxy->stackValue(3);
	utf8 = asCString(utf8Oop);
	w = interpreterProxy->stackIntegerValue(2);
	h = interpreterProxy->stackIntegerValue(1);
	bitmapOop = interpreterProxy->stackValue(0);
	bitmapPtr = cWordsPtrminSize(bitmapOop, w * h);
	if (interpreterProxy->failed()) {
		return 0;
	}
	utf8Length = interpreterProxy->stSizeOf(utf8Oop);
	unicodeDrawString(utf8, utf8Length, &w, &h, bitmapPtr);
	result = interpreterProxy->makePointwithxValueyValue(w, h);
	interpreterProxy->popthenPush(5, result);
	return 0;
}

EXPORT int primitiveGetFontList(void) {
	int strOop;
	char *str;
	int count;
	int strLength;

	strOop = interpreterProxy->stackValue(0);
	str = asCString(strOop);
	if (interpreterProxy->failed()) {
		return 0;
	}
	strLength = interpreterProxy->stSizeOf(strOop);
	count = unicodeGetFontList(str, strLength);
	interpreterProxy->popthenPush(2, ((count << 1) | 1));
	return 0;
}

EXPORT int primitiveGetXRanges(void) {
	int count;
	int utf8Oop;
	int resultOop;
	int *resultPtr;
	int utf8Length;
	int resultLength;
	char *utf8;

	utf8Oop = interpreterProxy->stackValue(1);
	utf8 = asCString(utf8Oop);
	resultOop = interpreterProxy->stackValue(0);
	resultPtr = cWordsPtrminSize(resultOop, 0);
	if (interpreterProxy->failed()) {
		return 0;
	}
	utf8Length = interpreterProxy->stSizeOf(utf8Oop);
	resultLength = interpreterProxy->stSizeOf(resultOop);
	count = unicodeGetXRanges(utf8, utf8Length, resultPtr, resultLength);
	interpreterProxy->popthenPush(3, ((count << 1) | 1));
	return 0;
}

EXPORT int primitiveMeasureString(void) {
	int utf8Length;
	int result;
	int h;
	int utf8Oop;
	int w;
	char *utf8;

	utf8Oop = interpreterProxy->stackValue(0);
	utf8 = asCString(utf8Oop);
	if (interpreterProxy->failed()) {
		return 0;
	}
	w = h = 0;
	utf8Length = interpreterProxy->stSizeOf(utf8Oop);
	unicodeMeasureString(utf8, utf8Length, &w, &h);
	result = interpreterProxy->makePointwithxValueyValue(w, h);
	interpreterProxy->popthenPush(2, result);
	return 0;
}

EXPORT int primitiveSetColors(void) {
	int fgGreen;
	int bgBlue;
	int fgBlue;
	int bgRed;
	int bgGreen;
	int mapBGToTransparent;
	int fgRed;

	fgRed = interpreterProxy->stackIntegerValue(6);
	fgGreen = interpreterProxy->stackIntegerValue(5);
	fgBlue = interpreterProxy->stackIntegerValue(4);
	bgRed = interpreterProxy->stackIntegerValue(3);
	bgGreen = interpreterProxy->stackIntegerValue(2);
	bgBlue = interpreterProxy->stackIntegerValue(1);
	mapBGToTransparent = interpreterProxy->booleanValueOf(interpreterProxy->stackValue(0));
	if (interpreterProxy->failed()) {
		return 0;
	}
	unicodeSetColors(fgRed, fgGreen, fgBlue, bgRed, bgGreen, bgBlue, mapBGToTransparent);
	interpreterProxy->pop(7);
	return 0;
}

EXPORT int primitiveSetFont(void) {
	int boldFlag;
	int fontSize;
	char fontName[200];
	int antiAliasFlag;
	int italicFlag;

	copyStringintomax(interpreterProxy->stackValue(4), fontName, 200);
	fontSize = interpreterProxy->stackIntegerValue(3);
	boldFlag = interpreterProxy->booleanValueOf(interpreterProxy->stackValue(2));
	italicFlag = interpreterProxy->booleanValueOf(interpreterProxy->stackValue(1));
	antiAliasFlag = interpreterProxy->booleanValueOf(interpreterProxy->stackValue(0));
	if (interpreterProxy->failed()) {
		return 0;
	}
	unicodeSetFont(fontName, fontSize, boldFlag, italicFlag, antiAliasFlag);
	interpreterProxy->pop(5);
	return 0;
}

EXPORT int setInterpreter(struct VirtualMachine* anInterpreter) {
	int ok;

	interpreterProxy = anInterpreter;
	ok = interpreterProxy->majorVersion() == VM_PROXY_MAJOR;
	if (ok == 0) {
		return 0;
	}
	ok = interpreterProxy->minorVersion() >= VM_PROXY_MINOR;
	return ok;
}
