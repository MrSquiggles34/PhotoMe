#include "toolManager.h"
#include "tool.h"

void ToolManager::SetTool(std::unique_ptr<Tool> tool) {
	activeTool = std::move(tool);
}

Tool * ToolManager::GetTool() {
	return activeTool.get();
}

void ToolManager::MousePressed(int x, int y) {
	if (activeTool)
		activeTool->MousePressed(x, y);
}

void ToolManager::MouseDragged(int x, int y) {
	if (activeTool)
		activeTool->MouseDragged(x, y);
}

void ToolManager::MouseReleased(int x, int y) {
	if (activeTool)
		activeTool->MouseReleased(x, y);
}

