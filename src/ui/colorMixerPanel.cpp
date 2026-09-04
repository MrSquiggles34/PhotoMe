#include "colorMixerPanel.h"

#include "../core/editor.h"
#include "../document/document.h"
#include "../document/layer.h"
#include "../rendering/colorMixerEffect.h"

#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QSignalBlocker>
#include <QPushButton>

ColorMixerPanel::ColorMixerPanel(Editor * editor)
	: editor(editor) {
	QVBoxLayout * layout = new QVBoxLayout(this);

	// --------------------------------------------------
	// Title
	// --------------------------------------------------

	QLabel * title = new QLabel("Color Mixer", this);
	layout->addWidget(title);

	// --------------------------------------------------
	// Add Color Mixer
	// --------------------------------------------------

	QPushButton * addButton = new QPushButton("Add Color Mixer", this);

	layout->addWidget(addButton);

	connect(
		addButton,
		&QPushButton::clicked,
		this,
		&ColorMixerPanel::AddColorMixer);

	// --------------------------------------------------
	// Hue
	// --------------------------------------------------

	QLabel * hueLabel = new QLabel("Hue", this);
	layout->addWidget(hueLabel);

	hueSlider = new QSlider(Qt::Horizontal, this);
	hueSlider->setRange(-180, 180);
	hueSlider->setValue(0);

	layout->addWidget(hueSlider);

	hueValue = new QLabel("0", this);
	layout->addWidget(hueValue);

	// --------------------------------------------------
	// Saturation
	// --------------------------------------------------

	QLabel * saturationLabel = new QLabel("Saturation", this);

	layout->addWidget(saturationLabel);

	saturationSlider = new QSlider(Qt::Horizontal, this);

	saturationSlider->setRange(-100, 100);
	saturationSlider->setValue(0);

	layout->addWidget(saturationSlider);

	saturationValue = new QLabel("0", this);
	layout->addWidget(saturationValue);

	// --------------------------------------------------
	// Luminance
	// --------------------------------------------------

	QLabel * luminanceLabel = new QLabel("Luminance", this);

	layout->addWidget(luminanceLabel);

	luminanceSlider = new QSlider(Qt::Horizontal, this);

	luminanceSlider->setRange(-100, 100);
	luminanceSlider->setValue(0);

	layout->addWidget(luminanceSlider);

	luminanceValue = new QLabel("0", this);
	layout->addWidget(luminanceValue);

	// --------------------------------------------------
	// Connections
	// --------------------------------------------------

	connect(
		hueSlider,
		&QSlider::valueChanged,
		this,
		&ColorMixerPanel::HueChanged);

	connect(
		saturationSlider,
		&QSlider::valueChanged,
		this,
		&ColorMixerPanel::SaturationChanged);

	connect(
		luminanceSlider,
		&QSlider::valueChanged,
		this,
		&ColorMixerPanel::LuminanceChanged);
}

void ColorMixerPanel::HueChanged(int value) {
	ofLogNotice()
		<< "HUE SLIDER: "
		<< value;

	hueValue->setText(QString::number(value));

	Layer * layer = editor->GetDocument()->GetActiveLayer();

	if (!layer) {
		ofLogNotice() << "NO ACTIVE LAYER";
		return;
	}

	ColorMixerEffect * effect = layer->GetEffect<ColorMixerEffect>();

	if (!effect) {
		ofLogNotice() << "NO COLOR MIXER EFFECT";
		return;
	}

	effect->SetHue(
		static_cast<float>(value));

	ofLogNotice()
		<< "EFFECT HUE: "
		<< effect->GetHue();
}

void ColorMixerPanel::SaturationChanged(int value) {
	saturationValue->setText(QString::number(value));

	Layer * layer = editor->GetDocument()->GetActiveLayer();

	if (!layer)
		return;

	ColorMixerEffect * effect = layer->GetEffect<ColorMixerEffect>();

	if (!effect)
		return;

	effect->SetSaturation(
		static_cast<float>(value) / 100.0f);
}

void ColorMixerPanel::LuminanceChanged(int value) {
	luminanceValue->setText(QString::number(value));

	Layer * layer = editor->GetDocument()->GetActiveLayer();

	if (!layer)
		return;

	ColorMixerEffect * effect = layer->GetEffect<ColorMixerEffect>();

	if (!effect)
		return;

	effect->SetLuminance(
		static_cast<float>(value) / 100.0f);
}

void ColorMixerPanel::Refresh() {
	QSignalBlocker hueBlocker(hueSlider);
	QSignalBlocker saturationBlocker(saturationSlider);
	QSignalBlocker luminanceBlocker(luminanceSlider);

	Layer * layer = editor->GetDocument()->GetActiveLayer();

	if (!layer) {
		hueSlider->setValue(0);
		saturationSlider->setValue(0);
		luminanceSlider->setValue(0);
		return;
	}

	ColorMixerEffect * effect = layer->GetEffect<ColorMixerEffect>();

	if (!effect) {
		hueSlider->setValue(0);
		saturationSlider->setValue(0);
		luminanceSlider->setValue(0);
		return;
	}

	hueSlider->setValue(
		static_cast<int>(effect->GetHue()));

	saturationSlider->setValue(
		static_cast<int>(
			effect->GetSaturation() * 100.0f));

	luminanceSlider->setValue(
		static_cast<int>(
			effect->GetLuminance() * 100.0f));
}

void ColorMixerPanel::AddColorMixer() {
	editor->AddColorMixerEffect();

	Refresh();
}
