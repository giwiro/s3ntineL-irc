# s3ntineL-irc
```
        _____       __  _            __         _               _                ___       _.--.
   ____|__  /____  / /_(_)___  ___  / /        (_)_________     \`.|\..----...-'`   `-._.-'_.-'`
  / ___//_ </ __ \/ __/ / __ \/ _ \/ /  ______/ / ___/ ___/     /  ' `         ,       __.--'
 (__  )__/ / / / / /_/ / / / /  __/ /__/_____/ / /  / /__       )/' _/     \   `-_,   /
/____/____/_/ /_/\__/_/_/ /_/\___/_____/    /_/_/   \___/       `-'" `"\_  ,_.-;_.-\_ ',
                                                                    _.-'_./   {_.'   ; /
                                                                   {_.-``-'         {_/
```
Multiplatform, post-exploitation tool to maintain the continuous access to a computer
using irc as communication bus and capable of spawning a remote shell.

This software keeps an open socket to IRC, so it can receive instructions at all time and
automatically generates a nickname and use the device user as username.

In order to receive a valid command, it has to be sent in this format: `.COMMAND arg` \
All available commands are listed in the `command_handler_proxy` function at the `irc.cpp` file.
Some commands are only available if the message was sent as a private message.


## Compile

### Unix
Since this project uses Makefile, just execute:

```shell script
$ make IRC_CHANNEL=somecustomircchannel all
```

It's important you set up the `IRC_CHANNEL` variable in order to prevent unauthorized access to the
IRC channel.

### Windows
Just open the project with Visual Studio and build it as a regular project. \
BUT !!!!!!!!!! Make sure you change the `IRC_CHANNEL` variable for your project in order to prevent unauthorized access to the
IRC channel.
You can change the value here (change the key `IRC_CHANNEL`):

`Project > s3ntineL-irc Properties > Configuration Properties > C/C++ > Preprocessor > Preprocessor Definitions`


## Legal Disclamer

```
The author does not hold any responsibility for the bad use of this tool,
remember that attacking targets without prior consent is illegal and punished by law.
```
