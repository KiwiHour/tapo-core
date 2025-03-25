A C++ (and a bit of arduino) core library for tapo devices L510, L520, L610, L530, L535, L630, L900, L920, L930, P100, P105, P110 and P115.\
I only own an L530, but I am confident that the other devices listed will work as intended.

If a device you use isn't listed, there's a good chance `GenericDevice` will provide some use to you (currently only has `turnOn` and `turnOff` functionality. But, hey, better than nothing)

Feel free to submit a pull request to add a device you've tested, or add functionality to currently existing ones.

## Requirements 
- [tplink-core](https://github.com/KiwiHour/tplink-core) by... me!
- [json](https://github.com/nlohmann/json) by [nlohmann](https://github.com/nlohmann)

## How to use
Firstly, follow tplink-core's documentation and set it up as required.\
Then you can initialise a device like so (if the tplink-core handshake fails, it will throw a `runtime_error`)
```cpp
try {
  L530 l530("192.168.X.XXX");
}
catch (exception error) {
  cout << "Error creating device:" << endl;
  cout << error.what() << endl;
}
```

Then you're free to use any of the methods available to that device. Have fun!
