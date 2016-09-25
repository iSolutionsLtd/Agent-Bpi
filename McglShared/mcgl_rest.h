#ifndef _MCGL_REST
#define _MCGL_REST

#include "mcgl_xml.h"

class CMcglRest
{
public:
	CMcglRest(void);
	bool sendRestPremeasurements(mcglQ_item* pItem);
	bool sendRestPrefile(mcglQ_item* pItem);
	bool sendRestFile(mcglQ_item* pItem);
	
protected:
	std::string getUserName(mcglQ_item* pItem);
	bool sendRest(const std::string& sURl,const std::string& sJson);
	std::string buildPremeasurementsJson(mcglQ_item* pItem);
	std::string buildPrefileJson(mcglQ_item* pItem);
	std::string buildFileJson(mcglQ_item* pItem);
};

#endif //_MCGL_REST