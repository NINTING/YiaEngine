#ifndef SCENE_INDEX_ARRAY_H
#define SCENE_INDEX_ARRAY_H

#include"SceneEnum.h"

namespace YiaEngine
{
	namespace Scene
	{
		class IndexArray
		{
		public:
			IndexArray(void* data, size_t size,
				DataType data_type = DataType::kUint16_1, uint32_t material_id = 0) :
				data_type_(data_type), count_(size), data_(data) 
			{
				data_size_ = count_ * DataFormatStride(data_type);
			}
			IndexArray(const IndexArray& indexarray) = default;
			IndexArray(IndexArray&& indexarray) = default;
			IndexArray& operator = (const IndexArray&) = default;
			IndexArray& operator = (IndexArray&&) = default;

			size_t data_size()
			{
				return data_size_;
			}
			size_t count()
			{
				return count_;
			}
			void* data()
			{
				return data_;
			}
		private:
			DataType data_type_;
			void* data_;
			size_t data_size_;
			size_t count_;
			//一个模型可能会包含多个材质，每个索引列表可能引用不同的材质
			//material_id_与GemometryNode的materialRef相对应
			uint32_t material_id_;

			//网格构成存在不连续时，将会使用strip形式，
			//当存在line_strip 或者 triangle_trip时，会在不连续的地方
			//给出重置信号，restart就是这个重置信号。

			//when the mesh is discontinuous,data_type_ = xxx_trip
			//when the data_type_ = xxx_trip, the reset signal will be given at the 
			//discontinus place. restart is the reset signal
			uint64_t restart_;

		};
	}
}
#endif //SCENE_INDEX_ARRAY_H