#pragma once

#include <memory>

#include "../rendering/camera2D.h"

class Document;
class Renderer;
class CommandManager;

class Editor {
	public:
		Editor();
		~Editor();

		void Setup();
		void Update();
		void Draw();


		void SetCameraPosition();
		void SetCameraZoom();


		Document* GetDocument();
		Renderer* GetRenderer();
		CommandManager* GetCommandManager();



	private:
		std::unique_ptr<Document> document;
		std::unique_ptr<Renderer> renderer;
		std::unique_ptr<CommandManager> commands;

		Camera2D camera;
};
