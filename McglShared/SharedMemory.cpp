#include "SharedMemory.h"
#include <sys/mman.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
 
 const string CSharedMemory::sLockSemaphoreName = "/semaphoreInit";
CSharedMemory::CSharedMemory( const string& sName ):m_sName(sName),m_iD(-1),m_Ptr(NULL),
   m_SemID(NULL), m_nSize(0)
{
   /**
   * Semaphore open
   */
   m_SemID = sem_open(sLockSemaphoreName.c_str(), O_CREAT, S_IRUSR | S_IWUSR, 1);
}
 
bool CSharedMemory::Create( size_t nSize, int mode /*= READ_WRITE*/ )
{
   m_nSize = nSize;
   m_iD = shm_open(m_sName.c_str(), O_CREAT | mode, S_IRWXU | S_IRWXG);
   if(m_iD < 0)
   {
      switch(errno)
      {
      case EACCES:
         throw CSharedMemoryException("Permission Exception ");
         break;
      case EEXIST:
         throw CSharedMemoryException("Shared memory object specified by name already exists.");
         break;
      case EINVAL:
         throw CSharedMemoryException("Invalid shared memory name passed.");
         break;
      case EMFILE:
         throw CSharedMemoryException("The process already has the maximum number of files open.");
         break;
      case ENAMETOOLONG:
         throw CSharedMemoryException("The length of name exceeds PATH_MAX.");
         break;
      case ENFILE:
         throw CSharedMemoryException("The limit on the total number of files open on the system has been reached");
         break;
      default:
         throw CSharedMemoryException("Invalid exception occurred in shared memory creation");
         break;
      }
   }
   /* adjusting mapped file size (make room for the whole segment to map)      --  ftruncate() */
   ftruncate(m_iD, m_nSize);
 
   return true;
}
 
bool CSharedMemory::Attach( int mode /*= A_READ | A_WRITE*/ )
{
   /* requesting the shared segment    --  mmap() */
   m_Ptr = mmap(NULL, m_nSize, mode, MAP_SHARED, m_iD, 0);
   if (m_Ptr == NULL)
   {
      throw CSharedMemoryException("Exception in attaching the shared memory region");
   }
   return true;
}
 
bool CSharedMemory::Detach()
{
   return !munmap(m_Ptr, m_nSize);
}
 
bool CSharedMemory::Lock()
{
   return !sem_wait(m_SemID);
}
 
bool CSharedMemory::UnLock()
{
   return !sem_post(m_SemID);
}
 
CSharedMemory::~CSharedMemory()
{
   Clear();
}
 
void CSharedMemory::Clear()
{
   if(m_iD != -1)
   {
      if ( shm_unlink(m_sName.c_str()) < 0 )
      {
         perror("shm_unlink");
      }
   }
   /**
   * Semaphore unlink: Remove a named semaphore  from the system.
   */
   if(m_SemID != NULL)
   {
      /**
      * Semaphore Close: Close a named semaphore
      */
      if ( sem_close(m_SemID) < 0 )
      {
         perror("sem_close");
      }
      /**
      * Semaphore unlink: Remove a named semaphore  from the system.
      */
      if ( sem_unlink(sLockSemaphoreName.c_str()) < 0 )
      {
         perror("sem_unlink");
      }
   }
}
 
CSharedMemoryException::CSharedMemoryException( const string &message, bool bSysMsg /*= false*/ ) throw()
{
 
}
 
CSharedMemoryException::~CSharedMemoryException() throw ()
{
 
}

CMCGLSharedMemory::CMCGLSharedMemory()
{
	m_pSharedData=NULL;
	m_pSheraedMem=NULL;
	Connect();
}
CMCGLSharedMemory::~CMCGLSharedMemory()
{
	if (m_pSheraedMem)
	{
		delete m_pSheraedMem;
		m_pSheraedMem=NULL;
	}
}

	
void CMCGLSharedMemory::Connect()
{
	if (m_pSharedData)
	{
		return;
	}
	if (m_pSheraedMem)
	{
		delete m_pSheraedMem;
		m_pSheraedMem=NULL;
	}
	try
	{
		m_pSheraedMem=new CSharedMemory("McglStat");
		m_pSheraedMem->Create(sizeof(McglSharedMem));
		m_pSheraedMem->Attach();
		m_pSharedData = (McglSharedMem*)m_pSheraedMem->GetData();
		LOG(INFO) << "CMCGLSharedMemory::Connect connected";
	} catch (std::exception& ex)
	{
		m_pSharedData=NULL;
		delete m_pSheraedMem;
		m_pSheraedMem=NULL;
		LOG(ERROR)<<"CMCGLSharedMemory::Connect Exception:"<<ex.what();
	}

}

bool CMCGLSharedMemory::IsConnected()
{
		if (m_pSheraedMem==NULL)
		{
			Connect();
			if (m_pSheraedMem==NULL)
				return false;
		}
		return true;
}

void CMCGLSharedMemory::SetPending(bool bPending)
{
	if (!IsConnected())
		return;
	m_pSharedData->bPending=bPending;
}

bool CMCGLSharedMemory::GetPending()
{
	if (!IsConnected())
		return false;
	return m_pSharedData->bPending;
}
void CMCGLSharedMemory::SetDetected(bool bDetected)
{
	if (!IsConnected())
		return;
	m_pSharedData->bDetected=bDetected;
}
bool CMCGLSharedMemory::GetDetected()
{
	if (!IsConnected())
		return false;
	return m_pSharedData->bDetected;
}
void CMCGLSharedMemory::SetUploading(bool bUploading)
{
	if (!IsConnected())
		return;
	m_pSharedData->bUploading=bUploading;
}
bool CMCGLSharedMemory::GetUploading()
{
	if (!IsConnected())
		return false;
	return m_pSharedData->bUploading;
}
void CMCGLSharedMemory::SetScanNewParams(bool bNew)
{
	if (!IsConnected())
		return;
	m_pSharedData->bScanNewParams=bNew;
}
bool CMCGLSharedMemory::GetScanNewParams()
{
	if (!IsConnected())
		return false;
	return m_pSharedData->bScanNewParams;
	
}
void CMCGLSharedMemory::SetUploadNewParams(bool bNew)
{
	if (!IsConnected())
		return;
	m_pSharedData->bUploadNewParams=bNew;
}
bool CMCGLSharedMemory::GetUploadNewParams()
{
	if (!IsConnected())
		return false;
	return m_pSharedData->bUploadNewParams;
}

void CMCGLSharedMemory::SetUploadingError(bool bError)
{
	if (!IsConnected())
		return;
	m_pSharedData->bUploadError=bError;
}

bool CMCGLSharedMemory::GetUploadingError()
{
	if (!IsConnected())
		return false;
	return m_pSharedData->bUploadError;
}