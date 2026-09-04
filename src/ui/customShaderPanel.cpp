#include "customShaderPanel.h"

#include "../core/editor.h"
#include "../document/document.h"
#include "../document/layer.h"
#include "../rendering/customShaderEffect.h"
#include "../rendering/shaderParameter.h"

#include <QCheckBox>
#include <QColorDialog>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>

CustomShaderPanel::CustomShaderPanel(Editor * editor)
	: editor(editor) {

	layout = new QVBoxLayout(this);

	// --------------------------------------------------
	// Title
	// --------------------------------------------------

	QLabel * title = new QLabel("Custom Shader", this);

	layout->addWidget(title);

	// --------------------------------------------------
	// Current shader
	// --------------------------------------------------

	shaderLabel = new QLabel("No shader loaded", this);

	shaderLabel->setWordWrap(true);

	layout->addWidget(shaderLabel);

	// --------------------------------------------------
	// Load
	// --------------------------------------------------

	loadButton = new QPushButton(
		"Load Fragment Shader...",
		this);

	layout->addWidget(loadButton);

	// --------------------------------------------------
	// Reload
	// --------------------------------------------------

	reloadButton = new QPushButton(
		"Reload Shader",
		this);

	layout->addWidget(reloadButton);

	// --------------------------------------------------
	// Parameter
	// --------------------------------------------------
	parameterLayout = new QVBoxLayout();

	layout->addLayout(parameterLayout);

	// --------------------------------------------------
	// Connections
	// --------------------------------------------------

	connect(
		loadButton,
		&QPushButton::clicked,
		this,
		&CustomShaderPanel::LoadShader);

	connect(
		reloadButton,
		&QPushButton::clicked,
		this,
		&CustomShaderPanel::Refresh);
}

void CustomShaderPanel::LoadShader() {

	if (!editor)
		return;

	QString path = QFileDialog::getOpenFileName(
		this,
		"Load Fragment Shader",
		QString(),
		"Fragment Shaders (*.frag);;All Files (*)");

	if (path.isEmpty())
		return;

	qDebug() << "Selected shader:"
			 << path;

	qDebug() << "Editor pointer:"
			 << static_cast<void *>(editor);

	std::string shaderPath = path.toLocal8Bit().constData();

	qDebug()
		<< "Qt path length:"
		<< path.length();

	qDebug()
		<< "std::string length:"
		<< shaderPath.length();

	bool success = editor->AddCustomShader(shaderPath);

	qDebug() << "Returned from AddCustomShader";

	qDebug() << "Success:"
			 << success;

	if (success) {
		Refresh();
	}
}

void CustomShaderPanel::Refresh() {

	qDebug() << "REFRESH 1";

	if (!editor)
		return;

	qDebug() << "REFRESH 2";

	Layer * layer = editor->GetDocument()->GetActiveLayer();

	qDebug() << "REFRESH 3"
			 << static_cast<void *>(layer);

	if (!layer) {

		shaderLabel->setText(
			"No active layer");

		RebuildParameters();

		return;
	}

	qDebug() << "REFRESH 4";

	CustomShaderEffect * effect = layer->GetEffect<CustomShaderEffect>();

	qDebug() << "REFRESH 5"
			 << static_cast<void *>(effect);

	if (!effect) {

		shaderLabel->setText(
			"No custom shader");

		RebuildParameters();

		return;
	}

	qDebug() << "REFRESH 6";

	shaderLabel->setText(
		"Shader effect attached");

	qDebug() << "REFRESH 7";

	RebuildParameters();

	qDebug() << "REFRESH 8";
}


void CustomShaderPanel::RebuildParameters() {
	// --------------------------------------------------
	// Remove old parameter widgets
	// --------------------------------------------------

	qDebug() << "REBUILD 1";

	qDebug() << "layout:"
			 << static_cast<void *>(layout);

	qDebug() << "parameterLayout:"
			 << static_cast<void *>(parameterLayout);

	qDebug() << "REBUILD 2";

	if (!parameterLayout) {
		qDebug() << "ERROR: parameterLayout is NULL";
		return;
	}


	while (QLayoutItem * item = parameterLayout->takeAt(0)) {
		if (QWidget * widget = item->widget()) {
			delete widget;
		}

		delete item;
	}

	// --------------------------------------------------
	// Get active layer
	// --------------------------------------------------

	qDebug() << "REBUILD 3";

	if (!editor)
		return;

	qDebug() << "REBUILD 4";

	Layer * layer = editor->GetDocument()->GetActiveLayer();

	qDebug() << "REBUILD 5";

	if (!layer)
		return;

	// --------------------------------------------------
	// Get custom shader
	// --------------------------------------------------

	CustomShaderEffect * effect = layer->GetEffect<CustomShaderEffect>();

	qDebug() << "REBUILD 6"
			 << static_cast<void *>(effect);

	if (!effect)
		return;

	// --------------------------------------------------
	// Generate controls
	// --------------------------------------------------

	qDebug() << "REBUILD 7";

	const auto & parameters = effect->GetParameters();

		qDebug() << "REBUILD 8"
			 << parameters.size();

	for (const ShaderParameter & parameter :
		parameters) {
		// ----------------------------------------------
		// FLOAT
		// ----------------------------------------------

		if (parameter.type == ShaderParameterType::Float) {
			QLabel * label = new QLabel(
				QString::fromUtf8(
					parameter.label.c_str()),
				this);

			parameterLayout->addWidget(label);

			QSlider * slider = new QSlider(
				Qt::Horizontal,
				this);

			int minimum = static_cast<int>(
				parameter.min * 100.0f);

			int maximum = static_cast<int>(
				parameter.max * 100.0f);

			int value = static_cast<int>(
				parameter.floatValue * 100.0f);

			slider->setRange(
				minimum,
				maximum);

			slider->setValue(
				value);

			parameterLayout->addWidget(slider);

			connect(
				slider,
				&QSlider::valueChanged,
				this,
				[this,
					effect,
					name = parameter.name](int value) {
					effect->SetFloatParameter(
						name,
						static_cast<float>(
							value)
							/ 100.0f);
				});
		}

		// ----------------------------------------------
		// INT
		// ----------------------------------------------

		else if (
			parameter.type == ShaderParameterType::Int) {
			QLabel * label = new QLabel(
				QString::fromUtf8(
					parameter.label.c_str()),
				this);

			parameterLayout->addWidget(label);

			QSpinBox * spinBox = new QSpinBox(this);

			spinBox->setRange(
				static_cast<int>(
					parameter.min),
				static_cast<int>(
					parameter.max));

			spinBox->setValue(
				parameter.intValue);

			parameterLayout->addWidget(spinBox);

			connect(
				spinBox,
				QOverload<int>::of(
					&QSpinBox::valueChanged),
				this,
				[this,
					effect,
					name = parameter.name](int value) {
					effect->SetIntParameter(
						name,
						value);
				});
		}

		// ----------------------------------------------
		// BOOL
		// ----------------------------------------------

		else if (
			parameter.type == ShaderParameterType::Bool) {
			QCheckBox * checkBox = new QCheckBox(
				QString::fromUtf8(
					parameter.label.c_str()),
				this);

			checkBox->setChecked(
				parameter.boolValue);

			parameterLayout->addWidget(checkBox);

			connect(
				checkBox,
				&QCheckBox::toggled,
				this,
				[this,
					effect,
					name = parameter.name](bool checked) {
					effect->SetBoolParameter(
						name,
						checked);
				});
		}

		// ----------------------------------------------
		// COLOR
		// ----------------------------------------------

		else if (
			parameter.type == ShaderParameterType::Color) {
			QPushButton * button = new QPushButton(
				QString::fromUtf8(
					parameter.label.c_str()),
				this);

			parameterLayout->addWidget(button);

			connect(
				button,
				&QPushButton::clicked,
				this,
				[this,
					effect,
					name = parameter.name]() {
					const auto & parameters = effect->GetParameters();

					glm::vec4 currentColor(
						1.0f);

					for (
						const ShaderParameter &
							parameter :
						parameters) {
						if (
							parameter.name == name) {
							currentColor = parameter.colorValue;

							break;
						}
					}

					QColor current(
						static_cast<int>(
							currentColor.r * 255.0f),
						static_cast<int>(
							currentColor.g * 255.0f),
						static_cast<int>(
							currentColor.b * 255.0f),
						static_cast<int>(
							currentColor.a * 255.0f));

					QColor color = QColorDialog::getColor(
						current,
						this,
						"Choose Color",
						QColorDialog::
							ShowAlphaChannel);

					if (!color.isValid())
						return;

					glm::vec4 newColor(
						color.redF(),
						color.greenF(),
						color.blueF(),
						color.alphaF());

					effect->SetColorParameter(
						name,
						newColor);
				});
		}

		qDebug() << "REBUILD 9";
	}
}
