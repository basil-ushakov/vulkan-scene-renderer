void VKHQ::crt_window(){
	logF({.f=NLNE|DTAB,.c=1,.b=1,.h=WATC,.m=0,.bg=0,
		  .s="Creating Window:",.bs=0,.hs=BLUC});
    SDL_Init(SDL_INIT_VIDEO);
	logF({.f=NLNE|DTAB,.s="SDL2 initialized!",.bs=0,.hs=BLUC,});
	
	win_flags.insert(SDL_WindowFlags(SDL_WINDOW_VULKAN));
  	win_flags.insert(SDL_WindowFlags(SDL_WINDOW_RESIZABLE));
// 	win_flags.insert(SDL_WindowFlags(SDL_WINDOW_FULLSCREEN));
	
	std::string flagName;
	EnumList flagNames;
	for(uint32_t flag:win_flags){
		if(flag==SDL_WINDOW_VULKAN){flagName="VULKAN";}
		else if(flag==SDL_WINDOW_FULLSCREEN){flagName="FULLSCREEN";}
		else if(flag==SDL_WINDOW_RESIZABLE){flagName="RESIZABLE";}
		else if(flag==SDL_WINDOW_BORDERLESS){flagName="BORDERLESS";}
		flagNames.push_back(flagName);
	}

	logF({.f=NLNE|ENUM,.c=1,.b=9,.h=WATC,
		  .s="Window Flags:",.hs=CYNC,
		  .vec=flagNames,.be=3,.he=LBLC});

	_window = SDL_CreateWindow(
		"[VkApp]",//GachiVisionEngine
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		_windowExtent.width,
		_windowExtent.height,
		mergedFlags(win_flags)
	);

	logF({.f=NLNE|MDTB,.c=1,.b=1,.h=WATC,.m=0,.bg=0,
		  .s="Created Window!",.bs=0,.hs=BLUC,.ms=0});
}

void VKHQ::crt_surface(){
	logF({.f=NLNE,.c=1,.b=1,.h=BLUC,
		  .s="Creating surface:",.hs=BLUC});
    if(SDL_Vulkan_CreateSurface(_window,_instance,&_surface)!=SDL_TRUE){
		wrtSysMsg(RERR,"Failed to create SDL2 window surface!");
    }
    else{
		logF({.f=NLNE|DTAB,.s="Created SDL2 surface!",.bs=0,.hs=BLUC,});
    }
//    if (glfwCreateWindowSurface(_instance, _window, nullptr, &_surface) != VK_SUCCESS) {
//        throw std::runtime_error("Failed to create GLFW window surface!");
//    }
	logF({.f=NLNE|MDTB,.c=1,.b=1,.h=BLUC,
		  .s="Created Surface!",.hs=BLUC});
}
