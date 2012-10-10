/* Automatically generated from Squeak on (2 January 2009 10:06:55 am) */

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

#include "scratchOps.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

/*** Variables ***/
struct VirtualMachine* interpreterProxy;
const char *moduleName = "ScratchPlugin 2 January 2009 (e)";

/*** Functions ***/
static int bitmapatputHsv(unsigned int *bitmap, int i, int hue, int saturation, int brightness);
static double * checkedFloatPtrOf(int oop);
static unsigned int * checkedUnsignedIntPtrOf(int oop);
static int hueFromRGBminmax(int r, int g, int b, int min, int max);
static int interpolateandfrac(int pix1, int pix2, int frac2);
static int interpolatedFromxywidthheight(unsigned int *bitmap, int xFixed, int yFixed, int w, int h);
EXPORT int primClose(void);
EXPORT int primGetOption(void);
EXPORT int primIsPortOpen(void);
EXPORT int primOpenPortNamed(void);
EXPORT int primPortCount(void);
EXPORT int primPortName(void);
EXPORT int primRead(void);
EXPORT int primSetOption(void);
EXPORT int primWrite(void);
EXPORT int primitiveBlur(void);
EXPORT int primitiveBrightnessShift(void);
EXPORT int primitiveCondenseSound(void);
EXPORT int primitiveDoubleSize(void);
EXPORT int primitiveExtractChannel(void);
EXPORT int primitiveFisheye(void);
EXPORT int primitiveGetFolderPath(void);
EXPORT int primitiveHalfSizeAverage(void);
EXPORT int primitiveHalfSizeDiagonal(void);
EXPORT int primitiveHueShift(void);
EXPORT int primitiveInterpolate(void);
EXPORT int primitiveIsHidden(void);
EXPORT int primitiveOpenURL(void);
EXPORT int primitiveSaturationShift(void);
EXPORT int primitiveScale(void);
EXPORT int primitiveSetUnicodePasteBuffer(void);
EXPORT int primitiveSetWindowTitle(void);
EXPORT int primitiveShortToLongPath(void);
EXPORT int primitiveWaterRipples1(void);
EXPORT int primitiveWhirl(void);
EXPORT int setInterpreter(struct VirtualMachine* anInterpreter);

static int bitmapatputHsv(unsigned int *bitmap, int i, int hue, int saturation, int brightness) {
	int outPix;
	int v;
	int t;
	int hF;
	int hI;
	int p;
	int q;

	hI = hue / 60;
	hF = hue % 60;
	p = (1000 - saturation) * brightness;
	q = (1000 - ((saturation * hF) / 60)) * brightness;
	t = (1000 - ((saturation * (60 - hF)) / 60)) * brightness;
	v = (brightness * 1000) / 3922;
	p = p / 3922;
	q = q / 3922;
	t = t / 3922;
	if (0 == hI) {
		outPix = ((((unsigned) v << 16)) + (((unsigned) t << 8))) + p;
	}
	if (1 == hI) {
		outPix = ((((unsigned) q << 16)) + (((unsigned) v << 8))) + p;
	}
	if (2 == hI) {
		outPix = ((((unsigned) p << 16)) + (((unsigned) v << 8))) + t;
	}
	if (3 == hI) {
		outPix = ((((unsigned) p << 16)) + (((unsigned) q << 8))) + v;
	}
	if (4 == hI) {
		outPix = ((((unsigned) t << 16)) + (((unsigned) p << 8))) + v;
	}
	if (5 == hI) {
		outPix = ((((unsigned) v << 16)) + (((unsigned) p << 8))) + q;
	}
	if (outPix == 0) {
		outPix = 1;
	}
	bitmap[i] = outPix;
	return 0;
}

static double * checkedFloatPtrOf(int oop) {
	interpreterProxy->success(interpreterProxy->isWordsOrBytes(oop));
	if (interpreterProxy->failed()) {
		return 0;
	}
	return ((double *) (interpreterProxy->firstIndexableField(oop)));
}

static unsigned int * checkedUnsignedIntPtrOf(int oop) {
	interpreterProxy->success(interpreterProxy->isWords(oop));
	if (interpreterProxy->failed()) {
		return 0;
	}
	return ((unsigned int *) (interpreterProxy->firstIndexableField(oop)));
}

static int hueFromRGBminmax(int r, int g, int b, int min, int max) {
	int span;
	int result;

	span = max - min;
	if (span == 0) {
		return 0;
	}
	if (r == max) {
		result = (60 * (g - b)) / span;
	} else {
		if (g == max) {
			result = 120 + ((60 * (b - r)) / span);
		} else {
			result = 240 + ((60 * (r - g)) / span);
		}
	}
	if (result < 0) {
		return result + 360;
	}
	return result;
}

static int interpolateandfrac(int pix1, int pix2, int frac2) {
	int result;
	int g;
	int b;
	int r;
	int frac1;

	if (pix1 == 0) {
		return pix2;
	}
	if (pix2 == 0) {
		return pix1;
	}
	frac1 = 1024 - frac2;
	r = ((frac1 * ((((unsigned) pix1 >> 16)) & 255)) + (frac2 * ((((unsigned) pix2 >> 16)) & 255))) / 1024;
	g = ((frac1 * ((((unsigned) pix1 >> 8)) & 255)) + (frac2 * ((((unsigned) pix2 >> 8)) & 255))) / 1024;
	b = ((frac1 * (pix1 & 255)) + (frac2 * (pix2 & 255))) / 1024;
	result = ((((unsigned) r << 16)) + (((unsigned) g << 8))) + b;
	if (result == 0) {
		result = 1;
	}
	return result;
}

static int interpolatedFromxywidthheight(unsigned int *bitmap, int xFixed, int yFixed, int w, int h) {
	int yFrac;
	int y;
	int x;
	int topPix;
	int xFrac;
	int bottomPix;
	int index;
	int result;
	int g;
	int b;
	int r;
	int frac1;
	int result1;
	int g1;
	int b1;
	int r1;
	int frac11;
	int result2;
	int g2;
	int b2;
	int r2;
	int frac12;

	x = ((unsigned) xFixed >> 10);
	if ((x < -1) || (x >= w)) {
		return 0;
	}
	y = ((unsigned) yFixed >> 10);
	if ((y < -1) || (y >= h)) {
		return 0;
	}
	xFrac = xFixed & 1023;
	if (x == -1) {
		x = 0;
		xFrac = 0;
	}
	if (x == (w - 1)) {
		xFrac = 0;
	}
	yFrac = yFixed & 1023;
	if (y == -1) {
		y = 0;
		yFrac = 0;
	}
	if (y == (h - 1)) {
		yFrac = 0;
	}
	index = (y * w) + x;
	topPix = (bitmap[index]) & 16777215;
	if (xFrac > 0) {
		/* begin interpolate:and:frac: */
		if (topPix == 0) {
			topPix = (bitmap[index + 1]) & 16777215;
			goto l1;
		}
		if (((bitmap[index + 1]) & 16777215) == 0) {
			topPix = topPix;
			goto l1;
		}
		frac1 = 1024 - xFrac;
		r = ((frac1 * ((((unsigned) topPix >> 16)) & 255)) + (xFrac * ((((unsigned) ((bitmap[index + 1]) & 16777215) >> 16)) & 255))) / 1024;
		g = ((frac1 * ((((unsigned) topPix >> 8)) & 255)) + (xFrac * ((((unsigned) ((bitmap[index + 1]) & 16777215) >> 8)) & 255))) / 1024;
		b = ((frac1 * (topPix & 255)) + (xFrac * (((bitmap[index + 1]) & 16777215) & 255))) / 1024;
		result = ((((unsigned) r << 16)) + (((unsigned) g << 8))) + b;
		if (result == 0) {
			result = 1;
		}
		topPix = result;
	l1:	/* end interpolate:and:frac: */;
	}
	if (yFrac == 0) {
		return topPix;
	}
	index = ((y + 1) * w) + x;
	bottomPix = (bitmap[index]) & 16777215;
	if (xFrac > 0) {
		/* begin interpolate:and:frac: */
		if (bottomPix == 0) {
			bottomPix = (bitmap[index + 1]) & 16777215;
			goto l2;
		}
		if (((bitmap[index + 1]) & 16777215) == 0) {
			bottomPix = bottomPix;
			goto l2;
		}
		frac11 = 1024 - xFrac;
		r1 = ((frac11 * ((((unsigned) bottomPix >> 16)) & 255)) + (xFrac * ((((unsigned) ((bitmap[index + 1]) & 16777215) >> 16)) & 255))) / 1024;
		g1 = ((frac11 * ((((unsigned) bottomPix >> 8)) & 255)) + (xFrac * ((((unsigned) ((bitmap[index + 1]) & 16777215) >> 8)) & 255))) / 1024;
		b1 = ((frac11 * (bottomPix & 255)) + (xFrac * (((bitmap[index + 1]) & 16777215) & 255))) / 1024;
		result1 = ((((unsigned) r1 << 16)) + (((unsigned) g1 << 8))) + b1;
		if (result1 == 0) {
			result1 = 1;
		}
		bottomPix = result1;
	l2:	/* end interpolate:and:frac: */;
	}
	/* begin interpolate:and:frac: */
	if (topPix == 0) {
		return bottomPix;
	}
	if (bottomPix == 0) {
		return topPix;
	}
	frac12 = 1024 - yFrac;
	r2 = ((frac12 * ((((unsigned) topPix >> 16)) & 255)) + (yFrac * ((((unsigned) bottomPix >> 16)) & 255))) / 1024;
	g2 = ((frac12 * ((((unsigned) topPix >> 8)) & 255)) + (yFrac * ((((unsigned) bottomPix >> 8)) & 255))) / 1024;
	b2 = ((frac12 * (topPix & 255)) + (yFrac * (bottomPix & 255))) / 1024;
	result2 = ((((unsigned) r2 << 16)) + (((unsigned) g2 << 8))) + b2;
	if (result2 == 0) {
		result2 = 1;
	}
	return result2;
}

EXPORT int primClose(void) {
	int portNum;

	portNum = interpreterProxy->stackIntegerValue(0);
	if (interpreterProxy->failed()) {
		return 0;
	}
	SerialPortClose(portNum);
	interpreterProxy->pop(1);
	return 0;
}

EXPORT int primGetOption(void) {
	int result;
	int portNum;
	int attrNum;

	portNum = interpreterProxy->stackIntegerValue(1);
	attrNum = interpreterProxy->stackIntegerValue(0);
	if (interpreterProxy->failed()) {
		return 0;
	}
	result = SerialPortGetOption(portNum, attrNum);
	if (result == -1) {
		interpreterProxy->success(0);
		return 0;
	}
	interpreterProxy->pop(3);
	interpreterProxy->pushInteger(result);
	return 0;
}

EXPORT int primIsPortOpen(void) {
	int result;
	int portNum;

	portNum = interpreterProxy->stackIntegerValue(0);
	if (interpreterProxy->failed()) {
		return 0;
	}
	result = SerialPortIsOpen(portNum);
	interpreterProxy->pop(2);
	interpreterProxy->pushBool(result != 0);
	return 0;
}

EXPORT int primOpenPortNamed(void) {
	char nameStr[1000];
	int baudRate;
	int i;
	int count;
	char * src;
	int portNum;
	int nameOop;

	nameOop = interpreterProxy->stackValue(1);
	baudRate = interpreterProxy->stackIntegerValue(0);
	if (((nameOop & 1)) || (!(interpreterProxy->isBytes(nameOop)))) {
		interpreterProxy->success(0);
		return 0;
	}
	if (interpreterProxy->failed()) {
		return 0;
	}
	src = ((char *) (interpreterProxy->firstIndexableField(nameOop)));
	count = interpreterProxy->stSizeOf(nameOop);
	for (i = 0; i <= (count - 1); i += 1) {
		nameStr[i] = (src[i]);
	}
	nameStr[count] = 0;
	portNum = SerialPortOpenPortNamed(nameStr, baudRate);
	if (portNum == -1) {
		interpreterProxy->success(0);
		return 0;
	}
	interpreterProxy->popthenPush(3, ((portNum << 1) | 1));
	return 0;
}

EXPORT int primPortCount(void) {
	int result;

	result = SerialPortCount();
	if (result == -1) {
		interpreterProxy->success(0);
		return 0;
	}
	interpreterProxy->popthenPush(1, ((result << 1) | 1));
	return 0;
}

EXPORT int primPortName(void) {
	char nameStr[1000];
	int i;
	int count;
	int resultOop;
	int portIndex;
	char* dst;

	portIndex = interpreterProxy->stackIntegerValue(0);
	if (interpreterProxy->failed()) {
		return 0;
	}
	SerialPortName(portIndex, nameStr, 1000);
	count = strlen(nameStr);
	if (count == 0) {
		interpreterProxy->success(0);
		return 0;
	}
	resultOop = interpreterProxy->instantiateClassindexableSize(interpreterProxy->classString(), count);
	dst = ((char *) (interpreterProxy->firstIndexableField(resultOop)));
	for (i = 0; i <= (count - 1); i += 1) {
		dst[i] = (nameStr[i]);
	}
	interpreterProxy->popthenPush(2, resultOop);
	return 0;
}

EXPORT int primRead(void) {
	char *bufPtr;
	int bytesRead;
	int bufSize;
	int portNum;
	int bufOop;

	portNum = interpreterProxy->stackIntegerValue(1);
	bufOop = interpreterProxy->stackValue(0);
	if (((bufOop & 1)) || (!(interpreterProxy->isBytes(bufOop)))) {
		interpreterProxy->success(0);
		return 0;
	}
	bufPtr = ((char *) (interpreterProxy->firstIndexableField(bufOop)));
	bufSize = interpreterProxy->stSizeOf(bufOop);
	if (interpreterProxy->failed()) {
		return 0;
	}
	bytesRead = SerialPortRead(portNum, bufPtr, bufSize);
	interpreterProxy->pop(3);
	interpreterProxy->pushInteger(bytesRead);
	return 0;
}

EXPORT int primSetOption(void) {
	int result;
	int portNum;
	int attrNum;
	int attrValue;

	portNum = interpreterProxy->stackIntegerValue(2);
	attrNum = interpreterProxy->stackIntegerValue(1);
	attrValue = interpreterProxy->stackIntegerValue(0);
	if (interpreterProxy->failed()) {
		return 0;
	}
	result = SerialPortSetOption(portNum, attrNum, attrValue);
	if (result == -1) {
		interpreterProxy->success(0);
		return 0;
	}
	interpreterProxy->pop(3);
	return 0;
}

EXPORT int primWrite(void) {
	char *bufPtr;
	int bufSize;
	int portNum;
	int bufOop;
	int bytesWritten;

	portNum = interpreterProxy->stackIntegerValue(1);
	bufOop = interpreterProxy->stackValue(0);
	if (((bufOop & 1)) || (!(interpreterProxy->isBytes(bufOop)))) {
		interpreterProxy->success(0);
		return 0;
	}
	bufPtr = ((char *) (interpreterProxy->firstIndexableField(bufOop)));
	bufSize = interpreterProxy->stSizeOf(bufOop);
	if (interpreterProxy->failed()) {
		return 0;
	}
	bytesWritten = SerialPortWrite(portNum, bufPtr, bufSize);
	interpreterProxy->pop(3);
	interpreterProxy->pushInteger(bytesWritten);
	return 0;
}

EXPORT int primitiveBlur(void) {
	int width;
	int outPix;
	unsigned int * in;
	int gTotal;
	int inOop;
	int outOop;
	unsigned int * out;
	int pix;
	int y;
	int x;
	int bTotal;
	int height;
	int sz;
	int dX;
	int rTotal;
	int n;
	int dY;

	inOop = interpreterProxy->stackValue(2);
	outOop = interpreterProxy->stackValue(1);
	width = interpreterProxy->stackIntegerValue(0);
	/* begin checkedUnsignedIntPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(inOop));
	if (interpreterProxy->failed()) {
		in = 0;
		goto l1;
	}
	in = ((unsigned int *) (interpreterProxy->firstIndexableField(inOop)));
l1:	/* end checkedUnsignedIntPtrOf: */;
	/* begin checkedUnsignedIntPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(outOop));
	if (interpreterProxy->failed()) {
		out = 0;
		goto l2;
	}
	out = ((unsigned int *) (interpreterProxy->firstIndexableField(outOop)));
l2:	/* end checkedUnsignedIntPtrOf: */;
	sz = interpreterProxy->stSizeOf(inOop);
	interpreterProxy->success((interpreterProxy->stSizeOf(outOop)) == sz);
	if (interpreterProxy->failed()) {
		return 0;
	}
	height = sz / width;
	for (y = 1; y <= (height - 2); y += 1) {
		for (x = 1; x <= (width - 2); x += 1) {
			n = rTotal = gTotal = bTotal = 0;
			for (dY = -1; dY <= 1; dY += 1) {
				for (dX = -1; dX <= 1; dX += 1) {
					pix = (in[((y + dY) * width) + (x + dX)]) & 16777215;
					if (!(pix == 0)) {
						rTotal += (((unsigned) pix >> 16)) & 255;
						gTotal += (((unsigned) pix >> 8)) & 255;
						bTotal += pix & 255;
						n += 1;
					}
				}
			}
			if (n == 0) {
				outPix = 0;
			} else {
				outPix = ((((unsigned) (rTotal / n) << 16)) + (((unsigned) (gTotal / n) << 8))) + (bTotal / n);
			}
			out[(y * width) + x] = outPix;
		}
	}
	interpreterProxy->pop(3);
	return 0;
}

EXPORT int primitiveBrightnessShift(void) {
	int hue;
	unsigned int * in;
	int max;
	int brightness;
	int i;
	int inOop;
	int outOop;
	int pix;
	int shift;
	unsigned int * out;
	int g;
	int min;
	int b;
	int r;
	int saturation;
	int sz;
	int span;
	int result;

	inOop = interpreterProxy->stackValue(2);
	outOop = interpreterProxy->stackValue(1);
	shift = interpreterProxy->stackIntegerValue(0);
	/* begin checkedUnsignedIntPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(inOop));
	if (interpreterProxy->failed()) {
		in = 0;
		goto l2;
	}
	in = ((unsigned int *) (interpreterProxy->firstIndexableField(inOop)));
l2:	/* end checkedUnsignedIntPtrOf: */;
	sz = interpreterProxy->stSizeOf(inOop);
	/* begin checkedUnsignedIntPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(outOop));
	if (interpreterProxy->failed()) {
		out = 0;
		goto l3;
	}
	out = ((unsigned int *) (interpreterProxy->firstIndexableField(outOop)));
l3:	/* end checkedUnsignedIntPtrOf: */;
	interpreterProxy->success((interpreterProxy->stSizeOf(outOop)) == sz);
	if (interpreterProxy->failed()) {
		return 0;
	}
	for (i = 0; i <= (sz - 1); i += 1) {
		pix = (in[i]) & 16777215;
		if (!(pix == 0)) {
			r = (((unsigned) pix >> 16)) & 255;
			g = (((unsigned) pix >> 8)) & 255;
			b = pix & 255;
			max = min = r;
			if (g > max) {
				max = g;
			}
			if (b > max) {
				max = b;
			}
			if (g < min) {
				min = g;
			}
			if (b < min) {
				min = b;
			}
			/* begin hueFromR:G:B:min:max: */
			span = max - min;
			if (span == 0) {
				hue = 0;
				goto l1;
			}
			if (r == max) {
				result = (60 * (g - b)) / span;
			} else {
				if (g == max) {
					result = 120 + ((60 * (b - r)) / span);
				} else {
					result = 240 + ((60 * (r - g)) / span);
				}
			}
			if (result < 0) {
				hue = result + 360;
				goto l1;
			}
			hue = result;
		l1:	/* end hueFromR:G:B:min:max: */;
			if (max == 0) {
				saturation = 0;
			} else {
				saturation = ((max - min) * 1000) / max;
			}
			brightness = (max * 1000) / 255;
			brightness += shift * 10;
			if (brightness > 1000) {
				brightness = 1000;
			}
			if (brightness < 0) {
				brightness = 0;
			}
			bitmapatputHsv(out, i, hue, saturation, brightness);
		}
	}
	interpreterProxy->pop(3);
	return 0;
}

EXPORT int primitiveCondenseSound(void) {
	int max;
	int j;
	int i;
	int count;
	int factor;
	int v;
	int srcOop;
	int dstOop;
	short *src;
	short *dst;
	int sz;

	srcOop = interpreterProxy->stackValue(2);
	dstOop = interpreterProxy->stackValue(1);
	factor = interpreterProxy->stackIntegerValue(0);
	interpreterProxy->success(interpreterProxy->isWords(srcOop));
	interpreterProxy->success(interpreterProxy->isWords(dstOop));
	count = (2 * (interpreterProxy->stSizeOf(srcOop))) / factor;
	sz = 2 * (interpreterProxy->stSizeOf(dstOop));
	interpreterProxy->success(sz >= count);
	if (interpreterProxy->failed()) {
		return 0;
	}
	src = ((short *) (interpreterProxy->firstIndexableField(srcOop)));
	dst = ((short *) (interpreterProxy->firstIndexableField(dstOop)));
	for (i = 1; i <= count; i += 1) {
		max = 0;
		for (j = 1; j <= factor; j += 1) {
			v = *src++;
			if (v < 0) {
				v = 0 - v;
			}
			if (v > max) {
				max = v;
			}
		}
		*dst++ = max;
	}
	interpreterProxy->pop(3);
	return 0;
}

EXPORT int primitiveDoubleSize(void) {
	unsigned int * in;
	int inW;
	int inH;
	int i;
	int inOop;
	int outOop;
	unsigned int * out;
	int outW;
	int outH;
	int pix;
	int y;
	int x;
	int dstX;
	int dstY;
	int baseIndex;

	inOop = interpreterProxy->stackValue(7);
	inW = interpreterProxy->stackIntegerValue(6);
	inH = interpreterProxy->stackIntegerValue(5);
	outOop = interpreterProxy->stackValue(4);
	outW = interpreterProxy->stackIntegerValue(3);
	outH = interpreterProxy->stackIntegerValue(2);
	dstX = interpreterProxy->stackIntegerValue(1);
	dstY = interpreterProxy->stackIntegerValue(0);
	/* begin checkedUnsignedIntPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(inOop));
	if (interpreterProxy->failed()) {
		in = 0;
		goto l1;
	}
	in = ((unsigned int *) (interpreterProxy->firstIndexableField(inOop)));
l1:	/* end checkedUnsignedIntPtrOf: */;
	/* begin checkedUnsignedIntPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(outOop));
	if (interpreterProxy->failed()) {
		out = 0;
		goto l2;
	}
	out = ((unsigned int *) (interpreterProxy->firstIndexableField(outOop)));
l2:	/* end checkedUnsignedIntPtrOf: */;
	interpreterProxy->success((dstX + (2 * inW)) < outW);
	interpreterProxy->success((dstY + (2 * inH)) < outH);
	if (interpreterProxy->failed()) {
		return 0;
	}
	for (y = 0; y <= (inH - 1); y += 1) {
		baseIndex = ((dstY + (2 * y)) * outW) + dstX;
		for (x = 0; x <= (inW - 1); x += 1) {
			pix = in[x + (y * inW)];
			i = baseIndex + (2 * x);
			out[i] = pix;
			out[i + 1] = pix;
			out[i + outW] = pix;
			out[(i + outW) + 1] = pix;
		}
	}
	interpreterProxy->pop(8);
	return 0;
}

EXPORT int primitiveExtractChannel(void) {
	int i;
	int srcOop;
	int dstOop;
	short *src;
	short *dst;
	int sz;
	int rightFlag;

	srcOop = interpreterProxy->stackValue(2);
	dstOop = interpreterProxy->stackValue(1);
	rightFlag = interpreterProxy->booleanValueOf(interpreterProxy->stackValue(0));
	interpreterProxy->success(interpreterProxy->isWords(srcOop));
	interpreterProxy->success(interpreterProxy->isWords(dstOop));
	sz = interpreterProxy->stSizeOf(srcOop);
	interpreterProxy->success((interpreterProxy->stSizeOf(dstOop)) >= (sz / 2));
	if (interpreterProxy->failed()) {
		return 0;
	}
	src = ((short *) (interpreterProxy->firstIndexableField(srcOop)));
	dst = ((short *) (interpreterProxy->firstIndexableField(dstOop)));
	if (rightFlag) {
		src++;
	}
	for (i = 1; i <= sz; i += 1) {
		*dst++ = *src; src += 2;
	}
	interpreterProxy->pop(3);
	return 0;
}

EXPORT int primitiveFisheye(void) {
	int width;
	int srcX;
	int srcY;
	int pix;
	int power;
	int y;
	double ang;
	double scaledPower;
	double dx;
	double dy;
	int sz;
	unsigned int * in;
	int inOop;
	int outOop;
	unsigned int * out;
	int centerX;
	int centerY;
	int x;
	double r;
	int height;

	inOop = interpreterProxy->stackValue(3);
	outOop = interpreterProxy->stackValue(2);
	width = interpreterProxy->stackIntegerValue(1);
	power = interpreterProxy->stackIntegerValue(0);
	/* begin checkedUnsignedIntPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(inOop));
	if (interpreterProxy->failed()) {
		in = 0;
		goto l1;
	}
	in = ((unsigned int *) (interpreterProxy->firstIndexableField(inOop)));
l1:	/* end checkedUnsignedIntPtrOf: */;
	/* begin checkedUnsignedIntPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(outOop));
	if (interpreterProxy->failed()) {
		out = 0;
		goto l2;
	}
	out = ((unsigned int *) (interpreterProxy->firstIndexableField(outOop)));
l2:	/* end checkedUnsignedIntPtrOf: */;
	sz = interpreterProxy->stSizeOf(inOop);
	interpreterProxy->success((interpreterProxy->stSizeOf(outOop)) == sz);
	if (interpreterProxy->failed()) {
		return 0;
	}
	height = sz / width;
	centerX = width / 2;
	centerY = height / 2;
	height = sz / width;
	centerX = width / 2;
	centerY = height / 2;
	scaledPower = power / 100.0;
	for (x = 0; x <= (width - 1); x += 1) {
		for (y = 0; y <= (height - 1); y += 1) {
			dx = (x - centerX) / (((double) centerX ));
			dy = (y - centerY) / (((double) centerY ));
			r = pow((sqrt((dx * dx) + (dy * dy))),scaledPower);
			if (r <= 1.0) {
				ang = atan2(dy,dx);
				srcX = ((int) (1024 * (centerX + ((r * (cos(ang))) * centerX))) );
				srcY = ((int) (1024 * (centerY + ((r * (sin(ang))) * centerY))) );
			} else {
				srcX = 1024 * x;
				srcY = 1024 * y;
			}
			pix = interpolatedFromxywidthheight(in, srcX, srcY, width, height);
			out[(y * width) + x] = pix;
		}
	}
	interpreterProxy->pop(4);
	return 0;
}

EXPORT int primitiveGetFolderPath(void) {
	char nameStr[2000];
	int i;
	int count;
	int folderID;
	int resultOop;
	char* dst;

	folderID = interpreterProxy->stackIntegerValue(0);
	if (interpreterProxy->failed()) {
		return 0;
	}
	GetFolderPathForID(folderID, nameStr, 2000);
	count = strlen(nameStr);
	resultOop = interpreterProxy->instantiateClassindexableSize(interpreterProxy->classString(), count);
	dst = ((char *) (interpreterProxy->firstIndexableField(resultOop)));
	for (i = 0; i <= (count - 1); i += 1) {
		dst[i] = (nameStr[i]);
	}
	interpreterProxy->popthenPush(2, resultOop);
	return 0;
}

EXPORT int primitiveHalfSizeAverage(void) {
	int srcX;
	int srcY;
	int pixel;
	int srcIndex;
	int dstIndex;
	int y;
	int b;
	int dstX;
	int dstY;
	int dstW;
	int dstH;
	unsigned int * in;
	int inW;
	int inH;
	int g;
	unsigned int * out;
	int outW;
	int outH;
	int x;
	int r;
	int oop;
	int oop1;

	/* begin checkedUnsignedIntPtrOf: */
	oop = interpreterProxy->stackValue(11);
	interpreterProxy->success(interpreterProxy->isWords(oop));
	if (interpreterProxy->failed()) {
		in = 0;
		goto l1;
	}
	in = ((unsigned int *) (interpreterProxy->firstIndexableField(oop)));
l1:	/* end checkedUnsignedIntPtrOf: */;
	inW = interpreterProxy->stackIntegerValue(10);
	inH = interpreterProxy->stackIntegerValue(9);
	/* begin checkedUnsignedIntPtrOf: */
	oop1 = interpreterProxy->stackValue(8);
	interpreterProxy->success(interpreterProxy->isWords(oop1));
	if (interpreterProxy->failed()) {
		out = 0;
		goto l2;
	}
	out = ((unsigned int *) (interpreterProxy->firstIndexableField(oop1)));
l2:	/* end checkedUnsignedIntPtrOf: */;
	outW = interpreterProxy->stackIntegerValue(7);
	outH = interpreterProxy->stackIntegerValue(6);
	srcX = interpreterProxy->stackIntegerValue(5);
	srcY = interpreterProxy->stackIntegerValue(4);
	dstX = interpreterProxy->stackIntegerValue(3);
	dstY = interpreterProxy->stackIntegerValue(2);
	dstW = interpreterProxy->stackIntegerValue(1);
	dstH = interpreterProxy->stackIntegerValue(0);
	interpreterProxy->success((srcX >= 0) && (srcY >= 0));
	interpreterProxy->success((srcX + (2 * dstW)) <= inW);
	interpreterProxy->success((srcY + (2 * dstH)) <= inH);
	interpreterProxy->success((dstX >= 0) && (dstY >= 0));
	interpreterProxy->success((dstX + dstW) <= outW);
	interpreterProxy->success((dstY + dstH) <= outH);
	if (interpreterProxy->failed()) {
		return 0;
	}
	for (y = 0; y <= (dstH - 1); y += 1) {
		srcIndex = (inW * (srcY + (2 * y))) + srcX;
		dstIndex = (outW * (dstY + y)) + dstX;
		for (x = 0; x <= (dstW - 1); x += 1) {
			pixel = in[srcIndex];
			r = pixel & 16711680;
			g = pixel & 65280;
			b = pixel & 255;
			pixel = in[srcIndex + 1];
			r += pixel & 16711680;
			g += pixel & 65280;
			b += pixel & 255;
			pixel = in[srcIndex + inW];
			r += pixel & 16711680;
			g += pixel & 65280;
			b += pixel & 255;
			pixel = in[(srcIndex + inW) + 1];
			r += pixel & 16711680;
			g += pixel & 65280;
			b += pixel & 255;
			out[dstIndex] = (((((unsigned) r >> 2)) & 16711680) | (((((unsigned) g >> 2)) & 65280) | (((unsigned) b >> 2))));
			srcIndex += 2;
			dstIndex += 1;
		}
	}
	interpreterProxy->pop(12);
	return 0;
}

EXPORT int primitiveHalfSizeDiagonal(void) {
	int srcX;
	int srcY;
	int y;
	int srcIndex;
	int dstIndex;
	int b;
	int dstX;
	int dstY;
	int dstW;
	int dstH;
	int p1;
	int p2;
	unsigned int * in;
	int inW;
	int inH;
	int g;
	unsigned int * out;
	int outW;
	int outH;
	int x;
	int r;
	int oop;
	int oop1;

	/* begin checkedUnsignedIntPtrOf: */
	oop = interpreterProxy->stackValue(11);
	interpreterProxy->success(interpreterProxy->isWords(oop));
	if (interpreterProxy->failed()) {
		in = 0;
		goto l1;
	}
	in = ((unsigned int *) (interpreterProxy->firstIndexableField(oop)));
l1:	/* end checkedUnsignedIntPtrOf: */;
	inW = interpreterProxy->stackIntegerValue(10);
	inH = interpreterProxy->stackIntegerValue(9);
	/* begin checkedUnsignedIntPtrOf: */
	oop1 = interpreterProxy->stackValue(8);
	interpreterProxy->success(interpreterProxy->isWords(oop1));
	if (interpreterProxy->failed()) {
		out = 0;
		goto l2;
	}
	out = ((unsigned int *) (interpreterProxy->firstIndexableField(oop1)));
l2:	/* end checkedUnsignedIntPtrOf: */;
	outW = interpreterProxy->stackIntegerValue(7);
	outH = interpreterProxy->stackIntegerValue(6);
	srcX = interpreterProxy->stackIntegerValue(5);
	srcY = interpreterProxy->stackIntegerValue(4);
	dstX = interpreterProxy->stackIntegerValue(3);
	dstY = interpreterProxy->stackIntegerValue(2);
	dstW = interpreterProxy->stackIntegerValue(1);
	dstH = interpreterProxy->stackIntegerValue(0);
	interpreterProxy->success((srcX >= 0) && (srcY >= 0));
	interpreterProxy->success((srcX + (2 * dstW)) <= inW);
	interpreterProxy->success((srcY + (2 * dstH)) <= inH);
	interpreterProxy->success((dstX >= 0) && (dstY >= 0));
	interpreterProxy->success((dstX + dstW) <= outW);
	interpreterProxy->success((dstY + dstH) <= outH);
	if (interpreterProxy->failed()) {
		return 0;
	}
	for (y = 0; y <= (dstH - 1); y += 1) {
		srcIndex = (inW * (srcY + (2 * y))) + srcX;
		dstIndex = (outW * (dstY + y)) + dstX;
		for (x = 0; x <= (dstW - 1); x += 1) {
			p1 = in[srcIndex];
			p2 = in[(srcIndex + inW) + 1];
			r = (((unsigned) ((p1 & 16711680) + (p2 & 16711680)) >> 1)) & 16711680;
			g = (((unsigned) ((p1 & 65280) + (p2 & 65280)) >> 1)) & 65280;
			b = ((unsigned) ((p1 & 255) + (p2 & 255)) >> 1);
			out[dstIndex] = (r | (g | b));
			srcIndex += 2;
			dstIndex += 1;
		}
	}
	interpreterProxy->pop(12);
	return 0;
}

EXPORT int primitiveHueShift(void) {
	int hue;
	unsigned int * in;
	int max;
	int brightness;
	int i;
	int inOop;
	int outOop;
	int pix;
	int shift;
	unsigned int * out;
	int g;
	int min;
	int b;
	int r;
	int saturation;
	int sz;
	int span;
	int result;

	inOop = interpreterProxy->stackValue(2);
	outOop = interpreterProxy->stackValue(1);
	shift = interpreterProxy->stackIntegerValue(0);
	/* begin checkedUnsignedIntPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(inOop));
	if (interpreterProxy->failed()) {
		in = 0;
		goto l2;
	}
	in = ((unsigned int *) (interpreterProxy->firstIndexableField(inOop)));
l2:	/* end checkedUnsignedIntPtrOf: */;
	sz = interpreterProxy->stSizeOf(inOop);
	/* begin checkedUnsignedIntPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(outOop));
	if (interpreterProxy->failed()) {
		out = 0;
		goto l3;
	}
	out = ((unsigned int *) (interpreterProxy->firstIndexableField(outOop)));
l3:	/* end checkedUnsignedIntPtrOf: */;
	interpreterProxy->success((interpreterProxy->stSizeOf(outOop)) == sz);
	if (interpreterProxy->failed()) {
		return 0;
	}
	for (i = 0; i <= (sz - 1); i += 1) {
		pix = (in[i]) & 16777215;
		if (!(pix == 0)) {
			r = (((unsigned) pix >> 16)) & 255;
			g = (((unsigned) pix >> 8)) & 255;
			b = pix & 255;
			max = min = r;
			if (g > max) {
				max = g;
			}
			if (b > max) {
				max = b;
			}
			if (g < min) {
				min = g;
			}
			if (b < min) {
				min = b;
			}
			brightness = (max * 1000) / 255;
			if (max == 0) {
				saturation = 0;
			} else {
				saturation = ((max - min) * 1000) / max;
			}
			if (brightness < 110) {
				brightness = 110;
				saturation = 1000;
			}
			if (saturation < 90) {
				saturation = 90;
			}
			if ((brightness == 110) || (saturation == 90)) {
				hue = 0;
			} else {
				/* begin hueFromR:G:B:min:max: */
				span = max - min;
				if (span == 0) {
					hue = 0;
					goto l1;
				}
				if (r == max) {
					result = (60 * (g - b)) / span;
				} else {
					if (g == max) {
						result = 120 + ((60 * (b - r)) / span);
					} else {
						result = 240 + ((60 * (r - g)) / span);
					}
				}
				if (result < 0) {
					hue = result + 360;
					goto l1;
				}
				hue = result;
			l1:	/* end hueFromR:G:B:min:max: */;
			}
			hue = ((hue + shift) + 360000000) % 360;
			bitmapatputHsv(out, i, hue, saturation, brightness);
		}
	}
	interpreterProxy->pop(3);
	return 0;
}

EXPORT int primitiveInterpolate(void) {
	int width;
	unsigned int * in;
	int result;
	int inOop;
	int yFixed;
	int sz;
	int xFixed;

	inOop = interpreterProxy->stackValue(3);
	width = interpreterProxy->stackIntegerValue(2);
	xFixed = interpreterProxy->stackIntegerValue(1);
	yFixed = interpreterProxy->stackIntegerValue(0);
	/* begin checkedUnsignedIntPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(inOop));
	if (interpreterProxy->failed()) {
		in = 0;
		goto l1;
	}
	in = ((unsigned int *) (interpreterProxy->firstIndexableField(inOop)));
l1:	/* end checkedUnsignedIntPtrOf: */;
	sz = interpreterProxy->stSizeOf(inOop);
	if (interpreterProxy->failed()) {
		return 0;
	}
	result = interpolatedFromxywidthheight(in, xFixed, yFixed, width, sz / width);
	interpreterProxy->pop(5);
	interpreterProxy->pushInteger(result);
	return 0;
}

EXPORT int primitiveIsHidden(void) {
	int result;
	int count;
	int i;
	char fullPath[1000];
	char * src;
	int pathOop;

	pathOop = interpreterProxy->stackValue(0);
	if (((pathOop & 1)) || (!(interpreterProxy->isBytes(pathOop)))) {
		interpreterProxy->success(0);
	}
	if (interpreterProxy->failed()) {
		return 0;
	}
	src = ((char *) (interpreterProxy->firstIndexableField(pathOop)));
	count = interpreterProxy->stSizeOf(pathOop);
	if (count >= 1000) {
		interpreterProxy->success(0);
		return 0;
	}
	for (i = 0; i <= (count - 1); i += 1) {
		fullPath[i] = (src[i]);
	}
	fullPath[count] = 0;
	result = IsFileOrFolderHidden(fullPath);
	interpreterProxy->pop(2);
	interpreterProxy->pushBool(result != 0);
	return 0;
}

EXPORT int primitiveOpenURL(void) {
	char * src;
	int count;
	int i;
	int urlOop;
	char urlStr[2000];

	urlOop = interpreterProxy->stackValue(0);
	if (((urlOop & 1)) || (!(interpreterProxy->isBytes(urlOop)))) {
		interpreterProxy->success(0);
	}
	if (interpreterProxy->failed()) {
		return 0;
	}
	src = ((char *) (interpreterProxy->firstIndexableField(urlOop)));
	count = interpreterProxy->stSizeOf(urlOop);
	if (count >= 2000) {
		interpreterProxy->success(0);
		return 0;
	}
	for (i = 0; i <= (count - 1); i += 1) {
		urlStr[i] = (src[i]);
	}
	urlStr[count] = 0;
	OpenURL(urlStr);
	interpreterProxy->pop(1);
	return 0;
}

EXPORT int primitiveSaturationShift(void) {
	int hue;
	unsigned int * in;
	int max;
	int brightness;
	int i;
	int inOop;
	int outOop;
	int pix;
	int shift;
	unsigned int * out;
	int g;
	int min;
	int b;
	int r;
	int saturation;
	int sz;
	int span;
	int result;

	inOop = interpreterProxy->stackValue(2);
	outOop = interpreterProxy->stackValue(1);
	shift = interpreterProxy->stackIntegerValue(0);
	/* begin checkedUnsignedIntPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(inOop));
	if (interpreterProxy->failed()) {
		in = 0;
		goto l2;
	}
	in = ((unsigned int *) (interpreterProxy->firstIndexableField(inOop)));
l2:	/* end checkedUnsignedIntPtrOf: */;
	sz = interpreterProxy->stSizeOf(inOop);
	/* begin checkedUnsignedIntPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(outOop));
	if (interpreterProxy->failed()) {
		out = 0;
		goto l3;
	}
	out = ((unsigned int *) (interpreterProxy->firstIndexableField(outOop)));
l3:	/* end checkedUnsignedIntPtrOf: */;
	interpreterProxy->success((interpreterProxy->stSizeOf(outOop)) == sz);
	if (interpreterProxy->failed()) {
		return 0;
	}
	for (i = 0; i <= (sz - 1); i += 1) {
		pix = (in[i]) & 16777215;
		if (!(pix < 2)) {
			r = (((unsigned) pix >> 16)) & 255;
			g = (((unsigned) pix >> 8)) & 255;
			b = pix & 255;
			max = min = r;
			if (g > max) {
				max = g;
			}
			if (b > max) {
				max = b;
			}
			if (g < min) {
				min = g;
			}
			if (b < min) {
				min = b;
			}
			brightness = (max * 1000) / 255;
			if (max == 0) {
				saturation = 0;
			} else {
				saturation = ((max - min) * 1000) / max;
			}
			if (saturation > 0) {
				/* begin hueFromR:G:B:min:max: */
				span = max - min;
				if (span == 0) {
					hue = 0;
					goto l1;
				}
				if (r == max) {
					result = (60 * (g - b)) / span;
				} else {
					if (g == max) {
						result = 120 + ((60 * (b - r)) / span);
					} else {
						result = 240 + ((60 * (r - g)) / span);
					}
				}
				if (result < 0) {
					hue = result + 360;
					goto l1;
				}
				hue = result;
			l1:	/* end hueFromR:G:B:min:max: */;
				saturation += shift * 10;
				if (saturation > 1000) {
					saturation = 1000;
				}
				if (saturation < 0) {
					saturation = 0;
				}
				bitmapatputHsv(out, i, hue, saturation, brightness);
			}
		}
	}
	interpreterProxy->pop(3);
	return 0;
}

EXPORT int primitiveScale(void) {
	int w1;
	int w2;
	int w3;
	int w4;
	int yIncr;
	int p1;
	int p2;
	int p3;
	int p4;
	int xIncr;
	int outPix;
	unsigned int * in;
	int inW;
	int inH;
	int inX;
	int inOop;
	int outOop;
	int inY;
	int outW;
	int outH;
	unsigned int * out;
	int outY;
	int outX;
	int t;
	int tWeight;

	inOop = interpreterProxy->stackValue(5);
	inW = interpreterProxy->stackIntegerValue(4);
	inH = interpreterProxy->stackIntegerValue(3);
	outOop = interpreterProxy->stackValue(2);
	outW = interpreterProxy->stackIntegerValue(1);
	outH = interpreterProxy->stackIntegerValue(0);
	interpreterProxy->success((interpreterProxy->stSizeOf(inOop)) == (inW * inH));
	interpreterProxy->success((interpreterProxy->stSizeOf(outOop)) == (outW * outH));
	/* begin checkedUnsignedIntPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(inOop));
	if (interpreterProxy->failed()) {
		in = 0;
		goto l1;
	}
	in = ((unsigned int *) (interpreterProxy->firstIndexableField(inOop)));
l1:	/* end checkedUnsignedIntPtrOf: */;
	/* begin checkedUnsignedIntPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(outOop));
	if (interpreterProxy->failed()) {
		out = 0;
		goto l2;
	}
	out = ((unsigned int *) (interpreterProxy->firstIndexableField(outOop)));
l2:	/* end checkedUnsignedIntPtrOf: */;
	if (interpreterProxy->failed()) {
		return 0;
	}
	inX = inY = 0;
	xIncr = (inW * 1024) / outW;
	yIncr = (inH * 1024) / outH;
	for (outY = 0; outY <= (outH - 1); outY += 1) {
		inX = 0;
		for (outX = 0; outX <= (outW - 1); outX += 1) {
			w1 = (1024 - (inX & 1023)) * (1024 - (inY & 1023));
			w2 = (inX & 1023) * (1024 - (inY & 1023));
			w3 = (1024 - (inX & 1023)) * (inY & 1023);
			w4 = (inX & 1023) * (inY & 1023);
			t = ((((unsigned) inY) >> 10) * inW) + (((unsigned) inX) >> 10);
			p1 = in[t];
			if ((((unsigned) inX) >> 10) < (inW - 1)) {
				p2 = in[t + 1];
			} else {
				p2 = p1;
			}
			if ((((unsigned) inY) >> 10) < (inH - 1)) {
				t += inW;
			}
			p3 = in[t];
			if ((((unsigned) inX) >> 10) < (inW - 1)) {
				p4 = in[t + 1];
			} else {
				p4 = p3;
			}
			tWeight = 0;
			if (p1 == 0) {
				p1 = p2;
				tWeight += w1;
			}
			if (p2 == 0) {
				p2 = p1;
				tWeight += w2;
			}
			if (p3 == 0) {
				p3 = p4;
				tWeight += w3;
			}
			if (p4 == 0) {
				p4 = p3;
				tWeight += w4;
			}
			if (p1 == 0) {
				p1 = p3;
				p2 = p4;
			}
			if (p3 == 0) {
				p3 = p1;
				p4 = p2;
			}
			outPix = 0;
			if (tWeight < 500000) {
				t = (((w1 * ((((unsigned) p1) >> 16) & 255)) + (w2 * ((((unsigned) p2) >> 16) & 255))) + (w3 * ((((unsigned) p3) >> 16) & 255))) + (w4 * ((((unsigned) p4) >> 16) & 255));
				outPix = ((((unsigned) t) >> 20) & 255) << 16;
				t = (((w1 * ((((unsigned) p1) >> 8) & 255)) + (w2 * ((((unsigned) p2) >> 8) & 255))) + (w3 * ((((unsigned) p3) >> 8) & 255))) + (w4 * ((((unsigned) p4) >> 8) & 255));
				outPix = outPix | (((((unsigned) t) >> 20) & 255) << 8);
				t = (((w1 * (p1 & 255)) + (w2 * (p2 & 255))) + (w3 * (p3 & 255))) + (w4 * (p4 & 255));
				outPix = outPix | ((((unsigned) t) >> 20) & 255);
				if (outPix == 0) {
					outPix = 1;
				}
			}
			out[(outY * outW) + outX] = outPix;
			inX += xIncr;
		}
		inY += yIncr;
	}
	interpreterProxy->pop(6);
	return 0;
}

EXPORT int primitiveSetUnicodePasteBuffer(void) {
	short int *utf16;
	int strOop;
	int count;

	strOop = interpreterProxy->stackValue(0);
	if (((strOop & 1)) || (!(interpreterProxy->isBytes(strOop)))) {
		interpreterProxy->success(0);
	}
	if (interpreterProxy->failed()) {
		return 0;
	}
	utf16 = ((short int *) (interpreterProxy->firstIndexableField(strOop)));
	count = interpreterProxy->stSizeOf(strOop);
	SetUnicodePasteBuffer(utf16, count);
	interpreterProxy->pop(1);
	return 0;
}

EXPORT int primitiveSetWindowTitle(void) {
	char * src;
	char titleStr[1000];
	int count;
	int i;
	int titleOop;

	titleOop = interpreterProxy->stackValue(0);
	if (((titleOop & 1)) || (!(interpreterProxy->isBytes(titleOop)))) {
		interpreterProxy->success(0);
	}
	if (interpreterProxy->failed()) {
		return 0;
	}
	src = ((char *) (interpreterProxy->firstIndexableField(titleOop)));
	count = interpreterProxy->stSizeOf(titleOop);
	if (count >= 1000) {
		interpreterProxy->success(0);
		return 0;
	}
	for (i = 0; i <= (count - 1); i += 1) {
		titleStr[i] = (src[i]);
	}
	titleStr[count] = 0;
	SetScratchWindowTitle(titleStr);
	interpreterProxy->pop(1);
	return 0;
}

EXPORT int primitiveShortToLongPath(void) {
	int result;
	int count;
	int i;
	int resultOop;
	char longPath[1000];
	char * ptr;
	int shortPathOop;
	char shortPath[1000];

	shortPathOop = interpreterProxy->stackValue(0);
	if (((shortPathOop & 1)) || (!(interpreterProxy->isBytes(shortPathOop)))) {
		interpreterProxy->success(0);
		return 0;
	}
	ptr = ((char *) (interpreterProxy->firstIndexableField(shortPathOop)));
	count = interpreterProxy->stSizeOf(shortPathOop);
	if (count >= 1000) {
		interpreterProxy->success(0);
		return 0;
	}
	for (i = 0; i <= (count - 1); i += 1) {
		shortPath[i] = (ptr[i]);
	}
	shortPath[count] = 0;
	result = WinShortToLongPath(shortPath, longPath, 1000);
	if (result == -1) {
		interpreterProxy->success(0);
		return 0;
	}
	count = strlen(longPath);
	resultOop = interpreterProxy->instantiateClassindexableSize(interpreterProxy->classString(), count);
	ptr = ((char *) (interpreterProxy->firstIndexableField(resultOop)));
	for (i = 0; i <= (count - 1); i += 1) {
		ptr[i] = (longPath[i]);
	}
	interpreterProxy->popthenPush(2, resultOop);
	return 0;
}

EXPORT int primitiveWaterRipples1(void) {
	int width;
	int t1;
	int val;
	int j;
	double * aArray;
	int h;
	unsigned int pix;
	int y;
	int power;
	int allPix;
	int val2;
	int f;
	int d;
	int aArOop;
	double dist;
	int dx;
	int dy;
	double dx2;
	double dy2;
	int q;
	unsigned int * in;
	int ripply;
	int i;
	int inOop;
	int outOop;
	int g;
	unsigned int * out;
	int x;
	double * bArray;
	int t;
	int height;
	double temp;
	int bArOop;
	int blops;
	int newLoc;

	inOop = interpreterProxy->stackValue(5);
	outOop = interpreterProxy->stackValue(4);
	width = interpreterProxy->stackIntegerValue(3);
	/* begin checkedUnsignedIntPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(inOop));
	if (interpreterProxy->failed()) {
		in = 0;
		goto l1;
	}
	in = ((unsigned int *) (interpreterProxy->firstIndexableField(inOop)));
l1:	/* end checkedUnsignedIntPtrOf: */;
	/* begin checkedUnsignedIntPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(outOop));
	if (interpreterProxy->failed()) {
		out = 0;
		goto l2;
	}
	out = ((unsigned int *) (interpreterProxy->firstIndexableField(outOop)));
l2:	/* end checkedUnsignedIntPtrOf: */;
	allPix = interpreterProxy->stSizeOf(inOop);
	ripply = interpreterProxy->stackIntegerValue(2);
	aArOop = interpreterProxy->stackValue(1);
	bArOop = interpreterProxy->stackValue(0);
	/* begin checkedFloatPtrOf: */
	interpreterProxy->success(interpreterProxy->isWordsOrBytes(aArOop));
	if (interpreterProxy->failed()) {
		aArray = 0;
		goto l3;
	}
	aArray = ((double *) (interpreterProxy->firstIndexableField(aArOop)));
l3:	/* end checkedFloatPtrOf: */;
	/* begin checkedFloatPtrOf: */
	interpreterProxy->success(interpreterProxy->isWordsOrBytes(bArOop));
	if (interpreterProxy->failed()) {
		bArray = 0;
		goto l4;
	}
	bArray = ((double *) (interpreterProxy->firstIndexableField(bArOop)));
l4:	/* end checkedFloatPtrOf: */;
	interpreterProxy->success((interpreterProxy->stSizeOf(outOop)) == allPix);
	if (interpreterProxy->failed()) {
		return 0;
	}
	height = allPix / width;
	t1 = rand();
	blops = (t1 % ripply) - 1;
	for (t = 0; t <= ((blops / 2) - 1); t += 1) {
		t1 = rand();
		x = t1 % width;
		t1 = rand();
		y = t1 % height;
		t1 = rand();
		power = t1 % 8;
		for (g = -4; g <= 4; g += 1) {
			for (h = -4; h <= 4; h += 1) {
				dist = ((double) ((g * g) + (h * h)) );
				if ((dist < 25) && (dist > 0)) {
					dx = ((int) (x + g) );
					dy = ((int) (y + h) );
					if ((dx > 0) && ((dy > 0) && ((dy < height) && (dx < width)))) {
						aArray[(dy * width) + dx] = (power * (((double) ((((double) 1.0 )) - (dist / (((double) 25.0 )))) )));
					}
				}
			}
		}
	}
	for (f = 1; f <= (width - 2); f += 1) {
		for (d = 1; d <= (height - 2); d += 1) {
			val = (d * width) + f;
			aArray[val] = ((((((((((bArray[val + 1]) + (bArray[val - 1])) + (bArray[val + width])) + (bArray[val - width])) + ((bArray[(val - 1) - width]) / 2)) + ((bArray[(val - 1) + width]) / 2)) + ((bArray[(val + 1) - width]) / 2)) + ((bArray[(val + 1) + width]) / 2)) / 4) - (aArray[val]));
			aArray[val] = ((aArray[val]) * (((double) 0.9 )));
		}
	}
	for (q = 0; q <= (width * height); q += 1) {
		temp = bArray[q];
		bArray[q] = (aArray[q]);
		aArray[q] = temp;
	}
	for (j = 0; j <= (height - 1); j += 1) {
		for (i = 0; i <= (width - 1); i += 1) {
			if ((i > 1) && ((i < (width - 1)) && ((j > 1) && (j < (height - 1))))) {
				val2 = (j * width) + i;
				dx2 = ((double) ((((aArray[val2]) - (aArray[val2 - 1])) + ((aArray[val2 + 1]) - (aArray[val2]))) * 64) );
				dy2 = ((double) ((((aArray[val2]) - (aArray[val2 - width])) + ((aArray[val2 + width]) - (aArray[val2]))) / 64) );
				if (dx2 < -2) {
					dx2 = -2;
				}
				if (dx2 > 2) {
					dx2 = 2;
				}
				if (dy2 < -2) {
					dy2 = -2;
				}
				if (dy2 > 2) {
					dy2 = 2;
				}
				newLoc = ((int) (((j + dy2) * width) + (i + dx2)) );
				if ((newLoc < (width * height)) && (newLoc >= 0)) {
					pix = in[newLoc];
				} else {
					pix = in[i + (j * width)];
				}
			} else {
				pix = in[i + (j * width)];
			}
			out[i + (j * width)] = pix;
		}
	}
	interpreterProxy->pop(6);
	return 0;
}

EXPORT int primitiveWhirl(void) {
	int width;
	double whirlRadians;
	int degrees;
	double scaleX;
	double scaleY;
	int pix;
	double d;
	double ang;
	double sina;
	int y;
	double dx;
	double dy;
	int sz;
	unsigned int * in;
	double radiusSquared;
	int inOop;
	int outOop;
	double factor;
	double cosa;
	unsigned int * out;
	int centerX;
	int centerY;
	int radius;
	int x;
	int height;

	inOop = interpreterProxy->stackValue(3);
	outOop = interpreterProxy->stackValue(2);
	width = interpreterProxy->stackIntegerValue(1);
	degrees = interpreterProxy->stackIntegerValue(0);
	/* begin checkedUnsignedIntPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(inOop));
	if (interpreterProxy->failed()) {
		in = 0;
		goto l1;
	}
	in = ((unsigned int *) (interpreterProxy->firstIndexableField(inOop)));
l1:	/* end checkedUnsignedIntPtrOf: */;
	/* begin checkedUnsignedIntPtrOf: */
	interpreterProxy->success(interpreterProxy->isWords(outOop));
	if (interpreterProxy->failed()) {
		out = 0;
		goto l2;
	}
	out = ((unsigned int *) (interpreterProxy->firstIndexableField(outOop)));
l2:	/* end checkedUnsignedIntPtrOf: */;
	sz = interpreterProxy->stSizeOf(inOop);
	interpreterProxy->success((interpreterProxy->stSizeOf(outOop)) == sz);
	if (interpreterProxy->failed()) {
		return 0;
	}
	height = sz / width;
	centerX = width / 2;
	centerY = height / 2;
	if (centerX < centerY) {
		radius = centerX;
		scaleX = (((double) centerY )) / centerX;
		scaleY = 1.0;
	} else {
		radius = centerY;
		scaleX = 1.0;
		if (centerY < centerX) {
			scaleY = (((double) centerX )) / centerY;
		} else {
			scaleY = 1.0;
		}
	}
	whirlRadians = (-3.141592653589793 * degrees) / 180.0;
	radiusSquared = ((double) (radius * radius) );
	for (x = 0; x <= (width - 1); x += 1) {
		for (y = 0; y <= (height - 1); y += 1) {
			dx = scaleX * (((double) (x - centerX) ));
			dy = scaleY * (((double) (y - centerY) ));
			d = (dx * dx) + (dy * dy);
			if (d < radiusSquared) {
				factor = 1.0 - ((sqrt(d)) / radius);
				ang = whirlRadians * (factor * factor);
				sina = sin(ang);
				cosa = cos(ang);
				pix = interpolatedFromxywidthheight(in, ((int) (1024.0 * ((((cosa * dx) - (sina * dy)) / scaleX) + centerX)) ), ((int) (1024.0 * ((((sina * dx) + (cosa * dy)) / scaleY) + centerY)) ), width, height);
				out[(width * y) + x] = pix;
			}
		}
	}
	interpreterProxy->pop(4);
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
