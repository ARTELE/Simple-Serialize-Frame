#include <iostream>
#include "ValueTreeSerialize.h"

struct IdentityInformation
{
	int age = 0;
	float weight = 0;
	char sex;
	std::string name;
	std::vector<IdentityInformation> friends;

	template<typename T>
	void Serialize(T& serializer)
	{
		SERIALIZE(age);
		SERIALIZE(weight);
		SERIALIZE(sex);
		SERIALIZE(name);
		SERIALIZE(friends);
	}
};

int main()
{
	IdentityInformation info;
	info.age = 30;
	info.name = "artele";
	info.sex = 'M';
	info.weight = 72.5;

	IdentityInformation fri;
	info.friends.push_back(fri);
	info.friends.push_back(fri);
	info.friends.push_back(fri);

	// write
	auto writeRoot = std::make_shared<bml::ValueNode>("root");
	ssf::ValueTreeWrite(writeRoot).Serialize(info, "IdentityInformation");
	bml::Serialize::ToTextFile(writeRoot, "info.txt");

	// read
	IdentityInformation infoTest;
	auto readRoot = bml::Serialize::FromTextFile("info.txt");
	ssf::ValueTreeRead(readRoot).Serialize(infoTest, "IdentityInformation");

	return 0;
}