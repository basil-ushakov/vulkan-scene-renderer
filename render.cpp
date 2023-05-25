void VKHQ::draw(){
	//ToDo:
	//------------------
	//Recheck this!
    
    vkWaitForFences(_device,1,&_inFltFens[_crntFram],VK_TRUE,UINT64_MAX);

    u32      imgInd;
    VkResult Rslt = vkAcquireNextImageKHR(_device,_swapChn,UINT64_MAX,_avbleImgSemphr[_crntFram],VK_NULL_HANDLE,&imgInd);

    if(Rslt==VK_ERROR_OUT_OF_DATE_KHR){
    logF({.f=NLNE,.c=1,
		  .s="RECREATING",.hs=REDC});
        rcrt_swapchn();
        return;
    } else if(Rslt!=VK_SUCCESS&&Rslt!=VK_SUBOPTIMAL_KHR){
        wrtSysMsg(RERR,"Failed to acquire SwapChainImage!");
    }

    upd_unifBuf(_crntFram);

    vkResetFences(_device,1,&_inFltFens[_crntFram]);
    vkResetCommandBuffer(_cmdBuf[_crntFram],0);
    use_cmdBuf(_cmdBuf[_crntFram],imgInd);
    
    VkSemaphore waitSemphrs[] = {
        _avbleImgSemphr[_crntFram]
    };

    VkPipelineStageFlags waitStgs[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };

    VkSemaphore sgnlSemphrs[] = {
        _finishedImgSemphr[_crntFram]
    };

    VkSubmitInfo sbmtInfo{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = waitSemphrs,
        .pWaitDstStageMask = waitStgs,
        .commandBufferCount = 1,
        .pCommandBuffers = &_cmdBuf[_crntFram],
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = sgnlSemphrs,
    };

    if(vkQueueSubmit(_graphxQueue,1,&sbmtInfo,_inFltFens[_crntFram])!=VK_SUCCESS){
        wrtSysMsg(RERR,"Failed to submit Fence InFlight!");
    }

    VkSwapchainKHR swpChns[] = {
        _swapChn
    };

    VkPresentInfoKHR prsntInfo{
        .sType=VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount=1,
        .pWaitSemaphores=sgnlSemphrs,
        .swapchainCount=std::size(swpChns),
        .pSwapchains=swpChns,
        .pImageIndices=&imgInd,
    };
    
    Rslt = vkQueuePresentKHR(_presentQueue,&prsntInfo);

    if(Rslt==VK_ERROR_OUT_OF_DATE_KHR||Rslt==VK_SUBOPTIMAL_KHR){
        logF({.f=NLNE,.c=1,
              .s="RECREATING",.hs=REDC});
        rcrt_swapchn();
    }
    else if(Rslt!=VK_SUCCESS){
        wrtSysMsg(RERR,"Failed to present SwapChainImage!");
    }

    _crntFram=(_crntFram+1)%_framInFlt;
}
