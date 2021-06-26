
//
#include<iostream>
//


//
#include"Scene/SceneObject.h"
#include"Scene/SceneNode.h"
#include"Common/AssetLoder/AssetLoder.h"
#include"Common/MemoryManager.h"


int main()
{
    AssetLoder assetLoder;
    
    /*std::shared_ptr<GeometryObject>obj = std::make_shared<GeometryObject>();
    std::shared_ptr<MeshObject> mesh = std::make_shared<MeshObject>();
    std::unique_ptr<GeometryNode> node;
    mesh->Serialize();
   node = assetLoder.LoadModel("Fox/Fox.gltf");*/
    //  std::cout<< node->Serialize()<<std::endl;

    /*float* data = mesh_object->object_ref()->
   std::shared_ptr<GeometryObject> geo_obj = std::make_shared<GeometryObject>();
    geo_obj->AddMesh(mesh_object);
    std::cout << mesh_object->ToString();
    
    node->set_object_ref(geo_obj);*/
    /*auto guid = xg::newGuid();
    std::cout << guid.str();*/
    std::unique_ptr<YiaEngine::Scene::GeometryNode> node;
    node = assetLoder.LoadModel("Fox/Fox.gltf");
    return 0;                                                                                                                                                                                                                                                                    
}