
#include "mcgl_prop.h"


CMcglProp::CMcglProp(void)
{
	hostName="lab1_Test";
	device="/dev/mmcblk0p3";
	scanLoopTimeSec=10;
	uploadLoopTimeSec=10;
	
	tmpFolder="/usr/mcgl/";
	watchFolder="/";
	maxResumes=3;
	
	ftp_host="192.168.10.10";
	ftp_user="admini";
	ftp_pass="Nthr@2012";
	/*ftp_host="104.155.110.197";
	ftp_user="golem1";
	ftp_pass="golem1";*/
	
	defExt.uEofTime=120;
	defExt.uMaxHoldSize=10*1024;
	defExt.uMaxHoldTime=60;
	
	/*rest_premeasurements="https://iucc-assaf-anderson.firebaseio.com/logs/premeasurements_queue.json";
	rest_prefile="https://iucc-assaf-anderson.firebaseio.com/logs/prefile_queue.json";
	rest_files="https://iucc-assaf-anderson.firebaseio.com/logs/files_queue.json";*/
	
	rest_premeasurements="http://imc.ismx.tv/mcgl/RestDump.php";
	rest_prefile="http://imc.ismx.tv/mcgl/RestDump.php";
	rest_files="http://imc.ismx.tv/mcgl/RestDump.php";
	rest_resume=3;
	
}


XMLError CMcglProp::loadProps(std::string sFile)
{
	if (sFile=="")
	{
		sPropFile=sExPath+"mcglProp.xml";
	}else
		sPropFile=sFile;
		
	XMLError err=xmlDoc.LoadFile(sPropFile.c_str());
	if (err)
		return err;
		
	
	//tinyxml2::XMLDocument xmlDoc2;
	tinyxml2::XMLElement* pProps = xmlDoc.RootElement();
	if (pProps==NULL)
		return XML_NO_ATTRIBUTE;

	LOG(INFO) << "CMcglProp::loadProps " << sPropFile.c_str();
	
	tinyxml2::XMLElement* pProp = pProps->FirstChildElement("host_name");
	if (pProp)
		hostName=pProp->GetText();
	
	pProp = pProps->FirstChildElement("storage_device");
	if (pProp)
		device=pProp->GetText();
		
	pProp = pProps->FirstChildElement("scan_time");
	if (pProp)
		scanLoopTimeSec=(uint32_t)atoi(pProp->GetText());
		
	pProp = pProps->FirstChildElement("upload_time");
	if (pProp)
		uploadLoopTimeSec=(uint32_t)atoi(pProp->GetText());
		
	pProp = pProps->FirstChildElement("max_resumes");
	if (pProp)
		maxResumes=(uint8_t)atoi(pProp->GetText());
		
	pProp = pProps->FirstChildElement("tmp_folder");
	if (pProp)
		tmpFolder=pProp->GetText();
	
	pProp = pProps->FirstChildElement("watch_folder");
	if (pProp)
		watchFolder=pProp->GetText();
		
	pProp = pProps->FirstChildElement("ftp");
	if (pProp)
	{
		tinyxml2::XMLElement* pFtp = pProp->FirstChildElement("host");
		if (pFtp)
			ftp_host=pFtp->GetText();
			
		pFtp = pProp->FirstChildElement("user");
		if (pFtp)
			ftp_user=pFtp->GetText();
			
		pFtp = pProp->FirstChildElement("pass");
		if (pFtp)
			ftp_pass=pFtp->GetText();
	}
	
	pProp = pProps->FirstChildElement("rest");
	if (pProp)
	{
		tinyxml2::XMLElement* pRest = pProp->FirstChildElement("premeasurements");
		if (pRest)
			rest_premeasurements=pRest->GetText();
			
		pRest = pProp->FirstChildElement("prefile");
		if (pRest)
			rest_prefile=pRest->GetText();
			
		pRest = pProp->FirstChildElement("files");
		if (pRest)
			rest_files=pRest->GetText();
			
		pRest = pProp->FirstChildElement("resume");
		if (pRest)
			rest_resume=atol(pRest->GetText());
	}
	pProp = pProps->FirstChildElement("extensions");
	if (pProp)
	{
		loadExtensions(pProp);
	}
	return XML_SUCCESS;
}

int CMcglProp::loadExtensions(tinyxml2::XMLElement* pExtEl)
{
	if (pExtEl==NULL)
	{
		tinyxml2::XMLElement* pProps = xmlDoc.RootElement();
		if (pProps==NULL)
			return XML_NO_ATTRIBUTE;
			
		tinyxml2::XMLElement* pExtEl = pProps->FirstChildElement("extensions");
		if (pExtEl==NULL)
			return XML_NO_ATTRIBUTE;
	}
	
	
	
	tinyxml2::XMLElement* pExtItem = pExtEl->FirstChildElement();
	while(pExtItem!=NULL)
	{
		std::string sExt = pExtItem->Name();
		if (sExt != "")
		{
			bool bDefault=sExt=="default";
			propExtension newExt;
			std::transform(sExt.begin(), sExt.end(), sExt.begin(), ::tolower);
			newExt.sExt=sExt;
			tinyxml2::XMLElement* pExtProp = pExtItem->FirstChildElement("type");
			if ((pExtProp!=NULL) && (pExtProp->GetText()))
			{
				if (bDefault)
					defExt.type = (ExtensionType)atol(pExtProp->GetText());
				else
					newExt.type = (ExtensionType)atol(pExtProp->GetText());
			}else
				newExt.type = defExt.type;
				
			pExtProp = pExtItem->FirstChildElement("EofTime");
			if ((pExtProp!=NULL) && (pExtProp->GetText()))
			{
				if (bDefault)
					defExt.uEofTime = defExt.uEofTime;
				else
					newExt.uEofTime = atol(pExtProp->GetText());
			}else
				newExt.uEofTime = defExt.uEofTime;
				
			pExtProp = pExtItem->FirstChildElement("MaxHoldTime");
			if ((pExtProp!=NULL) && (pExtProp->GetText()))
			{
				if (bDefault)
					defExt.uMaxHoldTime = atol(pExtProp->GetText());
				else
					newExt.uMaxHoldTime = atol(pExtProp->GetText());
			}else
				newExt.uMaxHoldTime = defExt.uMaxHoldTime;
				
			pExtProp = pExtItem->FirstChildElement("MaxHoldSize");
			if ((pExtProp!=NULL) && (pExtProp->GetText()))
			{
				if (bDefault)
					defExt.uMaxHoldSize = atol(pExtProp->GetText())*1024;
				else
					newExt.uMaxHoldSize = atol(pExtProp->GetText())*1024;
			}else
				newExt.uMaxHoldSize = defExt.uMaxHoldSize;
			
			if (!bDefault)
				mapExtensions[sExt] = newExt;
			LOG(INFO) << "Extension added " << sExt << " type " << newExt.type;
			pExtItem = pExtItem->NextSiblingElement();
		}
		
	}
	
	return XML_SUCCESS;
}

bool CMcglProp::GetExtension(std::string sExt,propExtension* pExt)
{
	if (mapExtensions.find(sExt) == mapExtensions.end())
	{
		*pExt = defExt;
		return false;
	}
	
	*pExt=mapExtensions[sExt];
	return true;
}