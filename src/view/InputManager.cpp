#include "InputManager.hpp"

bool InputManager::isSpriteClicked(Sprite sp, Mouse::Button mButton, RenderWindow &window) const {
	if (Mouse::isButtonPressed(mButton)) {
		if (sp.getGlobalBounds().contains(getMousePosition(window))) {
			return true;
		}
	}
	return false;
}
Vector2f InputManager::getMousePosition(RenderWindow &window) const {
	Vector2i tmp = Mouse::getPosition(window);
	Vector2f v(tmp.x, tmp.y);
	return v;
}