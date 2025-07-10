#include "tplink-core.cpp"
#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;

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
	void handshake()
	{
		int handshakeResult = core.handshake();
		if (handshakeResult > 0)
		{
			throw std::runtime_error("Failed to shake hands with the device, error code: " + to_string(handshakeResult));
		}
	}

public:
	string ipAddress;

	GenericDevice(string _ipAddress)
	{
		ipAddress = _ipAddress;
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
	bool incrementCookieAge(int milliseconds)
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

	int turnOn()
	{
		json params;
		params["device_on"] = true;
		return setDeviceInfo(params);
	}

	int turnOff()
	{
		json params;
		params["device_on"] = false;
		return setDeviceInfo(params);
	}

	bool isOn()
	{
		return getDeviceInfo()["device_on"];
	}

	json getDeviceUsage()
	{
		return sendRequest("get_device_usage")["result"];
	}

protected:
	json getDeviceInfo()
	{
		return sendRequest("get_device_info")["result"];
	}
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

	int getBrightness()
	{
		return getDeviceInfo()["brightness"];
	}

	BulbAttributes getAttributes()
	{
		BulbAttributes attributes;
		json deviceInfo = getDeviceInfo();
		attributes.powerState = deviceInfo["device_on"];
		attributes.brightness = deviceInfo["brightness"];
		return attributes;
	}
};

class ColorLightBulb : public LightBulb
{
public:
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

	int getColorTemperature()
	{
		return getDeviceInfo()["color_temp"];
	}

	int getHue()
	{
		return getDeviceInfo()["hue"];
	}

	int getSaturation()
	{
		return getDeviceInfo()["saturation"];
	}

	ColorBulbAttributes getAttributes()
	{
		ColorBulbAttributes attributes;
		json deviceInfo = getDeviceInfo();

		attributes.powerState = deviceInfo["device_on"];
		attributes.brightness = deviceInfo["brightness"];
		attributes.colorTemperature = deviceInfo["color_temp"];
		attributes.hue = deviceInfo["hue"];
		attributes.saturation = deviceInfo["saturation"];

		return attributes;
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