// void VKHQ::crt_cmdPool(VkCommandPoolCreateFlags flag,uint32_t qFam,VkCommandPool* cmdPool){
void VKHQ::crt_cmdPool(VKHQ_cmdpoolcrtI info) {
    logF({.f=NLNE,.c=1,.b=1,.h=CYNC,
          .s="Creating CommandPool:",.hs=YLWC,
          .f2=NLNE|DTAB});

	VkCommandPoolCreateInfo poolInfo{
		.sType=VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags=info.f,
		.queueFamilyIndex=info.qFam,
	};
	
	EnumList flagNames;
	if(info.f & VK_COMMAND_POOL_CREATE_TRANSIENT_BIT)
		flagNames.push_back("TRANSIENT");
	if(info.f & VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)
		flagNames.push_back("RESET COMMANDBUFFER");
	if(info.f & VK_COMMAND_POOL_CREATE_PROTECTED_BIT)
		flagNames.push_back("PROTECTED");

	logF({.f=NLNE|ENUM,.c=1,.b=9,.h=YLWC,
		  .s="CommandBuffer Flags:",.hs=YLWC,
		  .vec=flagNames,.be=3,.he=YLWC});

	logF({.f=NLNE|PVAL,.c=1,
		  .s="QueueFamilyIndex is: ",.hs=YLWC,
		  .v=cstVal(info.qFam),.hv=LBLC});

	if(vkCreateCommandPool(_device,&poolInfo,nullptr,&info.cmdp)!=VK_SUCCESS){
		wrtSysMsg(RERR,"Couldn't create CommandPool!");
	}

    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=CYNC,
          .s="Created CommandPool!",.hs=YLWC});
}

void VKHQ::crt_cmdBuf(){
    logF({.f=NLNE,.c=1,.b=1,.h=REDC,
          .s="Creating CommandBuffer:",.hs=GLDC,
          .f2=NLNE|DTAB});

	_cmdBuf.resize(_framInFlt);
	logF({.f=NLNE|PVAL,.c=1,
		  .s="CommandBuffer count is: ",.hs=REDC,
		  .v=cstVal(_cmdBuf.size()),.hv=CYNC});

	VkCommandBufferAllocateInfo alcInfo{
		.sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool=_cmdPool,
		.level=VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount=(uint32_t) _cmdBuf.size(),
	};
	
	if(vkAllocateCommandBuffers(_device, &alcInfo, _cmdBuf.data())!=VK_SUCCESS){
		wrtSysMsg(RERR,"Couldn't create CommandBuffer!");
	}

    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=REDC,
          .s="Created CommandBuffer!",.hs=GLDC});
}

void VKHQ::use_cmdBuf(VkCommandBuffer cmdBuf,uint32_t imgInd){
	t.start();
	float timer=0.0f;
	while((t.end())<1000/_framLim){
		timer+=t.end();
	}

	VkCommandBufferBeginInfo cmdBufBgnInfo{
		.sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags=0,
	};

	if(vkBeginCommandBuffer(cmdBuf,&cmdBufBgnInfo)!=VK_SUCCESS){
		wrtSysMsg(RERR,"Failed to start CommandBuffer!");
	}

	std::array<VkClearValue,2> clrColr{};
		clrColr[0].color={.07f,.07f,.07f,.0f};
		clrColr[1].depthStencil={1.0f,0};

	VkRenderPassBeginInfo rndrPassInfo{
		.sType=VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass=_rndrPass,
		.framebuffer=_swapChnFramBufs[imgInd],
		.clearValueCount=std::size(clrColr),
		.pClearValues=&clrColr[0],
	};
	rndrPassInfo.renderArea.offset={0,0};
	rndrPassInfo.renderArea.extent=_swapChnExtent;

	vkCmdBeginRenderPass(cmdBuf,&rndrPassInfo,VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(cmdBuf,VK_PIPELINE_BIND_POINT_GRAPHICS,_graphxPline);

		VkBuffer vertBufs[]={_objs[0].vertBuf};

		VkDeviceSize offsets[]={0};
		vkCmdBindVertexBuffers(cmdBuf,0,1,vertBufs,offsets);
		vkCmdBindIndexBuffer(cmdBuf,_objs[0].indBuf,0,VK_INDEX_TYPE_UINT32);

		if(viewAng.x>360.f)viewAng.x=  0.0f;
		else if(viewAng.x<  0.f)viewAng.x=360.0f;
		if(viewAng.y> 89.f)viewAng.y= 89.0f;
		else if(viewAng.y<-89.f)viewAng.y=-89.0f;
		if(viewAng.z>360.f)viewAng.z=  0.0f;
		else if(viewAng.z<  0.f)viewAng.z=360.0f;

		glm::vec3 dir;
				  dir.x   = cos(glm::radians(viewAng.x)) * cos(glm::radians(viewAng.y));
				  dir.y   = sin(glm::radians(viewAng.x)) * cos(glm::radians(viewAng.y));
				  dir.z   = sin(glm::radians(viewAng.y));
		glm::vec3 viewDir = glm::normalize(dir);

		objAng.second=vec3(0.f,0.f,1.f);
		glm::mat4 m 	   = glm::rotate(glm::mat4(1.0f),radians(objAng.first),objAng.second);
		glm::mat4 v 	   = glm::lookAt(viewPos,viewPos+viewDir, glm::vec3(0.f,0.f,1.f)+viewAbs);
		glm::mat4 p		   = glm::perspective(glm::radians(60.f), _swapChnExtent.width / (float) _swapChnExtent.height, 0.1f, 1000.0f);
				  p[1][1] *= -1;

		MVP pconst;
		pconst.m = m;
		pconst.v = v;
		pconst.p = p;

		vkCmdPushConstants(cmdBuf,_graphxPlineLayt,VK_SHADER_STAGE_VERTEX_BIT,0,sizeof(MVP), &pconst);
		vkCmdBindDescriptorSets(cmdBuf,VK_PIPELINE_BIND_POINT_GRAPHICS,_graphxPlineLayt,0,1,&_descrSets[_crntFram],0,nullptr);

		vkCmdDrawIndexed(cmdBuf,static_cast<uint32_t>(inds.size()),1,0,0,0);

	vkCmdEndRenderPass(cmdBuf);

	if(vkEndCommandBuffer(cmdBuf)!=VK_SUCCESS){
		wrtSysMsg(RERR,"Failed to end CommandBuffer!");
	}

	t.end();

	_framCount++;
}

VkCommandBuffer VKHQ::use_singleCmd(){
    logF({.f=NLNE,.c=1,.b=1,.h=CYNC,
          .s="Starting using of SingleCommandBuffer:",.hs=LBLC,
          .f2=NLNE|DTAB});

	VkCommandBufferAllocateInfo allocInfo{
		.sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool=_cmdPool,
		.level=VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount=1,
	};

	VkCommandBuffer cmdBuf;

	logF({.f=NLNE,.c=1,
		  .s="Allocating CommandBuffer!",.hs=LBLC});
	vkAllocateCommandBuffers(_device,&allocInfo,&cmdBuf);

	VkCommandBufferBeginInfo bgnInfo{
		.sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags=VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
	};

	logF({.f=NLNE,.c=1,
		  .s="Recording in SingleCommandBuffer!",.hs=LBLC});

	vkBeginCommandBuffer(cmdBuf,&bgnInfo);

	return cmdBuf;
}

void VKHQ::end_singleCmd(VkCommandBuffer cmdBuf,VkQueue& queue){
	vkEndCommandBuffer(cmdBuf);

	logF({.f=NLNE,.c=1,
		  .s="Recorded in SingleCommandBuffer!",.hs=LBLC});

	VkSubmitInfo sbmtInfo{
		.sType=VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.commandBufferCount=1,
		.pCommandBuffers=&cmdBuf,
	};

	logF({.f=NLNE,.c=1,
		  .s="Submiting SingleCommandBuffer into queue!",.hs=LBLC});

	vkQueueSubmit(queue,1,&sbmtInfo,VK_NULL_HANDLE);
	vkQueueWaitIdle(queue);

	vkFreeCommandBuffers(_device,_cmdPool,1,&cmdBuf);
    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=CYNC,
          .s="Finished using of SingleCommandBuffer!",.hs=LBLC});
}
