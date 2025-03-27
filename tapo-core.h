#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include "tplink-core.cpp"

using namespace std;
using namespace nlohmann;

class GenericDevice
{
private:
	TPLinkCore core;
	int cookieAge_ms; // cookie age in milliseconds

	// Throws runtime error if handshake fails
	void handshake();

public:
	string ipAddress;

	GenericDevice(string _ipAddress);

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
	bool incrementCookieAge(int milliseconds);
	int turnOn();
	int turnOff();
	json getDeviceUsage();

protected:
	// Returns the error code
	int setDeviceInfo(json params);
	json getDeviceInfo();
	json sendRequest(string method, json params = "{}");
};

class LightBulb : public GenericDevice
{
public:
	using GenericDevice::GenericDevice;

	int setBrightness(int brightness);
	int getBrightness();
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