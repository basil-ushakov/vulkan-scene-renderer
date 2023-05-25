bool VKHQ::chk_validLayers(const std::vector<const char*> valLayers){
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : valLayers) {
        bool layerFound = false;
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound){
            return false;
        }
    }
    return true;
}

u32 war_amount{0};

static VKAPI_ATTR VkBool32 VKAPI_CALL dbgCallbck(
    VkDebugUtilsMessageSeverityFlagBitsEXT mesWrngType,
    VkDebugUtilsMessageTypeFlagsEXT mesCntxtType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbckData,
    void* pUserData) {
    wrtSysMsg(VALD,pCallbckData->pMessage);

    war_amount++;

    if(war_amount == 5)
    wrtSysMsg(RERR,"Everything goes wrong");

    return VK_FALSE;
}

VkResult VKHQ::crt_dbgMsngr(VkInstance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if(func != nullptr){
        logF({.f=NLNE,.s="Created DebugMessenger!",.hs=YLWC});
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }else{
        logF({.f=NLNE,.s="Cant created DebugMessenger!",.hs=YLWC});
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void VKHQ::destr_dbgUtilsMsngr(VkInstance instance,
	VkDebugUtilsMessengerEXT debugMessenger,
	const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

//
// VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
// 

void VKHQ::set_dbgMsngr(){
	logF({.f=NLNE,.c=1,.b=1,.h=GLDC,
		  .s="Setuping DebugMessenger:",.hs=YLWC,
          .f2=NLNE|DTAB});

    logF({.f=NLNE,.c=1,.b=0,.h=GLDC,
		  .s="VALIDATION LAYER: ",.hs=YLWC,
          .s2=logFBool(boolValLayers),.hs2=YLWC,
    });
    if(boolValLayers){
	VkDebugUtilsMessengerCreateInfoEXT createInfo{
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		.pfnUserCallback = dbgCallbck,
	};
	if(crt_dbgMsngr(_instance, &createInfo, nullptr, &_dbgUtilMsngr)!=VK_SUCCESS){
        wrtSysMsg(RERR,"Failed to set-up debug messenger!");
	}
	logF({.f=NLNE|MDTB,.c=1,.b=1,.h=GLDC,
		  .s="Setuped DebugMessenger!",.hs=YLWC});
    }
    else{
	logF({.f=NLNE|MDTB,.c=1,.b=1,.h=GLDC,
		  .s="Not Setuped DebugMessenger!",.hs=YLWC});
    }
}

void VKHQ::pop_dbgMsngr(VkDebugUtilsMessengerCreateInfoEXT& createInfo){
	logF({.f=NLNE,.c=1,.b=1,.h=ORGC,
		  .s="Populating DebugMessenger:",.hs=GLDC,
          .f2=NLNE|DTAB});
    
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity =
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                           | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                           | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = dbgCallbck;

	logF({.f=NLNE,.c=1,
		  .s="Binded DebugCallback!",.hs=GLDC});
    
	logF({.f=NLNE|MDTB,.c=1,.b=1,.h=ORGC,
		  .s="Populated DebugMessenger!",.hs=GLDC});
}

void VKHQ::pop_dbgMsngr2(vk::DebugUtilsMessengerCreateInfoEXT& createInfo){
	logF({.f=NLNE,.c=1,.b=1,.h=ORGC,
		  .s="Populating DebugMessenger:",.hs=GLDC,
          .f2=NLNE|DTAB});
    
	createInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning;
	
    createInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral    |
                             vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                             vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;

	createInfo.pfnUserCallback = dbgCallbck;

	logF({.f=NLNE,.c=1,
		  .s="Binded DebugCallback!",.hs=GLDC});
    
	logF({.f=NLNE|MDTB,.c=1,.b=1,.h=ORGC,
		  .s="Populated DebugMessenger!",.hs=GLDC});
}

