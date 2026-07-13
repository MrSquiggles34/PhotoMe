#include "layerPanel.h"

#include "../core/editor.h"
#include "../document/document.h"
#include "../document/layer.h"

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

	Document* document = editor->GetDocument();

	for (const Layer & layer : document->GetLayers()) {

		auto* item = new QListWidgetItem(QString::fromUtf8(layer.GetName().c_str()));

		// Store the LayerID inside the item
		item->setData(Qt::UserRole, QVariant::fromValue<qulonglong>(layer.GetID()));

		layerList->addItem(item);

		if (layer.GetID() == document->GetActiveLayerID()) {
			layerList->setCurrentItem(item);
		}
	}
}

void LayerPanel::OnLayerSelected() {
	auto * item = layerList->currentItem();

	if (!item)
		return;

	LayerID id = item->data(Qt::UserRole).toULongLong();

	editor->GetDocument()->SetActiveLayer(id);
}

void LayerPanel::OnAddLayer() {
	editor->GetDocument()->AddLayer();

	Refresh();
}

void LayerPanel::OnDeleteLayer() {
	int row = layerList->currentRow();

	if (row < 0)
		return;

	editor->GetDocument()->RemoveLayer(row);

	Refresh();
}

void LayerPanel::OnMoveLayerUp() {
	int row = layerList->currentRow();

	if (row < 0)
		return;

	editor->GetDocument()->MoveLayerUp(row);

	Refresh();
}

void LayerPanel::OnMoveLayerDown() {
	int row = layerList->currentRow();

	if (row < 0)
		return;

	editor->GetDocument()->MoveLayerDown(row);

	Refresh();
}
