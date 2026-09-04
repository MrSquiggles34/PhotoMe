#include "poissonStampTool.h"

#include "../core/editor.h"
#include "../document/document.h"
#include "../document/layer.h"
#include "../document/selection.h"
#include "../rendering/camera2D.h"

PoissonStampTool::PoissonStampTool(Editor * editor) : Tool(editor), editor(editor) {
}

bool PoissonStampTool::StartStamp() {

	Selection * selection = editor->GetSelection();

	if (!selection || !selection->IsActive())
		return false;

	Layer * layer = editor->GetDocument()->GetActiveLayer();

	if (!layer)
		return false;

	const ofFbo & layerFbo = layer->GetFbo();

	const int left = selection->GetLeft();
	const int top = selection->GetTop();
	const int right = selection->GetRight();
	const int bottom = selection->GetBottom();

	selectionWidth = right - left;
	selectionHeight = bottom - top;

	if (selectionWidth <= 0 || selectionHeight <= 0)
		return false;

	// ---------------------------------------------
	// Read the layer into an image
	// ---------------------------------------------

	ofPixels pixels;

	layerFbo.readToPixels(pixels);

	ofImage source;
	source.setFromPixels(pixels);

	// ---------------------------------------------
	// Allocate selected region
	// ---------------------------------------------

	selectedRegion.allocate(selectionWidth, selectionHeight, OF_IMAGE_COLOR_ALPHA);

	cloneMask.assign(
		selectionWidth * selectionHeight,
		false);

	const ofFbo & selectionFbo = selection->GetMask();

	ofPixels selectionPixels;
	selectionFbo.readToPixels(selectionPixels);

	// ---------------------------------------------
	// Copy selected pixels
	// ---------------------------------------------

	for (int y = 0; y < selectionHeight; ++y) {

		for (int x = 0; x < selectionWidth; ++x) {

			int sourceX = left + x;
			int sourceY = top + y;

			if (sourceX < 0 || sourceX >= source.getWidth() || sourceY < 0 || sourceY >= source.getHeight()) {

				continue;
			}

			selectedRegion.setColor(x, y, source.getColor(sourceX, sourceY));

			// Selection mask is RGBA8.
			ofColor maskColor = selectionPixels.getColor(sourceX, sourceY);

			if (maskColor.a > 0) {
				cloneMask[y * selectionWidth + x] = true;
			}
		}
	}

	selectedRegion.update();

	active = true;

	previewPosition = glm::vec2(left, top);

	return true;
}

void PoissonStampTool::CancelStamp() {

	active = false;

	selectedRegion.clear();
	cloneMask.clear();

	selectionWidth = 0;
	selectionHeight = 0;

	previewPosition = glm::vec2(0);
}

bool PoissonStampTool::IsActive() const {
	return active;
}

void PoissonStampTool::Update() {

	if (!active)
		return;

	glm::vec2 mouse(ofGetMouseX(), ofGetMouseY());

	previewPosition = editor->GetCamera()->ScreenToWorld(mouse);
}

void PoissonStampTool::Draw() {

	if (!active || !selectedRegion.isAllocated())
		return;

	ofPushStyle();

	ofSetColor(255, 180);

	selectedRegion.draw(previewPosition.x, previewPosition.y);

	ofNoFill();

	ofSetColor(0, 255, 0, 180);

	ofDrawRectangle(previewPosition.x, previewPosition.y, selectionWidth, selectionHeight);

	ofFill();

	ofPopStyle();
}

void PoissonStampTool::MousePressed(int x, int y) {

	if (!active)
		return;

	glm::vec2 world = editor->GetCamera()->ScreenToWorld(glm::vec2(x, y));

	PlaceStamp(static_cast<int>(world.x), static_cast<int>(world.y));
}

void PoissonStampTool::MouseDragged(int x, int y) {
}

void PoissonStampTool::MouseReleased(int x, int y) {
}

void PoissonStampTool::PlaceStamp(int x, int y) {

	Layer * layer = editor->GetDocument()->GetActiveLayer();

	if (!layer)
		return;

	ofFbo & layerFbo = layer->GetFbo();

	// ---------------------------------------------
	// Read current layer into pixels
	// ---------------------------------------------

	ofPixels targetPixels;

	layerFbo.readToPixels(targetPixels);

	// ---------------------------------------------
	// Compute Poisson divergence
	// ---------------------------------------------

	ComputeDivergence(
		targetPixels,
		x,
		y);

	if (pixelsR.empty()) {
		ofLogNotice() << "Poisson stamp contains no pixels.";
		return;
	}

	// ---------------------------------------------
	// Solve Poisson equation
	// ---------------------------------------------

	SolvePoisson(
		targetPixels,
		x,
		y);

	// ---------------------------------------------
	// Put pixels back into the layer FBO
	// ---------------------------------------------

	layerFbo.begin();

	ofClear(0, 0, 0, 0);

	ofImage result;
	result.setFromPixels(targetPixels);

	result.draw(0, 0);

	layerFbo.end();

	// ---------------------------------------------
	// Stamp is finished
	// ---------------------------------------------

	ofLogNotice()
		<< "Poisson stamp placed at "
		<< x << ", " << y;

	CancelStamp();

	// Return to brush
	editor->SetBrushTool();
}

void PoissonStampTool::ComputeDivergence(
	const ofPixels & targetPixels,
	int pasteX,
	int pasteY) {

	int targetWidth = targetPixels.getWidth();
	int targetHeight = targetPixels.getHeight();

	// ---------------------------------------------
	// Give every masked target pixel an index
	// ---------------------------------------------

	indexMap.assign(
		targetWidth * targetHeight,
		-1);

	int maskedPixels = 0;

	for (int y = 0; y < selectionHeight; ++y) {

		for (int x = 0; x < selectionWidth; ++x) {

			if (!cloneMask[y * selectionWidth + x])
				continue;

			int tx = pasteX + x;
			int ty = pasteY + y;

			if (tx < 0 || tx >= targetWidth || ty < 0 || ty >= targetHeight)
				continue;

			indexMap[ty * targetWidth + tx] = maskedPixels++;
		}
	}

	// ---------------------------------------------
	// Allocate divergence vectors
	// ---------------------------------------------

	pixelsR.assign(maskedPixels, 0.0);
	pixelsG.assign(maskedPixels, 0.0);
	pixelsB.assign(maskedPixels, 0.0);

	// ---------------------------------------------
	// Compute divergence
	// ---------------------------------------------

	for (int y = 0; y < selectionHeight; ++y) {

		for (int x = 0; x < selectionWidth; ++x) {

			if (!cloneMask[y * selectionWidth + x])
				continue;

			int tx = pasteX + x;
			int ty = pasteY + y;

			if (tx < 0 || tx >= targetWidth || ty < 0 || ty >= targetHeight)
				continue;

			int i = indexMap[ty * targetWidth + tx];

			if (i == -1)
				continue;

			ofColor center = selectedRegion.getColor(x, y);

			int leftX = std::max(x - 1, 0);
			int rightX = std::min(x + 1, selectionWidth - 1);
			int upY = std::max(y - 1, 0);
			int downY = std::min(y + 1, selectionHeight - 1);

			ofColor left = selectedRegion.getColor(leftX, y);

			ofColor right = selectedRegion.getColor(rightX, y);

			ofColor up = selectedRegion.getColor(x, upY);

			ofColor down = selectedRegion.getColor(x, downY);

			pixelsR[i] = 4.0 * center.r - left.r - right.r - up.r - down.r;

			pixelsG[i] = 4.0 * center.g - left.g - right.g - up.g - down.g;

			pixelsB[i] = 4.0 * center.b - left.b - right.b - up.b - down.b;
		}
	}
}

void PoissonStampTool::SolvePoisson(
	ofPixels & targetPixels,
	int pasteX,
	int pasteY) {

	int targetWidth = targetPixels.getWidth();
	int targetHeight = targetPixels.getHeight();

	int N = pixelsR.size();

	if (N == 0)
		return;

	// ---------------------------------------------
	// Build RHS
	// ---------------------------------------------

	bR.resize(N);
	bG.resize(N);
	bB.resize(N);

	for (int i = 0; i < N; ++i) {

		bR[i] = pixelsR[i];
		bG[i] = pixelsG[i];
		bB[i] = pixelsB[i];
	}

	// ---------------------------------------------
	// Build sparse matrix
	// ---------------------------------------------

	triplets.clear();

	for (int y = 0; y < selectionHeight; ++y) {

		for (int x = 0; x < selectionWidth; ++x) {

			if (!cloneMask[y * selectionWidth + x])
				continue;

			int tx = pasteX + x;
			int ty = pasteY + y;

			if (tx < 0 || tx >= targetWidth || ty < 0 || ty >= targetHeight)
				continue;

			int i = indexMap[ty * targetWidth + tx];

			if (i == -1)
				continue;

			int count = 0;

			const int neighbors[4][2] = {
				{ tx - 1, ty },
				{ tx + 1, ty },
				{ tx, ty - 1 },
				{ tx, ty + 1 }
			};

			for (const auto & neighbor : neighbors) {

				int nx = neighbor[0];
				int ny = neighbor[1];

				if (nx < 0 || nx >= targetWidth || ny < 0 || ny >= targetHeight)
					continue;

				int j = indexMap[ny * targetWidth + nx];

				if (j != -1) {

					// Neighbor is inside the
					// Poisson region.
					triplets.emplace_back(
						i,
						j,
						-1.0);

					count++;

				} else {

					// Neighbor is outside the
					// region, so use the
					// target boundary color.

					ofColor boundary = targetPixels.getColor(nx, ny);

					bR[i] += boundary.r;
					bG[i] += boundary.g;
					bB[i] += boundary.b;

					count++;
				}
			}

			triplets.emplace_back(
				i,
				i,
				static_cast<double>(count));
		}
	}

	A.resize(N, N);

	A.setFromTriplets(
		triplets.begin(),
		triplets.end());

	// ---------------------------------------------
	// Solve
	// ---------------------------------------------

	Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;

	solver.analyzePattern(A);
	solver.factorize(A);

	if (solver.info() != Eigen::Success) {

		ofLogError()
			<< "Poisson factorization failed.";

		return;
	}

	Eigen::VectorXd xR = solver.solve(bR);

	Eigen::VectorXd xG = solver.solve(bG);

	Eigen::VectorXd xB = solver.solve(bB);

	if (solver.info() != Eigen::Success) {

		ofLogError()
			<< "Poisson solve failed.";

		return;
	}

	// ---------------------------------------------
	// Write solution back to target
	// ---------------------------------------------

	for (int y = 0; y < selectionHeight; ++y) {

		for (int x = 0; x < selectionWidth; ++x) {

			if (!cloneMask[y * selectionWidth + x])
				continue;

			int tx = pasteX + x;
			int ty = pasteY + y;

			if (tx < 0 || tx >= targetWidth || ty < 0 || ty >= targetHeight)
				continue;

			int i = indexMap[ty * targetWidth + tx];

			if (i == -1)
				continue;

			ofColor result;

			result.r = glm::clamp(
				static_cast<int>(std::round(xR[i])),
				0,
				255);

			result.g = glm::clamp(
				static_cast<int>(std::round(xG[i])),
				0,
				255);

			result.b = glm::clamp(
				static_cast<int>(std::round(xB[i])),
				0,
				255);

			result.a = 255;

			targetPixels.setColor(
				tx,
				ty,
				result);
		}
	}
}
