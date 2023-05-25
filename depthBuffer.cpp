void VKHQ::crt_depthBuf(){
    logF({.f=NLNE,.c=1,.b=1,.h=BLCC,
          .s="Creating DepthBuffer:",.hs=GRYC,
          .f2=NLNE|DTAB});

    logF({.f=NLNE,.c=1,
          .s="Finding DepthFormat!",.hs=GRYC});
    VkFormat depthFormat=get_depthFormat();

    VKHQ_imgcrtI imgCrtI{
		.w     = _swapChnExtent.width,
        .h     = _swapChnExtent.height,
        .l     = 1,
        .smplC = msaaSmpls,
        .frmt  = depthFormat,
        .tile  = VK_IMAGE_TILING_OPTIMAL,
        .usgF  = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        .memF  = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        .img   = _depthBuf,
        .dmem  = _depthBufMem
	};crt_img(imgCrtI);

    _depthView=crt_imgView(_depthBuf,1,depthFormat,VK_IMAGE_ASPECT_DEPTH_BIT);

    VKHQ_imgLaytcrtI imgLaytCrtI{
        .img     = _depthBuf,
        .l       = 1,
        .frmt    = depthFormat,
        .oldLayt = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayt = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };chg_imgLayt(imgLaytCrtI);

    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=BLCC,
          .s="Created DepthBuffer!",.hs=GRYC});
}

VkFormat VKHQ::get_depthFormat(){
    return get_supprtFormat({VK_FORMAT_D32_SFLOAT,VK_FORMAT_D32_SFLOAT_S8_UINT,VK_FORMAT_D24_UNORM_S8_UINT},
                             VK_IMAGE_TILING_OPTIMAL,
                             VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

VkFormat VKHQ::get_supprtFormat(const std::vector<VkFormat>& formats,VkImageTiling tiling,VkFormatFeatureFlags flags){
    for(VkFormat format:formats){
        VkFormatProperties det;
        vkGetPhysicalDeviceFormatProperties(_physGPU,format,&det);

        if(tiling==VK_IMAGE_TILING_LINEAR&&(det.linearTilingFeatures&flags)==flags)return format;
        if(tiling==VK_IMAGE_TILING_OPTIMAL&&(det.optimalTilingFeatures&flags)==flags)return format;
    }

    wrtSysMsg(IARG,"Failed to find SupportedFormat!");
    return VkFormat();
}

bool VKHQ::chk_stncl(VkFormat format){
    return format==VK_FORMAT_D32_SFLOAT_S8_UINT||format==VK_FORMAT_D24_UNORM_S8_UINT;
}
