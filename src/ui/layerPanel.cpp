#include "layerPanel.h"

#include "../core/editor.h"


LayerPanel::LayerPanel(Editor* editor, QWidget* parent): QWidget(parent), editor(editor) {
	layerList = new QListWidget(this);

	addButton = new QPushButton("+", this);
	deleteButton = new QPushButton("-", this);
	upButton = new QPushButton("Up", this);
	downButton = new QPushButton("Down", this);
	mergeDownButton = new QPushButton("Merge Down", this);

	brushButton = new QPushButton("Brush", this);
	brushSettingsButton = new QPushButton("...", this);
	brushSettingsButton->setFixedSize(25, 25);

	eraserButton = new QPushButton("Erase", this);
	selectButton = new QPushButton("Select", this);
	StampButton = new QPushButton("Stamp", this);

	customShaderButton = new QPushButton("Add Custom Shader", this);
	newImageButton = new QPushButton("Add New Image", this);

	saveButton = new QPushButton("Save Project", this);
	loadButton = new QPushButton("Load Project", this);

	// Layer buttons
	auto* buttonLayout = new QHBoxLayout();
	buttonLayout->addWidget(addButton);
	buttonLayout->addWidget(deleteButton);
	buttonLayout->addWidget(upButton);
	buttonLayout->addWidget(downButton);
	buttonLayout->addWidget(mergeDownButton);
	
	// Tool buttons
	auto * toolButtonLayout = new QHBoxLayout();
	toolButtonLayout->addWidget(brushButton);
	toolButtonLayout->addWidget(brushSettingsButton);
	toolButtonLayout->addWidget(eraserButton);
	toolButtonLayout->addWidget(selectButton);
	toolButtonLayout->addWidget(StampButton);

	// Add things buttons
	auto * addNewLayout = new QHBoxLayout();
	addNewLayout->addWidget(customShaderButton);
	addNewLayout->addWidget(newImageButton);

	// Serialize Buttons
	auto * serializeButtonLayout = new QHBoxLayout();
	serializeButtonLayout->addWidget(saveButton);
	serializeButtonLayout->addWidget(loadButton);

	// Layout
	auto* layout = new QVBoxLayout(this);
	layout->addWidget(layerList);
	layout->addLayout(buttonLayout);
	layout->addLayout(toolButtonLayout);
	layout->addLayout(addNewLayout);
	layout->addLayout(serializeButtonLayout);



	setLayout(layout);

	// Layers
	connect(layerList, &QListWidget::itemSelectionChanged, this, &LayerPanel::OnLayerSelected);
	connect(addButton, &QPushButton::clicked, this, &LayerPanel::OnAddLayer);
	connect(deleteButton, &QPushButton::clicked, this, &LayerPanel::OnDeleteLayer);
	connect(upButton, &QPushButton::clicked, this, &LayerPanel::OnMoveLayerUp);
	connect(downButton, &QPushButton::clicked, this, &LayerPanel::OnMoveLayerDown);
	connect(mergeDownButton, &QPushButton::clicked, this, &LayerPanel::OnMergeLayerDown);

	// Tool buttons
	connect(brushButton,&QPushButton::clicked, this, &LayerPanel::OnBrushTool);
	connect(brushSettingsButton, &QPushButton::clicked, this, &LayerPanel::OnBrushSettingsButton);

	connect(eraserButton, &QPushButton::clicked, this, &LayerPanel::OnEraserTool);
	connect(selectButton, &QPushButton::clicked, this, &LayerPanel::OnRectangleSelectTool);
	connect(StampButton, &QPushButton::clicked, this, &LayerPanel::OnStampTool);

	connect(customShaderButton, &QPushButton::clicked, this, &LayerPanel::OnShaderButton);
	connect(newImageButton, &QPushButton::clicked, this, &LayerPanel::OnImageButton);

	connect(saveButton, &QPushButton::clicked, this, &LayerPanel::OnSaveProject);
	connect(loadButton, &QPushButton::clicked, this, &LayerPanel::OnLoadProject);

	Refresh();
}


void LayerPanel::Refresh() {
	layerList->clear();

	auto layers = editor->GetLayerInfo();

	for (const LayerInfo & layer : layers) {

		auto * item = new QListWidgetItem(QString::fromUtf8(layer.name.c_str()));

		// Store the LayerID inside the item
		item->setData(Qt::UserRole, QVariant::fromValue<qulonglong>(layer.id));

		layerList->addItem(item);

		if (layer.selected) {
			layerList->setCurrentItem(item);
		}
	}
}

// Tool button handlers

void LayerPanel::OnLayerSelected() {
	auto * item = layerList->currentItem();

	if (!item)
		return;

	LayerID id = item->data(Qt::UserRole).toULongLong();

	editor->SetActiveLayer(id);
}

void LayerPanel::OnAddLayer() {
	editor->AddLayer();
}

void LayerPanel::OnDeleteLayer() {
	auto * item = layerList->currentItem();

	if (!item)
		return;

	LayerID id = item->data(Qt::UserRole).toULongLong();

	editor->RemoveLayer(id);
}

void LayerPanel::OnMoveLayerUp() {
	auto * item = layerList->currentItem();

	if (!item)
		return;

	LayerID id = item->data(Qt::UserRole).toULongLong();

	editor->MoveLayerUp(id);
}

void LayerPanel::OnMoveLayerDown() {
	auto * item = layerList->currentItem();

	if (!item)
		return;

	LayerID id = item->data(Qt::UserRole).toULongLong();

	editor->MoveLayerDown(id);
}

void LayerPanel::OnBrushTool() {
	editor->SetBrushTool();
}

void LayerPanel::OnBrushSettingsButton() {
	editor->ToggleBrushPanel();
}

void LayerPanel::OnEraserTool() {
	editor->SetBrushTool();
	editor->ToggleBrushMode();
}

void LayerPanel::OnRectangleSelectTool() {
	editor->SetRectangleSelectTool();
}

void LayerPanel::OnStampTool() {
	editor->ToggleStampTool();
}

void LayerPanel::OnMergeLayerDown() {
	auto * item = layerList->currentItem();

	if (!item)
		return;

	LayerID id = item->data(Qt::UserRole).toULongLong();

	editor->MergeDown(id);
}

void LayerPanel::OnShaderButton() {
	if (!editor)
		return;

	editor->ToggleCustomShaderPanel();
}

// Save Project
// LayerPanel.cpp
void LayerPanel::OnSaveProject() {
	QString path = QFileDialog::getSaveFileName(
		this,
		"Save PhotoMe Project",
		QString(),
		"PhotoMe Project (*.photome)");

	if (path.isEmpty())
		return;

	if (!path.endsWith(".photome", Qt::CaseInsensitive))
		path += ".photome";

	bool success = editor->SaveProject(
		path.toLocal8Bit().constData());

	if (!success)
		qDebug() << "Failed to save project.";
}

void LayerPanel::OnLoadProject() {
	QString path = QFileDialog::getOpenFileName(
		this,
		"Load PhotoMe Project",
		QString(),
		"PhotoMe Project (*.photome)");

	if (path.isEmpty())
		return;

	bool success = editor->LoadProject(
		path.toLocal8Bit().constData());

	if (!success) {
		qDebug() << "Failed to load project.";
	}
}

void LayerPanel::OnImageButton() {
	QString path = QFileDialog::getOpenFileName(
		this,
		"Add Image",
		QString(),
		"Images (*.png *.jpg *.jpeg *.bmp *.tif *.tiff)");

	if (path.isEmpty())
		return;

	bool success = editor->AddImageLayer(
		path.toLocal8Bit().constData());

	if (!success) {
		qDebug()
			<< "Failed to add image layer.";
	}
}
