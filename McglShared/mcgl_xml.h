#ifndef _MCGL_XML
#define _MCGL_XML

#include <tinyxml2.h>
#include <uuid/uuid.h>


using namespace tinyxml2;

enum mcglQ_state
{
	Pending=0,
	prepare,
	uploading,
	error,
	finished
};

enum mcglQ_status
{
	Detected=0,
	Premeasure,
	Prefile,
	Uploaded,
	File
};

struct mcglQ_item
{
	uuid_t	transID;
	mcglQ_state state;
	mcglQ_status status;
	std::string sSrcPath;
	std::string sSrcFile;
	std::string sTmpFile;
	time_t createTime;
	time_t modifyTime;
	uint32_t fileSize;
	uint32_t createSize;
	uint8_t	 resumes;
	std::string sSHA1;
	std::string sFtpFile;
};

class CMcglXml
{
public:
	CMcglXml(void);
	
	
	XMLError OpenXml(const std::string& sXml);
	XMLError Refresh();
	XMLError createXml();
	
	XMLError findFile(mcglQ_item* pItem,const std::string& sPath,const std::string& sFile);
	XMLError addItem(mcglQ_item* pItem);
	XMLError updateItem(mcglQ_item* pItem);
	XMLError removeItem(uuid_t transID);
	bool getFirstUpload(mcglQ_item* pItem,mcglQ_state state=Pending);
	
	static time_t getGmtTime();
	static std::string buildFtpName(mcglQ_item* pItem);
protected:
	
	XMLElement* findItemByTransID(uuid_t transID);
	XMLError ItemFromElement(XMLElement* pEl,mcglQ_item* pItem);
	
	
	std::string sXmlPath;
	XMLDocument xmlDoc;	
	
};

#endif