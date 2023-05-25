void VKHQ::load_model(std::string& mdlPath,objectUnit& obj){
    tinyobj::attrib_t attr;

    obj.pathSrcMdl=mdlPath;

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn,err;

    if(!tinyobj::LoadObj(&attr,&shapes,&materials,&warn,&err,mdlPath.c_str())){
        wrtSysMsg(RERR,warn+err);
    }

    std::unordered_map<Vertex,uint32_t> individVerts{};

    for(const auto& shape:shapes){
        for(const auto& ind:shape.mesh.indices){
            Vertex vert{};
            
            vert.pos={
                attr.vertices[3*ind.vertex_index+0],
                attr.vertices[3*ind.vertex_index+1],
                attr.vertices[3*ind.vertex_index+2],
            };
            
            vert.texCoord={
                attr.texcoords[2*ind.texcoord_index+0],
                1.f-attr.texcoords[2*ind.texcoord_index+1]
            };
            
            vert.colr={
                1.0f,1.0f,1.0f
            };

            vert.norm={
                attr.normals[3*ind.normal_index+0],
                attr.normals[3*ind.normal_index+1],
                attr.normals[3*ind.normal_index+2],
            };
            
            if(individVerts.count(vert)==0){
                individVerts[vert]=static_cast<uint32_t>(verts.size());
                verts.push_back(vert);
            }
            
            inds.push_back(individVerts[vert]);
            vertCount++;
        }
    }
}


