
#include "mcgl_rest.h"
#include "jsoncpp/json/json.h"
#include <curl/curl.h>

CMcglRest::CMcglRest(void)
{
	
}

bool CMcglRest::sendRestPremeasurements(mcglQ_item* pItem)
{
	std::string sJson=buildPremeasurementsJson(pItem);
	bool bRes=false;
	for (int x=0;x<mcglProps.rest_resume;x++)
	{
		bRes = sendRest(mcglProps.rest_premeasurements,sJson);
		if (bRes)
			return bRes;
	}
	return false;
}

bool CMcglRest::sendRestPrefile(mcglQ_item* pItem)
{
	std::string sJson=buildPrefileJson(pItem);
	bool bRes=false;
	for (int x=0;x<mcglProps.rest_resume;x++)
	{
		bRes =  sendRest(mcglProps.rest_prefile,sJson);
		if (bRes)
			return bRes;
	}
	return false;
}

bool CMcglRest::sendRestFile(mcglQ_item* pItem)
{
	std::string sJson=buildFileJson(pItem);
	bool bRes=false;
	for (int x=0;x<mcglProps.rest_resume;x++)
	{
		bRes =  sendRest(mcglProps.rest_files,sJson);
		if (bRes)
			return bRes;
	}
	return false;
}
	
std::string CMcglRest::buildPremeasurementsJson(mcglQ_item* pItem)
{
	
	Json::Value val;
	val["golem"] = mcglProps.hostName;
	val["user"]  = getUserName(pItem);
	val["name"]  = "x";
	val["timestamp"]  = ((uint64_t)time(NULL))*1000;
	val["files"] 	= pItem->sSrcFile;
	val["finished"]	= false;
	val["ftpfilename"] 	= pItem->sFtpFile;
	
	Json::FastWriter fastWriter;
	std::string output = fastWriter.write(val);
	return output;
}

std::string CMcglRest::buildPrefileJson(mcglQ_item* pItem)
{
	Json::Value val;
	val["golem"] = mcglProps.hostName;
	val["user"]  = getUserName(pItem);
	val["filename"]  = pItem->sSrcFile;
	val["filesize"]  = pItem->fileSize;
	val["ftpfilename"] 	= pItem->sFtpFile;
	val["hash"] = pItem->sSHA1;
	
	val["timestamp"]	= ((uint64_t)time(NULL))*1000;
	
	Json::FastWriter fastWriter;
	std::string output = fastWriter.write(val);
	return output;
}

std::string CMcglRest::buildFileJson(mcglQ_item* pItem)
{
	Json::Value val;
	val["golem"] = mcglProps.hostName;
	val["hash"] = pItem->sSHA1;
	val["timestamp"]	= ((uint64_t)time(NULL))*1000;
	val["clouded"]  = false;
	val["ftpfilename"] 	= pItem->sFtpFile;
	
	Json::FastWriter fastWriter;
	std::string output = fastWriter.write(val);
	return output;
}

std::string CMcglRest::getUserName(mcglQ_item* pItem)
{
	std::string sUser=pItem->sSrcPath.substr(mcglProps.watchFolder.size());
	sUser = sUser.substr(0,sUser.find("/"));
	//sUser.erase(std::remove(sUser.begin(), sUser.end(), '/'), sUser.end());
	return sUser;
}

bool CMcglRest::sendRest(const std::string& sURl,const std::string& sJson)
{
	CURL *curl;
	CURLcode res;

	bool bOK=true;
	/* get a curl handle */ 
	curl = curl_easy_init();
	if(curl) 
	{
		
		struct curl_slist *headers=NULL;
		headers = curl_slist_append(headers, "Content-Type: application/json");
		
		curl_easy_setopt(curl, CURLOPT_URL, sURl.c_str());
		
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sJson.c_str());
		
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60L);

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

		res = curl_easy_perform(curl);
		
		long http_code = 0;
		curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
		
		if(res != CURLE_OK)
		{
			LOG(ERROR) << "rest failed error " << http_code << " curlErr=" << res << " " << curl_easy_strerror(res);
			LOG(ERROR) << "rest = " << sJson;
			bOK=false;
		}else
		{
			
			
			if (http_code!=200)
			{
				LOG(ERROR) << "rest http code " << http_code;
				bOK=false;
			}else
				LOG(INFO) << "rest url " << sURl << " returned 200 OK";
		}
		
		

		
		
		curl_easy_cleanup(curl);
	}else
		LOG(ERROR) << "curl_easy_init failed"; 
	curl_global_cleanup();
  
	return bOK;
}

