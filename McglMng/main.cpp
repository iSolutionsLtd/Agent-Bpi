#include <stdio.h>

_INITIALIZE_EASYLOGGINGPP;

CMcglHW mcglHW;
CMCGLSharedMemory* g_pMcglData=NULL;

int main(int argc, char **argv)
{
	_START_EASYLOGGINGPP(argc, argv);
	easyloggingpp::Configurations c;
	c.setToDefault();
	easyloggingpp::Loggers::setFilename("/var/log/mcglMng.log");
	c.setAll(easyloggingpp::ConfigurationType::ToFile,"1");
	easyloggingpp::Loggers::reconfigureLogger("default", c);
	
	LOG(INFO) << "argc="<<argc<<"argv[1]="<<argv[1];
	setlocale(LC_CTYPE, "en_US.UTF-8") ;
	g_pMcglData = new CMCGLSharedMemory;
	if ((argc>1) && (string(argv[1])=="p"))
	{
		LOG(INFO) << "McglMng Set New Properties";
		g_pMcglData->SetScanNewParams();
		g_pMcglData->SetUploadNewParams();
		if (g_pMcglData)
			delete g_pMcglData;
		return 0;
		
		
	}
	mcglHW.startHW();
		
	while(1)
	{
		if (g_pMcglData->GetDetected())
		{
			mcglHW.setPendingLed(true,10);
			g_pMcglData->SetDetected(false);
		}else if (mcglHW.getPendingLed()!=2)
		{
			mcglHW.setPendingLed(g_pMcglData->GetPending());
		}
		
		if (g_pMcglData->GetUploadingError())
		{
			mcglHW.setUploadLed(true,40,true);
			g_pMcglData->SetUploadingError(false);
		}else if (g_pMcglData->GetUploading())
		{
			mcglHW.setUploadLed(true,20);
		}else if (mcglHW.getUploadLed()==2)
		{
			mcglHW.setUploadLed(false,1);
		}else if (mcglHW.getUploadLed()!=3)
		{
			mcglHW.setUploadLed(false);
		}
		sleep(2);
	}
	
	mcglHW.stopHW();
	if (g_pMcglData)
		delete g_pMcglData;
	return 0;
}
