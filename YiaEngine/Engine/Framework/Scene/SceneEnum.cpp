

#include<assert.h>

#include"SceneEnum.h"

size_t YiaEngine::Scene::DataTypeStride(DataType data_type)
{

	int stride_in_bytes = 0;
	switch (data_type)
	{
	case YiaEngine::Scene::DataType::kUint16_1:
		stride_in_bytes = sizeof(uint16_t);
		break;
	case YiaEngine::Scene::DataType::kUint32_1:
	case YiaEngine::Scene::DataType::kFloat_1:
	case YiaEngine::Scene::DataType::kUint16_2:
		stride_in_bytes = 4;
		break;
	case YiaEngine::Scene::DataType::kUint16_3:
		stride_in_bytes = 6;
		break;
	case YiaEngine::Scene::DataType::kUint16_4:
	case YiaEngine::Scene::DataType::kDouble_1:
	case YiaEngine::Scene::DataType::kUint32_2:
	case YiaEngine::Scene::DataType::kFloat_2:
		stride_in_bytes = 8;
		break;
	case YiaEngine::Scene::DataType::kUint32_3:
	case YiaEngine::Scene::DataType::kFloat_3:
		stride_in_bytes = 12;
		break;
	case YiaEngine::Scene::DataType::kDouble_2:
	case YiaEngine::Scene::DataType::kUint32_4:
	case YiaEngine::Scene::DataType::kFloat_4:
		stride_in_bytes = 16;
		break;
	case YiaEngine::Scene::DataType::kDouble_3:
		stride_in_bytes = 24;
		break;
	case YiaEngine::Scene::DataType::kDouble_4:
		stride_in_bytes = 32;
		break;
	default:
		assert(false);
		break;
	}
	return stride_in_bytes;
}
