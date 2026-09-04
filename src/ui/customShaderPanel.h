#pragma once

#include <QWidget>

class QPushButton;
class QLabel;
class QVBoxLayout;
class Editor;

class CustomShaderPanel : public QWidget {

public:
	explicit CustomShaderPanel(Editor * editor);

	void Refresh();

private:
	void LoadShader();

	void RebuildParameters();

private:
	Editor * editor = nullptr;

	QVBoxLayout * layout = nullptr;
	QVBoxLayout * parameterLayout = nullptr;

	QPushButton * loadButton = nullptr;
	QPushButton * reloadButton = nullptr;

	QLabel * shaderLabel = nullptr;
};
