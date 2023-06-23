#pragma once

struct QFamInds {
	std::optional<uint32_t> gFam;
	std::optional<uint32_t> pFam;
	std::optional<uint32_t> cFam;
	std::optional<uint32_t> tFam;
	
	uint32_t qFamCount;

	bool ready(){
		return gFam.has_value() && gFam.has_value() && tFam.has_value() && cFam.has_value();
	}
};

struct SwapChnSupportDet{
    VkSurfaceCapabilitiesKHR capblites;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> prsntModes;
};

struct Vertex{
	glm::vec3 pos;
	glm::vec3 colr;
	glm::vec2 texCoord;
	glm::vec3 norm;
	
	static VkVertexInputBindingDescription get_bindDscr(){
		VkVertexInputBindingDescription bindDscr{
			.binding=0,
			.stride=sizeof(Vertex),
			.inputRate=VK_VERTEX_INPUT_RATE_VERTEX,
		};
		return bindDscr;
	}

	static std::array<VkVertexInputAttributeDescription,4> get_attrDscr(){
		std::array<VkVertexInputAttributeDescription,4> attrDscr{};
			attrDscr[0].binding=0;
			attrDscr[0].location=0;
			attrDscr[0].format=VK_FORMAT_R32G32B32_SFLOAT;
			attrDscr[0].offset=offsetof(Vertex, pos);

			attrDscr[1].binding=0;
			attrDscr[1].location=1;
			attrDscr[1].format=VK_FORMAT_R32G32B32_SFLOAT;
			attrDscr[1].offset=offsetof(Vertex, colr);
			
			attrDscr[2].binding=0;
			attrDscr[2].location=2;
			attrDscr[2].format=VK_FORMAT_R32G32_SFLOAT;
			attrDscr[2].offset=offsetof(Vertex, texCoord);
			
			attrDscr[3].binding=0;
			attrDscr[3].location=3;
			attrDscr[3].format=VK_FORMAT_R32G32B32_SFLOAT;
			attrDscr[3].offset=offsetof(Vertex, norm);
			
		return attrDscr;
	}
	
	bool operator==(const Vertex& rV)const{
		return pos==rV.pos&&colr==rV.colr&&texCoord==rV.texCoord&&norm==rV.norm;
	}
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vert) const {
			size_t p = hash<glm::vec3>()(vert.pos);
			size_t c = hash<glm::vec3>()(vert.colr);
			size_t t = hash<glm::vec2>()(vert.texCoord);
			size_t n = hash<glm::vec3>()(vert.norm);
 			return ((((p^(c<<1))>>1)^t)<<1)^n;
        }
    };
}

VkDevice* _deviceP=VK_NULL_HANDLE;
class objectUnit{
public:
	string pathSrcMdl{""};
	string pathTex{""};

    VkBuffer vertBuf;
    VkDeviceMemory vertBufMem;
	s vertSize{0};

    VkBuffer indBuf;
    VkDeviceMemory indBufMem;
	s indSize{0};
	
	VkDevice* deviceP = _deviceP;

	void erase(){
		logF({.f=NLNE,.c=1,
			.s="Destroy IndexBuffer!",.hs=WHTC});
		vkDestroyBuffer(*deviceP,indBuf,nullptr);
		logF({.f=NLNE,.c=1,
			.s="Free IndexBufferMemory!",.hs=WHTC});
		vkFreeMemory(*deviceP,indBufMem,nullptr);
		
		logF({.f=NLNE,.c=1,
			.s="Destroy VertexBuffer!",.hs=WHTC});
		vkDestroyBuffer(*deviceP,vertBuf,nullptr);
		logF({.f=NLNE,.c=1,
			.s="Free VertexBufferMemory!",.hs=WHTC});
		vkFreeMemory(*deviceP,vertBufMem,nullptr);
	}
};

struct MVP {
	alignas(16) glm::mat4 m;
	alignas(16) glm::mat4 v;
	alignas(16) glm::mat4 p;
};

struct UBO{
	alignas(16) glm::vec4 vP;
	alignas(16) glm::vec4 vD;
	alignas(16) glm::vec4 lP;
	alignas(4) float time;
	alignas(8) glm::uvec2 res;
	alignas(4) float sharp;
};

struct CUBO{
	alignas(4) float time;
	alignas(8) glm::uvec2 res;
};

struct CUBOOut{
	alignas(8) glm::vec2 outres;
};

//======================
//Structures for Creations:
//======================

typedef enum{
	ALL =0,A =0,
	FRAG=1,F =1,
	VERT=2,V =2,
	COMP=3,C =3,
	GEOM=4,G =4,
	TESC=5,TC=5,
	TESE=6,TE=6,
	
	NAS =0b11111111,
} VKHQ_ShaderStg;

VkPipeline		 VKHQ_plineNull	    = VK_NULL_HANDLE;
VkPipelineLayout VKHQ_plineLaytNull = VK_NULL_HANDLE;
VkImage			 VKHQ_imgNull       = VK_NULL_HANDLE;
VkDeviceMemory   VKHQ_dmemNull 	    = VK_NULL_HANDLE;
VkBuffer         VKHQ_bufNull		= VK_NULL_HANDLE;
VkImageLayout    VKHQ_imgLaytNull	= VK_IMAGE_LAYOUT_UNDEFINED;
VkCommandPool 	 VKHQ_cmdpoolNull	= VK_NULL_HANDLE;

enum{
	VKHQ_smpl_1BIT = 0x00000001, VKHQ_smpl_16BIT = 0x00000010,
	VKHQ_smpl_2BIT = 0x00000002, VKHQ_smpl_32BIT = 0x00000020,
	VKHQ_smpl_4BIT = 0x00000004, VKHQ_smpl_64BIT = 0x00000040,
	VKHQ_smpl_8BIT = 0x00000008,
} VKHQ_smplBIT;

const VkLogicOp VK_LOGIC_OP_FALSE = static_cast<VkLogicOp>(127);

struct VKHQ_gplineI{
	VkPipeline* 	  pline     = &VKHQ_plineNull;		// Pipeline
	VkPipelineLayout* plineLayt = &VKHQ_plineLaytNull;	// Pipeline Layout
	
	u32 indsS[7]{0};	// Indexes of used stages
	string s   {""};	// General Shader path
	string fs  {""};	// Fragment Shader path
	string vs  {""};	// Vertex Shader path
	string cs  {""};	// Compute Shader path
	string gc  {""};	// Geometry Shader path
	string tsc {""};	// Tessellation Control Shader path
	string tse {""};	// Tessellation Evalution Shader path
	
	f32 vw {0.f};	// Viewport width
	f32 vh {0.f};	// Viewport height
	f32 vx {0.f};	// Viewport X position
	f32 vy {0.f};	// Viewport Y position
	f32 sx {0.f};	// Scissors X position
	f32 sy {0.f};	// Scissors Y position

	VkSampleCountFlagBits smpls      {VK_SAMPLE_COUNT_1_BIT};	// Sample count
	VkBool32 			  smplShd    {VK_FALSE};				// Sample Shading
	f32 				  smplShdMin {0.f};						// Sample Shading Minimum
	VkSampleMask*         smplMsk	 {nullptr};					// Sample Mask
	VkBool32			  alphaToCov {VK_FALSE};				// Alpha to Coverage
	VkBool32			  alphaToOne {VK_FALSE};				// Alpha to One
	
	VkBool32      		  blnd        {VK_FALSE};					// Color Blend
	VkBlendFactor 		  blndClrSrc  {VK_BLEND_FACTOR_SRC_COLOR};	// Source Color Blend Factor
	VkBlendFactor 		  blndClrDst  {VK_BLEND_FACTOR_DST_COLOR};	// Destination Color Blend Factor
	VkBlendOp     		  blndClrOp   {VK_BLEND_OP_MAX};			// Color Blend Operator
	VkBlendFactor 		  blndAlphSrc {VK_BLEND_FACTOR_SRC_ALPHA};	// Source Alpha Blend
	VkBlendFactor 		  blndAlphDst {VK_BLEND_FACTOR_DST_ALPHA};	// Destination Alpha Blend
	VkBlendOp             blndAlphOp  {VK_BLEND_OP_MAX};			// Alpha Blend Operator
	f32 				  blndClr[4]  {0};							// Colours for Blending
	VkColorComponentFlags blndMsk    = VK_COLOR_COMPONENT_R_BIT |
									   VK_COLOR_COMPONENT_G_BIT |
									   VK_COLOR_COMPONENT_B_BIT |
									   VK_COLOR_COMPONENT_A_BIT;	// Blend Mask

	VkPipelineDepthStencilStateCreateFlags dpthStnclF   {0};					// Depth Stencil State Create Flags
	VkBool32							   dpthTest     {VK_TRUE};				// Depth Test
	VkBool32							   dpthWrt      {VK_TRUE};				// Depth Write
	VkCompareOp							   dpthOp       {VK_COMPARE_OP_LESS};	// Depth Operator
	VkBool32							   dpthBndsTest {VK_FALSE};				// Depth Bounds Test
	VkBool32							   stnclTest    {VK_FALSE};				// Stencil Test
	VkStencilOpState					   stnclFront = {
														.failOp 	 = VK_STENCIL_OP_REPLACE,	// Fail Condition Operator
														.passOp 	 = VK_STENCIL_OP_REPLACE,	// Pass Condition Operator
														.depthFailOp = VK_STENCIL_OP_REPLACE,	// Depth Fail Condition Operator
														.compareOp   = VK_COMPARE_OP_ALWAYS,	// Compare Operator
														.compareMask = 0xff,					// Compare Mask
														.writeMask 	 = 0xff,					// Write Mask
														.reference   = 1,						// Reference
														};						// Stencil Front Operator State
	VkStencilOpState					   stnclBack  = stnclFront;				// Stencil Back Operator State
	f32									 dpthBndsMin  {0.f};					// Depth Bounds Minimum
	f32									 dpthBndsMax  {0.f};					// Depth Bounds Maximum
	
	VkPipelineColorBlendStateCreateFlags blndClrF	  {0};											// Color Blend State Create Flags
	VkLogicOp 							 lOp 	  	  = static_cast<VkLogicOp>(VK_LOGIC_OP_FALSE);	// Logic Operator
	f32 								 blndCnsts[4] {0};											// Blend Constants
	
	VkPrimitiveTopology 					tplg   {VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST};	// Primitive Topology
	VkPipelineInputAssemblyStateCreateFlags tplgF  {0};										// Input Assembly State Create Flags
	VkBool32 								prmRst {VK_FALSE};								// Primitive Reset
	
	u32 			rastF 		 {0};								// Rasterization Flags
	VkBool32 		dpthClmp	 {VK_FALSE};						// Depth Clamping
	VkBool32 		rastDscrd	 {VK_FALSE};						// Discard Rasterization
	VkPolygonMode 	plygnM		 {VK_POLYGON_MODE_FILL};			// Polygon Mode Fill
	VkCullModeFlags cullM		 {VK_CULL_MODE_BACK_BIT};			// Cull Mode Back
	VkFrontFace 	frntFs		 {VK_FRONT_FACE_COUNTER_CLOCKWISE};	// Front Face
	VkBool32 		dpthBias 	 {VK_FALSE};						// Depth Bias
	f32 			dpthBiasCnst {0.f};								// Depth Bias Constant
	f32 			dpthBiasClmp {0.f};								// Depth Bias Clamp
	f32 			dpthBiasSlpe {0.f};								// Depth Bias Slope Factor
	f32 			lnw 		 {1.f};								// Line Width
};

struct VKHQ_cplineI{
	VkPipeline*       pline     = &VKHQ_plineNull;		// Pipeline
	VkPipelineLayout* plineLayt = &VKHQ_plineLaytNull;	// Pipeline Layout

	u32 lf {0};	// Layout Creation Flags
	
	string s  {""};	// Shader Path
	u32    sf {0};	// Shader Flags
	
	u32 pf {0};	// Pipeline Flags
};

struct VKHQ_imgcrtI{
	u32 w 										    {0};	// Width
	u32 h 										    {0};	// Height
	u32 z 										    {1};	// Depth
	u32 l 										    {1};	// Mip Level
	u32 as											{1};	// Array Size
	VkSampleCountFlagBits smplC {VK_SAMPLE_COUNT_1_BIT};	// Sample Count

	VkFormat      frmt {VK_FORMAT_UNDEFINED};		// Format
	VkImageTiling tile {VK_IMAGE_TILING_OPTIMAL};	// Image Tiling

	VkImageUsageFlags 	  usgF {0};	// Image Usage Flags
	VkMemoryPropertyFlags memF {0};	// Memory Property Flags

	VkImage*        img  = &VKHQ_imgNull;	// Image
	VkDeviceMemory* dmem = &VKHQ_dmemNull;	// Device Memory
};

struct VKHQ_bufcrtI{
	VkDeviceSize 	   	  size {0};							// Buffer Size
	VkBufferUsageFlags    usgF {0};							// Buffer Usage Flags
	VkSharingMode 	      shrM {VK_SHARING_MODE_EXCLUSIVE};	// Sharing Mode
	VkMemoryPropertyFlags memF {0};							// Memory Property Flags

	VkBuffer* 		buf  = &VKHQ_bufNull;	// Buffer
	VkDeviceMemory* dmem = &VKHQ_dmemNull;	// Device Memory

	string name = "N0NAME";	// Buffer Name
};

struct VKHQ_buftoimgI{
	VkBuffer* buf = &VKHQ_bufNull;	// Buffer
	VkImage*  img = &VKHQ_imgNull;	// Image

	u32 w {0};	// Width
	u32 h {0};	// Height

	u32 am {VK_IMAGE_ASPECT_COLOR_BIT};	// Aspect Mask
	u32 ml {0};							// MipMap Level
	u32 al {0};							// Base Array Layer
	u32 lc {1};							// Layer Count
};

struct VKHQ_imgLaytcrtI{
	VkImage* img = &VKHQ_imgNull;	// Image

	u32 l  {0}; // Mip Level
	u32 lc {1};	// Layer Count

	VkFormat 	     frmt  {VK_FORMAT_UNDEFINED};	// Format
	VkImageLayout oldLayt = VKHQ_imgLaytNull;		// Old Layout
	VkImageLayout newLayt = VKHQ_imgLaytNull;		// New Layout
};

struct VKHQ_mipmapcrtI{
	VkImage*  img = &VKHQ_imgNull;			// Image
	VkFormat frmt  {VK_FORMAT_UNDEFINED};	// Format

	s32	w {0};	// Width
	s32 h {0};	// Height

	u32 am {VK_IMAGE_ASPECT_COLOR_BIT};	// Aspect Mask
	u32 ml {0};							// MipMap Level
	u32 al {0};							// Base Array Layer
	u32 lc {1};							// Layer Count
};

struct VKHQ_cmdpoolcrtI{
	VkCommandPoolCreateFlags f     {0};					// Command Pool Creation Flags
	u32				 		 qFam  {0};					// Queue Family
	VkCommandPool*			 cmdp = &VKHQ_cmdpoolNull;	// Command Pool
};

// VkImage img,uint32_t mipLvl,VkFormat format,VkImageAspectFlags aspect
struct VKHQ_imgViewI{
	VkImageViewType t {VK_IMAGE_VIEW_TYPE_2D}; // ImageView Type
	VkFormat 	    f {};					   // Format
	// VkComponentMapping c {}; 			// Component Mapping

	u32 am {VK_IMAGE_ASPECT_COLOR_BIT};	// Aspect Mask
	u32 mb {0};							// Base MipMap Level
	u32 ml {0};							// MipMap Level Count
	u32 al {0};							// Base Array Layer
	u32 lc {1};							// Layer Count

	VkImage* img = &VKHQ_imgNull;
};

struct VKHQ_texSmplrI{
	u32 ml {0}; // Mip Level
};

struct VKHQ_texcrtI{
	std::vector<string> paths {""};
	string path 			 = "";

	u32	 w {0};
	u32  h {0};
	u32 ch {4};

	u32 ml {1};

	VKHQ_bufcrtI bufcrtI{
		.usgF = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		.shrM = VK_SHARING_MODE_EXCLUSIVE,
		.memF = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			  | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	};

	VKHQ_imgcrtI imgCrtI{
		.smplC = VK_SAMPLE_COUNT_1_BIT,
		.frmt  = VK_FORMAT_R8G8B8A8_SRGB,
		.tile  = VK_IMAGE_TILING_OPTIMAL,
		.usgF  = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		.memF  = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	};

	VKHQ_imgLaytcrtI imgLaytCrtI{
        .frmt    = VK_FORMAT_R8G8B8A8_SRGB,
        .oldLayt = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayt = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
	};

	VKHQ_buftoimgI buftoimgAlgnI{};
};
