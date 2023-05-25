void VKHQ::crt_buf(VKHQ_bufcrtI info){
    string bufTxt;
    u64 txtAddr=reinterpret_cast<u64>(info.buf);
    if(info.name == "N0NAME")
    bufTxt = std::to_string(txtAddr);
    else
    bufTxt = info.name;

    logF({.f=NLNE,.c=1,.b=4,.h=LBLC,
          .s="Creating Buffer",.hs=WATC,
          .s2=bufTxt,.bs2=3,.hs2=WATC,
          .f2=NLNE|DTAB});

    VkBufferCreateInfo bufInfo{
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size        = info.size,
        .usage       = info.usgF,
        .sharingMode = info.shrM
    };if(vkCreateBuffer(_device,&bufInfo,nullptr,&info.buf)!=VK_SUCCESS)
    wrtSysMsg(RERR,"Failed to create VertexBuffer!");

    VkMemoryRequirements memReq;
    vkGetBufferMemoryRequirements(_device,info.buf,&memReq);

    logF({.f=NLNE|PVAL,.c=1,
          .s="Size:",.hs=WATC,
          .v=cstVal(memReq.size),.bv=3,.hv=WATC});
    logF({.f=NLNE|PVAL,.c=1,
          .s="Alignment:",.hs=WATC,
          .v=cstVal(memReq.alignment),.bv=3,.hv=WATC});
    logF({.f=NLNE|PVAL,.c=1,
          .s="MemoryType:",.hs=WATC,
          .v=cstVal(memReq.memoryTypeBits),.bv=3,.hv=WATC});

    VkMemoryAllocateInfo allocInfo{
        .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize  = memReq.size,
        .memoryTypeIndex = get_memType(memReq.memoryTypeBits,info.memF),
    };if(vkAllocateMemory(_device,&allocInfo,nullptr,&info.dmem)!=VK_SUCCESS)
      wrtSysMsg(RERR,"Failed to allocate Memory for VertexBuffer!");

    logF({.f=NLNE,.c=1,
          .s="Allocated Memory for VertexBuffer!",.hs=WATC});

    if(vkBindBufferMemory(_device,info.buf,info.dmem,0) != VK_SUCCESS){
        wrtSysMsg(RERR,"Failed to bind BufferMemory!");
    }

    logF({.f=NLNE|MDTB,.c=1,.b=4,.h=LBLC,
          .s="Created Buffer",.hs=WATC,
          .s2=bufTxt,.bs2=3,.hs2=WATC});
}

void VKHQ::cpy_buf(VkBuffer srcBuf,VkBuffer dstBuf,VkDeviceSize size){
    string bufTxt=std::to_string(reinterpret_cast<u64>(dstBuf));
    logF({.f=NLNE,.c=1,.b=1,.h=GLDC,
          .s="Copying Buffer to",.hs=YLWC,
          .s2=bufTxt,.bs2=3,.hs2=YLWC,
          .f2=NLNE|DTAB});

    VkCommandBufferAllocateInfo allocInfo{
        .sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool=_cmdPool,
        .level=VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount=1,
    };
    
    VkCommandBuffer cmdBuf;
    vkAllocateCommandBuffers(_device,&allocInfo,&cmdBuf);
    
    VkCommandBufferBeginInfo cmdBufBgnInfo{
        .sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags=VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };
    
    vkBeginCommandBuffer(cmdBuf,&cmdBufBgnInfo);
    
        VkBufferCopy cpyRegion{};
        cpyRegion.size=size;
        vkCmdCopyBuffer(cmdBuf,srcBuf,dstBuf,1,&cpyRegion);
        
    vkEndCommandBuffer(cmdBuf);
    
    VkSubmitInfo submitInfo{
        .sType=VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount=1,
        .pCommandBuffers=&cmdBuf,
    };
    
    vkQueueSubmit(_graphxQueue,1,&submitInfo,VK_NULL_HANDLE);
    vkQueueWaitIdle(_graphxQueue);
    
    vkFreeCommandBuffers(_device,_cmdPool,1,&cmdBuf);
    
    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=GLDC,
          .s="Copied Buffer to",.hs=YLWC,
          .s2=bufTxt,.bs2=3,.hs2=YLWC});
}

uint32_t VKHQ::get_memType(uint32_t filter, VkMemoryPropertyFlags det){
    VkPhysicalDeviceMemoryProperties memDet;
    vkGetPhysicalDeviceMemoryProperties(_physGPU,&memDet);
    
    for(uint32_t i=0;i<memDet.memoryTypeCount;i++){
        if((filter&(1<<i))&&(memDet.memoryTypes[i].propertyFlags&det)==det){
            return i;
        }
    }
    wrtSysMsg(RERR,"Failed to get MemoryType!");
    return 0;
}

void VKHQ::crt_vertBuf(){
    logF({.f=NLNE,.c=1,.b=1,.h=LBLC,
          .s="Creating VertexBuffer:",.hs=CYNC,
          .f2=NLNE|DTAB});

    VkDeviceSize bufSize = sizeof(verts[0]) * verts.size();
    VkBuffer stgBuf;
    VkDeviceMemory stgBufMem;
    
    VKHQ_bufcrtI bufStgcrtI{
		.size = bufSize,
		.usgF = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		.shrM = VK_SHARING_MODE_EXCLUSIVE,
		.memF = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		.buf  = stgBuf,
		.dmem = stgBufMem,
		.name = "StagingBuffer"
	};crt_buf(bufStgcrtI);

    void* data;
    vkMapMemory(_device,stgBufMem,0,bufSize,0,&data);
        memcpy(data,verts.data(),(size_t) bufSize);
    logF({.f=NLNE,.c=1,
          .s="VertexBufferMemory",.hs=LBLC,
          .s2="mapped!",.bs2=1,.hs2=LIMC});
    vkUnmapMemory(_device,stgBufMem);
    logF({.f=NLNE,.c=1,
          .s="VertexBufferMemory",.hs=LBLC,
          .s2="unmapped!",.bs2=1,.hs2=REDC});

    VKHQ_bufcrtI bufVertcrtI{
		.size = bufSize,
		.usgF = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		.shrM = VK_SHARING_MODE_EXCLUSIVE,
		.memF = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		.buf  = _objs[0].vertBuf,
		.dmem = _objs[0].vertBufMem,
		.name = "VertexBuffer"
	};crt_buf(bufVertcrtI);

    cpy_buf(stgBuf,_objs[0].vertBuf,bufSize);

    vkDestroyBuffer(_device,stgBuf,nullptr);
    vkFreeMemory(_device,stgBufMem,nullptr);

    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=LBLC,
          .s="Created VertexBuffer!",.hs=CYNC});
}

void VKHQ::crt_indBuf(){
    logF({.f=NLNE,.c=1,.b=1,.h=MAGC,
          .s="Creating IndexBuffer:",.hs=PNKC,
          .f2=NLNE|DTAB});
    VkDeviceSize bufSize=sizeof(inds[0])*inds.size();
    VkBuffer stgBuf;
    VkDeviceMemory stgBufMem;

    VKHQ_bufcrtI bufStgcrtI{
		.size = bufSize,
		.usgF = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		.shrM = VK_SHARING_MODE_EXCLUSIVE,
		.memF = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		.buf  = stgBuf,
		.dmem = stgBufMem,
		.name = "StagingBuffer"
	};crt_buf(bufStgcrtI);
    
    void* data;
    vkMapMemory(_device,stgBufMem,0,bufSize,0,&data);
        memcpy(data,inds.data(),(size_t) bufSize);
    logF({.f=NLNE,.c=1,
          .s="IndexBufferMemory",.hs=PNKC,
          .s2="mapped!",.bs2=1,.hs2=LIMC});
    vkUnmapMemory(_device,stgBufMem);
    logF({.f=NLNE,.c=1,
          .s="IndexBufferMemory",.hs=PNKC,
          .s2="unmapped!",.bs2=1,.hs2=REDC});

    VKHQ_bufcrtI bufIndcrtI{
		.size = bufSize,
		.usgF = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		.shrM = VK_SHARING_MODE_EXCLUSIVE,
		.memF = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		.buf  = _objs[0].indBuf,
		.dmem = _objs[0].indBufMem,
		.name = "IndexBuffer"
	};crt_buf(bufIndcrtI);

    cpy_buf(stgBuf,_objs[0].indBuf,bufSize);
    
    vkDestroyBuffer(_device,stgBuf,nullptr);
    vkFreeMemory(_device,stgBufMem,nullptr);
    
    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=MAGC,
          .s="Created IndexBuffer!",.hs=PNKC});
}
