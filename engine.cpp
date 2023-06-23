#include "dpndlibs.h"
#include "aliases.h"
#include "addnlibs.h"
#include "types.h"
#include "engine.h"

#include "constructors.cpp"
#include "window.cpp"
#include "instance.cpp"
#include "validLayers.cpp"
#include "device.cpp"
#include "swapChain.cpp"
#include "cleanup.cpp"
#include "shaderModule.cpp"
#include "descriptors.cpp"
#include "graphicsPipeline.cpp"
#include "commandBuffer.cpp"
#include "multisampling.cpp"
#include "depthBuffer.cpp"
#include "images.cpp"
#include "models.cpp"
#include "vertexBuffer.cpp"
#include "render.cpp"
#include "sync.cpp"

#include "computePipeline.cpp"

void VKHQ::init(){
	logF({.f=NLNE|STDP,.c=1,.b=1,.h=REDC,
		  .s="Engine initialization Start:",.hs=REDC});

	crt_window();

	init_vulkan();
	set_dbgMsngr();

	crt_surface();

	get_physDevice();
	crt_device(); 

	crt_swapChn();
	crt_imgViews();
	crt_rndrPass();

	crt_descr();

	VKHQ_gplineI gplnInfo{
		.pline	   = &_graphxPline,
		.plineLayt = &_graphxPlineLayt,
		.fs		   = "shaders/frag.spv",
		.vs		   = "shaders/vert.spv",
		.vw		   = (float)_swapChnExtent.width,
		.vh		   = (float)_swapChnExtent.height,
		.smpls	   = VK_SAMPLE_COUNT_8_BIT,
	};crt_graphxPline(gplnInfo);
	
	VKHQ_cplineI cplnInfo{
		.pline	   = &_compPline,
		.plineLayt = &_compPlineLayt,
		.s		   = "shaders/comp.spv"
	};crt_compPline(cplnInfo);
		
	VKHQ_cmdpoolcrtI cmdpoolInfo{
		.f 	  = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.qFam = _qInds.gFam.value(),
		.cmdp = &_cmdPool,
	};crt_cmdPool(cmdpoolInfo);

	crt_colrBuf();
	crt_depthBuf();
	crt_framBuf();

	VKHQ_texcrtI texcrtInfo{
		.paths = {
			TEXTURE_PATH3,
			TEXTURE_PATH2,
			TEXTURE_PATH,
		},
		.path  = TEXTURE_PATH,
	};/* texcrtInfo addition */{
	texcrtInfo.bufcrtI.usgF = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	texcrtInfo.bufcrtI.shrM = VK_SHARING_MODE_EXCLUSIVE;
	texcrtInfo.bufcrtI.memF = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
							| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

	texcrtInfo.imgCrtI.img  = &_texBuf;
	texcrtInfo.imgCrtI.dmem = &_texBufMem;

	texcrtInfo.imgLaytCrtI.frmt    = VK_FORMAT_R8G8B8A8_SRGB;
	texcrtInfo.imgLaytCrtI.oldLayt = VK_IMAGE_LAYOUT_UNDEFINED;
	texcrtInfo.imgLaytCrtI.newLayt = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

	}crt_tex(texcrtInfo);

	load_model(MODEL_PATH,_objs[0]);
	crt_vertBuf();
	crt_indBuf();

	crt_unifBuf();
	crt_descrPool();
	crt_descrSets();

	crt_cmdBuf();
	crt_syncObj();

	_isInitialized = true;

	logF({.f=NLNE|MLTB|STDP,.c=10,.b=1,.h=REDC,
		  .s="Engine initialization End!",.hs=REDC});

	/* computePipeline example */{
	VkCommandBuffer cmd = use_singleCmd();
	
		vkCmdBindPipeline(cmd,VK_PIPELINE_BIND_POINT_COMPUTE,_compPline);
		VkMemoryBarrier bar{
			.sType =   VK_STRUCTURE_TYPE_MEMORY_BARRIER,
			.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT,
			.dstAccessMask =  VK_ACCESS_SHADER_READ_BIT
		};
		vkCmdPipelineBarrier(cmd,
							 VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,0,
							 1,&bar,
							 0,nullptr,
							 0,nullptr);
		vkCmdBindDescriptorSets(cmd,VK_PIPELINE_BIND_POINT_COMPUTE,_compPlineLayt,0,1,&_cDescrSet,0,nullptr);
		vkCmdDispatch(cmd,1,1,1);
		
	end_singleCmd(cmd,_graphxQueue); 
	
	CUBOOut cubo{};
	
	void* data;
	vkMapMemory(_device,_cOutUnifBufMem,0,sizeof(CUBOOut),0,&data);
	memcpy(&cubo,data,sizeof(CUBOOut));
	vkUnmapMemory(_device,_cOutUnifBufMem);
	
	logF({.f=NLNE|PVAL|VFLT,
		  .s="CUBO Y: ",.hs=CYNC,
		  .v=cubo.outres.y,.hv=YLWC});
	
	logF({.f=NLNE|PVAL|VFLT,
		  .s="Max res: ",.hs=CYNC,
		  .v=cstVal(_physGPUDet.limits.maxImageArrayLayers),.hv=YLWC});
	}
}

//NOTES:
//----------------------------------------------
//LogF Create new union type for Values     [X.]
//
//Check and Update Queues!     				[X.]
//

void VKHQ::run(){
	SDL_Event GnrlEvent;

	bool bQuit = false;

  	while(!bQuit)
	{
	draw();

		while(SDL_PollEvent(&GnrlEvent)!=0){

			auto ev_type 	 = GnrlEvent.type;
			auto ev_window   = GnrlEvent.window.event;
			auto ev_key_scan = GnrlEvent.key.keysym.scancode;
			auto ev_key_sym  = GnrlEvent.key.keysym.sym;

			if(ev_type == SDL_QUIT){
				bQuit = true;

				logF({.f=NLNE|CNTR|DTAB,.c=1,.b=10,.h=REDC,.m=0,.bg=0,
					  .s="SDL Terminate Event!",.bs=0,.hs=LIMC});
				logFFlag(MDTB);
			}
			if(ev_window == SDL_WINDOWEVENT_SIZE_CHANGED){
				logF({.f=NLNE|CNTR|DTAB,.c=1,.b=10,.h=REDC,.m=0,.bg=0,
					  .s="SDL Resize Event!",.bs=0,.hs=LIMC});
				logFFlag(MDTB);
			}

			switch(ev_key_scan){
				case SDL_SCANCODE_I:
					viewAng+=glm::vec3(0.0f,viewSpeed*60,0.0f);
					break;
				case SDL_SCANCODE_K:
					viewAng+=glm::vec3(0.0f,-viewSpeed*60,0.0f);
					break;
				case SDL_SCANCODE_J:
					viewAng+=glm::vec3(viewSpeed*60,0.0f,0.f);
					break;
				case SDL_SCANCODE_L:
					viewAng+=glm::vec3(-viewSpeed*60,0.0f,.0f);
					break;

				case SDL_SCANCODE_P:
					lightPos=glm::vec3(viewPos.x,viewPos.y,viewPos.z);
					break;

				case SDL_SCANCODE_UP:
					lightPos+=glm::vec3(.5f,0.f,0.f);
					break;
				case SDL_SCANCODE_DOWN:
					lightPos+=glm::vec3(-.5f,0.f,0.f);
					break;
				case SDL_SCANCODE_LEFT:
					lightPos+=glm::vec3(0.f,.5f,0.f);
					break;
				case SDL_SCANCODE_RIGHT:
					lightPos+=glm::vec3(0.f,-.5f,.0f);
					break;

				case SDL_SCANCODE_H:
					lightPos+=glm::vec3(0.f,0.f,.25f);
					break;
				case SDL_SCANCODE_N:
					lightPos+=glm::vec3(0.f,0.f,-.25f);
					break;

				case SDL_SCANCODE_Y:
					objAng.first+=1.f;
					break;
				case SDL_SCANCODE_U:
					objAng.first-=1.f;
					break;

				case SDL_SCANCODE_F:
					viewPos+=glm::vec3(camSpeed*sin(glm::radians(viewAng.x)),cos(glm::radians(viewAng.x))*-camSpeed,0.f);
					break;
				case SDL_SCANCODE_S:
					viewPos+=glm::vec3(-camSpeed*sin(glm::radians(viewAng.x)),cos(glm::radians(viewAng.x))*camSpeed,0.f);
					break;
				case SDL_SCANCODE_E:
					viewPos+=glm::vec3(camSpeed*cos(glm::radians(viewAng.x)),sin(glm::radians(viewAng.x))*camSpeed,0.f);
					break;
				case SDL_SCANCODE_D:
					viewPos+=glm::vec3(-camSpeed*cos(glm::radians(viewAng.x)),sin(glm::radians(viewAng.x))*-camSpeed,0.f);
					break;
			}
			
			switch(ev_key_sym){
				case SDLK_EQUALS:
					sharpness+=1.f;
					break;
				case SDLK_MINUS:
					sharpness-=1.f;
					break;

				case SDLK_LSHIFT:
					camSpeed+=.05f;
					logF({.f=NLNE|PVAL|VFLT,
						.s="CamSpeed: ",.hs=CYNC,
						.v=camSpeed,.hv=YLWC});
					break;
				case SDLK_LCTRL:
					camSpeed-=.05f;
					logF({.f=NLNE|PVAL|VFLT,
						.s="CamSpeed: ",.hs=CYNC,
						.v=camSpeed,.hv=YLWC});
					break;

				case SDLK_SPACE:
					viewPos+=glm::vec3(0.f,0.f,camSpeed);
					break;
				case SDLK_a:
					viewPos+=glm::vec3(0.f,0.f,-camSpeed);
					break;
			}
		}
	}

    logF({.f=NLNE|PVAL,.c=1,
          .s="Average FPS:",.hs=WHTC,
          .v=cstVal(1000/t.avg()),.bv=2,.hv=WHTC});
    logF({.f=NLNE|PVAL,.c=1,
          .s="Max FPS:",.hs=WHTC,
          .v=cstVal(1000/t.min()),.bv=2,.hv=WHTC});
    logF({.f=NLNE|PVAL,.c=1,
          .s="Min FPS:",.hs=WHTC,
          .v=cstVal(1000/t.max()),.bv=2,.hv=WHTC});

	vkDeviceWaitIdle(_device);
}
