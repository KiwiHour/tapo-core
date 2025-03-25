#include <string>
#include <nlohmann/json.hpp>
#include "tplink-core.cpp"

#include <Arduino.h>

using namespace std;
using namespace nlohmann;

class GenericDevice
{
private:
	TPLinkCore core;
	int cookieAge_ms = 0; // cookie age in milliseconds

public:
	string ipAddress;

	GenericDevice(string _ipAddress)
	{
		ipAddress = _ipAddress;
		core.deviceIP = _ipAddress;

		Serial.println(core.handshake());
	}

	/*
	 * VITAL!!!!
	 *
	 * This method MUST be called suitably consistently (the duration between calls depends on your project)
	 * Trying to control the Tapo device with an inaccurate cookie age *could* lead to a failure in encryption (and therefore the device won't accept your input)
	 * So its a good idea to ensure you increment the cookie age before sending anything to the device, especially for 24/7 runtime projects
	 *
	 * This method also automatically executes a core handshake after the cookie age has gone beyond the threshold
	 *
	 * @returns Whether or not a core handshake was executed or not
	 */
	bool incrementCookieAge(int milliseconds)
	{
		cookieAge_ms += milliseconds;

		// If the cookie age reaches past 95% of the max cookie duration, execute another handshake for a new cookie
		if (cookieAge_ms > (float)(core.cookieTimeout_s * 1000) * 0.95)
		{
			core.handshake();
			return true;
		}

		return false;
	}

	int on()
	{
		json params;
		params["device_on"] = true;
		return setDeviceInfo(params);
	}

	int off()
	{
		json params;
		params["device_on"] = false;
		return setDeviceInfo(params);
	}

	json getDeviceUsage()
	{
		return sendRequest("get_device_usage")["result"];
	}

protected:
	// Returns the error code
	int setDeviceInfo(json params)
	{
		return sendRequest("set_device_info", params)["error_code"];
	}

	json sendRequest(string method, json params = "{}") // Defaults to no parameters
	{
		json request;
		request["method"] = method;
		request["params"] = params;

		json response = json::parse(core.sendRequest(request.dump()));

		return response;
	}
};

class LightBulb : public GenericDevice
{
public:
	using GenericDevice::GenericDevice;

	int setBrightness(int brightness)
	{
		json params;
		params["brightness"] = brightness;
		return setDeviceInfo(params);
	}
};

class ColorLightBulb : public LightBulb
{
	using LightBulb::LightBulb;

	int setColorTemperature(int temperature_k)
	{
		json params;
		params["color_temp"] = temperature_k;
		return setDeviceInfo(params);
	}

	int setHue(int hue)
	{
		json params;
		params["color_temp"] = 0;
		params["hue"] = hue;
		return setDeviceInfo(params);
	}

	int setSaturation(int saturation)
	{
		json params;
		params["color_temp"] = 0;
		params["saturation"] = saturation;
		return setDeviceInfo(params);
	}
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