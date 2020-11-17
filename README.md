# twitchOnAir
A python / arduino combo that checks whether a twitch stream is online.. 

Since Twitch deprecated their old API a while ago it's been harder to do simple embedded stuff like a "stream online" sign, or whatnot. Here's a fix.

## Python script

Handles authorisation and token stuff, since there's no nice ways to do it in arduino... 
packages required:

```
requests time twitchAPI socket
```
twitchAPI requires a client_id and client_secret.

Get your own from [twitch](https://dev.twitch.tv/console/extensions/). Read more about authorising with the twitch api [here](https://dev.twitch.tv/docs/authentication).

## Arduino code

Runs on an ESP8266 (wemos D1 mini, actually), connects to the python script on $host through $tcpPort
and transmits $streamer, then waits a while for a response which is either "0\n" or "1\n" indicating whether the stream is live.

libraries required:

```
'ESP8266 framework for arduino' and https://github.com/tzapu/WiFiManager
```
