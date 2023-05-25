void VKHQ::crt_colrBuf(){
    logF({.f=NLNE,.c=1,.b=1,.h=GLDC,
          .s="Creating ColorBuffer:",.hs=YLWC,
          .f2=NLNE|DTAB});

    VkFormat colrFormat = _swapChnFormat;

    VKHQ_imgcrtI imgCrtI{
		.w     = _swapChnExtent.width,
        .h     = _swapChnExtent.height,
        .l     = 1,
        .smplC = msaaSmpls,
        .frmt  = colrFormat,
        .tile  = VK_IMAGE_TILING_OPTIMAL,
        .usgF  = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .memF  = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        .img   = _colrBuf,
        .dmem  = _colrBufMem
	};crt_img(imgCrtI);

    _colrView = crt_imgView(_colrBuf,1,colrFormat,VK_IMAGE_ASPECT_COLOR_BIT);

    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=GLDC,
          .s="Created ColorBuffer!",.hs=YLWC});
}

VkSampleCountFlagBits VKHQ::get_maxSmpl(){    
    logF({.f=NLNE|PVAL,.c=1,
          .s="FramebufferColorSampleCounts is: ",.hs=PNKC,
          .v=cstVal(_physGPUDet.limits.framebufferColorSampleCounts),.hv=YLWC});
    logF({.f=NLNE|PVAL,.c=1,
          .s="FramebufferDepthSampleCounts is: ",.hs=PNKC,
          .v=cstVal(_physGPUDet.limits.framebufferDepthSampleCounts),.hv=YLWC});
    
    VkSampleCountFlags samplCount = _physGPUDet.limits.framebufferColorSampleCounts&
                                    _physGPUDet.limits.framebufferDepthSampleCounts;
                                   
    if(samplCount & VK_SAMPLE_COUNT_64_BIT)return VK_SAMPLE_COUNT_64_BIT;
    if(samplCount & VK_SAMPLE_COUNT_32_BIT)return VK_SAMPLE_COUNT_32_BIT;
    if(samplCount & VK_SAMPLE_COUNT_16_BIT)return VK_SAMPLE_COUNT_16_BIT;
    if(samplCount & VK_SAMPLE_COUNT_8_BIT) return VK_SAMPLE_COUNT_8_BIT;
    if(samplCount & VK_SAMPLE_COUNT_4_BIT) return VK_SAMPLE_COUNT_4_BIT;
    if(samplCount & VK_SAMPLE_COUNT_2_BIT) return VK_SAMPLE_COUNT_2_BIT;

    return VK_SAMPLE_COUNT_1_BIT;
}
