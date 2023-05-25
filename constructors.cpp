void VKHQ::constr(bool manualInit,bool validLayersEnable){
	std::signal(SIGSEGV,deconstr);

    _deviceP=&_device;
    
    _objs.resize(2);
    
    logFMainFlags(LOGF);
    logFMainFlags(LOGF);

	wrtInLog("App Started in:");
    wrtFullDate();
 	logF({.f=NLNE|DTAB,.c=1,.b=5,.h=YLWC,.m=0,.bg=0,
          .s="App Started:",.bs=0,.hs=YLWC});
    logText+="";

    if(!manualInit){
        boolValLayers = validLayersEnable;
        init();
        run();
        cleanup();
    }
}

void VKHQ::deconstr(int signal){
//     if((!(_cleanuped)&&cleanup())){
//         wrtSysMsg(RERR,"Failed to CleanUp!");
//     }
    if(signal==SIGSEGV){
        wrtSysMsg(RERR,"SegVault!");
    }else if(signal==SIGTERM||signal==SIGABRT||signal==SIGINT){
        wrtSysMsg(RERR,"Termination!");
    }else if(signal==SIGILL){
        wrtSysMsg(RERR,"Invalid ProgramImage!");
    }
    logFRstSpace();

    logF({.f=NLNE|DTAB,.c=1,.b=5,.h=YLWC,.m=0,.bg=0,
		  .s="App Stopped!",.bs=0,.hs=YLWC});
    crt_logFile();
}
