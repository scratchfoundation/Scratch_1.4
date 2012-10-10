/* Automatically generated from Squeak on (12 May 2009 3:14:29 pm) */

#if defined(WIN32) || defined(_WIN32) || defined(Win32)
 #ifdef __cplusplus
  #define DLLEXPORT extern "C" __declspec(dllexport)
 #else
  #define DLLEXPORT __declspec(dllexport)
 #endif /* C++ */
#else
 #define DLLEXPORT
#endif /* WIN32 */

#include "sqVirtualMachine.h"

/* memory access macros */
#define byteAt(i) (*((unsigned char *) (i)))
#define byteAtput(i, val) (*((unsigned char *) (i)) = val)
#define longAt(i) (*((int *) (i)))
#define longAtput(i, val) (*((int *) (i)) = val)

/* WeDo Functions */
int WeDoOpenPort(void);
int WeDoClosePort(void);
int WeDoRead(char *bufPtr, int bufSize);
int WeDoWrite(char *bufPtr, int bufSize);


/*** Variables ***/
struct VirtualMachine* interpreterProxy;
const char *moduleName = "WeDoPlugin 12 May 2009 (e)";

/*** Functions ***/
DLLEXPORT int primClosePort(void);
DLLEXPORT int primOpenPort(void);
DLLEXPORT int primRead(void);
DLLEXPORT int primWrite(void);
DLLEXPORT int setInterpreter(struct VirtualMachine* anInterpreter);

DLLEXPORT int primClosePort(void) {
	interpreterProxy->success(WeDoClosePort());
	return 0;
}

DLLEXPORT int primOpenPort(void) {
	interpreterProxy->success(WeDoOpenPort());
	return 0;
}

DLLEXPORT int primRead(void) {
	char *bufPtr;
	int bufSize;
	int bufOop;
	int byteCount;

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
	byteCount = WeDoRead(bufPtr, bufSize);
	if (byteCount < 0) {
		interpreterProxy->success(0);
		return 0;
	}
	interpreterProxy->pop(2);
	interpreterProxy->pushInteger(byteCount);
	return 0;
}

DLLEXPORT int primWrite(void) {
	char *bufPtr;
	int bufSize;
	int bufOop;
	int byteCount;

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
	byteCount = WeDoWrite(bufPtr, bufSize);
	if (byteCount < 0) {
		interpreterProxy->success(0);
		return 0;
	}
	interpreterProxy->pop(2);
	interpreterProxy->pushInteger(byteCount);
	return 0;
}

DLLEXPORT int setInterpreter(struct VirtualMachine* anInterpreter) {
	int ok;

	interpreterProxy = anInterpreter;
	ok = interpreterProxy->majorVersion() == VM_PROXY_MAJOR;
	if (ok == 0) {
		return 0;
	}
	ok = interpreterProxy->minorVersion() >= VM_PROXY_MINOR;
	return ok;
}
