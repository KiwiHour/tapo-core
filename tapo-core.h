#pragma once

#include "tplink-core.h"
#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>

struct BulbAttributes
{
	bool powerState;
	int brightness;
};

struct ColorBulbAttributes : BulbAttributes
{
	int colorTemperature;
	int hue;
	int saturation;
};

class GenericDevice
{
private:
	TPLinkCore core;
	int cookieAge_ms = 0; // cookie age in milliseconds

	// Throws runtime error if handshake fails
	void handshake();

public:
	string ipAddress;

	GenericDevice(string _ipAddress)
		: ipAddress(_ipAddress)
	{
		core.deviceIP = _ipAddress;
		handshake();
	}

	/*
	 * VITAL!!!!
	 *
	 * This method MUST be called suitably consistently (the duration between calls depends on your project)
	 * Trying to control the Tapo device with an inaccurate cookie age *could* lead to a failure in encryption (and therefore the device won't accept your input)
	 * So its a good idea to ensure you increment the cookie age before sending anything to the device, especially for 24/7 runtime projects
	 *
	 * This method also automatically executes a core handshake after the cookie age has gone beyond the threshold
	 * @param milliseconds How many ms have gone past since the last increment call
	 * @returns Whether or not a core handshake was executed or not
	 */
	bool incrementCookieAge(int milliseconds);

	int turnOn();
	int turnOff();
	bool isOn();

	nlohmann::json getDeviceUsage();

protected:
	// Returns the error code
	int setDeviceInfo(nlohmann::json params);
	nlohmann::json getDeviceInfo();
	nlohmann::json sendRequest(string method, nlohmann::json params = "{}"); // Defaults to no parameters
};

class LightBulb : public GenericDevice
{
public:
	using GenericDevice::GenericDevice;

	int setBrightness(int brightness);
	int getBrightness();
	BulbAttributes getAttributes();
};

class ColorLightBulb : public LightBulb
{
public:
	using LightBulb::LightBulb;

	int setColorTemperature(int temperature_k);
	int setHue(int hue);
	int setSaturation(int saturation);
	int getColorTemperature();
	int getHue();
	int getSaturation();

	ColorBulbAttributes getAttributes();
};

// Give names to each for clarity

// Plugs (generic at the momement)
class P100 : public GenericDevice
{
	using GenericDevice::GenericDevice;
};

class P105 : public GenericDevice
{
	using GenericDevice::GenericDevice;
};

class P110 : public GenericDevice
{
	using GenericDevice::GenericDevice;
};

class P115 : public GenericDevice
{
	using GenericDevice::GenericDevice;
};

// Regular light bulbs
class L510 : public LightBulb
{
	using LightBulb::LightBulb;
};

class L520 : public LightBulb
{
	using LightBulb::LightBulb;
};

class L610 : public LightBulb
{
	using LightBulb::LightBulb;
};

// Coloured light bulbs
class L530 : public ColorLightBulb
{
	using ColorLightBulb::ColorLightBulb;
};

class L535 : public ColorLightBulb
{
	using ColorLightBulb::ColorLightBulb;
};

class L630 : public ColorLightBulb
{
	using ColorLightBulb::ColorLightBulb;
};

class L900 : public ColorLightBulb
{
	using ColorLightBulb::ColorLightBulb;
};

class L920 : public ColorLightBulb
{
	using ColorLightBulb::ColorLightBulb;
};

class L930 : public ColorLightBulb
{
	using ColorLightBulb::ColorLightBulb;
};