#include "tapo-core.h"

// ######################
// ### Generic device ###
// ######################

void GenericDevice::handshake()
{
	int handshakeResult = core.handshake();
	if (handshakeResult > 0)
	{
		throw std::runtime_error("Failed to shake hands with the device, error code: " + to_string(handshakeResult));
	}
}

bool GenericDevice::incrementCookieAge(int milliseconds)
{
	cookieAge_ms += milliseconds;

	// If the cookie age reaches past 95% of the max cookie duration, execute another handshake for a new cookie
	if (cookieAge_ms > (float)(core.cookieTimeout_s * 1000) * 0.95)
	{
		handshake();
		return true;
	}

	return false;
}

int GenericDevice::turnOn()
{
	nlohmann::json params;
	params["device_on"] = true;
	return setDeviceInfo(params);
}

int GenericDevice::turnOff()
{
	nlohmann::json params;
	params["device_on"] = false;
	return setDeviceInfo(params);
}

bool GenericDevice::isOn()
{
	return getDeviceInfo()["device_on"];
}

nlohmann::json GenericDevice::getDeviceUsage()
{
	return sendRequest("get_device_usage")["result"];
}

nlohmann::json GenericDevice::getDeviceInfo()
{
	return sendRequest("get_device_info")["result"];
}

// Returns the error code
int GenericDevice::setDeviceInfo(nlohmann::json params)
{
	return sendRequest("set_device_info", params)["error_code"];
}

nlohmann::json GenericDevice::sendRequest(string method, nlohmann::json params = "{}")
{
	nlohmann::json request;
	request["method"] = method;
	request["params"] = params;

	nlohmann::json response = nlohmann::json::parse(core.sendRequest(request.dump()));

	return response;
}

// ##################
// ### Light bulb ###
// ##################

int LightBulb::setBrightness(int brightness)
{
	nlohmann::json params;
	params["brightness"] = brightness;
	return setDeviceInfo(params);
}

int LightBulb::getBrightness()
{
	return getDeviceInfo()["brightness"];
}

BulbAttributes LightBulb::getAttributes()
{
	BulbAttributes attributes;
	nlohmann::json deviceInfo = getDeviceInfo();
	attributes.powerState = deviceInfo["device_on"];
	attributes.brightness = deviceInfo["brightness"];
	return attributes;
}

// ########################
// ### Color light bulb ###
// ########################

int ColorLightBulb::setColorTemperature(int temperature_k)
{
	nlohmann::json params;
	params["color_temp"] = temperature_k;
	return setDeviceInfo(params);
}

int ColorLightBulb::setHue(int hue)
{
	nlohmann::json params;
	params["color_temp"] = 0;
	params["hue"] = hue;
	return setDeviceInfo(params);
}

int ColorLightBulb::setSaturation(int saturation)
{
	nlohmann::json params;
	params["color_temp"] = 0;
	params["saturation"] = saturation;
	return setDeviceInfo(params);
}

int ColorLightBulb::getColorTemperature()
{
	return getDeviceInfo()["color_temp"];
}

int ColorLightBulb::getHue()
{
	return getDeviceInfo()["hue"];
}

int ColorLightBulb::getSaturation()
{
	return getDeviceInfo()["saturation"];
}

ColorBulbAttributes ColorLightBulb::getAttributes()
{
	ColorBulbAttributes attributes;
	nlohmann::json deviceInfo = getDeviceInfo();

	attributes.powerState = deviceInfo["device_on"];
	attributes.brightness = deviceInfo["brightness"];
	attributes.colorTemperature = deviceInfo["color_temp"];
	attributes.hue = deviceInfo["hue"];
	attributes.saturation = deviceInfo["saturation"];

	return attributes;
}