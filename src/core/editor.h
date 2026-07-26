#pragma once

#include <memory>
#include <string>
#include <vector>

#include "layerInfo.h"

class Document;
class Renderer;
class CommandManager;
class LayerPanel;
class Camera2D;

class Editor {
	public:
		Editor();
		~Editor();

		void Setup();
		void Update();
		void Draw();


		void SetCameraPosition();
		void SetCameraZoom();

		bool AddImageLayer(const std::string & path);
		void AddLayer();
		void RemoveLayer(LayerID id);
		void MoveLayerUp(LayerID id);
		void MoveLayerDown(LayerID id);
		void SetActiveLayer(LayerID id);
		void RefreshLayerPanel();

		Document* GetDocument();
		Renderer* GetRenderer();
		CommandManager* GetCommandManager();
		Camera2D * GetCamera();

		std::vector<LayerInfo> GetLayerInfo() const;
		

	private:
		std::unique_ptr<Document> document;
		std::unique_ptr<Renderer> renderer;
		std::unique_ptr<CommandManager> commandManager;
		std::unique_ptr<Camera2D> camera;
		std::unique_ptr<LayerPanel> layerPanel;

};
