#include "layerPanel.h"

#include "../core/editor.h"


LayerPanel::LayerPanel(Editor* editor, QWidget* parent): QWidget(parent), editor(editor) {
	layerList = new QListWidget(this);

	addButton = new QPushButton("+", this);
	deleteButton = new QPushButton("-", this);
	upButton = new QPushButton("Up", this);
	downButton = new QPushButton("Down", this);

	auto* buttonLayout = new QHBoxLayout();
	buttonLayout->addWidget(addButton);
	buttonLayout->addWidget(deleteButton);
	buttonLayout->addWidget(upButton);
	buttonLayout->addWidget(downButton);

	auto* layout = new QVBoxLayout(this);
	layout->addWidget(layerList);
	layout->addLayout(buttonLayout);

	setLayout(layout);

	connect(layerList, &QListWidget::itemSelectionChanged, this, &LayerPanel::OnLayerSelected);
	connect(addButton, &QPushButton::clicked, this, &LayerPanel::OnAddLayer);
	connect(deleteButton, &QPushButton::clicked, this, &LayerPanel::OnDeleteLayer);
	connect(upButton, &QPushButton::clicked, this, &LayerPanel::OnMoveLayerUp);
	connect(downButton, &QPushButton::clicked, this, &LayerPanel::OnMoveLayerDown);

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
