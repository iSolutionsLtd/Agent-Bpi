#pragma GCC diagnostic ignored "-Wwrite-strings"

#include "mcgl_uploader.h"
#include "timerClass.h"


_INITIALIZE_EASYLOGGINGPP;

std::string sExPath;
CMcglProp mcglProps;
CMCGLSharedMemory *g_pMcglData=NULL;


int main(int argc, char **argv)
{

	_START_EASYLOGGINGPP(argc, argv);
	easyloggingpp::Configurations c;
	c.setToDefault();
	easyloggingpp::Loggers::setFilename("/var/log/mcgl_uploader.log");
	c.setAll(easyloggingpp::ConfigurationType::ToFile,"1");
	easyloggingpp::Loggers::reconfigureLogger("default", c);
	
	LOG(INFO) << "MCGL uploader started";
	
	setlocale(LC_CTYPE, "en_US.UTF-8") ;
	
	sExPath=argv[0];
	sExPath=sExPath.substr(0,sExPath.rfind('/')+1);
	
	mcglProps.loadProps();
	g_pMcglData = new CMCGLSharedMemory;
	
	CMcglUploader uploader;
	
	while(1)
	{
		if (g_pMcglData->GetUploadNewParams())
		{
			g_pMcglData->SetUploadNewParams(false);
			mcglProps.loadProps();
		}
		uploader.UploadNext();
		sleep(mcglProps.uploadLoopTimeSec);
	}
	if (g_pMcglData)
		delete g_pMcglData;
	return 0;
}
