void VKHQ::crt_syncObj(){
    logF({.f=NLNE,.c=1,.b=1,.h=CYNC,
          .s="Creating SynchronizationObjects:",.hs=LIMC,
          .f2=NLNE|DTAB});

    _avbleImgSemphr.resize(_framInFlt);
    _finishedImgSemphr.resize(_framInFlt);
    _inFltFens.resize(_framInFlt);

    VkSemaphoreCreateInfo semphrInfo{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };

    VkFenceCreateInfo fensInfo{
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    for(size_t i=0;i<_framInFlt;i++){
        if(vkCreateSemaphore(_device,&semphrInfo,nullptr,&_avbleImgSemphr[i])!=VK_SUCCESS){
            wrtSysMsg(RERR,"Failed to create Semaphore AvailableImage!");
        }
        if(vkCreateSemaphore(_device,&semphrInfo,nullptr,&_finishedImgSemphr[i])!=VK_SUCCESS){
            wrtSysMsg(RERR,"Failed to create Semaphore FinishedImage!");
        }
        if(vkCreateFence(_device,&fensInfo,nullptr,&_inFltFens[i])!=VK_SUCCESS){
            wrtSysMsg(RERR,"Failed to create Fence InFlight!");
        }
    }

    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=CYNC,
          .s="Created SynchronizationObjects!",.hs=LIMC});
}
