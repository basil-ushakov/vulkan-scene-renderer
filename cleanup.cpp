bool VKHQ::cleanup(){
	if(_isInitialized){
		logF({.f=NLNE,.c=1,.b=1,.h=WHTC,
			.s="Cleanup Started:",.hs=WHTC,
			.f2=NLNE|DTAB});

		cleanup_swapChn();
		
		logF({.f=NLNE,.c=1,
			.s="Destroy Descriptors!",.hs=WHTC,
			.f2=NLNE|DTAB});
		for(size_t i=0;i<_unifBufsMems.size();i++){
			logF({.f=NLNE|PVAL,.c=1,
				.s="|> Destroy UniformBuffer number:",.hs=WHTC,
				.v=cstVal(i+1),.bv=3,.hv=WHTC});
			vkDestroyBuffer(_device,_unifBufs[i],nullptr);
			logF({.f=NLNE|PVAL,.c=1,
				.s="|> Free UniformBufferMemory number:",.hs=WHTC,
				.v=cstVal(i+1),.bv=3,.hv=WHTC});
			vkFreeMemory(_device,_unifBufsMems[i],nullptr);
		}
		
		logF({.f=MDTB|NLNE,.c=1,
			.s="Destroy ComputePipeline!",.hs=GRYC});
		vkDestroyPipeline(_device,_compPline,nullptr);
		logF({.f=NLNE,.c=1,
			.s="Destroy ComputePipelineLayout!",.hs=GRYC});
		vkDestroyPipelineLayout(_device,_compPlineLayt,nullptr);
		
		logF({.f=NLNE,.c=1,
			  .s="|> Destroy ComputeUniformBuffer:",.hs=WHTC}); 
		vkDestroyBuffer(_device,_cUnifBuf,nullptr);
		logF({.f=NLNE,.c=1,
		      .s="|> Free ComputeUniformBufferMemory:",.hs=WHTC}); 
		vkFreeMemory(_device,_cUnifBufMem,nullptr);
		
		logF({.f=NLNE,.c=1,
			  .s="|> Destroy ComputeOutUniformBuffer:",.hs=WHTC}); 
		vkDestroyBuffer(_device,_cOutUnifBuf,nullptr);
		logF({.f=NLNE,.c=1,
		      .s="|> Free ComputeOutUniformBufferMemory:",.hs=WHTC}); 
		vkFreeMemory(_device,_cOutUnifBufMem,nullptr);
		
		logF({.f=NLNE,.c=1,
			  .s="|> Destroy DescriptorPool!",.hs=WHTC}); 
		vkDestroyDescriptorPool(_device,_descrPool,nullptr);
		logF({.f=NLNE,.c=1,
			  .s="|> Destroy DescriptorSetLayout!",.hs=WHTC}); 
		vkDestroyDescriptorSetLayout(_device,_descrSetLayt,nullptr);
		
		logF({.f=NLNE,.c=1,
			  .s="|> Destroy ComputeDescriptorPool!",.hs=WHTC}); 
		vkDestroyDescriptorPool(_device,_cDescrPool,nullptr);
		logF({.f=NLNE,.c=1,
			  .s="|> Destroy ComputeDescriptorSetLayout!",.hs=WHTC}); 
		vkDestroyDescriptorSetLayout(_device,_cDescrSetLayt,nullptr);

		logF({.f=NLNE|MDTB,.c=1,
			.s="Destroy ImageView!",.hs=WHTC});
		vkDestroyImageView(_device,_texImgView,nullptr);
		logF({.f=NLNE,.c=1,
			.s="Destroy ImageSampler!",.hs=WHTC});
		vkDestroySampler(_device,_texSmplr,nullptr);
		logF({.f=NLNE,.c=1,
			.s="Destroy ImageBuffer!",.hs=WHTC});
		vkDestroyImage(_device,_texBuf,nullptr);
		logF({.f=NLNE,.c=1,
			.s="Free ImageBufferMemory!",.hs=WHTC});
		vkFreeMemory(_device,_texBufMem,nullptr);

		_objs[0].erase();

// 		logF({.f=NLNE,.c=1,
// 			.s="Destroy IndexBuffer!",.hs=WHTC});
// 		vkDestroyBuffer(_device,_indBuf,nullptr);
// 		logF({.f=NLNE,.c=1,
// 			.s="Free IndexBufferMemory!",.hs=WHTC});
// 		vkFreeMemory(_device,_indBufMem,nullptr);
// 		
// 		logF({.f=NLNE,.c=1,
// 			.s="Destroy VertexBuffer!",.hs=WHTC});
// 		vkDestroyBuffer(_device,_objs[0].vertBuf,nullptr);
// 		logF({.f=NLNE,.c=1,
// 			.s="Free VertexBufferMemory!",.hs=WHTC});
// 		vkFreeMemory(_device,_objs[0].vertBufMem,nullptr);

		logF({.f=NLNE|PVAL,.c=1,
			.s="Rendered frame count is:",.hs=WHTC,
			.v=cstVal(_framCount),.bv=3,.hv=WHTC,
			.f2=NLNE|DTAB});

		for(size_t i=0;i<_framInFlt;i++){
			logF({.f=NLNE|PVAL,.c=1,
					.s="|> Destroy Semaphore AvailbleImage number:",.hs=WHTC,
					.v=cstVal(i+1),.bv=3,.hv=WHTC}); 
			vkDestroySemaphore(_device,_avbleImgSemphr[i], nullptr);
			logF({.f=NLNE|PVAL,.c=1,
					.s="|> Destroy Semaphore FinishedImage number:",.hs=WHTC,
					.v=cstVal(i+1),.bv=3,.hv=WHTC}); 
			vkDestroySemaphore(_device,_finishedImgSemphr[i], nullptr);
			logF({.f=NLNE|PVAL,.c=1,
					.s="|> Destroy Fence InFlight number:",.hs=WHTC,
					.v=cstVal(i+1),.bv=3,.hv=WHTC}); 
			vkDestroyFence(_device,_inFltFens[i], nullptr);
		}

		logF({.f=MDTB|NLNE,.c=1,
			.s="Destroy CommandPool!",.hs=WHTC});
		vkDestroyCommandPool(_device,_cmdPool,nullptr);
		logF({.f=NLNE,.c=1,
			.s="Destroy Device!",.hs=WHTC});
		vkDestroyDevice(_device, nullptr);
		logF({.f=NLNE,.c=1,
			.s="Destroy DebugUtilsMessenger!",.hs=WHTC});
		destr_dbgUtilsMsngr(_instance, _dbgUtilMsngr, nullptr);
		logF({.f=NLNE,.c=1,
			.s="Destroy Surface!",.hs=WHTC});
		vkDestroySurfaceKHR(_instance, _surface, nullptr);
		logF({.f=NLNE,.c=1,
			.s="Destroy Instance!",.hs=WHTC});
		vkDestroyInstance(_instance, nullptr);
		logF({.f=NLNE,.c=1,
			.s="Destroy Window!",.hs=WHTC});
		SDL_DestroyWindow(_window);
		
		std::cout<<"\nCount is: "<<vertCount<<"\n";
		std::cout<<"Size is: "<<verts.size()<<"\n";
		std::cout<<"FramesCount is: "<<_framCount<<"\n";

		logF({.f=NLNE|MDTB,.c=1,.b=1,.h=WHTC,
			.s="Cleanup Finished!",.hs=WHTC});
	}
	_cleanuped=1;
	
	return 1;
}

bool VKHQ::cleanup_swapChn(){
	logF({.f=NLNE,.c=1,.b=1,.h=GRYC,
		  .s="Cleanup SwapChain Started:",.hs=GRYC,
		  .f2=NLNE|DTAB});

	logF({.f=NLNE,.c=1,
		  .s="Destroy DepthView!",.hs=GRYC});
	vkDestroyImageView(_device,_depthView,nullptr);
	logF({.f=NLNE,.c=1,
		  .s="Destroy DepthBuffer!",.hs=GRYC});
	vkDestroyImage(_device,_depthBuf,nullptr);
	logF({.f=NLNE,.c=1,
		  .s="Free DepthBufferMemory!",.hs=GRYC});
	vkFreeMemory(_device,_depthBufMem,nullptr);

	logF({.f=NLNE,.c=1,
		  .s="Destroy ColorView!",.hs=GRYC});
	vkDestroyImageView(_device,_colrView,nullptr);
	logF({.f=NLNE,.c=1,
		  .s="Destroy ColorBuffer!",.hs=GRYC});
	vkDestroyImage(_device,_colrBuf,nullptr);
	logF({.f=NLNE,.c=1,
		  .s="Free ColorBufferMemory!",.hs=GRYC});
	vkFreeMemory(_device,_colrBufMem,nullptr);
	
	uint32_t framBufNumber=0;
	logF({.f=NLNE,.c=1,
		.s="Destroy FrameBuffers!",.hs=GRYC,
		.f2=NLNE|DTAB});
	for(VkFramebuffer framBuf:_swapChnFramBufs){
		framBufNumber++;
		logF({.f=NLNE|PVAL,.c=1,
			  .s="|> Destroy FrameBuffer number:",.hs=GRYC,
			  .v=cstVal(framBufNumber),.bv=3,.hv=GRYC}); 
		vkDestroyFramebuffer(_device, framBuf, nullptr);
	}

	logF({.f=MDTB|NLNE,.c=1,
		  .s="Destroy GraphicalPipeline!",.hs=GRYC});
	vkDestroyPipeline(_device,_graphxPline,nullptr);
	logF({.f=NLNE,.c=1,
		  .s="Destroy GraphicalPipelineLayout!",.hs=GRYC});
	vkDestroyPipelineLayout(_device,_graphxPlineLayt,nullptr);
	logF({.f=NLNE,.c=1,
		  .s="Destroy RenderPass!",.hs=GRYC});
	vkDestroyRenderPass(_device,_rndrPass,nullptr);

	logF({.f=NLNE,.c=1,
		.s="Destroy ImageViews!",.hs=GRYC,
		.f2=NLNE|DTAB});
	uint32_t imgVCount=0;
	for(VkImageView imgV:_swapChnImgViews){
		imgVCount++;
		logF({.f=NLNE|PVAL,.c=1,
			  .s="|> Destroy ImageView number:",.hs=GRYC,
			  .v=cstVal(imgVCount),.bv=3,.hv=GRYC}); 

		vkDestroyImageView(_device,imgV,nullptr);
	}

	logF({.f=MDTB|NLNE,.c=1,
		  .s="Destroy SwapChain!",.hs=GRYC});
	vkDestroySwapchainKHR(_device,_swapChn,nullptr);

	logF({.f=MDTB|NLNE,.c=1,.b=1,.h=GRYC,
		  .s="Cleanup SwapChain Finished!",.hs=GRYC});

	return 1;
}
