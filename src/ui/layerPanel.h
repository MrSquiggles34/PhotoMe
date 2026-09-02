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
	void OnMergeLayerDown();

	void OnBrushTool();
	void OnEraserTool();
	void OnRectangleSelectTool();
	void OnStampTool();

private:
	Editor* editor = nullptr;

	QListWidget* layerList = nullptr;

	QPushButton* addButton = nullptr;
	QPushButton* deleteButton = nullptr;
	QPushButton* upButton = nullptr;
	QPushButton* downButton = nullptr;

	QPushButton * brushButton = nullptr;
	QPushButton * eraserButton = nullptr;
	QPushButton * selectButton = nullptr;
	QPushButton * StampButton = nullptr;
	QPushButton * mergeDownButton = nullptr;
};
