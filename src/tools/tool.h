#pragma once

class Editor;

class Tool {
public:
	explicit Tool(Editor * editor) : editor(editor) { }

	virtual ~Tool() = default;

	virtual void MousePressed(int x, int y) = 0;
	virtual void MouseDragged(int x, int y) = 0;
	virtual void MouseReleased(int x, int y) = 0;

protected:
	Editor * editor;
};
