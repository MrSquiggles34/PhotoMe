#pragma once

class Tool {
	public:
		virtual void MousePressed(int x, int y) = 0;
		virtual void MouseDragged(int x, int y) = 0;
		virtual void MouseReleased(int x, int y) = 0;
};
