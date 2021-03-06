#include "SceneCtrl.h"
#include "2d/CCSprite.h"
#include "WindowBox.h"
#include "EditorNode.h"
#include "Serializer.h"

Node* SceneCtrl::getUiRoot()
{
	return _rootNode;
}

bool SceneCtrl::init(float frameWidth, float frameHeight)
{
	Node::init();

	_rootNode = EditorNode::create();
	_rootNode->setContentSize(Size(400, 400));
	addChild(_rootNode);

	_boxesNode = new WindowBox(nullptr, true);
	_boxesNode->autorelease();
	_boxesNode->setGlobalZOrder(99999);
	_boxesNode->setLocalZOrder(99999);
	addChild(_boxesNode);

	onResize(frameWidth, frameHeight);

	return true;
}

void SceneCtrl::onResize(float frameWidth, float frameHeight)
{
	const Size& sz = _rootNode->getContentSize();
	_rootNode->setPosition(frameWidth / 2 - sz.width / 2, frameHeight / 2 - sz.height / 2);
	_boxesNode->Reset();
}

SceneCtrl* SceneCtrl::create(float frameWidth, float frameHeight)
{
	SceneCtrl* sc = new SceneCtrl;
	sc->autorelease();
	sc->init(frameHeight, frameHeight);
	return sc;
}

void SceneCtrl::mouseMoveEvent(float x, float y)
{


	float pixelDeltaX = x - _lastx;
	float pixelDeltaY = y - _lasty;

	if (_lastx == x && _lasty == y)
		return;

	_lastx = x;
	_lasty = y;

	if (_isMouseHoveredBox) {
		_boxesNode->updateWindowAreas(pixelDeltaX, pixelDeltaY, pixelDeltaX, pixelDeltaY);
		return;
	}

	if (_mousePressed)
	{


		if (_hoveredResizePoint != RESIZE_POINT_NONE)
		{
			// The mouse is pressed on a resize point; resize each selected window accordingly.
			switch (_hoveredResizePoint)
			{
			case RESIZE_POINT_WN:
				// Upper left
				_boxesNode->updateWindowAreas(pixelDeltaX, pixelDeltaY, 0.0f, 0.0f);
				break;
			case RESIZE_POINT_N:
				// Upper middle
				_boxesNode->updateWindowAreas(0.0f, pixelDeltaY, 0.0f, 0.0f);
				break;
			case RESIZE_POINT_NE:
				// Upper right
				_boxesNode->updateWindowAreas(0.0f, pixelDeltaY, pixelDeltaX, 0.0f);
				break;
			case RESIZE_POINT_E:
				// Middle right
				_boxesNode->updateWindowAreas(0.0f, 0.0f, pixelDeltaX, 0.0f);
				break;
			case RESIZE_POINT_ES:
				// Bottom right
				_boxesNode->updateWindowAreas(0.0f, 0.0f, pixelDeltaX, pixelDeltaY);
				break;
			case RESIZE_POINT_S:
				// Bottom middle
				_boxesNode->updateWindowAreas(0.0f, 0.0f, 0.0f, pixelDeltaY);
				break;
			case RESIZE_POINT_SW:
				// Bottom left
				_boxesNode->updateWindowAreas(pixelDeltaX, 0.0f, 0.0f, pixelDeltaY);
				break;
			case RESIZE_POINT_W:
				// Middle left
				_boxesNode->updateWindowAreas(pixelDeltaX, 0.0f, 0.0f, 0.0f);
				break;
			default:
				break;
			}
		}
	}
	else
	{
		_hoveredResizePoint = _boxesNode->getPointAtPosition(x, y);

		if (_hoveredResizePoint != -1)
		{
			//todo: update mouse status;
		}

	}
}

void SceneCtrl::mousePressEvent(float x, float y)
{

	_lasty = y;
	_lastx = x;

	if (_boxesNode->isPointInBoxRect(_lastx, _lasty))
	{
		_isMouseHoveredBox = true;
		emit selectedBox();
	}
	else {
		_mousePressed = true;
	}
}

void SceneCtrl::mouseReleaseEvent(float x, float y)
{
	_mousePressed = false;
	_isMouseHoveredBox = false;
}

SceneCtrl::SceneCtrl()
: _lastx(0)
, _lasty(0)
, _mousePressed(false)
, _isMouseHoveredBox(false)
{
	Global::sceneCtrl = this;
}

void SceneCtrl::setCurrentNode(Node* node)
{
	_boxesNode->setCurrentNode(node);
}

void SceneCtrl::registerNode(NodeInfo* info)
{
	_nodeTypeMapping[info->self] = info->typeName;
}

std::string SceneCtrl::getNodeType(Node* node)
{
	auto iter = _nodeTypeMapping.find(node);

	if (iter != _nodeTypeMapping.end())
		return iter->second;

	return "Node";
}

void SceneCtrl::setUiRoot(Node* node)
{
	_rootNode->removeFromParent();
	_rootNode = node;
	addChild(node);
	_boxesNode->setCurrentNode(nullptr);
}

