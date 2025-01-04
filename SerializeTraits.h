#pragma once
#include <string>
#include <vector>
#include <concepts>

namespace ssf
{
	#define SERIALIZE(x) serializer.Serialize(x, #x)

	template<typename T>
	concept SerializeCommonType = (
		std::is_integral_v<T> || 
		std::is_floating_point_v<T> || 
		std::is_enum_v<T> ||
		std::is_same_v<T, std::string>);

	template<typename T>
	class SerializeTraits
	{
	public:
		template<typename SerializeFunction>
		static void Serialize(T& data, SerializeFunction& serialize)
		{
			data.Serialize(serialize);
		}
	};
}