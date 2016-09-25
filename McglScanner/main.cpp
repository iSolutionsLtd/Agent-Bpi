
#pragma GCC diagnostic ignored "-Wwrite-strings"

#include "mcgl_fat32.h"


_INITIALIZE_EASYLOGGINGPP;

 std::string sExPath;
CMcglProp mcglProps;
CMCGLSharedMemory *g_pMcglData=NULL;


int main(int argc, char **argv)
{
	_START_EASYLOGGINGPP(argc, argv);
	easyloggingpp::Configurations c;
	c.setToDefault();
	easyloggingpp::Loggers::setFilename("/var/log/mcgl.log");
	c.setAll(easyloggingpp::ConfigurationType::ToFile,"1");
	easyloggingpp::Loggers::reconfigureLogger("default", c);
	
	setlocale(LC_CTYPE, "en_US.UTF-8") ;
	
	sExPath=argv[0];
	sExPath=sExPath.substr(0,sExPath.rfind('/')+1);
	LOG(INFO) << "MCGL scanner started";
	
	mcglProps.loadProps();
	g_pMcglData = new CMCGLSharedMemory;
	
	
	
	CMcglFat32 fat;
	if (fat.openCard((char*)mcglProps.device.c_str()))
	{
		LOG(ERROR) << "Card open Error";
		return 0;
	}
	
	fat.scanTree();
	fat.WriteSize();
	
	std::string stdCmd="modprobe g_mass_storage file="+mcglProps.device;
	stdCmd+=" stall=0";
	LOG(INFO) << "CMD=" << stdCmd.c_str();
	system(stdCmd.c_str());
	while(1)
	{
		if (g_pMcglData->GetScanNewParams())
		{
			g_pMcglData->SetScanNewParams(false);
			mcglProps.loadProps();
		}
		fat.scanTree();
		sleep(mcglProps.scanLoopTimeSec);
	}
	fat.closeCard();
	if (g_pMcglData)
		delete g_pMcglData;
	return 0;
}
