#ifndef _MCGL_PROP
#define _MCGL_PROP


#include <tinyxml2.h>

using namespace tinyxml2;

enum ExtensionType
{
	bulk=0,
	over,
	append
};

struct propExtension
{
	std::string sExt;
	ExtensionType type;
	uint32_t uEofTime;
	uint32_t uMaxHoldTime;
	uint32_t uMaxHoldSize;
};

class CMcglProp
{
public:
	CMcglProp(void);
	
	XMLError loadProps(std::string sFile="");
	
	std::string hostName;
	std::string device;
	uint32_t	scanLoopTimeSec;
	uint32_t	uploadLoopTimeSec;
	
	/*uint32_t    uploadLastWriteSec;
	uint32_t	uploadLastTime;
	uint32_t	uploadMaxDeltaSize;*/
	
	std::string tmpFolder;
	std::string watchFolder;
	uint8_t 	maxResumes;
	
	std::string ftp_host;
	std::string ftp_user;
	std::string ftp_pass;
	
	std::string rest_premeasurements;
	std::string rest_prefile;
	std::string rest_files;
	uint8_t rest_resume;
	
	propExtension defExt;
	
	bool GetExtension(std::string sExt,propExtension* pExt);
	std::string getPropPath()
	{
		return sPropFile;
	}
protected:
	std::map<std::string,propExtension> mapExtensions;
	int loadExtensions(tinyxml2::XMLElement* pExtEl=NULL);
	std::string sPropFile;
	tinyxml2::XMLDocument xmlDoc;
};



#endif //_MCGL_PROP