
#include <stdio.h>
#include <unistd.h>
#include <easylogging++.h>
#include <stdint.h>
#include <stddef.h>
#include <string>
#include <errno.h>
#include <algorithm>
#include <map>
#include "mcgl_prop.h"
#include "SharedMemory.h"



extern std::string sExPath;
extern CMcglProp mcglProps;
extern CMCGLSharedMemory *g_pMcglData;