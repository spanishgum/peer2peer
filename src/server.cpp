#include "../hdr/node.h"

static pthread_mutex_t service_lock = PTHREAD_MUTEX_INITIALIZER;

Node::Server::Server(int _port) {
	// testing
	this->test = 10;

	this->num_threads = NUM_THREADS;
	this->port = _port;
	this->cli_sockfds = 0;
	
	// Instantiate a socket
	// - (domain = AF_INET) (IPv4 Internet protocols)
	// - (type = SOCK_STREAM)
	// - (protocol = 0) (default)
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	// assign IP interface and port
	struct sockaddr_in *saddr = &this->addr;
	bzero((void*)saddr, sizeof(struct sockaddr_in));
	saddr->sin_family = AF_INET;
	saddr->sin_port = htons(this->port);
	saddr->sin_addr.s_addr = htonl(INADDR_ANY);
	
	// bind the socket to our internet address
	bind(this->sockfd, (struct sockaddr *)saddr, sizeof(struct sockaddr_in));
	
	// use a backlog queue length of 5
	listen(this->sockfd, 5);
	
}

Node::Server::~Server() {


}

string Node::Server::get_message(int cli_sockfd) {
	string str("");
	char buf[MAX_BUF];
	memset(&buf[0], 0, sizeof(buf));
	if (read(cli_sockfd, buf, sizeof(buf)) > 0)
		str += buf;
	return str;
}

int Node::Server::send_message(int cli_sockfd, string s) {
	char buf[MAX_BUF];
	int n;
	memset(&buf[0], 0, sizeof(buf));
	sprintf(buf, "%s", s.c_str());
	n = write(cli_sockfd, buf, sizeof(buf));
	return n;
}

Node::Server::thread_arg *Node::Server::get_thread_arg() {
	thread_arg *ta = new thread_arg;
	ta->server = this;
	ta->cli_sockfd = this->cli_sockfds++;
	return ta;
}

void *(Node::Server::serve)(void *arg) {
	struct sockaddr_in cli_addr;
	socklen_t sock_len;
	thread_arg *ta = (thread_arg *)arg;
	Server *S = ta->server;
	int cli_sockfd = ta->cli_sockfd;
	
	for (;;) {
		sock_len = sizeof(cli_addr);
		pthread_mutex_lock(&service_lock);
		if ((cli_sockfd = accept(S->sockfd, 
			(struct sockaddr *) &cli_addr, &sock_len)) < 0)
			fatal("accept: ");
		pthread_mutex_unlock(&service_lock);
		cout << "remote client\n"
			<< "\tIP   == " << inet_ntoa(cli_addr.sin_addr)
			<< "\n\tport == " << ntohs(cli_addr.sin_port) << "\n\n";
		
		while (1) {
			cout << "message from cli_sockfd: " << cli_sockfd 
				<< ":\n\t";
			string s = S->get_message(cli_sockfd);
			if (s.length()) {
				cout << s;
				if (S->send_message(cli_sockfd, s) < 0) {
					close(cli_sockfd);
					break;
				}
			}
			else {
				cout << "No message\n";
				close(cli_sockfd);
				break;
			}			
		}
	}
	delete (thread_arg *)arg;
	pthread_exit(0);
}

void Node::Server::init(Node & N) {
	cout << "Initializing server" 
		<< "\n\tport: " << this->port
		<< "\n\tthreads: " << this->num_threads
		<< endl;
	
	this->tid = new pthread_t[this->num_threads + 1];
	for (int i = 0; i < this->num_threads; ++i)
		pthread_create(&this->tid[i], NULL, 
			&Server::serve, (void *)this->get_thread_arg());
	for (int i = 0; i < this->num_threads; ++i)
		pthread_join(this->tid[i], NULL);
}

int Node::Server::pong() {
	
	
	
	return 0;
}

int Node::Server::query_hit() {
	
	
	return 0;
}



