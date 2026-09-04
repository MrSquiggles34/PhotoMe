#pragma once

#include <QWidget>

class Editor;
class QSlider;
class QLabel;
class QPushButton;

class BrushPanel : public QWidget {
public:
	explicit BrushPanel(
		Editor * editor,
		QWidget * parent = nullptr);

private:
	Editor * editor;

	QSlider * sizeSlider;
	QLabel * sizeLabel;

	QSlider * hardnessSlider;
	QLabel * hardnessLabel;

	QPushButton * colorButton;

	void OnSizeChanged(int value);
	void OnHardnessChanged(int value);
	void OnColorButton();
};
