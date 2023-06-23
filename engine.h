#pragma once

class VKHQ{

public:
    std::string MODEL_PATH    = "models/cube.obj";
    std::string TEXTURE_PATH  = "textures/viking_room.png";
    std::string TEXTURE_PATH2 = "textures/grass_seamless.png";
    std::string TEXTURE_PATH3 = "textures/stone.png";

    u32 timeout{0};

    volatile std::sig_atomic_t _sysSig;
    u32                        vertCount;
    bool                       boolValLayers  {false};
    std::set<u32>              win_flags;
    bool                       _isInitialized {false};
    bool                       _cleanuped     {false};
    u64                        _framLim       {UINT64_MAX};
    u32                        _framInFlt     {3};
    u32                        _crntFram      {0};
    u32                        _framCount     {0};
    bool                       _framBufRsz    {false};
    float                      time           {0};
    
    float                      camSpeed{.05f};
    float                      viewSpeed{.05f};
    glm::vec3                  viewPos{1.623f,1.589f,1.6f};
    glm::vec3                  viewDir{0.f};
    glm::vec3                  viewAng{225.f,-36.f,0.f};
    glm::vec3                  viewAbs{0.f};
    glm::vec3                  lightPos{0.f};
    std::pair<float,glm::vec3> objAng{0.f,glm::vec3(0.f)};
    float                      sharpness{0.f};

    VkExtent2D _windowExtent{900,900};

    struct SDL_Window* _window{ nullptr };
    
    const std::vector<const char*> validLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> deviceEXTs = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    
    VKHQ(bool manualInit=0,bool validLayersEnable=0){
    constr(manualInit,validLayersEnable);
    }
    /*
    * Launch Engine Init
    * ------------------------------------------------------
    * Calls private method "contr";
    * 
    */
    
    ~VKHQ(){
    deconstr(_sysSig);
    }
    /*
    * Stop Engine
    * ------------------------------------------------------
    * Calls private method "decontr";
    * 
    */

    void init();
    /*
    * Init Engine
    * ------------------------------------------------------
    * Initializing the Engine by calling all Init's
    * methods in a private modifier;
    * 
    */

    void run();
    /*
    * Running MainLoop
    * ------------------------------------------------------
    * A basic infinite Loop that calls the "draw" method every time
    * and waits for [SDL_Event] EventQuit to stop;
    * 
    * Also calls private method "draw";
    * 
    */
    
	inline bool cleanup();
    /*
    * CleanUping
    * ------------------------------------------------------
    * Clears, in the right order, all Objects to be CleanedUped;
    * 
    * Also calls private method "destrDbgUtilsMsngrEXT";
    * 
    */
    

private:


//======================
//Private Objects:
//======================    
    
    VkInstance _instance;
    VkDebugUtilsMessengerEXT _dbgUtilMsngr;
    VkPhysicalDevice _physGPU = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties _physGPUDet;
    uint32_t _deviceCount;
    QFamInds _qInds;
    VkDevice _device;
    VkSurfaceKHR _surface;
    VkQueue _graphxQueue;
    VkQueue _presentQueue;
    VkQueue _transfQueue;

    VkSwapchainKHR _swapChn;
    std::vector<VkImage> _swapChnImgs;
    VkFormat _swapChnFormat;
    VkExtent2D _swapChnExtent;
    std::vector<VkImageView> _swapChnImgViews;
    std::vector<VkFramebuffer> _swapChnFramBufs;
    
    VkRenderPass _rndrPass;
    VkPipelineLayout _graphxPlineLayt;
    VkPipeline _graphxPline;
    
    VkPipelineLayout _compPlineLayt;
    VkPipeline _compPline;

    VkDescriptorPool _descrPool;
    std::vector<VkDescriptorSet> _descrSets;
    VkDescriptorSetLayout _descrSetLayt;
    std::vector<VkBuffer> _unifBufs;
    std::vector<VkDeviceMemory> _unifBufsMems;
    std::vector<VkBuffer> _unifBufs2;
    std::vector<VkDeviceMemory> _unifBufsMems2;
    
    VkDescriptorPool _cDescrPool;
    VkDescriptorSet _cDescrSet;
    VkDescriptorSetLayout _cDescrSetLayt;
    VkBuffer _cUnifBuf;
    VkDeviceMemory _cUnifBufMem;
    VkBuffer _cOutUnifBuf;
    VkDeviceMemory _cOutUnifBufMem;

    VkCommandPool _cmdPool;
    std::vector<VkCommandBuffer> _cmdBuf;

    VkSampleCountFlagBits msaaSmpls{VK_SAMPLE_COUNT_1_BIT};
    VkImage _colrBuf;
    VkDeviceMemory _colrBufMem;
    VkImageView _colrView;

    VkImage _depthBuf;
    VkDeviceMemory _depthBufMem;
    VkImageView _depthView;

    std::vector<objectUnit> _objs;
    std::vector<Vertex> verts;
    std::vector<uint32_t> inds;

    uint32_t mipLvl;
    VkImage _texBuf;
    VkDeviceMemory _texBufMem;
    VkSampler _texSmplr;
    VkImageView _texImgView;

    std::vector<VkSemaphore> _avbleImgSemphr;
    std::vector<VkSemaphore> _finishedImgSemphr;
    std::vector<VkFence> _inFltFens;


//======================
//De/Constructor PrivateMethods:
//======================   
    
    void constr(bool manualInit,bool validLayersEnable);
    /*
    * Constructor
    * -----------------------------------------------------
    * Launching EngineInitializing sequence [Constructor];
    * 
    */ 

    static void deconstr(int signal);
    /*
    * Deconstructor
    * -----------------------------------------------------
    * Stop Engine Work [Deconstructor];
    * 
    */ 
    
    
//======================
//Init PrivateMethods:
//======================   
    
    void crt_window();
    /*
    * Creating Window
    * -----------------------------------------------------
    * There are declarating WindowFlags (How the window should work and/or showen):
    * Border name;Window 2D position over workspace, Width and Heighton start,
    * [Fullscreen,Resizable,Borderless] or not;
    * 
    * An [SDL_Window*] object is also created, which also points to the created window;
    * 
    */

    void init_vulkan();
    /*
    * Initializing instance -- General vulkan context
    * ------------------------------------------------------
    * Declarating app Info(Engine and App name,version;Api dependency version);
    * 
    * Creating [VkInstance] object -- main context of Vulkan 
    * and link to Drivers; 
    * 
    * Available Populating** for Validation Layers*;
    * 
    * Enabling (With creating [VkInstance] object) InstanceExtensions;
    * 
    */
    void init_vulkanCPP();
    
            std::vector<const char*> get_reqEXTs();
            /*
            * Getting Requirement Extensions
            * ------------------------------------------------------
            * Finding extensions such window surface,debug utils support;
            * 
            */
            
            void pop_dbgMsngr(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
            /*
            * **Populating DebugMessenger
            * ------------------------------------------------------
            * First fill of DebugMessenger structure fields of;
            * 
            */
            void pop_dbgMsngr2(vk::DebugUtilsMessengerCreateInfoEXT& createInfo);
    
            bool chk_validLayers(const std::vector<const char*> validationLayers);
            /*
            * Checking ValidationLayers support
            * ------------------------------------------------------
            * Checking support of making DebugMessenger;
            * 
            */

    void set_dbgMsngr();
    /*
    * *Setuping DebugMessenger for work of Validation Layers
    * ------------------------------------------------------
    * Validation Layers needed to debug 
    * Vulkan runtime warnings,errors,dependencies like responsibility for destroy or
    * right filling in information when using an object;
    * 
    * DebugCallback is invoked here;
    * 
    */
    
            VkResult crt_dbgMsngr(VkInstance instance,
                                  const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                  const VkAllocationCallbacks* pAllocator,
                                  VkDebugUtilsMessengerEXT* pDebugMessenger);
             /*
             * Creating DebugMessenger
             * ------------------------------------------------------
             * Creating [VkDebugUtilsMessengerEXT] and check 
             * the representation of this extension;
             * 
             */

    void crt_surface();
    /*
    * Creating Surface
    * ------------------------------------------------------
    * Creating [VkSurfaceKHR] object -- "Abstraction" 
    * of linked from Vulkan engine (rendered) result to Window 
    * through WSi(Window System Integration);
    * 
    */
    
    void get_physDevice();
    /*
    * Creating PhysicalDevice/s
    * ------------------------------------------------------
    * Creating [VkPhysicalDevice] object/s that query you Hardware (GPU/s)
    * Details: Feautures, Limits, Size of Memory and etc.;
    * 
    */
                
                bool chk_deviceSupport(VkPhysicalDevice device);
                /*
                * Checking PhysicalDevice support
                * ------------------------------------------------------
                * Verifies the ability to create a [vkPhysicalDevice] object 
                * and its support for the use of functions that depend on it;
                * 
                * Calls all subsequent methods in that order;
                * 
                */
                
                        QFamInds get_qFams(VkPhysicalDevice device,std::string callSite);
                        /*
                        * Finding QueueFamilies support
                        * ------------------------------------------------------
                        * Shows which family is supported by the device;
                        * 
                        * Could be called in other places;
                        * 
                        */

                        bool chk_deviceEXTsSupport(VkPhysicalDevice device);
                        /*
                        * Checking Extension support for PhysicalDevice
                        * ------------------------------------------------------
                        * Enumerates DeviceExtension properties and clear them
                        * for checking;
                        * 
                        */
                
                        SwapChnSupportDet chk_swapChnSupport(VkPhysicalDevice device);
                        /*
                        * Checking SwapChain support
                        * ------------------------------------------------------
                        * Checking PhysicalDevice support for SurfaceFormats
                        * and PresentModes for SwapChain FrameImages;
                        * 
                        */
                
    void crt_device();
    /*
    * Creating LogicalDevice
    * ------------------------------------------------------
    * Creating a [VkDevice] handler object that allows
    * other methods to perform its functions;
    * 
    * The "findQFamilies" method is called here;
    * 
    * Creating a [VkQueue] objects to query the order 
    * in which different (Cmd) commands are executed;
    * 
    * Enabling (With creating [VkDevice] object) 
    * DeviceExtensions: VkSwapchainKHR;
    * 
    */

    void crt_swapChn();
    /*
    * Creating SwapChain
    * ------------------------------------------------------
    * Creating [VkSwapchainKHR] object -- queue of FrameImages 
    * that should be displayed on the Screen/Window;
    * 
    * The "chkSupportSwapChn" method is called here;
    * 
    * Calls all subsequent methods in that order;
    * 
    */
    
                VkSurfaceFormatKHR get_SwapChnFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
                /*
                * Getting supported Formats
                * ------------------------------------------------------
                * Checks availability of SRGB surface format and color space;
                * 
                */
                
    
                VkPresentModeKHR get_SwapChnPrsntMode(const std::vector<VkPresentModeKHR>& availablePrsntModes);
                /*
                * Getting supported PresentModes
                * ------------------------------------------------------
                * Checking and getting PresentationMode (Type of VSYNC or its absence);
                * 
                */
    
                VkExtent2D get_SwapChnExtent(const VkSurfaceCapabilitiesKHR& capblites);
                /*
                * Getting sizes of SwapChain frames
                * ------------------------------------------------------
                * We also enter the Resolution of the ImageFrame there
                * and check if this go beyond the limits of UINT32 type;
                * 
                */
                
                        void rcrt_swapchn();
                        /*
                        * ReCreating SwapChain
                        * ------------------------------------------------------
                        * Repeating a sequence of creating SwapChain;
                        * 
                        */
    
    void crt_imgViews();
    /*
    * Create ImageViews
    * ------------------------------------------------------
    * Creating [VkImageView] object/s that contains details 
    * of Access and Properties about the [VkImage] object/s;
    * 
    */
    
                VkImageView crt_imgView(VKHQ_imgViewI info);
                /*
                * Create ImageView
                * ------------------------------------------------------
                * Create one [VkImageView] object;
                * 
                */
    
    void crt_rndrPass();
    /*
    * Create RenderPass (with SubPass)
    * ------------------------------------------------------
    * Creating [VkRenderPass] object handler that
    * describes scope, dependencies and how data should flow;
    * 
    * Subpass -- phase in RenderPass that read and writes Subset of the Attachments;
    * 
    */
    
    void crt_descr();
    /*
    * Create Descriptors
    * ------------------------------------------------------
    * Create [VkDescriptorSetLayout] object, that contains 
    * bindings to Descriptors and layout of them;
    * 
    */
    
    void crt_graphxPline(VKHQ_gplineI info);
    /*
    * Create GraphicalPipeline
    * ------------------------------------------------------
    * Creating a Call to the Sequence [VkPipeline] object handler
    * of FixedStages of GraphicalRenderer that 
    * describes how to draw scene;
    * 
    * Through each call to the GraphicalPipeline, Shaders are 
    * only individual (non similiar) shader type; 
    * 
    * Also reading ShaderBinary through "readF" method and 
    * pass this Data through "crt_shaderModl" method to create ShaderModule;
    * 
    */

            static std::vector<char> read_f(const std::string& fName,std::ios::openmode readMode);
            /*
            * Read File
            * ------------------------------------------------------
            * Reading file in 2 openmodes: String In , Binary;
            * 
            */
            
            VkShaderModule crt_shaderModl(const std::vector<char>& bin);
            /*
            * Create ShaderModule
            * ------------------------------------------------------
            * Creating [VkShaderModule] object -- container of 
            * SpirV ShaderBinary;
            * 
            */
    
    // void crt_cmdPool(VkCommandPoolCreateFlags flag,uint32_t qFam,VkCommandPool* cmdPool);
    void crt_cmdPool(VKHQ_cmdpoolcrtI info);
    /*
    * Create CommandPool
    * ------------------------------------------------------
    * Creating [VkCommandPool] object, that contains 
    * CommandBuffer/s and data for them;
    * 
    * Each CommandPool should be one per all threads because 
    * is multithread-asynchronizied;
    * 
    */
    
    void crt_colrBuf();
    /*
    * Create ColorBuffer
    * ------------------------------------------------------
    * Creating;
    * 
    */
    
            VkSampleCountFlagBits get_maxSmpl();
            /*
            * Get MaxSample
            * ------------------------------------------------------
            * Getting Maximal Sample value;
            * 
            */
    
    void crt_depthBuf();
    /*
    * Create DepthBuffer
    * ------------------------------------------------------
    * Creating [VkImage] object, that contain ImageDepth attachment;
    * 
    */

            VkFormat get_depthFormat();
            /*
            * Get DepthBufferFormat
            * ------------------------------------------------------
            * Finding [VkFormat] for DepthBuffer;
            * 
            */

            VkFormat get_supprtFormat(const std::vector<VkFormat>& formats,
                                      VkImageTiling tiling,VkFormatFeatureFlags flags);
            /*
            * Get SupportedFormat
            * ------------------------------------------------------
            * Finding supported [VkFormat] from vector;
            * 
            */
            
            bool chk_stncl(VkFormat format);
            /*
            * Check StencilSupport
            * ------------------------------------------------------
            * Checking support of Stencil for DepthBufferFormat;
            * 
            */
        
    void crt_framBuf();
    /*
    * Create Framebuffer
    * ------------------------------------------------------
    * Creating [VkFramebuffer] object -- binded to each other elements 
    * container of memory attachments (like ColorImageBuffer,DepthImageBuffer and etc.)
    * with RenderPass;
    * 
    */
    
    void load_model(std::string& mdlPath,objectUnit& obj);
    /*
    * Load Model
    * ------------------------------------------------------
    * Loads external model;
    * 
    */
    
            void crt_vertBuf();
            /*
            * Create VertexBuffer 
            * ------------------------------------------------------
            * Creating and allocating memory for array-object
            * that contains data of all used Vertex;
            * 
            * Also calls method "get_memType" to find needed MemoryType;
            * 
            */

                    void crt_buf(VKHQ_bufcrtI& info);
                    /*
                    * Create Buffer
                    * ------------------------------------------------------
                    * Create [VkBuffer] object with configuration of usage, share access,
                    * properties; that contains array of data;
                    * 
                    * Binds [VkBuffer] object to [VkDeviceMemory]; 
                    * 
                    */
                    
                    uint32_t get_memType(uint32_t filter, VkMemoryPropertyFlags det);
                    /*
                    * Finding MemoryType
                    * ------------------------------------------------------
                    * Finding and getting for any buffer equal needed MemoryType;
                    * 
                    */
            
                    void cpy_buf(VkBuffer srcBuf,VkBuffer dstBuf,VkDeviceSize size);
                    /*
                    * Copy Buffer
                    * ------------------------------------------------------
                    * Copies SourceBuffer to DestinationBuffer;
                    * 
                    */

    void crt_indBuf();
    /*
    * Create IndexBuffer 
    * ------------------------------------------------------
    * Creating IndexBuffer, that contains indexes to vertex
    * (Its needed to simply minimize amount of vertexes and memory size);
    * 
    */

    void crt_unifBuf();
    /*
    * Create UniformBuffer
    * ------------------------------------------------------
    * Create type of descriptor,throw which data transfer to shaders;
    * 
    */

                void upd_unifBuf(uint32_t crntImg);
                /*
                * Update UniformBuffer
                * ------------------------------------------------------
                * Update data of UniformBuffer;
                * 
                */

    void crt_tex(VKHQ_texcrtI& info);
    // void crt_tex();
    /*
    * Create Texture
    * ------------------------------------------------------
    * Loading & creating Texture [VkImage] through 
    * all subsequent methods in that order;
    *
    */

                void crt_img(VKHQ_imgcrtI& info);
                /*
                * Create Image
                * ------------------------------------------------------
                * Creating [VkImage] handler & [VkDeviceMemory] objects,
                * that contains bindings of created Image to ImageMemory;
                *  
                */
                
                void chg_imgLayt(VKHQ_imgLaytcrtI info);
                /*
                * Change ImageLayout
                * ------------------------------------------------------
                * Changing Layout of Image;
                *  
                */
        
                void cpy_bufToImg(VKHQ_buftoimgI info);
                /*
                * Copy Buffer To Image
                * ------------------------------------------------------
                * Transfering [VkBuffer] object data to [VkImage] object;
                *  
                */
        
                void crt_texImgView();
                /*
                * Create TextureImageView
                * ------------------------------------------------------
                * Creating single [VkImageView] object handler for Texture;
                *  
                */
                    
                void crt_texSmplr(VKHQ_texSmplrI info);
                /*
                * Create TextureSampler
                * ------------------------------------------------------
                * Creating [VkSampler] object handler descriptor,
                * that provides access of image to shader;
                *  
                */

                // void crt_mipmaps(VkImage& img,VkFormat format,
                //                 int32_t w,int32_t h,uint32_t mipLvl);
                void crt_mipmaps(VKHQ_mipmapcrtI info);
                /*
                * Create MipMaps
                * ------------------------------------------------------
                * Creating ;
                *  
                */
                
    void crt_descrPool();
    /*
    * Create DescriptorPool
    * ------------------------------------------------------
    * Create [VkDescriptorPool] object, that contains 
    * data for all descriptors which alocate in it;
    * 
    */
    
    void crt_descrSets();
    /*
    * Create DescritptorSets
    * ------------------------------------------------------
    * Create [VkDescriptorSet] objects, that groups 
    * all descriptors in sets;
    * 
    */
            
    void crt_cmdBuf();
    /*
    * Create CommandBuffer
    * ------------------------------------------------------
    * Creating [VkCommandBuffer] object, that contains and handles
    * Commands to represent into Queue/s;
    * 
    */
    
        void use_cmdBuf(VkCommandBuffer cmdBuf,uint32_t imgInd);
        /*
        * Use CommandBuffer to Draw
        * ------------------------------------------------------
        * Draws scene;  
        * 
        */
    
        VkCommandBuffer use_singleCmd();
        /*
        * Start using SingleCommand
        * ------------------------------------------------------
        * Creating one [VkCommandBuffer] object handler to One-Usage;
        * 
        * Can used in other methods;
        *  
        */
        
        void end_singleCmd(VkCommandBuffer cmdBuf,VkQueue& queue);
        /*
        * End using SingleCommand
        * ------------------------------------------------------
        * Submiting existing One-Usage CommandBuffer to Queue and 
        * Freeing him from CommandPool where he was allocated previosly;
        * 
        * Can used in other methods;
        *  
        */
        
    void crt_syncObj();
    /*
    * Create SynchronizationObjects
    * ------------------------------------------------------
    * Creating [VkSemaphore],[VkFence] objects for other 
    * methods of Engine for their synchronization;
    * 
    */    
    
    void crt_compPline(VKHQ_cplineI info);

//======================
//MainLoop PrivateMethods:
//======================  

    void draw();
    /*
    * Draw
    * ------------------------------------------------------
    * Draws scene/s by calling "useCmdBuf";
    * 
    * Also synchronized controls rendering by SynchronizationObjects;
    * 
    */
    
    
//======================
//CleanUp PrivateMethods:
//======================  
  
    inline bool cleanup_swapChn();
    /*
    * CleanUp SwapChain
    * ------------------------------------------------------
    * CleanUping SwapChain resources;
    * 
    */
    
    void destr_dbgUtilsMsngr(VkInstance instance,
                             VkDebugUtilsMessengerEXT debugMessenger,
                             const VkAllocationCallbacks* pAllocator);
    /*
    * Destruction of the DebugMessenger
    * ------------------------------------------------------
    * Destroys DebugMessenger for CleanUp;
    * 
    */    

};
