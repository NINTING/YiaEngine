
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
    
    std::shared_ptr<GeometryObject>obj = std::make_shared<GeometryObject>();
    std::shared_ptr<MeshObject> mesh;
    auto mesh_object = assetLoder.LoadMesh("Box/Box.gltf");
    std::shared_ptr<GeometryObject> geo_obj = std::make_shared<GeometryObject>();
    geo_obj->AddMesh(mesh_object);
    std::cout << mesh_object->ToString();
    std::unique_ptr<GeometryNode> node;
    node->set_object_ref(geo_obj);
    /*auto guid = xg::newGuid();
    std::cout << guid.str();*/
    return 0;                                                                                                                                                                                                                                                                    
}