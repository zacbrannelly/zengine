#pragma once

#include <ZEngine-Core/ImmediateUI/GUIWindow.h>

namespace ZEngine
{
	class UnsavedDialog : public GUIElement
	{
	public:
		UnsavedDialog();
		~UnsavedDialog();

		void RenderElement() override;
		GUIElementType GetType() override;

		void Reset();
		bool ShouldClose() const;
		bool ShouldSave() const;
		bool ShouldNotSave() const;

	private:
		bool _shouldClose;
		bool _shouldSave;
		bool _shouldNotSave;
	};
}
