#pragma once

#include <memory>

class Tool;

class ToolManager {
public:
	void SetTool(std::unique_ptr<Tool> tool);

	Tool * GetTool();

	void MousePressed(int x, int y);
	void MouseDragged(int x, int y);
	void MouseReleased(int x, int y);

private:
	std::unique_ptr<Tool> activeTool;
};
