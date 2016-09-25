#ifndef _MCGL_UPLOADER
#define _MCGL_UPLOADER

#include "mcgl_xml.h"
#include "mcgl_rest.h"

#include "ftplib.h"


class CMcglUploader
{
public:
	CMcglUploader(void);
	~CMcglUploader(void);
	
	int test(void);
	int UploadNext();
		
	
protected:
	int sha1File(const char *name,char* out);
	void completeItem(mcglQ_item* pItem);
	
	CMcglRest mcglRest;
	CMcglXml mcglXml;
	
};

#endif //_MCGL_UPLOADER