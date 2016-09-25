

#include <linux/fs.h>
#include <sys/ioctl.h>
#include <linux/mmc/ioctl.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include "mcgl_fat32.h"
#include "utf8.h"

#include "mcgl_rest.h"

static const uint8_t LfnOfs[] = {1,3,5,7,9,14,16,18,20,22,24,28,30};	/* Offset of LFN characters in the directory entry */
CMcglFat32::CMcglFat32(void)
{
	fd=0;
	uSizeSum=0;
	bUpdateSizeFile=false;
	memset(lfnBuff,0,sizeof(wchar_t)*255);
	XMLError err=mcglXml.OpenXml(sExPath+"mcglQ.xml");
	if (err==XML_ERROR_FILE_NOT_FOUND)
	{
		err=mcglXml.createXml();
		if (err)
		{
			LOG(ERROR)<<"can't create xml err=" << err;
		}
	}
}

CMcglFat32::~CMcglFat32()
{
	closeCard();
}

int CMcglFat32::openCard(char* sCard)
{
	
	fd=open(sCard,O_RDONLY);
	if (fd==-1) 
	{
		LOG(ERROR) << "Open card error \"" << strerror(errno) <<"\"";
		fd=0;
		return -1;
	}

	/*uint64_t size=0;
	if (ioctl(fd,BLKGETSIZE,&size)==-1) 
	{
		printf("%s",strerror(errno));
	}*/

	getBootSectorData();

	return 0;
}

void CMcglFat32::closeCard()
{
	if (fd==0)
		return;
	close(fd);
	fd=0;
}
int CMcglFat32::ReadSingleBlock(uint32_t ulStartBlock,uint8_t* pBuffer)
{
	int ret=0;
	//printf("\n low level reading at address %x. ", ulStartBlock);
	if (pBuffer==NULL)
		pBuffer=buffer;
	memset(buffer, 0, sizeof(__u8) * 512);
	
	lseek(fd,ulStartBlock*512,SEEK_SET);
	ret = read(fd,pBuffer,512);
	/*if (ret<=0)
		perror("ioctl");
	else 
		printf("succcess");*/
	
	return ret;
}

//***************************************************************************
//Function: to read data from boot sector of SD card, to determine important
//parameters like bytesPerSector, sectorsPerCluster etc.
//Arguments: none
//return: none
//***************************************************************************
uint8_t CMcglFat32::getBootSectorData(void)
{
	struct BS_Structure *bpb; //mapping the buffer onto the structure
	struct MBRinfo_Structure *mbr;
	struct partitionInfo_Structure *partition;
	uint32_t dataSectors;

	unusedSectors = 0;

	ReadSingleBlock(0);
	bpb = (struct BS_Structure *)buffer;

	if(bpb->jumpBoot[0]!=0xE9 && bpb->jumpBoot[0]!=0xEB)   //check if it is boot sector
	{
	  mbr = (struct MBRinfo_Structure *) buffer;       //if it is not boot sector, it must be MBR
	  
	  if(mbr->signature != 0xaa55) return 1;       //if it is not even MBR then it's not FAT32
		
	  partition = (struct partitionInfo_Structure *)(mbr->partitionData);//first partition
	  unusedSectors = partition->firstSector; //the unused sectors, hidden to the FAT
	  
	  ReadSingleBlock(partition->firstSector);//read the bpb sector
	  bpb = (struct BS_Structure *)buffer;
	  if(bpb->jumpBoot[0]!=0xE9 && bpb->jumpBoot[0]!=0xEB) return 1; 
	}

	bytesPerSector = bpb->bytesPerSector;
	//transmitHex(INT, bytesPerSector); transmitByte(' ');
	sectorPerCluster = bpb->sectorPerCluster;
	//transmitHex(INT, sectorPerCluster); transmitByte(' ');
	reservedSectorCount = bpb->reservedSectorCount;
	rootCluster = bpb->rootCluster;// + (sector / sectorPerCluster) +1;
	firstDataSector = /*bpb->hiddenSectors +*/reservedSectorCount + (bpb->numberofFATs * bpb->FATsize_F32);
	//firstDataSector-=2048;
	dataSectors = bpb->totalSectors_F32
				  - bpb->reservedSectorCount
				  - ( bpb->numberofFATs * bpb->FATsize_F32);
	totalClusters = dataSectors / sectorPerCluster;
	//transmitHex(LONG, totalClusters); transmitByte(' ');
	utotalSize=dataSectors*bytesPerSector;
	if((getFreeCluster (TOTAL_FREE)) > totalClusters)  //check if FSinfo free clusters count is valid
		 freeClusterCountUpdated = 0;
	else
		 freeClusterCountUpdated = 1;
	return 0;
}


//********************************************************************************************
//Function: to get or set next free cluster or total free clusters in FSinfo sector of SD card
//Arguments: 1.flag:TOTAL_FREE or NEXT_FREE, 
//			 2.flag: GET or SET 
//			 3.new FS entry, when argument2 is SET; or 0, when argument2 is GET
//return: next free cluster, if arg1 is NEXT_FREE & arg2 is GET
//        total number of free clusters, if arg1 is TOTAL_FREE & arg2 is GET
//		  0xffffffff, if any error or if arg2 is SET
//********************************************************************************************
uint32_t CMcglFat32::getFreeCluster(uint8_t totOrNext)
{
	struct FSInfo_Structure *FS = (struct FSInfo_Structure *) &buffer;


	ReadSingleBlock(unusedSectors + 1);

    if((FS->leadSignature != 0x41615252) || (FS->structureSignature != 0x61417272) || (FS->trailSignature !=0xaa550000))
		return 0xffffffff;


   if(totOrNext == TOTAL_FREE)
      return(FS->freeClusterCount);
  
	return(FS->nextFreeCluster);
 


}

//***************************************************************************
//Function: get cluster entry value from FAT to find out the next cluster in the chain
//or set new cluster entry in FAT
//Arguments: 1. current cluster number, 2. get_set (=GET, if next cluster is to be found or = SET,
//if next cluster is to be set 3. next cluster number, if argument#2 = SET, else 0
//return: next cluster number, if if argument#2 = GET, else 0
//****************************************************************************
uint32_t CMcglFat32::getNextCluster (uint32_t clusterNumber)
{
unsigned int FATEntryOffset;
uint32_t *FATEntryValue;
uint32_t FATEntrySector;
uint8_t retry = 0;

//get sector number of the cluster entry in the FAT
FATEntrySector = unusedSectors + reservedSectorCount + ((clusterNumber * 4) / bytesPerSector) ;

//get the offset address in that sector number
FATEntryOffset = (unsigned int) ((clusterNumber * 4) % bytesPerSector);

//read the sector into a buffer
while(retry <10)
{
	if(ReadSingleBlock(FATEntrySector)) 
		break; 
	retry++;
}

//get the cluster address from the buffer
FATEntryValue = (uint32_t *) &buffer[FATEntryOffset];

return ((*FATEntryValue) & 0x0fffffff);

}

//***************************************************************************
//Function: to calculate first sector address of any given cluster
//Arguments: cluster number for which first sector is to be found
//return: first sector address
//***************************************************************************
uint32_t CMcglFat32::getFirstSector(uint32_t clusterNumber)
{
  return (((clusterNumber - 2) * sectorPerCluster) + firstDataSector);
}



int CMcglFat32::fillTree(uint32_t ulCluster,dir_tree* pTree,string sPath)
{
	uint32_t cluster, sector, firstSector;
	struct dir_Structure *dir;
	unsigned int i;
	uint8_t curBuf[512];
	uSizeSum = 0;
	
cluster = ulCluster; //root cluster

while(1)
{
   firstSector = getFirstSector (cluster);

   for(sector = 0; sector < sectorPerCluster; sector++)
   {
     ReadSingleBlock (firstSector + sector,curBuf);
	
		
     for(i=0; i<bytesPerSector; i+=32)
     {
			dir = (struct dir_Structure *) &curBuf[i];

			if(dir->name[0] == EMPTY) //indicates end of the file list of the directory
			{
			 
			  return 0;   
			}
			if (dir->attrib==0x08)
			{
				//memset(lfnBuff,0,sizeof(lfnBuff));
				continue;
			}
			
			//memset(lfnBuff,0,sizeof(lfnBuff));

			if (dir->attrib == ATTR_LONG_NAME)
			{
				
				pickLFN(lfnBuff,&curBuf[i]);
			}else if(dir->name[0] != DELETED)
			{
				bool bFile = ((dir->attrib != 0x10) && (dir->attrib != 0x08));
				if ((!bFile) && (dir->name[0]=='.'))
					continue;
				
					
				
				pTree->childs.push_back(dir_tree());
				 
				
				dir_tree* pCurTree=&pTree->childs[pTree->childs.size()-1];
				
				fatDir2mcglDir(dir,&pCurTree->curDir);
				pCurTree->curDir.path=sPath;
				{
					//wstring tmpW=lfnBuff;
					pCurTree->curDir.lfnName=UTF32to8(lfnBuff);
					if (pCurTree->curDir.lfnName.size()==0)
					{
						std::string sFileName=pCurTree->curDir.name;
						sFileName=sFileName.substr(0,8);
						sFileName.erase(0, sFileName.find_first_not_of(' '));       //prefixing spaces
						sFileName.erase(sFileName.find_last_not_of(' ')+1);         //surfixing spaces

						if (bFile)
						{
							std::string sExt = pCurTree->curDir.name;
							sExt=sExt.substr(8);
							sExt.erase(0, sExt.find_first_not_of(' '));       //prefixing spaces
							sExt.erase(sExt.find_last_not_of(' ')+1);         //surfixing spaces
							sFileName+="."+sExt;
						}
						
						std::transform(sFileName.begin(), sFileName.end(), sFileName.begin(), ::tolower);
						pCurTree->curDir.lfnName=sFileName;
					}
					memset(lfnBuff,0,sizeof(lfnBuff));
				}
				
				if (bFile)
				{
					uSizeSum+=pCurTree->curDir.fileSize;
					VLOG(2) << "FILE " << pCurTree->curDir.lfnName << "  "  << pCurTree->curDir.fileSize << "B " << pCurTree->curDir.writeTime;
				}else
					VLOG(2) << "DIR " << pCurTree->curDir.lfnName;
					
				
				
				if ((dir->attrib == 0x10) && (dir->name[0]!='.'))
				{
					uint32_t ulstartCluster = (((uint32_t) dir->firstClusterHI) << 16) | dir->firstClusterLO;
					std::string sNextPath=sPath+pCurTree->curDir.lfnName+"/";
					fillTree(ulstartCluster,pCurTree,sNextPath);
				}
				 
			}else	
				memset(lfnBuff,0,sizeof(lfnBuff));
       }
     }
   }

   cluster = (getNextCluster (cluster));

   if(cluster > 0x0ffffff6)
   	 return 0;
   if(cluster == 0) 
   {printf("Error in getting cluster");  return 0;}
 
}

int CMcglFat32::pickLFN (			/* 1:succeeded, 0:buffer overflow or invalid LFN entry */
	wchar_t* lfnbuf,		/* Pointer to the LFN working buffer */
	uint8_t* dir			/* Pointer to the LFN entry */
)
{
	unsigned int i, s;
	wchar_t wc, uc;

	//struct lfn_Structure* LFN = (struct lfn_Structure*)dir;
	if ((*(dir + LDIR_FstClusLO)) != 0) return 0;	/* Check LDIR_FstClusLO */

	i = ((dir[LDIR_Ord] & 0x3F) - 1) * 13;	/* Offset in the LFN buffer */
	for (wc = 1, s = 0; s < 13; s++) {		/* Process all characters in the entry */
		uc = uint16_t(*(dir + LfnOfs[s]));		/* Pick an LFN character */
		if (wc) {
			//if (i >= _MAX_LFN) return 0;	/* Buffer overflow? */
			lfnbuf[i++] = wc = uc;			/* Store it */
		} else {
			if (uc != 0xFFFF) return 0;		/* Check filler */
		}
	}

	if (dir[LDIR_Ord] & LLEF) {				/* Put terminator if it is the last LFN part */
		if (i >= _MAX_LFN) return 0;		/* Buffer overflow? */
		lfnbuf[i] = (wchar_t)0;
		
	}
	
	return 1;		/* The part of LFN is valid */
}

time_t CMcglFat32::fatTime2tm(uint16_t date,uint16_t time)
{
	tm sTm;
	memset(&sTm,0,sizeof(tm));
	
	sTm.tm_mday = date & 0x1F;
	sTm.tm_mon = ((date>>5) & 0xF)-1;
	sTm.tm_year = (1980+(date>>9))-1900;
	sTm.tm_sec = (time & 0x1F)*2;
	sTm.tm_min = (time>>5) & 0x3F;
	sTm.tm_hour = (time>>11) & 0x1F;
	
	return mktime(&sTm);
}

int CMcglFat32::copyFile(mcgl_dir* dir,const char* dstName)
{
	LOG(INFO) << "copyFile dst=" << dstName;
	uint32_t firstSector=0;
	uint32_t cluster = dir->firstCluster;
	uint8_t curBuf[512];
	uint32_t ulSizeLeft=dir->fileSize;
	uint32_t ulcurRead=0;
	uint32_t ulActWrite=0;
	
	FILE* fd = NULL;
	fd = fopen(dstName,"wb");

	if(NULL == fd)
    {
        LOG(ERROR) << "copyFile fopen Failed file=" << dstName << " error=" << errno;
        return 1;
    }

	while(1)
	{
		firstSector = getFirstSector (cluster);

		for(unsigned int j=0; j<sectorPerCluster; j++)
		{
			ulcurRead=ReadSingleBlock(firstSector + j,curBuf);
			ulActWrite = min(ulcurRead,ulSizeLeft);
			fwrite(curBuf,1,ulActWrite,fd);
			ulSizeLeft-=ulActWrite;
			if (ulSizeLeft==0)
			{
				fclose(fd);
				return 0;
			}
		}
		cluster = getNextCluster (cluster);
		if(cluster == 0)
		{
			LOG(ERROR) << "copyFile cluster file=" << dstName;
			fclose(fd);
			return 1;
		}
	}
	return 0;
}

int CMcglFat32::scanTree()
{
	if (rootDir.childs.size()==0)
	{
		fillTree(2,&rootDir);
		return 1;
	}
	
	dir_tree curTree;
	fillTree(2,&curTree);
	
	cmpDirTree(&rootDir,&curTree);
	rootDir=curTree;
	if (bUpdateSizeFile)
	{
		bUpdateSizeFile=false;
		WriteSize();
	}
	return 0;
		
}

int CMcglFat32::cmpDirTree(dir_tree* pTreeA,dir_tree* pTreeB,bool bNewFolder)
{
	
	if (!bNewFolder)
	{
		for (std::vector<dir_tree>::iterator it = pTreeA->childs.begin() ; it != pTreeA->childs.end(); ++it)
		{
			if (it->curDir.lfnName.size()==0)
				continue;
			if (it->curDir.attrib==ATTR_DIRECTORY)
			{
				if (it->curDir.lfnName.at(0)=='.')
					continue;
				if (it->curDir.path.compare(0,mcglProps.watchFolder.size(),mcglProps.watchFolder))
					continue;
				dir_tree* pDir2=findDirTree(&pTreeB->childs,it->curDir.lfnName,it->curDir.attrib);
				if (pDir2)
					cmpDirTree((dir_tree*)&(*it),pDir2);
			}else
			{
				if (it->curDir.path.compare(0,mcglProps.watchFolder.size(),mcglProps.watchFolder))
					continue;
					
				if (it->curDir.lfnName.at(0)=='.')
					continue;
					
				if (it->curDir.lfnName.find_first_of('~')!=string::npos)
					continue;
				
				
				dir_tree* pFile2=findDirTree(&pTreeB->childs,it->curDir.lfnName,it->curDir.attrib);
				if (pFile2)
				{
					if ((pFile2->curDir.fileSize!=it->curDir.fileSize) || (pFile2->curDir.writeTime!=it->curDir.writeTime))
					{
						if (pFile2->curDir.fileSize!=it->curDir.fileSize)
						{
							LOG(INFO) << "File changed (size) " << pFile2->curDir.path <<  pFile2->curDir.lfnName;
						}else
							LOG(INFO) << "File changed (date) " << pFile2->curDir.path <<pFile2->curDir.lfnName;
						
						if (pFile2->curDir.fileSize==0)
						{
							LOG(INFO) << "File No Size Skipped " << pFile2->curDir.path <<  pFile2->curDir.lfnName;
						}else
							addFileToXml(pFile2);
						
					}
				}else
				{
					LOG(INFO) << "File deleted " << it->curDir.path << it->curDir.lfnName;
					bUpdateSizeFile=true;
				}
			}
		}
	
	}
	
	
	
	for (std::vector<dir_tree>::iterator it = pTreeB->childs.begin() ; it != pTreeB->childs.end(); ++it)
	{
		if (it->curDir.lfnName.size()==0)
			continue;
		
		if (it->curDir.lfnName.at(0)=='.')
			continue;
		if (it->curDir.lfnName.find_first_of('~')!=string::npos)
			continue;
		dir_tree* pFile1=findDirTree(&pTreeA->childs,it->curDir.lfnName,it->curDir.attrib);
		if (pFile1==NULL)
		{
			if (it->curDir.attrib!=ATTR_DIRECTORY)
			{
				if ((it->curDir.lfnName=="mcglProp.xml") && (it->curDir.path=="/"))
				{
					LOG(INFO) << "Prop File added " << it->curDir.path << it->curDir.lfnName;
					std::string sPropPath=mcglProps.getPropPath();
					copyFile(&it->curDir,sPropPath.c_str());
					mcglProps.loadProps();
					addFileToXml((dir_tree*)&(*it),true,true);
					continue;
					
				}
				LOG(INFO) << "File added " << it->curDir.path << it->curDir.lfnName << " size=" << it->curDir.fileSize;
				//if (it->curDir.fileSize>0)
				addFileToXml((dir_tree*)&(*it),true);
			}else if (!it->childs.empty())
			{
				dir_tree empTree;
				cmpDirTree(&empTree,(dir_tree*)&(*it),true);
				LOG(INFO) << "Folder added " << it->curDir.path << it->curDir.lfnName;
			}
								
		}
		
	}
	return 0;
}

dir_tree* CMcglFat32::findDirTree(vector<dir_tree>* pTree,const string& sSrc,uint8_t attrib)
{
	for (std::vector<dir_tree>::iterator it = pTree->begin() ; it != pTree->end(); ++it)
	{
		if ((!sSrc.compare(it->curDir.lfnName)) && (it->curDir.attrib==attrib))
			return (dir_tree*)&(*it);
	}
	return NULL;
}

void CMcglFat32::fatDir2mcglDir(dir_Structure* pSrcDir,mcgl_dir* pDstDir)
{
	pDstDir->attrib = pSrcDir->attrib;
	
	pDstDir->name.assign(12,'\0');
	pDstDir->name.assign((char*)pSrcDir->name,11);
	
	pDstDir->lastAccessTime = fatTime2tm(pSrcDir->lastAccessDate,(uint16_t)0);
	pDstDir->createTime = fatTime2tm(pSrcDir->createDate,pSrcDir->createTime);
	pDstDir->writeTime = fatTime2tm(pSrcDir->writeDate,pSrcDir->writeTime);
	pDstDir->fileSize = pSrcDir->fileSize;
	pDstDir->firstCluster = (((uint32_t) pSrcDir->firstClusterHI) << 16) | pSrcDir->firstClusterLO;
}

void CMcglFat32::addFileToXml(dir_tree* pFile,bool bAdded,bool bSys)
{
	mcglQ_item xmlItem;
	xmlItem.resumes=0;
	
	bUpdateSizeFile=true;
	
	g_pMcglData->SetDetected();
	bool bNewItem=true;
	if (!bSys)
	{
		if (!mcglXml.findFile(&xmlItem,pFile->curDir.path,pFile->curDir.lfnName))
		{
			if (xmlItem.state==Pending)
				bNewItem=false;
		}
	}else
		bNewItem=true;
	
	
	if (!bNewItem)
	{
		bool bUpdateItem=false;
		if (pFile->curDir.fileSize>0)
		{
			if (copyFile(&pFile->curDir,xmlItem.sTmpFile.c_str()))
			{
				bNewItem=true;
			}else
			{
				bUpdateItem=true;
			}
		}else
			bUpdateItem=true;
		
		if (xmlItem.status!=Premeasure)
		{
			CMcglRest rest;
			if (rest.sendRestPremeasurements(&xmlItem))
			{
				xmlItem.status=Premeasure;
				if (!bUpdateItem)
				{
					mcglXml.updateItem(&xmlItem);
				}
			}
			
		}
		if (bUpdateItem)
		{
			//xmlItem.itemTime=pFile->curDir.writeTime;
			xmlItem.modifyTime=CMcglXml::getGmtTime();
			xmlItem.fileSize=pFile->curDir.fileSize;
			mcglXml.updateItem(&xmlItem);
		}
	}

	
	if (bNewItem)
	{
		uuid_generate_time_safe(xmlItem.transID);
		char uuid_str[37];
		uuid_unparse_lower(xmlItem.transID,uuid_str);
		xmlItem.sSrcPath=pFile->curDir.path;
		//xmlItem.sSrcPath.erase(0, xmlItem.sSrcPath.find_first_not_of('/'));       //prefixing spaces
		//xmlItem.sSrcPath.erase(xmlItem.sSrcPath.find_last_not_of('/')+1);         //surfixing spaces
		LOG(INFO) << "add to xml " << xmlItem.sSrcPath;
		xmlItem.sSrcFile=pFile->curDir.lfnName;
		xmlItem.fileSize=pFile->curDir.fileSize;
		xmlItem.createSize=pFile->curDir.fileSize;
		/*if (bAdded)
			xmlItem.itemTime=pFile->curDir.createTime;
		else
			xmlItem.itemTime=pFile->curDir.writeTime;*/
		xmlItem.createTime=CMcglXml::getGmtTime();
		xmlItem.modifyTime=xmlItem.createTime;
		xmlItem.state=Pending;
		xmlItem.status=Detected;
		xmlItem.sTmpFile=mcglProps.tmpFolder;
		xmlItem.sTmpFile+=uuid_str;
		xmlItem.sFtpFile=CMcglXml::buildFtpName(&xmlItem);
		LOG(INFO) << "addFileToXml before copy size="<<xmlItem.fileSize<<" sys="<<bSys;
		if (xmlItem.fileSize>0)
		{
			if (!bSys)
			{
				if (copyFile(&pFile->curDir,xmlItem.sTmpFile.c_str()))
				{
					LOG(ERROR) << "addFileToXml FAILED to copy new temp " << xmlItem.sTmpFile.c_str();
					return;
				}
				if (bAdded)
				{
					CMcglRest rest;
					if (rest.sendRestPremeasurements(&xmlItem))
					{
						xmlItem.status=Premeasure;
					}
				}
			}
			
			mcglXml.addItem(&xmlItem);
		}
		
		
	}
	
}

void CMcglFat32::WriteSize()
{
	FILE *fp = NULL;
	std::string sSizeFile=sExPath+"partsize";
    fp = fopen(sSizeFile.c_str(),"w");
	if (fp != NULL)
	{  
			fprintf(fp, "%llu\n%llu\n%llu\n",utotalSize,uSizeSum,utotalSize-uSizeSum);                 
	}
	fclose(fp);
}

