void VKHQ::crt_swapChn(){
	logF({.f=NLNE,.c=1,.b=1,.h=WATC,
		  .s="Creating SwapChain:",.hs=LBLC,
          .f2=NLNE|DTAB});

    SwapChnSupportDet            det = chk_swapChnSupport(_physGPU);
    VkSurfaceFormatKHR surfaceFormat = get_SwapChnFormat(det.formats);
    VkPresentModeKHR       prsntMode = get_SwapChnPrsntMode(det.prsntModes);
    VkExtent2D                 extnt = get_SwapChnExtent(det.capblites);

    uint32_t frameCount = det.capblites.minImageCount+1;
	logF({.f=NLNE|PVAL,.c=1,
		  .s="SwapChain frame count: ",.hs=LBLC,
          .v=cstVal(frameCount)});

    if(det.capblites.maxImageCount>0&&frameCount>det.capblites.maxImageCount){
        frameCount=det.capblites.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapChainInfo{
        .sType=VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface=_surface,
        .minImageCount=frameCount,
        .imageFormat=surfaceFormat.format,
        .imageColorSpace=surfaceFormat.colorSpace,
        .imageExtent=extnt,
        .imageArrayLayers=1,
        .imageUsage=VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    };

    uint32_t qFamInds[]={
        _qInds.gFam.value(),
        _qInds.pFam.value()
    };

	logF({.f=NLNE|PVAL,.c=1,
		  .s="gFam value: ",.hs=LBLC,
          .v=cstVal(_qInds.gFam.value())});
	logF({.f=NLNE|PVAL,.c=1,
		  .s="pFam value: ",.hs=LBLC,
          .v=cstVal(_qInds.pFam.value())});

    string shrModeName;
    if(_qInds.gFam!=_qInds.pFam){
        swapChainInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        swapChainInfo.queueFamilyIndexCount = 2;
        swapChainInfo.pQueueFamilyIndices   = qFamInds;
        shrModeName                         = "CONCURRENT";
    }else{
        swapChainInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
        shrModeName                         = "EXCLUSIVE";
    }
	logF({.f=NLNE,.c=1,
		  .s="Image ShareMode is: ",.hs=LBLC,
          .s2=shrModeName});

    swapChainInfo.preTransform   = det.capblites.currentTransform;
    swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainInfo.presentMode    = prsntMode;
    swapChainInfo.clipped        = VK_TRUE;
    swapChainInfo.oldSwapchain   = VK_NULL_HANDLE;

    if(vkCreateSwapchainKHR(_device,&swapChainInfo,nullptr,&_swapChn)!=VK_SUCCESS){
        wrtSysMsg(RERR,"Failed to find suitable GPU!");
    }
    vkGetSwapchainImagesKHR(_device,_swapChn,&frameCount,nullptr);
    _swapChnImgs.resize(frameCount);
    vkGetSwapchainImagesKHR(_device,_swapChn,&frameCount,_swapChnImgs.data());

    _swapChnFormat = surfaceFormat.format;
    _swapChnExtent = extnt;

	logF({.f=NLNE|MDTB,.c=1,.b=1,.h=WATC,
		  .s="Created SwapChain!",.hs=LBLC});
}

SwapChnSupportDet VKHQ::chk_swapChnSupport(VkPhysicalDevice device){
	logF({.f=NLNE,.c=1,.b=2,.h=GRNC,
		  .s="Checking SwapChain support:",.hs=WATC,
          .f2=NLNE|DTAB});
    SwapChnSupportDet det;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device,_surface,&det.capblites);
    
    u32 formatCount;
    
    vkGetPhysicalDeviceSurfaceFormatsKHR(device,_surface,&formatCount,nullptr);

    string supportName = "NOT SUPPORTED";    
	logF({.f=NLNE|PVAL,.c=1,
		  .s="SurfaceFormat count is: ",.hs=WATC,
          .v=cstVal(formatCount),.hv=CYNC});

    if(formatCount!=0){
        det.formats.resize(formatCount);
        if(vkGetPhysicalDeviceSurfaceFormatsKHR(device,_surface,&formatCount,det.formats.data())==VK_SUCCESS){
            supportName = "SUPPORTED";
        }
    }
	logF({.f=NLNE,.c=1,
		  .s="SurfaceFormat is: ",.hs=WATC,
          .s2=supportName,.hs2=CYNC});

    uint32_t prsntCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device,_surface,&prsntCount,nullptr);
	logF({.f=NLNE|PVAL,.c=1,
		  .s="PresentMode count is: ",.hs=WATC,
          .v=cstVal(prsntCount),.hv=CYNC});

    supportName = "SUPPORTED";
    if(prsntCount!=0){
        det.prsntModes.resize(prsntCount);
        if(vkGetPhysicalDeviceSurfacePresentModesKHR(device,_surface,&prsntCount,det.prsntModes.data())!=VK_SUCCESS){
            supportName = "NOT SUPPORTED";
        }
    }
	logF({.f=NLNE,.c=1,
		  .s="PresentModes is: ",.hs=WATC,
          .s2=supportName,.hs2=CYNC});

	logF({.f=NLNE|MDTB,.c=1,.b=2,.h=GRNC,
		  .s="Checked SwapChain support!",.hs=WATC});
    return det;
}

VkSurfaceFormatKHR VKHQ::get_SwapChnFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats){
	logF({.f=NLNE,.c=1,.b=2,.h=YLWC,
		  .s="SWAPCHAIN FORMATS:",.hs=BLUC,
          .f2=DTAB|NLNE});
    for(const auto& availableFormat:availableFormats){
        if(availableFormat.format==VK_FORMAT_B8G8R8A8_UNORM&&
            availableFormat.colorSpace==VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
            logF({.f=NLNE,.c=1,
                  .s="SwapChain format is: B8G8R8A8_UNORM",.hs=BLUC});
            logF({.f=NLNE,.c=1,
                  .s="SwapChain colorSpace is: SRGB_NONLINEAR",.hs=BLUC});

            logF({.f=NLNE|MDTB,.c=1,.b=2,.h=YLWC,
                  .s="^SwapChain formats^",.hs=BLUC});
            return availableFormat;
        }
    }
	logF({.f=NLNE|MDTB,.c=1,.b=2,.h=YLWC,
		  .s="^SwapChain formats^",.hs=BLUC});
    return availableFormats[0];
}

inline bool chk_prior(u64 priorNew,u64& crnt){
    if(crnt<priorNew){
        crnt=priorNew;
        return true;
    }
    return false;
}
string prsntModeName="";
EnumList prsntModeNames;
VkPresentModeKHR VKHQ::get_SwapChnPrsntMode(const std::vector<VkPresentModeKHR>& availablePrsntModes){
	logF({.f=NLNE,.c=1,.b=2,.h=REDC,
		  .s="PRESENT MODES:",.hs=ORGC,
          .f2=DTAB|NLNE});

    bool             crush     {false};
    VkPresentModeKHR prsntMode;
    u64              prior     {0};

    for(u32 i=0;i<availablePrsntModes.size();i++){
        if(availablePrsntModes[i]==VK_PRESENT_MODE_MAILBOX_KHR){
            prsntModeNames.push_back("MAILBOX");
            if(chk_prior(6,prior)){
                prsntMode=VK_PRESENT_MODE_MAILBOX_KHR;
                prsntModeName="MAILBOX";
            }
        }
        else if(availablePrsntModes[i]==VK_PRESENT_MODE_FIFO_RELAXED_KHR){
            prsntModeNames.push_back("FIFO RELAXED");
            if(chk_prior(5,prior)){
                prsntMode=VK_PRESENT_MODE_FIFO_RELAXED_KHR;
                prsntModeName="FIFO RELAXED";
            }
        }
        else if(availablePrsntModes[i]==VK_PRESENT_MODE_FIFO_KHR){
            prsntModeNames.push_back("FIFO");
            if(chk_prior(4,prior)){
                prsntMode=VK_PRESENT_MODE_FIFO_KHR;
                prsntModeName="FIFO";
            }
        }
        else if(availablePrsntModes[i]==VK_PRESENT_MODE_IMMEDIATE_KHR){
            prsntModeNames.push_back("IMMEDIATE");
            if(chk_prior(3,prior)){
                prsntMode=VK_PRESENT_MODE_IMMEDIATE_KHR;
                prsntModeName="IMMEDIATE";
            }
        }
        else if(availablePrsntModes[i]==VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR){
            prsntModeNames.push_back("SHARED DEMAND REFRESH");
            if(chk_prior(2,prior)){
                prsntMode=VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR;
                prsntModeName="SHARED DEMAND REFRESH";
            }
        }
        else if(availablePrsntModes[i]==VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR){
            prsntModeNames.push_back("SHARED CONTINUOUS REFRESH");
            if(chk_prior(1,prior)){
                prsntMode=VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR;
                prsntModeName="SHARED CONTINUOUS REFRESH";
            }
        }
        else{
            crush = true;
        }
    }
	logF({.f=EDNL|ENUM,
		  .vec=prsntModeNames,.be=9,.he=ORGC});
    logF({.f=NLNE|PVAL,.c=1,
          .s="Priority: ",.hs=REDC,
          .v=cstVal(prior),.hv=YLWC});
    logF({.f=NLNE|PVAL,.c=1,
          .s="Count is: ",.hs=REDC,
          .v=cstVal(availablePrsntModes.size()),.hv=YLWC});

    if(crush)wrtSysMsg(RERR,"Cannot find PresentMode!");
    logF({.f=NLNE,.c=1,
          .s="Present mode is: ",.hs=REDC,
          .s2=prsntModeName,.hs2=YLWC});

    prsntModeNames.clear();
    
	logF({.f=NLNE|MDTB,.c=1,.b=2,.h=REDC,
		  .s="^Present modes^",.hs=ORGC});

    // return VK_PRESENT_MODE_IMMEDIATE_KHR;
    return prsntMode;
}

VkExtent2D VKHQ::get_SwapChnExtent(const VkSurfaceCapabilitiesKHR& capblites){
    logF({.f=NLNE,.c=1,.b=2,.h=CYNC,
          .s="SWAPCHAIN EXTENT:",.hs=LBLC,
          .f2=NLNE|DTAB});
    if(capblites.currentExtent.width!=std::numeric_limits<uint32_t>::max()){        
        logF({.f=NLNE,.c=1,
              .s="Current extent isn't bigger than maxValue of UINT32 Type!",.hs=BLUC});
        logF({.f=NLNE|PVAL,.c=1,
              .s="Extent width: ",.hs=BLUC,
              .v=cstVal(capblites.currentExtent.width),.hv=YLWC});
        logF({.f=NLNE|PVAL,.c=1,
              .s="Extent height: ",.hs=BLUC,
              .v=cstVal(capblites.currentExtent.height),.hv=YLWC});

        logF({.f=NLNE|MDTB,.c=1,.b=2,.h=CYNC,
              .s="^SwapChain extent^",.hs=LBLC,
              .f2=NLNE});
        return capblites.currentExtent;
    }else{
        logF({.f=NLNE,.c=1,
              .s="Current extent is bigger than maxValue of UINT32 Type!",.hs=BLUC});
        int width,height;
        SDL_Vulkan_GetDrawableSize(_window,&width,&height);
        VkExtent2D extnt={
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };
        extnt.width=std::clamp(extnt.width,capblites.minImageExtent.width,capblites.maxImageExtent.width);  
        extnt.height=std::clamp(extnt.height,capblites.minImageExtent.height,capblites.maxImageExtent.height);
      
        logF({.f=NLNE|PVAL,.c=1,
              .s="Extent width: ",.hs=BLUC,
              .v=cstVal(extnt.width),.hv=YLWC});
        logF({.f=NLNE|PVAL,.c=1,
              .s="Extent height: ",.hs=BLUC,
              .v=cstVal(extnt.height),.hv=YLWC});
        
        logF({.f=NLNE|MDTB,.c=1,.b=2,.h=CYNC,
              .s="^SwapChain extent^",.hs=LBLC,
              .f2=NLNE});
        return extnt;
    }
}

void VKHQ::crt_imgViews(){
    logF({.f=NLNE,.c=1,.b=1,.h=LBLC,
          .s="Creating SwapChain ImageViews:",.hs=CYNC,
          .f2=NLNE|DTAB});

    s swpChnSize=_swapChnImgs.size();
    _swapChnImgViews.resize(swpChnSize);
    logF({.f=NLNE|PVAL,.c=1,
          .s="Resizing SwapChain ImageViews array with size:",.hs=CYNC,
          .v=cstVal(swpChnSize),.bv=1,
          .f2=NLNE|DTAB});

    size_t i;
    for(i=0;i<swpChnSize;i++){
        _swapChnImgViews[i]=crt_imgView(_swapChnImgs[i],1,
                                        _swapChnFormat,VK_IMAGE_ASPECT_COLOR_BIT);
        logF({.f=NLNE|PVAL,.c=1,
              .s=">|Created SwapChain Image view number:",.hs=CYNC,
              .v=cstVal(i+1),.bv=3,.hv=YLWC}); 
    }
    logF({.f=NLNE|PVAL|MDTB,.c=1,
          .s="SwapChain ImageViews count is: ",.hs=CYNC,
          .v=cstVal(i),.hv=YLWC});

    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=LBLC,
          .s="Created SwapChain ImageViews!",.hs=CYNC,
          .f2=NLNE});
}

void VKHQ::rcrt_swapchn(){
    // if(timeout>30)wrtSysMsg(RERR,"A lot of Swapchain Recreating!");        

    logF({.f=NLNE,.c=1,.b=1,.h=GRYC,
          .s="ReCreating SwapChain:",.hs=GRYC,
          .f2=NLNE|DTAB});

    int width,height=0;
    SDL_Vulkan_GetDrawableSize(_window,&width,&height);
    while(width==0||height==0){
        SDL_Vulkan_GetDrawableSize(_window,&width,&height);
        SDL_WaitEvent(NULL);
    }

    logF({.f=NLNE|PVAL,.c=1,
          .s=">|Setting NewResolution",.hs=GRYC,
          .v=cstVal(width),.bv=1,.hv=GRYC,.f2=NTDS}); 
    logF({.f=PVAL|STDP,.c=1,
          .s="|",.hs=GRYC,
          .v=cstVal(height),.hv=GRYC}); 

    vkDeviceWaitIdle(_device);

    cleanup_swapChn();
    crt_swapChn();
    crt_imgViews();
    crt_rndrPass();
	
    VKHQ_gplineI gplnInfo{
		.pline=_graphxPline,
		.plineLayt=_graphxPlineLayt,
		.fs="shaders/frag.spv",
		.vs="shaders/vert.spv",
		.vw=(float)_swapChnExtent.width,
		.vh=(float)_swapChnExtent.height,
		.smpls=VK_SAMPLE_COUNT_8_BIT,
	};crt_graphxPline(gplnInfo);
    
    crt_colrBuf();
    crt_depthBuf();
    crt_framBuf();

    _framBufRsz = true;
    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=GRYC,
          .s="ReCreated SwapChain!",.hs=GRYC,
          .f2=NLNE});
    
    timeout+=1;
}
