void VKHQ::crt_device(){
	logF({.f=NLNE,.c=1,.b=1,.h=BLUC,
		  .s="Creating & Configuring Device:",.hs=GRNC});
    std::vector<VkDeviceQueueCreateInfo> QInfos;
    std::set<uint32_t> individQFamilies = {_qInds.gFam.value(),_qInds.pFam.value(),_qInds.tFam.value()};
	float QPriority = 1.0f;
	VkDeviceQueueCreateInfo QInfo{
		.sType=VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.queueFamilyIndex=0,
		.queueCount=1,
		.pQueuePriorities=&QPriority,
	};
	QInfos.push_back(QInfo);
	QInfo.queueFamilyIndex=1;
	QInfos.push_back(QInfo);
	QInfo.queueFamilyIndex=2;
// 	QInfos.push_back(QInfo);

	VkPhysicalDeviceFeatures deviceFeatures{
		.samplerAnisotropy=VK_TRUE,
	};

	logF({.f=NLNE|DTAB|PVAL,.s="QueueFamilyCount is:",.bs=0,.hs=BLUC,
		  .v=static_cast<float>(QInfos.size()),.bv=1,.hv=WATC});

	VkDeviceCreateInfo deviceInfo{
		.sType=VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.queueCreateInfoCount=(uint32_t) QInfos.size(),
		.pQueueCreateInfos=QInfos.data(),
		.enabledLayerCount=static_cast<uint32_t>(validLayers.size()),
		.ppEnabledLayerNames=validLayers.data(),
		.enabledExtensionCount=static_cast<uint32_t>(deviceEXTs.size()),
		.ppEnabledExtensionNames=deviceEXTs.data(),
		.pEnabledFeatures=&deviceFeatures,
	};
	logF({.f=NLNE|PVAL,.s="EXTs count is:",.bs=0,.hs=BLUC,
		  .v=static_cast<float>(deviceInfo.enabledExtensionCount),.bv=1,.hv=WATC});

	if(vkCreateDevice(_physGPU,&deviceInfo,nullptr,&_device)!=VK_SUCCESS){
		wrtSysMsg(RERR,"Couldn't create Device!");
	}
	logF({.f=NLNE,.s="Created Device!",.bs=0,.hs=BLUC,});

	std::string deviceState;
	if(_device==VK_NULL_HANDLE){
		deviceState="NULL";
	}else{
		deviceState="NOT NULL";
	}

	logF({.f=NLNE,.s="Device is:",.bs=0,.hs=BLUC,
				  .s2=deviceState,.bs2=1,.hs2=WATC});

	vkGetDeviceQueue(_device,_qInds.gFam.value(), 0, &_graphxQueue);
		logF({.f=NLNE|PVAL,.s="Getted graphics family queue:",.bs=0,.hs=BLUC,
			  .v=static_cast<float>(_qInds.gFam.value()),.bv=1,.hv=WATC});
    vkGetDeviceQueue(_device,_qInds.pFam.value(), 0, &_presentQueue);
		logF({.f=NLNE|PVAL,.s="Getted present family queue:",.bs=0,.hs=BLUC,
			  .v=static_cast<float>(_qInds.pFam.value()),.bv=1,.hv=WATC});
	vkGetDeviceQueue(_device,_qInds.tFam.value(), 0, &_transfQueue);
		logF({.f=NLNE|PVAL,.s="Getted transfer family queue:",.bs=0,.hs=BLUC,
			  .v=static_cast<float>(_qInds.tFam.value()),.bv=1,.hv=WATC});

	logF({.f=NLNE|MDTB,.c=1,.b=1,.h=BLUC,
		  .s="Created & Configured Device!",.hs=GRNC});
}

void VKHQ::get_physDevice(){
	logF({.f=NLNE,.c=1,.b=1,.h=LIMC,
		  .s="Getting Physical Device:",.hs=GRNC});
	_deviceCount=0;

	vkEnumeratePhysicalDevices(_instance, &_deviceCount,nullptr);
    if(_deviceCount==0){
		wrtSysMsg(RERR,"Failed to find GPUs with Vulkan support!");
	}
	logF({.f=NLNE|DTAB,.s="Finded VK forged GPU/s!",.bs=0,.hs=GRNC,});

    std::vector<VkPhysicalDevice> devices(_deviceCount);
	vkEnumeratePhysicalDevices(_instance, &_deviceCount,devices.data());
	logF({.f=NLNE|PVAL,.c=1,
		  .s="Device count is: ",.hs=GRNC,
		  .v=cstVal(_deviceCount)});

	for(const auto& hDevice:devices){
		if(chk_deviceSupport(hDevice)){
				_physGPU = hDevice;
				vkGetPhysicalDeviceProperties(_physGPU,&_physGPUDet);
				msaaSmpls=get_maxSmpl();
				break;
		}
	}
	if(_physGPU==VK_NULL_HANDLE){
		wrtSysMsg(RERR,"Failed to find suitable GPU!");
	}

	logF({.f=NLNE|DTAB,.s="Getted PhyscialDeviceDetails!",.bs=0,.hs=GRNC,});

	logF({.f=NLNE,.s="Finded suitable GPU/s!",.bs=0,.hs=BLUC,});

	logF({.f=NLNE|MDTB,.c=1,.b=1,.h=LIMC,
		  .s="Getted Physical Device!",.hs=GRNC});
}

bool VKHQ::chk_deviceSupport(VkPhysicalDevice device){
	logF({.f=NLNE,.c=1,.b=11,.h=GRNC,
		  .s="Checking Device support:",.hs=GLDC,
		  .f2=NLNE|DTAB});

	QFamInds inds = get_qFams(device,"Check Device Support");

	bool extsSupport = chk_deviceEXTsSupport(device);
	bool swapChnWorkable = false;
	if(extsSupport){
		SwapChnSupportDet det=chk_swapChnSupport(device);
		swapChnWorkable=!det.formats.empty()&&!det.prsntModes.empty();
	}

	logF({.f=NLNE|PVAL,.s="Inds gFam has value: ",.bs=0,.hs=GLDC,
		  .v=static_cast<float>(_qInds.gFam.has_value()),.hv=LIMC});
    logF({.f=NLNE,.c=1,.b=0,.h=GLDC,
		  .s="Extensions support: ",.hs=LIMC,
          .s2=logFBool(extsSupport),.hs2=LIMC});
	logF({.f=NLNE,.c=1,.b=0,.h=GLDC,
		  .s="SwapChain workable: ",.hs=LIMC,
          .s2=logFBool(swapChnWorkable),.hs2=LIMC});

	logF({.f=NLNE|MDTB,.c=1,.b=11,.h=GRNC,
		  .s="Checked Device support!",.hs=GLDC});
	
	return _qInds.gFam.has_value() && extsSupport && swapChnWorkable;
}

bool VKHQ::chk_deviceEXTsSupport(VkPhysicalDevice device){
	logF({.f=NLNE,.c=1,.b=1,.h=CYNC,
		  .s="Checking Device EXTs support:",.hs=LBLC,
		  .f2=NLNE|DTAB});
	uint32_t extsCount=0;
	
	if(device==VK_NULL_HANDLE){
		logF({.f=NLNE|MDTB,.c=1,.b=0,.h=CYNC,
			  .s="PhysicalDevice is: NULL!",.hs=LBLC});
		return false;
	}else{
		logF({.f=NLNE,.c=1,.b=0,.h=CYNC,
			  .s="PhysicalDevice is: NOT NULL!",.hs=LBLC});
	}
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extsCount, nullptr);
	std::vector<VkExtensionProperties> availableExts(extsCount);
	vkEnumerateDeviceExtensionProperties(device,nullptr,&extsCount,availableExts.data());

	std::set<std::string> reqExts(deviceEXTs.begin(),deviceEXTs.end());

	for(const auto& ext : availableExts){
		reqExts.erase(ext.extensionName);
	}
    logF({.f=NLNE,.c=1,.b=0,.h=CYNC,
		  .s="Temp Required EXTs array erased: ",.hs=LBLC,
          .s2=logFBool(reqExts.empty()),.hs2=PRPC});

	logF({.f=NLNE|MDTB,.c=1,.b=1,.h=CYNC,
		  .s="Checked Device EXTs support!",.hs=LBLC});
	return reqExts.empty();
}

QFamInds VKHQ::get_qFams(VkPhysicalDevice device,std::string callSite){
	logF({.f=NLNE,.c=1,.b=3,.h=MAGC,
		  .s="Finding QFamilies for ",.hs=PRPC,
          .s2=callSite,.bs2=6,.hs2=YLWC,
		  .f2=NLNE|DTAB});
	QFamInds inds;

	inds.qFamCount=0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &inds.qFamCount, nullptr);
	logF({.f=NLNE|PVAL,.c=1,
		  .s="QueueFamilies now is: ",.hs=PRPC,
	      .v=cstVal(inds.qFamCount),.hv=YLWC});
	std::vector<VkQueueFamilyProperties> QFamilies(inds.qFamCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &inds.qFamCount, QFamilies.data());

    u32 i=0;
	EnumList qFlagsNames;
    for (const auto& QFamily : QFamilies) {
		logF({.f=NLNE|PVAL,.s="Finded Queues Support for",.hs=PRPC,
			  .v=cstVal(i),.bv=2,.hv=YLWC});
		qFlagsNames.clear();
		if(QFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
			qFlagsNames.push_back("GRAPHICS");
			inds.gFam=i;
		}
		VkBool32 prsntSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _surface, &prsntSupport);
		if(prsntSupport){
			qFlagsNames.push_back("PRESENT");
			inds.pFam=i;
		}
		if(QFamily.queueFlags&VK_QUEUE_COMPUTE_BIT){
			qFlagsNames.push_back("COMPUTE");
			inds.cFam=i;
		}
		if(QFamily.queueFlags&VK_QUEUE_TRANSFER_BIT){
			qFlagsNames.push_back("TRANSFER");
			inds.tFam=i;
		}
		logF({.f=NLNE|ENUM,.c=1,.b=8,.h=PRPC,
		  	  .vec=qFlagsNames,.be=3,.he=YLWC});

        logF({.f=NLNE|PVAL,.c=1,
              .s="QueueCount now is: ",.hs=PRPC,
              .v=cstVal(QFamily.queueCount),.hv=YLWC});
        
		if(inds.ready()){
			i++;continue;
		}
	}
	logF({.f=NLNE,.s="Inds ready!",.hs=PRPC});

	_qInds=inds;
	logF({.f=NLNE|MDTB,.c=1,.b=3,.h=MAGC,
		  .s="Getted QFamilies for ",.hs=PRPC,
          .s2=callSite,.bs2=6,.hs2=YLWC});
	return inds;
}

/*
  |:>>VALID LAYER >>:Validation Error: [ VUID-vkQueueSubmit-pCommandBuffers-00074 ] Object 0: handle = 0x56464c5667e0, type = VK_OBJECT_TYPE_COMMAND_BUFFER; Object 1: handle = 0x56464bf4a9a0, type = VK_OBJECT_TYPE_QUEUE; | MessageID = 0xac0b8cd1 | vkQueueSubmit(): pSubmits[0].pCommandBuffers[0] Primary VkCommandBuffer 0x56464c5667e0[] created in queue family 0 is being submitted on VkQueue 0x56464bf4a9a0[] from queue family 1. The Vulkan spec states: Each element of the pCommandBuffers member of each element of pSubmits must have been allocated from a VkCommandPool that was created for the same queue family queue belongs to (https://vulkan.lunarg.com/doc/view/1.3.216.0/linux/1.3-extensions/vkspec.html#VUID-vkQueueSubmit-pCommandBuffers-00074)
*/

