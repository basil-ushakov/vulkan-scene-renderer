void VKHQ::crt_descr(){
    logF({.f=NLNE,.c=1,.b=1,.h=WATC,
          .s="Creating Descriptors:",.hs=WATC,
          .f2=NLNE|DTAB});
    VkDescriptorSetLayoutBinding UBOLaytBind{
		.binding=0,
		.descriptorType=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount=1,
		.stageFlags=VK_SHADER_STAGE_FRAGMENT_BIT,
		.pImmutableSamplers=nullptr,
	};
	VkDescriptorSetLayoutBinding smplrLaytBind{
		.binding=1,
		.descriptorType=VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.descriptorCount=1,
		.stageFlags=VK_SHADER_STAGE_FRAGMENT_BIT,
		.pImmutableSamplers=nullptr,
	};

	std::array<VkDescriptorSetLayoutBinding,2> binds = {
		UBOLaytBind,smplrLaytBind
	};
	
	VkDescriptorSetLayoutCreateInfo descrLaytInfo{
		.sType=VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.bindingCount=binds.size(),
		.pBindings=binds.data(),
	};
	
	if(vkCreateDescriptorSetLayout(_device,&descrLaytInfo,nullptr,&_descrSetLayt)!=VK_SUCCESS){
		wrtSysMsg(RERR,"Failed to create DescriptorSetLayout!");
	}

	VkDescriptorSetLayoutBinding CUBOLaytBind{
		.binding=0,
		.descriptorType=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount=1,
		.stageFlags=VK_SHADER_STAGE_COMPUTE_BIT,
		.pImmutableSamplers=nullptr,
	};
	VkDescriptorSetLayoutBinding CUBOOutLaytBind{
		.binding=1,
		.descriptorType=VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
		.descriptorCount=1,
		.stageFlags=VK_SHADER_STAGE_COMPUTE_BIT,
		.pImmutableSamplers=nullptr,
	};

	std::array<VkDescriptorSetLayoutBinding,2> binds2 = {
		CUBOLaytBind,CUBOOutLaytBind
	};

	VkDescriptorSetLayoutCreateInfo descrLaytInfo2{
		.sType=VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.bindingCount=binds2.size(),
		.pBindings=binds2.data(),
	};

	if(vkCreateDescriptorSetLayout(_device,&descrLaytInfo2,nullptr,&_cDescrSetLayt)!=VK_SUCCESS){
		wrtSysMsg(RERR,"Failed to create ComputeDescriptorSetLayout!");
	}

	logF({.f=NLNE|MDTB,.c=1,.b=1,.h=WATC,
          .s="Created Descriptors!",.hs=WATC});
}

void VKHQ::crt_unifBuf(){
    logF({.f=NLNE,.c=1,.b=1,.h=GRNC,
          .s="Creating UniformBuffer:",.hs=BLUC,
          .f2=NLNE|DTAB});

	VkDeviceSize bufSize2{sizeof(UBO)};
	VkDeviceSize bufSize3{sizeof(CUBO)};
	VkDeviceSize bufSize4{sizeof(CUBOOut)};

	_unifBufs.resize(_framInFlt);
	_unifBufsMems.resize(_framInFlt);

	for(size_t i=0;i<_framInFlt;i++){
		VKHQ_bufcrtI bufcrtI{
			.size = bufSize2,
			.usgF = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			.shrM = VK_SHARING_MODE_EXCLUSIVE,
			.memF = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			.buf  = _unifBufs[i],
			.dmem = _unifBufsMems[i],
			.name = "UniformBuffer",
		};crt_buf(bufcrtI);
	}

	VKHQ_bufcrtI bufCcrtI{
		.size = bufSize3,
		.usgF = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		.shrM = VK_SHARING_MODE_EXCLUSIVE,
		.memF = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		.buf  = _cUnifBuf,
		.dmem = _cUnifBufMem,
		.name = "ComputeUniformBuffer",
	};crt_buf(bufCcrtI);

	VKHQ_bufcrtI bufCOutcrtI{
		.size = bufSize4,
		.usgF = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
		.shrM = VK_SHARING_MODE_EXCLUSIVE,
		.memF = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		.buf  = _cOutUnifBuf,
		.dmem = _cOutUnifBufMem,
		.name = "ComputeOutUniformBuffer",
	};crt_buf(bufCOutcrtI);

    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=GRNC,
          .s="Created UniformBuffer!",.hs=BLUC});
}

void VKHQ::crt_descrPool(){
    logF({.f=NLNE,.c=1,.b=1,.h=LBLC,
          .s="Creating DescriptorPool:",.hs=BLUC,
          .f2=NLNE|DTAB});

	std::array<VkDescriptorPoolSize,2> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(_framInFlt);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(_framInFlt);

	VkDescriptorPoolCreateInfo poolInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.maxSets = static_cast<uint32_t>(_framInFlt),
		.poolSizeCount = poolSizes.size(),
		.pPoolSizes = poolSizes.data(),
	};
	if(vkCreateDescriptorPool(_device,&poolInfo,nullptr,&_descrPool)!=VK_SUCCESS){
		wrtSysMsg(RERR,"Failed to create DescriptorPool!");
	}

	std::array<VkDescriptorPoolSize,2> poolSizes2{};
	poolSizes2[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes2[0].descriptorCount = 1;
	poolSizes2[1].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	poolSizes2[1].descriptorCount = 1;
	
	VkDescriptorPoolCreateInfo poolInfo2{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.maxSets = 1,
		.poolSizeCount = poolSizes2.size(),
		.pPoolSizes = poolSizes2.data(),
	};
	if(vkCreateDescriptorPool(_device,&poolInfo2,nullptr,&_cDescrPool)!=VK_SUCCESS){
		wrtSysMsg(RERR,"Failed to create ComputeDescriptorPool!");
	}
	
    logF({.f=NLNE|MDTB,.c=1,.b=1,.h=LBLC,
          .s="Created DescriptorPool!",.hs=BLUC});
}

void VKHQ::crt_descrSets(){
    logF({.f=NLNE,.c=1,.b=1,.h=WATC,
          .s="Creating DescriptorSet:",.hs=YLWC,
          .f2=NLNE|DTAB});
	
	std::vector<VkDescriptorSetLayout> layts(_framInFlt, _descrSetLayt);
	VkDescriptorSetAllocateInfo allocInfo{
		.sType=VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool=_descrPool,
		.descriptorSetCount=static_cast<uint32_t>(_framInFlt),
		.pSetLayouts=layts.data(),
	};

	_descrSets.resize(_framInFlt);
	if(vkAllocateDescriptorSets(_device,&allocInfo,_descrSets.data())!=VK_SUCCESS){
		wrtSysMsg(RERR,"Failed to allocate DescriptorSets!");
	}

	for(size_t i=0;i<_framInFlt;i++){
		VkDescriptorBufferInfo descrBuf{
			.buffer=_unifBufs[i],
			.offset=0,
			.range=sizeof(UBO),
		};
		VkDescriptorImageInfo imgInfo{
			.sampler=_texSmplr,
			.imageView=_texImgView,
			.imageLayout=VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		};
		std::array<VkWriteDescriptorSet,2> wrtDescSets{};
			wrtDescSets[0].sType=VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			wrtDescSets[0].dstSet=_descrSets[i];
			wrtDescSets[0].dstBinding=0;
			wrtDescSets[0].dstArrayElement=0;
			wrtDescSets[0].descriptorCount=1;
			wrtDescSets[0].descriptorType=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			wrtDescSets[0].pBufferInfo=&descrBuf;

			wrtDescSets[1].sType=VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			wrtDescSets[1].dstSet=_descrSets[i];
			wrtDescSets[1].dstBinding=1;
			wrtDescSets[1].dstArrayElement=0;
			wrtDescSets[1].descriptorCount=1;
			wrtDescSets[1].descriptorType=VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			wrtDescSets[1].pImageInfo=&imgInfo;

		vkUpdateDescriptorSets(_device,static_cast<uint32_t>(wrtDescSets.size()),wrtDescSets.data(),0,nullptr);
	}
	
	VkDescriptorSetAllocateInfo allocInfo2{
		.sType=VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool=_cDescrPool,
		.descriptorSetCount = 1,
		.pSetLayouts=&_cDescrSetLayt,
	};
 
	if(vkAllocateDescriptorSets(_device,&allocInfo2,&_cDescrSet)!=VK_SUCCESS){
		wrtSysMsg(RERR,"Failed to allocate ComputeDescriptorSet!");
	}
 
	VkDescriptorBufferInfo descrBuf3{
		.buffer=_cUnifBuf,
		.offset=0,
		.range=sizeof(CUBO),
	};

	VkDescriptorBufferInfo descrBuf4{
		.buffer=_cOutUnifBuf,
		.offset=0,
		.range=sizeof(CUBOOut),
	};
 
	std::array<VkWriteDescriptorSet,2> wrtDescSets2{};
	wrtDescSets2[0].sType=VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	wrtDescSets2[0].dstSet=_cDescrSet;
	wrtDescSets2[0].dstBinding=0;
	wrtDescSets2[0].dstArrayElement=0;
	wrtDescSets2[0].descriptorCount=1;
	wrtDescSets2[0].descriptorType=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	wrtDescSets2[0].pBufferInfo=&descrBuf3;
	
	wrtDescSets2[1].sType=VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	wrtDescSets2[1].dstSet=_cDescrSet;
	wrtDescSets2[1].dstBinding=1;
	wrtDescSets2[1].dstArrayElement=0;
	wrtDescSets2[1].descriptorCount=1;
	wrtDescSets2[1].descriptorType=VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	wrtDescSets2[1].pBufferInfo=&descrBuf4;
 
	vkUpdateDescriptorSets(_device,static_cast<uint32_t>(wrtDescSets2.size()),wrtDescSets2.data(),0,nullptr);

	logF({.f=NLNE|MDTB,.c=1,.b=1,.h=WATC,
          .s="Created DescriptorSet!",.hs=YLWC});
}

using namespace glm;

void VKHQ::upd_unifBuf(uint32_t crntImg){
	static auto start = std::chrono::high_resolution_clock::now();
    auto curnt = std::chrono::high_resolution_clock::now();
	time = std::chrono::duration<float, std::chrono::seconds::period>(curnt-start).count();
	
	UBO ubo{};
	ubo.vP    = glm::vec4(viewPos,1.f);
	ubo.vD    = glm::vec4(viewDir,1.f);
	ubo.lP    = glm::vec4(lightPos,1.f);
	ubo.time  = time;
	ubo.sharp = sharpness;

	if(_framBufRsz||_isInitialized){
		ubo.res=vec2(_swapChnExtent.width,_swapChnExtent.height);
	}

	void* data;
	vkMapMemory(_device,_unifBufsMems[crntImg],0,sizeof(ubo),0,&data);
	memcpy(data,&ubo,sizeof(ubo));
	vkUnmapMemory(_device,_unifBufsMems[crntImg]);

// 	std::cout<<"\n\n\033[4;32mx = "<<viewPos.x<<"| ax ="<<viewAng.x<<"| Lx ="<<lightPos.x;
// 	std::cout<<"\ny = "<<viewPos.y<<"| ay ="<<viewAng.y<<"| Ly ="<<lightPos.y;
// 	std::cout<<"\nz = "<<viewPos.z<<"| az ="<<viewAng.z<<"| Lz ="<<lightPos.z<<"\n";
// 	std::cout<<"\nresx = "<<ubo.res.x<<"| resy ="<<ubo.res.y<<"\n";
// 	std::cout<<"\nswpx = "<<_swapChnExtent.width<<"| swpy ="<<_swapChnExtent.height<<"\n";
// 	std::cout<<"\nobjYaw = "<<objAng.first<<"\033[0;37\n";
// 	std::cout<<"\nsharpness = "<<sharpness<<"\033[0;37m";
}
