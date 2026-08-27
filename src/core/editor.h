#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ofMain.h"

#include "layerInfo.h"
#include "../document/brushPoint.h"
#include "../document/selection.h"
#include "../tools/brushMode.h"

class Document;
class Renderer;
class CommandManager;
class LayerPanel;
class Camera2D;
class ToolManager;
class Command;
class BrushStrokeCommand;
class Compositor;
class ColorMixerPanel;

class Editor {
	public:
		Editor();
		~Editor();

		void Setup();
		void Update();
		void Draw();


		void SetCameraPosition();
		void SetCameraZoom();

		void SetBrushTool();
		void SetRectangleSelectTool();
		void ToggleStampTool();

		void ExecuteCommand(std::unique_ptr<Command> command);
		void RecordCommand(std::unique_ptr<Command> command);

		bool AddImageLayer(const std::string & path);
		void AddLayer();
		void RemoveLayer(LayerID id);
		void MoveLayerUp(LayerID id);
		void MoveLayerDown(LayerID id);
		void SetActiveLayer(LayerID id);
		void RefreshLayerPanel();

		void PaintPoint(const ofVec2f & point, float radius, const ofColor & color, BrushMode mode);
		void BeginBrushStroke(const ofVec2f & point, float radius, const ofColor & color, BrushMode mode);
		void ContinueBrushStroke(const ofVec2f & point, float radius, const ofColor & color, BrushMode mode);
		void FinishBrushStroke();

		void ToggleBrushMode();

		void ClearSelection();

		void AddColorMixerEffect();

		Document* GetDocument();
		Renderer* GetRenderer();
		CommandManager* GetCommandManager();
		Camera2D * GetCamera();
		ToolManager * GetToolManager();
		Selection * GetSelection();
		Compositor * GetCompositor();

		std::vector<LayerInfo> GetLayerInfo() const;

		void MousePressed(int x, int y);
		void MouseDragged(int x, int y);
		void MouseReleased(int x, int y);

	private:
		std::unique_ptr<Document> document;
		std::unique_ptr<Renderer> renderer;
		std::unique_ptr<CommandManager> commandManager;
		std::unique_ptr<Camera2D> camera;
		std::unique_ptr<LayerPanel> layerPanel;
		std::unique_ptr<ToolManager> toolManager;
		std::unique_ptr<BrushStrokeCommand> activeBrushStroke;
		std::unique_ptr<Selection> selection;
		std::unique_ptr<Compositor> compositor;
		std::unique_ptr<ColorMixerPanel> colorMixerPanel;

};
