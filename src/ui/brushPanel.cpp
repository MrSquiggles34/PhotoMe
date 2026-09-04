#include "brushPanel.h"

#include "../core/editor.h"
#include "../tools/brushTool.h"

#include <QColorDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>

BrushPanel::BrushPanel(
	Editor * editor,
	QWidget * parent)
	: QWidget(parent)
	, editor(editor) {
	QLabel * sizeTitle = new QLabel("Size", this);

	sizeSlider = new QSlider(
		Qt::Horizontal,
		this);

	sizeSlider->setRange(
		1,
		200);

	sizeSlider->setValue(15);

	sizeLabel = new QLabel("15 px", this);

	QHBoxLayout * sizeLayout = new QHBoxLayout();

	sizeLayout->addWidget(
		sizeSlider);

	sizeLayout->addWidget(
		sizeLabel);

	QLabel * hardnessTitle = new QLabel("Hardness", this);

	hardnessSlider = new QSlider(
		Qt::Horizontal,
		this);

	hardnessSlider->setRange(
		0,
		100);

	hardnessSlider->setValue(100);

	hardnessLabel = new QLabel("100%", this);

	QHBoxLayout * hardnessLayout = new QHBoxLayout();

	hardnessLayout->addWidget(
		hardnessSlider);

	hardnessLayout->addWidget(
		hardnessLabel);

	colorButton = new QPushButton(
		"Color",
		this);

	QVBoxLayout * layout = new QVBoxLayout(this);

	layout->addWidget(
		sizeTitle);

	layout->addLayout(
		sizeLayout);

	layout->addWidget(
		hardnessTitle);

	layout->addLayout(
		hardnessLayout);

	layout->addWidget(
		colorButton);

	setLayout(layout);

	connect(
		sizeSlider,
		&QSlider::valueChanged,
		this,
		&BrushPanel::OnSizeChanged);

	connect(
		hardnessSlider,
		&QSlider::valueChanged,
		this,
		&BrushPanel::OnHardnessChanged);

	connect(
		colorButton,
		&QPushButton::clicked,
		this,
		&BrushPanel::OnColorButton);
}

void BrushPanel::OnSizeChanged(int value) {
	sizeLabel->setText(
		QString::number(value) + " px");

	editor->SetBrushSize(
		static_cast<float>(value));
}

void BrushPanel::OnHardnessChanged(int value) {
	hardnessLabel->setText(
		QString::number(value) + "%");

	editor->SetBrushHardness(
		static_cast<float>(value) / 100.0f);
}

void BrushPanel::OnColorButton() {
	QColor color = QColorDialog::getColor(
		Qt::black,
		this,
		"Brush Color");

	if (!color.isValid())
		return;

	editor->SetBrushColor(
		ofColor(
			color.red(),
			color.green(),
			color.blue(),
			color.alpha()));
}
