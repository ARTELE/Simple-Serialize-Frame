#pragma once
#include <string>
#include <vector>
#include <stack>
#include <typeinfo>
#include <memory>
#include <bitset>
#include "SerializeTraits.h"
#include "BML.h"

namespace ssf
{
	class ValueTreeSerializeBase
	{
	protected:
		std::shared_ptr<bml::ValueNode> mRoot;
		std::stack<std::shared_ptr<bml::ValueNode>> mStack;
	public:
		ValueTreeSerializeBase(const std::shared_ptr<bml::ValueNode>& root) : mRoot(root) { mStack.push(root); }
		auto Root() const { return mRoot; }
	};

	class ValueTreeRead : public ValueTreeSerializeBase
	{
	public:
		ValueTreeRead(const std::shared_ptr<bml::ValueNode>& root) : ValueTreeSerializeBase(root) {}
		bool IsWritting() const { return false; }
		bool IsReading() const { return true; }

		template<typename T>
		void Serialize(T& data, const std::string& name)
		{
			auto root = mStack.top();
			if (root != nullptr)
			{
				auto node = root->Find(name);
				if (node != nullptr)
				{
					mStack.push(node);
					SerializeTraits<T>::Serialize(data, *this);
					mStack.pop();
				}
			}
		}

		template<SerializeCommonType T>
		void Serialize(T& data, const std::string& name)
		{
			auto root = mStack.top();
			if (root != nullptr)
			{
				auto node = root->Find(name);
				if (node != nullptr)
				{
					data = node->Value<T>();
				}
			}
		}

		template<typename T>
		void SerializeSubData(T& data)
		{
			auto root = mStack.top();
			if (root != nullptr)
			{
				mStack.push(root);
				SerializeTraits<T>::Serialize(data, *this);
				mStack.pop();
			}
		}

		template<typename T>
		void Serialize(std::vector<T>& data, const std::string& name)
		{
			auto root = mStack.top();
			if (root != nullptr)
			{
				size_t size = root->ChildCount();
				data.resize(size);
				for (int i = 0; i < size; i++)
				{
					auto node = root->Child(i);
					mStack.push(node);
					SerializeSubData(data[i]);
					mStack.pop();
				}
			}
		}
	};

	class ValueTreeWrite : public ValueTreeSerializeBase
	{
	public:
		ValueTreeWrite(const std::shared_ptr<bml::ValueNode>& root) : ValueTreeSerializeBase(root) {}
		bool IsWritting() const { return true; }
		bool IsReading() const { return false; }

		template<typename T>
		void Serialize(T& data, const std::string& name)
		{
			auto root = mStack.top();
			if (root != nullptr)
			{
				auto node = std::make_shared<bml::ValueNode>(name);
				root->AddChild(node);
				mStack.push(node);
				SerializeTraits<T>::Serialize(data, *this);
				mStack.pop();
			}
		}

		template<SerializeCommonType T>
		void Serialize(T& data, const std::string& name)
		{
			auto root = mStack.top();
			if (root != nullptr)
			{
				root->AddChild(std::make_shared<bml::ValueNode>(name, data));
			}
		}

		template<typename T>
		void SerializeSubData(T& data)
		{
			auto root = mStack.top();
			if (root != nullptr)
			{
				mStack.push(root);
				SerializeTraits<T>::Serialize(data, *this);
				mStack.pop();
			}
		}

		template<typename T>
		void Serialize(std::vector<T>& data, const std::string& name)
		{
			auto root = mStack.top();
			if (root != nullptr)
			{
				auto node = std::make_shared<bml::ValueNode>(name);
				root->AddChild(node);
				for (int i = 0; i < data.size(); i++)
				{
					auto child = std::make_shared<bml::ValueNode>(typeid(T).name());
					node->AddChild(child);
					mStack.push(child);
					SerializeSubData(data[i]);
					mStack.pop();
				}
			}
		}
	};
}