void VKHQ::crt_rndrPass(){
    logF({.f=NLNE,.c=1,.b=1,.h=REDC,
          .s="Creating RenderPass:",.hs=REDC,
          .f2=NLNE|DTAB});

	//ToDo:
	//------------------
	//Recheck this!!!

	VkAttachmentDescription colrAttch{
		.format=_swapChnFormat,
		.samples=msaaSmpls,
		.loadOp=VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp=VK_ATTACHMENT_STORE_OP_STORE,
		.stencilLoadOp=VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp=VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout=VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout=VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
	};

	VkAttachmentDescription depthAttch{
		.format=get_depthFormat(),
		.samples=msaaSmpls,
		.loadOp=VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp=VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.stencilLoadOp=VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp=VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout=VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout=VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
	};

	VkAttachmentDescription colrAttchRslf{
		.format=_swapChnFormat,
		.samples=VK_SAMPLE_COUNT_1_BIT,
		.loadOp=VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.storeOp=VK_ATTACHMENT_STORE_OP_STORE,
		.stencilLoadOp=VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp=VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout=VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout=VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
	};

	VkAttachmentReference colrAttchRef{
		.attachment=0,
		.layout=VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
	};

	VkAttachmentReference depthAttchRef{
		.attachment=1,
		.layout=VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
	};

	VkAttachmentReference colrAttchRslfRef{
		.attachment=2,
		.layout=VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
	};

	VkSubpassDescription subpass{
		.pipelineBindPoint=VK_PIPELINE_BIND_POINT_GRAPHICS,
		.colorAttachmentCount=1,
		.pColorAttachments=&colrAttchRef,
		.pResolveAttachments=&colrAttchRslfRef,
		.pDepthStencilAttachment=&depthAttchRef,
	};

	VkSubpassDependency dpnds{
		.srcSubpass=VK_SUBPASS_EXTERNAL,
		.dstSubpass=0,
		.srcStageMask=VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT|
					  VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
		.dstStageMask=VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT|
					  VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
		.srcAccessMask=0,
		.dstAccessMask=VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT|
					   VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
	};

	std::array<VkAttachmentDescription,3> attchs={
		colrAttch,depthAttch,colrAttchRslf
	};

	logF({.f=NLNE|PVAL,.c=1,
		  .s="Attachment count is:",.hs=REDC,
		  .v=cstVal(attchs.size()),.bv=1,.hv=PNKC});

	VkRenderPassCreateInfo rndrPassInfo{
		.sType=VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.attachmentCount=attchs.size(),
		.pAttachments=attchs.data(),
		.subpassCount=1,
		.pSubpasses=&subpass,
		.dependencyCount=1,
		.pDependencies=&dpnds,
	};

	if(vkCreateRenderPass(_device,&rndrPassInfo,nullptr,&_rndrPass)!=VK_SUCCESS){
		wrtSysMsg(RERR,"Failed to create RenderPass!");
	}

    logF({.f=MDTB|NLNE,.c=1,.b=1,.h=REDC,
          .s="Created RenderPass!",.hs=REDC});
}

void VKHQ::crt_graphxPline(VKHQ_gplineI Info){
    logF({.f=NLNE,.c=1,.b=1,.h=LIMC,
          .s="Creating GraphicalPipeline:",.hs=LIMC,
          .f2=NLNE|DTAB});
	
	VkPushConstantRange pushCnst{
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
		.offset	    = 0,
		.size 		= sizeof(MVP)
	};
	
	VkPipelineLayoutCreateInfo plineLaytInfo{
		.sType 					= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.flags 					= 0,
		.setLayoutCount		    = 1,
		.pSetLayouts 			= &_descrSetLayt,
		.pushConstantRangeCount = 1,
		.pPushConstantRanges 	= &pushCnst,
	};

	if(vkCreatePipelineLayout(_device,&plineLaytInfo,nullptr,&Info.plineLayt)!=VK_SUCCESS){
		wrtSysMsg(RERR,"Failed to create PipelineLayout!");
	}

	std::vector<char> vShaderBin = read_f(Info.vs,std::ios::binary);
	std::vector<char> fShaderBin = read_f(Info.fs,std::ios::binary);

	VkShaderModule vShaderModl = crt_shaderModl(vShaderBin);
	VkShaderModule fShaderModl = crt_shaderModl(fShaderBin);

	VkPipelineShaderStageCreateInfo vShaderStage{
		.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage  = VK_SHADER_STAGE_VERTEX_BIT,
		.module = vShaderModl,
		.pName  = "main",
	};
		
	VkPipelineShaderStageCreateInfo fShaderStage{
		.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
		.module = fShaderModl,
		.pName  = "main",
	};
	
	VkPipelineShaderStageCreateInfo shaderStages[]={
		vShaderStage,fShaderStage
	};
	
	VkPipelineInputAssemblyStateCreateInfo inpAssmblyState{
		.sType 					= VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.flags 					= Info.tplgF,
		.topology 				= Info.tplg,
		.primitiveRestartEnable = Info.prmRst,
	};
	
	auto bindDscr = Vertex::get_bindDscr();
	auto attrDscr = Vertex::get_attrDscr();

	VkPipelineVertexInputStateCreateInfo vInpState{
		.sType 							 = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount 	 = 1,
		.pVertexBindingDescriptions 	 = &bindDscr,
		.vertexAttributeDescriptionCount = static_cast<uint32_t>(attrDscr.size()),
		.pVertexAttributeDescriptions 	 = attrDscr.data(),
	};
	
	VkViewport viewport{
		.x 		  = Info.vx,
		.y 		  = Info.vy,
		.width    = Info.vw,
		.height   = Info.vh,
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};
	
	VkOffset2D scissorOffset{
		(int32_t)Info.sx,(int32_t)Info.sy
	};
	
	VkRect2D scissor{
		.offset = scissorOffset,
		.extent = {(uint32_t)Info.vw-(uint32_t)Info.sx,
				   (uint32_t)Info.vh-(uint32_t)Info.sy},
	};

	logF({.f=NLNE,.c=1,
		  .s="Current Resolution:",.hs=LIMC,
		  .s2=(cstStr((uint32_t)Info.vw-(int32_t)Info.sx)+":X|"+
			   cstStr((uint32_t)Info.vh-(int32_t)Info.sy)+":Y"),.bs2=1,.hs2=YLWC});

	VkPipelineViewportStateCreateInfo viewportState{
		.sType 		   = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.pViewports    = &viewport,
		.scissorCount  = 1,
		.pScissors 	   = &scissor,
	};

	VkPipelineRasterizationStateCreateInfo rasterState{
		.sType 					 = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.flags 					 = Info.rastF,
		.depthClampEnable		 = Info.dpthClmp,
		.rasterizerDiscardEnable = Info.rastDscrd,
		.polygonMode 			 = Info.plygnM,
		.cullMode 				 = Info.cullM,
		.frontFace 				 = Info.frntFs,
		.depthBiasEnable 		 = Info.dpthBias,
		.depthBiasConstantFactor = Info.dpthBiasCnst,
		.depthBiasClamp 		 = Info.dpthBiasClmp,
		.depthBiasSlopeFactor 	 = Info.dpthBiasSlpe,
		.lineWidth 				 = Info.lnw,
	};

	VkPipelineMultisampleStateCreateInfo mltSampleState{
		.sType 				   = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.rasterizationSamples  = (VkSampleCountFlagBits)(Info.smpls),
		.sampleShadingEnable   = Info.smplShd,
		.minSampleShading 	   = Info.smplShdMin,
		.pSampleMask 		   = Info.smplMsk,
		.alphaToCoverageEnable = Info.alphaToCov,
		.alphaToOneEnable 	   = Info.alphaToOne,
	};

	VkPipelineDepthStencilStateCreateInfo depthStnclState{
		.sType 				   = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.flags 				   = Info.dpthStnclF,
		.depthTestEnable 	   = Info.dpthTest,
		.depthWriteEnable 	   = Info.dpthWrt,
		.depthCompareOp 	   = Info.dpthOp,
		.depthBoundsTestEnable = Info.dpthBndsTest,
		.stencilTestEnable 	   = Info.stnclTest,
		.front				   = Info.stnclFront,
		.back				   = Info.stnclBack,
		.minDepthBounds 	   = Info.dpthBndsMin,
		.maxDepthBounds 	   = Info.dpthBndsMax,
	};

	VkPipelineColorBlendAttachmentState colrBlendAttch{
		.blendEnable 		 = Info.blnd,
		.srcColorBlendFactor = Info.blndClrSrc,
		.dstColorBlendFactor = Info.blndClrDst,
		.colorBlendOp 		 = Info.blndClrOp,
		.srcAlphaBlendFactor = Info.blndAlphSrc,
		.dstAlphaBlendFactor = Info.blndAlphDst,
		.alphaBlendOp 		 = Info.blndAlphOp,
		.colorWriteMask		 = Info.blndMsk
	};

	float* blendConstants = Info.blndCnsts;
	VkPipelineColorBlendStateCreateInfo colrBlendState{
		.sType 			 = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.flags 			 = Info.blndClrF,
		.logicOpEnable 	 = Info.lOp==127?VK_FALSE:VK_TRUE,
		.logicOp 		 = Info.lOp,
		.attachmentCount = 1,
		.pAttachments 	 = &colrBlendAttch,
		.blendConstants  = {
							Info.blndCnsts[0],
							Info.blndCnsts[1],
							Info.blndCnsts[2],
							Info.blndCnsts[3],
						   }
	};

	float* blndColr = Info.blndClr;

#ifndef NDEBUG
	EnumList colrChnls;
	colrChnls.push_back("R=["+cstStr(blndColr[0])+"]");
	colrBlendState.blendConstants[0]=blndColr[0];
	colrChnls.push_back("G=["+cstStr(blndColr[1])+"]");
	colrBlendState.blendConstants[1]=blndColr[1];
	colrChnls.push_back("B=["+cstStr(blndColr[2])+"]");
	colrBlendState.blendConstants[2]=blndColr[2];
	colrChnls.push_back("A=["+cstStr(blndColr[3])+"]");
	colrBlendState.blendConstants[3]=blndColr[3];

	logF({.f=NLNE|ENUM,.c=1,.b=9,.h=GRNC,
		.s="Blend ColorChannels:",.hs=LIMC,
		.vec=colrChnls,.be=3,.he=PNKC});
#endif

	logF({.f=NLNE|PVAL,.c=1,
		  .s="ShaderStages count is:",.hs=LIMC,
		  .v=cstVal(std::size(shaderStages)),.bv=1,.hv=YLWC});

	VkGraphicsPipelineCreateInfo plineInfo{
		.sType 				 = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.flags 				 = 0,
		.stageCount 		 = std::size(shaderStages),
		.pStages 		 	 = shaderStages,
		.pVertexInputState	 = &vInpState,
		.pInputAssemblyState = &inpAssmblyState,
		// .pTessellationState = ,
		.pViewportState 	 = &viewportState,
		.pRasterizationState = &rasterState,
		.pMultisampleState 	 = &mltSampleState,
		.pDepthStencilState  = &depthStnclState,
		.pColorBlendState 	 = &colrBlendState,
		// .pDynamicState = ,
		.layout 			 = _graphxPlineLayt,
		.renderPass 		 = _rndrPass,
		.subpass 			 = 0,
		.basePipelineHandle  = VK_NULL_HANDLE,
		.basePipelineIndex	 = 0,
	};
	
	if(vkCreateGraphicsPipelines(_device,VK_NULL_HANDLE,1,&plineInfo,nullptr,&Info.pline)!=VK_SUCCESS){
		wrtSysMsg(RERR,"Failed to create GraphicalPipeline!");
	}

	vkDestroyShaderModule(_device,fShaderModl,nullptr);
	vkDestroyShaderModule(_device,vShaderModl,nullptr);

    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=LIMC,
          .s="Created GraphicalPipeline!",.hs=LIMC});
}

void VKHQ::crt_framBuf(){
	logF({.f=NLNE,.c=1,.b=1,.h=REDC,
		  .s="Creating FrameBuffer:",.hs=YLWC,
          .f2=NLNE|DTAB});
	s swapChnImgsSize=_swapChnImgViews.size();
	_swapChnFramBufs.resize(swapChnImgsSize);
    logF({.f=NLNE,.c=1,
          .s="Resizing SwapChainFramebuffers array!",.hs=YLWC});

    logF({.f=NLNE,.c=1,
          .s="Creating SwapChain FrameBuffers:",.hs=YLWC,
          .f2=NLNE|DTAB});

	for(size_t i=0;i<swapChnImgsSize;i++){
		std::array<VkImageView,3> attchs={
			_colrView,
			_depthView,
			_swapChnImgViews[i],
		};

		VkFramebufferCreateInfo framBufInfo{
			.sType=VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			.renderPass=_rndrPass,
			.attachmentCount=attchs.size(),
			.pAttachments=attchs.data(),
			.width=_swapChnExtent.width,
			.height=_swapChnExtent.height,
			.layers=1,
		};

		if(vkCreateFramebuffer(_device,&framBufInfo,nullptr,&_swapChnFramBufs[i])!=VK_SUCCESS){
			wrtSysMsg(RERR,"Failed to create FrameBuffer!");
		}

		logF({.f=NLNE|PVAL,.c=1,
			  .s=">|Created FrameBuffer number",.hs=YLWC,
			  .v=cstVal(i+1),.bv=3,.hv=YLWC}); 
	}

	logF({.f=MDTB|NLNE|PVAL,.c=1,
		  .s="FrameBuffer count is:",.hs=YLWC,
		  .v=cstVal(_swapChnFramBufs.size()),.bv=3,.hv=YLWC});

	logF({.f=NLNE|MDTB,.c=1,.b=1,.h=REDC,
		  .s="Created FrameBuffer!",.hs=YLWC});	
}


