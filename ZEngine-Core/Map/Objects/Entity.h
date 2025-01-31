#pragma once

#include "ZObject.h"
#include "../../Component/Component.h"
#include <vector>

namespace ZEngine
{
	class Transform;

	class Entity : public ZObject
	{
	public:
		Entity(std::string name); 
		~Entity();

		void InitComponents();

		void AddComponent(Component* comp, bool shouldInit = true);
		void RemoveComponent(Component* comp);
		Component* GetComponent(ObjectType type) const;
		Component* GetComponentByName(std::string name) const;
		std::vector<Component*> GetComponents(ObjectType type) const;

		template<class T> bool HasComponentType() const
		{
			return GetComponent(T::GetStaticType()) != nullptr;
		}

		template<class T> T* GetComponentByType() const
		{
			return static_cast<T*>(GetComponent(T::GetStaticType()));
		}

		template<class T> std::vector<T*> GetComponentsByType() const
		{
			std::vector<T*> results;
			GetComponentsByType(results, T::GetStaticType());
			return results;
		}

		const std::vector<Component*>& GetAllComponents() const;
		Transform* GetTransform() const;

		static ZObject* CreateInstance(std::string name, ObjectType type);
		static ZObject* Copy(std::string name, ZObject* object);

		static ObjectType GetStaticType()
		{
			return ENTITY;
		}
	private:
		std::vector<Component*> _components;
		Transform* _transform;

		template<class T> void GetComponentsByType(std::vector<T*>& results, ObjectType type) const
		{
			for (auto c : _components)
			{
				if (c->IsDerivedType(type))
					results.push_back(static_cast<T*>(c));
			}
		}
	};
}
