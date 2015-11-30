#ifndef _NODE_H
#define _NODE_H

#include <map>
#include <list>
#include <vector>
#include <string>
// #include <utility>

#include <fstream>
#include <sstream>
#include <iostream>

#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PEER_LIMIT 1000

#define PORT_MIN 1000
#define PORT_MAX 65535

#define MAX_BUF 512
#define NUM_THREADS 40
#define SERVER_PORT 15000

#define sstream stringstream

using namespace std;

void fatal(string s);


static const string
	GNU_CONN = "GNUTELLA CONNECT/0.4\r\n",
	GNU_OK = "GNUTELLA OK\r\n";

typedef union {
	int 
		message_id:16,
		protocol_id:1,
		ttl:1,
		hops:1,
		payload_len:4;
} msg_hdr;
	
class Node {
	class Client;
	class Server;
	
	private:
		
		int 
			neighborPort, 
			filePort, 
			numberOfPeers, 
			TTL;
			
		bool 
			isSeedNode;
			
		string 
			seedNodes, 
			localFilesDirectory, 
			localFiles,
			err_msg;
			
		vector<pair<string, int>> seeds;
		vector<pair<string, list<string>>> catalogue;
		
	public:
		// node acts as both 
		//  client and server
		
		Client *client;
		Server *server;
	
		// -- CONSTRUCTION ---------------------------------
		Node();
		~Node();
		
		// -- GETTERS --------------------------------------
		int get_neighborPort();
		int get_filePort();
		int get_numberOfPeers();
		int get_TTL();
		string get_seedNodes();
		bool get_isSeedNode();
		string get_localFilesDirectory();
		string get_localFiles();
		
		// -- SETTERS --------------------------------------
		bool set_neighborPort(int);
		bool set_filePort(int);
		bool set_numberOfPeers(int);
		bool set_TTL(int);
		bool set_seedNodes(string);
		bool set_isSeedNode(bool);
		bool set_localFilesDirectory(string);
		bool set_localFiles(string);
		
		// -- READERS --------------------------------------
		int read_seeds();
		int read_catalogue();
		
		// -- BEGIN ----------------------------------------
		void init(int);
		char *generate_hdr(int, int);
		
		// -- TESTING --------------------------------------
		void show_conf();
};


class Node::Client {
	private:
		int port;
	public:
		Client(int);
		~Client();
		void init(Node &);
		int ping();
		int query();
};


class Node::Server {

	typedef struct {
		Server *server;
		int cli_sockfd;
	} thread_arg;

	private:
		int sockfd;
		int cli_sockfds;
		
		struct sockaddr_in addr;
		int port;
		int num_threads;
		
		pthread_t *tid;
		int *tid_ptr;
		string response;
		
	public:
		int test;
		// -- CONSTRUCTION ---------------------------------
		Server(int);
		~Server();
		
		
		string get_message(int cli_sockfd);
		int send_message(int cli_sockfd, string);
		thread_arg *get_thread_arg();
		
		static void *serve(void *arg);
		void init(Node &);
		int pong();
		int query_hit();
};





#endif