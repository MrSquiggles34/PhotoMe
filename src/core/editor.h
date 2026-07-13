#pragma once

#include <memory>
#include <string>

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

		bool LoadImageLayer(const std::string& path);

		Document* GetDocument();
		Renderer* GetRenderer();
		CommandManager* GetCommandManager();
		Camera2D * GetCamera();



	private:
		std::unique_ptr<Document> document;
		std::unique_ptr<Renderer> renderer;
		std::unique_ptr<CommandManager> commands;
		std::unique_ptr<Camera2D> camera;
		std::unique_ptr<LayerPanel> layerPanel;
};
