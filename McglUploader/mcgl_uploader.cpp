
#pragma GCC diagnostic ignored "-Wwrite-strings"

#include "mcgl_uploader.h"
#include <openssl/sha.h>



CMcglUploader::CMcglUploader(void)
{
	
	
	mcglXml.OpenXml(sExPath+"mcglQ.xml");
	
	test();
}

CMcglUploader::~CMcglUploader(void)
{

}

int CMcglUploader::test(void)
{

	return 0;
}

int CMcglUploader::UploadNext()
{
	mcglQ_item newItem;
	if (mcglXml.getFirstUpload(&newItem))
	{
		
		completeItem(&newItem);
		LOG(INFO) << "start sending file " << newItem.sFtpFile;
		newItem.state=prepare;
		mcglXml.updateItem(&newItem);
		g_pMcglData->SetUploading();
		if (newItem.status<Prefile)
		{
			if (mcglRest.sendRestPrefile(&newItem))
			{
				newItem.status=Prefile;
				newItem.state=uploading;
				mcglXml.updateItem(&newItem);
			}else
			{
				newItem.state=error;
				mcglXml.updateItem(&newItem);
				g_pMcglData->SetUploadingError();
				g_pMcglData->SetUploading(false);
				return 1;
			}
		}
		
		
		if (newItem.status==Prefile)
		{
			ftplib ftp;
			LOG(INFO) << "ftp connect";
			if (!ftp.Connect(mcglProps.ftp_host.c_str()))
			{
				newItem.state=error;
				mcglXml.updateItem(&newItem);
				g_pMcglData->SetUploadingError();
				g_pMcglData->SetUploading(false);
				LOG(ERROR) << "failed connecting to ftp server " << mcglProps.ftp_host.c_str();
				return 1;
			}
			LOG(INFO) << "ftp login";
			if (!ftp.Login(mcglProps.ftp_user.c_str(), mcglProps.ftp_pass.c_str()))
			{
				newItem.state=error;
				mcglXml.updateItem(&newItem);
				ftp.Quit();
				g_pMcglData->SetUploadingError();
				g_pMcglData->SetUploading(false);
				LOG(ERROR) << "failed ftp server login user " << mcglProps.ftp_user;
				return 1;
			}
			
			LOG(INFO) << "ftp put";
			if (!ftp.Put(newItem.sTmpFile.c_str(),newItem.sFtpFile.c_str(),ftplib::image))
			{
				newItem.state=error;
				mcglXml.updateItem(&newItem);
				ftp.Quit();
				LOG(ERROR) << "failed uploading file " << newItem.sSrcFile << " resumes "<< (int)newItem.resumes;
				g_pMcglData->SetUploadingError();
				g_pMcglData->SetUploading(false);
				return 1;
			}
			
			ftp.Quit();
			remove(newItem.sTmpFile.c_str());
			newItem.status=Uploaded;
			mcglXml.updateItem(&newItem);
		}
		if (newItem.status==Uploaded)
		{
			if (!mcglRest.sendRestFile(&newItem))
			{
				newItem.state=error;
				mcglXml.updateItem(&newItem);
				g_pMcglData->SetUploadingError();
				g_pMcglData->SetUploading(false);
				return 1;
			}
		}		
		mcglXml.removeItem(newItem.transID);
		LOG(INFO) << "finish sending file "<<newItem.sSrcFile;
		g_pMcglData->SetUploading(false);
	}else if (mcglXml.getFirstUpload(&newItem,error))
	{
		if (newItem.resumes<mcglProps.maxResumes)
		{
			completeItem(&newItem);
			LOG(INFO) << "start resending file "<<newItem.sFtpFile;
			newItem.resumes++;
			mcglXml.updateItem(&newItem);
			g_pMcglData->SetUploading();
			
			if (newItem.status<Prefile)
			{
				if (mcglRest.sendRestPrefile(&newItem))
				{
					newItem.status=Prefile;
					newItem.state=uploading;
					mcglXml.updateItem(&newItem);
				}else
				{
					newItem.state=error;
					mcglXml.updateItem(&newItem);
					g_pMcglData->SetUploadingError();
					g_pMcglData->SetUploading(false);
					return 1;
				}
			}
			
			if (newItem.status==Prefile)
			{
				ftplib ftp;
				LOG(INFO) << "ftp connect";
				if (!ftp.Connect(mcglProps.ftp_host.c_str()))
				{
					newItem.state=error;
					mcglXml.updateItem(&newItem);
					g_pMcglData->SetUploadingError();
					g_pMcglData->SetUploading(false);
					LOG(ERROR) << "failed connecting to ftp server " << mcglProps.ftp_host.c_str();
					return 1;
				}
				LOG(INFO) << "ftp login";
				if (!ftp.Login(mcglProps.ftp_user.c_str(), mcglProps.ftp_pass.c_str()))
				{
					newItem.state=error;
					mcglXml.updateItem(&newItem);
					ftp.Quit();
					g_pMcglData->SetUploadingError();
					g_pMcglData->SetUploading(false);
					LOG(ERROR) << "failed ftp server login user " << mcglProps.ftp_user;
					return 1;
				}
				
				LOG(INFO) << "ftp put";
				if (!ftp.Put(newItem.sTmpFile.c_str(),newItem.sFtpFile.c_str(),ftplib::image))
				{
					newItem.state=error;
					mcglXml.updateItem(&newItem);
					ftp.Quit();
					LOG(ERROR) << "failed uploading file " << newItem.sSrcFile << " resumes "<< (int)newItem.resumes;
					g_pMcglData->SetUploadingError();
					g_pMcglData->SetUploading(false);
					return 1;
				}
				
				ftp.Quit();
				remove(newItem.sTmpFile.c_str());
				newItem.status=Uploaded;
				mcglXml.updateItem(&newItem);
			}
			if (newItem.status==Uploaded)
			{
				if (!mcglRest.sendRestFile(&newItem))
				{
					newItem.state=error;
					mcglXml.updateItem(&newItem);
					g_pMcglData->SetUploadingError();
					g_pMcglData->SetUploading(false);
					return 1;
				}
			}		
			mcglXml.removeItem(newItem.transID);
			LOG(INFO) << "finish resending file "<<newItem.sSrcFile;
			g_pMcglData->SetUploading(false);
			return 0;
		}else
		{
			LOG(ERROR) << "failed uploading file " << newItem.sSrcFile << " resumes "<< (int)newItem.resumes;
			mcglXml.removeItem(newItem.transID);
			remove(newItem.sTmpFile.c_str());
			g_pMcglData->SetUploading(false);
		}
		
	}
	return 1;
}

int CMcglUploader::sha1File(const char *name,char* out)
{
    FILE *f;
    unsigned char buf[8192];
    SHA_CTX sc;
    int err;

    f = fopen(name, "rb");
    if (f == NULL) {
        /* do something smart here: the file could not be opened */
        return 1;
    }
    SHA1_Init(&sc);
    for (;;) {
        size_t len;

        len = fread(buf, 1, sizeof buf, f);
        if (len == 0)
            break;
        SHA1_Update(&sc, buf, len);
    }
    err = ferror(f);
    fclose(f);
    if (err) {
        /* some I/O error was encountered; report the error */
        return 1;
    }
	unsigned char sha1[20];
	memset(sha1,0,20);
    SHA1_Final(sha1, &sc);
	
	
	memset(out,0,41);
	for(int i = 0; i<20; ++i)
	{
		sprintf(&out[i*2], "%02X", sha1[i]);
	}
	
    return 0;
}

void CMcglUploader::completeItem(mcglQ_item* pItem)
{
	bool bUpdate=false;
	if (pItem->sFtpFile.size()==0)
	{
		pItem->sFtpFile=CMcglXml::buildFtpName(pItem);
		bUpdate=true;
	}
	
	if (pItem->sSHA1.size()==0)
	{
		LOG(INFO)<<"start SHA1";
		char sha1[41];
		if (!sha1File(pItem->sTmpFile.c_str(),sha1))
		{
			LOG(INFO)<<"SUCCESS SHA1";
			pItem->sSHA1=sha1;
			bUpdate=true;
		}
		LOG(INFO)<<"finish SHA1";
	}
	if (bUpdate)
		mcglXml.updateItem(pItem);
	
}