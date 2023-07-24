#include "ScriptFunctionality.h"
#include <math.h>
#include "Entity.h"
#include <Script.h>
#include "StringTrim.h"
#include <format>
#include "Scene.h"
#include "ConsoleManager.h"
#include "ScriptManager.h"
#include <sstream>
#include <iomanip>

#include <ctime>
#include <RendererManager.h>
#include <SceneManager.h>

#define DELTA 0.001f


#define TIME_DIFF 1680833948

Scripting::ScriptFunctionality::~ScriptFunctionality()
{
}

Scripting::ScriptFunctionality::ScriptFunctionality()
{
}


ECS::Entity* Scripting::ScriptFunctionality::Entity()
{
	return Scripting::ScriptManager::instance()->GetCurrentScript()->getEntity();
}

std::string Scripting::ScriptFunctionality::Entity_Name(ECS::Entity* ent)
{
	return ent->getEntityName();
}

std::string Scripting::ScriptFunctionality::Entity_CurrentName()
{
	return Entity()->getEntityName();
}

void Scripting::ScriptFunctionality::Entity_Event(ECS::Entity* ent, cstring name) {
	ent->Event(name);
}

void Scripting::ScriptFunctionality::GlobalEvent(cstring name) {

	ECS::SceneManager::instance()->getCurrentScene()->Event(name);
}

std::string Scripting::ScriptFunctionality::Script()
{
	return Scripting::ScriptManager::instance()->GetCurrentScript()->GetName();
}

void Scripting::ScriptFunctionality::Print(cVariable val)
{
	std::string objectName = Scripting::ScriptManager::instance()->GetCurrentScript()->getEntity()->getEntityName();

	std::string scriptName = Scripting::ScriptManager::instance()->GetCurrentScript()->GetName();

	Console::Output::Print(objectName + ": " + scriptName, String_ToString(val));
}

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
	return powf(a , 1.0f / b);
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

bool Scripting::ScriptFunctionality::String_Equals(cstring a, cstring b)
{
	return a == b;
}

std::string Scripting::ScriptFunctionality::String_Concatenate(cstring a, cstring b)
{
	return a + b;
}

std::string Scripting::ScriptFunctionality::String_Substring(cstring a, int begin, int end)
{
	return a.substr(begin, end - begin);
}

std::string Scripting::ScriptFunctionality::String_Begining(cstring a, int b) {
	return a.substr(0, b);
}
std::string Scripting::ScriptFunctionality::String_End(cstring a, int b) {
	return a.substr(a.size() - b);
}
std::string Scripting::ScriptFunctionality::String_Trim(cstring a, cstring values) {
	return Utilities::trim(a, values);
}
std::string Scripting::ScriptFunctionality::String_TrimBlanks(cstring a) {
	return Utilities::trim(a);

}
char Scripting::ScriptFunctionality::String_GetLetter(cstring a, int b) {
	return a[b];
}
int Scripting::ScriptFunctionality::String_Find(cstring a, char c) {
	return a.find(c);
}

std::string Scripting::ScriptFunctionality::String_LeadingZeros(int number, int leadingZeros) {

	std::stringstream ss;
	ss << std::setw(leadingZeros) << std::setfill('0') << number;
	return ss.str();
}


std::string Scripting::ScriptFunctionality::String_RemoveDecimals(float number) {

	return std::to_string((int)number);
}

std::string Scripting::ScriptFunctionality::String_ToString(cVariable variable) {

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
		return variable.value.entity->getEntityName();
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

int Scripting::ScriptFunctionality::Time_Now()
{
	return std::time(0) - TIME_DIFF;
}

std::string Scripting::ScriptFunctionality::Time_WeekDay(int time)
{
	switch (Time_DayOfWeekIndex(time) - 1) {

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


std::string Scripting::ScriptFunctionality::Time_ShortWeekDay(int time)
{
	switch (Time_DayOfWeekIndex(time) - 1) {

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


std::string Scripting::ScriptFunctionality::Time_Month(int time)
{
	switch (Time_MonthIndex(time) - 1) {

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


std::string Scripting::ScriptFunctionality::Time_ShortMonth(int time)
{
	switch (Time_MonthIndex(time) - 1) {

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


int Scripting::ScriptFunctionality::Time_DayOfWeekIndex(int time)
{
	const std::time_t t(GetRealTime(time));
	std::tm now;
	localtime_s(&now, &t);

	return now.tm_wday + 1;
}

int Scripting::ScriptFunctionality::Time_MonthIndex(int time)
{
	const std::time_t t(GetRealTime(time));
	std::tm now;
	localtime_s(&now, &t);

	return now.tm_mon + 1;
}


int Scripting::ScriptFunctionality::Time_MonthDay(int time)
{
	const std::time_t t(GetRealTime(time));
	std::tm now;
	localtime_s(&now, &t);

	return now.tm_mday;
}


int Scripting::ScriptFunctionality::Time_Year(int time)
{
	const std::time_t t(GetRealTime(time));
	std::tm now;
	localtime_s(&now, &t);

	return now.tm_year + 1900;
}

int Scripting::ScriptFunctionality::Time_Hours(int time)
{
	const std::time_t t(GetRealTime(time));
	std::tm now;
	localtime_s(&now, &t);

	return now.tm_hour;
}


int Scripting::ScriptFunctionality::Time_Minutes(int time)
{
	const std::time_t t(GetRealTime(time));
	std::tm now;
	localtime_s(&now, &t);

	return now.tm_min;
}

int Scripting::ScriptFunctionality::Time_Seconds(int time)
{
	const std::time_t t(GetRealTime(time));
	std::tm now;
	localtime_s(&now, &t);

	return now.tm_sec;
}

std::string Scripting::ScriptFunctionality::Time_TimeHHMM(int time)
{
	const std::time_t t(GetRealTime(time));
	std::tm now;
	localtime_s(&now, &t);

	return
		String_LeadingZeros(now.tm_hour, 2) + ":" +
		String_LeadingZeros(now.tm_min, 2);
}


std::string Scripting::ScriptFunctionality::Time_TimeHHMMSS(int time)
{
	const std::time_t t(GetRealTime(time));
	std::tm now;
	localtime_s(&now, &t);

	return
		String_LeadingZeros(now.tm_hour, 2) + ":" +
		String_LeadingZeros(now.tm_min, 2) + ":" +
		String_LeadingZeros(now.tm_sec, 2);
}

std::string Scripting::ScriptFunctionality::Time_TimeStamp(int time)
{
	//Monday
	//Wed Apr  5 01:50 : 49 2023
	return std::format("{} {} {} {} {} ",
		Time_ShortWeekDay(time),
		Time_ShortMonth(time),
		Time_MonthDay(time),
		Time_TimeHHMM(time),
		Time_Year(time)
		);
}



std::string Scripting::ScriptFunctionality::Time_DDMMYY(int time)
{
	//Monday
	//Wed Apr  5 01:50 : 49 2023
	return std::format("{}/{}/{} ",
		Time_MonthDay(time),
		Time_MonthIndex(time),
		Time_Year(time)
	);
}

int Scripting::ScriptFunctionality::GetRealTime(int time) {

	return time + TIME_DIFF;
}


std::string Scripting::ScriptFunctionality::Time_Since(int time, int now)
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
		return Time_WeekDay(time);
	}

	if (Time_Year(time) != Time_Year(now)) {
		return Time_DDMMYY(time);
	}

	return std::format("{}-{}", Time_MonthDay(time), Time_ShortMonth(time));
}


Utilities::Vector2D Scripting::ScriptFunctionality::Camera_GetPosition()
{
	return Renderer::RendererManager::instance()->CameraPosition();
}

void Scripting::ScriptFunctionality::Camera_SetPosition(cVector2D newPosition)
{
	Renderer::RendererManager::instance()->SetCameraPosition(newPosition);
	ECS::SceneManager::instance()->SetCameraPosition(newPosition);
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


#include <Windows.h>
#include <shellapi.h>


void Scripting::ScriptFunctionality::Open_URL(cstring url) {

	ShellExecuteA(0, 0, url.c_str(), 0, 0, SW_SHOW);
}