#pragma once

#include "ofMain.h"
#include "tool.h"

#include <Eigen/Dense>
#include <Eigen/Sparse>

class Editor;

class PoissonStampTool : public Tool {

public:
	explicit PoissonStampTool(Editor * editor);

	void MousePressed(int x, int y) override;
	void MouseDragged(int x, int y) override;
	void MouseReleased(int x, int y) override;

	void Update();
	void Draw();

	bool StartStamp();
	void CancelStamp();

	bool IsActive() const;

private:
	Editor * editor;

	bool active = false;

	// Position of the top-left corner of the stamp
	glm::vec2 previewPosition;

	// Copy of the selected pixels
	ofImage selectedRegion;

	// Which pixels in selectedRegion actually belong
	// to the original selection.
	std::vector<bool> cloneMask;

	int selectionWidth = 0;
	int selectionHeight = 0;

	bool CaptureSelection();

	void PlaceStamp(int x, int y);

	// ---------------------------------------------
	// Poisson solver
	// ---------------------------------------------

	std::vector<int> indexMap;

	std::vector<double> pixelsR;
	std::vector<double> pixelsG;
	std::vector<double> pixelsB;

	using Triplet = Eigen::Triplet<double>;

	std::vector<Triplet> triplets;
	Eigen::SparseMatrix<double> A;

	Eigen::VectorXd bR;
	Eigen::VectorXd bG;
	Eigen::VectorXd bB;

	void ComputeDivergence(const ofPixels & targetPixels, int pasteX, int pasteY);

	void SolvePoisson(ofPixels & targetPixels, int pasteX, int pasteY);
};
