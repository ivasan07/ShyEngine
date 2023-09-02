#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "Scripting/Variable.h"



#define ECSfunc_Version 1.0
//Creation time : Sat Sep  2 15:33:53 2023


typedef Scripting::Variable(*CallableFunction)(std::vector<Scripting::Variable> const&);

class FunctionManager{
public: 
	static void CreateFunctionMap(std::unordered_map<std::string, CallableFunction>& map);

	
};



Scripting::Variable Animation_ChangeAnimationPath(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Animation_AdvanceFrame(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Animation_AdvanceAnimation(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Animation_SetFrame(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Animation_SetAnimation(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Animation_GetFrame(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Animation_GetAnimation(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Animation_SetAnimationLenght(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Animation_getAnimationLenght(std::vector<Scripting::Variable>const& vec);
Scripting::Variable BoxBody_getSize(std::vector<Scripting::Variable>const& vec);
Scripting::Variable BoxBody_setSize(std::vector<Scripting::Variable>const& vec);
Scripting::Variable BoxBody_setMass(std::vector<Scripting::Variable>const& vec);
Scripting::Variable BoxBody_getArea(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Image_getTextureWidth(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Image_getTextureHeight(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Image_setSrcRect(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Image_setRotaionPoint(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Image_scaledSize(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Image_loadTexture(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Image_setFlipMode(std::vector<Scripting::Variable>const& vec);
Scripting::Variable MusicEmitter_loadMusic(std::vector<Scripting::Variable>const& vec);
Scripting::Variable MusicEmitter_play(std::vector<Scripting::Variable>const& vec);
Scripting::Variable MusicEmitter_pause(std::vector<Scripting::Variable>const& vec);
Scripting::Variable MusicEmitter_stop(std::vector<Scripting::Variable>const& vec);
Scripting::Variable MusicEmitter_resume(std::vector<Scripting::Variable>const& vec);
Scripting::Variable MusicEmitter_playWithFadeIn(std::vector<Scripting::Variable>const& vec);
Scripting::Variable MusicEmitter_fadeOut(std::vector<Scripting::Variable>const& vec);
Scripting::Variable MusicEmitter_isPlaying(std::vector<Scripting::Variable>const& vec);
Scripting::Variable MusicEmitter_isPaused(std::vector<Scripting::Variable>const& vec);
Scripting::Variable MusicEmitter_rewind(std::vector<Scripting::Variable>const& vec);
Scripting::Variable MusicEmitter_setVolume(std::vector<Scripting::Variable>const& vec);
Scripting::Variable MusicEmitter_getVolume(std::vector<Scripting::Variable>const& vec);
Scripting::Variable MusicEmitter_shouldPlayOnStart(std::vector<Scripting::Variable>const& vec);
Scripting::Variable MusicEmitter_setLoop(std::vector<Scripting::Variable>const& vec);
Scripting::Variable MusicEmitter_isOnLoop(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_GetPlacement(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_SetPositioned(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_SetStreched(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_Move(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_MoveTo(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_SetSize(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_GetPosition(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_GetSize(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_GetTop(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_GetLeft(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_GetRight(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_GetBottom(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_SetTop(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_SetLeft(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_SetRight(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_SetBottom(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_GetAnchor(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_SetAnchor(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_SetAnchorCenter(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_SetAnchorTopLeft(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_SetAnchorTopRight(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_SetAnchorBottomLeft(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_SetAnchorBottomRight(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_GetColor(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_SetColor(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_GetRenderScale(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_SetRenderScale(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_ResetRenderScale(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_SetInteractable(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Overlay_IsInteractable(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayImage_SetTexture(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayImage_GetTexture(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_GetFit(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_SetFit(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_GetVerticalAlignment(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_GetHorizontalAlignment(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_SetVerticalAlignment(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_SetHorizontalAlignment(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_GetFont(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_SetFont(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_GetText(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_SetText(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_GetPointSize(std::vector<Scripting::Variable>const& vec);
Scripting::Variable OverlayText_SetPointSize(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ParticleSystem_startEmitting(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ParticleSystem_isEmitting(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ParticleSystem_loadTexture(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ParticleSystem_addBurst(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_setTrigger(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_isTrigger(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_setFriction(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_getFriction(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_setBounciness(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_getBounciness(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_getOffSet(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_addOffSet(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_setRotationFreezed(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_isRotationFreezed(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_setBodyType(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_getBodyType(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_setLinearDrag(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_getLinearDrag(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_setAngularDrag(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_getAngularDrag(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_setGravityScale(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_getGravityScale(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_getCollisionNormal(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_getCollisionPoint(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_setCollisionLayer(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_getCollisionLayer(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_setLinearVelocity(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_getLinearVelocity(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_setAngularVelocity(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_getAngularVelocity(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_applyForce(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_applyForceToCenter(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_applyTorque(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_applyLinearImpulse(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_applyLinearImpulseToCenter(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicBody_applyAngularImpulse(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_loadSound(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_play(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_pause(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_stop(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_resume(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_playWithfadeIn(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_fadeOut(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_isPlaying(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_isPaused(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_setVolume(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_getVolume(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_shouldPlayOnStart(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_setLoop(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_isOnLoop(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_enableSpatialSound(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_isSpatialSoundEnabled(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_setPanning(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_getPanning(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_setAudibleDistance(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundEmitter_getAudibleDistance(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_GetLocalPosition(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_GetWorldPosition(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_SetLocalPosition(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_SetLocalPositionX(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_SetLocalPositionY(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_SetWorldPosition(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_GetLocalScale(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_GetWorldScale(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_SetScale(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_SetScaleX(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_SetScaleY(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_SetWorldScale(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_GetLocalRotation(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_GetWorldRotation(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_SetLocalRotation(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_SetWorldRotation(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_Translate(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_TranslateX(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_TranslateY(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_Rotate(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_Scale(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Transform_SetTransformRelativeToNewParent(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_AnyKeyPressed(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_AnyKeyReleased(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsLetterPressed(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsLetterDown(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsLetterUp(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsLetterReleased(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsNumberPressed(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsNumberDown(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsNumberUp(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsNumberReleased(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsSpecialKeyPressed(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsSpecialKeyDown(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsSpecialKeyUp(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsSpecialKeyReleased(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_HorizontalMovement(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_VerticalMovement(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_Jump(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_Action(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_HasMouseMoved(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_HasMouseWheelMoved(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsMouseButtonDown(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsMouseButtonUp(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsMouseButtonPressed(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsMouseButtonReleased(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_GetMousePosition(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_GetMouseWheelScroll(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_ConnectedControllersCount(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_AnyControllerButtonPressed(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_AnyControllerButtonReleased(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_AnyControllerAxisMotion(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_AnyControllerConnected(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_AnyControllerDisconnected(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsControllerButtonPressedWithId(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsControllerButtonDownWithId(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsControllerButtonReleasedWithId(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_GetLeftTriggerValueWithId(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_GetRightTriggerValueWithId(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsLeftTriggerDownWithId(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsRightTriggerDownWithId(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_GetLeftStickHorizontalValueWithId(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_GetLeftStickVerticalValueWithId(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_GetRightStickHorizontalValueWithId(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_GetRightStickVerticalValueWithId(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_HasLeftStickMovedWithId(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_HasRightStickMovedWithId(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsControllerButtonPressed(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsControllerButtonDown(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsControllerButtonReleased(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_GetLeftTriggerValue(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_GetRightTriggerValue(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsLeftTriggerDown(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_IsRightTriggerDown(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_GetLeftStickHorizontalValue(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_GetLeftStickVerticalValue(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_GetRightStickHorizontalValue(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_GetRightStickVerticalValue(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_HasLeftStickMoved(std::vector<Scripting::Variable>const& vec);
Scripting::Variable InputManager_HasRightStickMoved(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsManager_getGravity(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsManager_setGravity(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsManager_addCollisionLayer(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsManager_removeCollisionLayer(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsManager_setCollisionBetweenLayers(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsManager_layersCollide(std::vector<Scripting::Variable>const& vec);
Scripting::Variable PhysicsManager_layersExists(std::vector<Scripting::Variable>const& vec);
Scripting::Variable RendererManager_getWidth(std::vector<Scripting::Variable>const& vec);
Scripting::Variable RendererManager_getHeight(std::vector<Scripting::Variable>const& vec);
Scripting::Variable RendererManager_toggleFullScreen(std::vector<Scripting::Variable>const& vec);
Scripting::Variable RendererManager_showCursor(std::vector<Scripting::Variable>const& vec);
Scripting::Variable RendererManager_resizeWindow(std::vector<Scripting::Variable>const& vec);
Scripting::Variable RendererManager_renameWindow(std::vector<Scripting::Variable>const& vec);
Scripting::Variable RendererManager_repositionWindow(std::vector<Scripting::Variable>const& vec);
Scripting::Variable RendererManager_SetWindowIcon(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SaveManager_SaveAll(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SaveManager_Save(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SaveManager_Load(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SaveManager_SetSlot(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SaveManager_GetSlot(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SaveManager_Set(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SaveManager_Get(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SaveManager_ClearSlot(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SaveManager_Exists(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SaveManager_DeleteSave(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SceneManager_ChangeScene(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SceneManager_ResetScene(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SceneManager_EndGame(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SceneManager_getNumberOfScenes(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SceneManager_GetCurrentScenePath(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Print(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Entity(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Create_EntityWithTransform(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Create_EntityWithOverlay(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Destroy_Entity(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Entity_Name(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Entity_CurrentName(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Event_EntityEvent(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Event_GlobalEvent(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Graph(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Math_Add(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Math_Subtract(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Math_Multiply(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Math_Divide(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Math_Power(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Math_Root(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Math_Max(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Math_Min(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Math_PlusOne(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Math_MinusOne(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Logic_Equals(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Logic_NotEquals(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Logic_Lesser(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Logic_Greater(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Logic_LesserOrEqual(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Logic_GreaterOrEqual(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Logic_And(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Logic_Or(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Logic_Negate(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Magnitude(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_X(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Y(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Angle(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_AngleWithVector(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Create(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Normalize(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Rotate(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Add(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Subtract(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Dot(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Cross(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Multiply(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Up(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Left(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Right(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Down(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_One(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Vector2D_Zero(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Text_Equals(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Text_Concatenate(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Text_Substring(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Text_Begining(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Text_End(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Text_Trim(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Text_TrimBlanks(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Text_GetLetter(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Text_Find(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Text_ToString(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Text_LeadingZeros(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Text_RemoveDecimals(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Attribute_Set(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Attribute_Get(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Attribute_SetLocal(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Attribute_GetLocal(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Attribute_SetGlobal(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Attribute_GetGlobal(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_RealTime_Now(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_RealTime_WeekDay(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_RealTime_ShortWeekDay(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_RealTime_Month(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_RealTime_ShortMonth(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_RealTime_DayOfWeekIndex(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_RealTime_MonthIndex(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_RealTime_MonthDay(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_RealTime_Year(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_RealTime_Hours(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_RealTime_Minutes(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_RealTime_Seconds(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_RealTime_Since(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_RealTime_HourTime(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_RealTime_PreciseHourTime(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_RealTime_TimeStamp(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_RealTime_Date(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Camera_GetPosition(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Camera_SetPosition(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Camera_GetScale(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Camera_SetScale(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Random_UnitValue(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Random_NumberBetween(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Random_Between(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_DegreesTo_Radians(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_RadiansTo_Degrees(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Random_AngleBetween(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Random_UnitVector(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Random_ScaledVector(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Random_Color(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_Random_ColorBetween(std::vector<Scripting::Variable>const& vec);
Scripting::Variable ScriptFunctionality_OpenURL(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundManager_setMasterVolume(std::vector<Scripting::Variable>const& vec);
Scripting::Variable SoundManager_setChannelsCapacity(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Time_GetTimeSinceBegining(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Time_GetTimeSinceBeginingMilliseconds(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Time_GetDeltaTime(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Time_GetFrameRate(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Time_GetPhysicsDeltaTime(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Time_ScaleWithDeltaTime(std::vector<Scripting::Variable>const& vec);
Scripting::Variable Time_ScaleWithPhysicsDeltaTime(std::vector<Scripting::Variable>const& vec);
