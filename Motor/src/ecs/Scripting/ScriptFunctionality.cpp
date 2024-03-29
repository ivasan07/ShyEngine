#include "ScriptFunctionality.h"
#include "RendererManager.h"
#include "ConsoleManager.h"
#include "ScriptManager.h"
#include "PrefabManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "StringTrim.h"
#include "Entity.h"
#include "Script.h"
#include "Scene.h"
#include "Random.h"

#include <sstream>
#include <iomanip>
#include <math.h>
#include <format>
#include <ctime>
#include <random>

#define DELTA 0.001f
#define TIME_DIFF 1680833948
#define PI 3.14159265f


Scripting::ScriptFunctionality::~ScriptFunctionality() {}

Scripting::ScriptFunctionality::ScriptFunctionality()
{
	std::srand(std::time(NULL));
}

void Scripting::ScriptFunctionality::Print(cVariable val)
{
	std::string objectName = Scripting::ScriptManager::instance()->GetCurrentScript()->getEntity()->getEntityName();

	std::string scriptName = Scripting::ScriptManager::instance()->GetCurrentScript()->GetName();

	Console::Output::Print(objectName + ": " + scriptName, Text_ToString(val));
}



// ------------------------------ Entity -----------------------------------

ECS::Entity* Scripting::ScriptFunctionality::Entity()
{
	return Scripting::ScriptManager::instance()->GetCurrentScript()->getEntity();
}

void Scripting::ScriptFunctionality::Create_EntityWithTransform(cstring entName) {

	ECS::PrefabManager::instance()->InstantiatePrefabWithTransform(entName, ECS::SceneManager::instance()->getCurrentScene());
}

void Scripting::ScriptFunctionality::Create_EntityWithOverlay(cstring entName) {

	ECS::PrefabManager::instance()->InstantiatePrefabWithOverlay(entName, ECS::SceneManager::instance()->getCurrentScene());
}

void Scripting::ScriptFunctionality::Destroy_Entity(ECS::Entity* ent) {

	ent->removeEntity();
}

std::string Scripting::ScriptFunctionality::Entity_Name(ECS::Entity* ent)
{
	return ent->getEntityName();
}

std::string Scripting::ScriptFunctionality::Entity_CurrentName()
{
	return Entity()->getEntityName();
}

void Scripting::ScriptFunctionality::Event_EntityEvent(ECS::Entity* ent, cstring name) {
	ent->Event(name);
}

void Scripting::ScriptFunctionality::Event_GlobalEvent(cstring name) {

	ECS::SceneManager::instance()->getCurrentScene()->Event(name);
}

std::string Scripting::ScriptFunctionality::Graph()
{
	return Scripting::ScriptManager::instance()->GetCurrentScript()->GetName();
}




// ------------------------------ Math -----------------------------------

float Scripting::ScriptFunctionality::Math_Add(float a, float b)
{
	return a + b;
}

float Scripting::ScriptFunctionality::Math_Subtract(float a, float b)
{
	return a - b;
}

float Scripting::ScriptFunctionality::Math_Multiply(float a, float b)
{
	return a * b;
}

float Scripting::ScriptFunctionality::Math_Divide(float a, float b)
{
	return a / b;
}

float Scripting::ScriptFunctionality::Math_Power(float a, float b)
{
	return powf(a, b);
}

float Scripting::ScriptFunctionality::Math_Root(float a, float b)
{
	return powf(a, 1.0f / b);
}

float Scripting::ScriptFunctionality::Math_Max(float a, float b)
{
	return std::max(a, b);
}

float Scripting::ScriptFunctionality::Math_Min(float a, float b)
{
	return std::min(a, b);
}

float Scripting::ScriptFunctionality::Math_PlusOne(float a)
{
	return a + 1;
}

float Scripting::ScriptFunctionality::Math_MinusOne(float a)
{
	return a - 1;
}

float Scripting::ScriptFunctionality::Math_Lerp(float a, float b, float t)
{
	return std::lerp(a, b, t);
}

float Scripting::ScriptFunctionality::Math_CubicLerp(float a, float b, float t)
{
	float t_ = 1 - t;
	return Math_Lerp(a, b, 1 - t_ * t_ * t_);
}

float Scripting::ScriptFunctionality::Math_Abs(float f)
{
	return std::abs(f);
}





// ------------------------------ Logic -----------------------------------

bool Scripting::ScriptFunctionality::Logic_Equals(float a, float b)
{
	return abs(a - b) <= DELTA;
}

bool Scripting::ScriptFunctionality::Logic_NotEquals(float a, float b)
{
	return abs(a - b) > DELTA;
}

bool Scripting::ScriptFunctionality::Logic_Lesser(float a, float b)
{
	return a < b;
}

bool Scripting::ScriptFunctionality::Logic_Greater(float a, float b)
{
	return a > b;
}

bool Scripting::ScriptFunctionality::Logic_LesserOrEqual(float a, float b)
{
	if (Logic_Equals(a, b))
		return true;

	return Logic_Lesser(a, b);
}

bool Scripting::ScriptFunctionality::Logic_GreaterOrEqual(float a, float b)
{
	if (Logic_Equals(a, b))
		return true;

	return Logic_Greater(a, b);
}

bool Scripting::ScriptFunctionality::Logic_And(bool a, bool b)
{
	return a && b;
}

bool Scripting::ScriptFunctionality::Logic_Or(bool a, bool b)
{
	return a || b;
}

bool Scripting::ScriptFunctionality::Logic_Negate(bool b)
{
	return !b;
}




// ------------------------------ Vector2D -----------------------------------

Vector2D Scripting::ScriptFunctionality::Vector2D_Create(float x, float y)
{
	return Vector2D(x, y);
}

float Scripting::ScriptFunctionality::Vector2D_Magnitude(cVector2D vec)
{
	return vec.magnitude();
}

float Scripting::ScriptFunctionality::Vector2D_X(cVector2D vec)
{
	return vec.getX();
}

float Scripting::ScriptFunctionality::Vector2D_Y(cVector2D vec)
{
	return vec.getY();
}

Vector2D Scripting::ScriptFunctionality::Vector2D_Normalize(cVector2D vec)
{
	return Vector2D(vec).normalize();
}

float Scripting::ScriptFunctionality::Vector2D_Angle(cVector2D vec)
{
	Vector2D norm(vec);
	norm.normalize();

	return norm.angle(Vector2D(1, 0));
}

float Scripting::ScriptFunctionality::Vector2D_AngleWithVector(cVector2D vec, cVector2D other)
{
	return vec.angle(other);
}

Vector2D Scripting::ScriptFunctionality::Vector2D_Rotate(cVector2D vec, float angle)
{
	return vec.rotate(angle);
}

Vector2D Scripting::ScriptFunctionality::Vector2D_Add(cVector2D a, cVector2D b)
{
	return a + b;
}

Vector2D Scripting::ScriptFunctionality::Vector2D_Subtract(cVector2D a, cVector2D b)
{
	return a - b;
}

Vector2D Scripting::ScriptFunctionality::Vector2D_Dot(cVector2D a, cVector2D b)
{
	//TODO: Dot product
	Console::Output::PrintWarning("Not implemented", "Vector2D_Dot operation is not implemented yet");
	return Vector2D();
}

Vector2D Scripting::ScriptFunctionality::Vector2D_Cross(cVector2D a, cVector2D b)
{
	//TODO: Dot product
	Console::Output::PrintWarning("Not implemented", "Vector2D_Cross operation is not implemented yet");
	return Vector2D();
}

Vector2D Scripting::ScriptFunctionality::Vector2D_Multiply(cVector2D a, float b)
{
	return a * b;
}


Vector2D Scripting::ScriptFunctionality::Vector2D_Lerp(cVector2D a, cVector2D b, float t) {

	return Vector2D(Math_Lerp(a.x_, b.x_, t), Math_Lerp(a.y_, b.y_, t));
}

Vector2D Scripting::ScriptFunctionality::Vector2D_CubicLerp(cVector2D a, cVector2D b, float t) {

	return Vector2D(Math_CubicLerp(a.x_, b.x_, t), Math_CubicLerp(a.y_, b.y_, t));
}

Vector2D Scripting::ScriptFunctionality::Vector2D_Up()
{
	return Vector2D(0, 1);
}

Vector2D Scripting::ScriptFunctionality::Vector2D_Down()
{
	return Vector2D(0, -1);
}

Vector2D Scripting::ScriptFunctionality::Vector2D_Left()
{
	return Vector2D(-1, 0);
}

Vector2D Scripting::ScriptFunctionality::Vector2D_Right()
{
	return Vector2D(1, 0);
}

Vector2D Scripting::ScriptFunctionality::Vector2D_Zero()
{
	return Vector2D(0, 0);
}

Vector2D Scripting::ScriptFunctionality::Vector2D_One()
{
	return Vector2D(1, 1);
}





// ------------------------------ Text -----------------------------------

bool Scripting::ScriptFunctionality::Text_Equals(cstring a, cstring b)
{
	return strcmp(a.c_str(), b.c_str()) == 0;
}

std::string Scripting::ScriptFunctionality::Text_Concatenate(cstring a, cstring b)
{
	return a + b;
}

std::string Scripting::ScriptFunctionality::Text_Substring(cstring a, int begin, int end)
{
	return a.substr(begin, end - begin);
}

std::string Scripting::ScriptFunctionality::Text_Begining(cstring a, int b) {
	return a.substr(0, b);
}
std::string Scripting::ScriptFunctionality::Text_End(cstring a, int b) {
	return a.substr(a.size() - b);
}
std::string Scripting::ScriptFunctionality::Text_Trim(cstring a, cstring values) {
	return Utilities::trim(a, values);
}
std::string Scripting::ScriptFunctionality::Text_TrimBlanks(cstring a) {
	return Utilities::trim(a);

}
char Scripting::ScriptFunctionality::Text_GetLetter(cstring a, int b) {
	return a[b];
}
int Scripting::ScriptFunctionality::Text_Find(cstring a, char c) {
	return a.find(c);
}

std::string Scripting::ScriptFunctionality::Text_LeadingZeros(int number, int leadingZeros) {

	std::stringstream ss;
	ss << std::setw(leadingZeros) << std::setfill('0') << number;
	return ss.str();
}


std::string Scripting::ScriptFunctionality::Text_RemoveDecimals(float number) {

	return std::to_string((int)number);
}

std::string Scripting::ScriptFunctionality::Text_ToString(cVariable variable) {

	switch (variable.type)
	{
	case Scripting::Variable::Type::Bool:
		return variable.value.Bool ? "Yes" : "No";
	case Scripting::Variable::Type::Char:
		return std::to_string(variable.value.Char);
	case Scripting::Variable::Type::Float:
		return std::to_string(variable.value.Float);
	case Scripting::Variable::Type::String:
		return variable.str;
	case Scripting::Variable::Type::Entity:

		if (variable.value.entity != nullptr)
			return variable.value.entity->getEntityName();
		return "-Entity not found-";
	case Scripting::Variable::Type::Color:
		return variable.value.color;
	case Scripting::Variable::Type::Vector2D:
		return variable.vector;
	case Scripting::Variable::Type::Null:
		return "Empty";
	default:
		return "Conversion to string not defined to this variable type";
		break;
	}
}




// ------------------------------ Attributes -----------------------------------

void Scripting::ScriptFunctionality::Attribute_Set(cstring name, cVariable val)
{
	Scripting::ScriptManager::instance()->GetCurrentScript()->Set(name, val);
}

Scripting::Variable Scripting::ScriptFunctionality::Attribute_Get(cstring name)
{
	return Scripting::ScriptManager::instance()->GetCurrentScript()->Get(name);
}

void Scripting::ScriptFunctionality::Attribute_SetLocal(cstring name, cVariable val)
{
	Entity()->SetAttribute(name, val);
}

Scripting::Variable Scripting::ScriptFunctionality::Attribute_GetLocal(cstring name)
{
	return Entity()->GetAttribute(name);
}

void Scripting::ScriptFunctionality::Attribute_SetGlobal(cstring name, cVariable val)
{
	ScriptManager::instance()->SetGlobal(name, val);
}

Scripting::Variable Scripting::ScriptFunctionality::Attribute_GetGlobal(cstring name)
{
	return ScriptManager::instance()->GetGlobal(name);
}




// ------------------------------ Time -----------------------------------

int Scripting::ScriptFunctionality::RealTime_Now()
{
	return std::time(0) - TIME_DIFF;
}

std::string Scripting::ScriptFunctionality::RealTime_WeekDay(int time)
{
	switch (RealTime_DayOfWeekIndex(time) - 1) {

	case 0:
		return "Sunday";
	case 1:
		return "Monday";
	case 2:
		return "Tuesday";
	case 3:
		return "Wednesday";
	case 4:
		return "Thursday";
	case 5:
		return "Friday";
	case 6:
		return "Saturday";
	default:
		return ":)";
	}
}

std::string Scripting::ScriptFunctionality::RealTime_ShortWeekDay(int time)
{
	switch (RealTime_DayOfWeekIndex(time) - 1) {

	case 0:
		return "Sun";
	case 1:
		return "Mon";
	case 2:
		return "Tues";
	case 3:
		return "Wed";
	case 4:
		return "Thurs";
	case 5:
		return "Fri";
	case 6:
		return "Sat";
	default:
		return ":)";
	}
}

std::string Scripting::ScriptFunctionality::RealTime_Month(int time)
{
	switch (RealTime_MonthIndex(time) - 1) {

	case 0:
		return "January";
	case 1:
		return "February";
	case 2:
		return "March";
	case 3:
		return "April";
	case 4:
		return "May";
	case 5:
		return "June";
	case 6:
		return "July";
	case 7:
		return "August";
	case 8:
		return "September";
	case 9:
		return "October";
	case 10:
		return "November";
	case 11:
		return "December";
	default:
		return ":)";
	}
}

std::string Scripting::ScriptFunctionality::RealTime_ShortMonth(int time)
{
	switch (RealTime_MonthIndex(time) - 1) {

	case 0:
		return "Jan";
	case 1:
		return "Feb";
	case 2:
		return "Mar";
	case 3:
		return "Apr";
	case 4:
		return "May";
	case 5:
		return "Jun";
	case 6:
		return "Jul";
	case 7:
		return "Aug";
	case 8:
		return "Sept";
	case 9:
		return "Oct";
	case 10:
		return "Nov";
	case 11:
		return "Dec";
	default:
		return ":)";
	}
}

int Scripting::ScriptFunctionality::RealTime_DayOfWeekIndex(int time)
{
	const std::time_t t(GetRealTime(time));
	std::tm now;
	localtime_s(&now, &t);

	return now.tm_wday + 1;
}

int Scripting::ScriptFunctionality::RealTime_MonthIndex(int time)
{
	const std::time_t t(GetRealTime(time));
	std::tm now;
	localtime_s(&now, &t);

	return now.tm_mon + 1;
}

int Scripting::ScriptFunctionality::RealTime_MonthDay(int time)
{
	const std::time_t t(GetRealTime(time));
	std::tm now;
	localtime_s(&now, &t);

	return now.tm_mday;
}

int Scripting::ScriptFunctionality::RealTime_Year(int time)
{
	const std::time_t t(GetRealTime(time));
	std::tm now;
	localtime_s(&now, &t);

	return now.tm_year + 1900;
}

int Scripting::ScriptFunctionality::RealTime_Hours(int time)
{
	const std::time_t t(GetRealTime(time));
	std::tm now;
	localtime_s(&now, &t);

	return now.tm_hour;
}

int Scripting::ScriptFunctionality::RealTime_Minutes(int time)
{
	const std::time_t t(GetRealTime(time));
	std::tm now;
	localtime_s(&now, &t);

	return now.tm_min;
}

int Scripting::ScriptFunctionality::RealTime_Seconds(int time)
{
	const std::time_t t(GetRealTime(time));
	std::tm now;
	localtime_s(&now, &t);

	return now.tm_sec;
}

std::string Scripting::ScriptFunctionality::RealTime_HourTime(int time)
{
	const std::time_t t(GetRealTime(time));
	std::tm now;
	localtime_s(&now, &t);

	return
		Text_LeadingZeros(now.tm_hour, 2) + ":" +
		Text_LeadingZeros(now.tm_min, 2);
}

std::string Scripting::ScriptFunctionality::RealTime_PreciseHourTime(int time)
{
	const std::time_t t(GetRealTime(time));
	std::tm now;
	localtime_s(&now, &t);

	return
		Text_LeadingZeros(now.tm_hour, 2) + ":" +
		Text_LeadingZeros(now.tm_min, 2) + ":" +
		Text_LeadingZeros(now.tm_sec, 2);
}

std::string Scripting::ScriptFunctionality::RealTime_TimeStamp(int time)
{
	//Monday
	//Wed Apr  5 01:50 : 49 2023
	return std::format("{} {} {} {} {} ",
		RealTime_ShortWeekDay(time),
		RealTime_ShortMonth(time),
		RealTime_MonthDay(time),
		RealTime_HourTime(time),
		RealTime_Year(time)
	);
}


std::string Scripting::ScriptFunctionality::RealTime_Date(int time)
{
	//Monday
	//Wed Apr  5 01:50 : 49 2023
	return std::format("{}/{}/{} ",
		RealTime_MonthDay(time),
		RealTime_MonthIndex(time),
		RealTime_Year(time)
	);
}

int Scripting::ScriptFunctionality::GetRealTime(int time) {

	return time + TIME_DIFF;
}

std::string Scripting::ScriptFunctionality::RealTime_Since(int time, int now)
{
	int dif = std::abs(now - time);

	if (dif < 30) {
		return std::to_string(dif) + " seconds ago";
	}

	if (dif < 60)
		return "Less than a minute ago";

	if (dif < 60 * 60)
		return std::to_string(dif / 60) + " minutes ago";

	if (dif < 60 * 60 * 24) {
		return std::to_string(dif / (60 * 24)) + " hours ago";
	}

	if (dif < 60 * 60 * 24 * 2) {
		return "Yesterday";
	}

	if (dif < 60 * 60 * 24 * 7) {
		return RealTime_WeekDay(time);
	}

	if (RealTime_Year(time) != RealTime_Year(now)) {
		return RealTime_Date(time);
	}

	return std::format("{}-{}", RealTime_MonthDay(time), RealTime_ShortMonth(time));
}





// ------------------------------ Camera -----------------------------------

Utilities::Vector2D Scripting::ScriptFunctionality::Camera_GetPosition()
{
	Vector2D vec = Renderer::RendererManager::instance()->CameraPosition();
	/*
		La posicion de la camara no funciona como una entidad sino que tiene las coordenadas invertidas
		Por eso hay que invertir la posicion x antes de pasarsela al renderer y a la escena

		La posicion y no es necesario invertirla porque el renderer invierte la posicion de las entidades

		Haciendo esto mantenemos una consistencia entre la posicion del renderizado y la posicion de las entidades
	*/
	vec.x_ *= -1;
	return vec;
}

void Scripting::ScriptFunctionality::Camera_SetPosition(cVector2D newPosition)
{
	Vector2D modifiedPosition = Vector2D(-newPosition.x_, newPosition.y_);

	/*
		La posicion de la camara no funciona como una entidad sino que tiene las coordenadas invertidas
		Por eso hay que invertir la posicion x antes de pasarsela al renderer y a la escena

		La posicion y no es necesario invertirla porque el renderer invierte la posicion de las entidades

		Haciendo esto mantenemos una consistencia entre la posicion del renderizado y la posicion de las entidades
	*/


	Renderer::RendererManager::instance()->SetCameraPosition(modifiedPosition);
	ECS::SceneManager::instance()->SetCameraPosition(modifiedPosition);
}

float Scripting::ScriptFunctionality::Camera_GetScale()
{
	return Renderer::RendererManager::instance()->CameraScale();
}

void Scripting::ScriptFunctionality::Camera_SetScale(float newScale)
{
	Renderer::RendererManager::instance()->SetCameraScale(newScale);
	ECS::SceneManager::instance()->SetCameraScale(newScale);
}




// ------------------------------ Random -----------------------------------

float Scripting::ScriptFunctionality::DegreesTo_Radians(float angle) {
	return angle * PI / 180.0f;
}

float Scripting::ScriptFunctionality::RadiansTo_Degrees(float angle)
{
	return angle * 180.0f / PI;
}

float Scripting::ScriptFunctionality::Random_NumberBetween(int min, int max)
{
	return Utilities::Random::RandomIntBetween(min, max);
}

float Scripting::ScriptFunctionality::Random_AngleBetween(float min, float max)
{
	return Utilities::Random::RandomAngleBetween(min, max);
}

Color Scripting::ScriptFunctionality::Random_Color()
{
	return Utilities::Random::RandomColor();
}

Color Scripting::ScriptFunctionality::Random_ColorBetween(cColor c1, cColor c2)
{
	return Utilities::Random::RandomColorBetween(c1, c2);
}


float Scripting::ScriptFunctionality::Random_UnitValue()
{
	return (float)std::rand() / RAND_MAX;
}

float Scripting::ScriptFunctionality::Random_Between(float a, float b)
{
	return a + Random_UnitValue() * (b - a);
}

Utilities::Vector2D Scripting::ScriptFunctionality::Random_UnitVector()
{
	return Utilities::Vector2D(Random_UnitValue(), Random_UnitValue()).normalize();
}

Utilities::Vector2D Scripting::ScriptFunctionality::Random_ScaledVector(float val)
{
	return Random_UnitVector() * val;
}

Utilities::Vector2D Scripting::ScriptFunctionality::Random_VectorBetween(cVector2D v1, cVector2D v2)
{
	return Utilities::Random::RandomVectorBetween(v1, v2);
}

Utilities::Vector2D Scripting::ScriptFunctionality::InputManager_GetMouseWorldPosition() {

	auto mousepos = Input::InputManager::instance()->GetMousePosition();

	auto width = Renderer::RendererManager::instance()->getWidth();
	auto height = Renderer::RendererManager::instance()->getHeight();

	float cameraScale = Camera_GetScale();

	mousepos.x_ -= width * 0.5f;
	mousepos.y_ -= height * 0.5f;

	mousepos.y_ *= -1;

	auto cameraPos = Camera_GetPosition();
	mousepos.x_ += cameraPos.x_;
	mousepos.y_ += cameraPos.y_;


	return mousepos;
}



#include <Windows.h>
#include <shellapi.h>

void Scripting::ScriptFunctionality::OpenURL(cstring url) {

	ShellExecuteA(0, 0, url.c_str(), 0, 0, SW_SHOW);
}

ECS::Entity* Scripting::ScriptFunctionality::Entity_Collided()
{
	return Scripting::ScriptManager::instance()->GetCollidedEntity();
}