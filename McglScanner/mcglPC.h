


#include <stdio.h>
#include <unistd.h>
#include <easylogging++.h>
#include <stdint.h>
#include <stddef.h>
#include "be.h"
#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <errno.h>
#include "mcgl_prop.h"
#include "SharedMemory.h"


extern std::string sExPath;
extern CMcglProp mcglProps;
extern CMCGLSharedMemory* g_pMcglData;