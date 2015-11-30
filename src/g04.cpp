#include "../hdr/g04.h"
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <errno.h>

void test(string proc, Node *thisNode) {
	cout << proc << thisNode 
		<< "\n\t" << thisNode->server->test << endl;
}


int main() {
	
	Node *thisNode = new Node();
	// pid_t client_pid, server_pid;
	
	// Node configuration / initialization
	if (configure(*thisNode) < 0) fatal("configuration");
	if (thisNode->read_seeds() < 0) fatal("seed file");
	if (thisNode->read_catalogue() < 0) fatal("catalogue");
	
	// test configuration
	thisNode->show_conf();
	
	// seed rand to avoid message ID collisions
	srand(time(NULL));
	
	int uc = 10;
	char * c = thisNode->generate_hdr(0, uc);
	cout << "---\n";
	for (int i = 0; i < 23; ++i)
		cout << (int)c[i] << endl;
	cout << "---\n";
	// printf("testing: -%s-\n", (char *) c);
	// Steps
	//
	//
	//
	//
	//
	//
	//
	//
	//
	
	
	
	
	
	
	
	
	/*
	
	// create a client operations process
	if ((client_pid = fork()) < 0) fatal("fork");
	if (client_pid == 0) {
		thisNode->init(0);
	
		// do client stuff
		exit(0);
	}
	wait(NULL);
	
	*/
	
	/*
	
	// create a server operations process
	if ((server_pid = fork()) < 0) fatal("fork");
	if (server_pid == 0) {
		thisNode->init(1);
		// do server stuff
		exit(0);
	}
	
	int ppid, status;
	if ((ppid = wait(&status)) < 0)
		fatal("wait: ");
	else {
		if (WIFSIGNALED(status) != 0)
			printf("Child process ended because of signal %d.n",
			WTERMSIG(status));
		else if (WIFEXITED(status) != 0)
			printf("Child process ended normally; status = %d.n",
			WEXITSTATUS(status));
		else
			printf("Child process did not end normally.n");
	}
	
	*/
	
	
	delete thisNode;
	return 0;
}






