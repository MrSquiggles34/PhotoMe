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
	eraserButton = new QPushButton("Erase", this);
	selectButton = new QPushButton("Select", this);
	StampButton = new QPushButton("Stamp", this);

	customShaderButton = new QPushButton("Custom Shader", this);

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
	toolButtonLayout->addWidget(eraserButton);
	toolButtonLayout->addWidget(selectButton);
	toolButtonLayout->addWidget(StampButton);

	// Layout
	auto* layout = new QVBoxLayout(this);
	layout->addWidget(layerList);
	layout->addLayout(buttonLayout);
	layout->addLayout(toolButtonLayout);
	layout->addWidget(customShaderButton);

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
	connect(eraserButton, &QPushButton::clicked, this, &LayerPanel::OnEraserTool);
	connect(selectButton, &QPushButton::clicked, this, &LayerPanel::OnRectangleSelectTool);
	connect(StampButton, &QPushButton::clicked, this, &LayerPanel::OnStampTool);

	connect(customShaderButton, &QPushButton::clicked, this, &LayerPanel::OnShaderButton);

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
