#include "commandManager.h"
#include "command.h"

CommandManager::CommandManager() = default;
CommandManager::~CommandManager() = default;

void CommandManager::Execute(std::unique_ptr<Command> command) {
	if (!command)
		return;

	command->Execute();

	undoStack.push_back(std::move(command));

	// Once a new command is executed,
	// the redo history is no longer valid.
	redoStack.clear();
}

void CommandManager::Record(std::unique_ptr<Command> command) {
    if (!command)
        return;

    undoStack.push_back(std::move(command));
    redoStack.clear();
}

void CommandManager::Undo() {
	if (undoStack.empty())
		return;

	std::unique_ptr<Command> command = std::move(undoStack.back());
	undoStack.pop_back();

	command->Undo();

	redoStack.push_back(std::move(command));
}

void CommandManager::Redo() {
	if (redoStack.empty())
		return;

	std::unique_ptr<Command> command = std::move(redoStack.back());
	redoStack.pop_back();

	command->Execute();

	undoStack.push_back(std::move(command));
}
