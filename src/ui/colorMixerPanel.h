#pragma once

#include <QWidget>

class QSlider;
class QLabel;
class QPushButton;
class Editor;

class ColorMixerPanel : public QWidget {

public:
	explicit ColorMixerPanel(Editor * editor);

	void Refresh();

	void HueChanged(int value);
	void SaturationChanged(int value);
	void LuminanceChanged(int value);

	void AddColorMixer();

private:
	Editor * editor = nullptr;

	QSlider * hueSlider = nullptr;
	QSlider * saturationSlider = nullptr;
	QSlider * luminanceSlider = nullptr;

	QLabel * hueValue = nullptr;
	QLabel * saturationValue = nullptr;
	QLabel * luminanceValue = nullptr;

};
