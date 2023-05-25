.PHONY: test clean demo FluxHQ

STB_INCLUDE_PATH = ~/Projects/FluxHQ/libs/stb
TINYOBJ_INCLUDE_PATH = ~/Projects/FluxHQ/libs/tinyobj
VOLK_INCLUDE_PATH = ~/Projects/FluxHQ/libs/volk
IMGUI_INCLUDE_PATH = ~/Projects/FluxHQ/libs/imgui

CFLAGS = -std=c++2a -L$(VULKAN_SDK)/include -I$(VOLK_INCLUDE_PATH) -I$(TINYOBJ_INCLUDE_PATH) -I$(IMGUI_INCLUDE_PATH)
LDFLAGS = -lSDL2main -lSDL2 -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -lm

demo: FluxHQ
	./$<

FluxHQ: main.cpp shaders/vert.spv shaders/frag.spv shaders/comp.spv
	g++ $(CFLAGS) -fPIC -o FluxHQ main.cpp $(LDFLAGS)

test: FluxHQ
	@echo $(STB_INCLUDE_PATH)
	make clean
	./FluxHQ

clean:
	rm FluxHQ shaders/frag.spv shaders/vert.spv || true

shaders/vert.spv: shaders/shader.vert
	glslc shaders/shader.vert -o shaders/vert.spv

shaders/frag.spv: shaders/shader.frag
	glslc shaders/shader.frag -o shaders/frag.spv

shaders/comp.spv: shaders/shader.comp
	glslc shaders/shader.comp -o shaders/comp.spv

commit:
	git push -u VKHQ master
