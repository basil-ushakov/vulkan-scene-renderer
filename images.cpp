void VKHQ::crt_img(VKHQ_imgcrtI& info){
    logF({.f=NLNE,.c=1,.b=1,.h=YLWC,
          .s="Creating Image:",.hs=YLWC,
          .f2=NLNE|DTAB});

    logF({.f=NLNE|PVAL,.c=1,
          .s="MipLevel count is:",.hs=YLWC,
          .v=cstVal(info.l),.bv=1,.hv=YLWC});

    VkImageCreateInfo imgInfo{
        .sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType     =                    VK_IMAGE_TYPE_2D,
        .format        =                           info.frmt,
        .samples       =                          info.smplC,
        .tiling        =                           info.tile,
        .usage         =                           info.usgF,
        .sharingMode   =           VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout =           VK_IMAGE_LAYOUT_UNDEFINED,
    };
    imgInfo.extent.width  = info.w;
    imgInfo.extent.height = info.h;
    imgInfo.extent.depth  = info.z;
    imgInfo.mipLevels     = info.l;
    imgInfo.arrayLayers   = info.as;

    if(vkCreateImage(_device,&imgInfo,nullptr,info.img) != VK_SUCCESS)
    wrtSysMsg(RERR,"Failed to create Image!");

    VkMemoryRequirements memReq;
    vkGetImageMemoryRequirements(_device,*info.img,&memReq);

    VkMemoryAllocateInfo allocInfo{
        .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize  = memReq.size,
        .memoryTypeIndex = get_memType(memReq.memoryTypeBits,info.memF),
    };

    if(vkAllocateMemory(_device,&allocInfo,nullptr,info.dmem) != VK_SUCCESS)
    wrtSysMsg(RERR,"Failed to allocate Memory for Image!");

    vkBindImageMemory(_device,*info.img,*info.dmem,0);
    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=YLWC,
          .s="Created Image!",.hs=YLWC});
}

void VKHQ::crt_tex(VKHQ_texcrtI& info){
    logF({.f =NLNE,.c=1,.b=1,.h=YLWC,
          .s ="Creating Texture:",.hs=REDC,
          .f2=NLNE|DTAB});

    u32 imgC {0}; // Image Count
    u32 iter {0}; // Iteration

    if(info.paths.size() == 0){
        logF({.f =NLNE,.c=1,
              .s ="No texture paths!",.hs=YLWC});
        logF({.f =NLNE,.c=1,
              .s ="Returning ...",.hs=YLWC});

        return;
    }

    std::vector<VkBuffer>       stgBufs;
    std::vector<VkDeviceMemory> stgBufsMem;

    int w,h,chnl;
    u32 mipL{0};

    for(string path : info.paths){
        iter++;

        logF({.f=NLNE,.c=1,
            .s="Loading by path",.hs=YLWC,
            .s2=path,.bs2=1,.hs2=LIMC});

        VkBuffer       stgBuf;
        VkDeviceMemory stgBufMem;

        stbi_uc* pix = stbi_load(path.c_str(),&w,&h,&chnl,STBI_rgb_alpha);
        if(!pix)
        wrtSysMsg(RERR,"Failed to load Image!");

        imgC++;

        static VkDeviceSize imgSize;

        static bool once = [&]{
            mipL  = static_cast<uint32_t>(std::floor(std::log2(std::max(w,h)))) + 1;
            imgSize = w * h * 4;

            return true;
        } ();

        logF({.f=NLNE|PVAL,.c=1,
            .s="MipMaps count is:",.hs=YLWC,
            .v=cstVal(mipL),.bv=1,.hv=REDC});

        VKHQ_bufcrtI bufcrtI{
            .size = imgSize,
            .usgF = info.bufcrtI.usgF,
            .shrM = info.bufcrtI.shrM,
            .memF = info.bufcrtI.memF,
            .buf  = &stgBuf,
            .dmem = &stgBufMem,
            .name = "Image",
        };crt_buf(bufcrtI);

        void* data;
        vkMapMemory(_device,stgBufMem,0,imgSize,0,&data);
        memcpy(data,pix,(size_t) imgSize);
        logF({.f=NLNE,.c=1,
            .s="ImageBufferMemory",.hs=YLWC,
            .s2="mapped!",.bs2=1,.hs2=LIMC});

        vkUnmapMemory(_device,stgBufMem);
        stbi_image_free(pix);
        logF({.f=NLNE,.c=1,
            .s="ImageBufferMemory",.hs=YLWC,
            .s2="unmapped!",.bs2=1,.hs2=REDC});

        stgBufs.push_back(stgBuf);
        stgBufsMem.push_back(stgBufMem);

        logF({.f=NLNE,.c=1,
            .s="Obj:",.hs=YLWC,
            .s2=std::to_string(stgBufs.size()),.bs2=1,.hs2=LIMC});
    }


    VKHQ_imgcrtI imgCrtI = info.imgCrtI;{
                 imgCrtI.w  = static_cast<u32>(w);
                 imgCrtI.h  = static_cast<u32>(h);
                 imgCrtI.l  = mipL;
                 imgCrtI.as = imgC;
    }crt_img(imgCrtI);

    VKHQ_imgLaytcrtI imgLaytCrtI{
        .img     = imgCrtI.img,
        .l       = mipL,
        .lc      = imgC,
        .frmt    = VK_FORMAT_R8G8B8A8_SRGB,
        .oldLayt = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayt = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
    };chg_imgLayt(imgLaytCrtI);

    VKHQ_buftoimgI buftoimgAlgnI     = info.buftoimgAlgnI;{
                   buftoimgAlgnI.img = imgCrtI.img;
                   buftoimgAlgnI.w   = static_cast<uint32_t>(w);
                   buftoimgAlgnI.h   = static_cast<uint32_t>(h);
    }

    for(u32 i = 0;i < stgBufs.size();i++){
        buftoimgAlgnI.buf = &stgBufs[i];
        buftoimgAlgnI.al  = i;

        cpy_bufToImg(buftoimgAlgnI);

        vkDestroyBuffer(_device,stgBufs[i],nullptr);
        vkFreeMemory(_device,stgBufsMem[i],nullptr);
    }

    /* External Image Info */{
    info.w  = w;
    info.h  = h;
    info.ml = mipL;
    }

    //TODO maybe add discard of this proccess ,
    //     If texture dont need MipMaps
    VKHQ_mipmapcrtI mipmapcrtI{
        .img  = imgCrtI.img,
        .frmt = VK_FORMAT_R8G8B8A8_SRGB, // Align to current format
        .w    = w,
        .h    = h,
        .ml   = mipL,
        .lc   = imgC,
    };crt_mipmaps(mipmapcrtI);

	// crt_texImgView();
    VKHQ_imgViewI imgViewI{
        .t   = VK_IMAGE_VIEW_TYPE_2D_ARRAY,
        .f   = VK_FORMAT_R8G8B8A8_SRGB,
        .am  = VK_IMAGE_ASPECT_COLOR_BIT,
        .ml  = mipL,
        .lc  = imgC,
        .img = &_texBuf,
    };
    _texImgView = crt_imgView(imgViewI);

    VKHQ_texSmplrI texSmplrI{
        .ml = mipL,
    };

	crt_texSmplr(texSmplrI);

    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=YLWC,
          .s="Created Texture!",.hs=REDC});
}

void VKHQ::chg_imgLayt(VKHQ_imgLaytcrtI info){
    logF({.f=NLNE,.c=1,.b=1,.h=PNKC,
          .s="Changing ImgLayout:",.hs=MAGC,
          .f2=NLNE|DTAB});

    logF({.f=NLNE,.c=1,
          .s="Old Layout is: ",.hs=MAGC,
          .s2="something",.hs2=YLWC});
    logF({.f=NLNE,.c=1,
          .s="New Layout is: ",.hs=MAGC,
          .s2="something",.hs2=YLWC});

    VkCommandBuffer cmdBuf = use_singleCmd();

    VkImageMemoryBarrier bar{
        .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .oldLayout           = info.oldLayt,
        .newLayout           = info.newLayt,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image               = *info.img,
    };
    bar.subresourceRange = {
        .baseMipLevel   = 0,
        .levelCount     = info.l,
        .baseArrayLayer = 0,
        .layerCount     = info.lc,
    };

    if(info.newLayt == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL){
        if(chk_stncl(info.frmt))
        bar.subresourceRange.aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;
        else
        bar.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    }else{
        bar.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    };

    VkPipelineStageFlags srcStg,dstStg;

    if(info.oldLayt == VK_IMAGE_LAYOUT_UNDEFINED &&
       info.newLayt == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL){
        bar.srcAccessMask = 0;
        bar.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        srcStg = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dstStg = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }else
    if(info.oldLayt == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
       info.newLayt == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL){
        bar.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        bar.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        srcStg = VK_PIPELINE_STAGE_TRANSFER_BIT;
        dstStg = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }else
    if(info.oldLayt == VK_IMAGE_LAYOUT_UNDEFINED &&
       info.newLayt == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL){
        bar.srcAccessMask = 0;
        bar.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
                            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        srcStg = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dstStg = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }else{
        wrtSysMsg(RERR,"Failed to change Image Layout!");
    }

    vkCmdPipelineBarrier(
        cmdBuf,
        srcStg,dstStg,
        0,
        0, nullptr,
        0, nullptr,
        1, &bar
    );

    end_singleCmd(cmdBuf,_graphxQueue);

    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=PNKC,
          .s="Changed ImgLayout!",.hs=MAGC});
}

void VKHQ::cpy_bufToImg(VKHQ_buftoimgI info){
    logF({.f=NLNE,.c=1,.b=1,.h=CYNC,
          .s="Copying Buffer to Image:",.hs=LBLC,
          .f2=NLNE|DTAB});

    VkCommandBuffer cmdBuf = use_singleCmd();
    
    VkBufferImageCopy reg{
        .bufferOffset      = 0,
        .bufferRowLength   = 0,
        .bufferImageHeight = 0,
    };
    reg.imageSubresource = {
        .aspectMask     = info.am,
        .mipLevel       = info.ml,
        .baseArrayLayer = info.al,
        .layerCount     = info.lc,
    };

    VkOffset3D offset = {0};
    VkExtent3D extnt  = {info.w,info.h,1};
    reg.imageOffset   = offset;
    reg.imageExtent   = extnt;

    vkCmdCopyBufferToImage(cmdBuf,*info.buf,*info.img,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           1,&reg);

    end_singleCmd(cmdBuf,_graphxQueue);
    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=CYNC,
          .s="Copyed Buffer to Image!",.hs=LBLC});
}

VkImageView VKHQ::crt_imgView(VKHQ_imgViewI info){
    VkImageViewCreateInfo viewInfo{
        .sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image    = *info.img,
        .viewType = info.t,
        .format   = info.f,
    };viewInfo.subresourceRange = {
        .aspectMask     = info.am,
        .baseMipLevel   = info.mb,
        .levelCount     = info.ml/*mipL*/,
        .baseArrayLayer = info.al,
        .layerCount     = info.lc,
    };
    
    VkImageView imgView;
    if(vkCreateImageView(_device,&viewInfo,nullptr,&imgView) != VK_SUCCESS)
    wrtSysMsg(RERR,"Failed to create ImageView!");

    return imgView;
}

void VKHQ::crt_texImgView(){
    // TODO Maybe deprecate 1 expression method ?
    VKHQ_imgViewI imgViewI{
        .t   = VK_IMAGE_VIEW_TYPE_2D_ARRAY,
        .f   = VK_FORMAT_R8G8B8A8_SRGB,
        .am  = VK_IMAGE_ASPECT_COLOR_BIT,
        .ml  = 12,
        .lc  = 3,
        .img = &_texBuf,
    };
    _texImgView = crt_imgView(imgViewI);
}

void VKHQ::crt_texSmplr(VKHQ_texSmplrI info){
    logF({.f=NLNE,.c=1,.b=1,.h=PRPC,
          .s="Creating ImageSampler:",.hs=PNKC,
          .f2=NLNE|DTAB});

    logF({.f=NLNE|PVAL,.c=1,
          .s="MaxSamplerAnisotropy is: ",.hs=PNKC,
          .v=cstVal(_physGPUDet.limits.maxSamplerAnisotropy),.hv=YLWC});

    VkSamplerCreateInfo smplrInfo{
        .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter               = VK_FILTER_LINEAR,
        .minFilter               = VK_FILTER_LINEAR,
        .mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .mipLodBias              = 0.f,
        .anisotropyEnable        = VK_TRUE,
        .maxAnisotropy           = _physGPUDet.limits.maxSamplerAnisotropy,
        .compareEnable           = VK_FALSE,
        .compareOp               = VK_COMPARE_OP_ALWAYS,
        .minLod                  = 0.f,
        .maxLod                  = static_cast<float>(info.ml),
        .borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        .unnormalizedCoordinates = VK_FALSE,
    };
    
    if(vkCreateSampler(_device,&smplrInfo,nullptr,&_texSmplr) != VK_SUCCESS)
    wrtSysMsg(RERR,"Failed to create TextureSampler!");

    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=PRPC,
          .s="Created ImageSampler!",.hs=PNKC});
}

void VKHQ::crt_mipmaps(VKHQ_mipmapcrtI info){
    logF({.f=NLNE,.c=1,.b=1,.h=GRNC,
          .s="Creating MipMaps:",.hs=LIMC,
          .f2=NLNE|DTAB});
    
    logF({.f=PVAL|NLNE,
          .s="MipMaps count is:",.hs=LIMC,
          .v=cstVal(info.ml),.hv=YLWC});

    VkFormatProperties formatDet;
    vkGetPhysicalDeviceFormatProperties(_physGPU,info.frmt,&formatDet);
    
    if(!(formatDet.optimalTilingFeatures&
         VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)){
        wrtSysMsg(RERR,"Failed to generate MipMaps!");
    }
    
    VkCommandBuffer cmdBuf = use_singleCmd();
    
    VkImageMemoryBarrier bar{
        .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image               = *info.img,
    };
    bar.subresourceRange = {
        .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
        .levelCount     = 1,
        .baseArrayLayer = info.al,
        .layerCount     = info.lc,
    };
    
    s32 mipW = info.w;
    s32 mipH = info.h;
    
    for(u32 i = 1;i < info.ml;i++){
        bar.subresourceRange.baseMipLevel = i - 1;
        bar.oldLayout                     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        bar.newLayout                     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        bar.srcAccessMask                 = VK_ACCESS_TRANSFER_WRITE_BIT;
        bar.dstAccessMask                 = VK_ACCESS_TRANSFER_READ_BIT;
        
        vkCmdPipelineBarrier(cmdBuf,
                             VK_PIPELINE_STAGE_TRANSFER_BIT,VK_PIPELINE_STAGE_TRANSFER_BIT,0,
                             0,nullptr,
                             0,nullptr,
                             1,&bar);
        
        VkImageBlit blit{};
        blit.srcOffsets[0] = {0,0,0};
        blit.srcOffsets[1] = {mipW,mipH,1};
        blit.dstOffsets[0] = {0,0,0};
        blit.dstOffsets[1] = {mipW>1 ? mipW/2:1,
                              mipH>1 ? mipH/2:1,
                                              1};

        blit.srcSubresource = {
            .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel       = i - 1,
            .baseArrayLayer = info.al,
            .layerCount     = info.lc,
        };
        blit.dstSubresource = {
            .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel       = i,
            .baseArrayLayer = info.al,
            .layerCount     = info.lc,
        };
        
        vkCmdBlitImage(cmdBuf,
            *info.img,VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            *info.img,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,&blit,
            VK_FILTER_LINEAR);

        bar.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        bar.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        bar.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        bar.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        
        vkCmdPipelineBarrier(cmdBuf,
            VK_PIPELINE_STAGE_TRANSFER_BIT,VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,0,
            0,nullptr,
            0,nullptr,
            1,&bar);
        
        if(mipW > 1)mipW /= 2;
        if(mipH > 1)mipH /= 2;
    }
    
    bar.subresourceRange.baseMipLevel = info.ml - 1;
    bar.oldLayout                     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    bar.newLayout                     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    bar.srcAccessMask                 = VK_ACCESS_TRANSFER_WRITE_BIT,
    bar.dstAccessMask                 = VK_ACCESS_SHADER_READ_BIT,
    
    vkCmdPipelineBarrier(cmdBuf,
        VK_PIPELINE_STAGE_TRANSFER_BIT,VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,0,
        0,nullptr,
        0,nullptr,
        1,&bar);
    
    end_singleCmd(cmdBuf,_graphxQueue);

    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=GRNC,
          .s="Created MipMaps!",.hs=LIMC});
}

/*
 *git init
 *git add -A 
 *git commit -m "Tiberium Sun unleashed"
 *git remote add VKHQ $
 *git push
*/
