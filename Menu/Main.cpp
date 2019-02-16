#include <Siv3D.hpp>

#include "Menu.hpp"

void Main() {

	Menu::Menu<int> menu;
	{
		Menu::Menu<int> branch01(menu, L"枝01");
		Menu::Menu<int> branch02(menu, L"枝02");
		Menu::Menu<int> branch03(menu, L"枝03");
		Menu::Menu<int> branch04(menu, L"枝04");
		
		branch01.addItem(Menu::Menu<int>(branch01, L"枝01-01"));
		branch01.addItem(Menu::Menu<int>(branch01, L"枝01-02"));
		branch01.addItem(Menu::Menu<int>(branch01, L"枝01-03"));
		branch01.addItem(Menu::Menu<int>(branch01, L"枝01-04"));

		menu.addItem(std::move(branch01)).addItem(std::move(branch02)).addItem(std::move(branch03)).addItem(std::move(branch04));
	}
	while (System::Update())
	{

		//menu.update();

		//menu.draw();

	}
}
