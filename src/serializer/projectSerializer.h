#pragma once

#include <string>

class Document;

class ProjectSerializer {
public:
	static bool Save(const Document & document, const std::string & path);

	static bool Load(Document & document, const std::string & path);
};

