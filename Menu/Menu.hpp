#pragma once

#include <Siv3D.hpp>
#include <functional>
#include <memory>

namespace Menu {

	struct Region {

		Region() = default;

		constexpr Region(int _top, int _bottom, int _right, int _left)
			: top(_top)
			, bottom(_bottom)
			, right(_right)
			, left(_left) {}

		int top = 0;
		int bottom = 0;
		int right = 0;
		int left = 0;
	};

	template<class Result>
	class IMenu {
	protected:

		Point pos{ 32,32 };
		Size size{ 32,32 };

		Region margin{ 2,2,2,16 };
		Region padding{ 2,2,2,2 };

		String name{ L"No Name" };

		std::vector<std::unique_ptr<IMenu>> items;
		int cursor = 0;
		bool selected = true;

		std::shared_ptr<Result> m_result;

		std::function<bool()> up;
		std::function<bool()> down;
		std::function<bool()> ok;
		std::function<bool()> cancel;

	public:

		IMenu() {

			up = []() {return Input::KeyUp.clicked; };
			down = []() {return Input::KeyDown.clicked; };
			ok = []() {return Input::KeyZ.clicked; };
			cancel = []() {return Input::KeyX.clicked; };

		}

		IMenu(const IMenu& base) : IMenu(base, L"No Name") {	}
		IMenu(const IMenu& base, const String& name) {

			this->name = name;
			pos = base.pos + Point(16, 16);

			up = base.up;
			down = base.down;
			ok = base.ok;
			cancel = base.cancel;

			m_result = base.m_result;
			selected = false;

		}

		IMenu(IMenu&& other) noexcept {
			*this = std::move(other);
		}

		IMenu& operator=(IMenu&& other) noexcept {
			if (this != &other) {

				this->pos = std::move(other.pos);
				this->size = std::move(other.size);

				this->margin = std::move(other.margin);
				this->padding = std::move(other.padding);

				this->name = std::move(other.name);

				this->items = std::move(other.items);
				this->cursor = other.cursor;
				this->selected = other.selected;

				this->m_result = other.m_result;

				this->up = std::move(other.up);
				this->down = std::move(other.down);
				this->ok = std::move(other.ok);
				this->cancel = std::move(other.cancel);

			}
			return *this;
		}

		void bindKeyUp(std::function<bool()> func) { up = func; }
		void bindKeyDown(std::function<bool()> func) { down = func; }
		void bindKeyOk(std::function<bool()> func) { ok = func; }
		void bindKeyCancel(std::function<bool()> func) { cancel = func; }

		virtual bool update() = 0;

		virtual void draw() const = 0;

		const Result& result() const {
			return *m_result;
		}

		const String& getName() const { return name; }
		bool getSelected() const { return selected; }
		void setSelected(bool f) { selected = f; }

		IMenu& addItem(IMenu&& item) {

			std::unique_ptr<IMenu> uItem;

			uItem.reset(&item);
			items.emplace_back(std::move(uItem));

			return *this;
		}

	};

	template<class Result>
	class Menu : public IMenu<Result> {
	private:



	public:

		Menu() : IMenu() {

		}
		Menu(const IMenu& base) : IMenu(base) {

		}
		Menu(const IMenu& base, const String& name) : IMenu(base, name) {

		}

		bool update() {

			const int Count = static_cast<int>(items.size());

			if (selected)
			{
				if (up())
				{
					cursor = Min(cursor + 1, Count - 1);
				}
				else if (down())
				{
					cursor = Max(cursor - 1, 0);
				}

				if (cancel())
				{
					return false;
				}
				else if (ok())
				{
					selected = false;
					items[cursor]->setSelected(true);
				}
			}
			else
			{
				if (!items[cursor]->update())
				{
					selected = true;
				}
			}

			return true;
		}

		void draw() const {

			Rect box(pos, size);

			box.drawFrame(1, 0, Palette::White);
			const int Count = static_cast<int>(items.size());

			for (int i = 0; i < Count; i++)
			{
				PutText(items[i]->getName()).from(pos.x, pos.y + i * 16);
			}

			if (!selected)
			{
				items[cursor]->draw();
			}

		}

	};

}
