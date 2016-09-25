#include "mcgl_xml.h"

#include <tinyxml2.cpp>

CMcglXml::CMcglXml(void)
{
	
}

XMLError CMcglXml::OpenXml(const std::string& sXml)
{
	sXmlPath=sXml;
	XMLError err=xmlDoc.LoadFile(sXmlPath.c_str());
	
	return err;
}

XMLError CMcglXml::Refresh()
{
	XMLError err=xmlDoc.LoadFile(sXmlPath.c_str());
	
	return err;
}

XMLError CMcglXml::createXml()
{
	XMLElement * pRoot = xmlDoc.NewElement("MCGLQ");
	XMLElement * pItems = xmlDoc.NewElement("ITEMS");
	pRoot->InsertFirstChild(pItems);
	xmlDoc.InsertFirstChild(pRoot);
	return xmlDoc.SaveFile(sXmlPath.c_str());
	
}

XMLError CMcglXml::findFile(mcglQ_item* pItem,const std::string& sPath,const std::string& sFile)
{
	Refresh();
	tinyxml2::XMLElement * pItems = xmlDoc.RootElement()->FirstChildElement("ITEMS");
	if (pItems==NULL)
		return XML_NO_ATTRIBUTE;
	
	for (tinyxml2::XMLElement* pEl=pItems->LastChildElement();pEl!=NULL;pEl=pEl->PreviousSiblingElement())
	{
		tinyxml2::XMLElement* pCurEl = pEl->FirstChildElement("SRCPATH");
		if (pCurEl==NULL)
			continue;
		if (!strcmp(pCurEl->GetText(),sPath.c_str()))
		{
			pCurEl = pEl->FirstChildElement("SRCFILE");
			if (pCurEl==NULL)
				continue;
			if (!strcmp(pCurEl->GetText(),sFile.c_str()))
			{
				return ItemFromElement(pEl,pItem);
			}
		}
	}
	
	return XML_NO_ATTRIBUTE;
}

XMLError CMcglXml::addItem(mcglQ_item* pNewItem)
{
	Refresh();
	XMLElement * pItems = xmlDoc.RootElement()->FirstChildElement("ITEMS");
	if (pItems==NULL)
		return XML_NO_ATTRIBUTE;
	
	
	char uuid_str[37];
	uuid_unparse_lower(pNewItem->transID,uuid_str);
	tinyxml2::XMLElement * pItem = xmlDoc.NewElement("ITEM");
	
	tinyxml2::XMLElement * pTRANSID = xmlDoc.NewElement("TRANSID");
	pTRANSID->SetText(uuid_str);
	pItem->InsertEndChild(pTRANSID);
	
	tinyxml2::XMLElement * pState = xmlDoc.NewElement("STATE");
	pState->SetText(pNewItem->state);
	pItem->InsertEndChild(pState);
	
	tinyxml2::XMLElement * pStatus = xmlDoc.NewElement("STATUS");
	pStatus->SetText(pNewItem->status);
	pItem->InsertEndChild(pStatus);
	
	tinyxml2::XMLElement * pSrcPath = xmlDoc.NewElement("SRCPATH");
	pSrcPath->SetText(pNewItem->sSrcPath.c_str());
	pItem->InsertEndChild(pSrcPath);
	
	tinyxml2::XMLElement * pSrcFile = xmlDoc.NewElement("SRCFILE");
	pSrcFile->SetText(pNewItem->sSrcFile.c_str());
	pItem->InsertEndChild(pSrcFile);
	
	tinyxml2::XMLElement * pTmpFile = xmlDoc.NewElement("TMPFILE");
	pTmpFile->SetText(pNewItem->sTmpFile.c_str());
	pItem->InsertEndChild(pTmpFile);
	
	tinyxml2::XMLElement * pModTime = xmlDoc.NewElement("MODTIME");
	pModTime->SetText((uint32_t)pNewItem->modifyTime);
	pItem->InsertEndChild(pModTime);
	
	tinyxml2::XMLElement * pCrTime = xmlDoc.NewElement("CRTIME");
	pCrTime->SetText((uint32_t)pNewItem->createTime);
	pItem->InsertEndChild(pCrTime);
	
	tinyxml2::XMLElement * pSize = xmlDoc.NewElement("SIZE");
	pSize->SetText(pNewItem->fileSize);
	pItem->InsertEndChild(pSize);
	
	tinyxml2::XMLElement * pSizeDelta = xmlDoc.NewElement("CRSIZE");
	pSizeDelta->SetText(pNewItem->createSize);
	pItem->InsertEndChild(pSizeDelta);
	
	tinyxml2::XMLElement * pFtp = xmlDoc.NewElement("FTPFILE");
	pFtp->SetText(pNewItem->sFtpFile.c_str());
	pItem->InsertEndChild(pFtp);
	
	tinyxml2::XMLElement * pSha1 = xmlDoc.NewElement("SHA1");
	pSha1->SetText(pNewItem->sSHA1.c_str());
	pItem->InsertEndChild(pSha1);
	
	tinyxml2::XMLElement * pResume = xmlDoc.NewElement("RESUMES");
	pResume->SetText(pNewItem->resumes);
	pItem->InsertEndChild(pResume);
	
	pItems->InsertEndChild(pItem);
	return xmlDoc.SaveFile(sXmlPath.c_str());
}

XMLError CMcglXml::updateItem(mcglQ_item* pNewItem)
{
	tinyxml2::XMLElement* pItem=findItemByTransID(pNewItem->transID);
	if (pItem==NULL)
	{
		return XML_NO_ATTRIBUTE;
	}
	
	char uuid_str[37];
	uuid_unparse_lower(pNewItem->transID,uuid_str);
	
	pItem->DeleteChildren();
	
	tinyxml2::XMLElement * pTRANSID = xmlDoc.NewElement("TRANSID");
	pTRANSID->SetText(uuid_str);
	pItem->InsertEndChild(pTRANSID);
	
	tinyxml2::XMLElement * pState = xmlDoc.NewElement("STATE");
	pState->SetText(pNewItem->state);
	pItem->InsertEndChild(pState);
	
	tinyxml2::XMLElement * pStatus = xmlDoc.NewElement("STATUS");
	pStatus->SetText(pNewItem->status);
	pItem->InsertEndChild(pStatus);
	
	tinyxml2::XMLElement * pSrcPath = xmlDoc.NewElement("SRCPATH");
	pSrcPath->SetText(pNewItem->sSrcPath.c_str());
	pItem->InsertEndChild(pSrcPath);
	
	tinyxml2::XMLElement * pSrcFile = xmlDoc.NewElement("SRCFILE");
	pSrcFile->SetText(pNewItem->sSrcFile.c_str());
	pItem->InsertEndChild(pSrcFile);
	
	tinyxml2::XMLElement * pTmpFile = xmlDoc.NewElement("TMPFILE");
	pTmpFile->SetText(pNewItem->sTmpFile.c_str());
	pItem->InsertEndChild(pTmpFile);
	
	tinyxml2::XMLElement * pModTime = xmlDoc.NewElement("MODTIME");
	pModTime->SetText((uint32_t)pNewItem->modifyTime);
	pItem->InsertEndChild(pModTime);
	
	tinyxml2::XMLElement * pCrTime = xmlDoc.NewElement("CRTIME");
	pCrTime->SetText((uint32_t)pNewItem->createTime);
	pItem->InsertEndChild(pCrTime);
	
	tinyxml2::XMLElement * pSize = xmlDoc.NewElement("SIZE");
	pSize->SetText(pNewItem->fileSize);
	pItem->InsertEndChild(pSize);
	
	tinyxml2::XMLElement * pSizeDelta = xmlDoc.NewElement("CRSIZE");
	pSizeDelta->SetText(pNewItem->createSize);
	pItem->InsertEndChild(pSizeDelta);
	
	tinyxml2::XMLElement * pFtp = xmlDoc.NewElement("FTPFILE");
	pFtp->SetText(pNewItem->sFtpFile.c_str());
	pItem->InsertEndChild(pFtp);
	
	tinyxml2::XMLElement * pSha1 = xmlDoc.NewElement("SHA1");
	pSha1->SetText(pNewItem->sSHA1.c_str());
	pItem->InsertEndChild(pSha1);
	
	tinyxml2::XMLElement * pResume = xmlDoc.NewElement("RESUMES");
	pResume->SetText(pNewItem->resumes);
	pItem->InsertEndChild(pResume);
	
	return xmlDoc.SaveFile(sXmlPath.c_str());
}

XMLElement * CMcglXml::findItemByTransID(uuid_t transID)
{
	Refresh();
	XMLElement * pItems = xmlDoc.RootElement()->FirstChildElement("ITEMS");
	if (pItems==NULL)
		return NULL;
	
	char uuid_str[37];
	uuid_unparse_lower(transID,uuid_str);
	
	for (tinyxml2::XMLElement* pItem=pItems->LastChildElement();pItem!=NULL;pItem=pItem->PreviousSiblingElement())
	{
		tinyxml2::XMLElement* pTransID = pItem->FirstChildElement("TRANSID");
		if (pTransID==NULL)
			continue;
			
		if (!strcmp(pTransID->GetText(),uuid_str))
		{
			return pItem;
		}
	}
	return NULL;
}

XMLError CMcglXml::ItemFromElement(tinyxml2::XMLElement* pEl,mcglQ_item* pItem)
{
	tinyxml2::XMLElement* pCurEl = pEl->FirstChildElement("TRANSID");
	if (pCurEl==NULL)
		return XML_NO_ATTRIBUTE;
	if (pCurEl->GetText())
		uuid_parse(pCurEl->GetText(),pItem->transID);
	
	pCurEl = pEl->FirstChildElement("STATE");
	if (pCurEl==NULL)
		return XML_NO_ATTRIBUTE;
	if (pCurEl->GetText())
		pItem->state=(mcglQ_state)atoi(pCurEl->GetText());
		
	pCurEl = pEl->FirstChildElement("STATUS");
	if (pCurEl==NULL)
		return XML_NO_ATTRIBUTE;
	if (pCurEl->GetText())
		pItem->status=(mcglQ_status)atoi(pCurEl->GetText());
	
	pCurEl = pEl->FirstChildElement("SRCPATH");
	if (pCurEl==NULL)
		return XML_NO_ATTRIBUTE;
	if (pCurEl->GetText())
		pItem->sSrcPath=pCurEl->GetText();
	
	pCurEl = pEl->FirstChildElement("SRCFILE");
	if (pCurEl==NULL)
		return XML_NO_ATTRIBUTE;
	if (pCurEl->GetText())
		pItem->sSrcFile=pCurEl->GetText();
	
	pCurEl = pEl->FirstChildElement("TMPFILE");
	if (pCurEl==NULL)
		return XML_NO_ATTRIBUTE;
	if (pCurEl->GetText())
		pItem->sTmpFile=pCurEl->GetText();
	
	pCurEl = pEl->FirstChildElement("MODTIME");
	if (pCurEl==NULL)
		return XML_NO_ATTRIBUTE;
	if (pCurEl->GetText())
		pItem->modifyTime=(time_t)atol(pCurEl->GetText());
	
	pCurEl = pEl->FirstChildElement("CRTIME");
	if (pCurEl==NULL)
		return XML_NO_ATTRIBUTE;
	if (pCurEl->GetText())
		pItem->createTime=(time_t)atol(pCurEl->GetText());
		
	pCurEl = pEl->FirstChildElement("SIZE");
	if (pCurEl==NULL)
		return XML_NO_ATTRIBUTE;
	if (pCurEl->GetText())
		pItem->fileSize=(uint32_t)atol(pCurEl->GetText());
	
	pCurEl = pEl->FirstChildElement("CRSIZE");
	if (pCurEl==NULL)
		return XML_NO_ATTRIBUTE;
	if (pCurEl->GetText())
		pItem->createSize=(uint32_t)atol(pCurEl->GetText());
		
	pCurEl = pEl->FirstChildElement("FTPFILE");
	if (pCurEl==NULL)
		return XML_NO_ATTRIBUTE;
	if (pCurEl->GetText())
		pItem->sFtpFile = pCurEl->GetText();
	
	pCurEl = pEl->FirstChildElement("SHA1");
	if (pCurEl==NULL)
		return XML_NO_ATTRIBUTE;
	if (pCurEl->GetText())
		pItem->sSHA1 = std::string(pCurEl->GetText());
	
	pCurEl = pEl->FirstChildElement("RESUMES");
	if (pCurEl==NULL)
		return XML_NO_ATTRIBUTE;
	if (pCurEl->GetText())
		pItem->resumes=(uint8_t)atol(pCurEl->GetText());
	return XML_SUCCESS;
	
}

bool CMcglXml::getFirstUpload(mcglQ_item* pItem,mcglQ_state state)
{
	if (Refresh())
		return false;
	XMLElement * pItems = xmlDoc.RootElement()->FirstChildElement("ITEMS");
	if (pItems==NULL)
		return false;
	bool bPending=false;
	for (tinyxml2::XMLElement* pEl=pItems->FirstChildElement();pEl!=NULL;pEl=pEl->NextSiblingElement())
	{
		tinyxml2::XMLElement* pCurEl = pEl->FirstChildElement("STATE");
		if (pCurEl==NULL)
			continue;
		
		mcglQ_state curState=(mcglQ_state)atol(pCurEl->GetText());
		
		if ((curState==Pending) || (curState==error))
		{
			bPending=true;
			g_pMcglData->SetPending();
		}
		
		if (curState==state)
		{
			mcglQ_item CurItem;
			if (ItemFromElement(pEl,&CurItem)!=XML_SUCCESS)
				continue;
			
			if (CurItem.sSrcFile=="mcglProp.xml")
			{
				LOG(INFO) << "New Prop File";
				mcglProps.loadProps();
				removeItem(CurItem.transID);
				return false;
			}
			if (CurItem.fileSize==0)
				continue;
			
			if (CurItem.createTime==0)
				continue;
				
			std::string sExt;
			if (CurItem.sSrcFile.rfind('.')!=std::string::npos)
			{
				sExt = CurItem.sSrcFile.substr(CurItem.sSrcFile.rfind('.')+1);
				std::transform(sExt.begin(), sExt.end(), sExt.begin(), ::tolower);
			}
		
			propExtension propExt;
			mcglProps.GetExtension(sExt,&propExt);
			
			//LOG(INFO) << "file " << CurItem.sSrcFile << " ext " << propExt.sExt << " type " << propExt.type;
			time_t gmtTime=getGmtTime();
			int iDeltaSec=(int)difftime(gmtTime,CurItem.modifyTime);
			
			//LOG(INFO) << "file " << CurItem.sSrcFile << " Time Delta " <<  iDeltaSec << " Size Delta " << (CurItem.fileSize-CurItem.createSize);
			
			if (iDeltaSec>(int)propExt.uEofTime)
			{
				LOG(INFO) << "file " << CurItem.sSrcFile << " EOF!";
				*pItem=CurItem;
				return true;
			}
				
			if (propExt.type==bulk)
				continue;
			
			if (propExt.uMaxHoldTime>0)
			{
				iDeltaSec=(int)difftime(gmtTime,CurItem.createTime);
				//LOG(INFO) << "Delta Time "<< iDeltaSec;
				if (iDeltaSec>(int)propExt.uMaxHoldTime)
				{
					LOG(INFO) << "file " << CurItem.sSrcFile << " MaxHoldTime pass";
					*pItem=CurItem;
					return true;
				}
			}
			
			
			
			if (propExt.uMaxHoldSize>0)
			{
				if (abs((int)CurItem.fileSize-(int)CurItem.createSize)>(int)propExt.uMaxHoldSize)
				{
					LOG(INFO) << "file " << CurItem.sSrcFile << " SizeDelta > MaxHoldSize";
					*pItem=CurItem;
					return true;
				}
			}
			
			
		}
	}	
	if (!bPending)
		g_pMcglData->SetPending(false);
	return false;
}

XMLError CMcglXml::removeItem(uuid_t transID)
{
	Refresh();
	tinyxml2::XMLElement* pEl = findItemByTransID(transID);
	if (pEl==NULL)
	{
		return XML_NO_ATTRIBUTE;
	}
	tinyxml2::XMLElement *pItems=pEl->Parent()->ToElement();
	LOG(INFO) << "CMcglXml::removeItem item " << (int)pItems;
	if (pItems==NULL)
	{
		return XML_NO_ATTRIBUTE;
	}
	pItems->DeleteChild(pEl);
	
	return xmlDoc.SaveFile(sXmlPath.c_str());
}

time_t CMcglXml::getGmtTime()
{
	time_t now;
	time(&now);
	return now;
	//tm local=*gmtime(&now);// convert curr to GMT, store as tm
	//return (mktime(&local));// convert GMT tm to GMT time_t
}

std::string CMcglXml::buildFtpName(mcglQ_item* pItem)
{
	std::string sNew;
	std::string sFolder=pItem->sSrcPath.substr(mcglProps.watchFolder.size());
	if (!sFolder.empty())
	{
		if ((char)(*sFolder.rbegin())=='/')
		{
			sFolder.resize (sFolder.size () - 1);
		}
	}

	std::replace( sFolder.begin(), sFolder.end(), '/', '#'); 
	//sFolder.erase(std::remove(sFolder.begin(), sFolder.end(), '/'), sFolder.end());
	std::string sName = pItem->sSrcFile.substr(0,pItem->sSrcFile.find('.'));
	std::string sExt;
	if (pItem->sSrcFile.rfind('.')!=std::string::npos)
		sExt = pItem->sSrcFile.substr(pItem->sSrcFile.rfind('.'));
	char cTime[18];
	memset(cTime,0,18);
	strftime(cTime, 18, "%Y%m%d%H%M%S", std::localtime(&pItem->createTime));
	std::string sTime=cTime;
	sNew=mcglProps.hostName+"_"+sFolder+"_"+sName+"_"+sTime+sExt;
	return sNew;

}