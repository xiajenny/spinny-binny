#pragma once
#include "GameEngine\EntitySystem\Entity.h"
#include "GameEngine\EntitySystem\Components\TextComponent.h"

namespace Game
{

	class TextEntity : public GameEngine::Entity
	{
	public:
		TextEntity(int z = -1);
		~TextEntity();

		virtual void OnAddToWorld() override;
		virtual void OnRemoveFromWorld() override;

	protected:
		GameEngine::TextComponent* m_textComponent;
	};
}
