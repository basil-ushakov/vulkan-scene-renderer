void VKHQ::crt_compPline(VKHQ_cplineI Info){
    logF({.f=NLNE,.c=1,.b=1,.h=WHTC,
          .s="Creating ComputePipeline:",.hs=LIMC,
          .f2=NLNE|DTAB});
	
	VkPipelineLayoutCreateInfo plineLaytInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.flags = Info.lf,
		.setLayoutCount = 1,
		.pSetLayouts = &_cDescrSetLayt,
		.pushConstantRangeCount = 0,
		.pPushConstantRanges = nullptr,
	};

	if(vkCreatePipelineLayout(_device,&plineLaytInfo,nullptr,&Info.plineLayt)!=VK_SUCCESS){
		wrtSysMsg(RERR,"Failed to create PipelineLayout!");
	}

	std::vector<char> cShaderBin = read_f(Info.s,std::ios::binary);
	
	VkShaderModule cShaderModl = crt_shaderModl(cShaderBin);
	
	VkPipelineShaderStageCreateInfo cShaderStage{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.flags = Info.sf,
		.stage = VK_SHADER_STAGE_COMPUTE_BIT,
		.module = cShaderModl,
		.pName = "main",
	};

	VkComputePipelineCreateInfo plineInfo{
		.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
		.pNext = nullptr,
		.flags = Info.pf,
		.stage = cShaderStage,
		.layout = Info.plineLayt,
		.basePipelineHandle = VK_NULL_HANDLE,
		.basePipelineIndex = 0
	};

	if(vkCreateComputePipelines(_device,VK_NULL_HANDLE,1,&plineInfo,nullptr,&Info.pline)!=VK_SUCCESS){
		wrtSysMsg(RERR,"Failed to create ComputePipeline!");
	}

	vkDestroyShaderModule(_device,cShaderModl,nullptr);

    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=WHTC,
          .s="Created ComputePipeline!",.hs=LIMC});
}


