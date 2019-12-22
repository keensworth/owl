// ======================================================================== //
// Copyright 2019 Ingo Wald                                                 //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#include "Context.h"
#include "Module.h"
#include "Geometry.h"
#include "owl/ll/Device.h"

#define LOG(message)                               \
  std::cout                                        \
  << GDT_TERMINAL_LIGHT_BLUE                       \
  << "#owl.ng: "                                   \
  << message                                       \
  << GDT_TERMINAL_DEFAULT << std::endl

#define LOG_OK(message)                         \
  std::cout                                     \
  << GDT_TERMINAL_BLUE                          \
  << "#owl.ng: "                                \
  << message                                    \
  << GDT_TERMINAL_DEFAULT << std::endl

namespace owl {

  Context::SP Context::create()
  {
    LOG("creating node-graph context");
    return std::make_shared<Context>();
  }
  
  Context::Context()
    : buffers(this),
      groups(this),
      rayGenTypes(this),
      rayGens(this),
      missProgTypes(this),
      missProgs(this),
      geomTypes(this),
      geoms(this),
      modules(this)
  {
    LOG("context ramping up - creating low-level devicegroup");
    // ll = ll::DeviceGroup::create();
    llo = lloContextCreate(nullptr,0);
    LOG_OK("device group created");
  }
  
  Buffer::SP Context::hostPinnedBufferCreate()
  {
    PING;
    Buffer::SP buffer = std::make_shared<HostPinnedBuffer>(this);
    PING;
    assert(buffer);
    return buffer;
  }

  Buffer::SP Context::deviceBufferCreate()
  {
    PING;
    Buffer::SP buffer = std::make_shared<DeviceBuffer>(this);
    PING;
    assert(buffer);
    return buffer;
  }

  RayGen::SP
  Context::createRayGen(const std::shared_ptr<RayGenType> &type)
  {
    return std::make_shared<RayGen>(this,type);
  }

  MissProg::SP
  Context::createMissProg(const std::shared_ptr<MissProgType> &type)
  {
    return std::make_shared<MissProg>(this,type);
  }

  InstanceGroup::SP Context::createInstanceGroup(size_t numChildren)
  {
    return std::make_shared<InstanceGroup>(this,numChildren);
  }

  RayGenType::SP
  Context::createRayGenType(Module::SP module,
                            const std::string &progName,
                            size_t varStructSize,
                            const std::vector<OWLVarDecl> &varDecls)
  {
    return std::make_shared<RayGenType>(this,
                                        module,progName,
                                        varStructSize,
                                        varDecls);
  }
  

  MissProgType::SP
  Context::createMissProgType(Module::SP module,
                            const std::string &progName,
                            size_t varStructSize,
                            const std::vector<OWLVarDecl> &varDecls)
  {
    return std::make_shared<MissProgType>(this,
                                        module,progName,
                                        varStructSize,
                                        varDecls);
  }
  

  GeomGroup::SP Context::trianglesGroupCreate(size_t numChildren)
  {
    return std::make_shared<TrianglesGroup>(this,numChildren);
  }


  GeomType::SP
  Context::createGeomType(OWLGeomKind kind,
                              size_t varStructSize,
                              const std::vector<OWLVarDecl> &varDecls)
  {
    switch(kind) {
    case OWL_GEOMETRY_TRIANGLES:
      return std::make_shared<TrianglesGeomType>(this,varStructSize,varDecls);
    case OWL_GEOMETRY_USER:
      return std::make_shared<UserGeomType>(this,varStructSize,varDecls);
    default:
      OWL_NOTIMPLEMENTED;
    }
  }

  Module::SP Context::createModule(const std::string &ptxCode)
  {
    return std::make_shared<Module>(ptxCode,modules.allocID());
  }

  std::shared_ptr<Geom> UserGeomType::createGeom()
  {
    GeomType::SP self
      = std::dynamic_pointer_cast<GeomType>(shared_from_this());
    assert(self);
    return std::make_shared<UserGeom>(context,self);
  }

  std::shared_ptr<Geom> TrianglesGeomType::createGeom()
  {
    GeomType::SP self
      = std::dynamic_pointer_cast<GeomType>(shared_from_this());
    assert(self);
    return std::make_shared<TrianglesGeom>(context,self);
  }


  void Context::buildSBT()
  {
    IGNORING_THIS();
  }

  void Context::buildPipeline()
  {
    IGNORING_THIS();
  }

  void Context::buildPrograms()
  {
    IGNORING_THIS();
  }

} // ::owl
