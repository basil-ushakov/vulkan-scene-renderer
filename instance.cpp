void VKHQ::init_vulkan(){
	logF({.f=NLNE,.c=1,.b=1,.h=GRNC,
		  .s="Creating Instance:",.hs=LIMC});
	
	VkApplicationInfo appInfo{
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext = NULL,
		.pApplicationName = "VkApp",
		.applicationVersion = VK_MAKE_VERSION(1,0,0),
		.pEngineName = "FluxHQ",
		.engineVersion = VK_MAKE_VERSION(0,1,0),
		.apiVersion = VK_API_VERSION_1_3,
	};

	logF({.f=NLNE|DTAB,.s="Setted AppInfo!",.bs=0,.hs=LIMC});

	auto exts = get_reqEXTs();

	VkLayerProperties props;
	// vkEnumerateInstanceLayerProperties(nullptr,&props);
		
	VkInstanceCreateInfo instnsInfo{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		// .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
		.pApplicationInfo = &appInfo,
		.enabledExtensionCount = static_cast<uint32_t>(exts.size()),
		.ppEnabledExtensionNames = exts.data(),
	};
	if(boolValLayers){
		VkDebugUtilsMessengerCreateInfoEXT dbgCreateInfo{};
		instnsInfo.enabledLayerCount = static_cast<uint32_t>(validLayers.size());
		instnsInfo.ppEnabledLayerNames = validLayers.data();
	
		pop_dbgMsngr(dbgCreateInfo);
		instnsInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &dbgCreateInfo;
	}
	else{
		instnsInfo.enabledLayerCount = 0;
		instnsInfo.ppEnabledLayerNames = NULL;
		instnsInfo.pNext=nullptr;
	}
	
	if (vkCreateInstance(&instnsInfo, nullptr, &_instance) != VK_SUCCESS) {
		wrtSysMsg(RERR,"Failed to create instance!");
	}

	logF({.f=NLNE,.m=0,.s="Created VulkanContext!",.hs=LIMC});

    if (!chk_validLayers(validLayers)) {
		wrtSysMsg(RERR,"Validation layers requested, but not available!");
    }
	logF({.f=NLNE,.c=1,.s="Validation layers requested and avaible!",.hs=LIMC});
	
	logF({.f=NLNE|MDTB,.c=1,.b=1,.h=GRNC,.m=0,.bg=0,
		  .s="Created Instance!",.bs=0,.hs=LIMC});
}

void VKHQ::init_vulkanCPP(){
	logF({.f=NLNE,.c=1,.b=1,.h=GRNC,
		  .s="Creating Instance:",.hs=LIMC});
	
	vk::ApplicationInfo appInfo{
		.pNext = NULL, 
		.pApplicationName = "VkApp",
		.applicationVersion = VK_MAKE_VERSION(1,0,0),
		.pEngineName = "FluxHQ",
		.engineVersion = VK_MAKE_VERSION(0,1,0),
		.apiVersion = VK_API_VERSION_1_3,
	};

	logF({.f=NLNE|DTAB,.s="Setted AppInfo!",.bs=0,.hs=LIMC});

	auto exts = get_reqEXTs();
		
	vk::InstanceCreateInfo instnsInfo{
		/*.flags =  | VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,*/
		.pApplicationInfo = &appInfo,
		.enabledExtensionCount = static_cast<uint32_t>(exts.size()),
		.ppEnabledExtensionNames = exts.data(),
	};
	if(boolValLayers){
		vk::DebugUtilsMessengerCreateInfoEXT dbgCreateInfo{};
		instnsInfo.enabledLayerCount = static_cast<uint32_t>(validLayers.size());
		instnsInfo.ppEnabledLayerNames = validLayers.data();
	
		// pop_dbgMsngr2(dbgCreateInfo);
		instnsInfo.pNext = (vk::DebugUtilsMessengerCreateInfoEXT*) &dbgCreateInfo;
	}
	else{
		instnsInfo.enabledLayerCount = 0;
		instnsInfo.ppEnabledLayerNames = NULL;
		instnsInfo.pNext = nullptr;
	}
	
	vk::Instance instance;
	// if (instance.createInstance(&instnsInfo, nullptr) != VK_SUCCESS) {
		// wrtSysMsg(RERR,"Failed to create instance!");
	// }

	logF({.f=NLNE,.m=0,.s="Created VulkanContext!",.hs=LIMC});

    if (!chk_validLayers(validLayers)) {
		wrtSysMsg(RERR,"Validation layers requested, but not available!");
    }
	logF({.f=NLNE,.c=1,.s="Validation layers requested and avaible!",.hs=LIMC});
	
	logF({.f=NLNE|MDTB,.c=1,.b=1,.h=GRNC,.m=0,.bg=0,
		  .s="Created Instance!",.bs=0,.hs=LIMC});
}

std::vector<const char*> VKHQ::get_reqEXTs(){
	uint32_t count = 0;
    SDL_Vulkan_GetInstanceExtensions(nullptr, &count,nullptr);
    std::vector<const char*> extensions(count);
	SDL_Vulkan_GetInstanceExtensions(nullptr, &count,extensions.data());
	
// 	const char** glfwExtensions;
//     glfwExtensions = glfwGetRequiredInstanceExtensions(&count);
// 	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + count);
	
	if(boolValLayers){
	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	
	EnumList extsNames;
	for(uint32_t i=0;i<extensions.size();i++){
		extsNames.push_back(extensions[i]);
	}
		
	logF({.f=NLNE,.c=1,.b=6,.h=WATC,
		  .s="WINDOW EXTs:",.hs=WATC});
	logF({.f=EDNL|ENUM,
		  .vec=extsNames,.be=3,.he=LBLC});
	logF({.f=NTDS,.c=1,.b=6,.h=WATC,
		  .s="^Window exts^",.hs=WATC});

    return extensions;
}
