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
				data_type_(data_type), size_(size), data_(data) {}
			IndexArray(const IndexArray& indexarray) = default;
			IndexArray(IndexArray&& indexarray) = default;
			IndexArray& operator = (const IndexArray&) = default;
			IndexArray& operator = (IndexArray&&) = default;
		private:
			DataType data_type_;
			void* data_;
			size_t size_;

			//һ��ģ�Ϳ��ܻ����������ʣ�ÿ�������б��������ò�ͬ�Ĳ���
			//material_id_��GemometryNode��materialRef���Ӧ
			uint32_t material_id_;

			//���񹹳ɴ��ڲ�����ʱ������ʹ��strip��ʽ��
			//������line_strip ���� triangle_tripʱ�����ڲ������ĵط�
			//���������źţ�restart������������źš�

			//when the mesh is discontinuous,data_type_ = xxx_trip
			//when the data_type_ = xxx_trip, the reset signal will be given at the 
			//discontinus place. restart is the reset signal
			uint64_t restart_;

		};
	}
}
#endif //SCENE_INDEX_ARRAY_H