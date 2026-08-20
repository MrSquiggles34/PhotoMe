#pragma once

#include <memory>
#include <vector>

class Command;

class CommandManager {
public:

	CommandManager();
	~CommandManager();

	CommandManager(const CommandManager &) = delete;
	CommandManager& operator=(const CommandManager &) = delete;

	void Execute(std::unique_ptr<Command> command);
	void Record(std::unique_ptr<Command> command);

	void Undo();
	void Redo();

private:
	std::vector<std::unique_ptr<Command>> undoStack;
	std::vector<std::unique_ptr<Command>> redoStack;
};
