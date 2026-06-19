# Optional set TINYOBJ_PATH to target specific version, otherwise defaults to external/tinyobjloader
# set(TINYOBJ_PATH /Users/brendan/dev/tinyobjloader)

# TINYOBJ_PATH = /home/wojciech/MojeRzeczy/Code/C++/libs
# CFLAGS = -std=c++17 -O2 -I$(TINYOBJ_PATH)
# LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

# # create list of all spv files and set as dependency
# vertSources = $(shell find ./shaders -type f -name "*.vert")
# vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSources))
# fragSources = $(shell find ./shaders -type f -name "*.frag")
# fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSources))

# #GLSLC = /usr/local/bin/glslc
# GLSLC = /bin/glslc

# TARGET = VulkanTest.out

# $(TARGET): $(vertObjFiles) $(fragObjFiles)
# $(TARGET): *.cpp *.hpp
# 	g++ $(CFLAGS) -o $(TARGET) *.cpp $(LDFLAGS)

# # make shader targets
# %.spv: %
# 	${GLSLC} $< -o $@


# .PHONY: test clean

# test: $(TARGET)
# 	./$(TARGET)

# clean:
# 	rm -f $(TARGET)
# 	rm -f *.spv