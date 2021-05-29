

//

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
    assetLoder.LoadMesh("Box/Box.gltf");

    std::unique_ptr<GeometryNode> node;

    node.set_object_ref(obj);
    return 0;                                                                                                                                                                                                                                                                    
}