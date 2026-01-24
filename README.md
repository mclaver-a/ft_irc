# 42'S FT_IRC

_This project has been created as part of the 42 curriculum by dmarijan & mclaver-._

## **Description**

<p align="justify">
Ft_irc aims to teach us the basic standard protocol of the "Internet Relay Chat", by creating a Server daemon written in c++98.
The program will allow us to touch on sockets, connections, and non-blocking operations.
It's an interesting project to understand where modern chat-based communication apps originate from, as well as how server to client communication is usually handled.
Our code is completely subject compliant (no forks, only one poll() call, multiple client handling, includes a README.md...).
</p>

## **Instructions**

To compile this project, simply clone it onto your machine, navigate into the folder and run make.

Other Makefile commands provided:
  - Make (all): compile the program into the ./ircserv binary, as well as create the Obj/ folder.
  - Make clean: delete the Obj/ folder, as well as it's contents.
  - Make fclean: same as clean, plus remove ./ircserv binary.
  - Make re: execute Make fclean + Make all.

To run the daemon, execute the binary as follows:

  `./ircserv <server_port> <server_password>`

To connect to the server via nc:

  `./nc -C 127.0.0.1 <server_port>` 
  
The -C flag is necessary to ensure proper message parsing, `man nc` for more info.

To connect to the server via HexChat:
  - Add the localhost IP and port (127.0.0.1/<chosen_port>) to your network list.
  - Make sure to tick the "Accept invalid SSL certificates" box.
  - Set login method as /PASS from the dropdown list.
  - Fill in the remaining fields accordingly.

## **Functionality**

<p align="justify">
Our reference client for this project is HexChat, a FOSS IRC client with built-in GUI.
  Although our server is capable of holding connections with any other client (namely nc), expect best functionality when using HexChat. 

  Note-worthy HexChat behaviour: 
</p>

  - Commands must be preceded by a "/" character. Any string input without such character will be interpreted as a chat message instead (calling /PRIVMSG automatically).
  - File transfer is automatically handled Client to Client. (This is technically Bonus behaviour)
  - Automatically authenticates through PASS, NICK and USER, although the input variables must be specified through HexChat's GUI.
  - Automatically reconnects to the server, as well as periodically sends /WHO requests.
  - Username must be comprised of valid characters (alpha-numeric, underscore, dash and dot only), Realname must be printable characters only.

### **Command List**

Following is a list of implemented commands. All commands are case-insensitive.
  - CAP: lists the server's special features (none in our case).
  - INVITE <#channel_name> <target_nick>: invites the specified user to the specified channel. If +i is set, an invite is required to join the channel.
  - JOIN <#channel_name>: attempts to join the specified channel. If specified channel name is nonexistant, will attempt to create it (requires ChanOP).
  - KICK <target_name> <#channel_name>: kicks the specified user from specified channel (requires ChanOP).
  - LIST: replies to the client with the current list of existing channels.
  - MODE <[+/-][itkol]> <misc_arguments>: changes the specified channel's properties:
      - [+/-] i: set or unset invite-only channel restriction.
      - [+/-] t: set/remove the restrictions of the TOPIC command to channel operators.
      - [+/-] k: set/remove the channel key (password).
      - [+/-] o: give/take channel operator privilege.
      - [+/-] l: set/remove the user limit to channel.
  - NICK <desired_nickname>: attempts to set specified string as user's nickname.
  - NOTICE <[target_user/target_channel]> <message>: sends a messages that won't trigger automatic responses.
  - OPER <target_user> <OP_password>: attempts to use inputted OP_password to register as a Server Administrator.
  - PART <channel_name> <parting_message>: leaves specified channel, parting message is optional.
  - PASS <server_password>: required to correctly authenticate to the server. If the inputted password differs from the argument given to the binary, the server will cut the connection to the client.
  - PRIVMSG <[target_user/channel_name]> <message_text>: sends a chat message to the specified user or channel.
  - QUIT <quit_message>: Leaves the server entirely, quit message is optional.
  - TOPIC <#channel_name> <new_topic>: attempts to change the channel's currently set topic. If +t is set, ChanOP is required.
  - USER <user_name> <unused_arg> <unused_arg> <real_name>: authenticates to the server and sets user and real names.
  - UNOPER <nick_name>: attempts to remove OP from specified user, requires OP.
  - WHO <channel_name>: server replies with the info of each client currently present in the specified channel.

### **Technical Choices**

We think the following functionalities are important to keep in mind when tackling this project:
  - Half-messages, interrupted by Ctrl+D while using nc, are saved in a buffer and are correctly patched back together as long as **no other** Ctrl-D signal is sent consecutively. Using more than one EOF signal in a row will result in the socket being one-sidedly closed, as per how interactive shells work. The server daemon has no way to detect this and as such, will **not close** the connection to the client until the nc process is killed entirely (via Ctrl-C or similar).
  - A suspended nc process (Ctrl+Z) will not get disconnected from the server. To revive a suspended process, simply execute "fg".
  
## **Resources**

### **AI Usage**

<p align="justify">
No AI generated code has been used during the development of this project. 
AI has only been employed to better understand certain quirks of the IRC standard.
</p>

### **References**

The following sources were consulted when designing our IRC server:
  - https://www.mirc.com/help/html/index.html?basic_irc_commands.html
  - https://datatracker.ietf.org/doc/html/rfc1459#section-6
  - https://modern.ircdocs.horse/#numerics
