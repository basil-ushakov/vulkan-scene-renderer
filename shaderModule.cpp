VkShaderModule VKHQ::crt_shaderModl(const std::vector<char>& bin){
    logF({.f=NLNE,.c=1,.b=3,.h=CYNC,
          .s="Creating ShaderModule:",.hs=YLWC,
          .f2=NLNE|DTAB});

	VkShaderModuleCreateInfo shaderModlInfo{
		.sType=VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize=bin.size(),
		.pCode=reinterpret_cast<const uint32_t*>(bin.data()),
	};

	VkShaderModule shaderModl;
	if(vkCreateShaderModule(_device,&shaderModlInfo,nullptr,&shaderModl)!=VK_SUCCESS){
		wrtSysMsg(RERR,"Failed create ShaderModule!");
	}

    logF({.f=NLNE|MDTB,.c=1,.b=3,.h=CYNC,
          .s="Created ShaderModule!",.hs=YLWC});
	return shaderModl;
}

std::vector<char> VKHQ::read_f(const std::string& fName,std::ios::openmode readMode){
    logF({.f=NLNE,.c=1,.b=3,.h=REDC,
          .s="Reading file:",.hs=ORGC,
		  .s2=fName,.bs2=3,.hs2=ORGC,
          .f2=NLNE|DTAB});

	string readModeName="UNDEFINED";
	if(readMode==std::ios::binary){readModeName="BINARY";}
	else if(readMode==std::ios::in){readModeName="STRING_IN";}
	
    logF({.f=NLNE,.c=1,
          .s="File's opening in ReadMode",.hs=ORGC,
		  .s2=readModeName,.bs2=2,.hs2=REDC});
	
	std::ifstream file(fName,std::ios::ate|readMode);
	if(!file.is_open()){
		wrtSysMsg(RERR,"Failed to read a File!");
	}

    logF({.f=NLNE,.c=1,
          .s="File readed Successfully!",.hs=ORGC});
	
	size_t fSize = (size_t)file.tellg();
	std::vector<char> buffer(fSize);
	
	file.seekg(0);
	file.read(buffer.data(), fSize);
	file.close();

    logF({.f=NLNE|MDTB,.c=1,.b=3,.h=REDC,
          .s="Readed file:",.hs=ORGC,
		  .s2=fName,.bs2=3,.hs2=ORGC});
	
	return buffer;
}
