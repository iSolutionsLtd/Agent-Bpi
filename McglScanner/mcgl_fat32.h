#ifndef MCGL_FAT32
#define MCGL_FAT32

#include "mcgl_xml.h"

#define MAX_PATH 255


using namespace std;
//Structure to access Master Boot Record for getting info about partioions

struct MBRinfo_Structure{
uint8_t	nothing[446];		//ignore, placed here to fill the gap in the structure
uint8_t	partitionData[64];	//partition records (16x4)
uint16_t	signature;		//0xaa55
};

//Structure to access info of the first partioion of the disk 
struct partitionInfo_Structure{ 				
uint8_t	status;				//0x80 - active partition
uint8_t 	headStart;			//starting head
uint16_t	cylSectStart;		//starting cylinder and sector
uint8_t	type;				//partition type 
uint8_t	headEnd;			//ending head of the partition
uint16_t	cylSectEnd;			//ending cylinder and sector
uint32_t	firstSector;		//total sectors between MBR & the first sector of the partition
uint32_t	sectorsTotal;		//size of this partition in sectors
};

//Structure to access boot sector data
struct BS_Structure{
uint8_t jumpBoot[3]; //default: 0x009000EB
uint8_t OEMName[8];
be_uint16_t bytesPerSector; //deafault: 512
uint8_t sectorPerCluster;
be_uint16_t reservedSectorCount;
uint8_t numberofFATs;
be_uint16_t rootEntryCount;
be_uint16_t totalSectors_F16; //must be 0 for FAT32
uint8_t mediaType;
be_uint16_t FATsize_F16; //must be 0 for FAT32
be_uint16_t sectorsPerTrack;
be_uint16_t numberofHeads;
uint32_t hiddenSectors;
uint32_t totalSectors_F32;
uint32_t FATsize_F32; //count of sectors occupied by one FAT
be_uint16_t extFlags;
be_uint16_t FSversion; //0x0000 (defines version 0.0)
uint32_t rootCluster; //first cluster of root directory (=2)
be_uint16_t FSinfo; //sector number of FSinfo structure (=1)
be_uint16_t BackupBootSector;
uint8_t reserved[12];
uint8_t driveNumber;
uint8_t reserved1;
uint8_t bootSignature;
uint32_t volumeID;
uint8_t volumeLabel[11]; //"NO NAME "
uint8_t fileSystemType[8]; //"FAT32"
uint8_t bootData[420];
be_uint16_t bootEndSignature; //0xaa55
};


//Structure to access FSinfo sector data
struct FSInfo_Structure
{
be_uint32_t leadSignature; //0x41615252
uint8_t reserved1[480];
be_uint32_t structureSignature; //0x61417272
be_uint32_t freeClusterCount; //initial: 0xffffffff
be_uint32_t nextFreeCluster; //initial: 0xffffffff
uint8_t reserved2[12];
be_uint32_t trailSignature; //0xaa550000
};

//Structure to access Directory Entry in the FAT
struct dir_Structure{
uint8_t name[11];
uint8_t attrib; //file attributes
uint8_t NTreserved; //always 0
uint8_t timeTenth; //tenths of seconds, set to 0 here
be_uint16_t createTime; //time file was created
be_uint16_t createDate; //date file was created
be_uint16_t lastAccessDate;
be_uint16_t firstClusterHI; //higher word of the first cluster number
be_uint16_t writeTime; //time of last write
be_uint16_t writeDate; //date of last write
be_uint16_t firstClusterLO; //lower word of the first cluster number
uint32_t fileSize; //size of file in bytes
};

struct mcgl_dir
{
	string path;
	string name;
	string lfnName;
	uint8_t attrib;
	time_t createTime;
	time_t lastAccessTime;
	time_t writeTime;
	uint32_t fileSize; //size of file in bytes
	uint32_t firstCluster;
};

typedef struct lfn_Structure
{
   uint8_t sequenceNo;            // Sequence number, 0xe5 for deleted entry
   uint8_t fileName_Part1[10];    // file name part
   uint8_t fileattribute;         // File attibute
   uint8_t reserved_1;
   uint8_t checksum;              // Checksum
   uint8_t fileName_Part2[12];    // WORD reserved_2;
   uint8_t fileName_Part3[4];
}LFN;

typedef struct _FileMarker_Part2
{
   uint32_t _Mark1;
   uint32_t _Mark2;
   uint32_t _Mark3;
}FMark;

struct dir_tree
{
	mcgl_dir curDir;
	vector<dir_tree> childs;
};
//Attribute definitions for file/directory
#define ATTR_READ_ONLY     0x01
#define ATTR_HIDDEN        0x02
#define ATTR_SYSTEM        0x04
#define ATTR_VOLUME_ID     0x08
#define ATTR_DIRECTORY     0x10
#define ATTR_ARCHIVE       0x20
#define ATTR_LONG_NAME     0x0f


#define DIR_ENTRY_SIZE     0x32
#define EMPTY              0x00
#define DELETED            0xe5
#define GET     0
#define SET     1
#define READ	0
#define VERIFY  1
#define ADD		0
#define REMOVE	1
#define LOW		0
#define HIGH	1	
#define TOTAL_FREE   1
#define NEXT_FREE    2
#define GET_LIST     0
#define GET_FILE     1
#define DELETE		 2
#define FAT_EOF		0x0fffffff

#define	LDIR_Ord			0		/* LFN entry order and LLE flag (1) */
#define	LDIR_FstClusLO		0x1A		/* Must be zero (WORD) */
#define	LLEF				0x40	/* Last long entry flag in LDIR_Ord */
#define _MAX_LFN			255

class CMcglFat32
{
public:
	CMcglFat32(void);
	~CMcglFat32();
	uint8_t getBootSectorData (void);
	int openCard(char* sCard);
	void closeCard();
	int ReadSingleBlock(uint32_t ulStartBlock,uint8_t* pBuffer=NULL);
	int fillTree(uint32_t ulCluster,dir_tree* pTree,string sPath="/");
	dir_tree rootDir;
	
	int scanTree();
	void WriteSize();
protected:
	void addFileToXml(dir_tree* pFile,bool bAdded=false,bool bSys=false);
	void fatDir2mcglDir(dir_Structure* pSrcDir,mcgl_dir* pDstDir);
	int copyFile(mcgl_dir* dir,const char* dstName);
	int pickLFN (wchar_t* lfnbuf,uint8_t* dir);
	uint32_t getFirstSector(uint32_t clusterNumber);
	uint32_t getFreeCluster(uint8_t totOrNext);
	uint32_t getNextCluster (uint32_t clusterNumber);
	

	uint32_t firstDataSector, rootCluster, totalClusters;
	uint16_t  bytesPerSector, sectorPerCluster, reservedSectorCount;
	uint32_t unusedSectors, appendFileSector, appendFileLocation, fileSize, appendStartCluster;
	uint8_t freeClusterCountUpdated;
	time_t fatTime2tm(uint16_t date,uint16_t time);
	int cmpDirTree(dir_tree* pTreeA,dir_tree* pTreeB,bool bNewFolder=false);
	dir_tree* findDirTree(vector<dir_tree>* pTree,const string& sSrc,uint8_t attrib);
	
	bool bUpdateSizeFile;
	uint64_t utotalSize;
	uint64_t uSizeSum;
	uint8_t buffer[512];
	wchar_t	lfnBuff[255];
	
	int fd;
	vector<dir_tree> diffQueue;
	CMcglXml mcglXml;
	
};
#endif