#pragma once

#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class Editor;

class LayerPanel : public QWidget {

public:
	explicit LayerPanel(Editor* editor, QWidget* parent = nullptr);

	void Refresh();

private:

	void OnLayerSelected();
	void OnAddLayer();
	void OnDeleteLayer();
	void OnMoveLayerUp();
	void OnMoveLayerDown();

private:
	Editor* editor = nullptr;

	QListWidget* layerList = nullptr;

	QPushButton* addButton = nullptr;
	QPushButton* deleteButton = nullptr;
	QPushButton* upButton = nullptr;
	QPushButton* downButton = nullptr;
};
